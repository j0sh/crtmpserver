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

#ifdef HAS_LUA
#include "vm/lua/luaapi_handler_rtmp.h"
#include "vm/lua/luaapi_helpers.h"
#include "application/baseclientapplication.h"
#include "protocols/rtmp/basertmpappprotocolhandler.h"
#include "protocols/protocolmanager.h"
#include "vm/baseappvirtualmachine.h"
#include "protocols/rtmp/messagefactories/messagefactories.h"

#define LUA_HANDLER_RTMP_GET_PROTOCOL(pProtocol,protocolId,L) \
LUA_GET_PROTOCOL(pProtocol,protocolId,L); \
if ((pProtocol->GetType() != PT_INBOUND_RTMP) \
		&& (pProtocol->GetType() != PT_OUTBOUND_RTMP)) { \
	FATAL("Protocol %d has invalid type",protocolId); \
	lua_pushboolean(L, false); \
	return 1; \
} \
do{}while(0)

#define LUA_HANDLER_RTMP_GET_HANDLER(pHandler,pApplication,L) \
BaseRTMPAppProtocolHandler *pHandler = NULL; \
do{ \
	string ____tempString="rtmp"; \
	pHandler=pApplication->GetProtocolHandler<BaseRTMPAppProtocolHandler > (____tempString); \
	if (pHandler == NULL) {\
		lua_pushboolean(L, false); \
		return 1; \
	} \
} while(0)

#define LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_H,NH) \
int luaapi_handler_rtmp_H(lua_State *L) { \
	LUA_INIT_PARAMS(params, L); \
	LUA_READ_PARAM(params, uint32_t, _V_NUMERIC, protocolId, 0, 0, true); \
	LUA_READ_PARAM(params, Variant, V_MAP, request, Variant(), 1, true); \
	LUA_GET_APPLICATION(pApp, L); \
	LUA_HANDLER_RTMP_GET_PROTOCOL(pProtocol, protocolId, L); \
	LUA_HANDLER_RTMP_GET_HANDLER(pHandler, pApp, L); \
	bool result = pHandler->BaseRTMPAppProtocolHandler::NH( \
		(BaseRTMPProtocol*) pProtocol, request); \
	lua_pushboolean(L, result); \
	return 1; \
}

#define LUA_HANDLER_RTMP_STANDARD_RESPONSE_CALL(luaapi_handler_rtmp_H,NH) \
int luaapi_handler_rtmp_H(lua_State *L) { \
	LUA_INIT_PARAMS(params, L); \
	LUA_READ_PARAM(params, uint32_t, _V_NUMERIC, protocolId, 0, 0, true); \
	LUA_READ_PARAM(params, Variant, V_MAP, request, Variant(), 1, true); \
	LUA_READ_PARAM(params, Variant, V_MAP, response, Variant(), 2, true); \
	LUA_GET_APPLICATION(pApp, L); \
	LUA_HANDLER_RTMP_GET_PROTOCOL(pProtocol, protocolId, L); \
	LUA_HANDLER_RTMP_GET_HANDLER(pHandler, pApp, L); \
	bool result = pHandler->BaseRTMPAppProtocolHandler::NH( \
		(BaseRTMPProtocol*) pProtocol, request,response); \
	lua_pushboolean(L, result); \
	return 1; \
}

namespace app_vmapp {

	int luaapi_handler_rtmp_pullExternalStream(lua_State *L) {
		LUA_INIT_PARAMS(params, L);
		LUA_READ_PARAM(params, Variant, V_MAP, variantUri, Variant(), 0, true);
		LUA_READ_PARAM(params, Variant, V_MAP, streamConfig, Variant(), 1, true);

		URI uri;
		if (!URI::FromVariant(variantUri, uri)) {
			FATAL("Invalid URI");
			lua_pushboolean(L, false);
			return 1;
		}
		LUA_GET_APPLICATION(pApp, L);
		LUA_HANDLER_RTMP_GET_HANDLER(pHandler, pApp, L);
		bool result = pHandler->BaseRTMPAppProtocolHandler::PullExternalStream(uri, streamConfig);
		lua_pushboolean(L, result);
		return 1;
	}

	int luaapi_handler_rtmp_pushLocalStream(lua_State *L) {
		NYIA;
		return 0;
	}

	int luaapi_handler_rtmp_outboundConnectionEstablished(lua_State *L) {
		LUA_INIT_PARAMS(params, L);
		LUA_READ_PARAM(params, uint32_t, _V_NUMERIC, protocolId, 0, 0, true);
		LUA_GET_APPLICATION(pApp, L);
		LUA_HANDLER_RTMP_GET_PROTOCOL(pProtocol, protocolId, L);
		LUA_HANDLER_RTMP_GET_HANDLER(pHandler, pApp, L);
		if (pProtocol->GetType() != PT_OUTBOUND_RTMP) {
			FATAL("Protocol %d has invalid type", protocolId);
			lua_pushboolean(L, false);
			return 1;
		}
		bool result = pHandler->BaseRTMPAppProtocolHandler::OutboundConnectionEstablished(
				(OutboundRTMPProtocol *) pProtocol);
		lua_pushboolean(L, result);
		return 1;
	}

	int luaapi_handler_rtmp_authenticateInbound(lua_State *L) {
		NYIA;
		return 0;
	}

	LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_inboundMessageAvailable, InboundMessageAvailable);
	LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_processWinAckSize, ProcessWinAckSize);
	LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_processPeerBW, ProcessPeerBW);
	LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_processAck, ProcessAck);
	LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_processChunkSize, ProcessChunkSize);
	LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_processUsrCtrl, ProcessUsrCtrl);
	LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_processNotify, ProcessNotify);
	LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_processFlexStreamSend, ProcessFlexStreamSend);
	LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_processSharedObject, ProcessSharedObject);
	LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_processInvoke, ProcessInvoke);
	LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_processInvokeConnect, ProcessInvokeConnect);
	LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_processInvokeCreateStream, ProcessInvokeCreateStream);
	LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_processInvokePublish, ProcessInvokePublish);
	LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_processInvokeSeek, ProcessInvokeSeek);
	LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_processInvokePlay, ProcessInvokePlay);
	LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_processInvokePauseRaw, ProcessInvokePauseRaw);
	LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_processInvokePause, ProcessInvokePause);
	LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_processInvokeCloseStream, ProcessInvokeCloseStream);
	LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_processInvokeReleaseStream, ProcessInvokeReleaseStream);
	LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_processInvokeDeleteStream, ProcessInvokeDeleteStream);
	LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_processInvokeOnStatus, ProcessInvokeOnStatus);
	LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_processInvokeFCPublish, ProcessInvokeFCPublish);
	LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_processInvokeGetStreamLength, ProcessInvokeGetStreamLength);
	LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_processInvokeOnBWDone, ProcessInvokeOnBWDone);
	LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_processInvokeGeneric, ProcessInvokeGeneric);
	LUA_HANDLER_RTMP_STANDARD_CALL(luaapi_handler_rtmp_processInvokeResultWithoutRequest, ProcessInvokeResult);
	LUA_HANDLER_RTMP_STANDARD_RESPONSE_CALL(luaapi_handler_rtmp_processInvokeResultWithRequest, ProcessInvokeResult);
	LUA_HANDLER_RTMP_STANDARD_RESPONSE_CALL(luaapi_handler_rtmp_processInvokeConnectResult, ProcessInvokeConnectResult);
	LUA_HANDLER_RTMP_STANDARD_RESPONSE_CALL(luaapi_handler_rtmp_processInvokeCreateStreamResult, ProcessInvokeCreateStreamResult);
	LUA_HANDLER_RTMP_STANDARD_RESPONSE_CALL(luaapi_handler_rtmp_processInvokeFCSubscribeResult, ProcessInvokeFCSubscribeResult);
	LUA_HANDLER_RTMP_STANDARD_RESPONSE_CALL(luaapi_handler_rtmp_processInvokeGenericResult, ProcessInvokeGenericResult);

	int luaapi_handler_rtmp_generateMetaFiles(lua_State *L) {
		LUA_GET_APPLICATION(pApp, L);
		LUA_HANDLER_RTMP_GET_HANDLER(pHandler, pApp, L);
		pHandler->BaseRTMPAppProtocolHandler::GenerateMetaFiles();
		lua_pushboolean(L, true);
		return 1;
	}

	int luaapi_handler_rtmp_getMetaData(lua_State *L) {
		LUA_INIT_PARAMS(params, L);
		LUA_READ_PARAM(params, string, V_STRING, streamName, "", 0, true);
		LUA_GET_APPLICATION(pApp, L);
		LUA_HANDLER_RTMP_GET_HANDLER(pHandler, pApp, L);
		Variant result = pHandler->BaseRTMPAppProtocolHandler::GetMetaData(streamName, true);
		if (!PushVariant(L, result)) {
			lua_pushboolean(L, false);
		}
		return 1;
	}

	int luaapi_handler_rtmp_sendRequest(lua_State *L) {
		LUA_INIT_PARAMS(params, L);
		LUA_READ_PARAM(params, uint32_t, _V_NUMERIC, protocolId, 0, 0, true);
		LUA_READ_PARAM(params, string, V_STRING, functionName, "", 1, true);
		LUA_READ_PARAM(params, bool, V_BOOL, trackResponse, false, 2, true);
		LUA_GET_APPLICATION(pApp, L);
		LUA_HANDLER_RTMP_GET_PROTOCOL(pProtocol, protocolId, L);
		LUA_HANDLER_RTMP_GET_HANDLER(pHandler, pApp, L);
		params.RemoveAt(0);
		params.RemoveAt(1);
		params[(uint32_t) 2] = Variant();
		Variant request = GenericMessageFactory::GetInvoke(3, 0, 0, false, 0,
				functionName, params);
		lua_pushboolean(L, pHandler->SendRTMPMessage(
				(BaseRTMPProtocol *) pProtocol, request, trackResponse));
		return 1;
	}

	int luaapi_handler_rtmp_sendResponse(lua_State *L) {
		LUA_INIT_PARAMS(params, L);
		LUA_READ_PARAM(params, uint32_t, _V_NUMERIC, protocolId, 0, 0, true);
		LUA_READ_PARAM(params, Variant, V_MAP, request, Variant(), 1, true);
		LUA_GET_APPLICATION(pApp, L);
		LUA_HANDLER_RTMP_GET_PROTOCOL(pProtocol, protocolId, L);
		LUA_HANDLER_RTMP_GET_HANDLER(pHandler, pApp, L);
		params.RemoveAt(0);
		params[(uint32_t) 1] = Variant();
		Variant response = GenericMessageFactory::GetInvokeResult(request, params);
		lua_pushboolean(L, pHandler->SendRTMPMessage(
				(BaseRTMPProtocol *) pProtocol, response));
		return 1;
	}
}
#endif	/* HAS_LUA */
