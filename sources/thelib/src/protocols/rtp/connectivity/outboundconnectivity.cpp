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
#include "protocols/rtp/rtspprotocol.h"

OutboundConnectivity::OutboundConnectivity(bool forceTcp, RTSPProtocol *pRTSPProtocol)
: BaseConnectivity() {
	_forceTcp = forceTcp;
	_pRTSPProtocol = pRTSPProtocol;

	_pOutStream = NULL;

	memset(&_dataMessage, 0, sizeof (_dataMessage));
	_dataMessage.msg_iov = new iovec[1];
	_dataMessage.msg_iovlen = 1;
	_dataMessage.msg_namelen = sizeof (sockaddr_in);

	memset(&_rtcpMessage, 0, sizeof (_rtcpMessage));
	_rtcpMessage.msg_iov = new iovec[1];
	_rtcpMessage.msg_iovlen = 1;
	_rtcpMessage.msg_namelen = sizeof (sockaddr_in);
	_rtcpMessage.msg_iov[0].iov_len = 28;
	_rtcpMessage.msg_iov[0].iov_base = new uint8_t[_rtcpMessage.msg_iov[0].iov_len];
	((uint8_t *) _rtcpMessage.msg_iov[0].iov_base)[0] = 0x80; //V,P,RC
	((uint8_t *) _rtcpMessage.msg_iov[0].iov_base)[1] = 0xc8; //PT=SR=200
	EHTONSP(((uint8_t *) _rtcpMessage.msg_iov[0].iov_base) + 2, 6); //length
	EHTONLP(((uint8_t *) _rtcpMessage.msg_iov[0].iov_base) + 4, 0); //SSRC
	_pRTCPNTP = ((uint8_t *) _rtcpMessage.msg_iov[0].iov_base) + 8;
	_pRTCPRTP = ((uint8_t *) _rtcpMessage.msg_iov[0].iov_base) + 16;
	_pRTCPSPC = ((uint8_t *) _rtcpMessage.msg_iov[0].iov_base) + 20;
	_pRTCPSOC = ((uint8_t *) _rtcpMessage.msg_iov[0].iov_base) + 24;

	_hasAudio = false;
	_hasVideo = false;
	_videoDataFd = -1;
	_videoDataPort = 0;
	_videoRTCPFd = -1;
	_videoRTCPPort = 0;
	_audioDataFd = -1;
	_audioDataPort = 0;
	_audioRTCPFd = -1;
	_audioRTCPPort = 0;

	_startupTime = (uint64_t) time(NULL);
}

OutboundConnectivity::~OutboundConnectivity() {
	delete[] _dataMessage.msg_iov;
	delete[] ((uint8_t *) _rtcpMessage.msg_iov[0].iov_base);
	delete[] _rtcpMessage.msg_iov;
	if (_pOutStream != NULL) {
		delete _pOutStream;
	}
	CLOSE_SOCKET(_videoDataFd);
	CLOSE_SOCKET(_videoRTCPFd);
	CLOSE_SOCKET(_audioDataFd);
	CLOSE_SOCKET(_audioRTCPFd);
}

bool OutboundConnectivity::Initialize() {
	if (_forceTcp) {
		_rtpClient.audioDataChannel = 0;
		_rtpClient.audioRtcpChannel = 1;
		_rtpClient.videoDataChannel = 2;
		_rtpClient.videoRtcpChannel = 3;
	} else {
		if (!InitializePorts(_videoDataFd, _videoDataPort, _videoRTCPFd, _videoRTCPPort)) {
			FATAL("Unable to initialize video ports");
			return false;
		}
		if (!InitializePorts(_audioDataFd, _audioDataPort, _audioRTCPFd, _audioRTCPPort)) {
			FATAL("Unable to initialize audio ports");
			return false;
		}
	}
	return true;
}

void OutboundConnectivity::SetOutStream(BaseOutNetRTPUDPStream *pOutStream) {
	_pOutStream = pOutStream;
}

string OutboundConnectivity::GetVideoPorts() {
	return format("%"PRIu16"-%"PRIu16, _videoDataPort, _videoRTCPPort);
}

string OutboundConnectivity::GetAudioPorts() {
	return format("%"PRIu16"-%"PRIu16, _audioDataPort, _audioRTCPPort);
}

string OutboundConnectivity::GetVideoChannels() {
	return format("%"PRIu8"-%"PRIu8, _rtpClient.videoDataChannel,
			_rtpClient.videoRtcpChannel);
}

string OutboundConnectivity::GetAudioChannels() {
	return format("%"PRIu8"-%"PRIu8, _rtpClient.audioDataChannel,
			_rtpClient.audioRtcpChannel);
}

uint32_t OutboundConnectivity::GetAudioSSRC() {
	if (_pOutStream != NULL)
		return _pOutStream->AudioSSRC();
	return 0;
}

uint32_t OutboundConnectivity::GetVideoSSRC() {
	if (_pOutStream != NULL)
		return _pOutStream->VideoSSRC();
	return 0;
}

uint16_t OutboundConnectivity::GetLastVideoSequence() {
	return _pOutStream->VideoCounter();
}

uint16_t OutboundConnectivity::GetLastAudioSequence() {
	return _pOutStream->AudioCounter();
}

void OutboundConnectivity::HasAudio(bool value) {
	_hasAudio = value;
	_pOutStream->HasAudioVideo(_hasAudio, _hasVideo);
}

void OutboundConnectivity::HasVideo(bool value) {
	_hasVideo = value;
	_pOutStream->HasAudioVideo(_hasAudio, _hasVideo);
}

bool OutboundConnectivity::RegisterUDPVideoClient(uint32_t rtspProtocolId,
		sockaddr_in &data, sockaddr_in &rtcp) {
	if (_rtpClient.hasVideo) {
		FATAL("Client already registered for video feed");
		return false;
	}
	_rtpClient.hasVideo = true;
	_rtpClient.isUdp = true;
	_rtpClient.videoDataAddress = data;
	_rtpClient.videoRtcpAddress = rtcp;
	_rtpClient.protocolId = rtspProtocolId;
	return true;
}

bool OutboundConnectivity::RegisterUDPAudioClient(uint32_t rtspProtocolId,
		sockaddr_in &data, sockaddr_in &rtcp) {
	if (_rtpClient.hasAudio) {
		FATAL("Client already registered for audio feed");
		return false;
	}
	_rtpClient.hasAudio = true;
	_rtpClient.isUdp = true;
	_rtpClient.audioDataAddress = data;
	_rtpClient.audioRtcpAddress = rtcp;
	_rtpClient.protocolId = rtspProtocolId;
	return true;
}

bool OutboundConnectivity::RegisterTCPVideoClient(uint32_t rtspProtocolId,
		uint8_t data, uint8_t rtcp) {
	if (_rtpClient.hasVideo) {
		FATAL("Client already registered for video feed");
		return false;
	}
	_rtpClient.hasVideo = true;
	_rtpClient.isUdp = false;
	_rtpClient.videoDataChannel = data;
	_rtpClient.videoRtcpChannel = rtcp;
	_rtpClient.protocolId = rtspProtocolId;
	return true;
}

bool OutboundConnectivity::RegisterTCPAudioClient(uint32_t rtspProtocolId,
		uint8_t data, uint8_t rtcp) {
	if (_rtpClient.hasAudio) {
		FATAL("Client already registered for audio feed");
		return false;
	}
	_rtpClient.hasAudio = true;
	_rtpClient.isUdp = false;
	_rtpClient.audioDataChannel = data;
	_rtpClient.audioRtcpChannel = rtcp;
	_rtpClient.protocolId = rtspProtocolId;
	return true;
}

void OutboundConnectivity::SignalDetachedFromInStream() {
	BaseProtocol *pProtocol = ProtocolManager::GetProtocol(_rtpClient.protocolId);
	if (pProtocol != NULL) {
		pProtocol->EnqueueForDelete();
	}
	_pRTSPProtocol = NULL;
}

bool OutboundConnectivity::FeedVideoData(msghdr &message,
		double absoluteTimestamp) {
	if (!FeedData(message, absoluteTimestamp, false)) {
		FATAL("Unable to feed video UDP clients");
		return false;
	}
	return true;
}

bool OutboundConnectivity::FeedAudioData(msghdr &message,
		double absoluteTimestamp) {
	if (!FeedData(message, absoluteTimestamp, true)) {
		FATAL("Unable to feed audio UDP clients");
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

		sockaddr_in address;
		memset(&address, 0, sizeof (address));
		address.sin_family = AF_INET;
		address.sin_port = 0;
		address.sin_addr.s_addr = INADDR_ANY;
		if (bind(dataFd, (sockaddr *) & address, sizeof (address)) != 0) {
			int err = errno;
			WARN("Unable to bind. Error: %s (%d)", strerror(err), err);
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
			int err = errno;
			WARN("Unable to bind on port %"PRIu16". Error: %s (%d)",
					ENTOHS(address.sin_port), strerror(err), err);
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

bool OutboundConnectivity::FeedData(msghdr &message, double absoluteTimestamp,
		bool isAudio) {
	if (absoluteTimestamp == 0)
		return true;

	double rate = isAudio ? _pOutStream->GetCapabilities()->aac._sampleRate : 90000.0;
	uint32_t ssrc = isAudio ? _pOutStream->AudioSSRC() : _pOutStream->VideoSSRC();
	uint16_t messageLength = 0;
	for (uint32_t i = 0; i < (uint32_t) message.msg_iovlen; i++) {
		messageLength += message.msg_iov[i].iov_len;
	}

	bool &hasTrack = isAudio ? _rtpClient.hasAudio : _rtpClient.hasVideo;
	uint32_t &packetsCount = isAudio ? _rtpClient.audioPacketsCount : _rtpClient.videoPacketsCount;
	uint32_t &bytesCount = isAudio ? _rtpClient.audioBytesCount : _rtpClient.videoBytesCount;
	uint32_t &startRTP = isAudio ? _rtpClient.audioStartRTP : _rtpClient.videoStartRTP;
	double &startTS = isAudio ? _rtpClient.audioStartTS : _rtpClient.videoStartTS;
	if (!hasTrack) {
		return true;
	}

	if (startRTP == 0xffffffff) {
		startRTP = ENTOHLP(((uint8_t *) message.msg_iov[0].iov_base) + 4);
		startTS = absoluteTimestamp;
	}

	if ((packetsCount % 500) == 0) {
		//FINEST("Send %c RTCP: %u", isAudio ? 'A' : 'V', packetsCount);
		EHTONLP(((uint8_t *) _rtcpMessage.msg_iov[0].iov_base) + 4, ssrc); //SSRC


		//NTP
		uint32_t integerValue = (uint32_t) (absoluteTimestamp / 1000.0);
		double fractionValue = (absoluteTimestamp / 1000.0 - ((uint32_t) (absoluteTimestamp / 1000.0)))*4294967296.0;
		uint64_t ntpVal = (_startupTime + integerValue + 2208988800ULL) << 32;
		ntpVal |= (uint32_t) fractionValue;
		EHTONLLP(_pRTCPNTP, ntpVal);

		//RTP
		uint64_t rtp = (uint64_t) (((double) (integerValue) + fractionValue / 4294967296.0) * rate);
		rtp &= 0xffffffff;
		EHTONLP(_pRTCPRTP, (uint32_t) rtp);

		//packet count
		EHTONLP(_pRTCPSPC, packetsCount);

		//octet count
		EHTONLP(_pRTCPSOC, bytesCount);
		//			FINEST("\n%s", STR(IOBuffer::DumpBuffer(((uint8_t *) _rtcpMessage.msg_iov[0].iov_base),
		//					_rtcpMessage.msg_iov[0].iov_len)));

		if (_rtpClient.isUdp) {
			int32_t rtcpFd = isAudio ? _audioRTCPFd : _videoRTCPFd;
			sockaddr_in &rtcpAddress = isAudio ? _rtpClient.audioRtcpAddress : _rtpClient.videoRtcpAddress;
			_rtcpMessage.msg_name = &rtcpAddress;
			if (sendmsg(rtcpFd, &_rtcpMessage, 0) <= 0) {
				FATAL("Unable to send message");
				return false;
			}
		} else {
			if (_pRTSPProtocol != NULL) {
				if (!_pRTSPProtocol->SendRaw(&_rtcpMessage,
						_rtcpMessage.msg_iov[0].iov_len, &_rtpClient, isAudio, false)) {
					FATAL("Unable to send raw rtcp audio data");
					return false;
				}
			}
		}
	}


	if (_rtpClient.isUdp) {
		int32_t dataFd = isAudio ? _audioDataFd : _videoDataFd;
		sockaddr_in &dataAddress = isAudio ? _rtpClient.audioDataAddress : _rtpClient.videoDataAddress;
		message.msg_name = &dataAddress;
		if (sendmsg(dataFd, &message, 0) < 0) {
			int err = errno;
			FATAL("Unable to send message: %d; %s", err, strerror(errno));
			return false;
		}
	} else {
		if (_pRTSPProtocol != NULL) {
			if (!_pRTSPProtocol->SendRaw(&message, messageLength, &_rtpClient,
					isAudio, true)) {
				FATAL("Unable to send raw rtcp audio data");
				return false;
			}
		}
	}

	packetsCount++;
	bytesCount += messageLength;

	return true;
}

#endif /* HAS_PROTOCOL_RTP */
