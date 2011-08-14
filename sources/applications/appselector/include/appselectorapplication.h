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

#ifndef _APPSELECTORAPPLICATION_H
#define _APPSELECTORAPPLICATION_H

#include "application/baseclientapplication.h"

namespace app_appselector {

#ifdef HAS_PROTOCOL_RTMP
	class RTMPAppProtocolHandler;
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_HTTP
	class HTTPAppProtocolHandler;
#endif /* HAS_PROTOCOL_HTTP */

	class AppSelectorApplication
	: public BaseClientApplication {
	private:
#ifdef HAS_PROTOCOL_RTMP
		RTMPAppProtocolHandler *_pRTMPHandler;
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_HTTP
		HTTPAppProtocolHandler *_pHTTPHandler;
#endif /* HAS_PROTOCOL_HTTP */
	public:
		AppSelectorApplication(Variant &configuration);
		virtual ~AppSelectorApplication();

		virtual bool Initialize();
	};
}

#endif	/* _APPSELECTORAPPLICATION_H */


