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
#ifndef _INBOUNDCONNECTIVITY_H
#define	_INBOUNDCONNECTIVITY_H

#include "protocols/rtp/connectivity/baseconnectivity.h"

class InboundRTPProtocol;
class RTCPProtocol;
class InNetRTPStream;
class RTSPProtocol;
class BaseProtocol;

class DLLEXP InboundConnectivity
: public BaseConnectivity {
private:
	RTSPProtocol *_pRTSP;

	InboundRTPProtocol *_pRTPVideo;
	RTCPProtocol *_pRTCPVideo;
	uint8_t _videoRR[60];
	InboundRTPProtocol *_pRTPAudio;
	RTCPProtocol *_pRTCPAudio;
	uint8_t _audioRR[60];

	InNetRTPStream *_pInStream;

	BaseProtocol *_pProtocols[256];
	IOBuffer _inputBuffer;
	sockaddr_in _dummyAddress;

	bool _forceTcp;
public:
	InboundConnectivity(RTSPProtocol *pRTSP);
	virtual ~InboundConnectivity();
	void EnqueueForDelete();

	bool Initialize(Variant &videoTrack, Variant &audioTrack,
			string streamName, bool forceTcp);

	string GetTransportHeaderLine(bool isAudio);

	bool FeedData(uint32_t channelId, uint8_t *pBuffer, uint32_t bufferLength);

	string GetAudioClientPorts();
	string GetVideoClientPorts();
	bool SendRR(bool isAudio);
	void ReportSR(uint64_t ntpMicroseconds, uint32_t rtpTimestamp, bool isAudio);
private:
	bool InitializeUDP(Variant &videoTrack, Variant &audioTrack);
	bool InitializeTCP(Variant &videoTrack, Variant &audioTrack);
	void Cleanup();
	bool CreateCarriers(InboundRTPProtocol *pRTP, RTCPProtocol *pRTCP);
};


#endif	/* _INBOUNDCONNECTIVITY_H */
#endif /* HAS_PROTOCOL_RTP */

