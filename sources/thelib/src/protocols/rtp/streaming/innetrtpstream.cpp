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


#include "protocols/rtp/streaming/innetrtpstream.h"
#include "streaming/streamstypes.h"
#include "streaming/nalutypes.h"
#include "streaming/baseoutstream.h"
#include "protocols/baseprotocol.h"

InNetRTPStream::InNetRTPStream(BaseProtocol *pProtocol,
		StreamsManager *pStreamsManager, string name, string SPS, string PPS)
: BaseInNetStream(pProtocol, pStreamsManager, ST_IN_NET_RTP, name) {
	_counter = 0;
	_SPS.ReadFromString(SPS);
	_PPS.ReadFromString(PPS);
	_lastTs = 0;
}

InNetRTPStream::~InNetRTPStream() {
}

StreamCapabilities * InNetRTPStream::GetCapabilities() {
	NYI;
	return NULL;
}

bool InNetRTPStream::IsCompatibleWithType(uint64_t type) {
	return type == ST_OUT_NET_RTMP_4_TS;
}

void InNetRTPStream::ReadyForSend() {

}

void InNetRTPStream::SignalOutStreamAttached(BaseOutStream *pOutStream) {
	if (_lastTs == 0)
		return;
	if (!pOutStream->FeedData(GETIBPOINTER(_SPS), GETAVAILABLEBYTESCOUNT(_SPS),
			0, GETAVAILABLEBYTESCOUNT(_SPS),
			_lastTs, false)) {
		FATAL("Unable to feed stream");
		if (pOutStream->GetProtocol() != NULL) {
			pOutStream->GetProtocol()->EnqueueForDelete();
		}
	}
	if (!pOutStream->FeedData(GETIBPOINTER(_PPS), GETAVAILABLEBYTESCOUNT(_PPS),
			0, GETAVAILABLEBYTESCOUNT(_PPS),
			_lastTs, false)) {
		FATAL("Unable to feed stream");
		if (pOutStream->GetProtocol() != NULL) {
			pOutStream->GetProtocol()->EnqueueForDelete();
		}
	}
}

void InNetRTPStream::SignalOutStreamDetached(BaseOutStream *pOutStream) {
	//NYIA;
}

bool InNetRTPStream::SignalPlay(double &absoluteTimestamp, double &length) {
	NYIR;
}

bool InNetRTPStream::SignalPause() {
	NYIR;
}

bool InNetRTPStream::SignalResume() {
	NYIR;
}

bool InNetRTPStream::SignalSeek(double &absoluteTimestamp) {
	NYIR;
}

bool InNetRTPStream::SignalStop() {
	NYIR;
}

bool InNetRTPStream::FeedData(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp, bool isAudio) {
	if (_lastTs > absoluteTimestamp) {
		WARN("Back time. ATS: %.2f LTS: %.2f; D: %.2f",
				absoluteTimestamp,
				_lastTs,
				absoluteTimestamp - _lastTs);
		return true;
	}
	LinkedListNode<BaseOutStream *> *pTemp = _pOutStreams;
	if (_lastTs == 0) {
		_lastTs = absoluteTimestamp;
		while (pTemp != NULL) {
			if (!pTemp->info->IsEnqueueForDelete()) {
				SignalOutStreamAttached(pTemp->info);
			}
			pTemp = pTemp->pPrev;
		}
	}
	_lastTs = absoluteTimestamp;
	pTemp = _pOutStreams;
	while (pTemp != NULL) {
		if (!pTemp->info->IsEnqueueForDelete()) {
			if (!pTemp->info->FeedData(pData, dataLength, processedLength, totalLength,
					absoluteTimestamp, isAudio)) {
				WARN("Unable to feed OS: %p", pTemp->info);
				pTemp->info->EnqueueForDelete();
				if (GetProtocol() == pTemp->info->GetProtocol()) {
					return false;
				}
			}
		}
		pTemp = pTemp->pPrev;
	}
	return true;
}

bool InNetRTPStream::FeedVideoData(uint8_t *pData, uint32_t dataLength,
		RTPHeader &rtpHeader) {
	//1. Check the counter first
	if (_counter == 0) {
		//this is the first packet. Make sure we start with a M packet
		if (!GET_RTP_M(rtpHeader)) {
			return true;
		}
		_counter = GET_RTP_SEQ(rtpHeader);
		return true;
	} else {
		if (_counter + 1 != GET_RTP_SEQ(rtpHeader)) {
			//WARN("Missing packet");
			_currentNalu.IgnoreAll();
			_counter = 0;
			return true;
		} else {
			_counter++;
		}
	}

	//2. get the nalu

	uint8_t naluType = NALU_TYPE(pData[0]);
	if (naluType <= 23) {
		//3. Standard NALU
		return FeedData(pData, dataLength, 0, dataLength,
				(double) rtpHeader._timestamp / 90.00, false);
	} else if (naluType == NALU_TYPE_FUA) {
		if (GETAVAILABLEBYTESCOUNT(_currentNalu) == 0) {
			_currentNalu.IgnoreAll();
			//start NAL
			if ((pData[1] >> 7) == 0) {
				WARN("Bogus nalu");
				_currentNalu.IgnoreAll();
				_counter = 0;
				return true;
			}
			pData[1] = (pData[0]&0xe0) | (pData[1]&0x1f);
			_currentNalu.ReadFromBuffer(pData + 1, dataLength - 1);
			return true;
		} else {
			//middle NAL
			_currentNalu.ReadFromBuffer(pData + 2, dataLength - 2);
			if (((pData[1] >> 6)&0x01) == 1) {
				if (!FeedData(GETIBPOINTER(_currentNalu),
						GETAVAILABLEBYTESCOUNT(_currentNalu), 0,
						GETAVAILABLEBYTESCOUNT(_currentNalu),
						(double) rtpHeader._timestamp / 90.00, false)) {
					FATAL("Unable to feed NALU");
					return false;
				}
				_currentNalu.IgnoreAll();
			}
			return true;
		}
	} else if (naluType == NALU_TYPE_STAPA) {
		uint32_t index = 1;
		double ts = (double) rtpHeader._timestamp / 90.00;
		while (index + 3 < dataLength) {
			uint16_t length = ntohsp(pData + index);
			index += 2;
			if (index + length > dataLength) {
				WARN("Bogus STAP-A");
				_currentNalu.IgnoreAll();
				_counter = 0;
				return true;
			}
			if (!FeedData(pData + index,
					length, 0,
					length,
					ts, false)) {
				FATAL("Unable to feed NALU");
				return false;
			}
			index += length;
		}
		return true;
	} else {
		WARN("invalid NAL: %s", STR(NALUToString(naluType)));
		_currentNalu.IgnoreAll();
		_counter = 0;
		return true;
	}
}
