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

#ifndef _VMAPPAPPLICATION_H
#define _VMAPPAPPLICATION_H

#include "application/baseclientapplication.h"

namespace app_vmapp {

	class BaseAppVirtualMachine;
	class RTMPAppProtocolHandler;
	class RTPAppProtocolHandler;
	class RTSPAppProtocolHandler;
	class TSAppProtocolHandler;

	class VMAppApplication
	: public BaseClientApplication {
	private:
		BaseAppVirtualMachine *_pAppVM;
		RTMPAppProtocolHandler *_pRTMPHandler;
		RTPAppProtocolHandler *_pRTPHandler;
		RTSPAppProtocolHandler *_pRTSPHandler;
		TSAppProtocolHandler *_pTSHandler;
	public:
		VMAppApplication(Variant &configuration);
		virtual ~VMAppApplication();
		virtual bool Initialize();
		virtual bool OutboundConnectionFailed(Variant &customParameters);
		virtual void RegisterProtocol(BaseProtocol *pProtocol);
		virtual void UnRegisterProtocol(BaseProtocol *pProtocol);
		virtual void SignalStreamRegistered(BaseStream *pStream);
		virtual void SignalStreamUnRegistered(BaseStream *pStream);
		virtual bool PullExternalStream(Variant streamConfig);
		virtual bool PushLocalStream(Variant streamConfig);
	};
}

#endif	/* _VMAPPAPPLICATION_H */

