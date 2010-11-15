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
#include "protocols/rtp/connectivity/outboundconnectivity.h"
#include "protocols/rtp/streaming/baseoutnetrtpudpstream.h"
#include "protocols/protocolmanager.h"
#include "protocols/baseprotocol.h"

//#define RTP_DEBUG
#ifdef RTP_DEBUG
#define RTP_DEBUG_MESSAGE(sent) \
string message = format("%d %02x-%02x-%02x%02x-%02x%02x%02x%02x-%02x%02x%02x%02x", \
    _videoData.msg_iov[0].iov_len, \
    ((uint8_t *) _videoData.msg_iov[0].iov_base)[0], \
    ((uint8_t *) _videoData.msg_iov[0].iov_base)[1], \
    ((uint8_t *) _videoData.msg_iov[0].iov_base)[2], \
    ((uint8_t *) _videoData.msg_iov[0].iov_base)[3], \
    ((uint8_t *) _videoData.msg_iov[0].iov_base)[4], \
    ((uint8_t *) _videoData.msg_iov[0].iov_base)[5], \
    ((uint8_t *) _videoData.msg_iov[0].iov_base)[6], \
    ((uint8_t *) _videoData.msg_iov[0].iov_base)[7], \
    ((uint8_t *) _videoData.msg_iov[0].iov_base)[8], \
    ((uint8_t *) _videoData.msg_iov[0].iov_base)[9], \
    ((uint8_t *) _videoData.msg_iov[0].iov_base)[10], \
    ((uint8_t *) _videoData.msg_iov[0].iov_base)[11]); \
if (_videoData.msg_iov[0].iov_len == 12) { \
    message += format(" %02x%02x %s", \
        ((uint8_t *) _videoData.msg_iov[1].iov_base)[0], \
        ((uint8_t *) _videoData.msg_iov[1].iov_base)[1], \
        STR(NALUToString(((uint8_t *) _videoData.msg_iov[1].iov_base)[0]))); \
} else { \
    message += format(" %02x%02x %s %s", \
        ((uint8_t *) _videoData.msg_iov[0].iov_base)[12], \
        ((uint8_t *) _videoData.msg_iov[0].iov_base)[13], \
        STR(NALUToString(((uint8_t *) _videoData.msg_iov[0].iov_base)[13])), \
        STR(NALUToString(((uint8_t *) _videoData.msg_iov[0].iov_base)[12]))); \
} \
message += format(" s: %d; dl: %d; pl: %d; tl: %d;", sent, dataLength, processedLength, totalLength); \
if(processedLength+sentAmount+chunkSize==totalLength) \
    WARN("%s", STR(message)); \
else \
    FINEST("%s", STR(message));
#else
#define RTP_DEBUG_MESSAGE(sent)
#endif

#define RTP_SEND_MESSAGE(fd,vc,vd) \
do { \
    int sent = 0; \
    FOR_MAP(vc, uint32_t, sockaddr_in, i) { \
        vd.msg_name = &MAP_VAL(i); \
        sent = sendmsg(fd, &vd, 0); \
    } \
	/*FATAL("TS: %02x%02x%02x%02x", \
		((uint8_t *)vd.msg_iov[0].iov_base)[4], \
		((uint8_t *)vd.msg_iov[0].iov_base)[5], \
		((uint8_t *)vd.msg_iov[0].iov_base)[6], \
		((uint8_t *)vd.msg_iov[0].iov_base)[7]); */\
	RTP_DEBUG_MESSAGE(sent); \
} \
while (0)

OutboundConnectivity::OutboundConnectivity()
: BaseConnectivity() {
	_videoDataFd = -1;
	_videoDataPort = 0;
	_videoRTCPFd = -1;
	_videoRTCPPort = 0;
	_videoPacketsCount = 0;
	_videoBytesCount = 0;
	_videoFirstRtp = 0;
	_videoRtpTs = 0;
	_videoRtcpSent = false;

	_audioDataFd = -1;
	_audioDataPort = 0;
	_audioRTCPFd = -1;
	_audioRTCPPort = 0;
	_audioPacketsCount = 0;
	_audioBytesCount = 0;
	_audioFirstRtp = 0;
	_videoRtpTs = 0;
	_audioRtcpSent = false;

	_pOutStream = NULL;
	memset(&_message, 0, sizeof (_message));
	_message.msg_iov = new iovec[1];
	_message.msg_iovlen = 1;
	_message.msg_namelen = sizeof (sockaddr_in);

	_startupTime = 0;
}

OutboundConnectivity::~OutboundConnectivity() {
	delete[] _message.msg_iov;
	if (_pOutStream != NULL) {
		delete _pOutStream;
	}
	CLOSE_SOCKET(_videoDataFd);
	CLOSE_SOCKET(_audioDataFd);
	WARN("OC deleted: %p", this);
}

bool OutboundConnectivity::Initialize() {
	if (!InitializePorts(_videoDataFd, _videoDataPort, _videoRTCPFd, _videoRTCPPort)) {
		FATAL("Unable to initialize video ports");
		return false;
	}
	if (!InitializePorts(_audioDataFd, _audioDataPort, _audioRTCPFd, _audioRTCPPort)) {
		FATAL("Unable to initialize audio ports");
		return false;
	}
	return true;
}

BaseOutNetRTPUDPStream * OutboundConnectivity::GetOutStream() {
	return _pOutStream;
}

void OutboundConnectivity::SetOutStream(BaseOutNetRTPUDPStream *pOutStream) {
	_pOutStream = pOutStream;
}

string OutboundConnectivity::GetVideoServerPorts() {
	return format("%d-%d", _videoDataPort, _videoRTCPPort);
}

string OutboundConnectivity::GetAudioServerPorts() {
	return format("%d-%d", _audioDataPort, _audioRTCPPort);
}

uint32_t OutboundConnectivity::GetSSRC() {
	if (_pOutStream != NULL)
		return _pOutStream->SSRC();
	return 0;
}

uint16_t OutboundConnectivity::GetLastVideoSequence() {
	return _pOutStream->VideoCounter();
}

uint32_t OutboundConnectivity::GetLastVideoRTPTimestamp() {
	return _videoRtpTs;
}

uint16_t OutboundConnectivity::GetLastAudioSequence() {
	return _pOutStream->AudioCounter();
}

uint32_t OutboundConnectivity::GetLastAudioRTPTimestamp() {
	return _audioRtpTs;
}

void OutboundConnectivity::RegisterUDPVideoClient(uint32_t protocolId,
		sockaddr_in &data, sockaddr_in &rtcp) {
	_udpVideoDataClients[protocolId] = data;
	_udpVideoRTCPClients[protocolId] = rtcp;
}

void OutboundConnectivity::RegisterUDPAudioClient(uint32_t protocolId,
		sockaddr_in &data, sockaddr_in &rtcp) {
	_udpAudioDataClients[protocolId] = data;
	_udpAudioRTCPClients[protocolId] = rtcp;
}

void OutboundConnectivity::RegisterTCPClient(uint32_t protocolId) {
	_tcpClients[protocolId] = protocolId;
}

void OutboundConnectivity::UnRegisterClient(uint32_t protocolId) {
	_udpVideoDataClients.erase(protocolId);
	_udpVideoRTCPClients.erase(protocolId);
	_udpAudioDataClients.erase(protocolId);
	_udpAudioRTCPClients.erase(protocolId);
	_tcpClients.erase(protocolId);
}

bool OutboundConnectivity::HasClients() {
	return (_udpVideoDataClients.size() != 0)
			|| (_udpVideoRTCPClients.size() != 0)
			|| (_udpAudioDataClients.size() != 0)
			|| (_udpAudioRTCPClients.size() != 0)
			|| (_tcpClients.size() != 0);
}

void OutboundConnectivity::SignalDetachedFromInStream() {

	FOR_MAP(_udpVideoDataClients, uint32_t, sockaddr_in, i) {
		BaseProtocol *pProtocol = ProtocolManager::GetProtocol(MAP_KEY(i));
		if (pProtocol != NULL)
			pProtocol->EnqueueForDelete();
	}

	FOR_MAP(_udpVideoRTCPClients, uint32_t, sockaddr_in, i) {
		BaseProtocol *pProtocol = ProtocolManager::GetProtocol(MAP_KEY(i));
		if (pProtocol != NULL)
			pProtocol->EnqueueForDelete();
	}

	FOR_MAP(_udpAudioDataClients, uint32_t, sockaddr_in, i) {
		BaseProtocol *pProtocol = ProtocolManager::GetProtocol(MAP_KEY(i));
		if (pProtocol != NULL)
			pProtocol->EnqueueForDelete();
	}

	FOR_MAP(_udpAudioRTCPClients, uint32_t, sockaddr_in, i) {
		BaseProtocol *pProtocol = ProtocolManager::GetProtocol(MAP_KEY(i));
		if (pProtocol != NULL)
			pProtocol->EnqueueForDelete();
	}

	FOR_MAP(_tcpClients, uint32_t, uint32_t, i) {
		BaseProtocol *pProtocol = ProtocolManager::GetProtocol(MAP_KEY(i));
		if (pProtocol != NULL)
			pProtocol->EnqueueForDelete();
	}
}

bool OutboundConnectivity::FeedVideoData(uint8_t *pBuffer, uint32_t length) {
	_message.msg_iov[0].iov_base = pBuffer;
	_message.msg_iov[0].iov_len = length;
	return FeedVideoData(_message);
}

bool OutboundConnectivity::FeedAudioData(uint8_t *pBuffer, uint32_t length) {
	_message.msg_iov[0].iov_base = pBuffer;
	_message.msg_iov[0].iov_len = length;
	return FeedAudioData(_message);
}

bool OutboundConnectivity::FeedVideoData(msghdr &message) {
	_videoRtpTs = ENTOHLP(((uint8_t *) message.msg_iov[0].iov_base) + 4);
	if (!FeedVideoDataUDP(message)) {
		FATAL("Unable to feed video UDP clients");
		return false;
	}
	if (!FeedVideoDataTCP(message)) {
		FATAL("Unable to feed video TCP clients");
		return false;
	}

	return true;
}

bool OutboundConnectivity::FeedAudioData(msghdr &message) {
	_audioRtpTs = ENTOHLP(((uint8_t *) message.msg_iov[0].iov_base) + 4);
	if (!FeedAudioDataUDP(message)) {
		FATAL("Unable to feed audio UDP clients");
		return false;
	}
	if (!FeedAudioDataTCP(message)) {
		FATAL("Unable to feed audio TCP clients");
		return false;
	}

	return true;
}

bool OutboundConnectivity::InitializePorts(int32_t &dataFd, uint16_t &dataPort,
		int32_t &RTCPFd, uint16_t &RTCPPort) {
	for (int i = 0; i < 10; i++) {
		CLOSE_SOCKET(dataFd);
		CLOSE_SOCKET(RTCPFd);

		dataFd = socket(AF_INET, SOCK_DGRAM, 0);
		RTCPFd = socket(AF_INET, SOCK_DGRAM, 0);

		sockaddr_in address = {0};
		address.sin_family = AF_INET;
		address.sin_port = 0;
		address.sin_addr.s_addr = INADDR_ANY;
		if (bind(dataFd, (sockaddr *) & address, sizeof (address)) != 0) {
			WARN("Unable to bind");
			continue;
		}
		socklen_t len = sizeof (sockaddr);
		if (getsockname(dataFd, (sockaddr *) & address, &len) != 0) {
			ASSERT("Unable to get peer's address");
		}
		dataPort = ENTOHS(address.sin_port);

		if ((dataPort % 2) != 0) {
			address.sin_port = EHTONS(dataPort - 1);
		} else {
			address.sin_port = EHTONS(dataPort + 1);
		}
		if (bind(RTCPFd, (sockaddr *) & address, sizeof (address)) != 0) {
			WARN("Unable to bind");
			continue;
		}
		RTCPPort = ENTOHS(address.sin_port);

		if ((dataPort % 2) != 0) {
			uint16_t temp16 = dataPort;
			dataPort = RTCPPort;
			RTCPPort = temp16;

			int32_t temp32 = dataFd;
			dataFd = RTCPFd;
			RTCPFd = temp32;
			WARN("Ports switch");
		}

		return true;
	}

	FATAL("Unable to bind after 10 attempts");
	return false;
}
#define COMPUTE_BYTES_COUNT(m,c) \
	for(uint32_t i=0;i<(uint32_t)m.msg_iovlen;i++){ \
		c+=m.msg_iov[i].iov_len-12; \
	}

bool OutboundConnectivity::FeedVideoDataUDP(msghdr &message) {
	//	uint32_t packetRtp = ENTOHLP((uint8_t *) message.msg_iov[0].iov_base + 4);
	//	if ((((uint8_t *) message.msg_iov[0].iov_base)[1] >> 7) != 0) {
	//		//		FINEST("packetRtp: %d (%08x) %02x%02x%02x%02x %.2f %c",
	//		//				packetRtp,
	//		//				packetRtp,
	//		//				((uint8_t *) message.msg_iov[0].iov_base)[4],
	//		//				((uint8_t *) message.msg_iov[0].iov_base)[5],
	//		//				((uint8_t *) message.msg_iov[0].iov_base)[6],
	//		//				((uint8_t *) message.msg_iov[0].iov_base)[7],
	//		//				(double) packetRtp / 90000.0,
	//		//				((((uint8_t *) message.msg_iov[0].iov_base)[1] >> 7) != 0) ? '*' : ' ');
	//		if (____last == packetRtp) {
	//			WARN("Skip packet");
	//			return true;
	//		}
	//		____last = packetRtp;
	//	}
	RTP_SEND_MESSAGE(_videoDataFd, _udpVideoDataClients, message);
	_videoPacketsCount++;
	COMPUTE_BYTES_COUNT(message, _videoBytesCount);
	//uint16_t seq = ENTOHSP(((uint8_t *) message.msg_iov[0].iov_base) + 2);
	//FINEST("seq: %d", seq);
	if (((_videoPacketsCount % 300) == 0) || _videoPacketsCount <= 2) {
		uint8_t buff[28];
		if (CreateRTCPPacket(buff,
				(uint8_t *) message.msg_iov[0].iov_base,
				_pOutStream->SSRC(),
				90000,
				_videoPacketsCount,
				_videoBytesCount,
				false)) {
			_message.msg_iov[0].iov_base = buff;
			_message.msg_iov[0].iov_len = 28;

			RTP_SEND_MESSAGE(_videoRTCPFd, _udpVideoRTCPClients, _message);
		}
	}
	return true;
}

bool OutboundConnectivity::FeedVideoDataTCP(msghdr &message) {
	//NYIR;
	return true;
}

bool OutboundConnectivity::FeedAudioDataUDP(msghdr &message) {
	RTP_SEND_MESSAGE(_audioDataFd, _udpAudioDataClients, message);
	_audioPacketsCount++;
	COMPUTE_BYTES_COUNT(message, _audioBytesCount);
	//uint16_t seq = ENTOHSP(((uint8_t *) message.msg_iov[0].iov_base) + 2);
	//FINEST("seq: %d", seq);
	if (((_audioPacketsCount % 300) == 0) || (_audioPacketsCount <= 2)) {
		uint8_t buff[28];
		if (CreateRTCPPacket(buff,
				(uint8_t *) message.msg_iov[0].iov_base,
				_pOutStream->SSRC(),
				_pOutStream->GetCapabilities()->aac._sampleRate,
				_audioPacketsCount,
				_audioBytesCount,
				true)) {
			_message.msg_iov[0].iov_base = buff;
			_message.msg_iov[0].iov_len = 28;

			RTP_SEND_MESSAGE(_audioRTCPFd, _udpAudioRTCPClients, _message);
		}
	}
	return true;
}

bool OutboundConnectivity::FeedAudioDataTCP(msghdr &message) {
	//NYIR;
	return true;
}

bool OutboundConnectivity::CreateRTCPPacket_mystyle(uint8_t *pDest, uint8_t *pSrc,
		uint32_t ssrc, uint32_t rate, uint32_t packetsCount, uint32_t bytesCount,
		bool isAudio) {

	/*
	 0                   1                   2                   3
	 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|V=2|P|    RC   |   PT=SR=200   |             length            | header
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                         SSRC of sender                        |
	+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
	|              NTP timestamp, most significant word             | sender
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ info
	|             NTP timestamp, least significant word             |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                         RTP timestamp                         |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                     sender's packet count                     |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                      sender's octet count                     |
	+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
	 */

	//1. V,P,RC
	pDest[0] = 0x80;

	//2. PT
	pDest[1] = 0xc8;

	//3. Length
	pDest[2] = 0x00;
	pDest[3] = 0x06;

	//4. ssrc
	EHTONLP(pDest + 4, ssrc); //SSRC

	//5. setup the startup time
	if (_startupTime == 0) {
		GETCLOCKS(_startupTime);
	}

	if (isAudio) {
		if (_audioFirstRtp == 0) {
			_audioFirstRtp = ENTOHLP(pSrc + 4);
			return false;
		}
	} else {
		if (_videoFirstRtp == 0) {
			_videoFirstRtp = ENTOHLP(pSrc + 4);
			return false;
		}
	}

	uint32_t &firstRtp = isAudio ? _audioFirstRtp : _videoFirstRtp;

	//6. Get the current time
	double currentTime;
	GETCLOCKS(currentTime);

	//7. NTP
	uint64_t ntp;
	GETCUSTOMNTP(ntp, currentTime);
	EHTONLLP(pDest + 8, ntp);

	//6. RTP
	double rtpDouble = ((currentTime - _startupTime) / (double) CLOCKS_PER_SECOND) * rate;
	uint32_t rtp = (uint32_t) rtpDouble + firstRtp;
	EHTONLP(pDest + 16, rtp);

	//7. sender's packet count
	EHTONLP(pDest + 20, packetsCount);

	//8. sender's octet count
	EHTONLP(pDest + 24, bytesCount);

	FINEST("-----%s-----", isAudio ? "AUDIO" : "VIDEO");
	FINEST("_startupTime: %.2f", _startupTime);
	FINEST("currentTime: %.2f", currentTime);
	FINEST("currentTime - _startupTime: %.2f (%.4f)",
			currentTime - _startupTime,
			(currentTime - _startupTime) / (double) CLOCKS_PER_SECOND);
	FINEST("rate: %d", rate);
	FINEST("rtpDouble: %.2f", rtpDouble);
	FINEST("firstRtp: %d", firstRtp);
	FINEST("rtp: %d", rtp);
	uint32_t packetRtp = ENTOHLP(pSrc + 4);
	FINEST("packetRtp: %d", packetRtp);
	FINEST("diff: %d; (%.4f s)", packetRtp - rtp, ((double) packetRtp - rtp) / (double) rate);
	FINEST("---------------");

	return true;
}

bool OutboundConnectivity::CreateRTCPPacket_mystyle_only_once(uint8_t *pDest, uint8_t *pSrc,
		uint32_t ssrc, uint32_t rate, uint32_t packetsCount,
		uint32_t bytesCount, bool isAudio) {
	bool &rtcpSent = isAudio ? _audioRtcpSent : _videoRtcpSent;
	if (rtcpSent)
		return false;

	rtcpSent = CreateRTCPPacket_mystyle(pDest, pSrc, ssrc, rate, packetsCount,
			bytesCount, isAudio);
	return rtcpSent;
}

bool OutboundConnectivity::CreateRTCPPacket_live555style(uint8_t *pDest, uint8_t *pSrc,
		uint32_t ssrc, uint32_t rate, uint32_t packetsCount, uint32_t bytesCount,
		bool isAudio) {

	/*
	 0                   1                   2                   3
	 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|V=2|P|    RC   |   PT=SR=200   |             length            | header
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                         SSRC of sender                        |
	+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
	|              NTP timestamp, most significant word             | sender
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ info
	|             NTP timestamp, least significant word             |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                         RTP timestamp                         |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                     sender's packet count                     |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                      sender's octet count                     |
	+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
	 */

	//1. V,P,RC
	pDest[0] = 0x80;

	//2. PT
	pDest[1] = 0xc8;

	//3. Length
	pDest[2] = 0x00;
	pDest[3] = 0x06;

	//4. ssrc
	EHTONLP(pDest + 4, ssrc); //SSRC

	//5. NTP
	struct timeval timeNow;
	gettimeofday(&timeNow, NULL);
	EHTONLP(pDest + 8, timeNow.tv_sec + 0x83AA7E80);
	double fractionalPart = (timeNow.tv_usec / 15625.0)*0x04000000; // 2^32/10^6
	EHTONLP(pDest + 12, (uint32_t) fractionalPart);

	//6. RTP
	EHTONLP(pDest + 16, ToRTPTS(timeNow, rate));

	//7. sender's packet count
	EHTONLP(pDest + 20, packetsCount);

	//8. sender's octet count
	EHTONLP(pDest + 24, bytesCount);

	return true;
}

bool OutboundConnectivity::CreateRTCPPacket_none(uint8_t *pDest, uint8_t *pSrc,
		uint32_t ssrc, uint32_t rate, uint32_t packetsCount,
		uint32_t bytesCount, bool isAudio) {
	return false;
}
#endif /* HAS_PROTOCOL_RTP */
