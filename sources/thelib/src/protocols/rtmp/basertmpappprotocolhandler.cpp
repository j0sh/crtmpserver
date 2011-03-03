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
#include "application/baseclientapplication.h"
#include "protocols/rtmp/basertmpappprotocolhandler.h"
#include "protocols/rtmp/basertmpprotocol.h"
#include "protocols/rtmp/outboundrtmpprotocol.h"
#include "protocols/rtmp/messagefactories/messagefactories.h"
#include "application/clientapplicationmanager.h"
#include "protocols/ts/basetsappprotocolhandler.h"
#include "protocols/rtmp/streaming/baseoutnetrtmpstream.h"
#include "protocols/rtmp/streaming/infilertmpstream.h"
#include "protocols/rtmp/streaming/innetrtmpstream.h"
#include "streaming/streamstypes.h"
#include "streaming/baseinstream.h"
#include "streaming/baseinnetstream.h"

BaseRTMPAppProtocolHandler::BaseRTMPAppProtocolHandler(Variant &configuration)
: BaseAppProtocolHandler(configuration) {
	_validateHandshake = (bool)configuration[CONF_APPLICATION_VALIDATEHANDSHAKE];
	_keyframeSeek = (bool)configuration[CONF_APPLICATION_KEYFRAMESEEK];
	_clientSideBuffer = (int32_t) configuration[CONF_APPLICATION_CLIENTSIDEBUFFER];
	_seekGranularity = (uint32_t) ((double) configuration[CONF_APPLICATION_SEEKGRANULARITY]*1000);
	_mediaFolder = (string) configuration[CONF_APPLICATION_MEDIAFOLDER];
	_renameBadFiles = (bool)configuration[CONF_APPLICATION_RENAMEBADFILES];
	if (_configuration.HasKey(CONF_APPLICATION_AUTH)) {
		if ((!_configuration[CONF_APPLICATION_AUTH].HasKey(CONF_APPLICATION_AUTH_TYPE))
				|| (_configuration[CONF_APPLICATION_AUTH][CONF_APPLICATION_AUTH_TYPE] != V_STRING)
				|| (_configuration[CONF_APPLICATION_AUTH][CONF_APPLICATION_AUTH_TYPE] != CONF_APPLICATION_AUTH_TYPE_ADOBE)
				|| (!_configuration[CONF_APPLICATION_AUTH].HasKey(CONF_APPLICATION_AUTH_ENCODER_AGENTS))
				|| (_configuration[CONF_APPLICATION_AUTH][CONF_APPLICATION_AUTH_ENCODER_AGENTS] != V_MAP)
				|| (_configuration[CONF_APPLICATION_AUTH][CONF_APPLICATION_AUTH_ENCODER_AGENTS].MapSize() == 0)
				|| (!_configuration[CONF_APPLICATION_AUTH].HasKey(CONF_APPLICATION_AUTH_USERS_FILE))
				|| (_configuration[CONF_APPLICATION_AUTH][CONF_APPLICATION_AUTH_USERS_FILE] != V_STRING)
				|| (_configuration[CONF_APPLICATION_AUTH][CONF_APPLICATION_AUTH_USERS_FILE] == "")
				) {
			WARN("Invalid authentication configuration");
			_authMethod = "";
		} else {
			string usersFile = _configuration[CONF_APPLICATION_AUTH][CONF_APPLICATION_AUTH_USERS_FILE];
			if (usersFile[0] != '/') {
				usersFile = (string) _configuration[CONF_APPLICATION_DIRECTORY] + usersFile;
				_configuration[CONF_APPLICATION_AUTH][CONF_APPLICATION_AUTH_USERS_FILE] = usersFile;
			}
			if (!fileExists(_configuration[CONF_APPLICATION_AUTH][CONF_APPLICATION_AUTH_USERS_FILE])) {
				WARN("Invalid authentication configuration. Missing users file: %s",
						STR(_configuration[CONF_APPLICATION_AUTH][CONF_APPLICATION_AUTH_USERS_FILE]));
				_authMethod = "";
			} else {
				_adobeAuthSalt = generateRandomString(32);
				_adobeAuthSettings = _configuration[CONF_APPLICATION_AUTH];
				_authMethod = (string) _configuration[CONF_APPLICATION_AUTH][CONF_APPLICATION_AUTH_TYPE];
			}
		}
	} else {
		_authMethod = "";
	}

	if ((bool)configuration[CONF_APPLICATION_GENERATE_META_FILES]) {
		GenerateMetaFiles();
	}
}

BaseRTMPAppProtocolHandler::~BaseRTMPAppProtocolHandler() {

	FOR_MAP(_connections, uint32_t, BaseRTMPProtocol *, i) {
		MAP_VAL(i)->SetApplication(NULL);
		MAP_VAL(i)->EnqueueForDelete();
	}
}

bool BaseRTMPAppProtocolHandler::ValidateHandshake() {
	return _validateHandshake;
}

SOManager *BaseRTMPAppProtocolHandler::GetSOManager() {
	return &_soManager;
}

void BaseRTMPAppProtocolHandler::RegisterProtocol(BaseProtocol *pProtocol) {
	FINEST("Register protocol %s to application %s",
			STR(*pProtocol), STR(GetApplication()->GetName()));
	if (MAP_HAS1(_connections, pProtocol->GetId()))
		return;
	_connections[pProtocol->GetId()] = (BaseRTMPProtocol *) pProtocol;
	_nextInvokeId[pProtocol->GetId()] = 1;
}

void BaseRTMPAppProtocolHandler::UnRegisterProtocol(BaseProtocol *pProtocol) {
	_soManager.UnRegisterProtocol((BaseRTMPProtocol*) pProtocol);
	if (!MAP_HAS1(_connections, pProtocol->GetId())) {
		return;
	}
	_connections.erase(pProtocol->GetId());
	_nextInvokeId.erase(pProtocol->GetId());
	_resultMessageTracking.erase(pProtocol->GetId());
}

bool BaseRTMPAppProtocolHandler::PullExternalStream(URI uri, Variant streamConfig) {
	//1. normalize the stream name
	string streamName = "";
	if (streamConfig["localStreamName"] == V_STRING)
		streamName = (string) streamConfig["localStreamName"];
	trim(streamName);
	if (streamName == "") {
		streamConfig["localStreamName"] = "stream_" + generateRandomString(8);
		WARN("No localstream name for external URI: %s. Defaulted to %s",
				STR(uri.fullUri),
				STR(streamConfig["localStreamName"]));
	}

	//2. Prepare the custom parameters
	Variant parameters;
	parameters["customParameters"]["externalStreamConfig"] = streamConfig;
	parameters[CONF_APPLICATION_NAME] = GetApplication()->GetName();
	if (uri.scheme == "rtmp") {
		parameters[CONF_PROTOCOL] = CONF_PROTOCOL_OUTBOUND_RTMP;
	} else if (uri.scheme == "rtmpt") {
		parameters[CONF_PROTOCOL] = CONF_PROTOCOL_OUTBOUND_RTMPT;
	} else if (uri.scheme == "rtmpe") {
		parameters[CONF_PROTOCOL] = CONF_PROTOCOL_OUTBOUND_RTMPE;
	} else {
		FATAL("scheme %s not supported by RTMP handler", STR(uri.scheme));
		return false;
	}

	//3. start the connecting sequence
	return OutboundRTMPProtocol::Connect(uri.ip, uri.port, parameters);
}

bool BaseRTMPAppProtocolHandler::PushLocalStream(BaseInStream *pInStream, Variant streamConfig) {
	//1. Prepare the custom parameters
	Variant parameters;
	parameters["customParameters"]["localStreamConfig"] = streamConfig;
	parameters["customParameters"]["localStreamConfig"]["localUniqueStreamId"] = pInStream->GetUniqueId();
	parameters[CONF_APPLICATION_NAME] = GetApplication()->GetName();
	if (streamConfig["targetUri"]["scheme"] == "rtmp") {
		parameters[CONF_PROTOCOL] = CONF_PROTOCOL_OUTBOUND_RTMP;
	} else if (streamConfig["targetUri"]["scheme"] == "rtmpt") {
		parameters[CONF_PROTOCOL] = CONF_PROTOCOL_OUTBOUND_RTMPT;
	} else if (streamConfig["targetUri"]["scheme"] == "rtmpe") {
		parameters[CONF_PROTOCOL] = CONF_PROTOCOL_OUTBOUND_RTMPE;
	} else {
		FATAL("scheme %s not supported by RTMP handler",
				STR(streamConfig["targetUri"]["scheme"]));
		return false;
	}

	//2. start the connecting sequence
	return OutboundRTMPProtocol::Connect(
			streamConfig["targetUri"]["ip"],
			(uint16_t) streamConfig["targetUri"]["port"],
			parameters);
}

bool BaseRTMPAppProtocolHandler::OutboundConnectionEstablished(
		OutboundRTMPProtocol *pFrom) {
	if (NeedsToPullExternalStream(pFrom)) {
		return PullExternalStream(pFrom);
	}

	if (NeedsToPushLocalStream(pFrom)) {
		return PushLocalStream(pFrom);
	}

	WARN("You should override BaseRTMPAppProtocolHandler::OutboundConnectionEstablished");
	return false;
}

bool BaseRTMPAppProtocolHandler::AuthenticateInbound(BaseRTMPProtocol *pFrom,
		Variant &request, Variant &authState) {
	if (_authMethod == CONF_APPLICATION_AUTH_TYPE_ADOBE) {
		return AuthenticateInboundAdobe(pFrom, request, authState);
	} else {
		FATAL("Auth scheme not supported: %s", STR(_authMethod));
		return false;
	}
}

bool BaseRTMPAppProtocolHandler::InboundMessageAvailable(BaseRTMPProtocol *pFrom,
		Header &header, IOBuffer &inputBuffer) {
	Variant request;
	if (!_rtmpProtocolSerializer.Deserialize(header, inputBuffer, request)) {
		FATAL("Unable to deserialize message");
		return false;
	}

	return InboundMessageAvailable(pFrom, request);
}

bool BaseRTMPAppProtocolHandler::InboundMessageAvailable(BaseRTMPProtocol *pFrom,
		Variant &request) {
	//FINEST("request:\n%s", STR(request.ToString()));

	//1. Perform authentication
	Variant &parameters = pFrom->GetCustomParameters();
	if (!parameters.HasKey("authState"))
		parameters["authState"].IsArray(false);
	Variant &authState = parameters["authState"];

	switch (pFrom->GetType()) {
		case PT_INBOUND_RTMP:
		{
			if (_authMethod != "") {
				if (!AuthenticateInbound(pFrom, request, authState)) {
					FATAL("Unable to authenticate");
					return false;
				}
			} else {
				authState["stage"] = "authenticated";
				authState["canPublish"] = (bool)true;
				authState["canOverrideStreamName"] = (bool)false;
			}
			break;
		}
		case PT_OUTBOUND_RTMP:
		{
			authState["stage"] = "authenticated";
			authState["canPublish"] = (bool)true;
			authState["canOverrideStreamName"] = (bool)false;
			break;
		}
		default:
		{
			WARN("Invalid protocol type");
			return false;
		}
	}

	if (authState["stage"] == "failed") {
		WARN("Authentication failed");
		return false;
	}

	switch ((uint8_t) VH_MT(request)) {
		case RM_HEADER_MESSAGETYPE_WINACKSIZE:
		{
			return ProcessWinAckSize(pFrom, request);
		}
		case RM_HEADER_MESSAGETYPE_PEERBW:
		{
			return ProcessPeerBW(pFrom, request);
		}
		case RM_HEADER_MESSAGETYPE_ACK:
		{
			return ProcessAck(pFrom, request);
		}
		case RM_HEADER_MESSAGETYPE_CHUNKSIZE:
		{
			return ProcessChunkSize(pFrom, request);
		}
		case RM_HEADER_MESSAGETYPE_USRCTRL:
		{
			return ProcessUsrCtrl(pFrom, request);
		}
		case RM_HEADER_MESSAGETYPE_NOTIFY:
		{
			return ProcessNotify(pFrom, request);
		}
		case RM_HEADER_MESSAGETYPE_FLEXSTREAMSEND:
		{
			return ProcessFlexStreamSend(pFrom, request);
		}
		case RM_HEADER_MESSAGETYPE_INVOKE:
		{
			return ProcessInvoke(pFrom, request);
		}
		case RM_HEADER_MESSAGETYPE_SHAREDOBJECT:
		case RM_HEADER_MESSAGETYPE_FLEXSHAREDOBJECT:
		{
			return ProcessSharedObject(pFrom, request);
		}
		case RM_HEADER_MESSAGETYPE_FLEX:
		{
			return ProcessInvoke(pFrom, request);
		}
		case RM_HEADER_MESSAGETYPE_ABORTMESSAGE:
		{
			return true;
		}
		default:
		{
			FATAL("Request type not yet implemented:\n%s",
					STR(request.ToString()));
			return false;
		}
	}
}

bool BaseRTMPAppProtocolHandler::ProcessWinAckSize(BaseRTMPProtocol *pFrom,
		Variant &request) {
	if (request[RM_WINACKSIZE] != V_UINT32) {
		FATAL("Invalid message: %s", STR(request.ToString()));
		return false;
	}
	uint32_t size = (uint32_t) request[RM_WINACKSIZE];
	if ((size > 4 * 1024 * 1024) || size == 0) {
		FATAL("Invalid message: %s", STR(request.ToString()));
		return false;
	}
	pFrom->SetWinAckSize(request[RM_WINACKSIZE]);
	return true;
}

bool BaseRTMPAppProtocolHandler::ProcessPeerBW(BaseRTMPProtocol *pFrom,
		Variant &request) {
	//WARN("ProcessPeerBW:\n%s", STR(request.ToString()));
	WARN("ProcessPeerBW");
	return true;
}

bool BaseRTMPAppProtocolHandler::ProcessAck(BaseRTMPProtocol *pFrom,
		Variant &request) {
	//WARN("ProcessAck:\n%s", STR(request.ToString()));
	return true;
}

bool BaseRTMPAppProtocolHandler::ProcessChunkSize(BaseRTMPProtocol *pFrom,
		Variant &request) {
	if (request[RM_CHUNKSIZE] != V_UINT32) {
		FATAL("Invalid message: %s", STR(request.ToString()));
		return false;
	}
	uint32_t size = (uint32_t) request[RM_CHUNKSIZE];
	if ((size > 4 * 1024 * 1024) || size == 0) {
		FATAL("Invalid message: %s", STR(request.ToString()));
		return false;
	}
	if (!pFrom->SetInboundChunkSize(size)) {
		FATAL("Unable to set chunk size:\n%s", STR(request.ToString()));
		return false;
	}

	return true;
}

bool BaseRTMPAppProtocolHandler::ProcessUsrCtrl(BaseRTMPProtocol *pFrom,
		Variant &request) {
	switch ((uint16_t) M_USRCTRL_TYPE(request)) {
		case RM_USRCTRL_TYPE_PING_REQUEST:
		{
			//FINEST("request:\n%s", STR(request.ToString()));
			Variant response = ConnectionMessageFactory::GetPong();
			//ASSERT("response:\n%s", STR(response.ToString()));
			return SendRTMPMessage(pFrom, response);
		}
		case RM_USRCTRL_TYPE_STREAM_BEGIN:
		case RM_USRCTRL_TYPE_STREAM_SET_BUFFER_LENGTH:
		case RM_USRCTRL_TYPE_STREAM_IS_RECORDED:
		case RM_USRCTRL_TYPE_PING_RESPONSE:
		{
			WARN("User control message type: %s", STR(M_USRCTRL_TYPE_STRING(request)));
			return true;
		}
		case RM_USRCTRL_TYPE_UNKNOWN1:
		case RM_USRCTRL_TYPE_UNKNOWN2:
		{
			//WARN("User control message type: %s", STR(M_USRCTRL_TYPE_STRING(request)));
			return true;
		}
		default:
		{
			FATAL("Invalid user ctrl:\n%s", STR(request.ToString()));
			return false;
		}
	}
}

bool BaseRTMPAppProtocolHandler::ProcessNotify(BaseRTMPProtocol *pFrom,
		Variant &request) {
	//FINEST("request:\n%s", STR(request.ToString()));
	//1. Find the corresponding inbound stream
	InNetRTMPStream *pInNetRTMPStream = NULL;
	map<uint32_t, BaseStream *> possibleStreams = GetApplication()->
			GetStreamsManager()->FindByProtocolIdByType(pFrom->GetId(), ST_IN_NET_RTMP, false);

	FOR_MAP(possibleStreams, uint32_t, BaseStream *, i) {
		if (((InNetRTMPStream *) MAP_VAL(i))->GetRTMPStreamId() == (uint32_t) VH_SI(request)) {
			pInNetRTMPStream = (InNetRTMPStream *) MAP_VAL(i);
			break;
		}
	}
	if (pInNetRTMPStream == NULL) {
		WARN("No stream found. Searched for %d:%d. Message was:\n%s", pFrom->GetId(),
				(uint32_t) VH_SI(request),
				STR(request.ToString()));
		return true;
	}

	//2. Remove all string values starting with @
	//TODO: Wtf are those!?
	vector<string> removedKeys;

	FOR_MAP(M_NOTIFY_PARAMS(request), string, Variant, i) {
		if ((VariantType) MAP_VAL(i) == V_STRING) {
			if (((string) MAP_VAL(i)).find("@") == 0)
				ADD_VECTOR_END(removedKeys, MAP_KEY(i));
		}
	}

	FOR_VECTOR(removedKeys, i) {

		M_NOTIFY_PARAMS(request).RemoveKey(removedKeys[i]);
	}

	//3. Brodcast the message on the inbound stream
	return pInNetRTMPStream->SendStreamMessage(request);
}

bool BaseRTMPAppProtocolHandler::ProcessFlexStreamSend(BaseRTMPProtocol *pFrom,
		Variant &request) {
	//FINEST("request:\n%s", STR(request.ToString()));

	//1. Find the corresponding inbound stream
	InNetRTMPStream *pInNetRTMPStream = NULL;
	map<uint32_t, BaseStream *> possibleStreams = GetApplication()->
			GetStreamsManager()->FindByProtocolIdByType(pFrom->GetId(), ST_IN_NET_RTMP, false);

	FOR_MAP(possibleStreams, uint32_t, BaseStream *, i) {
		if (((InNetRTMPStream *) MAP_VAL(i))->GetRTMPStreamId() == (uint32_t) VH_SI(request)) {
			pInNetRTMPStream = (InNetRTMPStream *) MAP_VAL(i);
			break;
		}
	}
	if (pInNetRTMPStream == NULL) {
		WARN("No stream found. Searched for %d:%d", pFrom->GetId(),
				(uint32_t) VH_SI(request));
		return true;
	}

	//3. Remove all string values starting with @
	//TODO: Wtf are those!?
	vector<string> removedKeys;

	FOR_MAP(M_FLEXSTREAMSEND_PARAMS(request), string, Variant, i) {
		if ((VariantType) MAP_VAL(i) == V_STRING) {

			if (((string) MAP_VAL(i)).find("@") == 0)
				ADD_VECTOR_END(removedKeys, MAP_KEY(i));
		}
	}

	FOR_VECTOR(removedKeys, i) {

		M_FLEXSTREAMSEND_PARAMS(request).RemoveKey(removedKeys[i]);
	}

	//4. Brodcast the message on the inbound stream
	return pInNetRTMPStream->SendStreamMessage(request);
}

bool BaseRTMPAppProtocolHandler::ProcessSharedObject(BaseRTMPProtocol *pFrom,
		Variant &request) {
	return _soManager.Process(pFrom, request);
}

bool BaseRTMPAppProtocolHandler::ProcessInvoke(BaseRTMPProtocol *pFrom,
		Variant &request) {
	//FINEST("Request:\n%s", STR(request.ToString()));

	string functionName = request[RM_INVOKE][RM_INVOKE_FUNCTION];
	if (functionName == RM_INVOKE_FUNCTION_CONNECT) {
		return ProcessInvokeConnect(pFrom, request);
	} else if (functionName == RM_INVOKE_FUNCTION_CREATESTREAM) {
		return ProcessInvokeCreateStream(pFrom, request);
	} else if (functionName == RM_INVOKE_FUNCTION_PUBLISH) {
		return ProcessInvokePublish(pFrom, request);
	} else if (functionName == RM_INVOKE_FUNCTION_PLAY) {
		return ProcessInvokePlay(pFrom, request);
	} else if (functionName == RM_INVOKE_FUNCTION_PAUSERAW) {
		return ProcessInvokePauseRaw(pFrom, request);
	} else if (functionName == RM_INVOKE_FUNCTION_PAUSE) {
		return ProcessInvokePause(pFrom, request);
	} else if (functionName == RM_INVOKE_FUNCTION_SEEK) {
		return ProcessInvokeSeek(pFrom, request);
	} else if (functionName == RM_INVOKE_FUNCTION_CLOSESTREAM) {
		return ProcessInvokeCloseStream(pFrom, request);
	} else if (functionName == RM_INVOKE_FUNCTION_RELEASESTREAM) {
		return ProcessInvokeReleaseStream(pFrom, request);
	} else if (functionName == RM_INVOKE_FUNCTION_DELETESTREAM) {
		return ProcessInvokeDeleteStream(pFrom, request);
	} else if (functionName == RM_INVOKE_FUNCTION_RESULT) {
		return ProcessInvokeResult(pFrom, request);
	} else if (functionName == RM_INVOKE_FUNCTION_ERROR) {
		return ProcessInvokeResult(pFrom, request);
	} else if (functionName == RM_INVOKE_FUNCTION_ONSTATUS) {
		return ProcessInvokeOnStatus(pFrom, request);
	} else if (functionName == RM_INVOKE_FUNCTION_FCPUBLISH) {
		return ProcessInvokeFCPublish(pFrom, request);
	} else if (functionName == RM_INVOKE_FUNCTION_GETSTREAMLENGTH) {
		return ProcessInvokeGetStreamLength(pFrom, request);
	} else if (functionName == RM_INVOKE_FUNCTION_ONBWDONE) {
		return ProcessInvokeOnBWDone(pFrom, request);
	} else {
		return ProcessInvokeGeneric(pFrom, request);
	}
}

bool BaseRTMPAppProtocolHandler::ProcessInvokeConnect(BaseRTMPProtocol *pFrom,
		Variant & request) {
	//FINEST("\n%s", STR(request.ToString()));
	//1. Send the channel specific messages
	Variant response = GenericMessageFactory::GetWinAckSize(2500000);
	if (!SendRTMPMessage(pFrom, response)) {
		FATAL("Unable to send message to client");
		return false;
	}
	response = GenericMessageFactory::GetPeerBW(2500000, RM_PEERBW_TYPE_DYNAMIC);
	if (!SendRTMPMessage(pFrom, response)) {
		FATAL("Unable to send message to client");
		return false;
	}

	//2. Initialize stream 0
	response = StreamMessageFactory::GetUserControlStreamBegin(0);
	if (!SendRTMPMessage(pFrom, response)) {
		FATAL("Unable to send message to client");
		return false;
	}

	//3. Send the connect result and the onBWDone message
	response = ConnectionMessageFactory::GetInvokeConnectResult(request);
	if (!SendRTMPMessage(pFrom, response)) {
		FATAL("Unable to send message to client");
		return false;
	}
	response = GenericMessageFactory::GetInvokeOnBWDone();
	if (!SendRTMPMessage(pFrom, response)) {
		FATAL("Unable to send message to client");
		return false;
	}

	//4. Done
	return true;
}

bool BaseRTMPAppProtocolHandler::ProcessInvokeCreateStream(BaseRTMPProtocol *pFrom,
		Variant &request) {
	uint32_t id = 0;

	//1. Create the neutral stream
	if (pFrom->CreateNeutralStream(id) == NULL) {
		FATAL("Unable to create stream");
		return false;
	}

	//2. Send the response
	Variant response = StreamMessageFactory::GetInvokeCreateStreamResult(request, id);
	return SendRTMPMessage(pFrom, response);
}

bool BaseRTMPAppProtocolHandler::ProcessInvokePublish(BaseRTMPProtocol *pFrom,
		Variant &request) {
	//1. gather the required data from the request
	if (M_INVOKE_PARAM(request, 1) != V_STRING) {
		FATAL("Invalid request:\n%s", STR(request.ToString()));
		return false;
	}
	string streamName = M_INVOKE_PARAM(request, 1);

	//2. Check to see if we are allowed to create inbound streams
	if (!(bool)pFrom->GetCustomParameters()["authState"]["canPublish"]) {
		Variant response = StreamMessageFactory::GetInvokeOnStatusStreamPublishBadName(request, streamName);
		return pFrom->SendMessage(response);
	}


	bool recording = (M_INVOKE_PARAM(request, 2) == RM_INVOKE_PARAMS_PUBLISH_TYPERECORD);
	bool appending = (M_INVOKE_PARAM(request, 2) == RM_INVOKE_PARAMS_PUBLISH_TYPEAPPEND);
	FINEST("Try to publish stream %s.%s",
			STR(streamName), (recording || appending) ? " Also record/append it" : "");

	//3. Test to see if this stream name is already published somewhere
	map<uint32_t, BaseStream *> existingStreams =
			GetApplication()->GetStreamsManager()->FindByTypeByName(
			ST_IN_NET_RTMP, streamName, false, false);
	if (existingStreams.size() > 0) {
		if (!(bool)pFrom->GetCustomParameters()["authState"]["canOverrideStreamName"]) {
			WARN("Unable to override stream because this connection doesn't have the rights");
			Variant response = StreamMessageFactory::GetInvokeOnStatusStreamPublishBadName(
					request, streamName);
			return pFrom->SendMessage(response);
		} else {

			FOR_MAP(existingStreams, uint32_t, BaseStream *, i) {
				InNetRTMPStream *pTempStream = (InNetRTMPStream *) MAP_VAL(i);
				if (pTempStream->GetProtocol() != NULL) {
					WARN("Overriding stream R%d:U%d with name %s from connection %d",
							pTempStream->GetRTMPStreamId(),
							pTempStream->GetUniqueId(),
							STR(pTempStream->GetName()),
							pTempStream->GetProtocol()->GetId());
					((BaseRTMPProtocol *) pTempStream->GetProtocol())->CloseStream(
							pTempStream->GetRTMPStreamId(), true);
				}
			}
		}
	}

	//4. Create the network inbound stream
	InNetRTMPStream *pInNetRTMPStream = pFrom->CreateINS(VH_CI(request),
			VH_SI(request), streamName);
	if (pInNetRTMPStream == NULL) {
		FATAL("Unable to create inbound stream");
		return false;
	}

	//6. Get the list of waiting subscribers
	map<uint32_t, BaseOutStream *> subscribedOutStreams =
			GetApplication()->GetStreamsManager()->GetWaitingSubscribers(
			streamName, pInNetRTMPStream->GetType());
	FINEST("subscribedOutStreams count: %d", subscribedOutStreams.size());


	//7. Bind the waiting subscribers

	FOR_MAP(subscribedOutStreams, uint32_t, BaseOutStream *, i) {
		BaseOutStream *pBaseOutStream = MAP_VAL(i);
		pBaseOutStream->Link(pInNetRTMPStream);
	}

	//8. Send the streamPublished status message
	if (!pInNetRTMPStream->SendOnStatusStreamPublished()) {
		FATAL("Unable to send OnStatusStreamPublished");
		return false;
	}

	//9. Start recording if necessary
	if (recording || appending) {
		Variant meta = GetMetaData(streamName, false);

		if ((meta[META_MEDIA_TYPE] == MEDIA_TYPE_LIVE_OR_FLV) ||
				(meta[META_MEDIA_TYPE] == MEDIA_TYPE_FLV)) {
			if (!pInNetRTMPStream->RecordFLV(meta, appending)) {
				FATAL("Unable to bind the recording stream");
				return false;
			}
		} else if (meta[META_MEDIA_TYPE] == MEDIA_TYPE_MP4) {
			if (!pInNetRTMPStream->RecordMP4(meta)) {
				FATAL("Unable to bind the recording stream");
				return false;
			}
		}
	}

	//10. Done
	return true;
}

bool BaseRTMPAppProtocolHandler::ProcessInvokeSeek(BaseRTMPProtocol *pFrom,
		Variant & request) {
	//1. Read stream index and offset in millisecond
	uint32_t streamId = VH_SI(request);
	double timeOffset = 0.0;
	if ((VariantType) M_INVOKE_PARAM(request, 1) == V_DOUBLE)
		timeOffset = M_INVOKE_PARAM(request, 1);

	//2. Find the corresponding outbound stream
	BaseOutNetRTMPStream *pOutNetRTMPStream = NULL;
	map<uint32_t, BaseStream *> possibleStreams = GetApplication()->
			GetStreamsManager()->FindByProtocolIdByType(pFrom->GetId(), ST_OUT_NET_RTMP, true);

	FOR_MAP(possibleStreams, uint32_t, BaseStream *, i) {
		if (((BaseOutNetRTMPStream *) MAP_VAL(i))->GetRTMPStreamId() == streamId) {
			pOutNetRTMPStream = (BaseOutNetRTMPStream *) MAP_VAL(i);
			break;
		}
	}
	if (pOutNetRTMPStream == NULL) {
		FATAL("No out stream");
		return false;
	}

	return pOutNetRTMPStream->Seek(timeOffset);
}

bool BaseRTMPAppProtocolHandler::ProcessInvokePlay(BaseRTMPProtocol *pFrom,
		Variant & request) {
	//1. Minimal validation
	if (M_INVOKE_PARAM(request, 1) != V_STRING) {
		FATAL("Invalid request:\n%s", STR(request.ToString()));
		return false;
	}

	//2. Close any streams left open
	if (!pFrom->CloseStream(VH_SI(request), true)) {
		FATAL("Unable to close stream %d:%d", pFrom->GetId(),
				(uint32_t) VH_SI(request));
		return false;
	}

	//3. Gather required data from the request
	string streamName = M_INVOKE_PARAM(request, 1);
	double startTime = -2;
	double length = -1;
	if (M_INVOKE_PARAM(request, 2) == V_DOUBLE)
		startTime = M_INVOKE_PARAM(request, 2);
	if (M_INVOKE_PARAM(request, 3) == V_DOUBLE)
		length = M_INVOKE_PARAM(request, 3);

	if (startTime < 0 && startTime != -2 && startTime != -1)
		startTime = -2;

	if (length < 0 && length != -1)
		length = -1;

	//4. Get the metadata for the stream
	Variant metadata = GetMetaData(streamName, true);
	if (metadata != V_MAP) {
		FATAL("Unable to get metadata");
		return false;
	}

	INFO("Play request for stream name `%s`. Start: %.0f; length: %.0f",
			STR(streamName), startTime, length);

	//6. bind the network outbound stream to the inbound stream
	//depending on the type of the outbound stream
	switch ((int32_t) startTime) {
		case -2: //live or recorded
		{
			bool linked = false;

			//7. try to link to live stream
			if (!TryLinkToLiveStream(pFrom, VH_SI(request), streamName, linked)) {
				FATAL("Unable to link streams");
				return false;
			}
			if (linked)
				return true;

			//8. try to link to file stream
			if (!TryLinkToFileStream(pFrom, VH_SI(request), metadata, streamName,
					startTime, length, linked)) {
				FATAL("Unable to link streams");
				return false;
			}
			if (linked)
				return true;

			//9. Ok, no live/file stream. Just wait for the live stream now...
			WARN("We are going to wait for the live stream `%s`", STR(streamName));
			BaseOutNetRTMPStream * pBaseOutNetRTMPStream = pFrom->CreateONS(
					VH_SI(request),
					streamName,
					ST_IN_NET_RTMP);
			return pBaseOutNetRTMPStream != NULL;
		}
		case -1: //only live
		{
			bool linked = false;

			//10. try to link to live stream
			if (!TryLinkToLiveStream(pFrom, VH_SI(request), streamName, linked)) {
				FATAL("Unable to link streams");
				return false;
			}
			if (linked)
				return true;

			//11. Ok, no live/file stream. Just wait for the live stream now...
			WARN("We are going to wait for the live stream `%s`", STR(streamName));
			BaseOutNetRTMPStream * pBaseOutNetRTMPStream = pFrom->CreateONS(
					VH_SI(request),
					streamName,
					ST_IN_NET_RTMP);
			return pBaseOutNetRTMPStream != NULL;
		}
		default: //only recorded
		{
			//12. Perform little adjustment on metadata
			if (metadata[META_MEDIA_TYPE] == MEDIA_TYPE_LIVE_OR_FLV) {
				metadata[META_MEDIA_TYPE] = MEDIA_TYPE_FLV;
			}

			//13. try to link to file stream
			bool linked = false;
			if (!TryLinkToFileStream(pFrom, VH_SI(request), metadata, streamName,
					startTime, length, linked)) {
				FATAL("Unable to link streams");
				return false;
			}

			return linked;
		}
	}
}

bool BaseRTMPAppProtocolHandler::ProcessInvokePauseRaw(BaseRTMPProtocol *pFrom,
		Variant & request) {
	//1. Read stream index and offset in millisecond
	uint32_t streamId = VH_SI(request);
	double timeOffset = 0.0;
	if ((VariantType) M_INVOKE_PARAM(request, 1) == V_DOUBLE)
		timeOffset = M_INVOKE_PARAM(request, 1);

	//2. Find the corresponding outbound stream
	BaseOutNetRTMPStream *pBaseOutNetRTMPStream = NULL;
	map<uint32_t, BaseStream *> possibleStreams = GetApplication()->
			GetStreamsManager()->FindByProtocolIdByType(pFrom->GetId(), ST_OUT_NET_RTMP, true);

	FOR_MAP(possibleStreams, uint32_t, BaseStream *, i) {
		if (((BaseOutNetRTMPStream *) MAP_VAL(i))->GetRTMPStreamId() == streamId) {
			pBaseOutNetRTMPStream = (BaseOutNetRTMPStream *) MAP_VAL(i);
			break;
		}
	}
	if (pBaseOutNetRTMPStream == NULL) {
		FATAL("No out stream");
		return false;
	}

	//3. get the operation
	bool pause = M_INVOKE_PARAM(request, 1);
	if (pause) {
		//4. Pause it
		return pBaseOutNetRTMPStream->Pause();
	} else {
		double timeOffset = 0.0;
		if ((VariantType) M_INVOKE_PARAM(request, 2) == V_DOUBLE)
			timeOffset = M_INVOKE_PARAM(request, 2);

		//8. Perform seek
		if (!pBaseOutNetRTMPStream->Seek(timeOffset)) {
			FATAL("Unable to seek");
			return false;
		}

		//9. Resume
		return pBaseOutNetRTMPStream->Resume();
	}
}

bool BaseRTMPAppProtocolHandler::ProcessInvokePause(BaseRTMPProtocol *pFrom,
		Variant & request) {
	return ProcessInvokePauseRaw(pFrom, request);
}

bool BaseRTMPAppProtocolHandler::ProcessInvokeCloseStream(BaseRTMPProtocol *pFrom,
		Variant & request) {
	return pFrom->CloseStream(VH_SI(request), true);
}

bool BaseRTMPAppProtocolHandler::ProcessInvokeReleaseStream(BaseRTMPProtocol *pFrom,
		Variant & request) {
	//1. Attempt to find the stream
	map<uint32_t, BaseStream *> streams = GetApplication()->GetStreamsManager()->
			FindByProtocolIdByName(pFrom->GetId(), M_INVOKE_PARAM(request, 1), false);
	if (streams.size() > 0) {
		//2. Is this the correct kind?
		if (TAG_KIND_OF(MAP_VAL(streams.begin())->GetType(), ST_IN_NET_RTMP)) {
			//3. get the rtmp stream id
			InNetRTMPStream *pInNetRTMPStream = (InNetRTMPStream *) MAP_VAL(streams.begin());
			uint32_t streamId = pInNetRTMPStream->GetRTMPStreamId();

			//4. close the stream
			if (!pFrom->CloseStream(streamId, true)) {
				FATAL("Unable to close stream");
				return true;
			}

			//5. Send the response
			Variant response = StreamMessageFactory::GetInvokeReleaseStreamResult(3,
					streamId, M_INVOKE_ID(request), streamId);
			if (!pFrom->SendMessage(response)) {
				FATAL("Unable to send message to client");
				return false;
			}
		}
	}

	//3. Done
	return true;
}

bool BaseRTMPAppProtocolHandler::ProcessInvokeDeleteStream(BaseRTMPProtocol *pFrom,
		Variant & request) {
	return pFrom->CloseStream((uint32_t) M_INVOKE_PARAM(request, 1), false);
}

bool BaseRTMPAppProtocolHandler::ProcessInvokeOnStatus(BaseRTMPProtocol *pFrom,
		Variant & request) {
	if ((!NeedsToPullExternalStream(pFrom))
			&& (!NeedsToPushLocalStream(pFrom))) {
		WARN("Default implementation of ProcessInvokeOnStatus in application %s: Request:\n%s",
				STR(GetApplication()->GetName()),
				STR(request.ToString()));
		return true;
	}

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

	//pProtocol->

	//6. Get our hands on streaming parameters
	string path = "";
	if (NeedsToPullExternalStream(pFrom))
		path = "externalStreamConfig";
	else
		path = "localStreamConfig";
	Variant &parameters = pFrom->GetCustomParameters()["customParameters"][path];

	if (NeedsToPullExternalStream(pFrom)) {
		if (M_INVOKE_PARAM(request, 1)["code"] != "NetStream.Play.Start") {
			WARN("onStatus message ignored:\n%s", STR(request.ToString()));
			return true;
		}
		InNetRTMPStream *pStream = pProtocol->CreateINS(VH_CI(request),
				VH_SI(request), parameters["localStreamName"]);
		if (pStream == NULL) {
			FATAL("Unable to create stream");
			return false;
		}

		map<uint32_t, BaseOutStream *> waitingSubscribers =
				GetApplication()->GetStreamsManager()->GetWaitingSubscribers(
				pStream->GetName(),
				pStream->GetType());

		FOR_MAP(waitingSubscribers, uint32_t, BaseOutStream *, i) {
			pStream->Link(MAP_VAL(i));
		}
	} else {
		if (M_INVOKE_PARAM(request, 1)["code"] != "NetStream.Publish.Start") {
			WARN("onStatus message ignored:\n%s", STR(request.ToString()));
			return true;
		}

		BaseInStream *pBaseInStream =
				(BaseInStream *) GetApplication()->GetStreamsManager()->FindByUniqueId(
				(uint32_t) parameters["localUniqueStreamId"]);
		if (pBaseInStream == NULL) {
			FATAL("Unable to find the inbound stream with id %u",
					(uint32_t) parameters["localUniqueStreamId"]);
			return false;
		}

		//5. Create the network outbound stream
		BaseOutNetRTMPStream *pBaseOutNetRTMPStream = pProtocol->CreateONS(
				VH_SI(request),
				pBaseInStream->GetName(),
				pBaseInStream->GetType());
		if (pBaseOutNetRTMPStream == NULL) {
			FATAL("Unable to create outbound stream");
			return false;
		}
		pBaseOutNetRTMPStream->SetSendOnStatusPlayMessages(false);

		//6. Link and return
		if (!pBaseInStream->Link((BaseOutNetStream*) pBaseOutNetRTMPStream)) {
			FATAL("Unable to link streams");
			return false;
		}
	}

	return true;
}

bool BaseRTMPAppProtocolHandler::ProcessInvokeFCPublish(BaseRTMPProtocol *pFrom,
		Variant & request) {

	//1. Get the stream name
	string streamName = request[RM_INVOKE][RM_INVOKE_PARAMS][(uint32_t) 1];

	//2. Send the release stream response. Is identical to the one
	//needed by this fucker
	//TODO: this is a nasty hack
	Variant response = StreamMessageFactory::GetInvokeReleaseStreamResult(3, 0,
			M_INVOKE_ID(request), 0);
	if (!pFrom->SendMessage(response)) {
		FATAL("Unable to send message to client");
		return false;
	}

	//3. send the onFCPublish message
	response = StreamMessageFactory::GetInvokeOnFCPublish(3, 0, 0, false, 0,
			RM_INVOKE_PARAMS_ONSTATUS_CODE_NETSTREAMPUBLISHSTART, streamName);
	if (!SendRTMPMessage(pFrom, response)) {
		FATAL("Unable to send message to client");
		return false;
	}

	//4. Done
	return true;
}

bool BaseRTMPAppProtocolHandler::ProcessInvokeGetStreamLength(BaseRTMPProtocol *pFrom,
		Variant & request) {
	//FINEST("Request:\n%s", STR(request.ToString()));
	Variant metadata = GetMetaData(M_INVOKE_PARAM(request, 1), true);
	Variant params;
	params[(uint32_t) 0] = Variant();
	if ((VariantType) metadata == V_MAP)
		params[(uint32_t) 1] = (double) metadata[META_FILE_DURATION] / 1000.00;
	else
		params[(uint32_t) 1] = 0.0;

	Variant response = GenericMessageFactory::GetInvokeResult(request, params);
	if (!SendRTMPMessage(pFrom, response)) {
		FATAL("Unable to send message to client");

		return false;
	}
	return true;
}

bool BaseRTMPAppProtocolHandler::ProcessInvokeOnBWDone(BaseRTMPProtocol *pFrom,
		Variant &request) {
	//WARN("ProcessInvokeOnBWDone:\n%s", STR(request.ToString()));
	WARN("ProcessInvokeOnBWDone");
	return true;
}

bool BaseRTMPAppProtocolHandler::ProcessInvokeGeneric(BaseRTMPProtocol *pFrom,
		Variant & request) {
	WARN("Default implementation of ProcessInvokeGeneric: Request: %s",
			STR(M_INVOKE_FUNCTION(request)));

	return true;
}

bool BaseRTMPAppProtocolHandler::ProcessInvokeResult(BaseRTMPProtocol *pFrom,
		Variant & result) {
	if (!MAP_HAS2(_resultMessageTracking, pFrom->GetId(), M_INVOKE_ID(result))) {
		WARN("Unable to track response from PID %d:\n%s",
				pFrom->GetId(), STR(result.ToString()));

		return true;
	}
	return ProcessInvokeResult(pFrom,
			_resultMessageTracking[pFrom->GetId()][M_INVOKE_ID(result)],
			result);
}

bool BaseRTMPAppProtocolHandler::ProcessInvokeResult(BaseRTMPProtocol *pFrom,
		Variant &request, Variant & response) {
	string functionName = request[RM_INVOKE][RM_INVOKE_FUNCTION];
	if (functionName == RM_INVOKE_FUNCTION_CONNECT) {
		return ProcessInvokeConnectResult(pFrom, request, response);
	} else if (functionName == RM_INVOKE_FUNCTION_CREATESTREAM) {
		return ProcessInvokeCreateStreamResult(pFrom, request, response);
	} else if (functionName == RM_INVOKE_FUNCTION_FCSUBSCRIBE) {
		return ProcessInvokeFCSubscribeResult(pFrom, request, response);
	} else {
		FATAL("Invoke result not yet implemented: Request:\n%s\nResponse:\n%s",
				STR(request.ToString()),
				STR(response.ToString()));

		return false;
	}
}

bool BaseRTMPAppProtocolHandler::ProcessInvokeConnectResult(BaseRTMPProtocol *pFrom,
		Variant &request, Variant & response) {
	//1. Do we need to push/pull a stream?
	if ((!NeedsToPullExternalStream(pFrom))
			&& (!NeedsToPushLocalStream(pFrom))) {
		WARN("Default implementation of ProcessInvokeConnectResult: Request:\n%s\nResponse:\n%s",
				STR(request.ToString()),
				STR(response.ToString()));
		return true;
	}

	//2. See if the result is OK or not
	if (M_INVOKE_FUNCTION(response) != RM_INVOKE_FUNCTION_RESULT) {
		if ((M_INVOKE_FUNCTION(response) != RM_INVOKE_FUNCTION_ERROR)
				|| (M_INVOKE_PARAMS(response) != V_MAP)
				|| (M_INVOKE_PARAMS(response).MapSize() < 2)
				|| (M_INVOKE_PARAM(response, 1) != V_MAP)
				|| (!M_INVOKE_PARAM(response, 1).HasKey("level"))
				|| (M_INVOKE_PARAM(response, 1)["level"] != V_STRING)
				|| (M_INVOKE_PARAM(response, 1)["level"] != "error")
				|| (!M_INVOKE_PARAM(response, 1).HasKey("code"))
				|| (M_INVOKE_PARAM(response, 1)["code"] != V_STRING)
				|| (M_INVOKE_PARAM(response, 1)["code"] != "NetConnection.Connect.Rejected")
				|| (!M_INVOKE_PARAM(response, 1).HasKey("description"))
				|| (M_INVOKE_PARAM(response, 1)["description"] != V_STRING)
				|| (M_INVOKE_PARAM(response, 1)["description"] == "")
				) {
			FATAL("Connect failed:\n%s", STR(response.ToString()));
			return false;
		}
		string description = M_INVOKE_PARAM(response, 1)["description"];
		vector<string> parts;
		split(description, "?", parts);
		if (parts.size() != 2) {
			FATAL("Connect failed:\n%s", STR(response.ToString()));
			return false;
		}
		description = parts[1];
		map<string, string> params = mapping(description, "&", "=", true);
		if ((!MAP_HAS1(params, "reason"))
				|| (!MAP_HAS1(params, "user"))
				|| (!MAP_HAS1(params, "salt"))
				|| (!MAP_HAS1(params, "challenge"))
				|| (!MAP_HAS1(params, "opaque"))
				|| (params["reason"] != "needauth")
				) {
			FATAL("Connect failed:\n%s", STR(response.ToString()));
			return false;
		}

		Variant &customParameters = pFrom->GetCustomParameters();
		Variant &streamConfig = NeedsToPullExternalStream(pFrom)
				? customParameters["customParameters"]["externalStreamConfig"]
				: customParameters["customParameters"]["localStreamConfig"];

		FOR_MAP(params, string, string, i) {
			streamConfig["auth"][MAP_KEY(i)] = MAP_VAL(i);
		}

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

	if (NeedsToPullExternalStream(pFrom)) {
		Variant &parameters = pFrom->GetCustomParameters()["customParameters"]["externalStreamConfig"];
		Variant params;
		params.PushToArray(Variant());
		params.PushToArray(parameters["uri"]["document"]);

		Variant FCSubscribeRequest = StreamMessageFactory::GetInvokeFCSubscribe(
				parameters["uri"]["document"]);
		if (!SendRTMPMessage(pFrom, FCSubscribeRequest, true)) {
			FATAL("Unable to send request:\n%s", STR(FCSubscribeRequest.ToString()));
			return false;
		}
	}

	//3. Create the createStream request
	Variant createStreamRequest = StreamMessageFactory::GetInvokeCreateStream();

	//4. Send it
	if (!SendRTMPMessage(pFrom, createStreamRequest, true)) {
		FATAL("Unable to send request:\n%s", STR(createStreamRequest.ToString()));
		return false;
	}

	//5. Done
	return true;
}

bool BaseRTMPAppProtocolHandler::ProcessInvokeCreateStreamResult(BaseRTMPProtocol *pFrom,
		Variant &request, Variant & response) {
	//1. Do we need to push/pull a stream?
	if ((!NeedsToPullExternalStream(pFrom))
			&& (!NeedsToPushLocalStream(pFrom))) {
		WARN("Default implementation of ProcessInvokeCreateStreamResult: Request:\n%s\nResponse:\n%s",
				STR(request.ToString()),
				STR(response.ToString()));

		return true;
	}

	//2. Test and see if this connection is an outbound RTMP connection
	//and get a pointer to it
	if (pFrom->GetType() != PT_OUTBOUND_RTMP) {
		FATAL("This is not an outbound connection");
		return false;
	}
	OutboundRTMPProtocol *pProtocol = (OutboundRTMPProtocol *) pFrom;

	//3. Test the response
	if (M_INVOKE_FUNCTION(response) != RM_INVOKE_FUNCTION_RESULT) {
		FATAL("createStream failed:\n%s", STR(response.ToString()));
		return false;
	}
	if (M_INVOKE_PARAM(response, 1) != _V_NUMERIC) {
		FATAL("createStream failed:\n%s", STR(response.ToString()));
		return false;
	}

	//4. Get the assigned stream ID
	uint32_t rtmpStreamId = (uint32_t) M_INVOKE_PARAM(response, 1);

	//5. Create the neutral stream
	RTMPStream *pStream = pProtocol->CreateNeutralStream(rtmpStreamId);
	if (pStream == NULL) {
		FATAL("Unable to create neutral stream");
		return false;
	}


	//6. Get our hands on streaming parameters
	string path = "";
	if (NeedsToPullExternalStream(pFrom))
		path = "externalStreamConfig";
	else
		path = "localStreamConfig";
	Variant &parameters = pFrom->GetCustomParameters()["customParameters"][path];

	//7. Create publish/play request
	Variant publishPlayRequest;
	if (NeedsToPullExternalStream(pFrom)) {
		publishPlayRequest = StreamMessageFactory::GetInvokePlay(3, rtmpStreamId,
				parameters["uri"]["document"], -2, -1);
	} else {
		string targetStreamType = "";
		if (parameters["targetStreamType"] == V_STRING) {
			targetStreamType = (string) parameters["targetStreamType"];
		}
		if ((targetStreamType != "live")
				&& (targetStreamType != "record")
				&& (targetStreamType != "append")) {
			targetStreamType = "live";
		}
		publishPlayRequest = StreamMessageFactory::GetInvokePublish(3, rtmpStreamId,
				parameters["targetStreamName"], targetStreamType);
	}

	//8. Send it
	if (!SendRTMPMessage(pFrom, publishPlayRequest, true)) {
		FATAL("Unable to send request:\n%s", STR(publishPlayRequest.ToString()));
		return false;
	}

	//9. Done
	return true;
}

bool BaseRTMPAppProtocolHandler::ProcessInvokeFCSubscribeResult(BaseRTMPProtocol *pFrom,
		Variant &request, Variant &response) {
	return true;
}

bool BaseRTMPAppProtocolHandler::AuthenticateInboundAdobe(BaseRTMPProtocol *pFrom,
		Variant & request, Variant &authState) {

	//	FINEST("_configuration:\n%s", STR(_configuration.ToString()));
	//	FINEST("request:\n%s", STR(request.ToString()));

	if (!authState.HasKey("stage"))
		authState["stage"] = "inProgress";
	//FINEST("authState:\n%s", STR(authState.ToString()));

	if (authState["stage"] == "authenticated") {
		return true;
	}

	if (authState["stage"] != "inProgress") {
		FATAL("This protocol in not in the authenticating mode");
		return false;
	}

	//1. Validate the type of request
	if ((uint8_t) VH_MT(request) != RM_HEADER_MESSAGETYPE_INVOKE) {
		FINEST("This is not an invoke. Wait for it...");
		return true;
	}

	//2. Validate the invoke function name
	if (M_INVOKE_FUNCTION(request) != RM_INVOKE_FUNCTION_CONNECT) {
		FATAL("This is not a connect invoke");
		return false;
	}

	//3. Pick up the first param in the invoke
	Variant connectParams = M_INVOKE_PARAM(request, 0);
	if (connectParams != V_MAP) {
		FATAL("first invoke param must be a map");
		return false;
	}

	//4. pick up the agent name
	if ((!connectParams.HasKey(RM_INVOKE_PARAMS_CONNECT_FLASHVER))
			|| (connectParams[RM_INVOKE_PARAMS_CONNECT_FLASHVER] != V_STRING)) {
		WARN("Incorrect user agent");
		authState["stage"] = "authenticated";
		authState["canPublish"] = (bool)false;
		authState["canOverrideStreamName"] = (bool)false;
		return true;
	}
	string flashVer = (string) connectParams[RM_INVOKE_PARAMS_CONNECT_FLASHVER];

	//5. Do we have a list of encoder agents?
	_adobeAuthSettings = _configuration[CONF_APPLICATION_AUTH];

	//6. test the flash ver against the allowed encoder agents
	if (!_adobeAuthSettings[CONF_APPLICATION_AUTH_ENCODER_AGENTS].HasKey(flashVer)) {
		WARN("This agent is not on the list of allowed encoders: `%s`", STR(flashVer));
		authState["stage"] = "authenticated";
		authState["canPublish"] = (bool)false;
		authState["canOverrideStreamName"] = (bool)false;
		return true;
	}

	//7. pick up the tcUrl from the first param
	if ((!connectParams.HasKey(RM_INVOKE_PARAMS_CONNECT_APP))
			|| (connectParams[RM_INVOKE_PARAMS_CONNECT_APP] != V_STRING)) {
		WARN("Incorrect app url");
		authState["stage"] = "authenticated";
		authState["canPublish"] = (bool)false;
		authState["canOverrideStreamName"] = (bool)false;
		return true;
	}
	string appUrl = (string) connectParams[RM_INVOKE_PARAMS_CONNECT_APP];
	//FINEST("appUrl: %s", STR(appUrl));

	//8. Split the URI into parts
	vector<string> appUrlParts;
	split(appUrl, "?", appUrlParts);

	//9. Based on the parts count, we are in a specific stage
	switch (appUrlParts.size()) {
		case 1:
		{
			//bare request. We need to tell him that he needs auth
			Variant response = ConnectionMessageFactory::GetInvokeConnectError(request,
					"[ AccessManager.Reject ] : [ code=403 need auth; authmod=adobe ] : ");
			if (!pFrom->SendMessage(response)) {
				FATAL("Unable to send message");
				return false;
			}
			//FINEST("response:\n%s", STR(response.ToString()));

			response = ConnectionMessageFactory::GetInvokeClose();
			if (!pFrom->SendMessage(response)) {
				FATAL("Unable to send message");
				return false;
			}
			//FINEST("response:\n%s", STR(response.ToString()));

			pFrom->GracefullyEnqueueForDelete();
			return true;
		}
		case 2:
		{
			map<string, string> params = mapping(appUrlParts[1], "&", "=", false);
			if ((!MAP_HAS1(params, "authmod")) || (!MAP_HAS1(params, "user"))) {
				WARN("Invalid appUrl: %s", STR(appUrl));
				authState["stage"] = "authenticated";
				authState["canPublish"] = (bool)false;
				authState["canOverrideStreamName"] = (bool)false;
				return true;
			}

			//            FOR_MAP(params, string, string, i) {
			//                FINEST("%s: `%s`", STR(MAP_KEY(i)), STR(MAP_VAL(i)));
			//            }

			string user = params["user"];

			if (MAP_HAS1(params, "challenge")
					&& MAP_HAS1(params, "response")
					&& MAP_HAS1(params, "opaque")) {
				string challenge = params["challenge"];
				string response = params["response"];
				string opaque = params["opaque"];
				string password = GetAuthPassword(user);
				if (password == "") {
					WARN("No such user: `%s`", STR(user));
					Variant response = ConnectionMessageFactory::GetInvokeConnectError(request,
							//"[ AccessManager.Reject ] : [ authmod=adobe ] : ?reason=nosuchuser&opaque=nQoAAA==");
							"[ AccessManager.Reject ] : [ authmod=adobe ] : ?reason=authfailed&opaque=vgoAAA==");
					if (!pFrom->SendMessage(response)) {
						FATAL("Unable to send message");
						return false;
					}
					//FINEST("response:\n%s", STR(response.ToString()));

					response = ConnectionMessageFactory::GetInvokeClose();
					if (!pFrom->SendMessage(response)) {
						FATAL("Unable to send message");
						return false;
					}
					//FINEST("response:\n%s", STR(response.ToString()));

					pFrom->GracefullyEnqueueForDelete();
					return true;
				}

				string str1 = user + _adobeAuthSalt + password;
				string hash1 = b64(md5(str1, false));
				string str2 = hash1 + opaque + challenge;
				string wanted = b64(md5(str2, false));

				if (response == wanted) {
					authState["stage"] = "authenticated";
					authState["canPublish"] = (bool)true;
					authState["canOverrideStreamName"] = (bool)true;
					WARN("User `%s` authenticated", STR(user));
					return true;
				} else {
					WARN("Invalid password for user `%s`", STR(user));
					//					FATAL("Auth failed: s1: `%s`; h1: `%s`; s2: `%s`; h2: `%s`",
					//							STR(str1), STR(hash1), STR(str2), STR(hash2));
					Variant response = ConnectionMessageFactory::GetInvokeConnectError(request,
							"[ AccessManager.Reject ] : [ authmod=adobe ] : ?reason=authfailed&opaque=vgoAAA==");
					if (!pFrom->SendMessage(response)) {
						FATAL("Unable to send message");
						return false;
					}
					//FINEST("response:\n%s", STR(response.ToString()));

					response = ConnectionMessageFactory::GetInvokeClose();
					if (!pFrom->SendMessage(response)) {
						FATAL("Unable to send message");
						return false;
					}
					//FINEST("response:\n%s", STR(response.ToString()));

					pFrom->GracefullyEnqueueForDelete();
					return true;
				}
			} else {
				string challenge = generateRandomString(6) + "==";
				string opaque = challenge;
				string description = "[ AccessManager.Reject ] : [ authmod=adobe ] : ?reason=needauth&user=%s&salt=%s&challenge=%s&opaque=%s";

				description = format(description, STR(user), STR(_adobeAuthSalt),
						STR(challenge), STR(opaque));

				Variant response = ConnectionMessageFactory::GetInvokeConnectError(request, description);
				if (!pFrom->SendMessage(response)) {
					FATAL("Unable to send message");
					return false;
				}
				//FINEST("response:\n%s", STR(response.ToString()));

				response = ConnectionMessageFactory::GetInvokeClose();
				if (!pFrom->SendMessage(response)) {
					FATAL("Unable to send message");
					return false;
				}
				//FINEST("response:\n%s", STR(response.ToString()));

				pFrom->GracefullyEnqueueForDelete();
				return true;
			}
		}
		default:
		{
			FATAL("Invalid appUrl: %s", STR(appUrl));
			return false;
		}
	}
}

string BaseRTMPAppProtocolHandler::GetAuthPassword(string user) {
	string usersFile = _configuration[CONF_APPLICATION_AUTH][CONF_APPLICATION_AUTH_USERS_FILE];
	string fileName;
	string extension;
	splitFileName(usersFile, fileName, extension);
	Variant users;

	if (lowercase(extension) == "xml") {
		if (!Variant::DeserializeFromXmlFile(usersFile, users)) {
			FATAL("Unable to read users file: `%s`", STR(usersFile));
			return "";
		}
	} else if (lowercase(extension) == "lua") {
#ifdef HAS_LUA
		if (!ReadLuaFile(usersFile, "users", users)) {
			FATAL("Unable to read users file: `%s`", STR(usersFile));
			return "";
		}
#else
		ASSERT("Lua is not supported by the current build of the server");
		return "";
#endif /* HAS_LUA */
	} else {
		FATAL("Invalid file format: %s", STR(usersFile));
		return "";
	}

	if ((VariantType) users != V_MAP) {
		FATAL("Invalid users file: `%s`", STR(usersFile));
		return "";
	}

	if (users.HasKey(user)) {
		if ((VariantType) users[user] == V_STRING) {
			return users[user];
		} else {
			FATAL("Invalid users file: `%s`", STR(usersFile));
			return "";
		}
	} else {
		FATAL("User `%s` not present in users file: `%s`", STR(user),
				STR(usersFile));
		return "";
	}
}

Variant BaseRTMPAppProtocolHandler::GetMetaData(string streamName,
		bool extractInnerMetadata) {
	Variant result;
	//1. Store the original request and the keyframe seek flag
	result[META_REQUESTED_STREAM_NAME] = streamName;
	result[CONF_APPLICATION_KEYFRAMESEEK] = (bool)_keyframeSeek;
	result[CONF_APPLICATION_CLIENTSIDEBUFFER] = (int32_t) _clientSideBuffer;
	result[CONF_APPLICATION_SEEKGRANULARITY] = _seekGranularity;
	result[CONF_APPLICATION_RENAMEBADFILES] = (bool)_renameBadFiles;

	//2.Determine the media type
	vector<string> parts;
	split(streamName, ":", parts);
	if (parts.size() != 1 && parts.size() != 2 && parts.size() != 5) {
		FATAL("Invalid stream name format: %s", STR(streamName));
		return Variant();
	}

	if (parts.size() == 1) {
		result[META_MEDIA_TYPE] = MEDIA_TYPE_LIVE_OR_FLV;
	} else {
		//some other type
		result[META_MEDIA_TYPE] = lowercase(parts[0]);
	}

	//3. Establish the final file name we are searching for. If the
	//media type is missing, assume we are looking for a flv file
	string searchFor = "";
	if (result[META_MEDIA_TYPE] == MEDIA_TYPE_LIVE_OR_FLV)
		searchFor = parts[0] + ".flv";
	else if (result[META_MEDIA_TYPE] == MEDIA_TYPE_MP3)
		searchFor = parts[1] + ".mp3";
	else
		searchFor = parts[1];

	result[META_SERVER_FILE_NAME] = searchFor;
	result[META_SERVER_MEDIA_DIR] = _mediaFolder;

	result[META_SERVER_FULL_PATH] = normalizePath(_mediaFolder, searchFor);
	//ASSERT("result[META_SERVER_FULL_PATH]: %s\n_mediaFolder: %s\nsearchFor: %s",
	//	STR(result[META_SERVER_FULL_PATH]),
	//	STR(_mediaFolder),
	//	STR(searchFor));

	//FINEST("result[META_SERVER_FULL_PATH]: %s", STR(result[META_SERVER_FULL_PATH]));
	if (!result.HasKey(META_SERVER_FULL_PATH))
		result[META_SERVER_FULL_PATH] = "";

	//6. Test to see if we need to continue with load/cache the metadata
	if (result[META_SERVER_FULL_PATH] == "")
		return result;

	//7. Load the rest of the metadata from a cache or load it from file and
	//cache it after that
	string metaPath = (string) result[META_SERVER_FULL_PATH] + "."MEDIA_TYPE_META;
	if (fileExists(metaPath)) {
		if (GetFileModificationDate(metaPath) >= GetFileModificationDate(result[META_SERVER_FULL_PATH])) {
			if (!Variant::DeserializeFromXmlFile(metaPath, result)) {
				WARN("Unable to deserialize the meta file %s. Rebuilding it", STR(metaPath));
			} else {
				return result;
			}
		} else {
			WARN("Media file modified: %s. The *.seek and *.meta file will be recreated",
					STR(result[META_SERVER_FULL_PATH]));
		}
	}

	//8. We either have a bad meta file or we don't have it at all. Build it
	if (extractInnerMetadata) {
		if (!InFileRTMPStream::ResolveCompleteMetadata(result)) {
			FATAL("Unable to get metadata. Partial result:\n%s",
					STR(result.ToString()));
			return Variant();
		}
	}

	//9. Save it
	if (!result.SerializeToXmlFile(metaPath)) {
		WARN("Unable to serialize meta file %s. Content was: %s",
				STR(metaPath), STR(result.ToString()));
	}
	return result;
}

bool BaseRTMPAppProtocolHandler::SendRTMPMessage(BaseRTMPProtocol *pTo,
		Variant message, bool trackResponse) {
	switch ((uint8_t) VH_MT(message)) {
		case RM_HEADER_MESSAGETYPE_INVOKE:
		{
			if (M_INVOKE_FUNCTION(message) != RM_INVOKE_FUNCTION_RESULT) {
				uint32_t invokeId = 0;
				if (trackResponse) {
					if (!MAP_HAS1(_nextInvokeId, pTo->GetId())) {
						FATAL("Unable to get next invoke ID");
						return false;
					}
					invokeId = _nextInvokeId[pTo->GetId()];
					_nextInvokeId[pTo->GetId()] = invokeId + 1;
				}
				M_INVOKE_ID(message) = invokeId;
				//  FINEST("PID: %d; IID: %d", pTo->GetId(), invokeId);
				if (trackResponse)
					_resultMessageTracking[pTo->GetId()][invokeId] = message;
				return pTo->SendMessage(message);
			} else {
				return pTo->SendMessage(message);
			}
		}
		case RM_HEADER_MESSAGETYPE_FLEXSTREAMSEND:
		case RM_HEADER_MESSAGETYPE_WINACKSIZE:
		case RM_HEADER_MESSAGETYPE_PEERBW:
		case RM_HEADER_MESSAGETYPE_USRCTRL:
		case RM_HEADER_MESSAGETYPE_ABORTMESSAGE:
		{
			return pTo->SendMessage(message);
		}
		default:
		{
			FATAL("Unable to send message:\n%s", STR(message.ToString()));
			return false;
		}
	}
}

bool BaseRTMPAppProtocolHandler::TryLinkToLiveStream(BaseRTMPProtocol *pFrom,
		uint32_t streamId, string streamName, bool &linked) {
	linked = false;

	//1. Get get the short version of the stream name
	vector<string> parts;
	split(streamName, "?", parts);
	string shortName = parts[0];
	//FINEST("short name: `%s`; long name: `%s`", STR(shortName), STR(streamName));

	//2. Search for the long version first
	map<uint32_t, BaseStream *> inboundStreams =
			GetApplication()->GetStreamsManager()->FindByTypeByName(
			ST_IN_NET, streamName, true, false);

	//3. Search for the short version if necessary
	if (inboundStreams.size() == 0) {
		inboundStreams = GetApplication()->GetStreamsManager()->FindByTypeByName(
				ST_IN_NET, shortName + "?", true, true);
	}

	//4. Do we have some streams?
	if (inboundStreams.size() == 0) {
		WARN("No live streams found: `%s` or `%s`", STR(streamName), STR(shortName));
		return true;
	}

	//5. Get the first stream in the inboundStreams
	BaseInNetStream *pBaseInNetStream = (BaseInNetStream *) MAP_VAL(inboundStreams.begin());

	//6. Create the outbound stream
	BaseOutNetRTMPStream * pBaseOutNetRTMPStream = pFrom->CreateONS(streamId,
			streamName, pBaseInNetStream->GetType());
	if (pBaseOutNetRTMPStream == NULL) {
		FATAL("Unable to create network outbound stream");
		return false;
	}

	//7. Link them
	if (!pBaseInNetStream->Link(pBaseOutNetRTMPStream)) {
		FATAL("Link failed");
		return false;
	}

	//8. Done
	linked = true;
	return true;
}

bool BaseRTMPAppProtocolHandler::TryLinkToFileStream(BaseRTMPProtocol *pFrom,
		uint32_t streamId, Variant &metadata, string streamName, double startTime,
		double length, bool &linked) {
	linked = false;

	//1. Try to create the in file streams
	InFileRTMPStream *pRTMPInFileStream = pFrom->CreateIFS(metadata);
	if (pRTMPInFileStream == NULL) {
		WARN("No file streams found: %s", STR(streamName));
		return true;
	}

	//2. Try to create the out net stream
	BaseOutNetRTMPStream * pBaseOutNetRTMPStream = pFrom->CreateONS(
			streamId, streamName, pRTMPInFileStream->GetType());
	if (pBaseOutNetRTMPStream == NULL) {
		FATAL("Unable to create network outbound stream");
		return false;
	}

	//3. Link them
	if (!pRTMPInFileStream->Link(pBaseOutNetRTMPStream)) {
		FATAL("Link failed");
		return false;
	}

	//4. Register it to the signaled streams
	pFrom->SignalONS(pBaseOutNetRTMPStream);

	//5. Fire up the play routine
	if (!pRTMPInFileStream->Play(startTime, length)) {
		FATAL("Unable to start the playback");
		return false;
	}

	//6. Done
	linked = true;
	return true;
}

void BaseRTMPAppProtocolHandler::GenerateMetaFiles() {
	vector<string> files;
	if (!ListFolder(_configuration[CONF_APPLICATION_MEDIAFOLDER], files)) {
		FATAL("Unable to list folder %s",
				STR(_configuration[CONF_APPLICATION_MEDIAFOLDER]));
		return;
	}

	string file, name, extension;

	FOR_VECTOR_ITERATOR(string, files, i) {
		file = VECTOR_VAL(i);

		splitFileName(file, name, extension);
		extension = lowercase(extension);

		if (extension != MEDIA_TYPE_FLV
				&& extension != MEDIA_TYPE_MP3
				&& extension != MEDIA_TYPE_MP4
				&& extension != MEDIA_TYPE_M4A
				&& extension != MEDIA_TYPE_M4V
				&& extension != MEDIA_TYPE_MOV
				&& extension != MEDIA_TYPE_F4V
				&& extension != MEDIA_TYPE_TS
				&& extension != MEDIA_TYPE_NSV)
			continue;
		string flashName = "";
		if (extension == MEDIA_TYPE_FLV) {
			flashName = name;
		} else if (extension == MEDIA_TYPE_MP3) {
			flashName = extension + ":" + name;
		} else if (extension == MEDIA_TYPE_NSV) {
			flashName = extension + ":" + name;
		} else {
			if (extension == MEDIA_TYPE_MP4
					|| extension == MEDIA_TYPE_M4A
					|| extension == MEDIA_TYPE_M4V
					|| extension == MEDIA_TYPE_MOV
					|| extension == MEDIA_TYPE_F4V) {
				flashName = MEDIA_TYPE_MP4":" + name + "." + extension;
			} else {
				flashName = extension + ":" + name + "." + extension;
			}
		}

		FINEST("Parsing `%s`", STR(flashName));
		GetMetaData(flashName, true);
	}
}

bool BaseRTMPAppProtocolHandler::NeedsToPullExternalStream(
		BaseRTMPProtocol *pFrom) {
	Variant &parameters = pFrom->GetCustomParameters();
	if (parameters != V_MAP)
		return false;
	if (!parameters.HasKey("customParameters"))
		return false;
	if (parameters["customParameters"] != V_MAP)
		return false;
	if (!parameters["customParameters"].HasKey("externalStreamConfig"))
		return false;
	if (parameters["customParameters"]["externalStreamConfig"] != V_MAP)
		return false;
	if (!parameters["customParameters"]["externalStreamConfig"].HasKey("uri"))
		return false;
	if (parameters["customParameters"]["externalStreamConfig"]["uri"] != V_MAP)
		return false;
	return true;
}

bool BaseRTMPAppProtocolHandler::NeedsToPushLocalStream(BaseRTMPProtocol *pFrom) {
	Variant &parameters = pFrom->GetCustomParameters();
	if (parameters != V_MAP)
		return false;
	if (!parameters.HasKey("customParameters"))
		return false;
	if (parameters["customParameters"] != V_MAP)
		return false;
	if (!parameters["customParameters"].HasKey("localStreamConfig"))
		return false;
	if (parameters["customParameters"]["localStreamConfig"] != V_MAP)
		return false;
	if (!parameters["customParameters"]["localStreamConfig"].HasKey("targetUri"))
		return false;
	if (parameters["customParameters"]["localStreamConfig"]["targetUri"] != V_MAP)
		return false;
	return true;
}

bool BaseRTMPAppProtocolHandler::PullExternalStream(BaseRTMPProtocol *pFrom) {
	//1. Get the stream configuration and the URI from it
	Variant &streamConfig = pFrom->GetCustomParameters()["customParameters"]["externalStreamConfig"];

	//2. Issue the connect invoke
	return ConnectForPullPush(pFrom, "uri", streamConfig);
}

bool BaseRTMPAppProtocolHandler::PushLocalStream(BaseRTMPProtocol *pFrom) {
	//1. Get the stream configuration and the URI from it
	Variant &streamConfig = pFrom->GetCustomParameters()["customParameters"]["localStreamConfig"];

	//2. Issue the connect invoke
	return ConnectForPullPush(pFrom, "targetUri", streamConfig);
}

bool BaseRTMPAppProtocolHandler::ConnectForPullPush(BaseRTMPProtocol *pFrom,
		string uriPath, Variant &streamConfig) {
	//FINEST("streamConfig:\n%s", STR(streamConfig.ToString()));
	URI uri;
	if (!URI::FromVariant(streamConfig[uriPath], uri)) {
		FATAL("Unable to parse uri:\n%s", STR(streamConfig["targetUri"]));
		return false;
	}

	//2. get the application name
	string appName = uri.documentPath;
	if (appName == "/")
		appName = uri.document;
	if (appName != "") {
		if (appName[0] == '/')
			appName = appName.substr(1, appName.size() - 1);
	}
	if (appName == "") {
		FATAL("Invalid uri: %s", STR(uri.fullUri));
		return false;
	}
	if (uri.userName != "") {
		if (streamConfig.HasKey("auth")) {
			string user = uri.userName;
			string password = uri.password;
			string salt = streamConfig["auth"]["salt"];
			string opaque = streamConfig["auth"]["opaque"];
			string challenge = streamConfig["auth"]["challenge"];
			string response = b64(md5(b64(md5(user + salt + password, false)) + opaque + challenge, false));
			appName = appName + "?authmod=adobe"
					+ "&user=" + uri.userName
					+ "&challenge=" + challenge
					+ "&opaque=" + opaque
					+ "&salt=" + salt
					+ "&response=" + response;
			streamConfig["emulateUserAgent"] = "FMLE/3.0 (compatible; FMSc/1.0)";
		} else {
			appName = appName + "?authmod=adobe&user=" + uri.userName;
			streamConfig["emulateUserAgent"] = "FMLE/3.0 (compatible; FMSc/1.0)";
		}
	}

	//3. Compute tcUrl: rtmp://host/appName
	string tcUrl = format("%s://%s%s/%s",
			STR(uri.scheme),
			STR(uri.host),
			STR(uri.port == 1935 ? "" : format(":%d", uri.port)),
			STR(appName));

	//4. Get the user agent
	string userAgent = "";
	if (streamConfig["emulateUserAgent"] == V_STRING) {
		userAgent = (string) streamConfig["emulateUserAgent"];
	}
	if (userAgent == "") {
		userAgent = HTTP_HEADERS_SERVER_US;
	}

	//5. Get swfUrl and pageUrl
	string swfUrl = "";
	if (streamConfig["swfUrl"] == V_STRING) {
		swfUrl = (string) streamConfig["swfUrl"];
	}
	string pageUrl = "";
	if (streamConfig["pageUrl"] == V_STRING) {
		pageUrl = (string) streamConfig["pageUrl"];
	}

	//6. Prepare the connect request
	Variant connectRequest = ConnectionMessageFactory::GetInvokeConnect(
			appName, //string appName
			tcUrl, //string tcUrl
			3191, //double audioCodecs
			239, //double capabilities
			userAgent, //string flashVer
			false, //bool fPad
			pageUrl, //string pageUrl
			swfUrl, //string swfUrl
			252, //double videoCodecs
			1, //double videoFunction
			0 //double objectEncoding
			);

	//7. Send it
	if (!SendRTMPMessage(pFrom, connectRequest, true)) {
		FATAL("Unable to send request:\n%s", STR(connectRequest.ToString()));
		return false;
	}

	return true;
}
#endif /* HAS_PROTOCOL_RTMP */

