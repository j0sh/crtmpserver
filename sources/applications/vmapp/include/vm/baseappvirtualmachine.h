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

#ifndef _BASEAPPVIRTUALMACHINE_H
#define	_BASEAPPVIRTUALMACHINE_H

#include "common.h"

class BaseVM;
class BaseClientApplication;
class BaseProtocol;
class OutboundRTMPProtocol;
class BaseRTMPProtocol;
class BaseInStream;
class BaseStream;
class InNetTSStream;

namespace app_vmapp {

	typedef enum _APIFunctionID {
		handlers_application_initCompleted = 0,
		handlers_application_outboundConnectionFailed,
		handlers_application_preRegisterProtocol,
		handlers_application_postRegisterProtocol,
		handlers_application_preUnRegisterProtocol,
		handlers_application_postUnRegisterProtocol,
		handlers_application_signalStreamRegistered,
		handlers_application_signalStreamUnRegistered,
		handlers_application_pullExternalStream,
		handlers_application_pushLocalStream,
		handlers_rtmp_preRegisterProtocol,
		handlers_rtmp_postRegisterProtocol,
		handlers_rtmp_preUnRegisterProtocol,
		handlers_rtmp_postUnRegisterProtocol,
		handlers_rtmp_pullExternalStream,
		handlers_rtmp_pushLocalStream,
		handlers_rtmp_outboundConnectionEstablished,
		handlers_rtmp_authenticateInbound,
		handlers_rtmp_inboundMessageAvailable,
		handlers_rtmp_processWinAckSize,
		handlers_rtmp_processPeerBW,
		handlers_rtmp_processAck,
		handlers_rtmp_processChunkSize,
		handlers_rtmp_processUsrCtrl,
		handlers_rtmp_processNotify,
		handlers_rtmp_processFlexStreamSend,
		handlers_rtmp_processSharedObject,
		handlers_rtmp_processInvoke,
		handlers_rtmp_processInvokeConnect,
		handlers_rtmp_processInvokeCreateStream,
		handlers_rtmp_processInvokePublish,
		handlers_rtmp_processInvokeSeek,
		handlers_rtmp_processInvokePlay,
		handlers_rtmp_processInvokePauseRaw,
		handlers_rtmp_processInvokePause,
		handlers_rtmp_processInvokeCloseStream,
		handlers_rtmp_processInvokeReleaseStream,
		handlers_rtmp_processInvokeDeleteStream,
		handlers_rtmp_processInvokeOnStatus,
		handlers_rtmp_processInvokeFCPublish,
		handlers_rtmp_processInvokeGetStreamLength,
		handlers_rtmp_processInvokeOnBWDone,
		handlers_rtmp_processInvokeGeneric,
		handlers_rtmp_processInvokeResultWithoutRequest,
		handlers_rtmp_processInvokeResultWithRequest,
		handlers_rtmp_processInvokeConnectResult,
		handlers_rtmp_processInvokeCreateStreamResult,
		handlers_rtmp_processInvokeFCSubscribeResult,
		handlers_rtmp_processInvokeGenericResult,
		handlers_mpegts_preRegisterProtocol,
		handlers_mpegts_postRegisterProtocol,
		handlers_mpegts_preUnRegisterProtocol,
		handlers_mpegts_postUnRegisterProtocol,
		handlers_mpegts_programSetupCompleted,
		APIFunctionID_size
	} APIFunctionID;

	class BaseAppVirtualMachine {
	protected:
		BaseClientApplication *_pApplication;
	protected:
		BaseAppVirtualMachine();
	public:
		static BaseAppVirtualMachine *GetVM(string type, string script,
				BaseClientApplication *pApplication);
		virtual ~BaseAppVirtualMachine();
		virtual bool HasFunction(APIFunctionID functionId) = 0;
		virtual bool Call(APIFunctionID functionId) = 0;
		virtual bool Call(APIFunctionID functionId, Variant &variant) = 0;
		virtual bool Call(APIFunctionID functionId, BaseStream *pBaseStream) = 0;
		virtual bool Call(APIFunctionID functionId, InNetTSStream *pInNetTSStream) = 0;
		virtual bool Call(APIFunctionID functionId, BaseProtocol *pProtocol) = 0;
		virtual bool Call(APIFunctionID functionId, OutboundRTMPProtocol *pProtocol) = 0;
		virtual bool Call(APIFunctionID functionId, BaseRTMPProtocol *pProtocol,
				Variant &variant) = 0;
		virtual bool Call(APIFunctionID functionId, BaseRTMPProtocol *pProtocol,
				Variant &variant1, Variant &variant2) = 0;
		virtual bool Call(APIFunctionID functionId, URI &uri, Variant &variant) = 0;
		virtual bool Call(APIFunctionID functionId, BaseInStream *pInStream,
				Variant &variant) = 0;
	protected:
		virtual bool BindAPI() = 0;
		virtual bool Initialize(string script) = 0;
	};
}

#endif	/* _BASEAPPVIRTUALMACHINE_H */

