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

BaseRTSPAppProtocolHandler::BaseRTSPAppProtocolHandler(Variant &configuration)
: BaseAppProtocolHandler(configuration) {

}

BaseRTSPAppProtocolHandler::~BaseRTSPAppProtocolHandler() {
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

	//5. Start play
	if (!Play(pRTSPProtocol)) {
		FATAL("Unable to initiate play on uri %s",
				STR(parameters["url"]));
		pRTSPProtocol->EnqueueForDelete();
		return;
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
	customParameters["uri"] = uri.ToVariant();

	//3. Connect
	if (!TCPConnector<BaseRTSPAppProtocolHandler>::Connect(
			uri.ip,
			uri.port,
			chain, customParameters)) {
		FATAL("Unable to connect to %s:%hu",
				STR(customParameters["uri"]["ip"]),
				(uint16_t) customParameters["uri"]["port"]);
		return false;
	}

	//4. Done
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
	string requestSessionId = "";
	string connectionSessionId = "";
	if (!requestHeaders[RTSP_HEADERS].HasKey(RTSP_HEADERS_CSEQ, false)) {
		FATAL("Request doesn't have %s:\n%s", RTSP_HEADERS_CSEQ,
				STR(requestHeaders.ToString()));
		return false;
	}
	if (requestHeaders[RTSP_HEADERS].HasKey(RTSP_HEADERS_SESSION, false)) {
		requestSessionId = (string) requestHeaders[RTSP_HEADERS].GetValue(
				RTSP_HEADERS_SESSION, false);
	}
	if (pFrom->GetCustomParameters().HasKey(RTSP_HEADERS_SESSION)) {
		connectionSessionId = (string) pFrom->GetCustomParameters()[RTSP_HEADERS_SESSION];
	}
	if (requestSessionId != connectionSessionId) {
		FATAL("Invalid session ID. Wanted: `%s`; Got: `%s`",
				STR(connectionSessionId),
				STR(requestSessionId));
		return false;
	}

	pFrom->ClearResponseMessage();
	pFrom->PushResponseHeader(RTSP_HEADERS_CSEQ,
			requestHeaders[RTSP_HEADERS].GetValue(RTSP_HEADERS_CSEQ, false));

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
	} else {
		FATAL("Method not implemented yet:\n%s", STR(requestHeaders.ToString()));
		return false;
	}

	if (pFrom->GetCustomParameters().HasKey(RTSP_HEADERS_SESSION)) {
		pFrom->PushResponseHeader(RTSP_HEADERS_SESSION,
				pFrom->GetCustomParameters()[RTSP_HEADERS_SESSION]);
	}

	return true;
}

bool BaseRTSPAppProtocolHandler::HandleRTSPResponse(RTSPProtocol *pFrom,
		Variant &responseHeaders, string &responseContent) {
	//1. Test the sequence
	if (!responseHeaders[RTSP_HEADERS].HasKey(RTSP_HEADERS_CSEQ, false)) {
		FATAL("Invalid response:\n%s", STR(responseHeaders.ToString()));
		return false;
	}
	if ((string) responseHeaders[RTSP_HEADERS].GetValue(RTSP_HEADERS_CSEQ, false) != format("%u",
			pFrom->LastRequestSequence())) {
		FATAL("Invalid response sequence. Wanted: %s; Got: %s",
				STR(format("%u", pFrom->LastRequestSequence())),
				STR(responseHeaders[RTSP_HEADERS].GetValue(RTSP_HEADERS_CSEQ, false)));
		return false;
	}

	//2. Get the request, get the response and call the stack further
	return HandleRTSPResponse(pFrom,
			pFrom->GetRequestHeaders(),
			pFrom->GetRequestContent(),
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
	if (uri.document == "") {
		FATAL("Inavlid stream name");
		return false;
	}

	//2. Get the inbound stream capabilities
	BaseInNetStream *pInStream = GetInboundStream(uri.document);
	StreamCapabilities *pCapabilities = GetInboundStreamCapabilities(uri.document);
	if (pCapabilities == NULL) {
		FATAL("Inbound stream %s not found", STR(uri.document));
		return false;
	}

	//3. Prepare the body of the response
	string outboundContent = "";
	outboundContent += "v=0\r\n";
	outboundContent += "o=- 0 0 IN IP4 0.0.0.0\r\n";
	outboundContent += "s=" + uri.document + "\r\n";
	outboundContent += "u=http://www.rtmpd.com/\r\n";
	outboundContent += "e=crtmpserver@gmail.com\r\n";
	outboundContent += "c=IN IP4 0.0.0.0\r\n";
	outboundContent += "t=0 0\r\n";
	outboundContent += "a=recvonly\r\n";
	outboundContent += GetAudioTrack(pFrom, pCapabilities);
	outboundContent += GetVideoTrack(pFrom, pCapabilities);

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

	//2. Get the outbound connectivity
	OutboundConnectivity *pOutboundConnectivity = GetOutboundConnectivity(pFrom);
	if (pOutboundConnectivity == NULL) {
		FATAL("Unable to get the outbound connectivity");
		return false;
	}

	//3. get the transport header line
	string raw = requestHeaders[RTSP_HEADERS].GetValue(RTSP_HEADERS_TRANSPORT, false);
	vector<string> parts;
	split(raw, ";", parts);

	bool RTP_AVP_UDP_found = false;
	bool unicast_found = false;
	bool client_port_found = false;
	uint16_t dataPortNumber = 0;
	uint16_t rtcpPortNumber = 0;
	string clientPorts;

	for (uint32_t i = 0; i < parts.size(); i++) {
		if ((parts[i] == "RTP/AVP/UDP") || (parts[i] == "RTP/AVP")) {
			RTP_AVP_UDP_found = true;
			continue;
		} else if (parts[i] == "unicast") {
			unicast_found = true;
			continue;
		} else if (parts[i].find("client_port=") == 0) {
			vector<string> client_port_parts;
			split(parts[i], "=", client_port_parts);
			if (client_port_parts.size() != 2) {
				WARN("Invalid client_port transport part: %s", STR(parts[i]));
				continue;
			}
			vector<string> ports;
			split(client_port_parts[1], "-", ports);
			if (ports.size() != 2) {
				WARN("Invalid client_port transport part: %s", STR(parts[i]));
				continue;
			}

			dataPortNumber = atoi(STR(ports[0]));
			if (format("%hu", dataPortNumber) != ports[0]) {
				WARN("Invalid client_port transport part: %s", STR(parts[i]));
				continue;
			}
			rtcpPortNumber = atoi(STR(ports[1]));
			if (format("%hu", rtcpPortNumber) != ports[1]) {
				WARN("Invalid client_port transport part: %s", STR(parts[i]));
				continue;
			}
			clientPorts = client_port_parts[1];

			client_port_found = true;
			continue;
		}
	}

	//4. If something is missing, abort
	if (!RTP_AVP_UDP_found) {
		FATAL("RTP/AVP/UDP or RTP/AVP transport part not found: %s", STR(raw));
		return false;
	}
	if (!unicast_found) {
		FATAL("unicast transport part not found: %s", STR(raw));
		return false;
	}
	if (!client_port_found) {
		FATAL("client_port transport part not found: %s", STR(raw));
		return false;
	}

	//5. Find out if this is audio or video
	bool isAudioTrack = false;

	if (((string) requestHeaders[RTSP_FIRST_LINE][RTSP_URL]).find((string) pFrom->GetCustomParameters()["audioTrackId"]) != string::npos) {
		isAudioTrack = true;
	} else if (((string) requestHeaders[RTSP_FIRST_LINE][RTSP_URL]).find((string) pFrom->GetCustomParameters()["videoTrackId"]) != string::npos) {
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
		pFrom->GetCustomParameters()["audioDataPortNumber"] = dataPortNumber;
		pFrom->GetCustomParameters()["audioRtcpPortNumber"] = rtcpPortNumber;
		pFrom->GetCustomParameters()["audioTrackUri"] = requestHeaders[RTSP_FIRST_LINE][RTSP_URL];
		pOutboundConnectivity->HasAudio(true);
	} else {
		pFrom->GetCustomParameters()["videoDataPortNumber"] = dataPortNumber;
		pFrom->GetCustomParameters()["videoRtcpPortNumber"] = rtcpPortNumber;
		pFrom->GetCustomParameters()["videoTrackUri"] = requestHeaders[RTSP_FIRST_LINE][RTSP_URL];
		pOutboundConnectivity->HasVideo(true);
	}

	//10. Create a session
	if (!pFrom->GetCustomParameters().HasKey(RTSP_HEADERS_SESSION))
		pFrom->GetCustomParameters()[RTSP_HEADERS_SESSION] = generateRandomString(8);

	//10 Compose the response
	pFrom->PushResponseFirstLine(RTSP_VERSION_1_0, 200, "OK");
	pFrom->PushResponseHeader(RTSP_HEADERS_SESSION, pFrom->GetCustomParameters()[RTSP_HEADERS_SESSION]);
	pFrom->PushResponseHeader(RTSP_HEADERS_TRANSPORT,
			format("RTP/AVP/UDP;unicast;source=%s;client_port=%s;server_port=%s;ssrc=%08x",
			STR(((TCPCarrier *) pFrom->GetIOHandler())->GetNearEndpointAddressIp()),
			STR(clientPorts),
			isAudioTrack ? STR(pOutboundConnectivity->GetAudioServerPorts())
			: STR(pOutboundConnectivity->GetVideoServerPorts()),
			pOutboundConnectivity->GetSSRC()));

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
	string transportLine = lowercase(requestHeaders[RTSP_HEADERS].GetValue(RTSP_HEADERS_TRANSPORT, false));
	map<string, string> parameters = mapping(transportLine, ";", "=", true);

	//2. Check and see if it has RTP/AVP/TCP,RTP/AVP/UDP or RTP/AVP
	if ((!MAP_HAS1(parameters, "rtp/avp/tcp"))
			&& (!MAP_HAS1(parameters, "rtp/avp/usp"))
			&& (!MAP_HAS1(parameters, "rtp/avp"))) {
		FATAL("Invalid transport line: %s", STR(transportLine));
		return false;
	}

	//3. Check to see if it has either client_port OR interleaved
	if ((!MAP_HAS1(parameters, "client_port"))
			&& (!MAP_HAS1(parameters, "interleaved"))) {
		FATAL("Invalid transport line: %s", STR(transportLine));
		return false;
	}
	if ((MAP_HAS1(parameters, "client_port"))
			&& (MAP_HAS1(parameters, "interleaved"))) {
		FATAL("Invalid transport line: %s", STR(transportLine));
		return false;
	}

	//3. extract the ports/channels and also setup forceTcp
	string portsOrChannelsString = "";
	vector<string> portsOrChannelsParts;
	uint16_t portsOrChannels[2];
	bool forceTcp = false;
	if (MAP_HAS1(parameters, "client_port")) {
		portsOrChannelsString = parameters["client_port"];
		forceTcp = false;
	} else {
		portsOrChannelsString = parameters["interleaved"];
		forceTcp = true;
	}
	pFrom->GetCustomParameters()["forceTcp"] = (bool)forceTcp;
	split(portsOrChannelsString, "-", portsOrChannelsParts);
	if (portsOrChannelsParts.size() != 2) {
		FATAL("Invalid transport line: %s", STR(transportLine));
		return false;
	}
	portsOrChannels[0] = atoi(STR(portsOrChannelsParts[0]));
	portsOrChannels[1] = atoi(STR(portsOrChannelsParts[1]));
	if ((portsOrChannels[0] % 2) != 0) {
		FATAL("Invalid transport line: %s", STR(transportLine));
		return false;
	}
	if ((portsOrChannels[0] + 1) != portsOrChannels[1]) {
		FATAL("Invalid transport line: %s", STR(transportLine));
		return false;
	}

	//4. Find the track inside the pendingTracks ccollection and setup the ports or channels
	if (pFrom->GetCustomParameters()["pendingTracks"] != V_MAP) {
		FATAL("Invalid state. No pending tracks");
		return false;
	}
	string controlUri = requestHeaders[RTSP_FIRST_LINE][RTSP_URL];

	bool trackFound = false;

	FOR_MAP(pFrom->GetCustomParameters()["pendingTracks"], string, Variant, i) {
		if (MAP_VAL(i)["controlUri"] == controlUri) {
			MAP_VAL(i)["portsOrChannels"][(uint32_t) 0] = (uint16_t) portsOrChannels[0];
			MAP_VAL(i)["portsOrChannels"][(uint32_t) 1] = (uint16_t) portsOrChannels[1];
			trackFound = true;
			break;
		}
	}
	if (!trackFound) {
		FATAL("track %s not found", STR(controlUri));
		return false;
	}

	//5. Create a session
	if (!pFrom->GetCustomParameters().HasKey(RTSP_HEADERS_SESSION)) {
		pFrom->GetCustomParameters()[RTSP_HEADERS_SESSION] = generateRandomString(8);
	}

	//6. prepare the response
	pFrom->PushResponseFirstLine(RTSP_VERSION_1_0, 200, "OK");
	pFrom->PushResponseHeader(RTSP_HEADERS_SESSION, pFrom->GetCustomParameters()[RTSP_HEADERS_SESSION]);

	//7. Send it
	return pFrom->SendResponseMessage();
}

bool BaseRTSPAppProtocolHandler::HandleRTSPRequestPlay(RTSPProtocol *pFrom,
		Variant &requestHeaders, string &requestContent) {

	//1. Get the outbound connectivity
	OutboundConnectivity *pOutboundConnectivity = GetOutboundConnectivity(pFrom);
	if (pOutboundConnectivity == NULL) {
		FATAL("Unable to get the outbound connectivity");
		return false;
	}

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
		pOutboundConnectivity->RegisterUDPVideoClient(pFrom->GetId(),
				videoDataAddress, videoRtcpAddress);
	}

	//5. Register the audio
	if (audioDataPortNumber != 0) {
		sockaddr_in audioDataAddress = ((TCPCarrier *) pFrom->GetIOHandler())->GetFarEndpointAddress();
		audioDataAddress.sin_port = EHTONS(audioDataPortNumber);
		sockaddr_in audioRtcpAddress = ((TCPCarrier *) pFrom->GetIOHandler())->GetFarEndpointAddress();
		audioRtcpAddress.sin_port = EHTONS(audioRtcpPortNumber);
		pOutboundConnectivity->RegisterUDPAudioClient(pFrom->GetId(),
				audioDataAddress, audioRtcpAddress);
	}

	//6. prepare the response
	pFrom->PushResponseFirstLine(RTSP_VERSION_1_0, 200, "OK");
	pFrom->PushResponseHeader(RTSP_HEADERS_RANGE, "npt=0.0-");

	string rtpInfoVideo = "";
	string rtpInfoAudio = "";
	if (pFrom->GetCustomParameters().HasKey("videoTrackUri")) {
		rtpInfoVideo = format("url=%s;seq=%u;rtptime=%u",
				STR(pFrom->GetCustomParameters()["videoTrackUri"]),
				pOutboundConnectivity->GetLastVideoSequence(),
				0);
	}
	if (pFrom->GetCustomParameters().HasKey("audioTrackId")) {
		rtpInfoAudio = format("url=%s;seq=%u;rtptime=%u",
				STR(pFrom->GetCustomParameters()["audioTrackId"]),
				pOutboundConnectivity->GetLastAudioSequence(),
				0);
	}
	string rtpInfo = rtpInfoVideo;
	if (rtpInfo != "") {
		if (rtpInfoAudio != "") {
			rtpInfo += ", " + rtpInfoAudio;
		}
	} else {
		rtpInfo = rtpInfoAudio;
	}

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
		FATAL("Invalid DESCRIBE response:\n%s", STR(requestHeaders.ToString()));
		return false;
	}
	if ((string) requestHeaders[RTSP_HEADERS].GetValue(RTSP_HEADERS_CONTENT_TYPE, false)
			!= RTSP_HEADERS_ACCEPT_APPLICATIONSDP) {
		FATAL("Invalid DESCRIBE response:\n%s", STR(requestHeaders.ToString()));
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
	pFrom->GetCustomParameters()["sdpStreamName"] = sdp.GetStreamName();

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

	Variant audioTrack;
	Variant videoTrack;

	FOR_MAP(pFrom->GetCustomParameters()["pendingTracks"], string, Variant, i) {
		if (!MAP_VAL(i).HasKey("portsOrChannels")) {
			FATAL("Not all pending tracks are correctly initialized");
			return false;
		}
		if ((bool)MAP_VAL(i)["isAudio"]) {
			audioTrack = MAP_VAL(i);
		} else {
			videoTrack = MAP_VAL(i);
		}
	}


	//2. Get the stream name
	string streamName = pFrom->GetCustomParameters()["sdpStreamName"];
	if (streamName == "") {
		streamName = format("rtsp_stream_%u", pFrom->GetId());
	}

	//3. Create the inbound connectivity
	if (pFrom->GetInboundConnectivity(videoTrack, audioTrack,
			streamName) == NULL) {
		FATAL("Unable to get the inbound connectivity");
		return false;
	}

	//4. Send back the response
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
	} else {
		FATAL("Response for method %s not implemented yet", STR(method));
		return false;
	}
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
	} else {
		FATAL("Response for method %s not implemented yet", STR(method));
		return false;
	}
}

bool BaseRTSPAppProtocolHandler::HandleRTSPResponse200Options(
		RTSPProtocol *pFrom, Variant &requestHeaders, string &requestContent,
		Variant &responseHeaders, string &responseContent) {
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

	//4. Test the presence of the wanted methods
	if (//(!MAP_HAS1(parts, RTSP_METHOD_OPTIONS)) ||
			(!MAP_HAS1(parts, RTSP_METHOD_DESCRIBE))
			|| (!MAP_HAS1(parts, RTSP_METHOD_SETUP))
			|| (!MAP_HAS1(parts, RTSP_METHOD_PLAY))
			) {
		FATAL("Some of the supported methods are missing: %s", STR(raw));
		return false;
	}

	//5. Prepare the DESCRIBE method
	string url = requestHeaders[RTSP_FIRST_LINE][RTSP_URL];
	pFrom->ClearRequestMessage();
	pFrom->PushRequestFirstLine(RTSP_METHOD_DESCRIBE, url, RTSP_VERSION_1_0);
	pFrom->PushRequestHeader(RTSP_HEADERS_ACCEPT, RTSP_HEADERS_ACCEPT_APPLICATIONSDP);

	//6. Send it

	return pFrom->SendRequestMessage();
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

	//5. Store the tracks inside the session for later use
	if (audioTrack != V_NULL) {
		pFrom->GetCustomParameters()["pendingTracks"][(uint32_t) SDP_TRACK_GLOBAL_INDEX(audioTrack)] = audioTrack;
	}
	if (videoTrack != V_NULL) {
		pFrom->GetCustomParameters()["pendingTracks"][(uint32_t) SDP_TRACK_GLOBAL_INDEX(videoTrack)] = videoTrack;
	}

	//6. Create the inbound connectivity
	if (pFrom->GetInboundConnectivity(videoTrack, audioTrack,
			sdp.GetStreamName()) == NULL) {
		FATAL("Unable to get the inbound connectivity");

		return false;
	}

	//7. Start sending the setup commands on the pending tracks;
	return SendSetupTrackMessages(pFrom, "");
}

bool BaseRTSPAppProtocolHandler::HandleRTSPResponse200Setup(
		RTSPProtocol *pFrom, Variant &requestHeaders, string &requestContent,
		Variant &responseHeaders, string &responseContent) {
	if ((uint32_t) responseHeaders[RTSP_FIRST_LINE][RTSP_STATUS_CODE] != 200) {
		FATAL("request %s failed with response %s",
				STR(requestHeaders.ToString()),
				STR(responseHeaders.ToString()));
		return false;
	}

	if (pFrom->GetCustomParameters()["pendingTracks"].MapSize() != 0) {
		string sessionId = "";
		if (responseHeaders[RTSP_HEADERS].HasKey(RTSP_HEADERS_SESSION, false)) {
			sessionId = (string) responseHeaders[RTSP_HEADERS].GetValue(RTSP_HEADERS_SESSION, false);
		}
		return SendSetupTrackMessages(pFrom, sessionId);
	}

	//2. Do the play command
	string uri = (string) pFrom->GetCustomParameters()["uri"]["fullUri"];

	//3. prepare the play command
	pFrom->ClearRequestMessage();
	pFrom->PushRequestFirstLine(RTSP_METHOD_PLAY, uri, RTSP_VERSION_1_0);
	pFrom->PushRequestHeader(RTSP_HEADERS_SESSION,
			responseHeaders[RTSP_HEADERS].GetValue(RTSP_HEADERS_SESSION, false));

	//4. Done

	return pFrom->SendRequestMessage();
}

bool BaseRTSPAppProtocolHandler::HandleRTSPResponse200Play(
		RTSPProtocol *pFrom, Variant &requestHeaders, string &requestContent,
		Variant &responseHeaders, string &responseContent) {
	if ((uint32_t) responseHeaders[RTSP_FIRST_LINE][RTSP_STATUS_CODE] != 200) {
		FATAL("request %s failed with response %s",
				STR(requestHeaders.ToString()),
				STR(responseHeaders.ToString()));

		return false;
	}
	return true;
}

bool BaseRTSPAppProtocolHandler::HandleRTSPResponse404Play(RTSPProtocol *pFrom, Variant &requestHeaders,
		string &requestContent, Variant &responseHeaders,
		string &responseContent) {

	return false;
}

bool BaseRTSPAppProtocolHandler::Play(RTSPProtocol *pFrom) {
	//1. Save the URL in the custom parameters
	string uri = (string) pFrom->GetCustomParameters()["uri"]["fullUri"];

	//2. prepare the options command
	pFrom->ClearRequestMessage();
	pFrom->PushRequestFirstLine(RTSP_METHOD_OPTIONS, uri, RTSP_VERSION_1_0);

	//3. Send it
	if (!pFrom->SendRequestMessage()) {
		FATAL("Unable to send the %s request", RTSP_METHOD_DESCRIBE);

		return false;
	}

	//4. Done
	return true;
}

OutboundConnectivity *BaseRTSPAppProtocolHandler::GetOutboundConnectivity(
		RTSPProtocol *pFrom) {
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
			pInNetStream);
	if (pOutboundConnectivity == NULL) {
		FATAL("Unable to get the outbound connectivity");

		return NULL;
	}

	return pOutboundConnectivity;
}

BaseInNetStream *BaseRTSPAppProtocolHandler::GetInboundStream(string streamName) {
	//1. get all the inbound network streams which begins with streamName
	map<uint32_t, BaseStream *> streams = GetApplication()->GetStreamsManager()
			->FindByTypeByName(ST_IN_NET, streamName, true, true);
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
	pFrom->GetCustomParameters()["audioTrackId"] = md5(format("A%u%s",
			pFrom->GetId(), STR(generateRandomString(4))), true);
	string result = "";
	if (pCapabilities->audioCodecId == CODEC_AUDIO_AAC) {
		result += "m=audio 0 RTP/AVP 96\r\n";
		result += "a=recvonly\r\n";
		result += format("a=rtpmap:96 mpeg4-generic/%u/2\r\n",
				pCapabilities->aac._sampleRate);
		FINEST("result: %s", STR(result));
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
	pFrom->GetCustomParameters()["videoTrackId"] = md5(format("V%u%s",
			pFrom->GetId(), STR(generateRandomString(4))), true);
	string result = "";
	if (pCapabilities->videoCodecId == CODEC_VIDEO_AVC) {
		result += "m=video 0 RTP/AVP 97\r\n";
		result += "a=recvonly\r\n";
		result += "a=control:"
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

bool BaseRTSPAppProtocolHandler::SendSetupTrackMessages(RTSPProtocol *pFrom, string sessionId) {
	if (pFrom->GetCustomParameters()["pendingTracks"].MapSize() == 0) {
		WARN("No more tracks");
		return true;
	}
	Variant track = MAP_VAL(pFrom->GetCustomParameters()["pendingTracks"].begin());
	if (track != V_NULL) {
		//6. Prepare the video SETUP request
		pFrom->ClearRequestMessage();
		pFrom->PushRequestFirstLine(RTSP_METHOD_SETUP,
				SDP_VIDEO_CONTROL_URI(track), RTSP_VERSION_1_0);
		pFrom->PushRequestHeader(RTSP_HEADERS_TRANSPORT,
				pFrom->GetTransportHeaderLine(SDP_TRACK_IS_AUDIO(track)));
		if (sessionId != "") {
			pFrom->PushRequestHeader(RTSP_HEADERS_SESSION, sessionId);
		}
		pFrom->GetCustomParameters()["pendingTracks"].RemoveKey(
				MAP_KEY(pFrom->GetCustomParameters()["pendingTracks"].begin()));
		return pFrom->SendRequestMessage();
	}

	return true;
}

#endif /* HAS_PROTOCOL_RTP */
