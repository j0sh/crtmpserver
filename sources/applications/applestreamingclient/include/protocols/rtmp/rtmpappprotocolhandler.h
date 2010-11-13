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

namespace app_applestreamingclient {
	class ClientContext;

	class RTMPAppProtocolHandler
	: public BaseRTMPAppProtocolHandler {
	public:
		RTMPAppProtocolHandler(Variant &configuration);
		virtual ~RTMPAppProtocolHandler();

		virtual void UnRegisterProtocol(BaseProtocol *pProtocol);

		virtual bool ProcessInvokeGeneric(BaseRTMPProtocol *pFrom,
				Variant &request);
	private:
		ClientContext * GetContext(BaseProtocol *pFrom);
		void ReleaseContext(BaseProtocol *pFrom);
		bool ProcessSetupStream(BaseRTMPProtocol *pFrom,
				Variant &request);
		bool ProcessGetBWInfo(BaseRTMPProtocol *pFrom,
				Variant &request);
	};
}

#endif	/* _RTMPAPPPROTOCOLHANDLER_H */
#endif /* HAS_PROTOCOL_RTMP */
