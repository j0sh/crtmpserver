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
#include "application/baseclientapplication.h"
#include "protocols/rtmp/outboundrtmpprotocol.h"
#include "protocols/rtmp/messagefactories/connectionmessagefactory.h"
#include "protocols/rtmp/messagefactories/streammessagefactory.h"

RTMPAppProtocolHandler::RTMPAppProtocolHandler(Variant &configuration)
: BaseRTMPAppProtocolHandler(configuration) {
	srand(time(NULL));
	_activeConnections = 0;
}

RTMPAppProtocolHandler::~RTMPAppProtocolHandler() {
}

void RTMPAppProtocolHandler::RegisterProtocol(BaseProtocol *pProtocol) {
	BaseRTMPAppProtocolHandler::RegisterProtocol(pProtocol);
	_activeConnections++;
	SpawnConnections();
}

void RTMPAppProtocolHandler::UnRegisterProtocol(BaseProtocol *pProtocol) {
	BaseRTMPAppProtocolHandler::UnRegisterProtocol(pProtocol);
	_activeConnections--;
	SpawnConnections();
}

void RTMPAppProtocolHandler::SpawnConnections() {
	string targetServer = _configuration["targetServer"];
	string targetApp = _configuration["targetApp"];
	uint32_t numberOfConnections = (uint32_t) _configuration["numberOfConnections"];
	if (_activeConnections >= numberOfConnections)
		return;
	bool randomAccessStreams = (bool)_configuration["randomAccessStreams"];
	//    FINEST("targetServer: %s, targetApp: %s; numberOfConnections: %d; randomAccessStreams: %d",
	//            STR(targetServer), STR(targetApp), numberOfConnections, randomAccessStreams);
	string streamName = GetStreamName(_activeConnections, randomAccessStreams);
	//FINEST("streamName: %s", STR(streamName));
	SpawnConnection(targetServer, targetApp, streamName);
}

bool RTMPAppProtocolHandler::OutboundConnectionEstablished(OutboundRTMPProtocol *pFrom) {
	//1. Get our hands on our connect and custom parameters
	Variant &connectParameters = pFrom->GetCustomParameters();
	Variant &customParameters = connectParameters["customParameters"];
	//    FINEST("connectParameters:\n%s", STR(connectParameters.ToString()));
	//    FINEST("customParameters:\n%s", STR(customParameters.ToString()));

	//2. Prepare the connect request
	Variant connectRequest = ConnectionMessageFactory::GetInvokeConnect(
			customParameters["remoteAppName"],
			customParameters["remoteUri"]);
	//FINEST("connectRequest:\n%s", STR(connectRequest.ToString()));

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
	Variant playRequest = StreamMessageFactory::GetInvokePlay(3, streamId,
			customParameters["remoteStreamName"], -2, -1);
	FINEST("playRequest:\n%s", STR(playRequest.ToString()));

	//7. Send it
	if (!SendRTMPMessage(pFrom, playRequest, true)) {
		FATAL("Unable to send request:\n%s", STR(playRequest.ToString()));
		return false;
	}

	//8. Done
	return true;
}

string RTMPAppProtocolHandler::GetStreamName(uint32_t index, bool randomAccessStreams) {
	if (randomAccessStreams) {
		return _configuration["streams"][rand() % _configuration["streams"].MapSize()];
	} else {
		return _configuration["streams"][index % _configuration["streams"].MapSize()];
	}
}

bool RTMPAppProtocolHandler::SpawnConnection(string targetServer, string targetApp, string streamName) {
	//1. Prepare our custom parameters
	Variant customParameters;
	customParameters["remoteAppName"] = targetApp;
	customParameters["remoteUri"] = format("rtmp://%s", STR(targetServer));
	customParameters["remoteStreamName"] = streamName;

	//2. Prepare the final connect parameters
	Variant parameters;
	parameters[CONF_APPLICATION_NAME] = GetApplication()->GetName();
	parameters[CONF_PROTOCOL] = CONF_PROTOCOL_OUTBOUND_RTMP;
	parameters["customParameters"] = customParameters;

	INFO("Initiate initiate connectin to server rtmp://%s/%s/%s",
			STR(targetServer),
			STR(targetApp),
			STR(streamName));

	//3. Initiate the connect routine
	return OutboundRTMPProtocol::Connect(targetServer, 1935, parameters);
}
#endif /* HAS_PROTOCOL_RTMP */

