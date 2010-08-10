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
	put_htonl(_buff + 4, GetId()); //SSRC of packet sender
	put_htonl(_buff + 12, 0); //fraction lost/cumulative number of packets lost
	put_htonl(_buff + 20, 0); //interarrival jitter
	put_htonl(_buff + 28, 0); // delay since last SR (DLSR)
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
	//1. Parse the SR
	uint8_t *pBuffer = GETIBPOINTER(buffer);
	uint32_t length = GETAVAILABLEBYTESCOUNT(buffer);
	if (length < 16)
		return true;

	if (pBuffer[1] != 200)
		return true;

	_lsr = ntohlp(pBuffer + 10);
	//FINEST("_lsr: %08x\n%s", _lsr, STR(buffer));
	buffer.IgnoreAll();

	//2. Send the RR
	return SendRR(*pPeerAddress);
}

bool RTCPProtocol::SignalInputData(IOBuffer &buffer) {
	return SignalInputData(buffer, &_dummy);
}

bool RTCPProtocol::SendRR(sockaddr_in &address) {
	//1. prepare the buffer
	uint32_t ssrc;
	uint32_t seq;
	_pConnectivity->GetSSRCAndSeq(GetId(), ssrc, seq);
	//FINEST("ssrc: %08x", ssrc);
	put_htonl(_buff + 8, ssrc); //SSRC_1 (SSRC of first source)
	put_htonl(_buff + 16, seq); //extended highest sequence number received
	put_htonl(_buff + 24, _lsr); //last SR (LSR)

	//2. send it
	if (!_pConnectivity->SendRTP(address, GetId(), _buff, 32)) {
		FATAL("Unable to send RR");
		if (_pConnectivity != NULL) {
			_pConnectivity->EnqueueForDelete();
			_pConnectivity = NULL;
		}
	}

	//3. Done
	return true;
	/*
			0                   1                   2                   3
			0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
		   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	header |V=2|P|    RC   |   PT=RR=201   |             length            |0
		   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		   |                     SSRC of packet sender                     |4
		   +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
	report |                 SSRC_1 (SSRC of first source)                 |8
	block  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	  1    | fraction lost |       cumulative number of packets lost       |12
		   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		   |           extended highest sequence number received           |16
		   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		   |                      interarrival jitter                      |20
		   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		   |                         last SR (LSR)                         |24
		   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		   |                   delay since last SR (DLSR)                  |28
		   +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
	 */
}

void RTCPProtocol::SetInbboundConnectivity(InboundConnectivity *pConnectivity) {
	_pConnectivity = pConnectivity;
}
#endif /* HAS_PROTOCOL_RTP */
