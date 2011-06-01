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
#include "protocols/rtmp/basertmpprotocol.h"
#include "streaming/baseoutstream.h"
#include "protocols/baseprotocol.h"
#include "protocols/rtmp/streaming/baseoutnetrtmpstream.h"
#include "streaming/streamstypes.h"
#include "protocols/rtmp/messagefactories/messagefactories.h"
#include "protocols/rtmp/basertmpprotocol.h"
#include "protocols/rtmp/streaming/baseoutnetrtmpstream.h"

InFileRTMPStream::BaseBuilder::BaseBuilder() {

}

InFileRTMPStream::BaseBuilder::~BaseBuilder() {

}

InFileRTMPStream::AVCBuilder::AVCBuilder() {
	_videoCodecHeaderInit[0] = 0x17;
	_videoCodecHeaderInit[1] = 0;
	_videoCodecHeaderInit[2] = 0;
	_videoCodecHeaderInit[3] = 0;
	_videoCodecHeaderInit[4] = 0;

	_videoCodecHeaderKeyFrame[0] = 0x17;
	_videoCodecHeaderKeyFrame[1] = 1;

	_videoCodecHeader[0] = 0x27;
	_videoCodecHeader[1] = 1;
}

InFileRTMPStream::AVCBuilder::~AVCBuilder() {

}

bool InFileRTMPStream::AVCBuilder::BuildFrame(FileClass* pFile, MediaFrame& mediaFrame, IOBuffer& buffer) {
	if (mediaFrame.isBinaryHeader) {
		buffer.ReadFromBuffer(_videoCodecHeaderInit, sizeof (_videoCodecHeaderInit));
	} else {
		if (mediaFrame.isKeyFrame) {
			// video key frame
			buffer.ReadFromBuffer(_videoCodecHeaderKeyFrame, sizeof (_videoCodecHeaderKeyFrame));
		} else {
			//video normal frame
			buffer.ReadFromBuffer(_videoCodecHeader, sizeof (_videoCodecHeader));
		}
		mediaFrame.compositionOffset = (EHTONL(mediaFrame.compositionOffset & 0x00ffffff)) >> 8;
		buffer.ReadFromBuffer((uint8_t *) & mediaFrame.compositionOffset, 3);
	}

	if (!pFile->SeekTo(mediaFrame.start)) {
		FATAL("Unable to seek to position %"PRIu64, mediaFrame.start);
		return false;
	}

	if (!buffer.ReadFromFs(*pFile, (uint32_t) mediaFrame.length)) {
		FATAL("Unable to read %"PRIu64" bytes from offset %"PRIu64, mediaFrame.length, mediaFrame.start);
		return false;
	}

	return true;
}

InFileRTMPStream::AACBuilder::AACBuilder() {
	_audioCodecHeaderInit[0] = 0xaf;
	_audioCodecHeaderInit[1] = 0;
	_audioCodecHeader[0] = 0xaf;
	_audioCodecHeader[1] = 0x01;
}

InFileRTMPStream::AACBuilder::~AACBuilder() {

}

bool InFileRTMPStream::AACBuilder::BuildFrame(FileClass* pFile, MediaFrame& mediaFrame, IOBuffer& buffer) {
	//1. add the binary header
	if (mediaFrame.isBinaryHeader) {
		buffer.ReadFromBuffer(_audioCodecHeaderInit, sizeof (_audioCodecHeaderInit));
	} else {
		buffer.ReadFromBuffer(_audioCodecHeader, sizeof (_audioCodecHeader));
	}

	//2. Seek into the data file at the correct position
	if (!pFile->SeekTo(mediaFrame.start)) {
		FATAL("Unable to seek to position %"PRIu64, mediaFrame.start);
		return false;
	}

	//3. Read the data
	if (!buffer.ReadFromFs(*pFile, (uint32_t) mediaFrame.length)) {
		FATAL("Unable to read %"PRIu64" bytes from offset %"PRIu64, mediaFrame.length, mediaFrame.start);
		return false;
	}

	return true;
}

InFileRTMPStream::MP3Builder::MP3Builder() {

}

InFileRTMPStream::MP3Builder::~MP3Builder() {

}

bool InFileRTMPStream::MP3Builder::BuildFrame(FileClass *pFile,
		MediaFrame &mediaFrame, IOBuffer &buffer) {
	buffer.ReadFromRepeat(0x2f, 1);

	//2. Seek into the data file at the correct position
	if (!pFile->SeekTo(mediaFrame.start)) {
		FATAL("Unable to seek to position %"PRIu64, mediaFrame.start);
		return false;
	}

	//3. Read the data
	if (!buffer.ReadFromFs(*pFile, (uint32_t) mediaFrame.length)) {
		FATAL("Unable to read %"PRIu64" bytes from offset %"PRIu64, mediaFrame.length, mediaFrame.start);
		return false;
	}

	return true;
}

InFileRTMPStream::PassThroughBuilder::PassThroughBuilder() {

}

InFileRTMPStream::PassThroughBuilder::~PassThroughBuilder() {

}

bool InFileRTMPStream::PassThroughBuilder::BuildFrame(FileClass *pFile,
		MediaFrame &mediaFrame, IOBuffer &buffer) {
	//1. Seek into the data file at the correct position
	if (!pFile->SeekTo(mediaFrame.start)) {
		FATAL("Unable to seek to position %"PRIu64, mediaFrame.start);
		return false;
	}

	//2. Read the data
	if (!buffer.ReadFromFs(*pFile, (uint32_t) mediaFrame.length)) {
		FATAL("Unable to read %"PRIu64" bytes from offset %"PRIu64, mediaFrame.length, mediaFrame.start);
		return false;
	}

	//3. Done
	return true;
}

InFileRTMPStream::InFileRTMPStream(BaseProtocol *pProtocol,
		StreamsManager *pStreamsManager, string name)
: BaseInFileStream(pProtocol, pStreamsManager, ST_IN_FILE_RTMP, name) {
	_chunkSize = 4 * 1024 * 1024;
	_pAudioBuilder = NULL;
	_pVideoBuilder = NULL;
}

InFileRTMPStream::~InFileRTMPStream() {
	if (_pAudioBuilder != NULL) {
		delete _pAudioBuilder;
		_pAudioBuilder = NULL;
	}
	if (_pVideoBuilder != NULL) {
		delete _pVideoBuilder;
		_pVideoBuilder = NULL;
	}
}

bool InFileRTMPStream::Initialize(int32_t clientSideBufferLength) {
	//1. Base init
	if (!BaseInFileStream::Initialize(clientSideBufferLength)) {
		FATAL("Unable to initialize stream");
		return false;
	}

	//2. Get stream capabilities
	StreamCapabilities *pCapabilities = GetCapabilities();
	if (pCapabilities == NULL) {
		FATAL("Invalid stream capabilities");
		return false;
	}

	//3. Create the video builder
	if ((pCapabilities->videoCodecId != 0)
			&& (pCapabilities->videoCodecId != CODEC_VIDEO_UNKNOWN)
			&& (pCapabilities->videoCodecId != CODEC_VIDEO_AVC)
			&& (pCapabilities->videoCodecId != CODEC_VIDEO_PASS_THROUGH)) {
		FATAL("Invalid video stream capabilities: %s", STR(tagToString(pCapabilities->videoCodecId)));
		return false;
	}
	if (pCapabilities->videoCodecId == CODEC_VIDEO_AVC) {
		_pVideoBuilder = new AVCBuilder();
	} else if (pCapabilities->videoCodecId == CODEC_VIDEO_PASS_THROUGH) {
		_pVideoBuilder = new PassThroughBuilder();
	}

	//4. Create the audio builder
	if ((pCapabilities->audioCodecId != 0)
			&& (pCapabilities->audioCodecId != CODEC_AUDIO_UNKNOWN)
			&& (pCapabilities->audioCodecId != CODEC_AUDIO_AAC)
			&& (pCapabilities->audioCodecId != CODEC_AUDIO_MP3)
			&& (pCapabilities->audioCodecId != CODEC_AUDIO_PASS_THROUGH)) {
		FATAL("Invalid audio stream capabilities: %s", STR(tagToString(pCapabilities->audioCodecId)));
		return false;
	}
	if (pCapabilities->audioCodecId == CODEC_AUDIO_AAC) {
		_pAudioBuilder = new AACBuilder();
	} else if (pCapabilities->audioCodecId == CODEC_AUDIO_MP3) {
		_pAudioBuilder = new MP3Builder();
	} else if (pCapabilities->audioCodecId == CODEC_AUDIO_PASS_THROUGH) {
		_pAudioBuilder = new PassThroughBuilder();
	}

	return true;
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

	if (metadata[META_MEDIA_TYPE] == MEDIA_TYPE_FLV
			|| metadata[META_MEDIA_TYPE] == MEDIA_TYPE_LIVE_OR_FLV
			|| metadata[META_MEDIA_TYPE] == MEDIA_TYPE_MP3
			|| metadata[META_MEDIA_TYPE] == MEDIA_TYPE_MP4
			|| metadata[META_MEDIA_TYPE] == MEDIA_TYPE_M4A
			|| metadata[META_MEDIA_TYPE] == MEDIA_TYPE_M4V
			|| metadata[META_MEDIA_TYPE] == MEDIA_TYPE_MOV
			//||metadata[META_MEDIA_TYPE] == MEDIA_TYPE_NSV
			) {
		pResult = new InFileRTMPStream((BaseProtocol *) pRTMPProtocol,
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

bool InFileRTMPStream::BuildFrame(FileClass *pFile, MediaFrame &mediaFrame,
		IOBuffer &buffer) {
	switch (mediaFrame.type) {
		case MEDIAFRAME_TYPE_AUDIO:
			if (_pAudioBuilder != NULL)
				return _pAudioBuilder->BuildFrame(pFile, mediaFrame, buffer);
			return true;
		case MEDIAFRAME_TYPE_VIDEO:
			if (_pVideoBuilder != NULL)
				return _pVideoBuilder->BuildFrame(pFile, mediaFrame, buffer);
			return true;
		default:
			return true;
	}
}

bool InFileRTMPStream::FeedMetaData(FileClass *pFile, MediaFrame &mediaFrame) {
	//1. Seek into the data file at the correct position
	if (!pFile->SeekTo(mediaFrame.start)) {
		FATAL("Unable to seek to position %"PRIu64, mediaFrame.start);
		return false;
	}

	//2. Read the data
	_metadataBuffer.IgnoreAll();
	if (!_metadataBuffer.ReadFromFs(*pFile, (uint32_t) mediaFrame.length)) {
		FATAL("Unable to read %"PRIu64" bytes from offset %"PRIu64, mediaFrame.length, mediaFrame.start);
		return false;
	}

	//3. Parse the metadata
	_metadataName = "";
	_metadataParameters.Reset();

	_tempVariant.Reset();
	if (!_amfSerializer.Read(_metadataBuffer, _tempVariant)) {
		WARN("Unable to read metadata");
		return true;
	}
	if (_tempVariant != V_STRING) {
		WARN("Unable to read metadata");
		return true;
	}
	_metadataName = ((string) _tempVariant);

	while (GETAVAILABLEBYTESCOUNT(_metadataBuffer) > 0) {
		_tempVariant.Reset();
		if (!_amfSerializer.Read(_metadataBuffer, _tempVariant)) {
			WARN("Unable to read metadata");
			return true;
		}
		_metadataParameters.PushToArray(_tempVariant);
	}

	Variant message = GenericMessageFactory::GetNotify(
			((BaseOutNetRTMPStream *) _pOutStreams->info)->GetCommandsChannelId(),
			((BaseOutNetRTMPStream *) _pOutStreams->info)->GetRTMPStreamId(),
			mediaFrame.absoluteTime,
			true,
			_metadataName,
			_metadataParameters);

	//5. Send it
	return ((BaseRTMPProtocol *) _pProtocol)->SendMessage(message);
}
#endif /* HAS_PROTOCOL_RTMP */

