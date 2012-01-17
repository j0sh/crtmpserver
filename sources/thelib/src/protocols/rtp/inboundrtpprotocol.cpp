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
#include "protocols/rtp/inboundrtpprotocol.h"
#include "protocols/rtp/rtspprotocol.h"
#include "protocols/rtp/streaming/innetrtpstream.h"
#include "protocols/rtp/connectivity/inboundconnectivity.h"

InboundRTPProtocol::InboundRTPProtocol()
: BaseProtocol(PT_INBOUND_RTP) {
	_spsPpsPeriod = 0;
	_pInStream = NULL;
	_pConnectivity = NULL;
	memset(&_rtpHeader, 0, sizeof (_rtpHeader));
	_lastSeq = 0;
	_seqRollOver = 0;
	_isAudio = false;
	_packetsCount = 0;
#ifdef RTP_DETECT_ROLLOVER
	_lastTimestamp = 0;
	_timestampRollover = 0;
#endif
}

InboundRTPProtocol::~InboundRTPProtocol() {
}

bool InboundRTPProtocol::Initialize(Variant &parameters) {
	GetCustomParameters() = parameters;
	return true;
}

bool InboundRTPProtocol::AllowFarProtocol(uint64_t type) {
	return type == PT_RTSP
			|| type == PT_UDP;
}

bool InboundRTPProtocol::AllowNearProtocol(uint64_t type) {
	return false;
}

bool InboundRTPProtocol::SignalInputData(int32_t recvAmount) {
	NYIR;
}

bool InboundRTPProtocol::SignalInputData(IOBuffer &buffer) {
	NYIR;
}

bool InboundRTPProtocol::SignalInputData(IOBuffer &buffer,
		sockaddr_in *pPeerAddress) {
	//1. Get the raw buffer and its length
	uint8_t *pBuffer = GETIBPOINTER(buffer);
	uint32_t length = GETAVAILABLEBYTESCOUNT(buffer);

	//2. Do we have enough data?
	if (length < 12) {
		buffer.IgnoreAll();
		return true;
	}

	//3. Get the RTP header parts that we are interested in
	_rtpHeader._flags = ENTOHLP(pBuffer);
	_rtpHeader._timestamp = ENTOHLP(pBuffer + 4);
	_rtpHeader._ssrc = ENTOHLP(pBuffer + 8);

	//4. Advance the sequence roll-over counter if necessary
	if (GET_RTP_SEQ(_rtpHeader) < _lastSeq) {
		if ((_lastSeq - GET_RTP_SEQ(_rtpHeader)) > (0xffff >> 2)) {
			_seqRollOver++;
			_lastSeq = GET_RTP_SEQ(_rtpHeader);
		} else {
			buffer.IgnoreAll();
			return true;
		}
	} else {
		_lastSeq = GET_RTP_SEQ(_rtpHeader);
	}

	//5. Do we have enough data?
	if (length < ((uint32_t) 12 + GET_RTP_CC(_rtpHeader)*4 + 1)) {
		buffer.IgnoreAll();
		return true;
	}

	//6. Skip the RTP header
	pBuffer += 12 + GET_RTP_CC(_rtpHeader)*4;
	length -= 12 + GET_RTP_CC(_rtpHeader)*4;
	if (GET_RTP_P(_rtpHeader))
		length -= pBuffer[length - 1];

	//7. Detect rollover and adjust the timestamp
#ifdef RTP_DETECT_ROLLOVER
	if (_rtpHeader._timestamp < _lastTimestamp) {
		if ((((_rtpHeader._timestamp & 0x80000000) >> 31) == 0)
				&& (((_lastTimestamp & 0x80000000) >> 31) == 1)) {
			_timestampRollover++;
			_lastTimestamp = _rtpHeader._timestamp;
		}
	} else {
		_lastTimestamp = _rtpHeader._timestamp;
	}
	_rtpHeader._timestamp = (_timestampRollover << 32) | _rtpHeader._timestamp;
#endif

	//5. Feed the data to the stream
	if (_pInStream != NULL) {
		if (_isAudio) {
			if (!_pInStream->FeedAudioData(pBuffer, length, _rtpHeader)) {
				FATAL("Unable to stream data");
				if (_pConnectivity != NULL) {
					_pConnectivity->EnqueueForDelete();
					_pConnectivity = NULL;
				}
				return false;
			}
		} else {
			if (!_pInStream->FeedVideoData(pBuffer, length, _rtpHeader)) {
				FATAL("Unable to stream data");
				if (_pConnectivity != NULL) {
					_pConnectivity->EnqueueForDelete();
					_pConnectivity = NULL;
				}
				return false;
			}
		}
	}

	//6. Ignore the data
	buffer.IgnoreAll();

	//7. Increment the packets count
	_packetsCount++;

	//8. Send the RR if necesary
	if ((_packetsCount % 300) == 0) {

		if (_pConnectivity != NULL) {
			if (!_pConnectivity->SendRR(_isAudio)) {
				FATAL("Unable to send RR");
				_pConnectivity->EnqueueForDelete();
				_pConnectivity = NULL;
				return false;
			}
		}
	}

	//7. Done
	return true;
}

uint32_t InboundRTPProtocol::GetSSRC() {
	return _rtpHeader._ssrc;
}

uint32_t InboundRTPProtocol::GetExtendedSeq() {
	return (((uint32_t) _seqRollOver) << 16) | _lastSeq;
}

void InboundRTPProtocol::SetStream(InNetRTPStream *pInStream, bool isAudio) {
	_pInStream = pInStream;
	_isAudio = isAudio;
}

void InboundRTPProtocol::SetInbboundConnectivity(InboundConnectivity *pConnectivity) {
	_pConnectivity = pConnectivity;
}
#endif /* HAS_PROTOCOL_RTP */
