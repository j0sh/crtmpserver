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
#include "protocols/rtmp/rtmpappprotocolhandler.h"
#include "protocols/rtmp/basertmpappprotocolhandler.h"
#include "protocols/rtmp/messagefactories/messagefactories.h"
#include "protocols/rtmp/basertmpprotocol.h"
#include "clientcontext.h"
#include "application/baseclientapplication.h"
#include "eventsink/rtmpeventsink.h"

RTMPAppProtocolHandler::RTMPAppProtocolHandler(Variant &configuration)
: BaseRTMPAppProtocolHandler(configuration) {

}

RTMPAppProtocolHandler::~RTMPAppProtocolHandler() {
}

void RTMPAppProtocolHandler::UnRegisterProtocol(BaseProtocol *pProtocol) {
	BaseRTMPAppProtocolHandler::UnRegisterProtocol(pProtocol);
	ReleaseContext(pProtocol);
}

bool RTMPAppProtocolHandler::ProcessInvokeGeneric(BaseRTMPProtocol *pFrom,
		Variant &request) {
	string functionName = M_INVOKE_FUNCTION(request);
	if (functionName == "setupStream") {
		return ProcessSetupStream(pFrom, request);
	} else {
		WARN("Invalid function name");
		return BaseRTMPAppProtocolHandler::ProcessInvokeGeneric(pFrom, request);
	}
}

ClientContext * RTMPAppProtocolHandler::GetContext(BaseProtocol *pFrom) {
	uint32_t contextId = pFrom->GetCustomParameters()["contextId"];
	ClientContext *pContext = ClientContext::GetContext(contextId,
			GetApplication()->GetId(), pFrom->GetType());
	if (pContext == NULL) {
		FATAL("Unable to get context");
		return NULL;
	}
	((RTMPEventSink *) pContext->EventSink())->SetProtocolId(pFrom->GetId());
	pFrom->GetCustomParameters()["contextId"] = pContext->Id();
	return pContext;
}

void RTMPAppProtocolHandler::ReleaseContext(BaseProtocol *pFrom) {
	uint32_t contextId = pFrom->GetCustomParameters()["contextId"];
	ClientContext::ReleaseContext(contextId);
	pFrom->GetCustomParameters()["contextId"] = (uint32_t) 0;
}

bool RTMPAppProtocolHandler::ProcessSetupStream(BaseRTMPProtocol *pFrom,
		Variant &request) {
	//1. Delete the old context
	ReleaseContext(pFrom);

	//2. get the context
	ClientContext *pContext = GetContext(pFrom);
	if (pContext == NULL) {
		FATAL("Unable to get context");
		return false;
	}

	//3. Setup the new connecting string
	pContext->RawConnectingString(M_INVOKE_PARAM(request, 1));

	//4. StartProcessing
	return pContext->StartProcessing();
}
#endif /* HAS_PROTOCOL_RTMP */
