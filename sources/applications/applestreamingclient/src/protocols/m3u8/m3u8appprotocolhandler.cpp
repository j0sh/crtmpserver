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

#include "protocols/m3u8/m3u8appprotocolhandler.h"
#include "protocols/m3u8/childm3u8protocol.h"
#include "protocols/m3u8/masterm3u8protocol.h"
#include "protocols/http/basehttpprotocol.h"
#include "clientcontext.h"
using namespace app_applestreamingclient;

M3U8AppProtocolHandler::M3U8AppProtocolHandler(Variant &configuration)
: BaseAppProtocolHandler(configuration) {

}

M3U8AppProtocolHandler::~M3U8AppProtocolHandler() {
}

void M3U8AppProtocolHandler::RegisterProtocol(BaseProtocol *pProtocol) {
	//1. Get the proper protocol type
	BaseM3U8Protocol *pM3U8Protocol = (BaseM3U8Protocol *) pProtocol;

	//2. Do the HTTP request
	if (!pM3U8Protocol->DoHTTPRequest()) {
		FATAL("Unable to do the HTTP request");
		pProtocol->EnqueueForDelete();
	}
}

void M3U8AppProtocolHandler::UnRegisterProtocol(BaseProtocol *pProtocol) {
}
