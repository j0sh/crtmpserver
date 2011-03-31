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


#ifdef HAS_PROTOCOL_TS
#include "protocols/ts/basetsappprotocolhandler.h"
#include "protocols/baseprotocol.h"
#include "protocols/ts/innettsstream.h"

BaseTSAppProtocolHandler::BaseTSAppProtocolHandler(Variant &configuration)
: BaseAppProtocolHandler(configuration) {

}

BaseTSAppProtocolHandler::~BaseTSAppProtocolHandler() {
}

void BaseTSAppProtocolHandler::RegisterProtocol(BaseProtocol *pProtocol) {
	if (MAP_HAS1(_connections, pProtocol->GetId())) {
		ASSERT("Protocol already registered");
	}
	_connections[pProtocol->GetId()] = (InboundTSProtocol *) pProtocol;
}

void BaseTSAppProtocolHandler::UnRegisterProtocol(BaseProtocol *pProtocol) {
	_connections.erase(pProtocol->GetId());
}

void BaseTSAppProtocolHandler::ProgramSetupCompleted(InNetTSStream *pInNetTSStream) {
	INFO("Stream available (%u): %s", pInNetTSStream->GetUniqueId(),
			STR(pInNetTSStream->GetName()));
}
#endif	/* HAS_PROTOCOL_TS */

