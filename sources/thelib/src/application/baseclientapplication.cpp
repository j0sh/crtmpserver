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
	if (configuration.HasKeyChain(V_MAP, false, 1, CONF_APPLICATION_ALIASES)) {

		FOR_MAP((configuration[CONF_APPLICATION_ALIASES]), string, Variant, i) {
			ADD_VECTOR_END(_aliases, MAP_VAL(i));
		}
	}
	_isDefault = false;
	if (configuration.HasKeyChain(V_BOOL, false, 1, CONF_APPLICATION_DEFAULT))
		_isDefault = (bool)configuration[CONF_APPLICATION_DEFAULT];
	_allowDuplicateInboundNetworkStreams = false;
	if (configuration.HasKeyChain(V_BOOL, false, 1,
			CONF_APPLICATION_ALLOW_DUPLICATE_INBOUND_NETWORK_STREAMS))
		_allowDuplicateInboundNetworkStreams =
			(bool)configuration[CONF_APPLICATION_ALLOW_DUPLICATE_INBOUND_NETWORK_STREAMS];
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

bool BaseClientApplication::ActivateAcceptors(vector<IOHandler *> &acceptors) {
	for (uint32_t i = 0; i < acceptors.size(); i++) {
		if (!ActivateAcceptor(acceptors[i])) {
			FATAL("Unable to activate acceptor");
			return false;
		}
	}
	return true;
}

bool BaseClientApplication::ActivateAcceptor(IOHandler *pIOHandler) {
	switch (pIOHandler->GetType()) {
		case IOHT_ACCEPTOR:
		{
			TCPAcceptor *pAcceptor = (TCPAcceptor *) pIOHandler;
			pAcceptor->SetApplication(this);
			return pAcceptor->StartAccept();
		}
		case IOHT_UDP_CARRIER:
		{
			UDPCarrier *pUDPCarrier = (UDPCarrier *) pIOHandler;
			pUDPCarrier->GetProtocol()->GetNearEndpoint()->SetApplication(this);
			return pUDPCarrier->StartAccept();
		}
		default:
		{
			FATAL("Invalid acceptor type");
			return false;
		}
	}
}

string BaseClientApplication::GetServicesInfo() {
	map<uint32_t, IOHandler *> handlers = IOHandlerManager::GetActiveHandlers();
	string result = "";

	FOR_MAP(handlers, uint32_t, IOHandler *, i) {
		result += GetServiceInfo(MAP_VAL(i));
	}
	return result;
}

bool BaseClientApplication::AcceptTCPConnection(TCPAcceptor *pTCPAcceptor) {
	return pTCPAcceptor->Accept();
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

bool BaseClientApplication::GetAllowDuplicateInboundNetworkStreams() {
	return _allowDuplicateInboundNetworkStreams;
}

bool BaseClientApplication::StreamNameAvailable(string streamName,
		BaseProtocol *pProtocol) {
	if (_allowDuplicateInboundNetworkStreams)
		return true;
	return _streamsManager.StreamNameAvailable(streamName);
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
	INFO("Stream %s(%"PRIu32") with name `%s` registered to application `%s` from protocol %s(%"PRIu32")",
			STR(tagToString(pStream->GetType())),
			pStream->GetUniqueId(),
			STR(pStream->GetName()),
			STR(_name),
			(pStream->GetProtocol() != NULL) ? STR(tagToString(pStream->GetProtocol()->GetType())) : "",
			(pStream->GetProtocol() != NULL) ? pStream->GetProtocol()->GetId() : (uint32_t) 0
			);
}

void BaseClientApplication::SignalStreamUnRegistered(BaseStream *pStream) {
	INFO("Stream %s(%"PRIu32") with name `%s` unregistered from application `%s` from protocol %s(%"PRIu32")",
			STR(tagToString(pStream->GetType())),
			pStream->GetUniqueId(),
			STR(pStream->GetName()),
			STR(_name),
			(pStream->GetProtocol() != NULL) ? STR(tagToString(pStream->GetProtocol()->GetType())) : "",
			(pStream->GetProtocol() != NULL) ? pStream->GetProtocol()->GetId() : (uint32_t) 0
			);
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

	//2. Loop over the stream definitions and validate duplicated stream names
	Variant streamConfigs;
	streamConfigs.IsArray(false);

	FOR_MAP(_configuration["externalStreams"], string, Variant, i) {
		Variant &temp = MAP_VAL(i);
		if ((!temp.HasKeyChain(V_STRING, false, 1, "localStreamName"))
				|| ((string) temp.GetValue("localStreamName", false) == "")) {
			WARN("External stream configuration is doesn't have localStreamName property invalid:\n%s",
					STR(temp.ToString()));
			continue;
		}
		string localStreamName = (string) temp.GetValue("localStreamName", false);
		if (!GetAllowDuplicateInboundNetworkStreams()) {
			if (streamConfigs.HasKey(localStreamName)) {
				WARN("External stream configuration produces duplicated stream names\n%s",
						STR(temp.ToString()));
				continue;
			}
		}
		streamConfigs[localStreamName] = temp;
	}


	//2. Loop over the stream definitions and spawn the streams

	FOR_MAP(streamConfigs, string, Variant, i) {
		Variant &streamConfig = MAP_VAL(i);
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
	streamConfig["uri"] = uri;

	//3. Depending on the scheme name, get the curresponding protocol handler
	///TODO: integrate this into protocol factory manager via protocol factories
	string scheme = uri.scheme();
	BaseAppProtocolHandler *pProtocolHandler = GetProtocolHandler(scheme);
	if (pProtocolHandler == NULL) {
		WARN("Unable to find protocol handler for scheme %s in application %s",
				STR(scheme),
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
	streamConfig["targetUri"] = uri;

	//3. Depending on the scheme name, get the curresponding protocol handler
	///TODO: integrate this into protocol factory manager via protocol factories
	string scheme = uri.scheme();
	BaseAppProtocolHandler *pProtocolHandler = GetProtocolHandler(scheme);
	if (pProtocolHandler == NULL) {
		WARN("Unable to find protocol handler for scheme %s in application %s",
				STR(scheme),
				STR(GetName()));
		return false;
	}

	//4. Initiate the stream pulling sequence
	return pProtocolHandler->PushLocalStream(streamConfig);
}

bool BaseClientApplication::ParseAuthentication() {
	//1. Get the authentication configuration node
	if (!_configuration.HasKeyChain(V_MAP, false, 1, CONF_APPLICATION_AUTH)) {
		if (_configuration.HasKey(CONF_APPLICATION_AUTH, false)) {
			WARN("Authentication node is present for application %s but is empty or invalid", STR(_name));
		}
		return true;
	}
	Variant &auth = _configuration[CONF_APPLICATION_AUTH];

	//2. Cycle over all access schemas

	FOR_MAP(auth, string, Variant, i) {
		//3. get the schema
		string scheme = MAP_KEY(i);

		//4. Get the handler
		BaseAppProtocolHandler *pHandler = GetProtocolHandler(scheme);
		if (pHandler == NULL) {
			WARN("Authentication parsing for app name %s failed. No handler registered for schema %s",
					STR(_name),
					STR(scheme));
			return true;
		}

		//5. Call the handler
		if (!pHandler->ParseAuthenticationNode(MAP_VAL(i), _authSettings[scheme])) {
			FATAL("Authentication parsing for app name %s failed. scheme was %s",
					STR(_name),
					STR(scheme));
			return false;
		}
	}

	return true;
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

string BaseClientApplication::GetServiceInfo(IOHandler *pIOHandler) {
	if ((pIOHandler->GetType() != IOHT_ACCEPTOR)
			&& (pIOHandler->GetType() != IOHT_UDP_CARRIER))
		return "";
	if (pIOHandler->GetType() == IOHT_ACCEPTOR) {
		if ((((TCPAcceptor *) pIOHandler)->GetApplication() == NULL)
				|| (((TCPAcceptor *) pIOHandler)->GetApplication()->GetId() != GetId())) {
			return "";
		}
	} else {
		if ((((UDPCarrier *) pIOHandler)->GetProtocol() == NULL)
				|| (((UDPCarrier *) pIOHandler)->GetProtocol()->GetNearEndpoint()->GetApplication() == NULL)
				|| (((UDPCarrier *) pIOHandler)->GetProtocol()->GetNearEndpoint()->GetApplication()->GetId() != GetId())) {
			return "";
		}
	}
	Variant &params = pIOHandler->GetType() == IOHT_ACCEPTOR ?
			((TCPAcceptor *) pIOHandler)->GetParameters()
			: ((UDPCarrier *) pIOHandler)->GetParameters();
	if (params != V_MAP)
		return "";
	stringstream ss;
	ss << "+---+---------------+-----+-------------------------+-------------------------+" << endl;
	ss << "|";
	ss.width(3);
	ss << (pIOHandler->GetType() == IOHT_ACCEPTOR ? "tcp" : "udp");
	ss << "|";

	ss.width(3 * 4 + 3);
	ss << (string) params[CONF_IP];
	ss << "|";

	ss.width(5);
	ss << (uint16_t) params[CONF_PORT];
	ss << "|";

	ss.width(25);
	ss << (string) params[CONF_PROTOCOL];
	ss << "|";

	ss.width(25);
	ss << GetName();
	ss << "|";

	ss << endl;

	return ss.str();
}
