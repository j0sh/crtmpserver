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

#ifdef HAS_PROTOCOL_RTMP
#include "protocols/rtmp/streaming/infilertmpstream.h"
#include "mediaformats/basemediadocument.h"
#include "mediaformats/flv/flvdocument.h"
#include "mediaformats/mp3/mp3document.h"
#include "mediaformats/mp4/mp4document.h"
#include "protocols/rtmp/streaming/infilertmpflvstream.h"
#include "protocols/rtmp/basertmpprotocol.h"
#include "streaming/baseoutstream.h"
#include "protocols/baseprotocol.h"
#include "protocols/rtmp/streaming/baseoutnetrtmpstream.h"
#include "protocols/rtmp/streaming/infilertmpmp3stream.h"
#include "protocols/rtmp/streaming/infilertmpmp4stream.h"
#include "protocols/rtmp/streaming/infilertmpnsvstream.h"
#include "streaming/streamstypes.h"

InFileRTMPStream::InFileRTMPStream(BaseProtocol *pProtocol,
		StreamsManager *pStreamsManager, uint64_t type, string name)
: BaseInFileStream(pProtocol, pStreamsManager, type, name) {
	if (!TAG_KIND_OF(type, ST_IN_FILE_RTMP)) {
		ASSERT("Incorrect stream type. Wanted a stream type in class %s and got %s",
				STR(tagToString(ST_IN_FILE_RTMP)), STR(tagToString(type)));
	}
	_chunkSize = 4 * 1024 * 1024;
}

InFileRTMPStream::~InFileRTMPStream() {
}

bool InFileRTMPStream::FeedData(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp, bool isAudio) {
	ASSERT("Operation not supported");
	return false;
}

bool InFileRTMPStream::IsCompatibleWithType(uint64_t type) {
	return TAG_KIND_OF(type, ST_OUT_NET_RTMP);
}

uint32_t InFileRTMPStream::GetChunkSize() {
	return _chunkSize;
}

InFileRTMPStream *InFileRTMPStream::GetInstance(BaseRTMPProtocol *pRTMPProtocol,
		StreamsManager *pStreamsManager, Variant &metadata) {
	metadata[META_RTMP_META][HTTP_HEADERS_SERVER] = HTTP_HEADERS_SERVER_US;
	if (!fileExists(metadata[META_SERVER_FULL_PATH])) {
		FATAL("File not found. fullPath: `%s`", STR(metadata[META_SERVER_FULL_PATH]));
		return false;
	}

	InFileRTMPStream *pResult = NULL;

	if (metadata[META_MEDIA_TYPE] == MEDIA_TYPE_FLV ||
			metadata[META_MEDIA_TYPE] == MEDIA_TYPE_LIVE_OR_FLV) {
		pResult = new InFileRTMPFLVStream((BaseProtocol *) pRTMPProtocol,
				pStreamsManager, metadata[META_SERVER_FULL_PATH]);
	} else if (metadata[META_MEDIA_TYPE] == MEDIA_TYPE_MP3) {
		pResult = new InFileRTMPMP3Stream((BaseProtocol *) pRTMPProtocol,
				pStreamsManager, metadata[META_SERVER_FULL_PATH]);
	} else if (metadata[META_MEDIA_TYPE] == MEDIA_TYPE_NSV) {
		pResult = new InFileRTMPNSVStream((BaseProtocol *) pRTMPProtocol,
				pStreamsManager, metadata[META_SERVER_FULL_PATH]);
	} else if (metadata[META_MEDIA_TYPE] == MEDIA_TYPE_MP4 ||
			metadata[META_MEDIA_TYPE] == MEDIA_TYPE_M4A ||
			metadata[META_MEDIA_TYPE] == MEDIA_TYPE_M4V ||
			metadata[META_MEDIA_TYPE] == MEDIA_TYPE_MOV) {
		pResult = new InFileRTMPMP4Stream((BaseProtocol *) pRTMPProtocol,
				pStreamsManager, metadata[META_SERVER_FULL_PATH]);
	} else {
		FATAL("File type not supported yet. Metadata:\n%s",
				STR(metadata.ToString()));
	}

	if (pResult != NULL) {
		pResult->SetCompleteMetadata(metadata);
	}

	return pResult;
}

void InFileRTMPStream::SetCompleteMetadata(Variant &completeMetadata) {
	_completeMetadata = completeMetadata;
}

Variant InFileRTMPStream::GetCompleteMetadata() {
	return _completeMetadata;
}

void InFileRTMPStream::SignalOutStreamAttached(BaseOutStream *pOutStream) {
	//2. Set a big chunk size on the corresponding connection
	if (TAG_KIND_OF(pOutStream->GetType(), ST_OUT_NET_RTMP)) {
		((BaseRTMPProtocol *) pOutStream->GetProtocol())->TrySetOutboundChunkSize(_chunkSize);
		((BaseOutNetRTMPStream *) pOutStream)->SetFeederChunkSize(_chunkSize);
		((BaseOutNetRTMPStream *) pOutStream)->CanDropFrames(false);
	}
}

void InFileRTMPStream::SignalOutStreamDetached(BaseOutStream *pOutStream) {
	FINEST("outbound stream %u detached from inbound stream %u",
			pOutStream->GetUniqueId(), GetUniqueId());
}
#endif /* HAS_PROTOCOL_RTMP */

