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

#include "protocols/rtp/rtspappprotocolhandler.h"
#include "clientcontext.h"
#include "eventsink/baseeventsink.h"
#include "eventsink/varianteventsink.h"
#include "protocols/rtp/rtspprotocol.h"
#include "application/baseclientapplication.h"
using namespace app_applestreamingclient;

RTSPAppProtocolHandler::RTSPAppProtocolHandler(Variant &configuration)
: BaseRTSPAppProtocolHandler(configuration) {

}

RTSPAppProtocolHandler::~RTSPAppProtocolHandler() {
}

void RTSPAppProtocolHandler::RegisterProtocol(BaseProtocol *pProtocol) {
	BaseRTSPAppProtocolHandler::RegisterProtocol(pProtocol);
}

bool RTSPAppProtocolHandler::HandleRTSPRequest(RTSPProtocol *pFrom,
		Variant &requestHeaders, string &content) {
	return BaseRTSPAppProtocolHandler::HandleRTSPRequest(pFrom, requestHeaders,
			content);
}

ClientContext * RTSPAppProtocolHandler::GetContext(uint32_t contextId,
		uint64_t protocolType) {
	ClientContext *pContext = ClientContext::GetContext(contextId,
			GetApplication()->GetId(), protocolType);
	if (pContext == NULL) {
		FATAL("Unable to get context");
		return NULL;
	}
	return pContext;
}
