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
#include "protocols/rtmp/messagefactories/genericmessagefactory.h"
#include "protocols/rtmp/header.h"

Variant GenericMessageFactory::GetChunkSize(uint32_t chunkSize) {
	Variant result;

	VH(result, HT_FULL, 2, 0, 0, RM_HEADER_MESSAGETYPE_CHUNKSIZE, 0, true);

	M_CHUNKSIZE(result) = (uint32_t) chunkSize;

	return result;
}

Variant GenericMessageFactory::GetAck(uint64_t amount) {
	Variant result;

	VH(result, HT_FULL, 2, 0, 0, RM_HEADER_MESSAGETYPE_ACK, 0, true);

	result[RM_ACK] = (uint32_t) amount;

	return result;
}

Variant GenericMessageFactory::GetWinAckSize(uint32_t value) {
	Variant result;

	VH(result, HT_FULL, 2, 0, 0, RM_HEADER_MESSAGETYPE_WINACKSIZE, 0, true);

	result[RM_WINACKSIZE] = value;

	return result;
}

Variant GenericMessageFactory::GetPeerBW(uint32_t value, uint8_t type) {
	Variant result;

	VH(result, HT_FULL, 2, 0, 0, RM_HEADER_MESSAGETYPE_PEERBW, 0, true);

	result[RM_PEERBW][RM_PEERBW_VALUE] = (uint32_t) value;
	result[RM_PEERBW][RM_PEERBW_TYPE] = type;

	return result;
}

Variant GenericMessageFactory::GetAbortMessage(uint32_t channelId) {
	Variant result;

	VH(result, HT_FULL, 2, 0, 0, RM_HEADER_MESSAGETYPE_ABORTMESSAGE, 0, true);

	M_ABORTMESSAGE(result) = (uint32_t) channelId;

	return result;
}

Variant GenericMessageFactory::GetInvoke(uint32_t channelId, uint32_t streamId,
		double timeStamp, bool isAbsolute, double requestId,
		string functionName, Variant &parameters) {
	Variant result;

	VH(result, HT_FULL, channelId, timeStamp, 0, RM_HEADER_MESSAGETYPE_INVOKE,
			streamId, isAbsolute);

	M_INVOKE_ID(result) = (double) requestId;
	M_INVOKE_FUNCTION(result) = functionName;

	uint32_t index = 0;

	FOR_MAP(parameters, string, Variant, i) {
		M_INVOKE_PARAM(result, index) = MAP_VAL(i);
		index++;
	}

	return result;
}

Variant GenericMessageFactory::GetNotify(uint32_t channelId,
		uint32_t streamId, double timeStamp, bool isAbsolute,
		string handlerName, Variant params) {
	Variant result;

	VH(result, HT_FULL, channelId, timeStamp, 0, RM_HEADER_MESSAGETYPE_NOTIFY,
			streamId, isAbsolute);

	M_NOTIFY_PARAM(result, 0) = handlerName;

	for (uint32_t i = 0; i < params.MapSize(); i++) {
		M_NOTIFY_PARAM(result, i + 1) = params[i];
	}

	return result;
}

Variant GenericMessageFactory::GetInvokeOnBWDone(double kbpsSpeed) {
	Variant parameters;
	parameters.PushToArray(Variant());
	parameters.PushToArray(kbpsSpeed);
	return GetInvoke(
			3,
			0,
			0, false,
			0,
			RM_INVOKE_FUNCTION_ONBWDONE,
			parameters);
}

Variant GenericMessageFactory::GetInvokeOnStatus(uint32_t channelId, uint32_t streamId,
		double timeStamp, bool isAbsolute, double requestId, Variant &message) {

	Variant result;

	result[(uint32_t) 0] = Variant();
	result[(uint32_t) 1] = message;

	return GetInvoke(channelId, streamId, timeStamp, isAbsolute, requestId,
			RM_INVOKE_FUNCTION_ONSTATUS, result);
}

Variant GenericMessageFactory::GetInvokeResult(uint32_t channelId,
		uint32_t streamId, double requestId, Variant firstParam,
		Variant &secondParam) {
	Variant result;

	result[(uint32_t) 0] = firstParam;
	result[(uint32_t) 1] = secondParam;

	return GetInvoke(channelId, streamId, 0, false, requestId,
			RM_INVOKE_FUNCTION_RESULT, result);
}

Variant GenericMessageFactory::GetInvokeResult(Variant &request, Variant &parameters) {

	return GetInvoke(
			VH_CI(request),
			VH_SI(request),
			0, false,
			M_INVOKE_ID(request),
			RM_INVOKE_FUNCTION_RESULT,
			parameters);
}

Variant GenericMessageFactory::GetInvokeError(uint32_t channelId, uint32_t streamId,
		double requestId, Variant firstParam, Variant &secondParam) {
	Variant result;

	result[(uint32_t) 0] = firstParam;
	result[(uint32_t) 1] = secondParam;

	return GetInvoke(channelId, streamId, 0, false, requestId,
			RM_INVOKE_FUNCTION_ERROR, result);
}

Variant GenericMessageFactory::GetInvokeError(Variant &request, Variant &parameters) {
	return GetInvoke(
			VH_CI(request),
			VH_SI(request),
			0, false,
			M_INVOKE_ID(request),
			RM_INVOKE_FUNCTION_ERROR,
			parameters);
}

Variant GenericMessageFactory::GetInvokeCallFailedError(Variant &request) {

	Variant secondParams;
	secondParams[RM_INVOKE_PARAMS_RESULT_LEVEL] = RM_INVOKE_PARAMS_RESULT_LEVEL_ERROR;
	secondParams[RM_INVOKE_PARAMS_RESULT_CODE] = "NetConnection.Call.Failed";
	secondParams[RM_INVOKE_PARAMS_RESULT_DESCRIPTION] =
			format("call to function %s failed", STR(M_INVOKE_FUNCTION(request)));

	return GenericMessageFactory::GetInvokeError(
			VH_CI(request),
			VH_SI(request),
			M_INVOKE_ID(request),
			Variant(),
			secondParams);
}
#endif /* HAS_PROTOCOL_RTMP */

