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


#ifdef HAS_PROTOCOL_RTP

#include "rtspappprotocolhandler.h"
#include "protocols/rtp/rtspprotocol.h"
#include "protocols/rtp/sdp.h"
#include "protocols/protocolfactorymanager.h"
#include "netio/netio.h"
#include "flvplaybackapplication.h"
#include "protocols/protocolmanager.h"

RTSPAppProtocolHandler::RTSPAppProtocolHandler(Variant& configuration)
: BaseRTSPAppProtocolHandler(configuration) {

}

RTSPAppProtocolHandler::~RTSPAppProtocolHandler() {
}

void RTSPAppProtocolHandler::RegisterProtocol(BaseProtocol *pProtocol) {
	//1. Is this a client RTSP protocol?
	if (pProtocol->GetType() != PT_RTSP)
		return;
	if (pProtocol->GetCustomParameters() != V_MAP)
		return;
	if (!pProtocol->GetCustomParameters().HasKey("isClient"))
		return;
	if (pProtocol->GetCustomParameters()["isClient"] != V_BOOL)
		return;
	if (!((bool)pProtocol->GetCustomParameters()["isClient"]))
		return;

	//2. Get the protocol
	RTSPProtocol *pRTSPProtocol = (RTSPProtocol *) pProtocol;

	//3. Get the URI
	if (pRTSPProtocol->GetCustomParameters()["url"] != V_STRING) {
		FATAL("No URL specified");
		pRTSPProtocol->EnqueueForDelete();
		return;
	}
	string url = (string) pRTSPProtocol->GetCustomParameters()["url"];

	//4. Start play
	if (!Play(pRTSPProtocol, url)) {
		FATAL("Unable to initiate play on uri %s", STR(url));
		pRTSPProtocol->EnqueueForDelete();
		return;
	}
}

bool RTSPAppProtocolHandler::HandleRTSPResponse200Setup(RTSPProtocol *pFrom,
		Variant &requestHeaders, string &requestContent, Variant &responseHeaders,
		string &responseContent) {
	//1. Call the base class
	if (!BaseRTSPAppProtocolHandler::HandleRTSPResponse200Setup(pFrom,
			requestHeaders, requestContent, responseHeaders, responseContent)) {
		FATAL("Unable to handle response");
		return false;
	}

	//2. Do the play command
	string url = (string) pFrom->GetCustomParameters()["url"] + "/";

	//3. prepare the play command
	pFrom->ClearRequestMessage();
	pFrom->PushRequestFirstLine(RTSP_METHOD_PLAY, url, RTSP_VERSION_1_0);
	pFrom->PushRequestHeader(RTSP_HEADERS_SESSION,
			responseHeaders[RTSP_HEADERS].GetValue(RTSP_HEADERS_SESSION, false));

	//4. Done
	return pFrom->SendRequestMessage();
}


#endif /* HAS_PROTOCOL_RTP */

