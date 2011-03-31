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
#include "protocols/rtmp/streaming/infilertmpmp4stream.h"
#include "streaming/streamstypes.h"

InFileRTMPMP4Stream::InFileRTMPMP4Stream(BaseProtocol *pProtocol,
		StreamsManager *pStreamsManager, string name)
: InFileRTMPStream(pProtocol, pStreamsManager, ST_IN_FILE_RTMP_MP4, name) {
	_videoCodecHeaderInit[0] = 0x17;
	_videoCodecHeaderInit[1] = 0;
	_videoCodecHeaderInit[2] = 0;
	_videoCodecHeaderInit[3] = 0;
	_videoCodecHeaderInit[4] = 0;

	_videoCodecHeaderKeyFrame[0] = 0x17;
	_videoCodecHeaderKeyFrame[1] = 1;

	_videoCodecHeader[0] = 0x27;
	_videoCodecHeader[1] = 1;

	_audioCodecHeaderInit[0] = 0xaf;
	_audioCodecHeaderInit[1] = 0;

	_audioCodecHeader[0] = 0xaf;
	_audioCodecHeader[1] = 0x01;
}

InFileRTMPMP4Stream::~InFileRTMPMP4Stream() {
}

bool InFileRTMPMP4Stream::BuildFrame(FileClass *pFile, MediaFrame &mediaFrame,
		IOBuffer &buffer) {
	//1. add the binary header
	if (mediaFrame.isBinaryHeader) {
		if (mediaFrame.type == MEDIAFRAME_TYPE_AUDIO) {
			// audio extra data
			buffer.ReadFromBuffer(_audioCodecHeaderInit, sizeof (_audioCodecHeaderInit));
		} else {
			// video extra data
			buffer.ReadFromBuffer(_videoCodecHeaderInit, sizeof (_videoCodecHeaderInit));
		}
	} else {
		if (mediaFrame.type == MEDIAFRAME_TYPE_AUDIO) {
			buffer.ReadFromBuffer(_audioCodecHeader, sizeof (_audioCodecHeader));
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

bool InFileRTMPMP4Stream::FeedMetaData(FileClass *pFile, MediaFrame &mediaFrame) {
	NYI;
	return true;
}
#endif /* HAS_PROTOCOL_RTMP */

