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
#include "protocols/rtmp/messagefactories/connectionmessagefactory.h"
#include "protocols/rtmp/rtmpprotocolserializer.h"

Variant ConnectionMessageFactory::GetPong() {
	Variant result;

	VH(result, HT_FULL, 2, 0, 0, RM_HEADER_MESSAGETYPE_USRCTRL,
			0, true);

	M_USRCTRL_TYPE(result) = (uint16_t) RM_USRCTRL_TYPE_PING_RESPONSE;
	M_USRCTRL_TYPE_STRING(result) =
			RTMPProtocolSerializer::GetUserCtrlTypeString(RM_USRCTRL_TYPE_PING_RESPONSE);
	uint64_t ts = (uint64_t) time(NULL)*1000;
	M_USRCTRL_PONG(result) = (uint32_t) (ts & 0x00000000ffffffffLL);

	return result;
}

Variant ConnectionMessageFactory::GetInvokeConnect(string appName, string tcUrl,
		double audioCodecs, double capabilities, string flashVer, bool fPad,
		string pageUrl, string swfUrl, double videoCodecs, double videoFunction,
		double objectEncoding) {

	Variant connectRequest;

	connectRequest[(uint32_t) 0][RM_INVOKE_PARAMS_CONNECT_APP] = appName;
	connectRequest[(uint32_t) 0][RM_INVOKE_PARAMS_CONNECT_AUDIOCODECS] = audioCodecs;
	connectRequest[(uint32_t) 0][RM_INVOKE_PARAMS_CONNECT_FLASHVER] = flashVer;
	connectRequest[(uint32_t) 0][RM_INVOKE_PARAMS_CONNECT_FPAD] = (bool)fPad;

	if (pageUrl != "")
		connectRequest[(uint32_t) 0][RM_INVOKE_PARAMS_CONNECT_PAGEURL] = pageUrl;
	else
		connectRequest[(uint32_t) 0][RM_INVOKE_PARAMS_CONNECT_PAGEURL] = Variant();

	if (swfUrl != "")
		connectRequest[(uint32_t) 0][RM_INVOKE_PARAMS_CONNECT_SWFURL] = swfUrl;
	else
		connectRequest[(uint32_t) 0][RM_INVOKE_PARAMS_CONNECT_SWFURL] = Variant();

	if (tcUrl != "")
		connectRequest[(uint32_t) 0][RM_INVOKE_PARAMS_CONNECT_TCURL] = tcUrl;
	else
		connectRequest[(uint32_t) 0][RM_INVOKE_PARAMS_CONNECT_TCURL] = Variant();

	connectRequest[(uint32_t) 0][RM_INVOKE_PARAMS_CONNECT_VIDEOCODECS] = videoCodecs;
	connectRequest[(uint32_t) 0][RM_INVOKE_PARAMS_CONNECT_VIDEOFUNCTION] = videoFunction;
	connectRequest[(uint32_t) 0]["objectEncoding"] = objectEncoding;

	return GenericMessageFactory::GetInvoke(3, 0, 0, false, 1, RM_INVOKE_FUNCTION_CONNECT, connectRequest);
}

Variant ConnectionMessageFactory::GetInvokeClose() {
	Variant close;
	close[(uint32_t) 0] = Variant();
	return GenericMessageFactory::GetInvoke(3, 0, 0, false, 0, RM_INVOKE_FUNCTION_CLOSE,
			close);
}

Variant ConnectionMessageFactory::GetInvokeConnectResult(uint32_t channelId,
		uint32_t streamId, uint32_t requestId, string level, string code,
		string decription, double objectEncoding) {
	Variant firstParams;

	firstParams["fmsVer"] = "FMS/3,0,1,123";
	firstParams["capabilities"] = 31.0;

	Variant secondParams;
	secondParams[RM_INVOKE_PARAMS_RESULT_LEVEL] = level;
	secondParams[RM_INVOKE_PARAMS_RESULT_CODE] = code;
	secondParams[RM_INVOKE_PARAMS_RESULT_DESCRIPTION] = decription;
	secondParams[RM_INVOKE_PARAMS_RESULT_OBJECTENCODING] = objectEncoding;


	return GenericMessageFactory::GetInvokeResult(channelId, streamId, requestId,
			firstParams, secondParams);
}

Variant ConnectionMessageFactory::GetInvokeConnectResult(Variant request, string level,
		string code, string decription) {

	double objectEncoding = 0;
	if (M_INVOKE_PARAM(request, 0).HasKey(RM_INVOKE_PARAMS_RESULT_OBJECTENCODING))
		objectEncoding = M_INVOKE_PARAM(request, 0)[RM_INVOKE_PARAMS_RESULT_OBJECTENCODING];
	return GetInvokeConnectResult(
			VH_CI(request),
			VH_SI(request),
			M_INVOKE_ID(request),
			level,
			code,
			decription,
			objectEncoding);
}

Variant ConnectionMessageFactory::GetInvokeConnectError(uint32_t channelId, uint32_t streamId,
		uint32_t requestId, string level, string code, string decription) {

	Variant secondParams;
	secondParams[RM_INVOKE_PARAMS_RESULT_LEVEL] = level;
	secondParams[RM_INVOKE_PARAMS_RESULT_CODE] = code;
	secondParams[RM_INVOKE_PARAMS_RESULT_DESCRIPTION] = decription;


	return GenericMessageFactory::GetInvokeError(channelId, streamId, requestId,
			Variant(), secondParams);
}

Variant ConnectionMessageFactory::GetInvokeConnectError(Variant request,
		string decription, string level, string code) {
	double objectEncoding = 0;
	if (M_INVOKE_PARAM(request, 0).HasKey(RM_INVOKE_PARAMS_RESULT_OBJECTENCODING))
		objectEncoding = M_INVOKE_PARAM(request, 0)[RM_INVOKE_PARAMS_RESULT_OBJECTENCODING];
	return GetInvokeConnectError(
			VH_CI(request),
			VH_SI(request),
			M_INVOKE_ID(request),
			level,
			code,
			decription);
}
#endif /* HAS_PROTOCOL_RTMP */

