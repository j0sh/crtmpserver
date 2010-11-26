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
		StreamsManager *pStreamsManager, string name)
: BaseInNetStream(pProtocol, pStreamsManager, ST_IN_NET_TS, name) {
	//audio section
	_pAudioPidDescriptor = NULL;
	_ptsTimeAudio = 0;
#ifdef COMPUTE_DTS_TIME
	_dtsTimeAudio = 0;
#endif
	_deltaTimeAudio = -1;
	_lastGotAudioTimestamp = 0;
	_lastSentAudioTimestamp = 0;
	_audioPacketsCount = 0;

	//video section
	_pVideoPidDescriptor = NULL;
	_ptsTimeVideo = 0;
#ifdef COMPUTE_DTS_TIME
	_dtsTimeVideo = 0;
#endif
	_deltaTimeVideo = -1;

	_feedTime = 0;
	_cursor = 0;

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

bool InNetTSStream::FeedData(uint8_t *pData, uint32_t length, bool packetStart,
		bool isAudio) {
	double &ptsTime = isAudio ? _ptsTimeAudio : _ptsTimeVideo;
#ifdef COMPUTE_DTS_TIME
	double &dtsTime = isAudio ? _dtsTimeAudio : _dtsTimeVideo;
#endif
	double &deltaTime = isAudio ? _deltaTimeAudio : _deltaTimeVideo;
	double absoluteTime = 0;
	if (packetStart) {
		if (length >= 8) {
			uint32_t pesHeaderLength = pData[8];
			if (pesHeaderLength + 9 > length) {
				FATAL("Not enough data");
				return false;
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
				uint64_t value = (pPTS[0]&0x0f) >> 1;
				value = (value << 8) | pPTS[1];
				value = (value << 7) | (pPTS[2] >> 1);
				value = (value << 8) | pPTS[3];
				value = (value << 7) | (pPTS[4] >> 1);
				double tempPtsTime = (double) value / 90.00;
				if (ptsTime > tempPtsTime) {
					WARN("Back time");
					return true;
				}
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
				return true;
			}

			if (deltaTime < 0)
				deltaTime = ptsTime;

			absoluteTime = (ptsTime - deltaTime);
			_feedTime = _feedTime < absoluteTime ? absoluteTime : _feedTime;
			//FINEST("%c: absoluteTime: %.2f;", isAudio ? 'A' : 'V', absoluteTime);

			pData += 9 + pesHeaderLength;
			length -= (9 + pesHeaderLength);

		} else {
			FATAL("Not enoght data");
			return false;
		}
	}

	if (isAudio)
		return HandleAudioData(pData, length, ptsTime - deltaTime, packetStart);
	else
		return HandleTSVideoData(pData, length, ptsTime - deltaTime, packetStart);
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
			|| (type == ST_OUT_NET_RTP);
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

bool InNetTSStream::HandleAudioData(uint8_t *pRawBuffer, uint32_t rawBufferLength,
		double timestamp, bool packetStart) {
	//the payload here respects this format:
	//6.2  Audio Data Transport Stream, ADTS
	//iso13818-7 page 26/206

	//	FINEST("rawBufferLength: %d; GETAVAILABLEBYTESCOUNT(_audioBuffer): %d",
	//			rawBufferLength, GETAVAILABLEBYTESCOUNT(_audioBuffer));


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
			//WARN("Bogus marker %02x %02x. Skip one byte", pBuffer[0], pBuffer[1]);
			_audioBuffer.Ignore(1);
			continue;
		} else {
			//FINEST("OK: %d", pBuffer[1]&0x01);
		}

		//4. Read the frame length and see if we have enough data.
		//NOTE: frameLength INCLUDES the headers, that is why we test it directly
		//on bufferLength
		uint32_t frameLength = ((((pBuffer[3]&0x03) << 8) | pBuffer[4]) << 3) | (pBuffer[5] >> 5);
		if (frameLength < 8) {
			WARN("Bogus framelength %d. Skip one byte", frameLength);
			FINEST("_audioBuffer:\n%s", STR(_audioBuffer));
			_audioBuffer.Ignore(1);
			continue;
		}
		if (bufferLength < frameLength) {
			//			FINEST("Not enough data. Wanted: %d; Got: %d",
			//					frameLength,
			//					GETAVAILABLEBYTESCOUNT(_audioBuffer));
			break;
		}

		double ts = timestamp + (((double) _audioPacketsCount * 1024.00) / (double) _streamCapabilities.aac._sampleRate)*1000.0;
		_audioPacketsCount++;
		if (_lastSentAudioTimestamp >= ts) {
			ts = _lastSentAudioTimestamp;
		}
		_lastSentAudioTimestamp = ts;
		//FINEST("timestamp: %.2f; _audioPacketsCount: %d", timestamp, _audioPacketsCount);

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

bool InNetTSStream::HandleVideoData_version1(uint8_t *pBuffer, uint32_t length,
		double timestamp, bool packetStart) {
	//1. Store the data inside our buffer
	_currentNal.ReadFromBuffer(pBuffer, length);

	//2. Get the size of our buffer
	uint32_t size = GETAVAILABLEBYTESCOUNT(_currentNal);
	if (size < 13)
		return true;

	//3. Get the raw pointer
	uint8_t *pNalBuffer = GETIBPOINTER(_currentNal);

	//4. Cycle through the buffer and detect nals
	while (_cursor < size - 4) {
		//5. Read the current position
		uint32_t test = ENTOHLP((pNalBuffer + _cursor));
		if (test == 1) {
			//6. This is the beginning of a new NALU. Process it
			//if it has some data inside it
			if (_cursor > 0) {
				if (!ProcessNal(timestamp)) {
					FATAL("Unable to process NALU");
					return false;
				}
			}

			//8. Ignore what we've just processed
			_currentNal.Ignore(_cursor + 4);

			//9. Optimize the NALU buffer
			_currentNal.MoveData();

			//10. Reposition the cursor
			_cursor = 0;

			//11. Update the size and the buffer pointer
			pNalBuffer = GETIBPOINTER(_currentNal);
			size = GETAVAILABLEBYTESCOUNT(_currentNal);
			if (size < 4)
				break;
		} else {
			//12. We are in the middle of a NAL
			_cursor++;
		}
	}

	return true;
}

bool InNetTSStream::HandleVideoData_version2(uint8_t *pBuffer, uint32_t length,
		double timestamp, bool packetStart) {
	//1. Store the data inside our buffer
	_currentNal.ReadFromBuffer(pBuffer, length);

	uint32_t size = GETAVAILABLEBYTESCOUNT(_currentNal);
	if (size < 4)
		return true;

	uint8_t *pNalBuffer = GETIBPOINTER(_currentNal);

	_cursor = 0;
	if (_firstNAL) {
		while (_cursor < size - 4) {
			if (ENTOHLP(pNalBuffer + _cursor) == 1) {
				_currentNal.Ignore(_cursor + 4);
				_firstNAL = false;
				_cursor += 4;
				break;
			}
			_cursor++;
		}
	}

	while (_cursor < size - 4) {
		if (ENTOHLP(pNalBuffer + _cursor) == 1) {
			if (!ProcessNal(timestamp)) {
				FATAL("Unable to process NALU");
				return false;
			}
			_currentNal.Ignore(_cursor + 4);
			pNalBuffer = GETIBPOINTER(_currentNal);
			size = GETAVAILABLEBYTESCOUNT(_currentNal);
			_cursor = 0;
			if (size < 4)
				break;
		} else {
			_cursor++;
		}
	}

	return true;
}

bool InNetTSStream::HandleVideoData_version3(uint8_t *pBuffer, uint32_t length,
		double timestamp, bool packetStart) {
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
		while (_cursor < size - 4) {
			testValue = ENTOHLP(pNalBuffer + _cursor);
			if ((testValue >> 8) == 1) {
				_currentNal.Ignore(_cursor + 3);
				_firstNAL = false;
				_cursor = 0;
				size = GETAVAILABLEBYTESCOUNT(_currentNal);
				pNalBuffer = GETIBPOINTER(_currentNal);
				break;
			}
			if (testValue == 1) {
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
	return true;
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
	//FINEST("_streamCapabilities.audioCodecId: %016llx", _streamCapabilities.audioCodecId);
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

