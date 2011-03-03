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
		//FINEST("--- AUDIO DATA");

		// MP3 Audio
		buffer.ReadFromRepeat(0x2f, 1);

		//1. Seek into the data file at the correct position
		if (!pFile->SeekTo(mediaFrame.start)) {
			FATAL("Unable to seek to position %llu", mediaFrame.start);
			return false;
		}

		//3. Read the data
		if (!buffer.ReadFromFs(*pFile, (uint32_t) mediaFrame.length)) {
			FATAL("Unable to read %llu bytes from offset %llu", mediaFrame.length, mediaFrame.start);
			return false;
		}

	} else {
		if (mediaFrame.isBinaryHeader) {
			//FINEST("--- VIDEO EXTRA DATA");

			if (!BuildFrameHeaders(pFile, mediaFrame, buffer)) {
				FATAL("Unable to build frame header...");
				return false;
			}
			//FINEST("sizeof spspps:%d", sizeof (_pSPSPPS));
			//FINEST("frame Headers:\n%s", STR(buffer));
		} else {
			if (mediaFrame.isKeyFrame) {
				// video key frame
				//FINEST("--- VIDEO KEY FRAME");
				buffer.ReadFromBuffer(_videoCodecHeaderKeyFrame, sizeof (_videoCodecHeaderKeyFrame));
			} else {
				//video normal frame
				//            string str = format("%02x %02x %02x %02x %02x",
				//                    GETIBPOINTER(buffer)[0],
				//
				//FINEST("--- VIDEO NORMAL FRAME");
				buffer.ReadFromBuffer(_videoCodecHeader, sizeof (_videoCodecHeader));
			}


			//composition timestamp is always 0 for NSV
			buffer.ReadFromRepeat(0, 3);
			//FINEST("length : %08x", mediaFrame.length);
			// need help here...
			buffer.ReadFromRepeat(0, 4);
			EHTONLP((GETIBPOINTER(buffer) + 5), mediaFrame.length);

			//buffer.ReadFromBuffer((uint8_t *) &mediaFrame.length, 4);
			//            string str = format("%02x %02x %02x %02x %02x",
			//                    GETIBPOINTER(buffer)[0],
			//                    GETIBPOINTER(buffer)[1],
			//                    GETIBPOINTER(buffer)[2],
			//                    GETIBPOINTER(buffer)[3],
			//                    GETIBPOINTER(buffer)[4]);
			//            if (_currentFrame.isKeyFrame)
			//                WARN("%s", STR(str));
			//            else
			//                FINEST("%s", STR(str));

			/*if (mediaFrame.type == MEDIAFRAME_TYPE_VIDEO) {
				FINEST("Media frame:\n%s", STR(mediaFrame));
				FINEST("Headers:\n%s", STR(buffer));
			}
			 */

			//1. Seek into the data file at the correct position
			if (!pFile->SeekTo(mediaFrame.start)) {
				FATAL("Unable to seek to position %llu", mediaFrame.start);
				return false;
			}

			//3. Read the data
			if (!buffer.ReadFromFs(*pFile, (uint32_t) mediaFrame.length)) {
				FATAL("Unable to read %llu bytes from offset %llu", mediaFrame.length, mediaFrame.start);
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
		FATAL("Unable to seek to position %llu", mediaFrame.start);
		return false;
	}

	IOBuffer pbuf;
	if (!pbuf.ReadFromFs(*pFile, (uint32_t) mediaFrame.length)) {
		FATAL("Unable to read %llu bytes from offset %llu", mediaFrame.length, mediaFrame.start);
		return false;
	}
	//FINEST("video data offset %x", mediaFrame.start);
	//FINEST("data len %d", mediaFrame.length);
	//FINEST(" buffer: \n%s", STR(pbuf));

	uint8_t *pData = GETIBPOINTER(pbuf);
	uint32_t dataLength = mediaFrame.length;
	//FINEST("%02x - %s", (uint8_t) NALU_TYPE(pData[0]), STR(NALUToString((uint8_t) pData[0])));

	switch (NALU_TYPE(pData[0])) {
		case NALU_TYPE_SPS:
		{
			//FINEST("WE GOT SPS");
			//1. Prepare the SPS part from video codec
			//FINEST("spspps length: %d ppsstrt:%d", _SPSPPSLength, _PPSStart);
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
			//FINEST("spspps length: %d ppsstrt:%d", _SPSPPSLength, _PPSStart);

			return true;
		}

		case NALU_TYPE_PPS:
		{
			//FINEST("WE GOT PPS");
			//2. Prepare the PPS part from video codec
			if (dataLength > 128) {
				FATAL("PPS too big");
				return false;
			}
			if (!_spsAvailable) {
				WARN("No SPS available yet");
				return true;
			}
			//FINEST("spspps length: %d ppsstrt:%d", _SPSPPSLength, _PPSStart);
			_pSPSPPS[_PPSStart] = 1;
			EHTONSP(_pSPSPPS + _PPSStart + 1, (uint16_t) dataLength);
			memcpy(_pSPSPPS + _PPSStart + 1 + 2, pData, dataLength);
			_SPSPPSLength = _PPSStart + 1 + 2 + dataLength;
			_spsAvailable = false;

			//FINEST("spspps length: %d", _SPSPPSLength);
			buffer.ReadFromBuffer(_pSPSPPS, _SPSPPSLength); //sizeof (_pSPSPPS));

			return true;
		}
	}
	return true;
}
#endif /* HAS_PROTOCOL_RTMP */

