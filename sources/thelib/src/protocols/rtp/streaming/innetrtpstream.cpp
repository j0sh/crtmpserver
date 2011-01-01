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
#include "protocols/rtp/streaming/innetrtpstream.h"
#include "streaming/streamstypes.h"
#include "streaming/nalutypes.h"
#include "streaming/baseoutstream.h"
#include "protocols/baseprotocol.h"
#include "protocols/rtmp/basertmpprotocol.h"
#include "protocols/rtmp/streaming/baseoutnetrtmpstream.h"

InNetRTPStream::InNetRTPStream(BaseProtocol *pProtocol,
		StreamsManager *pStreamsManager, string name, string SPS, string PPS, string AAC)
: BaseInNetStream(pProtocol, pStreamsManager, ST_IN_NET_RTP, name) {
	_counter = 0;
	_lastVideoTs = 0;
	_lastAudioTs = 0;

	if (AAC.length() != 0) {
		_capabilities.InitAudioAAC(
				(uint8_t *) STR(AAC),
				AAC.length());
	}

	if ((SPS.length() != 0) && (PPS.length() != 0)) {
		_capabilities.InitVideoH264(
				(uint8_t *) STR(SPS),
				SPS.length(),
				(uint8_t *) STR(PPS),
				PPS.length());
	}
}

InNetRTPStream::~InNetRTPStream() {
}

StreamCapabilities * InNetRTPStream::GetCapabilities() {
	return &_capabilities;
}

bool InNetRTPStream::IsCompatibleWithType(uint64_t type) {
	return type == ST_OUT_NET_RTMP_4_TS;
}

void InNetRTPStream::ReadyForSend() {

}

void InNetRTPStream::SignalOutStreamAttached(BaseOutStream *pOutStream) {
	if (TAG_KIND_OF(pOutStream->GetType(), ST_OUT_NET_RTMP)) {
		((BaseOutNetRTMPStream *) pOutStream)->CanDropFrames(true);
	}
	if (_lastVideoTs != 0) {
		if (!pOutStream->FeedData(
				_capabilities.avc._pSPS,
				_capabilities.avc._spsLength,
				0,
				_capabilities.avc._spsLength,
				_lastVideoTs,
				false)) {
			FATAL("Unable to feed stream");
			if (pOutStream->GetProtocol() != NULL) {
				pOutStream->GetProtocol()->EnqueueForDelete();
			}
		}
		if (!pOutStream->FeedData(
				_capabilities.avc._pPPS,
				_capabilities.avc._ppsLength,
				0,
				_capabilities.avc._ppsLength,
				_lastVideoTs,
				false)) {
			FATAL("Unable to feed stream");
			if (pOutStream->GetProtocol() != NULL) {
				pOutStream->GetProtocol()->EnqueueForDelete();
			}
		}
	}
	if (_lastAudioTs != 0) {
		uint8_t *pTemp = new uint8_t[_capabilities.aac._aacLength + 2];
		memcpy(pTemp + 2, _capabilities.aac._pAAC, _capabilities.aac._aacLength);
		if (!pOutStream->FeedData(
				pTemp + 2,
				_capabilities.aac._aacLength,
				0,
				_capabilities.aac._aacLength,
				_lastAudioTs,
				true)) {
			FATAL("Unable to feed stream");
			if (pOutStream->GetProtocol() != NULL) {
				pOutStream->GetProtocol()->EnqueueForDelete();
			}
		}
		delete[] pTemp;
	}
#ifdef HAS_PROTOCOL_RTMP
	if (TAG_KIND_OF(pOutStream->GetType(), ST_OUT_NET_RTMP)) {
		((BaseRTMPProtocol *) pOutStream->GetProtocol())->TrySetOutboundChunkSize(4 * 1024 * 1024);
		((BaseOutNetRTMPStream *) pOutStream)->CanDropFrames(true);
	}
#endif /* HAS_PROTOCOL_RTMP */
}

void InNetRTPStream::SignalOutStreamDetached(BaseOutStream *pOutStream) {
	//NYIA;
}

bool InNetRTPStream::SignalPlay(double &absoluteTimestamp, double &length) {
	return true;
}

bool InNetRTPStream::SignalPause() {
	FATAL("Pause is not supported on inbound RTSP streams");
	return false;
	//return true;
}

bool InNetRTPStream::SignalResume() {
	FATAL("Resume is not supported on inbound RTSP streams");
	return false;
}

bool InNetRTPStream::SignalSeek(double &absoluteTimestamp) {
	FATAL("Seek is not supported on inbound RTSP streams");
	return false;
}

bool InNetRTPStream::SignalStop() {
	return true;
}

bool InNetRTPStream::FeedData(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp, bool isAudio) {
	double &lastTs = isAudio ? _lastAudioTs : _lastVideoTs;

	if ((-1.0 < (lastTs * 100.00 - absoluteTimestamp * 100.00))
			&& ((lastTs * 100.00 - absoluteTimestamp * 100.00) < 1.00)) {
		absoluteTimestamp = lastTs;
	}

	if (lastTs * 100.00 > absoluteTimestamp * 100.00) {
		WARN("Back time on %s. ATS: %.08f LTS: %.08f; D: %.8f; isAudio: %d",
				STR(GetName()),
				absoluteTimestamp,
				lastTs,
				absoluteTimestamp - lastTs,
				isAudio);
		return true;
	}
	LinkedListNode<BaseOutStream *> *pTemp = _pOutStreams;
	if (lastTs == 0) {
		lastTs = absoluteTimestamp;
		while (pTemp != NULL) {
			if (!pTemp->info->IsEnqueueForDelete()) {
				SignalOutStreamAttached(pTemp->info);
			}
			pTemp = pTemp->pPrev;
		}
	}
	lastTs = absoluteTimestamp;
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
	double ts = (double) rtpHeader._timestamp / (double) _capabilities.avc._rate * 1000.0;
	uint8_t naluType = NALU_TYPE(pData[0]);
	if (naluType <= 23) {
		//3. Standard NALU
		//FINEST("Standard NALU");
		return FeedData(pData, dataLength, 0, dataLength, ts, false);
	} else if (naluType == NALU_TYPE_FUA) {
		if (GETAVAILABLEBYTESCOUNT(_currentNalu) == 0) {
			//			FINEST("--------------");
			//			FINEST("First NALU_TYPE_FUA");
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
			//			FINEST("NALU_TYPE_FUA: %s; s: %d",
			//					STR(NALUToString(pData[1])),
			//					GETAVAILABLEBYTESCOUNT(_currentNalu));
			return true;
		} else {
			//middle NAL
			_currentNalu.ReadFromBuffer(pData + 2, dataLength - 2);
			//			FINEST("MIDDLE NALU_TYPE_FUA: end: %d; s: %d",
			//					((pData[1] >> 6)&0x01),
			//					GETAVAILABLEBYTESCOUNT(_currentNalu));
			if (((pData[1] >> 6)&0x01) == 1) {
				//				FINEST("NALU_TYPE_FUA feed; s: %d",
				//						GETAVAILABLEBYTESCOUNT(_currentNalu));
				//				FINEST("--------------");
				if (!FeedData(GETIBPOINTER(_currentNalu),
						GETAVAILABLEBYTESCOUNT(_currentNalu), 0,
						GETAVAILABLEBYTESCOUNT(_currentNalu),
						ts,
						false)) {
					FATAL("Unable to feed NALU");
					return false;
				}
				_currentNalu.IgnoreAll();
			}
			return true;
		}
	} else if (naluType == NALU_TYPE_STAPA) {
		uint32_t index = 1;
		//FINEST("ts: %.2f; delta: %.2f", ts, ts - _lastTs);
		while (index + 3 < dataLength) {
			uint16_t length = ENTOHSP(pData + index);
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

//double ____last = 0;

bool InNetRTPStream::FeedAudioData(uint8_t *pData, uint32_t dataLength,
		RTPHeader &rtpHeader) {
	//1. Compute chunks count
	uint16_t chunksCount = ENTOHSP(pData);
	if ((chunksCount % 16) != 0) {
		FATAL("Invalid AU headers length: %04x", chunksCount);
		return false;
	}
	chunksCount = chunksCount / 16;

	//3. Feed the buffer chunk by chunk
	uint32_t cursor = 2 + 2 * chunksCount;
	//	string msg = "";
	//	for (uint32_t i = 0; i < cursor; i++) {
	//		msg += format("%02x ", pData[i]);
	//	}
	//	FINEST("%s", STR(msg));
	uint16_t chunkSize = 0;
	double ts = 0;
	for (uint32_t i = 0; i < chunksCount; i++) {
		if (i != (uint32_t) (chunksCount - 1)) {
			chunkSize = (ENTOHSP(pData + 2 + 2 * i)) >> 3;
		} else {
			chunkSize = dataLength - cursor;
		}
		ts = (double) (rtpHeader._timestamp + i * 1024) / (double) _capabilities.aac._sampleRate * 1000.00;
		if ((cursor + chunkSize) > dataLength) {
			FATAL("Unable to feed data: cursor: %d; chunkSize: %d; dataLength: %d; chunksCount: %d",
					cursor, chunkSize, dataLength, chunksCount);
			return false;
		}
		if (!FeedData(pData + cursor,
				chunkSize,
				0,
				chunkSize,
				ts, true)) {
			FATAL("Unable to feed data");
			return false;
		}
		cursor += chunkSize;

	}

	return true;
}
#endif /* HAS_PROTOCOL_RTP */
