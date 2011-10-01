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


#ifndef _PROXYPUBLISHAPPLICATION_H
#define _PROXYPUBLISHAPPLICATION_H

#include "application/baseclientapplication.h"

class BaseInStream;

namespace app_proxypublish {
#ifdef HAS_PROTOCOL_RTMP
	class RTMPAppProtocolHandler;
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_LIVEFLV
	class LiveFLVAppProtocolHandler;
#endif /* HAS_PROTOCOL_LIVEFLV */
#ifdef HAS_PROTOCOL_RTP
	class RTPAppProtocolHandler;
	class RTSPAppProtocolHandler;
#endif /* HAS_PROTOCOL_RTP */
	class JobsTimerAppProtocolHandler;

	class ProxyPublishApplication
	: public BaseClientApplication {
	private:
#ifdef HAS_PROTOCOL_RTMP
		RTMPAppProtocolHandler *_pRTMPHandler;
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_LIVEFLV
		LiveFLVAppProtocolHandler *_pLiveFLVHandler;
#endif /* HAS_PROTOCOL_LIVEFLV */
#ifdef HAS_PROTOCOL_RTP
		RTPAppProtocolHandler *_pRTPHandler;
		RTSPAppProtocolHandler *_pRTSPHandler;
#endif /* HAS_PROTOCOL_RTP */
		JobsTimerAppProtocolHandler *_pJobsHandler;
		map<uint32_t, uint32_t> _protocolsToStream;
		map<uint32_t, map<uint32_t, uint32_t> >_streamToProtocols;
		Variant _targetServers;
		bool _abortOnConnectError;
		uint32_t _jobsTimerProtocolId;
	public:
		ProxyPublishApplication(Variant &configuration);
		virtual ~ProxyPublishApplication();

		virtual bool Initialize();

		virtual void UnRegisterProtocol(BaseProtocol *pProtocol);

		virtual void SignalStreamRegistered(BaseStream *pStream);
	private:
		void EnqueuePush(Variant &parameters);
		void EnqueuePull(Variant &parameters);
		bool InitiateForwardingStream(BaseInStream *pStream);
		bool InitiateForwardingStream(BaseInStream *pStream, Variant &target);
	};
}

#endif	/* _PROXYPUBLISHAPPLICATION_H */


