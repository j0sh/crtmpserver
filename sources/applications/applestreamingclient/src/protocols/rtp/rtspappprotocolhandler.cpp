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

RTSPAppProtocolHandler::RTSPAppProtocolHandler(Variant &configuration)
: BaseRTSPAppProtocolHandler(configuration) {

}

RTSPAppProtocolHandler::~RTSPAppProtocolHandler() {
}

void RTSPAppProtocolHandler::RegisterProtocol(BaseProtocol *pProtocol) {
	BaseRTSPAppProtocolHandler::RegisterProtocol(pProtocol);
	//	vector<uint32_t> contextIds = ClientContext::GetContextIds();
	//	for (uint32_t i = 0; i < contextIds.size(); i++) {
	//		FINEST("Inspecting context id: %d", contextIds[i]);
	//		ClientContext *pContext = GetContext(contextIds[i],
	//				pProtocol->GetType());
	//		if (pContext == NULL) {
	//			WARN("Context id %d is NULL", contextIds[i]);
	//			continue;
	//		}
	//		if (pContext->EventSink()->GetType() != EVENT_SYNC_VARIANT) {
	//			WARN("Context id %d is not Variant friendly", contextIds[i]);
	//			continue;
	//		}
	//		VariantEventSink *pSink = (VariantEventSink *) pContext->EventSink();
	//		vector<string> streams = pSink->GetStreamNames();
	//		for (uint32_t j = 0; j < streams.size(); j++) {
	//			FINEST("Adding stream %s", STR(streams[j]));
	//			pProtocol->GetCustomParameters()["customStreamName"] = streams[j];
	//			return;
	//		}
	//	}
	//	pProtocol->EnqueueForDelete();
}

bool RTSPAppProtocolHandler::HandleRTSPRequest(RTSPProtocol *pFrom,
		Variant &requestHeaders, string &content) {
	//	string uri = requestHeaders[RTSP_FIRST_LINE][RTSP_URL];
	//	string streamName = "";
	//	if (!pFrom->GetCustomParameters().HasKey("initialStreamName")) {
	//		streamName = GetStreamName(uri);
	//		pFrom->GetCustomParameters()["initialStreamName"] = streamName;
	//	} else {
	//		streamName = (string) pFrom->GetCustomParameters()["initialStreamName"];
	//	}
	//	replace(uri, streamName, pFrom->GetCustomParameters()["customStreamName"]);
	//	requestHeaders[RTSP_FIRST_LINE][RTSP_URL] = uri;
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
