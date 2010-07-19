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

class ProxyPublishApplication
: public BaseClientApplication {
private:
#ifdef HAS_PROTOCOL_RTMP
	RTMPAppProtocolHandler *_pRTMPHandler;
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_LIVEFLV
	LiveFLVAppProtocolHandler *_pLiveFLV;
#endif /* HAS_PROTOCOL_LIVEFLV */
#ifdef HAS_PROTOCOL_RTP
	RTPAppProtocolHandler *_pRTP;
	RTSPAppProtocolHandler *_pRTSP;
#endif /* HAS_PROTOCOL_RTP */
	map<uint32_t, uint32_t> _protocolsToStream;
	map<uint32_t, map<uint32_t, uint32_t> >_streamToProtocols;
	Variant _targetServers;
	bool _abortOnConnectError;
public:
	ProxyPublishApplication(Variant &configuration);
	virtual ~ProxyPublishApplication();

	virtual bool Initialize();

	virtual void SignalStreamRegistered(BaseStream *pStream);
	virtual void SignalStreamUnRegistered(BaseStream *pStream);
private:
	bool InitiateForwardingStream(BaseStream *pStream);
	bool InitiateForwardingStream(BaseStream *pStream, Variant &target);
#ifdef HAS_PROTOCOL_RTP
public:
	static bool SignalProtocolCreated(BaseProtocol *pProtocol,
			Variant &parameters);
private:
	bool SpawnRTSPProtocols();
	bool SpawnRTSPProtocol(vector<uint64_t> &chain, Variant &node);
#endif /* HAS_PROTOCOL_RTP */
};


#endif	/* _PROXYPUBLISHAPPLICATION_H */


