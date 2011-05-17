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

typedef struct _RTPClient {
	uint32_t protocolId;
	bool isUdp;

	bool hasAudio;
	sockaddr_in audioDataAddress;
	sockaddr_in audioRtcpAddress;
	uint32_t audioPacketsCount;
	uint32_t audioBytesCount;
	uint32_t audioStartRTP;
	double audioStartTS;

	bool hasVideo;
	sockaddr_in videoDataAddress;
	sockaddr_in videoRtcpAddress;
	uint32_t videoPacketsCount;
	uint32_t videoBytesCount;
	uint32_t videoStartRTP;
	double videoStartTS;

	_RTPClient() {
		protocolId = 0;
		isUdp = false;

		hasAudio = false;
		memset(&audioDataAddress, 0, sizeof (audioDataAddress));
		memset(&audioRtcpAddress, 0, sizeof (audioRtcpAddress));
		audioPacketsCount = 0;
		audioBytesCount = 0;
		audioStartRTP = 0xffffffff;
		audioStartTS = -1;

		hasVideo = false;
		memset(&videoDataAddress, 0, sizeof (videoDataAddress));
		memset(&videoRtcpAddress, 0, sizeof (videoRtcpAddress));
		videoPacketsCount = 0;
		videoBytesCount = 0;
		videoStartRTP = 0xffffffff;
		videoStartTS = -1;
	}
} RTPClient;

class DLLEXP OutboundConnectivity
: public BaseConnectivity {
private:
	BaseOutNetRTPUDPStream *_pOutStream;
	msghdr _dataMessage;
	msghdr _rtcpMessage;
	uint8_t *_pRTCPNTP;
	uint8_t *_pRTCPRTP;
	uint8_t *_pRTCPSPC;
	uint8_t *_pRTCPSOC;
	uint64_t _startupTime;
	map<uint32_t, RTPClient> _clients;
	bool _hasAudio;
	bool _hasVideo;

	int32_t _videoDataFd;
	uint16_t _videoDataPort;
	int32_t _videoRTCPFd;
	uint16_t _videoRTCPPort;

	int32_t _audioDataFd;
	uint16_t _audioDataPort;
	int32_t _audioRTCPFd;
	uint16_t _audioRTCPPort;
public:
	OutboundConnectivity();
	virtual ~OutboundConnectivity();
	bool Initialize();
	void SetOutStream(BaseOutNetRTPUDPStream *pOutStream);
	string GetVideoServerPorts();
	string GetAudioServerPorts();
	uint32_t GetAudioSSRC();
	uint32_t GetVideoSSRC();
	uint16_t GetLastVideoSequence();
	uint16_t GetLastAudioSequence();
	void HasAudio(bool value);
	void HasVideo(bool value);
	bool RegisterUDPVideoClient1(uint32_t rtspProtocolId, sockaddr_in &data,
			sockaddr_in &rtcp);
	bool RegisterUDPAudioClient1(uint32_t rtspProtocolId, sockaddr_in &data,
			sockaddr_in &rtcp);
	void UnRegisterClient(uint32_t protocolId);
	bool HasClients();
	void SignalDetachedFromInStream();
	bool FeedVideoData(uint8_t *pBuffer, uint32_t length, double absoluteTimestamp);
	bool FeedAudioData(uint8_t *pBuffer, uint32_t length, double absoluteTimestamp);
	bool FeedVideoData(msghdr &message, double absoluteTimestamp);
	bool FeedAudioData(msghdr &message, double absoluteTimestamp);
private:
	bool InitializePorts(int32_t &dataFd, uint16_t &dataPort,
			int32_t &RTCPFd, uint16_t &RTCPPort);
	bool FeedDataUDP(msghdr &message, double absoluteTimestamp, bool isAudio);
	bool FeedDataTCP(msghdr &message, double absoluteTimestamp, bool isAudio);
};


#endif	/* _OUTBOUNDCONNECTIVITY_H */
#endif /* HAS_PROTOCOL_RTP */

