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


#ifdef HAS_PROTOCOL_LIVEFLV

#ifndef _BASELIVEFLVAPPPROTOCOLHANDLER_H
#define	_BASELIVEFLVAPPPROTOCOLHANDLER_H

#include "application/baseappprotocolhandler.h"

class InboundLiveFLVProtocol;

class DLLEXP BaseLiveFLVAppProtocolHandler
: public BaseAppProtocolHandler {
private:
	map<uint32_t, InboundLiveFLVProtocol *> _protocols;
public:
	BaseLiveFLVAppProtocolHandler(Variant &configuration);
	virtual ~BaseLiveFLVAppProtocolHandler();

	virtual void RegisterProtocol(BaseProtocol *pProtocol);
	virtual void UnRegisterProtocol(BaseProtocol *pProtocol);
};

#endif	/* _BASELIVEFLVAPPPROTOCOLHANDLER_H */
#endif /* HAS_PROTOCOL_LIVEFLV */


