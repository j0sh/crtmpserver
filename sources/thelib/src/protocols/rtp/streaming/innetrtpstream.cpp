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
		StreamsManager *pStreamsManager, string name, string SPS, string PPS,
		string AAC, uint32_t bandwidthHint, uint8_t rtcpDetectionInterval)
: BaseInNetStream(pProtocol, pStreamsManager, ST_IN_NET_RTP, name) {
	_hasAudio = false;
	if (AAC.length() != 0) {
		_capabilities.InitAudioAAC(
				(uint8_t *) STR(AAC),
				AAC.length());
		_hasAudio = true;
	}

	_hasVideo = false;
	if ((SPS.length() != 0) && (PPS.length() != 0)) {
		_capabilities.InitVideoH264(
				(uint8_t *) STR(SPS),
				SPS.length(),
				(uint8_t *) STR(PPS),
				PPS.length());
		_hasVideo = true;
	}

	_capabilities.bandwidthHint = bandwidthHint;

	_audioSequence = 0;
	_audioPacketsCount = 0;
	_audioDroppedPacketsCount = 0;
	_audioBytesCount = 0;
	_audioNTP = 0;
	_audioRTP = 0;
	_audioLastTs = 0;
	_audioRTPRollCount = 0;
	_audioLastRTP = 0;
	_audioFirstTimestamp = -1;
	_lastAudioRTCPRTP = 0;
	_audioRTCPRTPRollCount = 0;

	_videoSequence = 0;
	_videoPacketsCount = 0;
	_videoDroppedPacketsCount = 0;
	_videoBytesCount = 0;
	_videoNTP = 0;
	_videoRTP = 0;
	_videoLastTs = 0;
	_videoRTPRollCount = 0;
	_videoLastRTP = 0;
	_videoFirstTimestamp = -1;
	_lastVideoRTCPRTP = 0;
	_videoRTCPRTPRollCount = 0;

	_rtcpPresence = RTCP_PRESENCE_UNKNOWN;
	_rtcpDetectionInterval = rtcpDetectionInterval;
	_rtcpDetectionStart = 0;

	_avCodecsSent=false;
}

InNetRTPStream::~InNetRTPStream() {
}

StreamCapabilities * InNetRTPStream::GetCapabilities() {
	return &_capabilities;
}

bool InNetRTPStream::IsCompatibleWithType(uint64_t type) {
	return (type == ST_OUT_NET_RTMP_4_TS)
			|| (type == ST_OUT_FILE_HLS)
			|| (type == ST_OUT_NET_RTP);
}

void InNetRTPStream::ReadyForSend() {

}

void InNetRTPStream::SignalOutStreamAttached(BaseOutStream *pOutStream) {
	if (_hasVideo && _hasAudio) {
		if ((_videoLastTs != 0) && (_audioLastTs != 0)) {
			if (_videoLastTs < _audioLastTs) {
				FeedVideoCodecSetup(pOutStream);
				FeedAudioCodecSetup(pOutStream);
				_avCodecsSent=true;
			}
		}
	} else {
		if (_videoLastTs != 0) {
			FeedVideoCodecSetup(pOutStream);
			_avCodecsSent=true;
		}
		if (_audioLastTs != 0) {
			FeedAudioCodecSetup(pOutStream);
			_avCodecsSent=true;
		}
	}

#ifdef HAS_PROTOCOL_RTMP
	if (TAG_KIND_OF(pOutStream->GetType(), ST_OUT_NET_RTMP)) {
		((BaseRTMPProtocol *) pOutStream->GetProtocol())->TrySetOutboundChunkSize(4 * 1024 * 1024);
		((BaseOutNetRTMPStream *) pOutStream)->SetFeederChunkSize(4 * 1024 * 1024);
		((BaseOutNetRTMPStream *) pOutStream)->CanDropFrames(true);
	}
#endif /* HAS_PROTOCOL_RTMP */
}

void InNetRTPStream::SignalOutStreamDetached(BaseOutStream *pOutStream) {
}

bool InNetRTPStream::SignalPlay(double &absoluteTimestamp, double &length) {
	return true;
}

bool InNetRTPStream::SignalPause() {
	return true;
}

bool InNetRTPStream::SignalResume() {
	return true;
}

bool InNetRTPStream::SignalSeek(double &absoluteTimestamp) {
	return true;
}

bool InNetRTPStream::SignalStop() {
	return true;
}

//#define DEBUG_RTCP_PRESENCE(...) FINEST(__VA_ARGS__)
#define DEBUG_RTCP_PRESENCE(...)

bool InNetRTPStream::FeedData(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp, bool isAudio) {
	switch (_rtcpPresence) {
		case RTCP_PRESENCE_UNKNOWN:
		{
			DEBUG_RTCP_PRESENCE("RTCP_PRESENCE_UNKNOWN: %"PRIz"u", (time(NULL) - _rtcpDetectionStart));
			if (_rtcpDetectionInterval == 0) {
				WARN("RTCP disabled on stream %s(%"PRIu32") with name %s. A/V drifting may occur over long periods of time",
						STR(tagToString(GetType())), GetUniqueId(), STR(GetName()));
				_rtcpPresence = RTCP_PRESENCE_ABSENT;
				return true;
			}
			if (_rtcpDetectionStart == 0) {
				_rtcpDetectionStart = time(NULL);
				return true;
			}
			if ((time(NULL) - _rtcpDetectionStart) > _rtcpDetectionInterval) {
				WARN("Stream %s(%"PRIu32") with name %s doesn't have RTCP. A/V drifting may occur over long periods of time",
						STR(tagToString(GetType())), GetUniqueId(), STR(GetName()));
				_rtcpPresence = RTCP_PRESENCE_ABSENT;
				return true;
			}
			bool audioRTCPPresent = false;
			bool videoRTCPPresent = false;
			if (_hasAudio) {
				if (_audioNTP != 0)
					DEBUG_RTCP_PRESENCE("Audio RTCP detected");
				audioRTCPPresent = (_audioNTP != 0);
			} else {
				audioRTCPPresent = true;
			}
			if (_hasVideo) {
				if (_videoNTP != 0)
					DEBUG_RTCP_PRESENCE("Video RTCP detected");
				videoRTCPPresent = (_videoNTP != 0);
			} else {
				videoRTCPPresent = true;
			}
			if (audioRTCPPresent && videoRTCPPresent) {
				DEBUG_RTCP_PRESENCE("RTCP available on stream %s(%"PRIu32") with name %s.",
						STR(tagToString(GetType())), GetUniqueId(), STR(GetName()));
				_rtcpPresence = RTCP_PRESENCE_AVAILABLE;
			}
			return true;
			break;
		}
		case RTCP_PRESENCE_AVAILABLE:
		{
			DEBUG_RTCP_PRESENCE("RTCP_PRESENCE_AVAILABLE");
			double &rtp = isAudio ? _audioRTP : _videoRTP;
			double &ntp = isAudio ? _audioNTP : _videoNTP;
			absoluteTimestamp = ntp + absoluteTimestamp - rtp;
			break;
		}
		case RTCP_PRESENCE_ABSENT:
		{
			DEBUG_RTCP_PRESENCE("RTCP_PRESENCE_ABSENT");
			double &firstTimestamp = isAudio ? _audioFirstTimestamp : _videoFirstTimestamp;
			if (firstTimestamp < 0)
				firstTimestamp = absoluteTimestamp;
			absoluteTimestamp -= firstTimestamp;
			break;
		}
		default:
		{
			ASSERT("Invalid _rtcpPresence: %"PRIu8, _rtcpPresence);
			return false;
		}
	}

	double &lastTs = isAudio ? _audioLastTs : _videoLastTs;

	if ((-1.0 < (lastTs * 100.00 - absoluteTimestamp * 100.00))
			&& ((lastTs * 100.00 - absoluteTimestamp * 100.00) < 1.00)) {
		absoluteTimestamp = lastTs;
	}

	if (lastTs * 100.00 > absoluteTimestamp * 100.00) {
		WARN("Back time on %s. ATS: %.08f LTS: %.08f; D: %.8f; isAudio: %"PRIu8,
				STR(GetName()),
				absoluteTimestamp,
				lastTs,
				absoluteTimestamp - lastTs,
				isAudio);
		return true;
	}
	LinkedListNode<BaseOutStream *> *pTemp = _pOutStreams;
	lastTs = absoluteTimestamp;
	if (!_avCodecsSent) {
		while (pTemp != NULL) {
			if (!pTemp->info->IsEnqueueForDelete()) {
				SignalOutStreamAttached(pTemp->info);
			}
			pTemp = pTemp->pPrev;
		}
		if (!_avCodecsSent) {
			return true;
		}
	}
	if (_hasAudio && _hasVideo) {
		if ((_audioLastTs == 0) || (_videoLastTs == 0)) {
			return true;
		}
	}
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
	if (_videoSequence == 0) {
		//this is the first packet. Make sure we start with a M packet
		if (!GET_RTP_M(rtpHeader)) {
			return true;
		}
		_videoSequence = GET_RTP_SEQ(rtpHeader);
		return true;
	} else {
		if ((uint16_t) (_videoSequence + 1) != (uint16_t) GET_RTP_SEQ(rtpHeader)) {
			WARN("Missing video packet. Wanted: %"PRIu16"; got: %"PRIu16" on stream: %s",
					(uint16_t) (_videoSequence + 1),
					(uint16_t) GET_RTP_SEQ(rtpHeader),
					STR(GetName()));
			_currentNalu.IgnoreAll();
			_videoDroppedPacketsCount++;
			_videoSequence = 0;
			return true;
		} else {
			_videoSequence++;
		}
	}

	//2. get the nalu
	uint64_t rtpTs = ComputeRTP(rtpHeader._timestamp, _videoLastRTP, _videoRTPRollCount);
	double ts = (double) rtpTs / (double) _capabilities.avc._rate * 1000.0;
	uint8_t naluType = NALU_TYPE(pData[0]);
	if (naluType <= 23) {
		//3. Standard NALU
		//FINEST("V: %08"PRIx32, rtpHeader._timestamp);
		_videoPacketsCount++;
		_videoBytesCount += dataLength;
		return FeedData(pData, dataLength, 0, dataLength, ts, false);
	} else if (naluType == NALU_TYPE_FUA) {
		if (GETAVAILABLEBYTESCOUNT(_currentNalu) == 0) {
			_currentNalu.IgnoreAll();
			//start NAL
			if ((pData[1] >> 7) == 0) {
				WARN("Bogus nalu");
				_currentNalu.IgnoreAll();
				_videoSequence = 0;
				return true;
			}
			pData[1] = (pData[0]&0xe0) | (pData[1]&0x1f);
			_currentNalu.ReadFromBuffer(pData + 1, dataLength - 1);
			return true;
		} else {
			//middle NAL
			_currentNalu.ReadFromBuffer(pData + 2, dataLength - 2);
			if (((pData[1] >> 6)&0x01) == 1) {
				//FINEST("V: %08"PRIx32, rtpHeader._timestamp);
				_videoPacketsCount++;
				_videoBytesCount += GETAVAILABLEBYTESCOUNT(_currentNalu);
				if (!FeedData(GETIBPOINTER(_currentNalu),
						GETAVAILABLEBYTESCOUNT(_currentNalu),
						0,
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
		while (index + 3 < dataLength) {
			uint16_t length = ENTOHSP(pData + index);
			index += 2;
			if (index + length > dataLength) {
				WARN("Bogus STAP-A");
				_currentNalu.IgnoreAll();
				_videoSequence = 0;
				return true;
			}
			_videoPacketsCount++;
			_videoBytesCount += length;
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
		_videoSequence = 0;
		return true;
	}
}

bool InNetRTPStream::FeedAudioData(uint8_t *pData, uint32_t dataLength,
		RTPHeader &rtpHeader) {
	if (_audioSequence == 0) {
		//this is the first packet. Make sure we start with a M packet
		if (!GET_RTP_M(rtpHeader)) {
			return true;
		}
		_audioSequence = GET_RTP_SEQ(rtpHeader);
		return true;
	} else {
		if ((uint16_t) (_audioSequence + 1) != (uint16_t) GET_RTP_SEQ(rtpHeader)) {
			WARN("Missing audio packet. Wanted: %"PRIu16"; got: %"PRIu16" on stream: %s",
					(uint16_t) (_audioSequence + 1),
					(uint16_t) GET_RTP_SEQ(rtpHeader),
					STR(GetName()));
			_audioDroppedPacketsCount++;
			_audioSequence = 0;
			return true;
		} else {
			_audioSequence++;
		}
	}

	//1. Compute chunks count
	uint16_t chunksCount = ENTOHSP(pData);
	if ((chunksCount % 16) != 0) {
		FATAL("Invalid AU headers length: %"PRIx16, chunksCount);
		return false;
	}
	chunksCount = chunksCount / 16;

	//3. Feed the buffer chunk by chunk
	uint32_t cursor = 2 + 2 * chunksCount;
	uint16_t chunkSize = 0;
	double ts = 0;
	uint64_t rtpTs = ComputeRTP(rtpHeader._timestamp, _audioLastRTP, _audioRTPRollCount);
	for (uint32_t i = 0; i < chunksCount; i++) {
		if (i != (uint32_t) (chunksCount - 1)) {
			chunkSize = (ENTOHSP(pData + 2 + 2 * i)) >> 3;
		} else {
			chunkSize = (uint16_t) (dataLength - cursor);
		}
		ts = (double) (rtpTs + i * 1024) / (double) _capabilities.aac._sampleRate * 1000.00;
		if ((cursor + chunkSize) > dataLength) {
			FATAL("Unable to feed data: cursor: %"PRIu32"; chunkSize: %"PRIu16"; dataLength: %"PRIu32"; chunksCount: %"PRIu16,
					cursor, chunkSize, dataLength, chunksCount);
			return false;
		}
		_audioPacketsCount++;
		_audioBytesCount += chunkSize;
		if (!FeedData(pData + cursor - 2,
				chunkSize + 2,
				0,
				chunkSize + 2,
				ts, true)) {
			FATAL("Unable to feed data");
			return false;
		}
		cursor += chunkSize;

	}

	return true;
}

void InNetRTPStream::GetStats(Variant &info, uint32_t namespaceId) {
	BaseInNetStream::GetStats(info, namespaceId);
	info["audio"]["bytesCount"] = _audioBytesCount;
	info["audio"]["packetsCount"] = _audioPacketsCount;
	info["audio"]["droppedPacketsCount"] = _audioDroppedPacketsCount;
	info["video"]["bytesCount"] = _videoBytesCount;
	info["video"]["packetsCount"] = _videoPacketsCount;
	info["video"]["droppedPacketsCount"] = _videoDroppedPacketsCount;
}

void InNetRTPStream::ReportSR(uint64_t ntpMicroseconds, uint32_t rtpTimestamp,
		bool isAudio) {
	if (isAudio) {
		_audioRTP = (double) ComputeRTP(rtpTimestamp, _lastAudioRTCPRTP,
				_audioRTCPRTPRollCount) / (double) _capabilities.aac._sampleRate * 1000.0;
		_audioNTP = (double) ntpMicroseconds / 1000.0;
	} else {
		_videoRTP = (double) ComputeRTP(rtpTimestamp, _lastVideoRTCPRTP,
				_videoRTCPRTPRollCount) / (double) _capabilities.avc._rate * 1000.0;
		_videoNTP = (double) ntpMicroseconds / 1000.0;
	}
}

void InNetRTPStream::FeedVideoCodecSetup(BaseOutStream* pOutStream) {
	if (!pOutStream->FeedData(
			_capabilities.avc._pSPS,
			_capabilities.avc._spsLength,
			0,
			_capabilities.avc._spsLength,
			_videoLastTs,
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
			_videoLastTs,
			false)) {
		FATAL("Unable to feed stream");
		if (pOutStream->GetProtocol() != NULL) {
			pOutStream->GetProtocol()->EnqueueForDelete();
		}
	}
}

void InNetRTPStream::FeedAudioCodecSetup(BaseOutStream* pOutStream) {
	uint8_t *pTemp = new uint8_t[_capabilities.aac._aacLength + 2];
	memcpy(pTemp + 2, _capabilities.aac._pAAC, _capabilities.aac._aacLength);
	if (!pOutStream->FeedData(
			pTemp + 2,
			_capabilities.aac._aacLength,
			0,
			_capabilities.aac._aacLength,
			_audioLastTs,
			true)) {
		FATAL("Unable to feed stream");
		if (pOutStream->GetProtocol() != NULL) {
			pOutStream->GetProtocol()->EnqueueForDelete();
		}
	}
	delete[] pTemp;
}

uint64_t InNetRTPStream::ComputeRTP(uint32_t &currentRtp, uint32_t &lastRtp,
		uint32_t &rtpRollCount) {
	if (lastRtp > currentRtp) {
		if (((lastRtp >> 31) == 0x01) && ((currentRtp >> 31) == 0x00)) {
			FINEST("RollOver");
			rtpRollCount++;
		}
	}
	lastRtp = currentRtp;
	return (((uint64_t) rtpRollCount) << 32) | currentRtp;
}

#endif /* HAS_PROTOCOL_RTP */
