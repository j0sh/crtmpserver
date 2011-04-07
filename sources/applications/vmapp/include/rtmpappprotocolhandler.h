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
#ifndef _RTMPAPPPROTOCOLHANDLER_H
#define	_RTMPAPPPROTOCOLHANDLER_H

#include "protocols/rtmp/basertmpappprotocolhandler.h"
namespace app_vmapp {

	class BaseAppVirtualMachine;

	class RTMPAppProtocolHandler
	: public BaseRTMPAppProtocolHandler {
	private:
		BaseAppVirtualMachine *_pAppVM;
	public:
		RTMPAppProtocolHandler(Variant &configuration, BaseAppVirtualMachine *pAppVM);
		virtual ~RTMPAppProtocolHandler();

		virtual void RegisterProtocol(BaseProtocol *pProtocol);
		virtual void UnRegisterProtocol(BaseProtocol *pProtocol);
		virtual bool PullExternalStream(URI uri, Variant streamConfig);
		virtual bool PushLocalStream(BaseInStream *pInStream, Variant streamConfig);
		virtual bool OutboundConnectionEstablished(OutboundRTMPProtocol *pFrom);
		virtual bool AuthenticateInbound(BaseRTMPProtocol *pFrom, Variant &request,
				Variant &authState);
		virtual bool InboundMessageAvailable(BaseRTMPProtocol *pFrom, Variant &request);

		virtual bool ProcessWinAckSize(BaseRTMPProtocol *pFrom, Variant &request);
		virtual bool ProcessPeerBW(BaseRTMPProtocol *pFrom, Variant &request);
		virtual bool ProcessAck(BaseRTMPProtocol *pFrom, Variant &request);
		virtual bool ProcessChunkSize(BaseRTMPProtocol *pFrom, Variant &request);
		virtual bool ProcessUsrCtrl(BaseRTMPProtocol *pFrom, Variant &request);
		virtual bool ProcessNotify(BaseRTMPProtocol *pFrom, Variant &request);
		virtual bool ProcessFlexStreamSend(BaseRTMPProtocol *pFrom, Variant &request);
		virtual bool ProcessSharedObject(BaseRTMPProtocol *pFrom, Variant &request);
		virtual bool ProcessInvoke(BaseRTMPProtocol *pFrom, Variant &request);
		virtual bool ProcessInvokeConnect(BaseRTMPProtocol *pFrom, Variant &request);
		virtual bool ProcessInvokeCreateStream(BaseRTMPProtocol *pFrom,
				Variant &request);
		virtual bool ProcessInvokePublish(BaseRTMPProtocol *pFrom,
				Variant &request);
		virtual bool ProcessInvokeSeek(BaseRTMPProtocol *pFrom,
				Variant &request);
		virtual bool ProcessInvokePlay(BaseRTMPProtocol *pFrom,
				Variant &request);
		virtual bool ProcessInvokePauseRaw(BaseRTMPProtocol *pFrom,
				Variant &request);
		virtual bool ProcessInvokePause(BaseRTMPProtocol *pFrom,
				Variant &request);
		virtual bool ProcessInvokeCloseStream(BaseRTMPProtocol *pFrom,
				Variant &request);
		virtual bool ProcessInvokeReleaseStream(BaseRTMPProtocol *pFrom,
				Variant &request);
		virtual bool ProcessInvokeDeleteStream(BaseRTMPProtocol *pFrom,
				Variant &request);
		virtual bool ProcessInvokeOnStatus(BaseRTMPProtocol *pFrom,
				Variant &request);
		virtual bool ProcessInvokeFCPublish(BaseRTMPProtocol *pFrom,
				Variant &request);
		virtual bool ProcessInvokeGetStreamLength(BaseRTMPProtocol *pFrom,
				Variant &request);
		virtual bool ProcessInvokeOnBWDone(BaseRTMPProtocol *pFrom,
				Variant &request);
		virtual bool ProcessInvokeGeneric(BaseRTMPProtocol *pFrom,
				Variant &request);
		virtual bool ProcessInvokeResult(BaseRTMPProtocol *pFrom,
				Variant &request);
		virtual bool ProcessInvokeResult(BaseRTMPProtocol *pFrom,
				Variant &request, Variant &response);
		virtual bool ProcessInvokeConnectResult(BaseRTMPProtocol *pFrom,
				Variant &request, Variant &response);
		virtual bool ProcessInvokeCreateStreamResult(BaseRTMPProtocol *pFrom,
				Variant &request, Variant &response);
		virtual bool ProcessInvokeFCSubscribeResult(BaseRTMPProtocol *pFrom,
				Variant &request, Variant &response);
		virtual bool ProcessInvokeGenericResult(BaseRTMPProtocol *pFrom,
			Variant &request, Variant &response);
	};
}
#endif	/* _RTMPAPPPROTOCOLHANDLER_H */
#endif /* HAS_PROTOCOL_RTMP */

