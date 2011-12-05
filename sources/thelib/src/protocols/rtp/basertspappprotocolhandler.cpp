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

#ifdef HAS_PROTOCOL_RTP
#include "streaming/basestream.h"
#include "protocols/rtp/basertspappprotocolhandler.h"
#include "protocols/rtp/rtspprotocol.h"
#include "streaming/streamstypes.h"
#include "streaming/baseinnetstream.h"
#include "application/baseclientapplication.h"
#include "protocols/rtp/streaming/baseoutnetrtpudpstream.h"
#include "protocols/rtp/streaming/outnetrtpudph264stream.h"
#include "netio/netio.h"
#include "protocols/rtp/connectivity/outboundconnectivity.h"
#include "application/clientapplicationmanager.h"
#include "protocols/http/httpauthhelper.h"
#include "protocols/rtp/connectivity/inboundconnectivity.h"

BaseRTSPAppProtocolHandler::BaseRTSPAppProtocolHandler(Variant &configuration)
: BaseAppProtocolHandler(configuration) {
	_lastUsersFileUpdate = 0;
}

BaseRTSPAppProtocolHandler::~BaseRTSPAppProtocolHandler() {
}

bool BaseRTSPAppProtocolHandler::ParseAuthenticationNode(Variant &node,
		Variant &result) {
	//1. Users file validation
	string usersFile = node[CONF_APPLICATION_AUTH_USERS_FILE];
	if ((usersFile[0] != '/') && (usersFile[0] != '.')) {
		usersFile = (string) _configuration[CONF_APPLICATION_DIRECTORY] + usersFile;
	}
	if (!fileExists(usersFile)) {
		FATAL("Invalid authentication configuration. Missing users file: %s", STR(usersFile));
		return false;
	}
	_usersFile = usersFile;

	if (!ParseUsersFile()) {
		FATAL("Unable to parse users file %s", STR(usersFile));
		return false;
	}

	return true;
}

void BaseRTSPAppProtocolHandler::RegisterProtocol(BaseProtocol *pProtocol) {
	Variant &parameters = pProtocol->GetCustomParameters();
	//1. Is this a client RTSP protocol?
	if (pProtocol->GetType() != PT_RTSP)
		return;
	if (parameters != V_MAP)
		return;
	if (!parameters.HasKey("isClient"))
		return;
	if (parameters["isClient"] != V_BOOL)
		return;
	if (!((bool)parameters["isClient"]))
		return;

	//2. Get the protocol
	RTSPProtocol *pRTSPProtocol = (RTSPProtocol *) pProtocol;

	//3. validate the networking mode
	if (parameters.HasKey("forceTcp")) {
		if (parameters["forceTcp"] != V_BOOL) {
			FATAL("Invalid forceTcp flag detected");
			pRTSPProtocol->EnqueueForDelete();
			return;
		}
	} else {
		parameters["forceTcp"] = (bool)false;
	}

	if ((parameters.HasKeyChain(V_MAP, true, 2, "customParameters", "externalStreamConfig"))
			|| (parameters.HasKeyChain(V_MAP, true, 2, "customParameters", "localStreamConfig"))) {
		//5. Start play
		if (!TriggerPlayOrAnnounce(pRTSPProtocol)) {
			FATAL("Unable to initiate play on uri %s",
					STR(parameters["uri"]));
			pRTSPProtocol->EnqueueForDelete();
			return;
		}
	} else {
		WARN("Bogus connection. Terminate it");
		pProtocol->EnqueueForDelete();
	}
}

void BaseRTSPAppProtocolHandler::UnRegisterProtocol(BaseProtocol *pProtocol) {

}

bool BaseRTSPAppProtocolHandler::PullExternalStream(URI uri, Variant streamConfig) {
	//1. get the chain
	vector<uint64_t> chain = ProtocolFactoryManager::ResolveProtocolChain(
			CONF_PROTOCOL_INBOUND_RTSP);
	if (chain.size() == 0) {
		FATAL("Unable to resolve protocol chain");
		return false;
	}

	//2. Save the app id inside the custom parameters and mark this connection
	//as client connection
	Variant customParameters = streamConfig;
	customParameters["customParameters"]["externalStreamConfig"] = streamConfig;
	customParameters["isClient"] = (bool)true;
	customParameters["appId"] = GetApplication()->GetId();
	customParameters["uri"] = uri;
	customParameters["connectionType"] = "pull";

	//3. Connect
	if (!TCPConnector<BaseRTSPAppProtocolHandler>::Connect(
			uri.ip(),
			uri.port(),
			chain, customParameters)) {
		FATAL("Unable to connect to %s:%hu",
				STR(customParameters["uri"]["ip"]),
				(uint16_t) customParameters["uri"]["port"]);
		return false;
	}

	//4. Done
	return true;
}

bool BaseRTSPAppProtocolHandler::PushLocalStream(Variant streamConfig) {
	//1. get the stream name
	string streamName = (string) streamConfig["localStreamName"];

	//2. Get the streams manager
	StreamsManager *pStreamsManager = GetApplication()->GetStreamsManager();

	//3. Search for all streams named streamName having the type of IN_NET
	map<uint32_t, BaseStream *> streams = pStreamsManager->FindByTypeByName(
			ST_IN_NET, streamName, true,
			GetApplication()->GetAllowDuplicateInboundNetworkStreams());
	if (streams.size() == 0) {
		FATAL("Stream %s not found", STR(streamName));
		return false;
	}

	//4. See if inside the returned collection of streams
	//we have something compatible with RTSP
	BaseInStream *pInStream = NULL;

	FOR_MAP(streams, uint32_t, BaseStream *, i) {
		if (MAP_VAL(i)->IsCompatibleWithType(ST_OUT_NET_RTP)) {
			pInStream = (BaseInStream *) MAP_VAL(i);
			break;
		}
	}
	if (pInStream == NULL) {
		WARN("Stream %s not found or is incompatible with RTSP output",
				STR(streamName));
		return false;
	}

	//5. get the chain
	vector<uint64_t> chain = ProtocolFactoryManager::ResolveProtocolChain(
			CONF_PROTOCOL_INBOUND_RTSP);
	if (chain.size() == 0) {
		FATAL("Unable to resolve protocol chain");
		return false;
	}

	//6. Save the app id inside the custom parameters and mark this connection
	//as client connection
	Variant customParameters = streamConfig;
	customParameters["customParameters"]["localStreamConfig"] = streamConfig;
	customParameters["customParameters"]["localStreamConfig"]["localUniqueStreamId"] = pInStream->GetUniqueId();
	customParameters["streamId"] = pInStream->GetUniqueId();
	customParameters["isClient"] = (bool)true;
	customParameters["appId"] = GetApplication()->GetId();
	customParameters["uri"] = streamConfig["targetUri"];
	customParameters["connectionType"] = "push";

	//7. Connect
	if (!TCPConnector<BaseRTSPAppProtocolHandler>::Connect(
			streamConfig["targetUri"]["ip"],
			(uint16_t) streamConfig["targetUri"]["port"],
			chain, customParameters)) {
		FATAL("Unable to connect to %s:%hu",
				STR(streamConfig["targetUri"]["ip"]),
				(uint16_t) streamConfig["targetUri"]["port"]);
		return false;
	}

	return true;
}

bool BaseRTSPAppProtocolHandler::SignalProtocolCreated(BaseProtocol *pProtocol,
		Variant &parameters) {
	//1. Sanitize
	if (parameters["appId"] != V_UINT32) {
		FATAL("Invalid custom parameters:\n%s", STR(parameters.ToString()));
		return false;
	}

	//2. Get the application
	BaseClientApplication *pApplication = ClientApplicationManager::FindAppById(
			parameters["appId"]);

	if (pProtocol == NULL) {
		FATAL("Connection failed:\n%s", STR(parameters.ToString()));
		return pApplication->OutboundConnectionFailed(parameters);
	}

	//3. Set it up inside the protocol
	pProtocol->SetApplication(pApplication);

	//4. Done
	return true;
}

bool BaseRTSPAppProtocolHandler::HandleRTSPRequest(RTSPProtocol *pFrom,
		Variant &requestHeaders, string &requestContent) {
	string method = requestHeaders[RTSP_FIRST_LINE][RTSP_METHOD];

	//1. we need a CSeq
	if (!requestHeaders[RTSP_HEADERS].HasKey(RTSP_HEADERS_CSEQ, false)) {
		FATAL("Request doesn't have %s:\n%s", RTSP_HEADERS_CSEQ,
				STR(requestHeaders.ToString()));
		return false;
	}

	//2. Validate session id
	string wantedSessionId = pFrom->GetSessionId();
	if (wantedSessionId != "") {
		string requestSessionId = "";
		vector<string> parts;
		if (!requestHeaders[RTSP_HEADERS].HasKeyChain(V_STRING, false, 1, RTSP_HEADERS_SESSION)) {
			FATAL("No session id");
			return false;
		}

		requestSessionId = (string) requestHeaders[RTSP_HEADERS].GetValue(
				RTSP_HEADERS_SESSION, false);
		split(requestSessionId, ";", parts);
		if (parts.size() >= 1)
			requestSessionId = parts[0];

		if (requestSessionId != wantedSessionId) {
			FATAL("Invalid session ID. Wanted: `%s`; Got: `%s`",
					STR(wantedSessionId),
					STR(requestSessionId));
			return false;
		}
	}

	//4. Prepare a fresh new response. Add the sequence number
	pFrom->ClearResponseMessage();
	pFrom->PushResponseHeader(RTSP_HEADERS_CSEQ,
			requestHeaders[RTSP_HEADERS].GetValue(RTSP_HEADERS_CSEQ, false));

	//5. Do we have authentication? We will authenticate everything except "OPTIONS"
	if ((_usersFile != "") && NeedAuthentication(pFrom, requestHeaders,
			requestContent)) {
		//6. Re-parse authentication file if necessary
		if (!ParseUsersFile()) {
			FATAL("Unable to parse authentication file");
			return false;
		}

		//7. Get the real name to use it further in authentication process
		string realmName = GetAuthenticationRealm(pFrom, requestHeaders,
				requestContent);

		//8. Do we have that realm?
		if (!_realms.HasKey(realmName)) {
			FATAL("Realm `%s` not found", STR(realmName));
			return false;
		}
		Variant &realm = _realms[realmName];

		//9. Is the user even trying to authenticate?
		if (!requestHeaders[RTSP_HEADERS].HasKey(
				RTSP_HEADERS_AUTHORIZATION, false)) {
			return SendAuthenticationChallenge(pFrom, realm);
		} else {
			//14. The client sent us some response. Validate it now
			//Did we ever sent him an authorization challange?
			if (!pFrom->GetCustomParameters().HasKey("wwwAuthenticate")) {
				FATAL("Client tried to authenticate and the server didn't required that");
				return false;
			}

			//15. Get the server challenge
			string wwwAuthenticate = pFrom->GetCustomParameters()["wwwAuthenticate"];

			//16. Get the client response
			string authorization = (string) requestHeaders[RTSP_HEADERS].GetValue(
					RTSP_HEADERS_AUTHORIZATION, false);

			//17. Try to authenticate
			if (!HTTPAuthHelper::ValidateAuthRequest(wwwAuthenticate,
					authorization,
					method,
					(string) requestHeaders[RTSP_FIRST_LINE][RTSP_URL],
					realm)) {
				WARN("Authorization failed: challenge: %s; response: %s",
						STR(wwwAuthenticate), STR(authorization));
				return SendAuthenticationChallenge(pFrom, realm);
			}

			//18. Success. User authenticated
			//INFO("User authenticated: %s", STR(authorization));
		}
	}

	if (method == RTSP_METHOD_OPTIONS) {
		if (!HandleRTSPRequestOptions(pFrom, requestHeaders, requestContent)) {
			return false;
		}
	} else if (method == RTSP_METHOD_DESCRIBE) {
		if (!HandleRTSPRequestDescribe(pFrom, requestHeaders, requestContent)) {
			return false;
		}
	} else if (method == RTSP_METHOD_SETUP) {
		if (!HandleRTSPRequestSetup(pFrom, requestHeaders, requestContent)) {
			return false;
		}
	} else if (method == RTSP_METHOD_PLAY) {
		if (!HandleRTSPRequestPlay(pFrom, requestHeaders, requestContent)) {
			return false;
		}
	} else if (method == RTSP_METHOD_TEARDOWN) {
		if (!HandleRTSPRequestTearDown(pFrom, requestHeaders, requestContent)) {
			return false;
		}
	} else if (method == RTSP_METHOD_ANNOUNCE) {
		if (!HandleRTSPRequestAnnounce(pFrom, requestHeaders, requestContent)) {
			return false;
		}
	} else if (method == RTSP_METHOD_RECORD) {
		if (!HandleRTSPRequestRecord(pFrom, requestHeaders, requestContent)) {
			return false;
		}
	} else if (method == RTSP_METHOD_PAUSE) {
		if (!HandleRTSPRequestPause(pFrom, requestHeaders, requestContent)) {
			return false;
		}
	} else {
		FATAL("Method not implemented yet:\n%s", STR(requestHeaders.ToString()));
		return false;
	}

	return true;
}

bool BaseRTSPAppProtocolHandler::HandleRTSPResponse(RTSPProtocol *pFrom,
		Variant &responseHeaders, string &responseContent) {
	if (responseHeaders[RTSP_HEADERS].HasKeyChain(V_STRING, false, 1, RTSP_HEADERS_SESSION)) {
		string sessionId = (string) responseHeaders[RTSP_HEADERS].GetValue(RTSP_HEADERS_SESSION, false);
		if (!pFrom->SetSessionId(sessionId)) {
			FATAL("Unable to set sessionId");
			return false;
		}
	}

	//1. Test the sequence
	if (!responseHeaders[RTSP_HEADERS].HasKey(RTSP_HEADERS_CSEQ, false)) {
		FATAL("Invalid response:\n%s", STR(responseHeaders.ToString()));
		return false;
	}

	uint32_t seqId = atoi(STR(responseHeaders[RTSP_HEADERS].GetValue(RTSP_HEADERS_CSEQ, false)));
	Variant requestHeaders;
	string requestContent;
	if (!pFrom->GetRequest(seqId, requestHeaders, requestContent)) {
		FATAL("Invalid response sequence");
		return false;
	}

	//2. Get the request, get the response and call the stack further
	return HandleRTSPResponse(pFrom,
			requestHeaders,
			requestContent,
			responseHeaders,
			responseContent);
}

bool BaseRTSPAppProtocolHandler::HandleRTSPRequestOptions(RTSPProtocol *pFrom,
		Variant &requestHeaders, string &requestContent) {
	pFrom->PushResponseFirstLine(RTSP_VERSION_1_0, 200, "OK");
	pFrom->PushResponseHeader(RTSP_HEADERS_PUBLIC, "DESCRIBE, OPTIONS, PAUSE, PLAY, SETUP, TEARDOWN, ANNOUNCE, RECORD");
	return pFrom->SendResponseMessage();
}

bool BaseRTSPAppProtocolHandler::HandleRTSPRequestDescribe(RTSPProtocol *pFrom,
		Variant &requestHeaders, string &requestContent) {
	//1. get the stream name
	URI uri;
	if (!URI::FromString(requestHeaders[RTSP_FIRST_LINE][RTSP_URL], true, uri)) {
		FATAL("Invalid URI: %s", STR(requestHeaders[RTSP_FIRST_LINE][RTSP_URL]));
		return false;
	}
	string streamName = uri.documentWithFullParameters();
	if (streamName == "") {
		FATAL("Invalid stream name");
		return false;
	}

	//2. Get the inbound stream capabilities
	BaseInNetStream *pInStream = GetInboundStream(streamName);


	//3. Prepare the body of the response
	string outboundContent = ComputeSDP(pFrom, streamName, "", "0.0.0.0");
	if (outboundContent == "") {
		FATAL("Unable to compute SDP");
		return false;
	}

	//4. Save the stream id for later usage
	pFrom->GetCustomParameters()["streamId"] = pInStream->GetUniqueId();

	//5. Mark this connection as outbound connection
	pFrom->GetCustomParameters()["isInbound"] = false;

	//6. prepare the response
	pFrom->PushResponseFirstLine(RTSP_VERSION_1_0, 200, "OK");
	pFrom->PushResponseHeader(RTSP_HEADERS_CONTENT_TYPE, RTSP_HEADERS_ACCEPT_APPLICATIONSDP);
	pFrom->PushResponseContent(outboundContent, false);

	//7. Done
	return pFrom->SendResponseMessage();
}

bool BaseRTSPAppProtocolHandler::HandleRTSPRequestSetup(RTSPProtocol *pFrom,
		Variant &requestHeaders, string &requestContent) {
	if (pFrom->GetCustomParameters()["isInbound"] != V_BOOL) {
		FATAL("Invalid state");
		return false;
	}

	if ((bool)pFrom->GetCustomParameters()["isInbound"])
		return HandleRTSPRequestSetupInbound(pFrom, requestHeaders, requestContent);
	else
		return HandleRTSPRequestSetupOutbound(pFrom, requestHeaders, requestContent);
}

bool BaseRTSPAppProtocolHandler::HandleRTSPRequestSetupOutbound(RTSPProtocol *pFrom,
		Variant &requestHeaders, string &requestContent) {
	//1. Minimal sanitize
	if (!requestHeaders[RTSP_HEADERS].HasKey(RTSP_HEADERS_TRANSPORT, false)) {
		FATAL("RTSP %s request doesn't have %s header line",
				RTSP_METHOD_SETUP,
				RTSP_HEADERS_TRANSPORT);
		return false;
	}

	//2. get the transport header line
	string raw = requestHeaders[RTSP_HEADERS].GetValue(RTSP_HEADERS_TRANSPORT, false);
	Variant transport;
	if (!SDP::ParseTransportLine(raw, transport)) {
		FATAL("Unable to parse transport line %s", STR(raw));
		return false;
	}
	bool forceTcp = false;
	if (transport.HasKey("client_port")
			&& (transport.HasKey("rtp/avp/udp") || transport.HasKey("rtp/avp"))) {
		forceTcp = false;
	} else if (transport.HasKey("interleaved") && transport.HasKey("rtp/avp/tcp")) {
		forceTcp = true;
	} else {
		FATAL("Invalid transport line: %s", STR(transport.ToString()));
		return false;
	}
	pFrom->GetCustomParameters()["forceTcp"] = (bool)forceTcp;

	//3. Get the outbound connectivity
	OutboundConnectivity *pOutboundConnectivity = GetOutboundConnectivity(pFrom,
			forceTcp);
	if (pOutboundConnectivity == NULL) {
		FATAL("Unable to get the outbound connectivity");
		return false;
	}

	//5. Find out if this is audio or video
	bool isAudioTrack = false;
	string rawUrl = requestHeaders[RTSP_FIRST_LINE][RTSP_URL];
	string audioTrackId = "trackID=" + (string) pFrom->GetCustomParameters()["audioTrackId"];
	string videoTrackId = "trackID=" + (string) pFrom->GetCustomParameters()["videoTrackId"];
	/*FINEST("rawUrl: %s; audioTrackId: %s; videoTrackId: %s; fa: %d; fv: %d",
		STR(rawUrl),
		STR(audioTrackId),
		STR(videoTrackId),
		rawUrl.find(audioTrackId) != string::npos,
		rawUrl.find(videoTrackId) != string::npos
	);*/
	if (rawUrl.find(audioTrackId) != string::npos) {
		isAudioTrack = true;
	} else if (rawUrl.find(videoTrackId) != string::npos) {
		isAudioTrack = false;
	} else {
		FATAL("Invalid track. Wanted: %s or %s; Got: %s",
				STR(pFrom->GetCustomParameters()["audioTrackId"]),
				STR(pFrom->GetCustomParameters()["videoTrackId"]),
				STR(requestHeaders[RTSP_FIRST_LINE][RTSP_URL]));
		return false;
	}
	pFrom->GetCustomParameters()["isAudioTrack"] = (bool)isAudioTrack;
	if (isAudioTrack) {
		if (forceTcp) {
			pFrom->GetCustomParameters()["audioDataChannelNumber"] = transport["interleaved"]["data"];
			pFrom->GetCustomParameters()["audioRtcpChannelNumber"] = transport["interleaved"]["rtcp"];
			pFrom->GetCustomParameters()["audioTrackUri"] = requestHeaders[RTSP_FIRST_LINE][RTSP_URL];
			pOutboundConnectivity->HasAudio(true);
		} else {
			pFrom->GetCustomParameters()["audioDataPortNumber"] = transport["client_port"]["data"];
			pFrom->GetCustomParameters()["audioRtcpPortNumber"] = transport["client_port"]["rtcp"];
			pFrom->GetCustomParameters()["audioTrackUri"] = requestHeaders[RTSP_FIRST_LINE][RTSP_URL];
			pOutboundConnectivity->HasAudio(true);
		}
	} else {
		if (forceTcp) {
			pFrom->GetCustomParameters()["videoDataChannelNumber"] = transport["interleaved"]["data"];
			pFrom->GetCustomParameters()["videoRtcpChannelNumber"] = transport["interleaved"]["rtcp"];
			pFrom->GetCustomParameters()["videoTrackUri"] = requestHeaders[RTSP_FIRST_LINE][RTSP_URL];
			pOutboundConnectivity->HasVideo(true);
		} else {
			pFrom->GetCustomParameters()["videoDataPortNumber"] = transport["client_port"]["data"];
			pFrom->GetCustomParameters()["videoRtcpPortNumber"] = transport["client_port"]["rtcp"];
			pFrom->GetCustomParameters()["videoTrackUri"] = requestHeaders[RTSP_FIRST_LINE][RTSP_URL];
			pOutboundConnectivity->HasVideo(true);
		}
	}

	//10. Create a session
	pFrom->GenerateSessionId();

	//10 Compose the response
	pFrom->PushResponseFirstLine(RTSP_VERSION_1_0, 200, "OK");
	if (forceTcp) {
		pFrom->PushResponseHeader(RTSP_HEADERS_TRANSPORT,
				format("RTP/AVP/TCP;unicast;interleaved=%s",
				STR(transport["interleaved"]["all"])));
	} else {
		pFrom->PushResponseHeader(RTSP_HEADERS_TRANSPORT,
				format("RTP/AVP/UDP;unicast;source=%s;client_port=%s;server_port=%s;ssrc=%08x",
				STR(((TCPCarrier *) pFrom->GetIOHandler())->GetNearEndpointAddressIp()),
				STR(transport["client_port"]["all"]),
				isAudioTrack ? STR(pOutboundConnectivity->GetAudioPorts())
				: STR(pOutboundConnectivity->GetVideoPorts()),
				isAudioTrack ? pOutboundConnectivity->GetAudioSSRC()
				: pOutboundConnectivity->GetVideoSSRC()));
	}

	//10. Done
	return pFrom->SendResponseMessage();
}

bool BaseRTSPAppProtocolHandler::HandleRTSPRequestSetupInbound(RTSPProtocol *pFrom,
		Variant &requestHeaders, string &requestContent) {
	//1. get the transport line and split it into parts
	if (!requestHeaders[RTSP_HEADERS].HasKey(RTSP_HEADERS_TRANSPORT, false)) {
		FATAL("No transport line");
		return false;
	}
	string transportLine = requestHeaders[RTSP_HEADERS].GetValue(RTSP_HEADERS_TRANSPORT, false);
	Variant transport;
	if (!SDP::ParseTransportLine(transportLine, transport)) {
		FATAL("Unable to parse transport line");
		return false;
	}

	//2. Check and see if it has RTP/AVP/TCP,RTP/AVP/UDP or RTP/AVP
	if ((!transport.HasKey("rtp/avp/tcp"))
			&& (!transport.HasKey("rtp/avp/udp"))
			&& (!transport.HasKey("rtp/avp"))) {
		FATAL("Invalid transport line: %s", STR(transportLine));
		return false;
	}

	//3. Check to see if it has either client_port OR interleaved
	if ((!transport.HasKey("client_port"))
			&& (!transport.HasKey("interleaved"))) {
		FATAL("Invalid transport line: %s", STR(transportLine));
		return false;
	}
	if ((transport.HasKey("client_port"))
			&& (transport.HasKey("interleaved"))) {
		FATAL("Invalid transport line: %s", STR(transportLine));
		return false;
	}

	//4. Get the InboundConnectivity
	InboundConnectivity *pConnectivity = pFrom->GetInboundConnectivity();

	//4. Find the track inside the pendingTracks collection and setup the ports or channels
	if (pFrom->GetCustomParameters()["pendingTracks"] != V_MAP) {
		FATAL("Invalid state. No pending tracks");
		return false;
	}
	string controlUri = requestHeaders[RTSP_FIRST_LINE][RTSP_URL];

	bool trackFound = false;

	FOR_MAP(pFrom->GetCustomParameters()["pendingTracks"], string, Variant, i) {
		Variant &track = MAP_VAL(i);
		if (track["controlUri"] == controlUri) {
			if (transport.HasKey("client_port")) {
				track["portsOrChannels"] = transport["client_port"];
				track["isTcp"] = (bool)false;
			} else {
				track["portsOrChannels"] = transport["interleaved"];
				track["isTcp"] = (bool)true;
			}
			if (!pConnectivity->AddTrack(track, (bool)track["isAudio"])) {
				FATAL("Unable to add audio track");
				return false;
			}
			transportLine = pConnectivity->GetTransportHeaderLine((bool)track["isAudio"], false);
			trackFound = true;
			break;
		}
	}
	if (!trackFound) {
		FATAL("track %s not found", STR(controlUri));
		return false;
	}

	//5. Create a session
	pFrom->GenerateSessionId();

	//6. prepare the response
	pFrom->PushResponseFirstLine(RTSP_VERSION_1_0, 200, "OK");
	pFrom->PushResponseHeader(RTSP_HEADERS_TRANSPORT, transportLine);

	//7. Send it
	return pFrom->SendResponseMessage();
}

bool BaseRTSPAppProtocolHandler::HandleRTSPRequestPlay(RTSPProtocol *pFrom,
		Variant &requestHeaders, string &requestContent) {

	//1. Get the outbound connectivity
	bool forceTcp = (bool)pFrom->GetCustomParameters().GetValue("forceTcp", false);
	OutboundConnectivity *pOutboundConnectivity = GetOutboundConnectivity(pFrom, true);
	if (pOutboundConnectivity == NULL) {
		FATAL("Unable to get the outbound connectivity");
		return false;
	}

	if (forceTcp) {
		//3. Get the audio/video client ports
		uint8_t videoDataChannelNumber = 0xff;
		uint8_t videoRtcpChannelNumber = 0xff;
		uint8_t audioDataChannelNumber = 0xff;
		uint8_t audioRtcpChannelNumber = 0xff;
		if (pFrom->GetCustomParameters().HasKey("audioDataChannelNumber")) {
			audioDataChannelNumber = (uint8_t) pFrom->GetCustomParameters()["audioDataChannelNumber"];
			audioRtcpChannelNumber = (uint8_t) pFrom->GetCustomParameters()["audioRtcpChannelNumber"];
		}
		if (pFrom->GetCustomParameters().HasKey("videoDataChannelNumber")) {
			videoDataChannelNumber = (uint8_t) pFrom->GetCustomParameters()["videoDataChannelNumber"];
			videoRtcpChannelNumber = (uint8_t) pFrom->GetCustomParameters()["videoRtcpChannelNumber"];
		}

		//4.register the video
		if (videoDataChannelNumber != 0xff) {
			if (!pOutboundConnectivity->RegisterTCPVideoClient(pFrom->GetId(),
					videoDataChannelNumber, videoRtcpChannelNumber)) {
				FATAL("Unable to register video stream");
				return false;
			}
		}

		//5. Register the audio
		if (audioDataChannelNumber != 0xff) {
			if (!pOutboundConnectivity->RegisterTCPAudioClient(pFrom->GetId(),
					audioDataChannelNumber, audioRtcpChannelNumber)) {
				FATAL("Unable to register audio stream");
				return false;
			}
		}
	} else {
		//3. Get the audio/video client ports
		uint16_t videoDataPortNumber = 0;
		uint16_t videoRtcpPortNumber = 0;
		uint16_t audioDataPortNumber = 0;
		uint16_t audioRtcpPortNumber = 0;
		if (pFrom->GetCustomParameters().HasKey("audioDataPortNumber")) {
			audioDataPortNumber = (uint16_t) pFrom->GetCustomParameters()["audioDataPortNumber"];
			audioRtcpPortNumber = (uint16_t) pFrom->GetCustomParameters()["audioRtcpPortNumber"];
		}
		if (pFrom->GetCustomParameters().HasKey("videoDataPortNumber")) {
			videoDataPortNumber = (uint16_t) pFrom->GetCustomParameters()["videoDataPortNumber"];
			videoRtcpPortNumber = (uint16_t) pFrom->GetCustomParameters()["videoRtcpPortNumber"];
		}

		//4.register the video
		if (videoDataPortNumber != 0) {
			sockaddr_in videoDataAddress = ((TCPCarrier *) pFrom->GetIOHandler())->GetFarEndpointAddress();
			videoDataAddress.sin_port = EHTONS(videoDataPortNumber);
			sockaddr_in videoRtcpAddress = ((TCPCarrier *) pFrom->GetIOHandler())->GetFarEndpointAddress();
			videoRtcpAddress.sin_port = EHTONS(videoRtcpPortNumber);
			if (!pOutboundConnectivity->RegisterUDPVideoClient(pFrom->GetId(),
					videoDataAddress, videoRtcpAddress)) {
				FATAL("Unable to register video stream");
				return false;
			}
		}

		//5. Register the audio
		if (audioDataPortNumber != 0) {
			sockaddr_in audioDataAddress = ((TCPCarrier *) pFrom->GetIOHandler())->GetFarEndpointAddress();
			audioDataAddress.sin_port = EHTONS(audioDataPortNumber);
			sockaddr_in audioRtcpAddress = ((TCPCarrier *) pFrom->GetIOHandler())->GetFarEndpointAddress();
			audioRtcpAddress.sin_port = EHTONS(audioRtcpPortNumber);
			if (!pOutboundConnectivity->RegisterUDPAudioClient(pFrom->GetId(),
					audioDataAddress, audioRtcpAddress)) {
				FATAL("Unable to register audio stream");
				return false;
			}
		}
	}

	//6. prepare the response
	pFrom->PushResponseFirstLine(RTSP_VERSION_1_0, 200, "OK");

	//7. Done
	return pFrom->SendResponseMessage();
}

bool BaseRTSPAppProtocolHandler::HandleRTSPRequestTearDown(RTSPProtocol *pFrom,
		Variant &requestHeaders, string &requestContent) {
	pFrom->EnqueueForDelete();
	return true;
}

bool BaseRTSPAppProtocolHandler::HandleRTSPRequestAnnounce(RTSPProtocol *pFrom,
		Variant &requestHeaders, string &requestContent) {
	//1. Make sure we ONLY handle application/sdp
	if (!requestHeaders[RTSP_HEADERS].HasKey(RTSP_HEADERS_CONTENT_TYPE, false)) {
		FATAL("Invalid ANNOUNCE request:\n%s", STR(requestHeaders.ToString()));
		return false;
	}
	if ((string) requestHeaders[RTSP_HEADERS].GetValue(RTSP_HEADERS_CONTENT_TYPE, false)
			!= RTSP_HEADERS_ACCEPT_APPLICATIONSDP) {
		FATAL("Invalid ANNOUNCE request:\n%s", STR(requestHeaders.ToString()));
		return false;
	}

	//2. Get the SDP
	SDP &sdp = pFrom->GetInboundSDP();

	//3. Parse the SDP
	if (!SDP::ParseSDP(sdp, requestContent)) {
		FATAL("Unable to parse the SDP");
		return false;
	}

	//4. Get the first video track
	Variant videoTrack = sdp.GetVideoTrack(0,
			requestHeaders[RTSP_FIRST_LINE][RTSP_URL]);
	Variant audioTrack = sdp.GetAudioTrack(0,
			requestHeaders[RTSP_FIRST_LINE][RTSP_URL]);

	//5. Store the tracks inside the session for later use
	if (audioTrack != V_NULL) {
		pFrom->GetCustomParameters()["pendingTracks"][(uint32_t) SDP_TRACK_GLOBAL_INDEX(audioTrack)] = audioTrack;
	}
	if (videoTrack != V_NULL) {
		pFrom->GetCustomParameters()["pendingTracks"][(uint32_t) SDP_TRACK_GLOBAL_INDEX(videoTrack)] = videoTrack;
	}

	//6. Mark this connection as inbound connection
	pFrom->GetCustomParameters()["isInbound"] = true;

	//7. Save the streamName
	string streamName = sdp.GetStreamName();
	if (streamName == "") {
		streamName = format("rtsp_stream_%u", pFrom->GetId());
	}
	pFrom->GetCustomParameters()["sdpStreamName"] = streamName;

	//8. Save the bandwidth hint
	pFrom->GetCustomParameters()["sdpBandwidthHint"] = (uint32_t) sdp.GetTotalBandwidth();

	//9. Get the inbound connectivity
	InboundConnectivity *pInboundConnectivity = pFrom->GetInboundConnectivity(
			streamName,
			sdp.GetTotalBandwidth(),
			(uint8_t) GetApplication()->GetConfiguration()[CONF_APPLICATION_RTCPDETECTIONINTERVAL]);
	if (pInboundConnectivity == NULL) {
		FATAL("Unable to create inbound connectivity");
		return false;
	}

	//8. Send back the response
	pFrom->PushResponseFirstLine(RTSP_VERSION_1_0, 200, "OK");
	return pFrom->SendResponseMessage();
}

bool BaseRTSPAppProtocolHandler::HandleRTSPRequestRecord(RTSPProtocol *pFrom,
		Variant &requestHeaders, string &requestContent) {
	//1. Make sure we have everything and we are in the proper state
	if ((pFrom->GetCustomParameters()["isInbound"] != V_BOOL)
			|| ((bool)pFrom->GetCustomParameters()["isInbound"] != true)) {
		FATAL("Invalid state");
		return false;
	}

	if (pFrom->GetCustomParameters()["pendingTracks"] != V_MAP) {
		FATAL("Invalid state");
		return false;
	}

	//3. Get the inbound connectivity
	InboundConnectivity *pConnectivity = pFrom->GetInboundConnectivity();
	if (pConnectivity == NULL) {
		FATAL("Unable to get inbound connectivity");
		return false;
	}
	if (!pConnectivity->Initialize()) {
		FATAL("Unable to initialize inbound connectivity");
		return false;
	}

	//4. Send back the response
	pFrom->PushResponseFirstLine(RTSP_VERSION_1_0, 200, "OK");
	return pFrom->SendResponseMessage();
}

bool BaseRTSPAppProtocolHandler::HandleRTSPRequestPause(RTSPProtocol *pFrom,
		Variant &requestHeaders, string &requestContent) {
	pFrom->PushResponseFirstLine(RTSP_VERSION_1_0, 200, "OK");
	return pFrom->SendResponseMessage();
}

bool BaseRTSPAppProtocolHandler::HandleRTSPResponse(RTSPProtocol *pFrom,
		Variant &requestHeaders, string &requestContent, Variant &responseHeaders,
		string &responseContent) {
	switch ((uint32_t) responseHeaders[RTSP_FIRST_LINE][RTSP_STATUS_CODE]) {
		case 200:
		{
			return HandleRTSPResponse200(pFrom, requestHeaders, requestContent,
					responseHeaders, responseContent);
		}
		case 401:
		{
			return HandleRTSPResponse401(pFrom, requestHeaders, requestContent,
					responseHeaders, responseContent);
		}
		case 404:
		{
			return HandleRTSPResponse404(pFrom, requestHeaders, requestContent,
					responseHeaders, responseContent);
		}
		default:
		{
			FATAL("Response not yet implemented. request:\n%s\nresponse:\n%s",
					STR(requestHeaders.ToString()),
					STR(responseHeaders.ToString()));

			return false;
		}
	}
}

bool BaseRTSPAppProtocolHandler::HandleRTSPResponse200(RTSPProtocol *pFrom,
		Variant &requestHeaders, string &requestContent, Variant &responseHeaders,
		string &responseContent) {
	//1. Get the method
	string method = requestHeaders[RTSP_FIRST_LINE][RTSP_METHOD];

	//2. Call the appropriate function
	if (method == RTSP_METHOD_OPTIONS) {
		return HandleRTSPResponse200Options(pFrom, requestHeaders, requestContent,
				responseHeaders, responseContent);
	} else if (method == RTSP_METHOD_DESCRIBE) {
		return HandleRTSPResponse200Describe(pFrom, requestHeaders, requestContent,
				responseHeaders, responseContent);
	} else if (method == RTSP_METHOD_SETUP) {
		return HandleRTSPResponse200Setup(pFrom, requestHeaders, requestContent,
				responseHeaders, responseContent);
	} else if (method == RTSP_METHOD_PLAY) {
		return HandleRTSPResponse200Play(pFrom, requestHeaders, requestContent,
				responseHeaders, responseContent);
	} else if (method == RTSP_METHOD_ANNOUNCE) {
		return HandleRTSPResponse200Announce(pFrom, requestHeaders, requestContent,
				responseHeaders, responseContent);
	} else if (method == RTSP_METHOD_RECORD) {
		return HandleRTSPResponse200Record(pFrom, requestHeaders, requestContent,
				responseHeaders, responseContent);
	} else {
		FATAL("Response for method %s not implemented yet", STR(method));
		return false;
	}
}

bool BaseRTSPAppProtocolHandler::HandleRTSPResponse401(RTSPProtocol *pFrom, Variant &requestHeaders,
		string &requestContent, Variant &responseHeaders,
		string &responseContent) {
	if ((!pFrom->GetCustomParameters().HasKeyChain(V_MAP, false, 1, "uri"))
			|| (!pFrom->GetCustomParameters().HasKeyChain(V_STRING, false, 2, "uri", "userName"))
			|| (!pFrom->GetCustomParameters().HasKeyChain(V_STRING, false, 2, "uri", "password"))
			|| ((string) pFrom->GetCustomParameters()["uri"]["userName"] == "")) {
		FATAL("No username/password provided");
		return false;
	}
	if ((!responseHeaders.HasKeyChain(V_STRING, false, 2, RTSP_HEADERS, HTTP_HEADERS_WWWAUTHENTICATE))
			|| ((string) responseHeaders[RTSP_HEADERS][HTTP_HEADERS_WWWAUTHENTICATE] == "")) {
		FATAL("Invalid 401 response: %s", STR(responseHeaders.ToString()));
		return false;
	}
	string username = pFrom->GetCustomParameters()["uri"]["userName"];
	string password = pFrom->GetCustomParameters()["uri"]["password"];
	if (!pFrom->SetAuthentication(
			(string) responseHeaders[RTSP_HEADERS][HTTP_HEADERS_WWWAUTHENTICATE],
			username,
			password)) {
		FATAL("Unable to authenticate: request headers:\n%s\nresponseHeaders:\n%s",
				STR(requestHeaders.ToString()),
				STR(responseHeaders.ToString()));
		return false;
	}
	return true;
}

bool BaseRTSPAppProtocolHandler::HandleRTSPResponse404(RTSPProtocol *pFrom, Variant &requestHeaders,
		string &requestContent, Variant &responseHeaders,
		string &responseContent) {
	//1. Get the method
	string method = requestHeaders[RTSP_FIRST_LINE][RTSP_METHOD];

	//2. Call the appropriate function
	if (method == RTSP_METHOD_PLAY) {
		return HandleRTSPResponse404Play(pFrom, requestHeaders, requestContent,
				responseHeaders, responseContent);
	} else if (method == RTSP_METHOD_DESCRIBE) {
		return HandleRTSPResponse404Describe(pFrom, requestHeaders, requestContent,
				responseHeaders, responseContent);
	} else {
		FATAL("Response for method %s not implemented yet\n%s", STR(method),
				STR(responseHeaders.ToString()));
		return false;
	}
}

bool BaseRTSPAppProtocolHandler::HandleRTSPResponse200Options(
		RTSPProtocol *pFrom, Variant &requestHeaders, string &requestContent,
		Variant &responseHeaders, string &responseContent) {
	if (pFrom->HasConnectivity()) {
		//FINEST("This is a keep alive timer....");
		return true;
	}

	if (!pFrom->GetCustomParameters().HasKeyChain(V_STRING, true, 1, "connectionType")) {
		FATAL("Bogus connection");
		pFrom->EnqueueForDelete();
		return false;
	}

	//1. Sanitize
	if (!responseHeaders[RTSP_HEADERS].HasKey(RTSP_HEADERS_PUBLIC, false)) {
		FATAL("Invalid response:\n%s", STR(responseHeaders.ToString()));
		return false;
	}

	//2. get the raw options
	string raw = responseHeaders[RTSP_HEADERS].GetValue(RTSP_HEADERS_PUBLIC,
			false);

	//3. split and normalize the options
	map<string, string> parts = mapping(raw, ",", ":", true);
	string url = requestHeaders[RTSP_FIRST_LINE][RTSP_URL];

	if (pFrom->GetCustomParameters()["connectionType"] == "pull") {
		//4. Test the presence of the wanted methods
		if ((!MAP_HAS1(parts, RTSP_METHOD_DESCRIBE))
				|| (!MAP_HAS1(parts, RTSP_METHOD_SETUP))
				|| (!MAP_HAS1(parts, RTSP_METHOD_PLAY))
				) {
			FATAL("Some of the supported methods are missing: %s", STR(raw));
			return false;
		}

		//5. Prepare the DESCRIBE method
		pFrom->PushRequestFirstLine(RTSP_METHOD_DESCRIBE, url, RTSP_VERSION_1_0);
		pFrom->PushRequestHeader(RTSP_HEADERS_ACCEPT, RTSP_HEADERS_ACCEPT_APPLICATIONSDP);
		return pFrom->SendRequestMessage();

	} else if (pFrom->GetCustomParameters()["connectionType"] == "push") {
		//4. Test the presence of the wanted methods
		if ((!MAP_HAS1(parts, RTSP_METHOD_ANNOUNCE))
				|| (!MAP_HAS1(parts, RTSP_METHOD_SETUP))
				|| (!MAP_HAS1(parts, RTSP_METHOD_RECORD))
				) {
			FATAL("Some of the supported methods are missing: %s", STR(raw));
			return false;
		}
		Variant parameters = pFrom->GetCustomParameters();
		pFrom->PushRequestFirstLine(RTSP_METHOD_ANNOUNCE, url, RTSP_VERSION_1_0);
		string sdp = ComputeSDP(pFrom,
				parameters["customParameters"]["localStreamConfig"]["localStreamName"],
				parameters["customParameters"]["localStreamConfig"]["targetStreamName"],
				parameters["customParameters"]["localStreamConfig"]["targetUri"]["host"]);
		if (sdp == "") {
			FATAL("Unable to compute sdp");
			return false;
		}
		pFrom->PushRequestHeader(RTSP_HEADERS_CONTENT_TYPE, RTSP_HEADERS_ACCEPT_APPLICATIONSDP);
		pFrom->PushRequestContent(sdp, false);
		return pFrom->SendRequestMessage();
	} else {
		FATAL("Bogus connection");
		pFrom->EnqueueForDelete();
		return false;
	}
}

bool BaseRTSPAppProtocolHandler::HandleRTSPResponse200Describe(
		RTSPProtocol *pFrom, Variant &requestHeaders, string &requestContent,
		Variant &responseHeaders, string &responseContent) {
	//1. Make sure we ONLY handle application/sdp
	if (!responseHeaders[RTSP_HEADERS].HasKey(RTSP_HEADERS_CONTENT_TYPE, false)) {
		FATAL("Invalid DESCRIBE response:\n%s", STR(requestHeaders.ToString()));
		return false;
	}
	if ((string) responseHeaders[RTSP_HEADERS].GetValue(RTSP_HEADERS_CONTENT_TYPE, false)
			!= RTSP_HEADERS_ACCEPT_APPLICATIONSDP) {
		FATAL("Invalid DESCRIBE response:\n%s", STR(requestHeaders.ToString()));
		return false;
	}

	//2. Get the SDP
	SDP &sdp = pFrom->GetInboundSDP();

	//3. Parse the SDP
	if (!SDP::ParseSDP(sdp, responseContent)) {
		FATAL("Unable to parse the SDP");
		return false;
	}

	//4. Get the first video track
	Variant videoTrack = sdp.GetVideoTrack(0,
			requestHeaders[RTSP_FIRST_LINE][RTSP_URL]);
	Variant audioTrack = sdp.GetAudioTrack(0,
			requestHeaders[RTSP_FIRST_LINE][RTSP_URL]);
	if ((videoTrack == V_NULL) && (audioTrack == V_NULL)) {
		FATAL("No compatible tracks found");
		return false;
	}

	bool forceTcp = false;
	if (pFrom->GetCustomParameters().HasKeyChain(V_BOOL, true, 1, "forceTcp"))
		forceTcp = (bool)pFrom->GetCustomParameters()["forceTcp"];

	uint8_t rtcpDetectionInterval = (uint8_t) GetApplication()->GetConfiguration()[CONF_APPLICATION_RTCPDETECTIONINTERVAL];
	if (pFrom->GetCustomParameters().HasKeyChain(_V_NUMERIC, true, 1, CONF_APPLICATION_RTCPDETECTIONINTERVAL))
		rtcpDetectionInterval = (uint8_t) pFrom->GetCustomParameters()[CONF_APPLICATION_RTCPDETECTIONINTERVAL];

	//5. Store the tracks inside the session for later use
	if (audioTrack != V_NULL) {
		audioTrack["isTcp"] = (bool)forceTcp;
		pFrom->GetCustomParameters()["pendingTracks"][(uint32_t) SDP_TRACK_GLOBAL_INDEX(audioTrack)] = audioTrack;
	}
	if (videoTrack != V_NULL) {
		videoTrack["isTcp"] = (bool)forceTcp;
		pFrom->GetCustomParameters()["pendingTracks"][(uint32_t) SDP_TRACK_GLOBAL_INDEX(videoTrack)] = videoTrack;
	}

	//6. Save the streamName
	string streamName = sdp.GetStreamName();
	if (streamName == "") {
		streamName = format("rtsp_stream_%u", pFrom->GetId());
	}
	pFrom->GetCustomParameters()["sdpStreamName"] = streamName;

	//7. Save the bandwidth hint
	pFrom->GetCustomParameters()["sdpBandwidthHint"] = (uint32_t) sdp.GetTotalBandwidth();

	//8. Get the inbound connectivity
	InboundConnectivity *pInboundConnectivity = pFrom->GetInboundConnectivity(
			streamName,
			sdp.GetTotalBandwidth(),
			rtcpDetectionInterval);
	if (pInboundConnectivity == NULL) {
		FATAL("Unable to create inbound connectivity");
		return false;
	}

	//9. Start sending the setup commands on the pending tracks;
	return SendSetupTrackMessages(pFrom);
}

bool BaseRTSPAppProtocolHandler::HandleRTSPResponse200Setup(
		RTSPProtocol *pFrom, Variant &requestHeaders, string &requestContent,
		Variant &responseHeaders, string &responseContent) {
	if (pFrom->GetCustomParameters()["connectionType"] == "pull") {
		if ((uint32_t) responseHeaders[RTSP_FIRST_LINE][RTSP_STATUS_CODE] != 200) {
			FATAL("request %s failed with response %s",
					STR(requestHeaders.ToString()),
					STR(responseHeaders.ToString()));
			return false;
		}

		if (pFrom->GetCustomParameters()["pendingTracks"].MapSize() != 0) {
			return SendSetupTrackMessages(pFrom);
		}

		//2. Do the play command
		string uri = (string) pFrom->GetCustomParameters()["uri"]["fullUri"];

		//3. prepare the play command
		pFrom->PushRequestFirstLine(RTSP_METHOD_PLAY, uri, RTSP_VERSION_1_0);

		return pFrom->SendRequestMessage();
	} else {
		if (!responseHeaders[RTSP_HEADERS].HasKey(RTSP_HEADERS_TRANSPORT, false)) {
			FATAL("RTSP %s request doesn't have %s header line",
					RTSP_METHOD_SETUP,
					RTSP_HEADERS_TRANSPORT);
			return false;
		}

		//3. get the transport header line
		string raw = responseHeaders[RTSP_HEADERS].GetValue(RTSP_HEADERS_TRANSPORT, false);
		Variant transport;
		if (!SDP::ParseTransportLine(raw, transport)) {
			FATAL("Unable to parse transport line %s", STR(raw));
			return false;
		}
		bool forceTcp = false;
		if (transport.HasKey("server_port")
				&& (transport.HasKey("rtp/avp/udp") || transport.HasKey("rtp/avp"))) {
			forceTcp = false;
		} else if (transport.HasKey("interleaved") && transport.HasKey("rtp/avp/tcp")) {
			forceTcp = true;
		} else {
			FATAL("Invalid transport line: %s", STR(transport.ToString()));
			return false;
		}

		if (forceTcp != (bool)pFrom->GetCustomParameters().GetValue("forceTcp", false)) {
			FATAL("Invalid transport line: %s", STR(transport.ToString()));
			return false;
		}

		OutboundConnectivity *pConnectivity = GetOutboundConnectivity(pFrom, forceTcp);
		if (pConnectivity == NULL) {
			FATAL("Unable to get outbound connectivity");
			return false;
		}

		Variant &params = pFrom->GetCustomParameters();
		if (params["lastSetup"] == "audio") {
			params["audioTransport"] = transport;
		} else {
			params["videoTransport"] = transport;
		}


		Variant &variantUri = params["uri"];

		string trackId = "";
		bool isAudio = false;
		if (params.HasKey("audioTrackId")) {
			trackId = (string) params["audioTrackId"];
			params.RemoveKey("audioTrackId");
			params["lastSetup"] = "audio";
			isAudio = true;
			pConnectivity->HasAudio(true);
		} else {
			if (params.HasKey("videoTrackId")) {
				trackId = (string) params["videoTrackId"];
				params.RemoveKey("videoTrackId");
				params["lastSetup"] = "video";
				pConnectivity->HasVideo(true);
			}
		}

		if (trackId != "") {
			string uri = (string) variantUri["fullUri"] + "/trackID=" + trackId;
			pFrom->PushRequestFirstLine(RTSP_METHOD_SETUP, uri, RTSP_VERSION_1_0);
			string transport = "";
			if (forceTcp) {
				transport = format("RTP/AVP/TCP;unicast;interleaved=%s;mode=record",
						isAudio ? STR(pConnectivity->GetAudioChannels())
						: STR(pConnectivity->GetVideoChannels()));
			} else {
				transport = format("RTP/AVP;unicast;client_port=%s;mode=record",
						isAudio ? STR(pConnectivity->GetAudioPorts())
						: STR(pConnectivity->GetVideoPorts()));
			}
			pFrom->PushRequestHeader(RTSP_HEADERS_TRANSPORT, transport);
			return pFrom->SendRequestMessage();
		} else {
			pFrom->PushRequestFirstLine(RTSP_METHOD_RECORD,
					(string) variantUri["fullUri"],
					RTSP_VERSION_1_0);
			return pFrom->SendRequestMessage();
		}
	}
}

bool BaseRTSPAppProtocolHandler::HandleRTSPResponse200Play(
		RTSPProtocol *pFrom, Variant &requestHeaders, string &requestContent,
		Variant &responseHeaders, string &responseContent) {
	//1. Get the inbound connectivity
	InboundConnectivity *pConnectivity = pFrom->GetInboundConnectivity();
	if (pConnectivity == NULL) {
		FATAL("Unable to get inbound connectivity");
		return false;
	}

	//2. Create the stream
	if (!pConnectivity->Initialize()) {
		FATAL("Unable to initialize inbound connectivity");
		return false;
	}

	//3. Enable keep alive
	return pFrom->EnableKeepAlive(10, pFrom->GetCustomParameters()["uri"]["fullUri"]);
}

bool BaseRTSPAppProtocolHandler::HandleRTSPResponse200Announce(RTSPProtocol *pFrom, Variant &requestHeaders,
		string &requestContent, Variant &responseHeaders,
		string &responseContent) {
	bool forceTcp = (bool)pFrom->GetCustomParameters().GetValue("forceTcp", false);
	OutboundConnectivity *pConnectivity = GetOutboundConnectivity(pFrom, forceTcp);
	if (pConnectivity == NULL) {
		FATAL("Unable to get outbound connectivity");
		return false;
	}

	Variant &params = pFrom->GetCustomParameters();
	string trackId = "";
	bool isAudio = false;
	if (params.HasKey("audioTrackId")) {
		trackId = (string) params["audioTrackId"];
		params.RemoveKey("audioTrackId");
		params["lastSetup"] = "audio";
		isAudio = true;
		pConnectivity->HasAudio(true);
	} else {
		if (params.HasKey("videoTrackId")) {
			trackId = (string) params["videoTrackId"];
			params.RemoveKey("videoTrackId");
			params["lastSetup"] = "video";
			pConnectivity->HasVideo(true);
		}
	}

	if (trackId != "") {
		Variant &variantUri = params["uri"];
		string uri = (string) variantUri["fullUri"] + "/trackID=" + trackId;
		pFrom->PushRequestFirstLine(RTSP_METHOD_SETUP, uri, RTSP_VERSION_1_0);
		string transport = "";
		if (forceTcp) {
			transport = format("RTP/AVP/TCP;unicast;interleaved=%s;mode=record",
					isAudio ? STR(pConnectivity->GetAudioChannels())
					: STR(pConnectivity->GetVideoChannels()));
		} else {
			transport = format("RTP/AVP;unicast;client_port=%s;mode=record",
					isAudio ? STR(pConnectivity->GetAudioPorts())
					: STR(pConnectivity->GetVideoPorts()));
		}
		pFrom->PushRequestHeader(RTSP_HEADERS_TRANSPORT, transport);
		return pFrom->SendRequestMessage();
	} else {
		FATAL("Bogus RTSP connection");
		pFrom->EnqueueForDelete();
		return false;
	}
}

bool BaseRTSPAppProtocolHandler::HandleRTSPResponse200Record(RTSPProtocol *pFrom,
		Variant &requestHeaders, string &requestContent, Variant &responseHeaders,
		string &responseContent) {
	bool forceTcp = (bool)pFrom->GetCustomParameters().GetValue("forceTcp", false);

	OutboundConnectivity *pConnectivity = GetOutboundConnectivity(pFrom, forceTcp);
	if (pConnectivity == NULL) {
		FATAL("Unable to get outbound connectivity");
		return false;
	}

	bool result = false;

	Variant &params = pFrom->GetCustomParameters();
	if (params.HasKey("audioTransport")) {
		if (forceTcp) {
			if (!pConnectivity->RegisterTCPAudioClient(pFrom->GetId(),
					(uint8_t) params["audioTransport"]["interleaved"]["data"],
					(uint8_t) params["audioTransport"]["interleaved"]["rtcp"])) {
				FATAL("Unable to register audio stream");
				return false;
			}
		} else {
			sockaddr_in dataAddress = ((TCPCarrier *) pFrom->GetIOHandler())->GetFarEndpointAddress();
			dataAddress.sin_port = EHTONS((uint16_t) params["audioTransport"]["server_port"]["data"]);
			sockaddr_in rtcpAddress = ((TCPCarrier *) pFrom->GetIOHandler())->GetFarEndpointAddress();
			rtcpAddress.sin_port = EHTONS((uint16_t) params["audioTransport"]["server_port"]["rtcp"]);
			if (!pConnectivity->RegisterUDPAudioClient(pFrom->GetId(),
					dataAddress, rtcpAddress)) {
				FATAL("Unable to register audio stream");
				return false;
			}
		}
		result |= true;
	}

	if (params.HasKey("videoTransport")) {
		if (forceTcp) {
			if (!pConnectivity->RegisterTCPVideoClient(pFrom->GetId(),
					(uint8_t) params["videoTransport"]["interleaved"]["data"],
					(uint8_t) params["videoTransport"]["interleaved"]["rtcp"])) {
				FATAL("Unable to register audio stream");
				return false;
			}
		} else {
			sockaddr_in dataAddress = ((TCPCarrier *) pFrom->GetIOHandler())->GetFarEndpointAddress();
			dataAddress.sin_port = EHTONS((uint16_t) params["videoTransport"]["server_port"]["data"]);
			sockaddr_in rtcpAddress = ((TCPCarrier *) pFrom->GetIOHandler())->GetFarEndpointAddress();
			rtcpAddress.sin_port = EHTONS((uint16_t) params["videoTransport"]["server_port"]["rtcp"]);
			if (!pConnectivity->RegisterUDPVideoClient(pFrom->GetId(),
					dataAddress, rtcpAddress)) {
				FATAL("Unable to register audio stream");
				return false;
			}
			result |= true;
		}
	}

	return result;
}

bool BaseRTSPAppProtocolHandler::HandleRTSPResponse404Play(RTSPProtocol *pFrom, Variant &requestHeaders,
		string &requestContent, Variant &responseHeaders,
		string &responseContent) {
	FATAL("PLAY: Resource not found: %s", STR(requestHeaders[RTSP_FIRST_LINE][RTSP_URL]));
	return false;
}

bool BaseRTSPAppProtocolHandler::HandleRTSPResponse404Describe(RTSPProtocol *pFrom, Variant &requestHeaders,
		string &requestContent, Variant &responseHeaders,
		string &responseContent) {
	FATAL("DESCRIBE: Resource not found: %s", STR(requestHeaders[RTSP_FIRST_LINE][RTSP_URL]));
	return false;
}

bool BaseRTSPAppProtocolHandler::TriggerPlayOrAnnounce(RTSPProtocol *pFrom) {
	//1. Save the URL in the custom parameters
	string uri = (string) pFrom->GetCustomParameters()["uri"]["fullUri"];

	//2. prepare the options command
	pFrom->PushRequestFirstLine(RTSP_METHOD_OPTIONS, uri, RTSP_VERSION_1_0);

	//3. Send it
	if (!pFrom->SendRequestMessage()) {
		FATAL("Unable to send the %s request", RTSP_METHOD_OPTIONS);
		return false;
	}

	//4. Done
	return true;
}

bool BaseRTSPAppProtocolHandler::NeedAuthentication(RTSPProtocol *pFrom,
		Variant &requestHeaders, string &requestContent) {
	//by default, authenticate everything except OPTIONS
	string method = requestHeaders[RTSP_FIRST_LINE][RTSP_METHOD];
	return (method != RTSP_METHOD_OPTIONS);
}

string BaseRTSPAppProtocolHandler::GetAuthenticationRealm(RTSPProtocol *pFrom,
		Variant &requestHeaders, string &requestContent) {
	//by default, just return the first realm
	if (_realms.MapSize() != 0)
		return MAP_KEY(_realms.begin());
	return "";
}

OutboundConnectivity *BaseRTSPAppProtocolHandler::GetOutboundConnectivity(
		RTSPProtocol *pFrom, bool forceTcp) {
	//1. Get the inbound stream
	BaseInNetStream *pInNetStream =
			(BaseInNetStream *) GetApplication()->GetStreamsManager()->FindByUniqueId(
			pFrom->GetCustomParameters()["streamId"]);
	if (pInNetStream == NULL) {
		FATAL("Inbound stream %u not found",
				(uint32_t) pFrom->GetCustomParameters()["streamId"]);
		return NULL;
	}

	//2. Get the outbound connectivity
	OutboundConnectivity *pOutboundConnectivity = pFrom->GetOutboundConnectivity(
			pInNetStream, forceTcp);
	if (pOutboundConnectivity == NULL) {
		FATAL("Unable to get the outbound connectivity");
		return NULL;
	}

	return pOutboundConnectivity;
}

BaseInNetStream *BaseRTSPAppProtocolHandler::GetInboundStream(string streamName) {
	//1. get all the inbound network streams which begins with streamName
	map<uint32_t, BaseStream *> streams = GetApplication()->GetStreamsManager()
			->FindByTypeByName(ST_IN_NET, streamName, true,
			GetApplication()->GetAllowDuplicateInboundNetworkStreams());
	if (streams.size() == 0)
		return NULL;

	//2. Get the fisrt value and see if it is compatible
	BaseInNetStream * pResult = (BaseInNetStream *) MAP_VAL(streams.begin());
	if (!pResult->IsCompatibleWithType(ST_OUT_NET_RTP)) {
		FATAL("The stream %s is not compatible with stream type %s",
				STR(streamName), STR(tagToString(ST_OUT_NET_RTP)));
		return NULL;
	}

	//2. Done
	return pResult;
}

StreamCapabilities *BaseRTSPAppProtocolHandler::GetInboundStreamCapabilities(
		string streamName) {
	BaseInNetStream *pInboundStream = GetInboundStream(streamName);
	if (pInboundStream == NULL) {
		FATAL("Stream %s not found", STR(streamName));
		return NULL;
	}

	return pInboundStream->GetCapabilities();
}

string BaseRTSPAppProtocolHandler::GetAudioTrack(RTSPProtocol *pFrom,
		StreamCapabilities *pCapabilities) {
	pFrom->GetCustomParameters()["audioTrackId"] = "1"; //md5(format("A%u%s",pFrom->GetId(), STR(generateRandomString(4))), true);
	string result = "";
	if (pCapabilities->audioCodecId == CODEC_AUDIO_AAC) {
		result += "m=audio 0 RTP/AVP 96\r\n";
		result += "a=recvonly\r\n";
		result += format("a=rtpmap:96 mpeg4-generic/%u/2\r\n",
				pCapabilities->aac._sampleRate);
		//FINEST("result: %s", STR(result));
		result += "a=control:trackID="
				+ (string) pFrom->GetCustomParameters()["audioTrackId"] + "\r\n";
		//rfc3640-fmtp-explained.txt Chapter 4.1
		result += format("a=fmtp:96 streamtype=5; profile-level-id=15; mode=AAC-hbr; %s; SizeLength=13; IndexLength=3; IndexDeltaLength=3;\r\n",
				STR(pCapabilities->aac.GetRTSPFmtpConfig()));
	} else {
		WARN("Unsupported audio codec: %s", STR(tagToString(pCapabilities->audioCodecId)));
	}
	return result;
}

string BaseRTSPAppProtocolHandler::GetVideoTrack(RTSPProtocol *pFrom,
		StreamCapabilities *pCapabilities) {
	pFrom->GetCustomParameters()["videoTrackId"] = "2"; //md5(format("V%u%s",pFrom->GetId(), STR(generateRandomString(4))), true);
	string result = "";
	if (pCapabilities->videoCodecId == CODEC_VIDEO_AVC) {
		result += "m=video 0 RTP/AVP 97\r\n";
		result += "a=recvonly\r\n";
		result += "a=control:trackID="
				+ (string) pFrom->GetCustomParameters()["videoTrackId"] + "\r\n";
		result += "a=rtpmap:97 H264/90000\r\n";
		result += "a=fmtp:97 profile-level-id=";
		result += format("%02hhX%02hhX%02hhX",
				pCapabilities->avc._pSPS[1],
				pCapabilities->avc._pSPS[2],
				pCapabilities->avc._pSPS[3]);
		result += "; packetization-mode=1; sprop-parameter-sets=";
		result += b64(pCapabilities->avc._pSPS,
				pCapabilities->avc._spsLength) + ",";
		result += b64(pCapabilities->avc._pPPS,
				pCapabilities->avc._ppsLength) + "\r\n";
	} else {

		WARN("Unsupported video codec: %s", STR(tagToString(pCapabilities->videoCodecId)));
	}
	return result;
}

bool BaseRTSPAppProtocolHandler::SendSetupTrackMessages(RTSPProtocol *pFrom) {
	//1. Get the pending tracks
	if (pFrom->GetCustomParameters()["pendingTracks"].MapSize() == 0) {
		WARN("No more tracks");
		return true;
	}

	//2. Get the inbound connectivity
	InboundConnectivity *pConnectivity = pFrom->GetInboundConnectivity();
	if (pConnectivity == NULL) {
		FATAL("Unable to get inbound connectivity");
		return false;
	}

	//3. Get the first pending track
	Variant track = MAP_VAL(pFrom->GetCustomParameters()["pendingTracks"].begin());
	if (track != V_MAP) {
		FATAL("Invalid track");
		return false;
	}

	//4. Add the track to the inbound connectivity
	if (!pConnectivity->AddTrack(track, (bool)track["isAudio"])) {
		FATAL("Unable to add the track to inbound connectivity");
		return false;
	}

	//6. Prepare the SETUP request
	pFrom->PushRequestFirstLine(RTSP_METHOD_SETUP,
			SDP_VIDEO_CONTROL_URI(track), RTSP_VERSION_1_0);
	pFrom->PushRequestHeader(RTSP_HEADERS_TRANSPORT,
			pConnectivity->GetTransportHeaderLine((bool)track["isAudio"], true));

	//7. Remove the track from pending
	pFrom->GetCustomParameters()["pendingTracks"].RemoveKey(
			MAP_KEY(pFrom->GetCustomParameters()["pendingTracks"].begin()));

	//8. Send the request message
	return pFrom->SendRequestMessage();
}

bool BaseRTSPAppProtocolHandler::ParseUsersFile() {
	//1. get the modification date
	double modificationDate = getFileModificationDate(_usersFile);
	if (modificationDate == 0) {
		FATAL("Unable to get last modification date for file %s", STR(_usersFile));
		_realms.Reset();
		return false;
	}

	//2. Do we need to re-parse everything?
	if (modificationDate == _lastUsersFileUpdate)
		return true;

	//3. Reset realms
	_realms.Reset();

	Variant users;
	//4. Read users
	if (!ReadLuaFile(_usersFile, "users", users)) {
		FATAL("Unable to read users file: `%s`", STR(_usersFile));
		_realms.Reset();
		return false;
	}

	FOR_MAP(users, string, Variant, i) {
		if ((VariantType) MAP_VAL(i) != V_STRING) {
			FATAL("Invalid user detected");
			_realms.Reset();
			return false;
		}
	}

	//5. read the realms
	Variant realms;
	if (!ReadLuaFile(_usersFile, "realms", realms)) {
		FATAL("Unable to read users file: `%s`", STR(_usersFile));
		_realms.Reset();
		return false;
	}

	if (realms != V_MAP) {
		FATAL("Invalid users file. Realms section is bogus: `%s`", STR(_usersFile));
		_realms.Reset();
		return false;
	}

	FOR_MAP(realms, string, Variant, i) {
		Variant &realm = MAP_VAL(i);
		if ((!realm.HasKeyChain(V_STRING, true, 1, "name"))
				|| ((string) realm["name"] == "")
				|| (!realm.HasKeyChain(V_STRING, true, 1, "method"))
				|| (((string) realm["method"] != "Basic") && ((string) realm["method"] != "Digest"))
				|| (!realm.HasKeyChain(V_MAP, true, 1, "users"))
				|| (realm["users"].MapSize() == 0)) {
			FATAL("Invalid users file. Realms section is bogus: `%s`", STR(_usersFile));
			_realms.Reset();
			return false;
		}
		_realms[realm["name"]]["name"] = realm["name"];
		_realms[realm["name"]]["method"] = realm["method"];

		FOR_MAP(realm["users"], string, Variant, i) {
			if (!users.HasKey(MAP_VAL(i))) {
				FATAL("Invalid users file. Realms section is bogus: `%s`", STR(_usersFile));
				_realms.Reset();
				return false;
			}
			_realms[realm["name"]]["users"][MAP_VAL(i)] = users[MAP_VAL(i)];
		}
	}

	_lastUsersFileUpdate = modificationDate;
	return true;
}

bool BaseRTSPAppProtocolHandler::SendAuthenticationChallenge(RTSPProtocol *pFrom,
		Variant &realm) {
	//10. Ok, the user doesn't know that this needs authentication. We
	//will respond back with a nice 401. Generate the line first
	string wwwAuthenticate = HTTPAuthHelper::GetWWWAuthenticateHeader(
			realm["method"],
			realm["name"]);

	//12. Save the nonce for later validation when new requests are coming in again
	pFrom->GetCustomParameters()["wwwAuthenticate"] = wwwAuthenticate;

	//13. send the response
	pFrom->PushResponseFirstLine(RTSP_VERSION_1_0, 401, "Unauthorized");
	pFrom->PushResponseHeader(HTTP_HEADERS_WWWAUTHENTICATE, wwwAuthenticate);
	return pFrom->SendResponseMessage();
}

string BaseRTSPAppProtocolHandler::ComputeSDP(RTSPProtocol *pFrom,
		string localStreamName, string targetStreamName, string host) {
	StreamCapabilities *pCapabilities = GetInboundStreamCapabilities(localStreamName);
	if (pCapabilities == NULL) {
		FATAL("Inbound stream %s not found", STR(localStreamName));
		return "";
	}
	string audioTrack = GetAudioTrack(pFrom, pCapabilities);
	string videoTrack = GetVideoTrack(pFrom, pCapabilities);
	if ((audioTrack == "") && (videoTrack == "")) {
		return "";
	}

	string nearAddress = "0.0.0.0";
	string farAddress = "0.0.0.0";
	if ((pFrom->GetIOHandler() != NULL)
			&& (pFrom->GetIOHandler()->GetType() == IOHT_TCP_CARRIER)) {
		nearAddress = ((TCPCarrier *) pFrom->GetIOHandler())->GetNearEndpointAddressIp();
		farAddress = ((TCPCarrier *) pFrom->GetIOHandler())->GetFarEndpointAddressIp();
	}

	if (targetStreamName == "")
		targetStreamName = localStreamName;

	//3. Prepare the body of the response
	string result = "";
	result += "v=0\r\n";
	result += format("o=- %"PRIu32" 0 IN IP4 %s\r\n", pFrom->GetId(), STR(nearAddress));
	result += "s=" + targetStreamName + "\r\n";
	result += "u=http://www.evostream.com\r\n";
	result += "e=contact@evostream.com\r\n";
	result += "c=IN IP4 " + nearAddress + "\r\n";
	result += "t=0 0\r\n";
	result += "a=recvonly\r\n";
	result += audioTrack + videoTrack;

	//FINEST("result:\n%s", STR(result));
	return result;
}

#endif /* HAS_PROTOCOL_RTP */
