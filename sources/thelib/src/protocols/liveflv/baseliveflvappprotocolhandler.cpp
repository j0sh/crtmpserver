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

#include "protocols/liveflv/baseliveflvappprotocolhandler.h"
#include "protocols/baseprotocol.h"
#include "application/baseclientapplication.h"

BaseLiveFLVAppProtocolHandler::BaseLiveFLVAppProtocolHandler(Variant &configuration)
: BaseAppProtocolHandler(configuration) {

}

BaseLiveFLVAppProtocolHandler::~BaseLiveFLVAppProtocolHandler() {
}

void BaseLiveFLVAppProtocolHandler::RegisterProtocol(BaseProtocol *pProtocol) {
	if (MAP_HAS1(_protocols, pProtocol->GetId())) {
		ASSERT("Protocol ID %u already registered", pProtocol->GetId());
		return;
	}
	if (pProtocol->GetType() != PT_INBOUND_LIVE_FLV) {
		ASSERT("This protocol can't be registered here");
		return;
	}
	_protocols[pProtocol->GetId()] = (InboundLiveFLVProtocol *) pProtocol;
	FINEST("protocol %s registered to app %s", STR(*pProtocol), STR(GetApplication()->GetName()));
}

void BaseLiveFLVAppProtocolHandler::UnRegisterProtocol(BaseProtocol *pProtocol) {
	if (!MAP_HAS1(_protocols, pProtocol->GetId())) {
		ASSERT("Protocol ID %u not registered", pProtocol->GetId());
		return;
	}
	if (pProtocol->GetType() != PT_INBOUND_LIVE_FLV) {
		ASSERT("This protocol can't be unregistered here");
		return;
	}
	_protocols.erase(pProtocol->GetId());
	FINEST("protocol %s unregistered from app %s", STR(*pProtocol), STR(GetApplication()->GetName()));
}
#endif /* HAS_PROTOCOL_LIVEFLV */
