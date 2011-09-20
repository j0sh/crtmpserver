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

#include "streaming/innetrawstream.h"
#include "streaming/streamstypes.h"
#include "streaming/baseoutstream.h"

InNetRawStream::InNetRawStream(BaseProtocol *pProtocol,
		StreamsManager *pStreamsManager, string name, uint64_t codecType)
: BaseInNetStream(pProtocol, pStreamsManager, ST_IN_NET_RAW, name) {
	_bytesCount = 0;
	_packetsCount = 0;
	_witness.Initialize("/tmp/" + name, FILE_OPEN_MODE_TRUNCATE);
	if (codecType == CODEC_AUDIO_ADTS) {
		_capabilities.Clear();
		_capabilities.InitAudioADTS();
	} else if (codecType == CODEC_AUDIO_MP3) {
		_capabilities.Clear();
		_capabilities.InitAudioMP3();
	} else {
		ASSERT("InNetRawStream only supports %s and %s codecs",
				STR(tagToString(CODEC_AUDIO_ADTS)),
				STR(tagToString(CODEC_AUDIO_MP3)));
	}
}

InNetRawStream::~InNetRawStream() {
}

bool InNetRawStream::IsCompatibleWithType(uint64_t type) {
	return type == ST_OUT_NET_RAW;
}

StreamCapabilities * InNetRawStream::GetCapabilities() {
	return &_capabilities;
}

void InNetRawStream::GetStats(Variant &info, uint32_t namespaceId) {
	BaseInNetStream::GetStats(info, namespaceId);
	info["video"]["bytesCount"] = _bytesCount;
	info["video"]["packetsCount"] = _packetsCount;
	info["video"]["droppedPacketsCount"] = 0;
	info["audio"]["bytesCount"] = 0;
	info["audio"]["packetsCount"] = 0;
	info["audio"]["droppedPacketsCount"] = 0;
}

void InNetRawStream::ReadyForSend() {

}

void InNetRawStream::SignalOutStreamAttached(BaseOutStream *pOutStream) {

}

void InNetRawStream::SignalOutStreamDetached(BaseOutStream *pOutStream) {

}

bool InNetRawStream::SignalPlay(double &absoluteTimestamp, double &length) {
	NYIR;
}

bool InNetRawStream::SignalPause() {
	NYIR;
}

bool InNetRawStream::SignalResume() {
	NYIR;
}

bool InNetRawStream::SignalSeek(double &absoluteTimestamp) {
	NYIR;
}

bool InNetRawStream::SignalStop() {
	NYIR;
}

bool InNetRawStream::FeedData(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp, bool isAudio) {
	_bytesCount += dataLength;
	_packetsCount++;
	_witness.WriteBuffer(pData, dataLength);
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
