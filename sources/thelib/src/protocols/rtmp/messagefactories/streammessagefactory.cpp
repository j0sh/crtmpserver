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
#include "protocols/rtmp/messagefactories/streammessagefactory.h"
#include "protocols/rtmp/header.h"
#include "protocols/rtmp/rtmpprotocolserializer.h"

Variant StreamMessageFactory::GetUserControlStream(uint16_t operation, uint32_t streamId) {
	Variant result;

	VH(result, HT_FULL, 2, 0, 0, RM_HEADER_MESSAGETYPE_USRCTRL,
			0, true);

	M_USRCTRL_TYPE(result) = (uint16_t) operation;
	M_USRCTRL_TYPE_STRING(result) =
			RTMPProtocolSerializer::GetUserCtrlTypeString(operation);
	M_USRCTRL_STREAMID(result) = streamId;

	return result;
}

Variant StreamMessageFactory::GetUserControlStreamBegin(uint32_t streamId) {
	return GetUserControlStream(RM_USRCTRL_TYPE_STREAM_BEGIN, streamId);
}

Variant StreamMessageFactory::GetUserControlStreamEof(uint32_t streamId) {
	return GetUserControlStream(RM_USRCTRL_TYPE_STREAM_EOF, streamId);
}

Variant StreamMessageFactory::GetUserControlStreamDry(uint32_t streamId) {
	return GetUserControlStream(RM_USRCTRL_TYPE_STREAM_DRY, streamId);
}

Variant StreamMessageFactory::GetUserControlStreamIsRecorded(uint32_t streamId) {
	return GetUserControlStream(RM_USRCTRL_TYPE_STREAM_IS_RECORDED, streamId);
}

Variant StreamMessageFactory::GetInvokeCreateStream() {
	Variant createStream;
	createStream[(uint32_t) 0] = Variant();
	return GenericMessageFactory::GetInvoke(3, 0, 0, false, 1,
			RM_INVOKE_FUNCTION_CREATESTREAM, createStream);
}

Variant StreamMessageFactory::GetInvokeCloseStream(uint32_t channelId, uint32_t streamId) {
	Variant closeStream;
	closeStream[(uint32_t) 0] = Variant();
	return GenericMessageFactory::GetInvoke(channelId, streamId, 0, false, 1,
			RM_INVOKE_FUNCTION_CLOSESTREAM, closeStream);
}

Variant StreamMessageFactory::GetInvokeDeleteStream(uint32_t channelId, uint32_t streamId) {
	Variant deleteStream;
	deleteStream[(uint32_t) 0] = Variant();
	return GenericMessageFactory::GetInvoke(channelId, streamId, 0, false, 1,
			RM_INVOKE_FUNCTION_DELETESTREAM, deleteStream);
}

Variant StreamMessageFactory::GetInvokePublish(uint32_t channelId, uint32_t streamId,
		string streamName, string mode) {
	Variant publish;
	publish[(uint32_t) 0] = Variant();
	publish[(uint32_t) 1] = streamName;
	publish[(uint32_t) 2] = mode;
	return GenericMessageFactory::GetInvoke(channelId, streamId, 0, false, 1,
			RM_INVOKE_FUNCTION_PUBLISH, publish);
}

Variant StreamMessageFactory::GetInvokePlay(uint32_t channelId, uint32_t streamId,
		string streamName, double start, double length) {
	Variant play;
	play[(uint32_t) 0] = Variant();
	play[(uint32_t) 1] = streamName;
	play[(uint32_t) 2] = start;
	play[(uint32_t) 3] = length;
	return GenericMessageFactory::GetInvoke(channelId, streamId, 0, false, 1,
			RM_INVOKE_FUNCTION_PLAY, play);
}

Variant StreamMessageFactory::GetInvokeFCSubscribe(string streamName) {
	Variant FCSubscribe;
	FCSubscribe.PushToArray(Variant());
	FCSubscribe.PushToArray(streamName);
	return GenericMessageFactory::GetInvoke(3, 0, 0, false, 1,
			RM_INVOKE_FUNCTION_FCSUBSCRIBE, FCSubscribe);
}

Variant StreamMessageFactory::GetInvokeCreateStreamResult(Variant &request,
		double createdStreamId) {
	return GetInvokeCreateStreamResult(VH_CI(request), VH_SI(request),
			M_INVOKE_ID(request), createdStreamId);
}

Variant StreamMessageFactory::GetInvokeCreateStreamResult(uint32_t channelId,
		uint32_t streamId, uint32_t requestId, double createdStreamId) {
	Variant result = (double) createdStreamId;
	return GenericMessageFactory::GetInvokeResult(channelId, streamId, requestId,
			Variant(), result);
}

Variant StreamMessageFactory::GetInvokeReleaseStreamResult(uint32_t channelId,
		uint32_t streamId, uint32_t requestId, double releasedStreamId) {
	Variant result;
	if (streamId != 0)
		result = streamId;
	return GenericMessageFactory::GetInvokeResult(channelId, streamId, requestId,
			Variant(), result);
}

Variant StreamMessageFactory::GetInvokeReleaseStreamErrorNotFound(Variant &request) {
	Variant secondParams;
	secondParams[RM_INVOKE_PARAMS_RESULT_LEVEL] = RM_INVOKE_PARAMS_RESULT_LEVEL_ERROR;
	secondParams[RM_INVOKE_PARAMS_RESULT_CODE] = "NetConnection.Call.Failed";
	secondParams[RM_INVOKE_PARAMS_RESULT_DESCRIPTION] = "Specified stream not found in call to releaseStream";

	return GenericMessageFactory::GetInvokeError(
			VH_CI(request),
			VH_SI(request),
			M_INVOKE_ID(request),
			Variant(),
			secondParams);
}

Variant StreamMessageFactory::GetInvokeOnFCPublish(uint32_t channelId,
		uint32_t streamId, double timeStamp, bool isAbsolute,
		double requestId, string code, string description) {
	Variant params;

	params[(uint32_t) 0] = Variant();
	params[(uint32_t) 1][RM_INVOKE_PARAMS_ONSTATUS_CODE] = code;
	params[(uint32_t) 1][RM_INVOKE_PARAMS_ONSTATUS_DESCRIPTION] = description;

	return GenericMessageFactory::GetInvoke(channelId, streamId,
			timeStamp, isAbsolute, requestId, "onFCPublish", params);
}

Variant StreamMessageFactory::GetInvokeOnStatusStreamPublishBadName(Variant &request,
		string streamName) {
	return GetInvokeOnStatusStreamPublishBadName(
			VH_CI(request),
			VH_SI(request),
			M_INVOKE_ID(request),
			streamName);
}

Variant StreamMessageFactory::GetInvokeOnStatusStreamPublishBadName(uint32_t channelId,
		uint32_t streamId, double requestId, string streamName) {
	Variant result;

	result[RM_INVOKE_PARAMS_ONSTATUS_LEVEL] = RM_INVOKE_PARAMS_RESULT_LEVEL_ERROR;
	result[RM_INVOKE_PARAMS_ONSTATUS_CODE] = "NetStream.Publish.BadName";
	result[RM_INVOKE_PARAMS_ONSTATUS_DESCRIPTION] = format("%s is not available", STR(streamName));
	result[RM_INVOKE_PARAMS_ONSTATUS_DETAILS] = streamName;
	result[RM_INVOKE_PARAMS_ONSTATUS_CLIENTID] = "";

	return GenericMessageFactory::GetInvokeOnStatus(channelId, streamId,
			0, false, requestId, result);
}

Variant StreamMessageFactory::GetInvokeOnStatusStreamPublished(uint32_t channelId,
		uint32_t streamId, double timeStamp, bool isAbsolute,
		double requestId, string level, string code, string description,
		string details, string clientId) {
	Variant result;

	result[RM_INVOKE_PARAMS_ONSTATUS_LEVEL] = level;
	result[RM_INVOKE_PARAMS_ONSTATUS_CODE] = code;
	result[RM_INVOKE_PARAMS_ONSTATUS_DESCRIPTION] = description;
	result[RM_INVOKE_PARAMS_ONSTATUS_DETAILS] = details;
	result[RM_INVOKE_PARAMS_ONSTATUS_CLIENTID] = clientId;

	return GenericMessageFactory::GetInvokeOnStatus(channelId, streamId,
			timeStamp, isAbsolute, requestId, result);
}

Variant StreamMessageFactory::GetInvokeOnStatusStreamPlayFailed(Variant &request,
		string streamName) {
	return GetInvokeOnStatusStreamPlayFailed(
			VH_CI(request),
			VH_SI(request),
			M_INVOKE_ID(request),
			streamName);
}

Variant StreamMessageFactory::GetInvokeOnStatusStreamPlayFailed(uint32_t channelId,
		uint32_t streamId, double requestId, string streamName) {
	Variant result;

	result[RM_INVOKE_PARAMS_ONSTATUS_LEVEL] = RM_INVOKE_PARAMS_RESULT_LEVEL_ERROR;
	result[RM_INVOKE_PARAMS_ONSTATUS_CODE] = "NetStream.Play.Failed";
	result[RM_INVOKE_PARAMS_ONSTATUS_DESCRIPTION] = format("Fail to play %s", STR(streamName));
	result[RM_INVOKE_PARAMS_ONSTATUS_DETAILS] = streamName;
	result[RM_INVOKE_PARAMS_ONSTATUS_CLIENTID] = "";

	return GenericMessageFactory::GetInvokeOnStatus(channelId, streamId,
			0, false, requestId, result);
}

Variant StreamMessageFactory::GetInvokeOnStatusStreamPlayReset(uint32_t channelId,
		uint32_t streamId, double timeStamp, bool isAbsolute,
		double requestId, string description, string details, string clientId) {
	Variant result;

	result[RM_INVOKE_PARAMS_ONSTATUS_LEVEL] =
			RM_INVOKE_PARAMS_ONSTATUS_LEVEL_STATUS;
	result[RM_INVOKE_PARAMS_ONSTATUS_CODE] =
			RM_INVOKE_PARAMS_ONSTATUS_CODE_NETSTREAMPLAYRESET;
	result[RM_INVOKE_PARAMS_ONSTATUS_DESCRIPTION] = description;
	result[RM_INVOKE_PARAMS_ONSTATUS_DETAILS] = details;
	result[RM_INVOKE_PARAMS_ONSTATUS_CLIENTID] = clientId;

	return GenericMessageFactory::GetInvokeOnStatus(channelId, streamId,
			timeStamp, isAbsolute, requestId, result);
}

Variant StreamMessageFactory::GetInvokeOnStatusStreamPlayStart(uint32_t channelId,
		uint32_t streamId, double timeStamp, bool isAbsolute,
		double requestId, string description, string details, string clientId) {
	Variant result;

	result[RM_INVOKE_PARAMS_ONSTATUS_LEVEL] =
			RM_INVOKE_PARAMS_ONSTATUS_LEVEL_STATUS;
	result[RM_INVOKE_PARAMS_ONSTATUS_CODE] =
			RM_INVOKE_PARAMS_ONSTATUS_CODE_NETSTREAMPLAYSTART;
	result[RM_INVOKE_PARAMS_ONSTATUS_DESCRIPTION] = description;
	result[RM_INVOKE_PARAMS_ONSTATUS_DETAILS] = details;
	result[RM_INVOKE_PARAMS_ONSTATUS_CLIENTID] = clientId;

	return GenericMessageFactory::GetInvokeOnStatus(channelId, streamId,
			timeStamp, isAbsolute, requestId, result);
}

Variant StreamMessageFactory::GetInvokeOnStatusStreamPlayStop(uint32_t channelId,
		uint32_t streamId, double timeStamp, bool isAbsolute,
		double requestId, string description, string details, string clientId) {
	Variant result;

	result[RM_INVOKE_PARAMS_ONSTATUS_LEVEL] =
			RM_INVOKE_PARAMS_ONSTATUS_LEVEL_STATUS;
	result[RM_INVOKE_PARAMS_ONSTATUS_CODE] =
			"NetStream.Play.Stop";
	result[RM_INVOKE_PARAMS_ONSTATUS_DESCRIPTION] = description;
	result[RM_INVOKE_PARAMS_ONSTATUS_DETAILS] = details;
	result[RM_INVOKE_PARAMS_ONSTATUS_CLIENTID] = clientId;
	result["reason"] = "";

	return GenericMessageFactory::GetInvokeOnStatus(channelId, streamId,
			timeStamp, isAbsolute, requestId, result);
}

Variant StreamMessageFactory::GetInvokeOnStatusStreamPlayUnpublishNotify(uint32_t channelId,
		uint32_t streamId, double timeStamp, bool isAbsolute,
		double requestId, string description, string clientId) {
	Variant result;

	result[RM_INVOKE_PARAMS_ONSTATUS_LEVEL] =
			RM_INVOKE_PARAMS_ONSTATUS_LEVEL_STATUS;
	result[RM_INVOKE_PARAMS_ONSTATUS_CODE] =
			"NetStream.Play.UnpublishNotify";
	result[RM_INVOKE_PARAMS_ONSTATUS_DESCRIPTION] = description;
	result[RM_INVOKE_PARAMS_ONSTATUS_CLIENTID] = clientId;

	return GenericMessageFactory::GetInvokeOnStatus(channelId, streamId,
			timeStamp, isAbsolute, requestId, result);
}

Variant StreamMessageFactory::GetInvokeOnStatusStreamSeekNotify(uint32_t channelId,
		uint32_t streamId, double timeStamp, bool isAbsolute,
		double requestId, string description, string details, string clientId) {
	Variant result;

	result[RM_INVOKE_PARAMS_ONSTATUS_LEVEL] =
			RM_INVOKE_PARAMS_ONSTATUS_LEVEL_STATUS;
	result[RM_INVOKE_PARAMS_ONSTATUS_CODE] =
			RM_INVOKE_PARAMS_ONSTATUS_CODE_NETSTREAMSEEKNOTIFY;
	result[RM_INVOKE_PARAMS_ONSTATUS_DESCRIPTION] = description;
	result[RM_INVOKE_PARAMS_ONSTATUS_DETAILS] = details;
	result[RM_INVOKE_PARAMS_ONSTATUS_CLIENTID] = clientId;

	return GenericMessageFactory::GetInvokeOnStatus(channelId, streamId,
			timeStamp, isAbsolute, requestId, result);
}

Variant StreamMessageFactory::GetInvokeOnStatusStreamPauseNotify(uint32_t channelId,
		uint32_t streamId, double timeStamp, bool isAbsolute,
		double requestId, string description, string details, string clientId) {
	Variant result;

	result[RM_INVOKE_PARAMS_ONSTATUS_LEVEL] =
			RM_INVOKE_PARAMS_ONSTATUS_LEVEL_STATUS;
	result[RM_INVOKE_PARAMS_ONSTATUS_CODE] =
			RM_INVOKE_PARAMS_ONSTATUS_CODE_NETSTREAMPAUSENOTIFY;
	result[RM_INVOKE_PARAMS_ONSTATUS_DESCRIPTION] = description;
	result[RM_INVOKE_PARAMS_ONSTATUS_DETAILS] = details;
	result[RM_INVOKE_PARAMS_ONSTATUS_CLIENTID] = clientId;

	return GenericMessageFactory::GetInvokeOnStatus(channelId, streamId,
			timeStamp, isAbsolute, requestId, result);
}

Variant StreamMessageFactory::GetInvokeOnStatusStreamUnpauseNotify(uint32_t channelId,
		uint32_t streamId, double timeStamp, bool isAbsolute,
		double requestId, string description, string details, string clientId) {
	Variant result;

	result[RM_INVOKE_PARAMS_ONSTATUS_LEVEL] =
			RM_INVOKE_PARAMS_ONSTATUS_LEVEL_STATUS;
	result[RM_INVOKE_PARAMS_ONSTATUS_CODE] =
			RM_INVOKE_PARAMS_ONSTATUS_CODE_NETSTREAMUNPAUSENOTIFY;
	result[RM_INVOKE_PARAMS_ONSTATUS_DESCRIPTION] = description;
	result[RM_INVOKE_PARAMS_ONSTATUS_DETAILS] = details;
	result[RM_INVOKE_PARAMS_ONSTATUS_CLIENTID] = clientId;

	return GenericMessageFactory::GetInvokeOnStatus(channelId, streamId,
			timeStamp, isAbsolute, requestId, result);
}

Variant StreamMessageFactory::GetNotifyRtmpSampleAccess(uint32_t channelId,
		uint32_t streamId, double timeStamp, bool isAbsolute,
		bool unknown1, bool unknown2) {
	Variant parameters;
	parameters[(uint32_t) 0] = (bool)unknown1;
	parameters[(uint32_t) 1] = (bool)unknown2;

	return GenericMessageFactory::GetNotify(channelId, streamId, timeStamp,
			isAbsolute, "|RtmpSampleAccess", parameters);
}

Variant StreamMessageFactory::GetNotifyOnMetaData(uint32_t channelId,
		uint32_t streamId, double timeStamp, bool isAbsolute,
		Variant metadata) {
	Variant parameters;
	metadata[HTTP_HEADERS_SERVER] = HTTP_HEADERS_SERVER_US;
	parameters[(uint32_t) 0] = metadata;
	return GenericMessageFactory::GetNotify(channelId, streamId, timeStamp,
			isAbsolute, "onMetaData", parameters);
}

Variant StreamMessageFactory::GetNotifyOnPlayStatusPlayComplete(uint32_t channelId,
		uint32_t streamId, double timeStamp, bool isAbsolute,
		double bytes, double duration) {
	Variant parameters;
	parameters[(uint32_t) 0]["bytes"] = bytes;
	parameters[(uint32_t) 0]["duration"] = duration;
	parameters[(uint32_t) 0]["level"] = "status";
	parameters[(uint32_t) 0]["code"] = "NetStream.Play.Complete";
	return GenericMessageFactory::GetNotify(channelId, streamId, timeStamp,
			isAbsolute, "onPlayStatus", parameters);
}

Variant StreamMessageFactory::GetNotifyOnStatusDataStart(uint32_t channelId,
		uint32_t streamId, double timeStamp, bool isAbsolute) {
	Variant parameters;
	parameters[(uint32_t) 0]["code"] = "NetStream.Data.Start";
	return GenericMessageFactory::GetNotify(channelId, streamId, timeStamp,
			isAbsolute, "onStatus", parameters);
}

Variant StreamMessageFactory::GetFlexStreamSend(uint32_t channelId,
		uint32_t streamId, double timeStamp, bool isAbsolute, string function,
		Variant &parameters) {
	Variant result;

	VH(result, HT_FULL, channelId, timeStamp, 0,
			RM_HEADER_MESSAGETYPE_FLEXSTREAMSEND, streamId, isAbsolute);

	result[RM_FLEXSTREAMSEND][RM_FLEXSTREAMSEND_UNKNOWNBYTE] = (uint8_t) 0;
	result[RM_FLEXSTREAMSEND][RM_FLEXSTREAMSEND_PARAMS][(uint32_t) 0] = function;

	FOR_MAP(parameters, string, Variant, i) {
		result[RM_FLEXSTREAMSEND][RM_FLEXSTREAMSEND_PARAMS][
				result[RM_FLEXSTREAMSEND][RM_FLEXSTREAMSEND_PARAMS].MapSize()] = MAP_VAL(i);
	}

	return result;
}
#endif /* HAS_PROTOCOL_RTMP */

