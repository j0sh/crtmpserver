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

#ifndef _INBOUNDCONNECTIVITY_H
#define	_INBOUNDCONNECTIVITY_H

#include "protocols/rtp/connectivity/baseconnectivity.h"

class InboundRTPProtocol;
class RTCPProtocol;
class InNetRTPStream;
class RTSPProtocol;

class InboundConnectivity
: public BaseConnectivity {
private:
	RTSPProtocol *_pRTSP;
	InboundRTPProtocol *_pRTPVideo;
	RTCPProtocol *_pRTCPVideo;
	InboundRTPProtocol *_pRTPAudio;
	RTCPProtocol *_pRTCPAudio;
	InNetRTPStream *_pInStream;
public:
	InboundConnectivity(RTSPProtocol *pRTSP);
	virtual ~InboundConnectivity();
	void EnqueueForDelete();

	bool Initialize(Variant &videoTrack, Variant &audioTrack);

	string GetAudioClientPorts();
	string GetVideoClientPorts();
	void GetSSRCAndSeq(uint32_t rtpId, uint32_t &ssrc, uint32_t &seq);
	bool SendRTP(sockaddr_in &address, uint32_t rtpId,
			uint8_t *pBuffer, uint32_t length);
private:
	void Cleanup();
	bool CreateCarriers(InboundRTPProtocol *pRTP, RTCPProtocol *pRTCP);
};


#endif	/* _INBOUNDCONNECTIVITY_H */

