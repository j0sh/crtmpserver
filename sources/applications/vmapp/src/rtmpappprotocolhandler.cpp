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

#include "rtmpappprotocolhandler.h"
#include "protocols/rtmp/messagefactories/messagefactories.h"
#include "vm/baseappvirtualmachine.h"
using namespace app_vmapp;

RTMPAppProtocolHandler::RTMPAppProtocolHandler(Variant &configuration,
		BaseAppVirtualMachine *pAppVM)
: BaseRTMPAppProtocolHandler(configuration) {
	_pAppVM = pAppVM;
}

RTMPAppProtocolHandler::~RTMPAppProtocolHandler() {
}

void RTMPAppProtocolHandler::RegisterProtocol(BaseProtocol *pProtocol) {
	if (_pAppVM->HasFunction(handlers_rtmp_preRegisterProtocol)) {
		_pAppVM->Call(handlers_rtmp_preRegisterProtocol, pProtocol);
	}
	BaseRTMPAppProtocolHandler::RegisterProtocol(pProtocol);
	if (_pAppVM->HasFunction(handlers_rtmp_postRegisterProtocol)) {
		_pAppVM->Call(handlers_rtmp_postRegisterProtocol, pProtocol);
	}
}

void RTMPAppProtocolHandler::UnRegisterProtocol(BaseProtocol *pProtocol) {
	if (_pAppVM->HasFunction(handlers_rtmp_preUnRegisterProtocol)) {
		_pAppVM->Call(handlers_rtmp_preUnRegisterProtocol, pProtocol);
	}
	BaseRTMPAppProtocolHandler::UnRegisterProtocol(pProtocol);
	if (_pAppVM->HasFunction(handlers_rtmp_postUnRegisterProtocol)) {
		_pAppVM->Call(handlers_rtmp_postUnRegisterProtocol, pProtocol);
	}
}

bool RTMPAppProtocolHandler::PullExternalStream(URI uri, Variant streamConfig) {
	if (_pAppVM->HasFunction(handlers_rtmp_pullExternalStream))
		return _pAppVM->Call(handlers_rtmp_pullExternalStream, uri, streamConfig);
	else
		return BaseRTMPAppProtocolHandler::PullExternalStream(uri, streamConfig);
}

bool RTMPAppProtocolHandler::PushLocalStream(BaseInStream *pInStream,
		Variant streamConfig) {
	if (_pAppVM->HasFunction(handlers_rtmp_pushLocalStream))
		return _pAppVM->Call(handlers_rtmp_pushLocalStream, pInStream, streamConfig);
	else
		return BaseRTMPAppProtocolHandler::PushLocalStream(pInStream, streamConfig);
}

bool RTMPAppProtocolHandler::OutboundConnectionEstablished(
		OutboundRTMPProtocol * pFrom) {
	if (_pAppVM->HasFunction(handlers_rtmp_outboundConnectionEstablished))
		return _pAppVM->Call(handlers_rtmp_outboundConnectionEstablished, pFrom);
	else
		return BaseRTMPAppProtocolHandler::OutboundConnectionEstablished(pFrom);
}

bool RTMPAppProtocolHandler::AuthenticateInbound(BaseRTMPProtocol *pFrom,
		Variant &request,
		Variant & authState) {
	if (_pAppVM->HasFunction(handlers_rtmp_authenticateInbound))
		return _pAppVM->Call(handlers_rtmp_authenticateInbound, pFrom, request,
			authState);
	else
		return BaseRTMPAppProtocolHandler::AuthenticateInbound(pFrom, request,
			authState);
}

bool RTMPAppProtocolHandler::InboundMessageAvailable(BaseRTMPProtocol *pFrom,
		Variant & request) {
	if (_pAppVM->HasFunction(handlers_rtmp_inboundMessageAvailable))
		return _pAppVM->Call(handlers_rtmp_inboundMessageAvailable, pFrom,
			request);
	else
		return BaseRTMPAppProtocolHandler::InboundMessageAvailable(pFrom,
			request);
}

bool RTMPAppProtocolHandler::ProcessWinAckSize(BaseRTMPProtocol *pFrom, Variant & request) {
	if (_pAppVM->HasFunction(handlers_rtmp_processWinAckSize))
		return _pAppVM->Call(handlers_rtmp_processWinAckSize, pFrom, request);
	else
		return BaseRTMPAppProtocolHandler::ProcessWinAckSize(pFrom, request);
}

bool RTMPAppProtocolHandler::ProcessPeerBW(BaseRTMPProtocol *pFrom, Variant & request) {
	if (_pAppVM->HasFunction(handlers_rtmp_processPeerBW))
		return _pAppVM->Call(handlers_rtmp_processPeerBW, pFrom, request);
	else
		return BaseRTMPAppProtocolHandler::ProcessPeerBW(pFrom, request);
}

bool RTMPAppProtocolHandler::ProcessAck(BaseRTMPProtocol *pFrom, Variant & request) {
	if (_pAppVM->HasFunction(handlers_rtmp_processAck))
		return _pAppVM->Call(handlers_rtmp_processAck, pFrom, request);
	else
		return BaseRTMPAppProtocolHandler::ProcessAck(pFrom, request);
}

bool RTMPAppProtocolHandler::ProcessChunkSize(BaseRTMPProtocol *pFrom, Variant & request) {
	if (_pAppVM->HasFunction(handlers_rtmp_processChunkSize))
		return _pAppVM->Call(handlers_rtmp_processChunkSize, pFrom, request);
	else
		return BaseRTMPAppProtocolHandler::ProcessChunkSize(pFrom, request);
}

bool RTMPAppProtocolHandler::ProcessUsrCtrl(BaseRTMPProtocol *pFrom, Variant & request) {
	if (_pAppVM->HasFunction(handlers_rtmp_processUsrCtrl))
		return _pAppVM->Call(handlers_rtmp_processUsrCtrl, pFrom, request);
	else
		return BaseRTMPAppProtocolHandler::ProcessUsrCtrl(pFrom, request);
}

bool RTMPAppProtocolHandler::ProcessNotify(BaseRTMPProtocol *pFrom, Variant & request) {
	if (_pAppVM->HasFunction(handlers_rtmp_processNotify))
		return _pAppVM->Call(handlers_rtmp_processNotify, pFrom, request);
	else
		return BaseRTMPAppProtocolHandler::ProcessNotify(pFrom, request);
}

bool RTMPAppProtocolHandler::ProcessFlexStreamSend(BaseRTMPProtocol *pFrom, Variant & request) {
	if (_pAppVM->HasFunction(handlers_rtmp_processFlexStreamSend))
		return _pAppVM->Call(handlers_rtmp_processFlexStreamSend, pFrom, request);
	else
		return BaseRTMPAppProtocolHandler::ProcessFlexStreamSend(pFrom, request);
}

bool RTMPAppProtocolHandler::ProcessSharedObject(BaseRTMPProtocol *pFrom, Variant & request) {
	if (_pAppVM->HasFunction(handlers_rtmp_processSharedObject))
		return _pAppVM->Call(handlers_rtmp_processSharedObject, pFrom, request);
	else
		return BaseRTMPAppProtocolHandler::ProcessSharedObject(pFrom, request);
}

bool RTMPAppProtocolHandler::ProcessInvoke(BaseRTMPProtocol *pFrom, Variant & request) {
	if (_pAppVM->HasFunction(handlers_rtmp_processInvoke))
		return _pAppVM->Call(handlers_rtmp_processInvoke, pFrom, request);
	else
		return BaseRTMPAppProtocolHandler::ProcessInvoke(pFrom, request);
}

bool RTMPAppProtocolHandler::ProcessInvokeConnect(BaseRTMPProtocol *pFrom, Variant & request) {
	if (_pAppVM->HasFunction(handlers_rtmp_processInvokeConnect))
		return _pAppVM->Call(handlers_rtmp_processInvokeConnect, pFrom, request);
	else
		return BaseRTMPAppProtocolHandler::ProcessInvokeConnect(pFrom, request);
}

bool RTMPAppProtocolHandler::ProcessInvokeCreateStream(BaseRTMPProtocol *pFrom, Variant & request) {
	if (_pAppVM->HasFunction(handlers_rtmp_processInvokeCreateStream))
		return _pAppVM->Call(handlers_rtmp_processInvokeCreateStream, pFrom, request);
	else
		return BaseRTMPAppProtocolHandler::ProcessInvokeCreateStream(pFrom, request);
}

bool RTMPAppProtocolHandler::ProcessInvokePublish(BaseRTMPProtocol *pFrom, Variant & request) {
	if (_pAppVM->HasFunction(handlers_rtmp_processInvokePublish))
		return _pAppVM->Call(handlers_rtmp_processInvokePublish, pFrom, request);
	else
		return BaseRTMPAppProtocolHandler::ProcessInvokePublish(pFrom, request);
}

bool RTMPAppProtocolHandler::ProcessInvokeSeek(BaseRTMPProtocol *pFrom, Variant & request) {
	if (_pAppVM->HasFunction(handlers_rtmp_processInvokeSeek))
		return _pAppVM->Call(handlers_rtmp_processInvokeSeek, pFrom, request);
	else
		return BaseRTMPAppProtocolHandler::ProcessInvokeSeek(pFrom, request);
}

bool RTMPAppProtocolHandler::ProcessInvokePlay(BaseRTMPProtocol *pFrom, Variant & request) {
	if (_pAppVM->HasFunction(handlers_rtmp_processInvokePlay))
		return _pAppVM->Call(handlers_rtmp_processInvokePlay, pFrom, request);
	else
		return BaseRTMPAppProtocolHandler::ProcessInvokePlay(pFrom, request);
}

bool RTMPAppProtocolHandler::ProcessInvokePauseRaw(BaseRTMPProtocol *pFrom, Variant & request) {
	if (_pAppVM->HasFunction(handlers_rtmp_processInvokePauseRaw))
		return _pAppVM->Call(handlers_rtmp_processInvokePauseRaw, pFrom, request);
	else
		return BaseRTMPAppProtocolHandler::ProcessInvokePauseRaw(pFrom, request);
}

bool RTMPAppProtocolHandler::ProcessInvokePause(BaseRTMPProtocol *pFrom, Variant & request) {
	if (_pAppVM->HasFunction(handlers_rtmp_processInvokePause))
		return _pAppVM->Call(handlers_rtmp_processInvokePause, pFrom, request);
	else
		return BaseRTMPAppProtocolHandler::ProcessInvokePause(pFrom, request);
}

bool RTMPAppProtocolHandler::ProcessInvokeCloseStream(BaseRTMPProtocol *pFrom, Variant & request) {
	if (_pAppVM->HasFunction(handlers_rtmp_processInvokeCloseStream))
		return _pAppVM->Call(handlers_rtmp_processInvokeCloseStream, pFrom, request);
	else
		return BaseRTMPAppProtocolHandler::ProcessInvokeCloseStream(pFrom, request);
}

bool RTMPAppProtocolHandler::ProcessInvokeReleaseStream(BaseRTMPProtocol *pFrom, Variant & request) {
	if (_pAppVM->HasFunction(handlers_rtmp_processInvokeReleaseStream))
		return _pAppVM->Call(handlers_rtmp_processInvokeReleaseStream, pFrom, request);
	else
		return BaseRTMPAppProtocolHandler::ProcessInvokeReleaseStream(pFrom, request);
}

bool RTMPAppProtocolHandler::ProcessInvokeDeleteStream(BaseRTMPProtocol *pFrom, Variant & request) {
	if (_pAppVM->HasFunction(handlers_rtmp_processInvokeDeleteStream))
		return _pAppVM->Call(handlers_rtmp_processInvokeDeleteStream, pFrom, request);
	else
		return BaseRTMPAppProtocolHandler::ProcessInvokeDeleteStream(pFrom, request);
}

bool RTMPAppProtocolHandler::ProcessInvokeOnStatus(BaseRTMPProtocol *pFrom, Variant & request) {
	if (_pAppVM->HasFunction(handlers_rtmp_processInvokeOnStatus))
		return _pAppVM->Call(handlers_rtmp_processInvokeOnStatus, pFrom, request);
	else
		return BaseRTMPAppProtocolHandler::ProcessInvokeOnStatus(pFrom, request);
}

bool RTMPAppProtocolHandler::ProcessInvokeFCPublish(BaseRTMPProtocol *pFrom, Variant & request) {
	if (_pAppVM->HasFunction(handlers_rtmp_processInvokeFCPublish))
		return _pAppVM->Call(handlers_rtmp_processInvokeFCPublish, pFrom, request);
	else
		return BaseRTMPAppProtocolHandler::ProcessInvokeFCPublish(pFrom, request);
}

bool RTMPAppProtocolHandler::ProcessInvokeGetStreamLength(BaseRTMPProtocol *pFrom, Variant & request) {
	if (_pAppVM->HasFunction(handlers_rtmp_processInvokeGetStreamLength))
		return _pAppVM->Call(handlers_rtmp_processInvokeGetStreamLength, pFrom, request);
	else
		return BaseRTMPAppProtocolHandler::ProcessInvokeGetStreamLength(pFrom, request);
}

bool RTMPAppProtocolHandler::ProcessInvokeOnBWDone(BaseRTMPProtocol *pFrom, Variant & request) {
	if (_pAppVM->HasFunction(handlers_rtmp_processInvokeOnBWDone))
		return _pAppVM->Call(handlers_rtmp_processInvokeOnBWDone, pFrom, request);
	else
		return BaseRTMPAppProtocolHandler::ProcessInvokeOnBWDone(pFrom, request);
}

bool RTMPAppProtocolHandler::ProcessInvokeGeneric(BaseRTMPProtocol *pFrom, Variant & request) {
	if (_pAppVM->HasFunction(handlers_rtmp_processInvokeGeneric))
		return _pAppVM->Call(handlers_rtmp_processInvokeGeneric, pFrom, request);
	else
		return BaseRTMPAppProtocolHandler::ProcessInvokeGeneric(pFrom, request);
}

bool RTMPAppProtocolHandler::ProcessInvokeResult(BaseRTMPProtocol *pFrom,
		Variant &request) {
	if (_pAppVM->HasFunction(handlers_rtmp_processInvokeResultWithoutRequest))
		return _pAppVM->Call(handlers_rtmp_processInvokeResultWithoutRequest, pFrom, request);
	else
		return BaseRTMPAppProtocolHandler::ProcessInvokeResult(pFrom, request);
}

bool RTMPAppProtocolHandler::ProcessInvokeResult(BaseRTMPProtocol *pFrom,
		Variant &request, Variant &response) {
	if (_pAppVM->HasFunction(handlers_rtmp_processInvokeResultWithRequest))
		return _pAppVM->Call(handlers_rtmp_processInvokeResultWithRequest, pFrom, request, response);
	else
		return BaseRTMPAppProtocolHandler::ProcessInvokeResult(pFrom, request, response);
}

bool RTMPAppProtocolHandler::ProcessInvokeConnectResult(BaseRTMPProtocol *pFrom,
		Variant &request, Variant &response) {
	if (_pAppVM->HasFunction(handlers_rtmp_processInvokeConnectResult))
		return _pAppVM->Call(handlers_rtmp_processInvokeConnectResult, pFrom, request, response);
	else
		return BaseRTMPAppProtocolHandler::ProcessInvokeConnectResult(pFrom, request, response);
}

bool RTMPAppProtocolHandler::ProcessInvokeCreateStreamResult(BaseRTMPProtocol *pFrom,
		Variant &request, Variant &response) {
	if (_pAppVM->HasFunction(handlers_rtmp_processInvokeCreateStreamResult))
		return _pAppVM->Call(handlers_rtmp_processInvokeCreateStreamResult, pFrom, request, response);
	else
		return BaseRTMPAppProtocolHandler::ProcessInvokeCreateStreamResult(pFrom, request, response);
}

bool RTMPAppProtocolHandler::ProcessInvokeFCSubscribeResult(BaseRTMPProtocol *pFrom,
		Variant &request, Variant &response) {
	if (_pAppVM->HasFunction(handlers_rtmp_processInvokeFCSubscribeResult))
		return _pAppVM->Call(handlers_rtmp_processInvokeFCSubscribeResult, pFrom, request, response);
	else
		return BaseRTMPAppProtocolHandler::ProcessInvokeFCSubscribeResult(pFrom, request, response);
}

bool RTMPAppProtocolHandler::ProcessInvokeGenericResult(BaseRTMPProtocol *pFrom,
		Variant &request, Variant &response) {
	if (_pAppVM->HasFunction(handlers_rtmp_processInvokeGenericResult))
		return _pAppVM->Call(handlers_rtmp_processInvokeGenericResult, pFrom, request, response);
	else
		return BaseRTMPAppProtocolHandler::ProcessInvokeGenericResult(pFrom, request, response);
}
