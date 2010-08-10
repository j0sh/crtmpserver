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

#include "flvplaybackapplication.h"
#include "protocols/protocoltypes.h"
#include "protocols/rtmp/basertmpprotocol.h"
#include "rtmpappprotocolhandler.h"
#include "liveflvappprotocolhandler.h"
#include "tsappprotocolhandler.h"
#include "rtpappprotocolhandler.h"
#include "rtspappprotocolhandler.h"
#include "netio/netio.h"
#include "application/clientapplicationmanager.h"

FLVPlaybackApplication::FLVPlaybackApplication(Variant &configuration)
: BaseClientApplication(configuration) {
#ifdef HAS_PROTOCOL_RTMP
	_pRTMPHandler = NULL;
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_LIVEFLV
	_pLiveFLV = NULL;
#endif /* HAS_PROTOCOL_LIVEFLV */
#ifdef HAS_PROTOCOL_TS
	_pTS = NULL;
#endif /* HAS_PROTOCOL_TS */
#ifdef HAS_PROTOCOL_RTP
	_pRTP = NULL;
	_pRTSP = NULL;
#endif /* HAS_PROTOCOL_RTP */
}

FLVPlaybackApplication::~FLVPlaybackApplication() {
#ifdef HAS_PROTOCOL_RTMP
	UnRegisterAppProtocolHandler(PT_INBOUND_RTMP);
	UnRegisterAppProtocolHandler(PT_OUTBOUND_RTMP);
	if (_pRTMPHandler != NULL)
		delete _pRTMPHandler;
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_LIVEFLV
	UnRegisterAppProtocolHandler(PT_INBOUND_LIVE_FLV);
	if (_pLiveFLV != NULL)
		delete _pLiveFLV;
#endif /* HAS_PROTOCOL_LIVEFLV */

#ifdef HAS_PROTOCOL_TS
	UnRegisterAppProtocolHandler(PT_INBOUND_TS);
	if (_pTS != NULL)
		delete _pTS;
#endif /* HAS_PROTOCOL_TS */

#ifdef HAS_PROTOCOL_RTP
	UnRegisterAppProtocolHandler(PT_INBOUND_RTP);
	if (_pRTP != NULL)
		delete _pRTP;

	UnRegisterAppProtocolHandler(PT_RTSP);
	if (_pRTSP != NULL)
		delete _pRTSP;
#endif /* HAS_PROTOCOL_RTP */
}

bool FLVPlaybackApplication::Initialize() {
#ifdef HAS_PROTOCOL_RTMP
	_pRTMPHandler = new RTMPAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_INBOUND_RTMP, _pRTMPHandler);
	RegisterAppProtocolHandler(PT_OUTBOUND_RTMP, _pRTMPHandler);
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_LIVEFLV
	_pLiveFLV = new LiveFLVAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_INBOUND_LIVE_FLV, _pLiveFLV);
#endif /* HAS_PROTOCOL_LIVEFLV */

#ifdef HAS_PROTOCOL_TS
	_pTS = new TSAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_INBOUND_TS, _pTS);
#endif /* HAS_PROTOCOL_TS */

#ifdef HAS_PROTOCOL_RTP
	_pRTP = new RTPAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_INBOUND_RTP, _pRTP);
	RegisterAppProtocolHandler(PT_RTCP, _pRTP);

	_pRTSP = new RTSPAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_RTSP, _pRTSP);

	return SpawnRTSPProtocols();
#endif /* HAS_PROTOCOL_RTP */

	return true;
}

#ifdef HAS_PROTOCOL_RTP

bool FLVPlaybackApplication::SpawnRTSPProtocols() {
	//1. Minimal verifications
	if (_configuration["rtspStreams"] == V_NULL)
		return true;

	if (_configuration["rtspStreams"] != V_MAP) {
		FATAL("Invalid rtspStreams node");
		return false;
	}

	//2. get the chain
	vector<uint64_t> chain = ProtocolFactoryManager::ResolveProtocolChain(
			CONF_PROTOCOL_INBOUND_RTSP);
	if (chain.size() == 0) {
		FATAL("Unable to resolve protocol chain");
		return false;
	}

	//3. Spawn the connections

	FOR_MAP(_configuration["rtspStreams"], string, Variant, i) {
		if (!SpawnRTSPProtocol(chain, MAP_VAL(i))) {
			FATAL("Unable to connect here:\n%s", STR(MAP_VAL(i).ToString()));
			return false;
		}
	}

	return true;
}

bool FLVPlaybackApplication::SpawnRTSPProtocol(vector<uint64_t> &chain, Variant &node) {
	//1. Minimal verification
	if (node != V_MAP) {
		FATAL("Invalid node");
		return false;
	}
	if ((node["forceTcp"] != V_BOOL) || (node["url"] != V_STRING)) {
		FATAL("Invalid node");
		return false;
	}

	//2. Split the URI
	string host;
	uint16_t port;
	string user;
	string pwd;
	string doc;

	if (!ParseURL(node["url"], host, port, user, pwd, doc)) {
		FATAL("Invalid URL: %s", STR(node["url"].ToString()));
		return false;
	}

	//3. resolve the host
	string ip = GetHostByName(host);
	if (ip == "") {
		FATAL("Unable to resolve host %s", STR(host));
		return false;
	}

	//4. Save the app id inside the custom parameters
	node["appId"] = GetId();
	node["isClient"] = true;

	//5. Connect
	if (!TCPConnector<FLVPlaybackApplication>::Connect(ip, port, chain, node)) {
		FATAL("Unable to connect to %s:%d", STR(host), port);
		return false;
	}

	//6. Done
	return true;
}

bool FLVPlaybackApplication::SignalProtocolCreated(BaseProtocol *pProtocol,
		Variant &parameters) {
	//1. Sanitize
	if (parameters["appId"] != V_UINT32) {
		FATAL("Invalid custom parameters:\n%s", STR(parameters.ToString()));
		return false;
	}

	//2. Get the application
	BaseClientApplication *pApplication = ClientApplicationManager::FindAppById(
			parameters["appId"]);

	//3. Set it up inside the protocol
	pProtocol->SetApplication(pApplication);

	//4. Done
	return true;
}

#endif /* HAS_PROTOCOL_RTP */
