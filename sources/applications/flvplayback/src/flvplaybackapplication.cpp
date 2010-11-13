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
using namespace app_flvplayback;

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
#endif /* HAS_PROTOCOL_RTP */

	return PullExternalStreams();
}
