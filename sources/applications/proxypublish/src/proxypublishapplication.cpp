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

#include "proxypublishapplication.h"
#include "rtmpappprotocolhandler.h"
#include "protocols/baseprotocol.h"
#include "liveflvappprotocolhandler.h"
#include "streaming/streamstypes.h"
#include "streaming/basestream.h"
#include "protocols/rtmp/outboundrtmpprotocol.h"
#include "rtpappprotocolhandler.h"
#include "rtspappprotocolhandler.h"
#include "protocols/protocolfactorymanager.h"
#include "application/clientapplicationmanager.h"
#include "netio/netio.h"

ProxyPublishApplication::ProxyPublishApplication(Variant &configuration)
: BaseClientApplication(configuration) {
#ifdef HAS_PROTOCOL_RTMP
	_pRTMPHandler = NULL;
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_LIVEFLV
	_pLiveFLV = NULL;
#endif /* HAS_PROTOCOL_LIVEFLV */
#ifdef HAS_PROTOCOL_RTP
	_pRTP = NULL;
	_pRTSP = NULL;
#endif /* HAS_PROTOCOL_RTP */
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
#ifdef HAS_PROTOCOL_RTP
	UnRegisterAppProtocolHandler(PT_INBOUND_RTP);
	if (_pRTP != NULL)
		delete _pRTP;

	UnRegisterAppProtocolHandler(PT_RTSP);
	if (_pRTSP != NULL)
		delete _pRTSP;
#endif /* HAS_PROTOCOL_RTP */
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

#ifdef HAS_PROTOCOL_RTP
	_pRTP = new RTPAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_INBOUND_RTP, _pRTP);
	RegisterAppProtocolHandler(PT_RTCP, _pRTP);

	_pRTSP = new RTSPAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_RTSP, _pRTSP);

	return SpawnRTSPProtocols();
#endif /* HAS_PROTOCOL_RTP */
	return true;
}

void ProxyPublishApplication::SignalStreamRegistered(BaseStream *pStream) {
	//1. Call the base class
	BaseClientApplication::SignalStreamRegistered(pStream);
	if ((!TAG_KIND_OF(pStream->GetType(), ST_IN_NET_LIVEFLV))
			&& (!TAG_KIND_OF(pStream->GetType(), ST_IN_NET_RTMP))
			&& (!TAG_KIND_OF(pStream->GetType(), ST_IN_NET_RTP))) {
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

bool ProxyPublishApplication::SpawnRTSPProtocols() {
	//1. Minimal verifications
	if (_configuration["rtspStreams"] == V_NULL)
		return true;

	if (_configuration["rtspStreams"] != V_MAP) {
		FATAL("Invalid rtspStreams node");
		return false;
	}

	//2. get the chain
	vector<uint64_t> chain = ProtocolFactoryManager::ResolveProtocolChain(
			CONF_PROTOCOL_INBOUND_RTSP);
	if (chain.size() == 0) {
		FATAL("Unable to resolve protocol chain");
		return false;
	}

	//3. Spawn the connections

	FOR_MAP(_configuration["rtspStreams"], string, Variant, i) {
		if (!SpawnRTSPProtocol(chain, MAP_VAL(i))) {
			FATAL("Unable to connect here:\n%s", STR(MAP_VAL(i).ToString()));
			return false;
		}
	}

	return true;
}

bool ProxyPublishApplication::SpawnRTSPProtocol(vector<uint64_t> &chain, Variant &node) {
	//1. Minimal verification
	if (node != V_MAP) {
		FATAL("Invalid node");
		return false;
	}
	if ((node["forceTcp"] != V_BOOL) || (node["url"] != V_STRING)) {
		FATAL("Invalid node");
		return false;
	}

	//2. Split the URI
	string host;
	uint16_t port;
	string user;
	string pwd;
	string doc;

	if (!ParseURL(node["url"], host, port, user, pwd, doc)) {
		FATAL("Invalid URL: %s", STR(node["url"].ToString()));
		return false;
	}

	//3. resolve the host
	string ip = GetHostByName(host);
	if (ip == "") {
		FATAL("Unable to resolve host %s", STR(host));
		return false;
	}

	//4. Save the app id inside the custom parameters
	node["appId"] = GetId();
	node["isClient"] = true;

	//5. Connect
	if (!TCPConnector<ProxyPublishApplication>::Connect(ip, port, chain, node)) {
		FATAL("Unable to connect to %s:%d", STR(host), port);
		return false;
	}

	//6. Done
	return true;
}

bool ProxyPublishApplication::SignalProtocolCreated(BaseProtocol *pProtocol,
		Variant &parameters) {
	//1. Sanitize
	if (parameters["appId"] != V_UINT32) {
		FATAL("Invalid custom parameters:\n%s", STR(parameters.ToString()));
		return false;
	}

	//2. Get the application
	BaseClientApplication *pApplication = ClientApplicationManager::FindAppById(
			parameters["appId"]);

	//3. Set it up inside the protocol
	pProtocol->SetApplication(pApplication);

	//4. Done
	return true;
}
