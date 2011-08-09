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


#ifndef _FLVPLAYBACKAPPLICATION_H
#define _FLVPLAYBACKAPPLICATION_H

#include "application/baseclientapplication.h"

namespace app_flvplayback {
#ifdef HAS_PROTOCOL_RTMP
	class RTMPAppProtocolHandler;
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_LIVEFLV
	class LiveFLVAppProtocolHandler;
#endif /* HAS_PROTOCOL_LIVEFLV */
#ifdef HAS_PROTOCOL_TS
	class TSAppProtocolHandler;
#endif /* HAS_PROTOCOL_TS */
#ifdef HAS_PROTOCOL_RTP
	class RTPAppProtocolHandler;
	class RTSPAppProtocolHandler;
#endif /* HAS_PROTOCOL_RTP */
#ifdef HAS_PROTOCOL_MMS
	class MMSAppProtocolHandler;
#endif /* HAS_PROTOCOL_MMS */
#ifdef HAS_PROTOCOL_RAWHTTPSTREAM
	class RawHTTPStreamAppProtocolHandler;
#endif /* HAS_PROTOCOL_RAWHTTPSTREAM */
#ifdef HAS_PROTOCOL_HTTP
	class HTTPAppProtocolHandler;
#endif /* HAS_PROTOCOL_HTTP */

	class FLVPlaybackApplication
	: public BaseClientApplication {
	private:
#ifdef HAS_PROTOCOL_RTMP
		RTMPAppProtocolHandler *_pRTMPHandler;
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_LIVEFLV
		LiveFLVAppProtocolHandler *_pLiveFLVHandler;
#endif /* HAS_PROTOCOL_LIVEFLV */
#ifdef HAS_PROTOCOL_TS
		TSAppProtocolHandler *_pTSHandler;
#endif /* HAS_PROTOCOL_TS */
#ifdef HAS_PROTOCOL_RTP
		RTPAppProtocolHandler *_pRTPHandler;
		RTSPAppProtocolHandler *_pRTSPHandler;
#endif /* HAS_PROTOCOL_RTP */
#ifdef HAS_PROTOCOL_MMS
		MMSAppProtocolHandler *_pMMSHandler;
#endif /* HAS_PROTOCOL_MMS */
#ifdef HAS_PROTOCOL_RAWHTTPSTREAM
		RawHTTPStreamAppProtocolHandler *_pRawHTTPStreamHandler;
#endif /* HAS_PROTOCOL_RAWHTTPSTREAM */
#ifdef HAS_PROTOCOL_HTTP
		HTTPAppProtocolHandler *_pHTTPHandler;
#endif /* HAS_PROTOCOL_HTTP */
	public:
		FLVPlaybackApplication(Variant &configuration);
		virtual ~FLVPlaybackApplication();

		virtual bool Initialize();
	};
}

#endif	/* _FLVPLAYBACKAPPLICATION_H */


