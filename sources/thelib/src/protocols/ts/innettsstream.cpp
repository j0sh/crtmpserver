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


#ifdef HAS_PROTOCOL_TS
#include "protocols/ts/innettsstream.h"
#include "streaming/streamstypes.h"
#include "protocols/ts/inboundtsprotocol.h"
#include "streaming/baseoutstream.h"
#include "streaming/nalutypes.h"

InNetTSStream::InNetTSStream(BaseProtocol *pProtocol,
		StreamsManager *pStreamsManager, string name, uint32_t bandwidthHint)
: BaseInNetStream(pProtocol, pStreamsManager, ST_IN_NET_TS, name) {
	_streamCapabilities.bandwidthHint = bandwidthHint;
	//audio section
	_pAudioPidDescriptor = NULL;
	_currentAudioSequenceNumber = -1;
	_lastRawPtsAudio = 0;
	_audioRollOverCount = 0;
	_ptsTimeAudio = 0;
#ifdef COMPUTE_DTS_TIME
	_dtsTimeAudio = 0;
#endif
	_deltaTimeAudio = -1;
	_lastGotAudioTimestamp = 0;
	_lastSentAudioTimestamp = 0;
	_audioPacketsCount = 0;
	_statsAudioPacketsCount = 0;
	_audioBytesCount = 0;
	_audioDroppedPacketsCount = 0;
	_audioDroppedBytesCount = 0;

	//video section
	_pVideoPidDescriptor = NULL;
	_currentVideoSequenceNumber = -1;
	_lastRawPtsVideo = 0;
	_videoRollOverCount = 0;
	_ptsTimeVideo = 0;
#ifdef COMPUTE_DTS_TIME
	_dtsTimeVideo = 0;
#endif
	_deltaTimeVideo = -1;

	_feedTime = 0;
	_cursor = 0;
	_videoPacketsCount = 0;
	_videoBytesCount = 0;
	_videoDroppedPacketsCount = 0;
	_videoDroppedBytesCount = 0;

	_firstNAL = true;
}

InNetTSStream::~InNetTSStream() {
	if (_pVideoPidDescriptor != NULL) {
		_pVideoPidDescriptor->payload.pStream = NULL;
	}
	if (_pAudioPidDescriptor != NULL) {
		_pAudioPidDescriptor->payload.pStream = NULL;
	}
}

StreamCapabilities * InNetTSStream::GetCapabilities() {
	return &_streamCapabilities;
}

void InNetTSStream::SetAudioVideoPidDescriptors(PIDDescriptor *pAudioPidDescriptor,
		PIDDescriptor *pVideoPidDescriptor) {
	_pAudioPidDescriptor = pAudioPidDescriptor;
	_pVideoPidDescriptor = pVideoPidDescriptor;
}

double InNetTSStream::GetFeedTime() {
	return _feedTime;
}

//#define __FORCE_ROLL_OVER_FOR_DEBUG 30
//#define __DUMP_TIMESTAMP_INFO_FOR_DEBUG
//#define __FORCE_DROPPING_PACKETS 200

#ifdef __FORCE_ROLL_OVER_FOR_DEBUG
uint64_t __aRoll = 0;
uint64_t __vRoll = 0;
#endif /* __FORCE_ROLL_OVER_FOR_DEBUG */

#define DROP_PACKET \
do { \
	if(!isAudio) { \
		/*WARN("Video packet dropped!!!");*/ \
		_currentNal.IgnoreAll(); \
		_cursor = 0; \
		_firstNAL = true; \
	} \
	uint64_t &droppedPacketsCount = isAudio ? _audioDroppedPacketsCount : _videoDroppedPacketsCount; \
	droppedPacketsCount++; \
} while (0)

bool InNetTSStream::FeedData(uint8_t *pData, uint32_t length, bool packetStart,
		bool isAudio, int8_t sequenceNumber) {
	double &ptsTime = isAudio ? _ptsTimeAudio : _ptsTimeVideo;
	uint64_t &lastRawPts = isAudio ? _lastRawPtsAudio : _lastRawPtsVideo;
	uint32_t &rollOverCount = isAudio ? _audioRollOverCount : _videoRollOverCount;
	int8_t &currentSequenceNumber = isAudio ? _currentAudioSequenceNumber : _currentVideoSequenceNumber;

#ifdef __FORCE_DROPPING_PACKETS
	if (!isAudio) {
		if ((rand() % __FORCE_DROPPING_PACKETS) == 0) {
			WARN("Dropping a video packet on the floor just for fun");
			currentSequenceNumber = 0;
		}
	}
#endif /* __FORCE_DROPPING_PACKETS */

	if (currentSequenceNumber == -1) {
		currentSequenceNumber = sequenceNumber;
	} else {
		if (((currentSequenceNumber + 1)&0x0f) != sequenceNumber) {
			//WARN("Missing %c packet", isAudio ? 'A' : 'V');
			currentSequenceNumber = sequenceNumber;
			DROP_PACKET;
			return true;
		} else {
			currentSequenceNumber = sequenceNumber;
		}
	}
#ifdef COMPUTE_DTS_TIME
	double &dtsTime = isAudio ? _dtsTimeAudio : _dtsTimeVideo;
#endif
	double &deltaTime = isAudio ? _deltaTimeAudio : _deltaTimeVideo;
	double absoluteTime = 0;
	if (packetStart) {
		if (length >= 8) {
			uint32_t pesHeaderLength = pData[8];
			if (pesHeaderLength + 9 > length) {
				WARN("Not enough data");
				DROP_PACKET;
				return true;
			}

			//compute the time:
			//http://dvd.sourceforge.net/dvdinfo/pes-hdr.html
			uint8_t *pPTS = NULL;
#ifdef COMPUTE_DTS_TIME
			uint8_t *pDTS = NULL;
#endif

			uint8_t ptsdstflags = pData[7] >> 6;

			if (ptsdstflags == 2) {
				pPTS = pData + 9;
			} else if (ptsdstflags == 3) {
				pPTS = pData + 9;
#ifdef COMPUTE_DTS_TIME
				pDTS = pData + 14;
#endif
			}

			if (pPTS != NULL) {
#ifdef __FORCE_ROLL_OVER_FOR_DEBUG
				uint64_t &roll = isAudio ? __aRoll : __vRoll;
#endif /* __FORCE_ROLL_OVER_FOR_DEBUG */
				uint64_t value = (pPTS[0]&0x0f) >> 1;
				value = (value << 8) | pPTS[1];
				value = (value << 7) | (pPTS[2] >> 1);
				value = (value << 8) | pPTS[3];
				value = (value << 7) | (pPTS[4] >> 1);
#ifdef __FORCE_ROLL_OVER_FOR_DEBUG
				if (roll == 0) {
					roll = 0x1ffffffffLL;
					roll -= value;
					roll -= (__FORCE_ROLL_OVER_FOR_DEBUG * 90000);
				}
				value = (value + roll)&0x1ffffffff;
#endif /* __FORCE_ROLL_OVER_FOR_DEBUG */
#ifdef __DUMP_TIMESTAMP_INFO_FOR_DEBUG
				string dbg = format("%c lastRawPts: %09"PRIx64" -> value: %09"PRIx64"; ", isAudio ? 'A' : 'V', lastRawPts, value);
#endif /* __DUMP_TIMESTAMP_INFO_FOR_DEBUG */
				if (((lastRawPts >> 32) == 1)
						&& ((value >> 32) == 0)) {
#ifdef __DUMP_TIMESTAMP_INFO_FOR_DEBUG
					dbg += format("%c Roll over!; ", isAudio ? 'A' : 'V');
#endif /* __DUMP_TIMESTAMP_INFO_FOR_DEBUG */
					rollOverCount++;
				}
				lastRawPts = value;
				value += (rollOverCount * 0x1ffffffffLL);
#ifdef __DUMP_TIMESTAMP_INFO_FOR_DEBUG
				dbg += format("final: %09"PRIx64"; ", value);
#endif /* __DUMP_TIMESTAMP_INFO_FOR_DEBUG */
				double tempPtsTime = (double) value / 90.00;
				if (ptsTime > tempPtsTime) {
					WARN("Back time");
					DROP_PACKET;
					return true;
				}
#ifdef __DUMP_TIMESTAMP_INFO_FOR_DEBUG
				dbg += format("diff: %.2f", tempPtsTime - ptsTime);
				FINEST("%s", STR(dbg));
#endif /* __DUMP_TIMESTAMP_INFO_FOR_DEBUG */
				ptsTime = tempPtsTime;
			}

#ifdef COMPUTE_DTS_TIME
			if (pDTS != NULL) {
				uint64_t value = (pDTS[0]&0x0f) >> 1;
				value = (value << 8) | pDTS[1];
				value = (value << 7) | (pDTS[2] >> 1);
				value = (value << 8) | pDTS[3];
				value = (value << 7) | (pDTS[4] >> 1);
				dtsTime = (double) value / 90.00;
			}
#endif
			if (pPTS == NULL) {
				WARN("No PTS!");
				DROP_PACKET;
				return true;
			}

			if (deltaTime < 0)
				deltaTime = ptsTime;

			absoluteTime = (ptsTime - deltaTime);
			_feedTime = _feedTime < absoluteTime ? absoluteTime : _feedTime;

			pData += 9 + pesHeaderLength;
			length -= (9 + pesHeaderLength);

		} else {
			WARN("Not enoght data");
			DROP_PACKET;
			return true;
		}
	}

	if (isAudio)
		return HandleAudioData(pData, length, ptsTime - deltaTime, packetStart);
	else
		return HandleVideoData(pData, length, ptsTime - deltaTime, packetStart);
}

bool InNetTSStream::FeedData(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp, bool isAudio) {
	LinkedListNode<BaseOutStream *> *pTemp = _pOutStreams;
	while (pTemp != NULL) {
		if (!pTemp->info->IsEnqueueForDelete()) {
			if (!pTemp->info->FeedData(pData, dataLength, processedLength, totalLength,
					absoluteTimestamp, isAudio)) {
				FINEST("Unable to feed OS: %p", pTemp->info);
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

void InNetTSStream::ReadyForSend() {
	NYI;
}

bool InNetTSStream::IsCompatibleWithType(uint64_t type) {
	return TAG_KIND_OF(type, ST_OUT_NET_RTMP_4_TS)
			|| (type == ST_OUT_NET_RTP)
			|| (type == ST_OUT_FILE_HLS);
}

void InNetTSStream::SignalOutStreamAttached(BaseOutStream *pOutStream) {
	NYI;
}

void InNetTSStream::SignalOutStreamDetached(BaseOutStream *pOutStream) {
	NYI;
}

bool InNetTSStream::SignalPlay(double &absoluteTimestamp, double &length) {
	return true;
}

bool InNetTSStream::SignalPause() {
	return true;
}

bool InNetTSStream::SignalResume() {
	return true;
}

bool InNetTSStream::SignalSeek(double &absoluteTimestamp) {
	return true;
}

bool InNetTSStream::SignalStop() {
	return true;
}

void InNetTSStream::GetStats(Variant &info, uint32_t namespaceId) {
	BaseInNetStream::GetStats(info, namespaceId);
	info["audio"]["packetsCount"] = _statsAudioPacketsCount;
	info["audio"]["droppedPacketsCount"] = _audioDroppedPacketsCount;
	info["audio"]["bytesCount"] = _audioBytesCount;
	info["audio"]["droppedBytesCount"] = _audioDroppedBytesCount;
	info["video"]["packetsCount"] = _videoPacketsCount;
	info["video"]["droppedPacketsCount"] = _videoDroppedPacketsCount;
	info["video"]["bytesCount"] = _videoBytesCount;
	info["video"]["droppedBytesCount"] = _videoDroppedBytesCount;
}

bool InNetTSStream::HandleAudioData(uint8_t *pRawBuffer, uint32_t rawBufferLength,
		double timestamp, bool packetStart) {
	_audioBytesCount += rawBufferLength;
	_statsAudioPacketsCount++;
	//the payload here respects this format:
	//6.2  Audio Data Transport Stream, ADTS
	//iso13818-7 page 26/206

	//1. Save the data
	_audioBuffer.ReadFromBuffer(pRawBuffer, rawBufferLength);

	InitializeAudioCapabilities(pRawBuffer, rawBufferLength);

	if (_lastGotAudioTimestamp != timestamp) {
		_audioPacketsCount = 0;
	}
	_lastGotAudioTimestamp = timestamp;

	for (;;) {
		//2. Get the buffer details: length and pointer
		uint32_t bufferLength = GETAVAILABLEBYTESCOUNT(_audioBuffer);
		uint8_t *pBuffer = GETIBPOINTER(_audioBuffer);

		//3. Do we have at least 6 bytes to read the length?
		if (bufferLength < 6) {
			break;
		}

		if ((ENTOHSP(pBuffer)&0xfff0) != 0xfff0) {
			_audioBuffer.Ignore(1);
			_audioDroppedBytesCount++;
			continue;
		} else {
		}

		//4. Read the frame length and see if we have enough data.
		//NOTE: frameLength INCLUDES the headers, that is why we test it directly
		//on bufferLength
		uint32_t frameLength = ((((pBuffer[3]&0x03) << 8) | pBuffer[4]) << 3) | (pBuffer[5] >> 5);
		if (frameLength < 8) {
			WARN("Bogus frameLength %u. Skip one byte", frameLength);
			FINEST("_audioBuffer:\n%s", STR(_audioBuffer));
			_audioBuffer.Ignore(1);
			continue;
		}
		if (bufferLength < frameLength) {
			break;
		}

		double ts = timestamp + (((double) _audioPacketsCount * 1024.00) / (double) _streamCapabilities.aac._sampleRate)*1000.0;
		_audioPacketsCount++;
		if (_lastSentAudioTimestamp >= ts) {
			ts = _lastSentAudioTimestamp;
		}
		_lastSentAudioTimestamp = ts;

		//5. Feed
		if (!FeedData(pBuffer, frameLength, 0, frameLength, ts, true)) {
			FATAL("Unable to feed audio data");
			return false;
		}

		//6. Ignore frameLength bytes
		_audioBuffer.Ignore(frameLength);
	}

	return true;
}

bool InNetTSStream::HandleVideoData(uint8_t *pBuffer, uint32_t length,
		double timestamp, bool packetStart) {
	_videoBytesCount += length;
	_videoPacketsCount++;
	//1. Store the data inside our buffer
	_currentNal.ReadFromBuffer(pBuffer, length);

	//2. Get the initial buffer and size
	uint32_t size = GETAVAILABLEBYTESCOUNT(_currentNal);
	uint8_t *pNalBuffer = GETIBPOINTER(_currentNal);
	uint32_t testValue = 0;

	//3. If this is the first NAL encountered, than lock
	//on the first byte from the first packet
	if (_firstNAL) {
		_cursor = 0;
		if (size < 4)
			return true;
		while (_cursor < size - 4) {
			testValue = ENTOHLP(pNalBuffer + _cursor);
			if ((testValue >> 8) == 1) {
				_videoDroppedBytesCount += (_cursor + 3);
				_currentNal.Ignore(_cursor + 3);
				_firstNAL = false;
				_cursor = 0;
				size = GETAVAILABLEBYTESCOUNT(_currentNal);
				pNalBuffer = GETIBPOINTER(_currentNal);
				break;
			}
			if (testValue == 1) {
				_videoDroppedBytesCount += (_cursor + 4);
				_currentNal.Ignore(_cursor + 4);
				_firstNAL = false;
				_cursor = 0;
				size = GETAVAILABLEBYTESCOUNT(_currentNal);
				pNalBuffer = GETIBPOINTER(_currentNal);
				break;
			}
			_cursor++;
		}
	}

	if (size < 4) {
		return true;
	}

	//4. Search for the next NAL boundary
	bool found = false;
	int8_t markerSize = 0;

	while (_cursor < size - 4) {
		testValue = ENTOHLP(pNalBuffer + _cursor);
		if ((testValue >> 8) == 1) {
			markerSize = 3;
			found = true;
		} else if (testValue == 1) {
			markerSize = 4;
			found = true;
		}

		if (!found) {
			_cursor++;
			continue;
		}
		found = false;

		if (!ProcessNal(timestamp)) {
			FATAL("Unable to process NALU");
			return false;
		}

		_currentNal.Ignore(_cursor + markerSize);
		pNalBuffer = GETIBPOINTER(_currentNal);
		size = GETAVAILABLEBYTESCOUNT(_currentNal);
		_cursor = 0;
		if (size < 4)
			break;
	}

	return true;
}

bool InNetTSStream::ProcessNal(double timestamp) {
	InitializeVideoCapabilities(GETIBPOINTER(_currentNal), _cursor);
	//5. Feed
	return FeedData(
			GETIBPOINTER(_currentNal),
			_cursor,
			0,
			_cursor,
			timestamp,
			false);
}

void InNetTSStream::InitializeVideoCapabilities(uint8_t *pData, uint32_t length) {
	switch (NALU_TYPE(pData[0])) {
		case NALU_TYPE_SPS:
		{
			_SPS.IgnoreAll();
			_SPS.ReadFromBuffer(pData, length);
			break;
		}
		case NALU_TYPE_PPS:
		{
			if (GETAVAILABLEBYTESCOUNT(_SPS) == 0)
				break;
			_PPS.IgnoreAll();
			_PPS.ReadFromBuffer(pData, length);
			if (!_streamCapabilities.InitVideoH264(
					GETIBPOINTER(_SPS),
					GETAVAILABLEBYTESCOUNT(_SPS),
					GETIBPOINTER(_PPS),
					GETAVAILABLEBYTESCOUNT(_PPS)
					)) {
				_streamCapabilities.ClearVideo();
				WARN("Unable to initialize h264 codec");
			}
			break;
		}
		default:
		{
			break;
		}
	}
}

void InNetTSStream::InitializeAudioCapabilities(uint8_t *pData, uint32_t length) {
	if (_streamCapabilities.audioCodecId == CODEC_AUDIO_UNKNOWN) {
		uint8_t mpegts2rtmpProfile[] = {
			1, 2, 3
		};

		BitArray codecSetup;
		//profile_index from MPEG-TS different from profile_index from RTMP
		//so we need a map
		uint8_t profile = pData[2] >> 6;
		codecSetup.PutBits<uint8_t > (mpegts2rtmpProfile[profile], 5);

		//frequence mapping is the same
		//iso13818-7.pdf page 46/206
		uint8_t sampling_frequency_index = (pData[2] >> 2)&0x0f;
		codecSetup.PutBits<uint8_t > (sampling_frequency_index, 4);

		//this could be an issue... AAC from RTMP only supports stereo
		//and we have mono in this file... Need to check out this one...
		codecSetup.PutBits<uint8_t > (2, 4);

		_streamCapabilities.InitAudioAAC(GETIBPOINTER(codecSetup),
				GETAVAILABLEBYTESCOUNT(codecSetup));
	}
}

#endif	/* HAS_PROTOCOL_TS */

