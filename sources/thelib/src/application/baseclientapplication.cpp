/* 
 *  Copyright (c) 2010,
 *  Gavriloaie Eugen-Andrei (shiretu@gmail.com)
 *
 *  This file is part of crtmpserver.
 *  crtmpserver is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  crtmpserver is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with crtmpserver.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "common.h"
#include "application/baseclientapplication.h"
#include "protocols/protocolmanager.h"
#include "application/baseappprotocolhandler.h"
#include "protocols/baseprotocol.h"
#include "streaming/basestream.h"
#include "application/clientapplicationmanager.h"
#include "streaming/streamstypes.h"

uint32_t BaseClientApplication::_idGenerator = 0;

BaseClientApplication::BaseClientApplication(Variant &configuration)
: _streamsManager(this) {
	_id = ++_idGenerator;
	_configuration = configuration;
	_name = (string) configuration[CONF_APPLICATION_NAME];
	if ((VariantType) configuration[CONF_APPLICATION_ALIASES] != V_NULL) {

		FOR_MAP((configuration[CONF_APPLICATION_ALIASES]), string, Variant, i) {
			ADD_VECTOR_END(_aliases, MAP_VAL(i));
		}
	}
	_isDefault = (VariantType) configuration[CONF_APPLICATION_DEFAULT] != V_NULL ?
			(bool)configuration[CONF_APPLICATION_DEFAULT] : false;
}

BaseClientApplication::~BaseClientApplication() {

}

uint32_t BaseClientApplication::GetId() {
	return _id;
}

string BaseClientApplication::GetName() {
	return _name;
}

Variant &BaseClientApplication::GetConfiguration() {
	return _configuration;
}

vector<string> BaseClientApplication::GetAliases() {
	return _aliases;
}

bool BaseClientApplication::IsDefault() {
	return _isDefault;
}

StreamsManager *BaseClientApplication::GetStreamsManager() {
	return &_streamsManager;
}

bool BaseClientApplication::Initialize() {
	return true;
}

void BaseClientApplication::RegisterAppProtocolHandler(uint64_t protocolType,
		BaseAppProtocolHandler *pAppProtocolHandler) {
	if (MAP_HAS1(_protocolsHandlers, protocolType))
		ASSERT("Invalid protocol handler type. Already registered");
	_protocolsHandlers[protocolType] = pAppProtocolHandler;
	pAppProtocolHandler->SetApplication(this);
}

void BaseClientApplication::UnRegisterAppProtocolHandler(uint64_t protocolType) {
	if (MAP_HAS1(_protocolsHandlers, protocolType))
		_protocolsHandlers[protocolType]->SetApplication(NULL);
	_protocolsHandlers.erase(protocolType);
}

BaseAppProtocolHandler *BaseClientApplication::GetProtocolHandler(BaseProtocol *pProtocol) {
	return GetProtocolHandler(pProtocol->GetType());
}

BaseAppProtocolHandler *BaseClientApplication::GetProtocolHandler(uint64_t protocolType) {
	if (!MAP_HAS1(_protocolsHandlers, protocolType)) {
		WARN("Protocol handler not activated for protocol type %s in application %s",
				STR(tagToString(protocolType)), STR(_name));
		return NULL;
	}
	return _protocolsHandlers[protocolType];
}

BaseAppProtocolHandler *BaseClientApplication::GetProtocolHandler(string &scheme) {
	BaseAppProtocolHandler *pResult = NULL;
	if (false) {

	}
#ifdef HAS_PROTOCOL_RTMP
	else if (scheme.find("rtmp") == 0) {
		pResult = GetProtocolHandler(PT_INBOUND_RTMP);
		if (pResult == NULL)
			pResult = GetProtocolHandler(PT_OUTBOUND_RTMP);
	}
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_RTP
	else if (scheme == "rtsp") {
		pResult = GetProtocolHandler(PT_RTSP);
	}
#endif /* HAS_PROTOCOL_RTP */
#ifdef HAS_PROTOCOL_MMS
	else if (scheme == "mms") {
		pResult = GetProtocolHandler(PT_OUTBOUND_MMS);
	}
#endif /* HAS_PROTOCOL_MMS */
	else {
		WARN("scheme %s not recognized", STR(scheme));
	}
	return pResult;
}

bool BaseClientApplication::OutboundConnectionFailed(Variant &customParameters) {
	WARN("You should override BaseRTMPAppProtocolHandler::OutboundConnectionFailed");
	return false;
}

void BaseClientApplication::RegisterProtocol(BaseProtocol *pProtocol) {
	if (!MAP_HAS1(_protocolsHandlers, pProtocol->GetType()))
		ASSERT("Protocol handler not activated for protocol type %s in application %s",
			STR(tagToString(pProtocol->GetType())),
			STR(_name));
	_protocolsHandlers[pProtocol->GetType()]->RegisterProtocol(pProtocol);
}

void BaseClientApplication::UnRegisterProtocol(BaseProtocol *pProtocol) {
	if (!MAP_HAS1(_protocolsHandlers, pProtocol->GetType()))
		ASSERT("Protocol handler not activated for protocol type %s in application %s",
			STR(tagToString(pProtocol->GetType())), STR(_name));
	_streamsManager.UnRegisterStreams(pProtocol->GetId());
	_protocolsHandlers[pProtocol->GetType()]->UnRegisterProtocol(pProtocol);
	FINEST("Protocol %s unregistered from application: %s", STR(*pProtocol), STR(_name));
}

void BaseClientApplication::SignalStreamRegistered(BaseStream *pStream) {
	INFO("Stream %u of type %s with name `%s` registered to application `%s`",
			pStream->GetUniqueId(),
			STR(tagToString(pStream->GetType())),
			STR(pStream->GetName()),
			STR(_name));
}

void BaseClientApplication::SignalStreamUnRegistered(BaseStream *pStream) {
	INFO("Stream %u of type %s with name `%s` unregistered from application `%s`",
			pStream->GetUniqueId(),
			STR(tagToString(pStream->GetType())),
			STR(pStream->GetName()),
			STR(_name));
}

bool BaseClientApplication::PullExternalStreams() {
	//1. Minimal verifications
	if (_configuration["externalStreams"] == V_NULL) {
		return true;
	}

	if (_configuration["externalStreams"] != V_MAP) {
		FATAL("Invalid rtspStreams node");
		return false;
	}

	//2. Loop over the stream definitions and spawn the streams

	FOR_MAP(_configuration["externalStreams"], string, Variant, i) {
		Variant &streamConfig = MAP_VAL(i);
		if (streamConfig != V_MAP) {
			WARN("External stream configuration is invalid:\n%s",
					STR(streamConfig.ToString()));
			continue;
		}
		if (!PullExternalStream(streamConfig)) {
			WARN("External stream configuration is invalid:\n%s",
					STR(streamConfig.ToString()));
		}
	}

	//3. Done
	return true;
}

bool BaseClientApplication::PullExternalStream(Variant streamConfig) {
	//1. Minimal verification
	if (streamConfig["uri"] != V_STRING) {
		FATAL("Invalid uri");
		return false;
	}

	//2. Split the URI
	URI uri;
	if (!URI::FromString(streamConfig["uri"], true, uri)) {
		FATAL("Invalid URI: %s", STR(streamConfig["uri"].ToString()));
		return false;
	}
	streamConfig["uri"] = uri.ToVariant();

	//3. Depending on the scheme name, get the curresponding protocol handler
	///TODO: integrate this into protocol factory manager via protocol factories
	BaseAppProtocolHandler *pProtocolHandler = GetProtocolHandler(uri.scheme);
	if (pProtocolHandler == NULL) {
		WARN("Unable to find protocol handler for scheme %s in application %s",
				STR(uri.scheme),
				STR(GetName()));
		return false;
	}

	//4. Initiate the stream pulling sequence
	return pProtocolHandler->PullExternalStream(uri, streamConfig);
}

bool BaseClientApplication::PushLocalStream(Variant streamConfig) {
	//1. Minimal verification
	if (streamConfig["targetUri"] != V_STRING) {
		FATAL("Invalid uri");
		return false;
	}
	if (streamConfig["localStreamName"] != V_STRING) {
		FATAL("Invalid local stream name");
		return false;
	}
	string streamName = (string) streamConfig["localStreamName"];
	trim(streamName);
	if (streamName == "") {
		FATAL("Invalid local stream name");
		return false;
	}
	streamConfig["localStreamName"] = streamName;

	//2. Split the URI
	URI uri;
	if (!URI::FromString(streamConfig["targetUri"], true, uri)) {
		FATAL("Invalid URI: %s", STR(streamConfig["targetUri"].ToString()));
		return false;
	}
	streamConfig["targetUri"] = uri.ToVariant();

	//3. Depending on the scheme name, get the curresponding protocol handler
	///TODO: integrate this into protocol factory manager via protocol factories
	BaseAppProtocolHandler *pProtocolHandler = GetProtocolHandler(uri.scheme);
	if (pProtocolHandler == NULL) {
		WARN("Unable to find protocol handler for scheme %s in application %s",
				STR(uri.scheme),
				STR(GetName()));
		return false;
	}

	//4. Initiate the stream pulling sequence
	return pProtocolHandler->PushLocalStream(streamConfig);
}

void BaseClientApplication::Shutdown(BaseClientApplication *pApplication) {
	//1. Get the list of all active protocols
	map<uint32_t, BaseProtocol *> protocols = ProtocolManager::GetActiveProtocols();

	//2. enqueue for delete for all protocols bound to pApplication

	FOR_MAP(protocols, uint32_t, BaseProtocol *, i) {
		if ((MAP_VAL(i)->GetApplication() != NULL)
				&& (MAP_VAL(i)->GetApplication()->GetId() == pApplication->GetId())) {
			MAP_VAL(i)->SetApplication(NULL);
			MAP_VAL(i)->EnqueueForDelete();
		}
	}

	//1. Get the list of all active IOHandlers and enqueue for delete for all services bound to pApplication
	map<uint32_t, IOHandler *> handlers = IOHandlerManager::GetActiveHandlers();

	FOR_MAP(handlers, uint32_t, IOHandler *, i) {
		BaseProtocol *pProtocol = MAP_VAL(i)->GetProtocol();
		BaseProtocol *pTemp = pProtocol;
		while (pTemp != NULL) {
			if ((pTemp->GetApplication() != NULL)
					&& (pTemp->GetApplication()->GetId() == pApplication->GetId())) {
				IOHandlerManager::EnqueueForDelete(MAP_VAL(i));
				break;
			}
			pTemp = pTemp->GetNearProtocol();
		}
	}

	handlers = IOHandlerManager::GetActiveHandlers();

	FOR_MAP(handlers, uint32_t, IOHandler *, i) {
		if ((MAP_VAL(i)->GetType() == IOHT_ACCEPTOR)
				&& (((TCPAcceptor *) MAP_VAL(i))->GetApplication() != NULL)) {
			if (((TCPAcceptor *) MAP_VAL(i))->GetApplication()->GetId() == pApplication->GetId())
				IOHandlerManager::EnqueueForDelete(MAP_VAL(i));
		}
	}

	//4. Unregister it
	ClientApplicationManager::UnRegisterApplication(pApplication);

	//5. Delete it
	delete pApplication;
}
