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

#include "protocols/key/keyappprotocolhandler.h"
#include "protocols/key/inboundkeyprotocol.h"
using namespace app_applestreamingclient;

KeyAppProtocolHandler::KeyAppProtocolHandler(Variant &configuration)
: BaseAppProtocolHandler(configuration) {

}

KeyAppProtocolHandler::~KeyAppProtocolHandler() {
}

void KeyAppProtocolHandler::RegisterProtocol(BaseProtocol *pProtocol) {
	//1. Get the proper protocol type
	InboundKeyProtocol *pKeyProtocol = (InboundKeyProtocol *) pProtocol;

	//2. Do the HTTP request
	if (!pKeyProtocol->DoHTTPRequest()) {
		FATAL("Unable to do the HTTP request");
		pProtocol->EnqueueForDelete();
	}
}

void KeyAppProtocolHandler::UnRegisterProtocol(BaseProtocol *pProtocol) {
}
