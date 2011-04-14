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
#include "vm/lua/luaappvirtualmachine.h"
#include "vm/lua/luaapi_helpers.h"
#include "vm/lua/luaapi_generics.h"
#include "vm/lua/luaapi_application.h"
#include "vm/lua/luaapi_protocols.h"
#include "vm/lua/luaapi_handler_rtmp.h"
#include "vmlua/basevmlua.h"
#include "application/baseclientapplication.h"
#include "protocols/rtmp/basertmpappprotocolhandler.h"
#include "protocols/rtmp/outboundrtmpprotocol.h"
#include "protocols/ts/innettsstream.h"
#include "vmapp.h"
using namespace app_vmapp;

LuaAppVirtualMachine::LuaAppVirtualMachine() {
	_pVM = NULL;
	memset(_functionsMappingIds, 0, sizeof (_functionsMappingIds));
}

LuaAppVirtualMachine::~LuaAppVirtualMachine() {
	Shutdown();
}

bool LuaAppVirtualMachine::HasFunction(APIFunctionID functionId) {
	return _functionsMappingIds[functionId] != 0;
}

bool LuaAppVirtualMachine::Call(APIFunctionID functionId) {
	if (_functionsMappingIds[functionId] == 0) {
		ASSERT("Function id %d not found", functionId);
	}
	Variant results;
	if (!_pVM->CallWithoutParams(_functionsMappingIds[functionId], results)) {
		FATAL("Unable to call function ID %d", functionId);
		return false;
	}
	LUA_READ_PARAM(results, bool, V_BOOL, result, false, 0, true);
	return result;
}

bool LuaAppVirtualMachine::Call(APIFunctionID functionId, Variant &variant) {
	if (_functionsMappingIds[functionId] == 0) {
		ASSERT("Function id %d not found", functionId);
	}
	Variant parameters;
	Variant results;
	parameters.PushToArray(variant);
	if (!_pVM->CallWithParams(_functionsMappingIds[functionId], parameters, results)) {
		FATAL("Unable to call function ID %d", functionId);
		return false;
	}
	LUA_READ_PARAM(results, bool, V_BOOL, result, false, 0, true);
	return result;
}

bool LuaAppVirtualMachine::Call(APIFunctionID functionId, BaseStream *pBaseStream) {
	if (_functionsMappingIds[functionId] == 0) {
		ASSERT("Function id %d not found", functionId);
	}
	Variant parameters;
	Variant results;
	parameters.PushToArray(pBaseStream->GetUniqueId());
	if (!_pVM->CallWithParams(_functionsMappingIds[functionId], parameters, results)) {
		FATAL("Unable to call function ID %d", functionId);
		return false;
	}
	LUA_READ_PARAM(results, bool, V_BOOL, result, false, 0, true);
	return result;
}

bool LuaAppVirtualMachine::Call(APIFunctionID functionId, InNetTSStream *pInNetTSStream) {
	if (_functionsMappingIds[functionId] == 0) {
		ASSERT("Function id %d not found", functionId);
	}
	Variant parameters;
	Variant results;
	parameters.PushToArray(pInNetTSStream->GetUniqueId());
	if (!_pVM->CallWithParams(_functionsMappingIds[functionId], parameters, results)) {
		FATAL("Unable to call function ID %d", functionId);
		return false;
	}
	LUA_READ_PARAM(results, bool, V_BOOL, result, false, 0, true);
	return result;
}

bool LuaAppVirtualMachine::Call(APIFunctionID functionId, BaseProtocol *pProtocol) {
	if (_functionsMappingIds[functionId] == 0) {
		ASSERT("Function id %d not found", functionId);
	}
	Variant parameters;
	Variant results;
	parameters.PushToArray(pProtocol->GetId());
	if (!_pVM->CallWithParams(_functionsMappingIds[functionId], parameters, results)) {
		FATAL("Unable to call function ID %d", functionId);
		return false;
	}
	LUA_READ_PARAM(results, bool, V_BOOL, result, false, 0, true);
	return result;
}

bool LuaAppVirtualMachine::Call(APIFunctionID functionId, OutboundRTMPProtocol *pProtocol) {
	if (_functionsMappingIds[functionId] == 0) {
		ASSERT("Function id %d not found", functionId);
	}
	Variant parameters;
	Variant results;
	parameters.PushToArray(pProtocol->GetId());
	if (!_pVM->CallWithParams(_functionsMappingIds[functionId], parameters, results)) {
		FATAL("Unable to call function ID %d", functionId);
		return false;
	}
	LUA_READ_PARAM(results, bool, V_BOOL, result, false, 0, true);
	return result;
}

bool LuaAppVirtualMachine::Call(APIFunctionID functionId, BaseRTMPProtocol *pProtocol,
		Variant &variant) {
	if (_functionsMappingIds[functionId] == 0) {
		ASSERT("Function id %d not found", functionId);
	}
	Variant parameters;
	Variant results;
	parameters.PushToArray(pProtocol->GetId());
	parameters.PushToArray(variant);
	if (!_pVM->CallWithParams(_functionsMappingIds[functionId], parameters, results)) {
		FATAL("Unable to call function ID %d", functionId);
		return false;
	}
	LUA_READ_PARAM(results, bool, V_BOOL, result, false, 0, true);
	return result;
}

bool LuaAppVirtualMachine::Call(APIFunctionID functionId, BaseRTMPProtocol *pProtocol,
		Variant &variant1, Variant &variant2) {
	if (_functionsMappingIds[functionId] == 0) {
		ASSERT("Function id %d not found", functionId);
	}
	Variant parameters;
	Variant results;
	parameters.PushToArray(pProtocol->GetId());
	parameters.PushToArray(variant1);
	parameters.PushToArray(variant2);
	if (!_pVM->CallWithParams(_functionsMappingIds[functionId], parameters, results)) {
		FATAL("Unable to call function ID %d", functionId);
		return false;
	}
	LUA_READ_PARAM(results, bool, V_BOOL, result, false, 0, true);
	return result;
}

bool LuaAppVirtualMachine::Call(APIFunctionID functionId, URI &uri, Variant &variant) {
	if (_functionsMappingIds[functionId] == 0) {
		ASSERT("Function id %d not found", functionId);
	}
	Variant parameters;
	Variant results;
	parameters.PushToArray(uri.ToVariant());
	parameters.PushToArray(variant);
	if (!_pVM->CallWithParams(_functionsMappingIds[functionId], parameters, results)) {
		FATAL("Unable to call function ID %d", functionId);
		return false;
	}
	LUA_READ_PARAM(results, bool, V_BOOL, result, false, 0, true);
	return result;
}

bool LuaAppVirtualMachine::Call(APIFunctionID functionId, BaseInStream *pInStream,
		Variant &variant) {
	if (_functionsMappingIds[functionId] == 0) {
		ASSERT("Function id %d not found", functionId);
	}
	NYIA;
	return false;
}

bool LuaAppVirtualMachine::BindAPI() {
	Shutdown();
	_pVM = new BaseVMLua();
	_pVM->SetOpaque(_pApplication);
	if (!_pVM->Initialize()) {
		FATAL("Lua virtual machine init failed");
		Shutdown();
		return false;
	}

	if (!BindAPIGenerics()) {
		FATAL("Unable to bind generics API");
		return false;
	}

	if (!BindAPIApplication()) {
		FATAL("Unable to bind generics API");
		return false;
	}

	if (!BindAPIProtocols()) {
		FATAL("Unable to bind generics API");
		return false;
	}

	if (!BindAPIHandlers()) {
		FATAL("Unable to bind handlers API");
		return false;
	}

	return true;
}

bool LuaAppVirtualMachine::Initialize(string script) {
	if (!_pVM->AddPackagePath((string) _pApplication->GetConfiguration()[CONF_APPLICATION_DIRECTORY] + "?.lua")) {
		FATAL("Unable to load additional package path");
		return false;
	}
	if (!(_pVM->LoadScriptFile(script, script))) {
		FATAL("Unable to load lua script");
		return false;
	}

	if (!GetFunctionsMappings()) {
		FATAL("Unable to load lua function mappings");
		return false;
	}

	return true;
}

bool LuaAppVirtualMachine::BindAPIGenerics() {
	struct luaL_Reg functions[] = {
		{"listFolder", luaapi_generics_listFolder},
		{"normalizePath", luaapi_generics_normalizePath},
		{"splitFileName", luaapi_generics_splitFileName},
		{NULL, NULL}
	};
	return _pVM->RegisterAPI("crtmpserver", functions);
}

bool LuaAppVirtualMachine::BindAPIApplication() {
	struct luaL_Reg functions[] = {
		{"getConfig", luaapi_application_getConfig},
		{"outboundConnectionFailed", luaapi_application_outboundConnectionFailed},
		{"pullExternalStream", luaapi_application_pullExternalStream},
		{"pushLocalStream", luaapi_application_pushLocalStream},
		{NULL, NULL}
	};
	return _pVM->RegisterAPI("crtmpserver.handlers.application", functions);
}

bool LuaAppVirtualMachine::BindAPIProtocols() {
	struct luaL_Reg functions[] = {
		{"getStackInfo", luaapi_protocols_getConfig},
		{"enqueueForDelete", luaapi_protocols_enqueueForDelete},
		{"gracefullyEnqueueForDelete", luaapi_protocols_gracefullyEnqueueForDelete},
		{NULL, NULL}
	};
	return _pVM->RegisterAPI("crtmpserver.protocols", functions);
}

bool LuaAppVirtualMachine::BindAPIHandlers() {
	if (!BindAPIHandlersRTMP()) {
		FATAL("Unable to bind RTMP handler API");
		return false;
	}

	if (!BindAPIHandlersRTSP()) {
		FATAL("Unable to bind RTSP handler API");
		return false;
	}

	if (!BindAPIHandlersMPEGTS()) {
		FATAL("Unable to bind MPEGTS handler API");
		return false;
	}

	if (!BindAPIHandlersLiveFLV()) {
		FATAL("Unable to bind LiveFLV handler API");
		return false;
	}

	return true;
}

bool LuaAppVirtualMachine::BindAPIHandlersRTMP() {

	struct luaL_Reg functions[] = {
		{"pullExternalStream", luaapi_handler_rtmp_pullExternalStream},
		{"pushLocalStream", luaapi_handler_rtmp_pushLocalStream},
		{"outboundConnectionEstablished", luaapi_handler_rtmp_outboundConnectionEstablished},
		{"authenticateInbound", luaapi_handler_rtmp_authenticateInbound},
		{"inboundMessageAvailable", luaapi_handler_rtmp_inboundMessageAvailable},
		{"processWinAckSize", luaapi_handler_rtmp_processWinAckSize},
		{"processPeerBW", luaapi_handler_rtmp_processPeerBW},
		{"processAck", luaapi_handler_rtmp_processAck},
		{"processChunkSize", luaapi_handler_rtmp_processChunkSize},
		{"processUsrCtrl", luaapi_handler_rtmp_processUsrCtrl},
		{"processNotify", luaapi_handler_rtmp_processNotify},
		{"processFlexStreamSend", luaapi_handler_rtmp_processFlexStreamSend},
		{"processSharedObject", luaapi_handler_rtmp_processSharedObject},
		{"processInvoke", luaapi_handler_rtmp_processInvoke},
		{"processInvokeConnect", luaapi_handler_rtmp_processInvokeConnect},
		{"processInvokeCreateStream", luaapi_handler_rtmp_processInvokeCreateStream},
		{"processInvokePublish", luaapi_handler_rtmp_processInvokePublish},
		{"processInvokeSeek", luaapi_handler_rtmp_processInvokeSeek},
		{"processInvokePlay", luaapi_handler_rtmp_processInvokePlay},
		{"processInvokePauseRaw", luaapi_handler_rtmp_processInvokePauseRaw},
		{"processInvokePause", luaapi_handler_rtmp_processInvokePause},
		{"processInvokeCloseStream", luaapi_handler_rtmp_processInvokeCloseStream},
		{"processInvokeReleaseStream", luaapi_handler_rtmp_processInvokeReleaseStream},
		{"processInvokeDeleteStream", luaapi_handler_rtmp_processInvokeDeleteStream},
		{"processInvokeOnStatus", luaapi_handler_rtmp_processInvokeOnStatus},
		{"processInvokeFCPublish", luaapi_handler_rtmp_processInvokeFCPublish},
		{"processInvokeGetStreamLength", luaapi_handler_rtmp_processInvokeGetStreamLength},
		{"processInvokeOnBWDone", luaapi_handler_rtmp_processInvokeOnBWDone},
		{"processInvokeGeneric", luaapi_handler_rtmp_processInvokeGeneric},
		{"processInvokeResultWithoutRequest", luaapi_handler_rtmp_processInvokeResultWithoutRequest},
		{"processInvokeResultWithRequest", luaapi_handler_rtmp_processInvokeResultWithRequest},
		{"processInvokeConnectResult", luaapi_handler_rtmp_processInvokeConnectResult},
		{"processInvokeCreateStreamResult", luaapi_handler_rtmp_processInvokeCreateStreamResult},
		{"processInvokeFCSubscribeResult", luaapi_handler_rtmp_processInvokeFCSubscribeResult},
		{"processInvokeGenericResult", luaapi_handler_rtmp_processInvokeGenericResult},
		{"generateMetaFiles", luaapi_handler_rtmp_generateMetaFiles},
		{"getMetaData", luaapi_handler_rtmp_getMetaData},
		{"sendRequest", luaapi_handler_rtmp_sendRequest},
		{"sendResponse", luaapi_handler_rtmp_sendResponse},
		{NULL, NULL}
	};
	return _pVM->RegisterAPI("crtmpserver.handlers.rtmp", functions);
}

bool LuaAppVirtualMachine::BindAPIHandlersRTSP() {
	NYI;
	return true;
}

bool LuaAppVirtualMachine::BindAPIHandlersMPEGTS() {
	return true;
}

bool LuaAppVirtualMachine::BindAPIHandlersLiveFLV() {
	NYI;
	return true;
}

bool LuaAppVirtualMachine::GetFunctionsMappings() {
	_functionsMappingIds[handlers_application_initCompleted] = _pVM->GetFunctionReference("customhandlers.application.initCompleted");
	_functionsMappingIds[handlers_application_outboundConnectionFailed] = _pVM->GetFunctionReference("customhandlers.application.outboundConnectionFailed");
	_functionsMappingIds[handlers_application_preRegisterProtocol] = _pVM->GetFunctionReference("customhandlers.application.preRegisterProtocol");
	_functionsMappingIds[handlers_application_postRegisterProtocol] = _pVM->GetFunctionReference("customhandlers.application.postRegisterProtocol");
	_functionsMappingIds[handlers_application_preUnRegisterProtocol] = _pVM->GetFunctionReference("customhandlers.application.preUnRegisterProtocol");
	_functionsMappingIds[handlers_application_postUnRegisterProtocol] = _pVM->GetFunctionReference("customhandlers.application.postUnRegisterProtocol");
	_functionsMappingIds[handlers_application_signalStreamRegistered] = _pVM->GetFunctionReference("customhandlers.application.signalStreamRegistered");
	_functionsMappingIds[handlers_application_signalStreamUnRegistered] = _pVM->GetFunctionReference("customhandlers.application.signalStreamUnRegistered");
	_functionsMappingIds[handlers_application_pullExternalStream] = _pVM->GetFunctionReference("customhandlers.application.pullExternalStream");
	_functionsMappingIds[handlers_application_pushLocalStream] = _pVM->GetFunctionReference("customhandlers.application.pushLocalStream");
	_functionsMappingIds[handlers_rtmp_preRegisterProtocol] = _pVM->GetFunctionReference("customhandlers.rtmp.preRegisterProtocol");
	_functionsMappingIds[handlers_rtmp_postRegisterProtocol] = _pVM->GetFunctionReference("customhandlers.rtmp.postRegisterProtocol");
	_functionsMappingIds[handlers_rtmp_preUnRegisterProtocol] = _pVM->GetFunctionReference("customhandlers.rtmp.preUnRegisterProtocol");
	_functionsMappingIds[handlers_rtmp_postUnRegisterProtocol] = _pVM->GetFunctionReference("customhandlers.rtmp.postUnRegisterProtocol");
	_functionsMappingIds[handlers_rtmp_pullExternalStream] = _pVM->GetFunctionReference("customhandlers.rtmp.pullExternalStream");
	_functionsMappingIds[handlers_rtmp_pushLocalStream] = _pVM->GetFunctionReference("customhandlers.rtmp.pushLocalStream");
	_functionsMappingIds[handlers_rtmp_outboundConnectionEstablished] = _pVM->GetFunctionReference("customhandlers.rtmp.outboundConnectionEstablished");
	_functionsMappingIds[handlers_rtmp_authenticateInbound] = _pVM->GetFunctionReference("customhandlers.rtmp.authenticateInbound");
	_functionsMappingIds[handlers_rtmp_inboundMessageAvailable] = _pVM->GetFunctionReference("customhandlers.rtmp.inboundMessageAvailable");
	_functionsMappingIds[handlers_rtmp_processWinAckSize] = _pVM->GetFunctionReference("customhandlers.rtmp.processWinAckSize");
	_functionsMappingIds[handlers_rtmp_processPeerBW] = _pVM->GetFunctionReference("customhandlers.rtmp.processPeerBW");
	_functionsMappingIds[handlers_rtmp_processAck] = _pVM->GetFunctionReference("customhandlers.rtmp.processAck");
	_functionsMappingIds[handlers_rtmp_processChunkSize] = _pVM->GetFunctionReference("customhandlers.rtmp.processChunkSize");
	_functionsMappingIds[handlers_rtmp_processUsrCtrl] = _pVM->GetFunctionReference("customhandlers.rtmp.processUsrCtrl");
	_functionsMappingIds[handlers_rtmp_processNotify] = _pVM->GetFunctionReference("customhandlers.rtmp.processNotify");
	_functionsMappingIds[handlers_rtmp_processFlexStreamSend] = _pVM->GetFunctionReference("customhandlers.rtmp.processFlexStreamSend");
	_functionsMappingIds[handlers_rtmp_processSharedObject] = _pVM->GetFunctionReference("customhandlers.rtmp.processSharedObject");
	_functionsMappingIds[handlers_rtmp_processInvoke] = _pVM->GetFunctionReference("customhandlers.rtmp.processInvoke");
	_functionsMappingIds[handlers_rtmp_processInvokeConnect] = _pVM->GetFunctionReference("customhandlers.rtmp.processInvokeConnect");
	_functionsMappingIds[handlers_rtmp_processInvokeCreateStream] = _pVM->GetFunctionReference("customhandlers.rtmp.processInvokeCreateStream");
	_functionsMappingIds[handlers_rtmp_processInvokePublish] = _pVM->GetFunctionReference("customhandlers.rtmp.processInvokePublish");
	_functionsMappingIds[handlers_rtmp_processInvokeSeek] = _pVM->GetFunctionReference("customhandlers.rtmp.processInvokeSeek");
	_functionsMappingIds[handlers_rtmp_processInvokePlay] = _pVM->GetFunctionReference("customhandlers.rtmp.processInvokePlay");
	_functionsMappingIds[handlers_rtmp_processInvokePauseRaw] = _pVM->GetFunctionReference("customhandlers.rtmp.processInvokePauseRaw");
	_functionsMappingIds[handlers_rtmp_processInvokePause] = _pVM->GetFunctionReference("customhandlers.rtmp.processInvokePause");
	_functionsMappingIds[handlers_rtmp_processInvokeCloseStream] = _pVM->GetFunctionReference("customhandlers.rtmp.processInvokeCloseStream");
	_functionsMappingIds[handlers_rtmp_processInvokeReleaseStream] = _pVM->GetFunctionReference("customhandlers.rtmp.processInvokeReleaseStream");
	_functionsMappingIds[handlers_rtmp_processInvokeDeleteStream] = _pVM->GetFunctionReference("customhandlers.rtmp.processInvokeDeleteStream");
	_functionsMappingIds[handlers_rtmp_processInvokeOnStatus] = _pVM->GetFunctionReference("customhandlers.rtmp.processInvokeOnStatus");
	_functionsMappingIds[handlers_rtmp_processInvokeFCPublish] = _pVM->GetFunctionReference("customhandlers.rtmp.processInvokeFCPublish");
	_functionsMappingIds[handlers_rtmp_processInvokeGetStreamLength] = _pVM->GetFunctionReference("customhandlers.rtmp.processInvokeGetStreamLength");
	_functionsMappingIds[handlers_rtmp_processInvokeOnBWDone] = _pVM->GetFunctionReference("customhandlers.rtmp.processInvokeOnBWDone");
	_functionsMappingIds[handlers_rtmp_processInvokeGeneric] = _pVM->GetFunctionReference("customhandlers.rtmp.processInvokeGeneric");
	_functionsMappingIds[handlers_rtmp_processInvokeResultWithoutRequest] = _pVM->GetFunctionReference("customhandlers.rtmp.processInvokeResultWithoutRequest");
	_functionsMappingIds[handlers_rtmp_processInvokeResultWithRequest] = _pVM->GetFunctionReference("customhandlers.rtmp.processInvokeResultWithRequest");
	_functionsMappingIds[handlers_rtmp_processInvokeConnectResult] = _pVM->GetFunctionReference("customhandlers.rtmp.processInvokeConnectResult");
	_functionsMappingIds[handlers_rtmp_processInvokeCreateStreamResult] = _pVM->GetFunctionReference("customhandlers.rtmp.processInvokeCreateStreamResult");
	_functionsMappingIds[handlers_rtmp_processInvokeFCSubscribeResult] = _pVM->GetFunctionReference("customhandlers.rtmp.processInvokeFCSubscribeResult");
	_functionsMappingIds[handlers_rtmp_processInvokeGenericResult] = _pVM->GetFunctionReference("customhandlers.rtmp.processInvokeGenericResult");
	_functionsMappingIds[handlers_mpegts_preRegisterProtocol] = _pVM->GetFunctionReference("customhandlers.mpegts.preRegisterProtocol");
	_functionsMappingIds[handlers_mpegts_postRegisterProtocol] = _pVM->GetFunctionReference("customhandlers.mpegts.postRegisterProtocol");
	_functionsMappingIds[handlers_mpegts_preUnRegisterProtocol] = _pVM->GetFunctionReference("customhandlers.mpegts.preUnRegisterProtocol");
	_functionsMappingIds[handlers_mpegts_postUnRegisterProtocol] = _pVM->GetFunctionReference("customhandlers.mpegts.postUnRegisterProtocol");
	_functionsMappingIds[handlers_mpegts_programSetupCompleted] = _pVM->GetFunctionReference("customhandlers.mpegts.programSetupCompleted");
	return true;
}

bool LuaAppVirtualMachine::ValidateHandler(Variant &handler) {
	vector<string> invalidApiFunctions;

	FOR_MAP(handler, string, Variant, apiFunction) {

		FOR_MAP(MAP_VAL(apiFunction), string, Variant, luaFunction) {
			if ((MAP_VAL(luaFunction) != V_STRING)
					|| ((lowerCase((string) MAP_VAL(luaFunction)) != "pre")
					&& (lowerCase((string) MAP_VAL(luaFunction)) != "post"))) {
				WARN("Invalid type for function %s", STR(MAP_KEY(luaFunction)));
				ADD_VECTOR_END(invalidApiFunctions, MAP_KEY(apiFunction));
				break;
			}
			if (!_pVM->HasFunction(MAP_KEY(luaFunction))) {
				WARN("Function %s not found", STR(MAP_KEY(luaFunction)));
				ADD_VECTOR_END(invalidApiFunctions, MAP_KEY(apiFunction));
				break;
			}
		}
	}

	FOR_VECTOR(invalidApiFunctions, i) {
		handler.RemoveKey(invalidApiFunctions[i]);
	}

	return handler.MapSize() != 0;
}

void LuaAppVirtualMachine::Shutdown() {
	if (_pVM != NULL) {
		_pVM->Shutdown();
		delete _pVM;
		_pVM = NULL;
	}
}
#endif	/* HAS_LUA */
