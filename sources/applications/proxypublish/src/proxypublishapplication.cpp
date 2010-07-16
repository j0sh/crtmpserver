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



#include "utils/core.h"
#include "proxypublishapplication.h"
#include "rtmpappprotocolhandler.h"
#include "protocols/baseprotocol.h"
#include "liveflvappprotocolhandler.h"
#include "streaming/streamstypes.h"
#include "streaming/basestream.h"
#include "protocols/rtmp/outboundrtmpprotocol.h"

ProxyPublishApplication::ProxyPublishApplication(Variant &configuration)
: BaseClientApplication(configuration) {
#ifdef HAS_PROTOCOL_RTMP
	_pRTMPHandler = NULL;
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_LIVEFLV
	_pLiveFLV = NULL;
#endif /* HAS_PROTOCOL_LIVEFLV */
}

ProxyPublishApplication::~ProxyPublishApplication() {
#ifdef HAS_PROTOCOL_RTMP
	UnRegisterAppProtocolHandler(PT_INBOUND_RTMP);
	UnRegisterAppProtocolHandler(PT_OUTBOUND_RTMP);
	if (_pRTMPHandler != NULL) {
		delete _pRTMPHandler;
		_pRTMPHandler = NULL;
	}
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_LIVEFLV
	UnRegisterAppProtocolHandler(PT_INBOUND_LIVE_FLV);
	if (_pLiveFLV != NULL) {
		delete _pLiveFLV;
		_pLiveFLV = NULL;
	}
#endif /* HAS_PROTOCOL_LIVEFLV */
}

bool ProxyPublishApplication::Initialize() {
	//1. read the target servers section, validate it and store it for later usage
	if (_configuration["abortOnConnectError"] != V_BOOL) {
		FATAL("Invalid abortOnConnectError");
		return false;
	}
	if (_configuration["targetServers"] != V_MAP) {
		FATAL("Invalid targetServers");
		return false;
	}
	//	if (_configuration["targetServers"].MapSize() == 0) {
	//		FATAL("Invalid targetServers");
	//		return false;
	//	}

	FOR_MAP(_configuration["targetServers"], string, Variant, i) {
		Variant &target = MAP_VAL(i);
		if (target != V_MAP) {
			FATAL("Invalid target: %s", STR(target.ToString()));
			return false;
		}
		if (target["targetUri"] != V_STRING) {
			FATAL("Invalid target: %s", STR(target.ToString()));
			return false;
		}
		if (target.HasKey("localStreamName")) {
			if (target["localStreamName"] != V_STRING) {
				FATAL("Invalid target: %s", STR(target.ToString()));
				return false;
			}
		}
		if (target.HasKey("targetStreamName")) {
			if (target["targetStreamName"] != V_STRING) {
				FATAL("Invalid target: %s", STR(target.ToString()));
				return false;
			}
		}
		if (target.HasKey("emulateUserAgent")) {
			if (target["emulateUserAgent"] != V_STRING) {
				FATAL("Invalid target: %s", STR(target.ToString()));
				return false;
			}
		} else {
			target["emulateUserAgent"] = HTTP_HEADERS_SERVER_US;
		}

		string host;
		string ip;
		uint16_t port;
		string application;
		string dummy;

		if (!ParseURL(target["targetUri"], host, port, dummy, dummy, application)) {
			FATAL("Invalid uri: %s", STR(target["targetUri"]));
			return false;
		}

		ip = GetHostByName(host);
		if (ip == "") {
			FATAL("Invalid target: %s", STR(target.ToString()));
			return false;
		}

		if (application.size() > 0) {
			if (application[0] == '/')
				application = application.substr(1);
		}

		target["targetIp"] = ip;
		target["targetPort"] = port;
		target["targetApplication"] = application;
		target["targetHost"] = host;

	}
	_targetServers = _configuration["targetServers"];
	_abortOnConnectError = (bool)_configuration["abortOnConnectError"];


	//1. Initialize the protocol handler(s)
#ifdef HAS_PROTOCOL_RTMP
	_pRTMPHandler = new RTMPAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_INBOUND_RTMP, _pRTMPHandler);
	RegisterAppProtocolHandler(PT_OUTBOUND_RTMP, _pRTMPHandler);
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_LIVEFLV
	_pLiveFLV = new LiveFLVAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_INBOUND_LIVE_FLV, _pLiveFLV);
#endif /* HAS_PROTOCOL_LIVEFLV */
	return true;
}

void ProxyPublishApplication::SignalStreamRegistered(BaseStream *pStream) {
	//1. Call the base class
	BaseClientApplication::SignalStreamRegistered(pStream);
	if ((!TAG_KIND_OF(pStream->GetType(), ST_IN_NET_LIVEFLV))
			&& (!TAG_KIND_OF(pStream->GetType(), ST_IN_NET_RTMP))) {
		FINEST("Stay put. We don't forward this kind of streams: %s",
				STR(tagToString(pStream->GetType())));
		return;
	}

	//2. Start the forwarding process
	if (!InitiateForwardingStream(pStream)) {
		FATAL("Unable to initiate the forwarding process");
		pStream->EnqueueForDelete();
	}
}

void ProxyPublishApplication::SignalStreamUnRegistered(BaseStream *pStream) {
	BaseClientApplication::SignalStreamUnRegistered(pStream);
}

bool ProxyPublishApplication::InitiateForwardingStream(BaseStream *pStream) {

	FOR_MAP(_targetServers, string, Variant, i) {
		Variant &target = MAP_VAL(i);
		if (!InitiateForwardingStream(pStream, target)) {
			WARN("Unable to forward stream %d of type %s with name `%s` owned by application `%s` to server %s",
					pStream->GetUniqueId(),
					STR(tagToString(pStream->GetType())),
					STR(pStream->GetName()),
					STR(GetName()),
					STR(format("rtmp://%s/%s", STR(target["ip"]), STR(target["appName"]))));
			if (_abortOnConnectError) {
				FATAL("Abort");
				return false;
			}
		}
	}
	return true;
}

bool ProxyPublishApplication::InitiateForwardingStream(BaseStream *pStream, Variant &target) {
#ifndef HAS_PROTOCOL_RTMP
	FATAL("RTMP protocol not supported");
	return false;
#else
	string remoteStreamName = pStream->GetName();
	if (target.HasKey("localStreamName")) {
		if (((string) target["localStreamName"]) != pStream->GetName()) {
			if (pStream->GetName().find(((string) target["localStreamName"]) + "?") != 0) {
				FINEST("Stream name not matching. Wanted: %s(?); Got: %s",
						STR(target["localStreamName"]),
						STR(pStream->GetName()));
				return true;
			}
		}
	}

	if (target.HasKey("targetStreamName")) {
		remoteStreamName = (string) target["targetStreamName"];
	}

	//1. Prepare our custom parameters
	Variant customParameters;
	customParameters["streamId"] = pStream->GetUniqueId();
	customParameters["remoteAppName"] = target["targetApplication"];
	customParameters["remoteUri"] = target["targetUri"];
	customParameters["remoteStreamName"] = remoteStreamName;
	customParameters["remoteUA"] = target["emulateUserAgent"];

	//2. Prepare the final connect parameters
	Variant parameters;
	parameters[CONF_APPLICATION_NAME] = GetName();
	parameters[CONF_PROTOCOL] = CONF_PROTOCOL_OUTBOUND_RTMP;
	parameters["customParameters"] = customParameters;

	INFO("Initiate forward stream %d of type %s with name `%s` owned by application `%s` to server %s",
			pStream->GetUniqueId(),
			STR(tagToString(pStream->GetType())),
			STR(pStream->GetName()),
			STR(GetName()),
			STR(format("%s/%s", STR(target["targetUri"]), STR(customParameters["remoteStreamName"]))));

	//3. Initiate the connect routine
	return OutboundRTMPProtocol::Connect(target["targetIp"], target["targetPort"], parameters);
#endif /* HAS_PROTOCOL_RTMP */
}

