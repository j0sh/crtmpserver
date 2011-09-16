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

#include "protocols/mms/innetmp3stream.h"
#include "streaming/baseoutstream.h"
#include "streaming/streamstypes.h"

InNetMP3Stream::InNetMP3Stream(BaseProtocol *pProtocol,
		StreamsManager *pStreamsManager, string name)
: BaseInNetStream(pProtocol, pStreamsManager, ST_IN_NET_MP3, name) {
	_bytesCount = 0;
	_packetsCount = 0;
	_capabilities.Clear();
	_capabilities.InitAudioMP3();
}

InNetMP3Stream::~InNetMP3Stream() {
}

bool InNetMP3Stream::IsCompatibleWithType(uint64_t type) {
	//return TAG_KIND_OF(type, ST_OUT_NET_RTMP);
	return true;
}

StreamCapabilities * InNetMP3Stream::GetCapabilities() {
	return &_capabilities;
}

void InNetMP3Stream::GetStats(Variant &info, uint32_t namespaceId) {
	BaseInNetStream::GetStats(info, namespaceId);
	info["audio"]["bytesCount"] = _bytesCount;
	info["audio"]["packetsCount"] = _packetsCount;
	info["audio"]["droppedPacketsCount"] = 0;
}

void InNetMP3Stream::ReadyForSend() {
	NYI;
}

void InNetMP3Stream::SignalOutStreamAttached(BaseOutStream *pOutStream) {
	NYI;
}

void InNetMP3Stream::SignalOutStreamDetached(BaseOutStream *pOutStream) {
	NYI;
}

bool InNetMP3Stream::SignalPlay(double &absoluteTimestamp, double &length) {
	NYIR;
}

bool InNetMP3Stream::SignalPause() {
	NYIR;
}

bool InNetMP3Stream::SignalResume() {
	return true;
}

bool InNetMP3Stream::SignalSeek(double &absoluteTimestamp) {
	return true;
}

bool InNetMP3Stream::SignalStop() {
	NYIR;
}

bool InNetMP3Stream::FeedData(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp, bool isAudio) {
	_bytesCount += dataLength;
	_packetsCount++;
	uint8_t c = 0x2f;
	LinkedListNode<BaseOutStream *> *pTemp = _pOutStreams;
	while (pTemp != NULL) {
		if (!pTemp->info->IsEnqueueForDelete()) {
			if (!pTemp->info->FeedData(&c, 1, 0, totalLength + 1,
					absoluteTimestamp, isAudio)) {
				WARN("Unable to feed OS: %p", pTemp->info);
				//_outProtocols.erase(pTemp->info->GetProtocol()->GetId());
				pTemp->info->EnqueueForDelete();
				if (GetProtocol() == pTemp->info->GetProtocol()) {
					return false;
				}
			}
		}
		if (!pTemp->info->IsEnqueueForDelete()) {
			if (!pTemp->info->FeedData(pData, dataLength, 1, totalLength + 1,
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

#endif /* HAS_PROTOCOL_MMS */
