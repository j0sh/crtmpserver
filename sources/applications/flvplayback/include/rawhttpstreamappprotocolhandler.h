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

#ifdef HAS_PROTOCOL_RAWHTTPSTREAM
#ifndef _RAWHTTPSTREAMAPPPROTOCOLHANDLER_H
#define	_RAWHTTPSTREAMAPPPROTOCOLHANDLER_H

#include "protocols/rawhttpstream/baserawhttpstreamappprotocolhandler.h"

namespace app_flvplayback {

	class RawHTTPStreamAppProtocolHandler
	: public BaseRawHTTPStreamAppProtocolHandler {
	public:
		RawHTTPStreamAppProtocolHandler(Variant &configuration);
		virtual ~RawHTTPStreamAppProtocolHandler();
	};
};

#endif	/* _RAWHTTPSTREAMAPPPROTOCOLHANDLER_H */
#endif /* HAS_PROTOCOL_RAWHTTPSTREAM */
