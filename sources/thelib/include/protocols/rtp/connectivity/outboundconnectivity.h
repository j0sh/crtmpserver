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
#ifndef _OUTBOUNDCONNECTIVITY_H
#define	_OUTBOUNDCONNECTIVITY_H

#include "protocols/rtp/connectivity/baseconnectivity.h"

class BaseOutNetRTPUDPStream;

class DLLEXP OutboundConnectivity
: public BaseConnectivity {
private:
	int32_t _videoDataFd;
	uint16_t _videoDataPort;
	int32_t _videoRTCPFd;
	uint16_t _videoRTCPPort;
	uint32_t _videoPacketsCount;
	uint32_t _videoBytesCount;

	int32_t _audioDataFd;
	uint16_t _audioDataPort;
	int32_t _audioRTCPFd;
	uint16_t _audioRTCPPort;
	uint32_t _audioPacketsCount;
	uint32_t _audioBytesCount;

	map<uint32_t, sockaddr_in> _udpVideoDataClients;
	map<uint32_t, sockaddr_in> _udpVideoRTCPClients;
	map<uint32_t, sockaddr_in> _udpAudioDataClients;
	map<uint32_t, sockaddr_in> _udpAudioRTCPClients;
	map<uint32_t, uint32_t> _tcpClients;
	BaseOutNetRTPUDPStream *_pOutStream;
	msghdr _message;
public:
	OutboundConnectivity();
	virtual ~OutboundConnectivity();

	bool Initialize();

	BaseOutNetRTPUDPStream * GetOutStream();
	void SetOutStream(BaseOutNetRTPUDPStream *pOutStream);

	string GetVideoServerPorts();
	string GetAudioServerPorts();
	uint32_t GetSSRC();

	void RegisterUDPVideoClient(uint32_t protocolId, sockaddr_in &data,
			sockaddr_in &rtcp);
	void RegisterUDPAudioClient(uint32_t protocolId, sockaddr_in &data,
			sockaddr_in &rtcp);
	void RegisterTCPClient(uint32_t protocolId);
	void UnRegisterClient(uint32_t protocolId);
	bool HasClients();

	void SignalDetachedFromInStream();

	bool FeedVideoData(uint8_t *pBuffer, uint32_t length);
	bool FeedAudioData(uint8_t *pBuffer, uint32_t length);
	bool FeedVideoData(msghdr &message);
	bool FeedAudioData(msghdr &message);
private:
	bool InitializePorts(int32_t &dataFd, uint16_t &dataPort,
			int32_t &RTCPFd, uint16_t &RTCPPort);
	bool FeedVideoDataUDP(msghdr &message);
	bool FeedVideoDataTCP(msghdr &message);
	bool FeedAudioDataUDP(msghdr &message);
	bool FeedAudioDataTCP(msghdr &message);
	bool CreateRTCPPacket(uint8_t *pDest, uint8_t *pSrc,
			uint32_t ssrc, uint32_t rate, uint32_t packetsCount,
			uint32_t bytesCount, bool isAudio);
};


#endif	/* _OUTBOUNDCONNECTIVITY_H */
#endif /* HAS_PROTOCOL_RTP */

