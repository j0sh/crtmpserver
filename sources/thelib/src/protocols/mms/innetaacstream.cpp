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

#ifdef HAS_PROTOCOL_MMS

#include "protocols/mms/innetaacstream.h"
#include "streaming/baseoutstream.h"
#include "streaming/streamstypes.h"

InNetAACStream::InNetAACStream(BaseProtocol *pProtocol,
		StreamsManager *pStreamsManager, string name)
: BaseInNetStream(pProtocol, pStreamsManager, ST_IN_NET_AAC, name) {
	_bytesCount = 0;
	_packetsCount = 0;
	_capabilities.Clear();
	_capabilities.audioCodecId = CODEC_AUDIO_UNKNOWN;
}

InNetAACStream::~InNetAACStream() {
}

bool InNetAACStream::IsCompatibleWithType(uint64_t type) {
	return (type == ST_OUT_NET_RTMP_4_TS)
			|| (type == ST_OUT_NET_RTP);
}

StreamCapabilities * InNetAACStream::GetCapabilities() {
	return &_capabilities;
}

void InNetAACStream::GetStats(Variant &info, uint32_t namespaceId) {
	BaseInNetStream::GetStats(info, namespaceId);
	info["audio"]["bytesCount"] = _bytesCount;
	info["audio"]["packetsCount"] = _packetsCount;
	info["audio"]["droppedPacketsCount"] = 0;
}

void InNetAACStream::ReadyForSend() {
	NYI;
}

void InNetAACStream::SignalOutStreamAttached(BaseOutStream *pOutStream) {
	NYI;
}

void InNetAACStream::SignalOutStreamDetached(BaseOutStream *pOutStream) {
	NYI;
}

bool InNetAACStream::SignalPlay(double &absoluteTimestamp, double &length) {
	NYIR;
}

bool InNetAACStream::SignalPause() {
	NYIR;
}

bool InNetAACStream::SignalResume() {
	return true;
}

bool InNetAACStream::SignalSeek(double &absoluteTimestamp) {
	return true;
}

bool InNetAACStream::SignalStop() {
	NYIR;
}

bool InNetAACStream::FeedData(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp, bool isAudio) {
	_bytesCount += dataLength;
	_packetsCount++;
	if (dataLength == 0)
		return true;
	if (_capabilities.audioCodecId == CODEC_AUDIO_UNKNOWN) {
		if (!InitStreamCapabilities(pData, dataLength)) {
			FATAL("Unable to init stream capabilities");
			return false;
		}
	}
	LinkedListNode<BaseOutStream *> *pTemp = _pOutStreams;
	while (pTemp != NULL) {
		if (!pTemp->info->IsEnqueueForDelete()) {
			if (!pTemp->info->FeedData(pData, dataLength, processedLength, totalLength,
					absoluteTimestamp, isAudio)) {
				WARN("Unable to feed OS: %p", pTemp->info);
				//_outProtocols.erase(pTemp->info->GetProtocol()->GetId());
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

bool InNetAACStream::InitStreamCapabilities(uint8_t *pData, uint32_t dataLength) {
	if (_capabilities.audioCodecId == CODEC_AUDIO_UNKNOWN) {
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

		_capabilities.InitAudioAAC(GETIBPOINTER(codecSetup),
				GETAVAILABLEBYTESCOUNT(codecSetup));
	}
	return true;
}

#endif /* HAS_PROTOCOL_MMS */
