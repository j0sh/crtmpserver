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


#ifdef HAS_PROTOCOL_RTMP
#include "rtmpappprotocolhandler.h"
#include "protocols/rtmp/messagefactories/messagefactories.h"
#include "protocols/rtmp/outboundrtmpprotocol.h"
#include "streaming/baseinstream.h"
#include "streaming/baseoutnetstream.h"
#include "application/baseclientapplication.h"

RTMPAppProtocolHandler::RTMPAppProtocolHandler(Variant &configuration)
: BaseRTMPAppProtocolHandler(configuration) {

}

RTMPAppProtocolHandler::~RTMPAppProtocolHandler() {
}

bool RTMPAppProtocolHandler::OutboundConnectionEstablished(OutboundRTMPProtocol *pFrom) {
	//1. Get our hands on our connect and custom parameters
	Variant &connectParameters = pFrom->GetCustomParameters();
	Variant &customParameters = connectParameters["customParameters"];
	//    FINEST("connectParameters:\n%s", STR(connectParameters.ToString()));
	//    FINEST("customParameters:\n%s", STR(customParameters.ToString()));

	//2. Prepare the connect request
	Variant connectRequest = ConnectionMessageFactory::GetInvokeConnect(
			customParameters["remoteAppName"], //string appName
			customParameters["remoteUri"], //string tcUrl
			0, //double audioCodecs
			0, //double capabilities
			customParameters["remoteUA"], //string flashVer
			false, //bool fPad
			"", //string pageUrl
			customParameters["remoteUri"] //string swfUrl
			//double videoCodecs
			//double videoFunction
			//double objectEncoding
			);

	//3. Send it
	if (!SendRTMPMessage(pFrom, connectRequest, true)) {
		FATAL("Unable to send request:\n%s", STR(connectRequest.ToString()));
		return false;
	}

	//4. Done
	return true;
}

bool RTMPAppProtocolHandler::ProcessInvokeConnectResult(BaseRTMPProtocol *pFrom,
		Variant &request, Variant &response) {
	//1. See if the result is OK or not
	if (M_INVOKE_FUNCTION(response) != RM_INVOKE_FUNCTION_RESULT) {
		FATAL("Connect failed:\n%s", STR(response.ToString()));
		return false;
	}
	if (M_INVOKE_PARAM(response, 1) != V_MAP) {
		FATAL("Connect failed:\n%s", STR(response.ToString()));
		return false;
	}
	if (M_INVOKE_PARAM(response, 1)["code"] != V_STRING) {
		FATAL("Connect failed:\n%s", STR(response.ToString()));
		return false;
	}
	if (M_INVOKE_PARAM(response, 1)["code"] != "NetConnection.Connect.Success") {
		FATAL("Connect failed:\n%s", STR(response.ToString()));
		return false;
	}

	//2. Create the createStream request
	Variant createStreamRequest = StreamMessageFactory::GetInvokeCreateStream();

	//3. Send it
	if (!SendRTMPMessage(pFrom, createStreamRequest, true)) {
		FATAL("Unable to send request:\n%s", STR(createStreamRequest.ToString()));
		return false;
	}

	//4. Done
	return true;
}

bool RTMPAppProtocolHandler::ProcessInvokeCreateStreamResult(BaseRTMPProtocol *pFrom,
		Variant &request, Variant &response) {
	//1. Test and see if this connection is an outbound RTMP connection
	//and get a pointer to it
	if (pFrom->GetType() != PT_OUTBOUND_RTMP) {
		FATAL("This is not an outbound connection");
		return false;
	}
	OutboundRTMPProtocol *pProtocol = (OutboundRTMPProtocol *) pFrom;

	//2. Test the response
	if (M_INVOKE_FUNCTION(response) != RM_INVOKE_FUNCTION_RESULT) {
		FATAL("createStream failed:\n%s", STR(response.ToString()));
		return false;
	}
	if (M_INVOKE_PARAM(response, 1) != _V_NUMERIC) {
		FATAL("createStream failed:\n%s", STR(response.ToString()));
		return false;
	}

	//3. Get the assigned stream ID
	uint32_t streamId = (uint32_t) M_INVOKE_PARAM(response, 1);

	//4. Create the neutral stream
	if (!pProtocol->CreateNeutralStream(streamId)) {
		FATAL("Unable to create neutral stream");
		return false;
	}

	//5. Get our hands on our connect and custom parameters
	Variant &connectParameters = pFrom->GetCustomParameters();
	Variant &customParameters = connectParameters["customParameters"];

	//6. Create the publish request
	Variant publishRequest = StreamMessageFactory::GetInvokePublish(3, streamId,
			customParameters["remoteStreamName"], "live");
	//FINEST("publishRequest:\n%s", STR(publishRequest.ToString()));

	//7. Send it
	if (!SendRTMPMessage(pFrom, publishRequest, true)) {
		FATAL("Unable to send request:\n%s", STR(publishRequest.ToString()));
		return false;
	}

	//8. Done
	return true;
}

bool RTMPAppProtocolHandler::ProcessInvokeOnStatus(BaseRTMPProtocol *pFrom,
		Variant &request) {
	//1. Test and see if this connection is an outbound RTMP connection
	//and get a pointer to it
	if (pFrom->GetType() != PT_OUTBOUND_RTMP) {
		FATAL("This is not an outbound connection");
		return false;
	}
	OutboundRTMPProtocol *pProtocol = (OutboundRTMPProtocol *) pFrom;

	//2. Validate the request
	if (M_INVOKE_PARAM(request, 1) != V_MAP) {
		FATAL("invalid onStatus:\n%s", STR(request.ToString()));
		return false;
	}
	if (M_INVOKE_PARAM(request, 1)["code"] != V_STRING) {
		FATAL("invalid onStatus:\n%s", STR(request.ToString()));
		return false;
	}
	if (M_INVOKE_PARAM(request, 1)["code"] != "NetStream.Publish.Start") {
		FATAL("invalid onStatus:\n%s", STR(request.ToString()));
		return false;
	}

	//3. Get our hands on our connect and custom parameters
	Variant &connectParameters = pFrom->GetCustomParameters();
	Variant &customParameters = connectParameters["customParameters"];

	//4. Find the inbound stream
	BaseInStream *pBaseInStream =
			(BaseInStream *) GetApplication()->GetStreamsManager()->FindByUniqueId(
			(uint32_t) customParameters["streamId"]);
	if (pBaseInStream == NULL) {
		FATAL("Unable to find the inbound stream with id %u",
				(uint32_t) customParameters["streamId"]);
		return false;
	}

	//5. Create the network outbound stream
	BaseOutNetRTMPStream *pBaseOutNetRTMPStream = pProtocol->CreateONS(
			VH_SI(request),
			pBaseInStream->GetName(), pBaseInStream->GetType());
	if (pBaseOutNetRTMPStream == NULL) {
		FATAL("Unable to create outbound stream");
		return false;
	}

	//6. Link and return
	return pBaseInStream->Link((BaseOutNetStream*) pBaseOutNetRTMPStream);
}

bool RTMPAppProtocolHandler::ProcessPeerBW(BaseRTMPProtocol *pFrom,
		Variant &request) {
	return true;
}

bool RTMPAppProtocolHandler::ProcessInvokeOnBWDone(BaseRTMPProtocol *pFrom,
		Variant &request) {
	return true;
}
#endif /* HAS_PROTOCOL_RTMP */

