/*
 *  Copyright (c) 2010,
 *  EvoStream (contact@evostream.com)
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
#include "protocols/rtmp/streaming/infilertmpnsvstream.h"
#include "streaming/streamstypes.h"
#include "protocols/rtmp/messagefactories/streammessagefactory.h"
#include "protocols/rtmp/basertmpprotocol.h"
#include "protocols/rtmp/streaming/baseoutnetrtmpstream.h"
#include "streaming/nalutypes.h"

#define SPSPPS_MAX_LENGTH 1024

InFileRTMPNSVStream::InFileRTMPNSVStream(BaseProtocol *pProtocol,
		StreamsManager *pStreamsManager, string name)
: InFileRTMPStream(pProtocol, pStreamsManager, ST_IN_FILE_RTMP_NSV, name) {

	_videoCodecHeaderKeyFrame[0] = 0x17;
	_videoCodecHeaderKeyFrame[1] = 1;

	_videoCodecHeader[0] = 0x27;
	_videoCodecHeader[1] = 1;

	_spsAvailable = false;
	_pSPSPPS = new uint8_t[SPSPPS_MAX_LENGTH];
	_SPSPPSLength = 0;
	_PPSStart = 0;

	_pSPSPPS[0] = 0x17; //0x10 - key frame; 0x07 - H264_CODEC_ID
	_pSPSPPS[1] = 0; //0: AVC sequence header; 1: AVC NALU; 2: AVC end of sequence
	_pSPSPPS[2] = 0; //CompositionTime
	_pSPSPPS[3] = 0; //CompositionTime
	_pSPSPPS[4] = 0; //CompositionTime
	_pSPSPPS[5] = 0x01; //version
	_pSPSPPS[9] = 0xff; //6 bits reserved (111111) + 2 bits nal size length - 1 (11)
	_pSPSPPS[10] = 0xe1; //3 bits reserved (111) + 5 bits number of sps (00001)


}

InFileRTMPNSVStream::~InFileRTMPNSVStream() {

}

bool InFileRTMPNSVStream::BuildFrame(FileClass *pFile, MediaFrame &mediaFrame,
		IOBuffer &buffer) {


	if (mediaFrame.type == MEDIAFRAME_TYPE_AUDIO) {
		// MP3 Audio
		buffer.ReadFromRepeat(0x2f, 1);

		//1. Seek into the data file at the correct position
		if (!pFile->SeekTo(mediaFrame.start)) {
			FATAL("Unable to seek to position %"PRIu64, mediaFrame.start);
			return false;
		}

		//3. Read the data
		if (!buffer.ReadFromFs(*pFile, (uint32_t) mediaFrame.length)) {
			FATAL("Unable to read %"PRIu64" bytes from offset %"PRIu64, mediaFrame.length, mediaFrame.start);
			return false;
		}

	} else {
		if (mediaFrame.isBinaryHeader) {
			if (!BuildFrameHeaders(pFile, mediaFrame, buffer)) {
				FATAL("Unable to build frame header...");
				return false;
			}
		} else {
			if (mediaFrame.isKeyFrame) {
				// video key frame
				buffer.ReadFromBuffer(_videoCodecHeaderKeyFrame, sizeof (_videoCodecHeaderKeyFrame));
			} else {
				//video normal frame
				buffer.ReadFromBuffer(_videoCodecHeader, sizeof (_videoCodecHeader));
			}


			//composition timestamp is always 0 for NSV
			buffer.ReadFromRepeat(0, 3);
			// need help here...
			buffer.ReadFromRepeat(0, 4);
			uint32_t frameLength=0;
			EHTONLP((GETIBPOINTER(buffer) + 5), frameLength);
			mediaFrame.length=frameLength;

			//1. Seek into the data file at the correct position
			if (!pFile->SeekTo(mediaFrame.start)) {
				FATAL("Unable to seek to position %"PRIu64, mediaFrame.start);
				return false;
			}

			//3. Read the data
			if (!buffer.ReadFromFs(*pFile, (uint32_t) mediaFrame.length)) {
				FATAL("Unable to read %"PRIu64" bytes from offset %"PRIu64, mediaFrame.length, mediaFrame.start);
				return false;
			}

		}
	}

	//3. Done
	return true;
}

bool InFileRTMPNSVStream::FeedMetaData(FileClass *pFile, MediaFrame & mediaFrame) {
	NYI;
	return true;
}

bool InFileRTMPNSVStream::BuildFrameHeaders(FileClass *pFile, MediaFrame &mediaFrame,
		IOBuffer &buffer) {

	//build codec bytes

	//1. Seek into the data file at the correct position
	if (!pFile->SeekTo(mediaFrame.start)) {
		FATAL("Unable to seek to position %"PRIu64, mediaFrame.start);
		return false;
	}

	IOBuffer pbuf;
	if (!pbuf.ReadFromFs(*pFile, (uint32_t) mediaFrame.length)) {
		FATAL("Unable to read %"PRIu64" bytes from offset %"PRIu64, mediaFrame.length, mediaFrame.start);
		return false;
	}

	uint8_t *pData = GETIBPOINTER(pbuf);
	if(mediaFrame.length>=0x100000000LL){
		FATAL("Invalid frame length");
		return false;
	}
	uint32_t dataLength = (uint32_t)mediaFrame.length;

	switch (NALU_TYPE(pData[0])) {
		case NALU_TYPE_SPS:
		{
			//1. Prepare the SPS part from video codec
			if (dataLength > 128) {
				FATAL("SPS too big");
				return false;
			}
			memcpy(_pSPSPPS + 6, pData + 1, 3); //profile,profile compat,level
			EHTONSP(_pSPSPPS + 11, (uint16_t) dataLength);
			memcpy(_pSPSPPS + 13, pData, dataLength);
			_PPSStart = 13 + dataLength;
			_spsAvailable = true;
			_SPSPPSLength = _PPSStart;

			return true;
		}

		case NALU_TYPE_PPS:
		{
			//2. Prepare the PPS part from video codec
			if (dataLength > 128) {
				FATAL("PPS too big");
				return false;
			}
			if (!_spsAvailable) {
				WARN("No SPS available yet");
				return true;
			}
			_pSPSPPS[_PPSStart] = 1;
			EHTONSP(_pSPSPPS + _PPSStart + 1, (uint16_t) dataLength);
			memcpy(_pSPSPPS + _PPSStart + 1 + 2, pData, dataLength);
			_SPSPPSLength = _PPSStart + 1 + 2 + dataLength;
			_spsAvailable = false;

			buffer.ReadFromBuffer(_pSPSPPS, _SPSPPSLength); //sizeof (_pSPSPPS));

			return true;
		}
	}
	return true;
}
#endif /* HAS_PROTOCOL_RTMP */

