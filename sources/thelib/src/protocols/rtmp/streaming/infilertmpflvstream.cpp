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
#include "protocols/rtmp/streaming/infilertmpflvstream.h"
#include "streaming/streamstypes.h"
#include "protocols/rtmp/messagefactories/streammessagefactory.h"
#include "protocols/rtmp/basertmpprotocol.h"
#include "protocols/rtmp/streaming/baseoutnetrtmpstream.h"

InFileRTMPFLVStream::InFileRTMPFLVStream(BaseProtocol *pProtocol,
		StreamsManager *pStreamsManager, string name)
: InFileRTMPStream(pProtocol, pStreamsManager, ST_IN_FILE_RTMP_FLV, name) {

}

InFileRTMPFLVStream::~InFileRTMPFLVStream() {
}

bool InFileRTMPFLVStream::BuildFrame(FileClass *pFile, MediaFrame &mediaFrame,
		IOBuffer &buffer) {
	//1. Seek into the data file at the correct position
	if (!pFile->SeekTo(mediaFrame.start)) {
		FATAL("Unable to seek to position %llu", mediaFrame.start);
		return false;
	}

	//2. Read the data
	if (!buffer.ReadFromFs(*pFile, (uint32_t) mediaFrame.length)) {
		FATAL("Unable to read %llu bytes from offset %llu", mediaFrame.length, mediaFrame.start);
		return false;
	}

	//3. Done
	return true;
}

bool InFileRTMPFLVStream::FeedMetaData(FileClass *pFile, MediaFrame &mediaFrame) {
	//1. Seek into the data file at the correct position
	if (!pFile->SeekTo(mediaFrame.start)) {
		FATAL("Unable to seek to position %llu", mediaFrame.start);
		return false;
	}

	//2. Read the data
	_metadataBuffer.IgnoreAll();
	if (!_metadataBuffer.ReadFromFs(*pFile, (uint32_t) mediaFrame.length)) {
		FATAL("Unable to read %llu bytes from offset %llu", mediaFrame.length, mediaFrame.start);
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
