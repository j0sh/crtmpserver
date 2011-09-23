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
#include "protocols/rtp/rtcpprotocol.h"
#include "protocols/rtp/connectivity/inboundconnectivity.h"

RTCPProtocol::RTCPProtocol()
: BaseProtocol(PT_RTCP) {
	_pConnectivity = NULL;
	_lsr = 0;
	_buff[0] = 0x81; //V,P,RC
	_buff[1] = 0xc9; //PT
	_buff[2] = 0x00; //length
	_buff[3] = 0x07; //length
	EHTONLP(_buff + 4, GetId()); //SSRC of packet sender
	EHTONLP(_buff + 12, 0); //fraction lost/cumulative number of packets lost
	EHTONLP(_buff + 20, 0); //interarrival jitter
	EHTONLP(_buff + 28, 0); // delay since last SR (DLSR)
	_isAudio = false;
	_ssrc = rand();
	_ssrc ^= GetId();
	_validLastAddress = false;
}

RTCPProtocol::~RTCPProtocol() {
}

bool RTCPProtocol::Initialize(Variant &parameters) {
	GetCustomParameters() = parameters;
	return true;
}

bool RTCPProtocol::AllowFarProtocol(uint64_t type) {
	return type == PT_UDP;
}

bool RTCPProtocol::AllowNearProtocol(uint64_t type) {
	NYIR;
}

bool RTCPProtocol::SignalInputData(int32_t recvAmount) {
	ASSERT("operation not supported");
	NYIR;
}

bool RTCPProtocol::SignalInputData(IOBuffer &buffer, sockaddr_in *pPeerAddress) {
	//0. Save the last known address
	if (&_lastAddress != pPeerAddress) {
		_lastAddress = *pPeerAddress;
		_validLastAddress = true;
	}

	//1. Parse the SR
	uint8_t *pBuffer = GETIBPOINTER(buffer);
	uint32_t bufferLength = GETAVAILABLEBYTESCOUNT(buffer);
	if (bufferLength < 16)
		return true;

	uint8_t PT = pBuffer[1];
	uint16_t len = ENTOHSP(pBuffer + 2);
	len = (len + 1)*4;
	if (len > bufferLength) {
		WARN("Invalid RTCP packet length: len %hu; bufferLength: %u", len, bufferLength);
		buffer.IgnoreAll();
		return true;
	}

	switch (PT) {
		case 200: //SR
		{
			if (len < 28) {
				WARN("Invalid RTCP packet length: %hu", len);
				buffer.IgnoreAll();
				return true;
			}
			uint32_t ntpSec = ENTOHLP(pBuffer + 8) - 2208988800UL;
			uint32_t ntpFrac = ENTOHLP(pBuffer + 12);
			uint64_t ntpMicroseconds = (uint32_t) (((double) ntpFrac / (double) (0x100000000LL))*1000000.0);
			ntpMicroseconds += ((uint64_t) ntpSec)*1000000;
			uint32_t rtpTimestamp = ENTOHLP(pBuffer + 16);
			_pConnectivity->ReportSR(ntpMicroseconds, rtpTimestamp, _isAudio);
			break;
		}
		default:
		{
			WARN("Unknown packet type: %hhu", PT);
			buffer.IgnoreAll();
			return true;
		}
	}

	if (pBuffer[1] != 200)
		return true;

	_lsr = ENTOHLP(pBuffer + 10);
	buffer.IgnoreAll();

	//2. Send the RR
	if (_pConnectivity == NULL) {
		FATAL("no connectivity");
		return false;
	}
	if (!_pConnectivity->SendRR(_isAudio)) {
		FATAL("Unable to send RR");
		_pConnectivity->EnqueueForDelete();
		_pConnectivity = NULL;
		return false;
	}

	return true;
}

bool RTCPProtocol::SignalInputData(IOBuffer &buffer) {
	return SignalInputData(buffer, &_lastAddress);
}

uint32_t RTCPProtocol::GetLastSenderReport() {
	return _lsr;
}

sockaddr_in *RTCPProtocol::GetLastAddress() {
	if (_validLastAddress)
		return &_lastAddress;
	else
		return NULL;
}

uint32_t RTCPProtocol::GetSSRC() {
	return _ssrc;
}

void RTCPProtocol::SetInbboundConnectivity(InboundConnectivity *pConnectivity, bool isAudio) {
	_pConnectivity = pConnectivity;
	_isAudio = isAudio;
}
#endif /* HAS_PROTOCOL_RTP */
