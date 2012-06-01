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
#include "protocols/rtmp/streaming/outnetrtmp4tsstream.h"
#include "streaming/streamstypes.h"
#include "streaming/nalutypes.h"
#include "protocols/http/basehttpprotocol.h"

#define SPSPPS_MAX_LENGTH 1024

OutNetRTMP4TSStream::OutNetRTMP4TSStream(BaseRTMPProtocol *pProtocol,
		StreamsManager *pStreamsManager, string name, uint32_t rtmpStreamId,
		uint32_t chunkSize)
: BaseOutNetRTMPStream(pProtocol, pStreamsManager, ST_OUT_NET_RTMP_4_TS,
name, rtmpStreamId, chunkSize) {
	_audioCodecSent = false;
	_audioIsG711 = false;
	_videoCodecSent = false;
	CanDropFrames(false);

	_inboundStreamIsRTP = false;
	_lastVideoTimestamp = -1;
	_isKeyFrame = false;
}

OutNetRTMP4TSStream::~OutNetRTMP4TSStream() {

}

bool OutNetRTMP4TSStream::IsCompatibleWithType(uint64_t type) {
	_inboundStreamIsRTP = TAG_KIND_OF(type, ST_IN_NET_RTP) || TAG_KIND_OF(type, ST_IN_NET_EXT);
	_videoCodecSent = (type == ST_IN_NET_AAC);
	return TAG_KIND_OF(type, ST_IN_NET_TS)
			|| TAG_KIND_OF(type, ST_IN_NET_RTP)
			|| TAG_KIND_OF(type, ST_IN_NET_AAC)
			|| TAG_KIND_OF(type, ST_IN_NET_EXT);
}

bool OutNetRTMP4TSStream::FeedData(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp, bool isAudio) {
	if (isAudio)
		return FeedAudioData(pData, dataLength, absoluteTimestamp);
	else
		return FeedVideoData(pData, dataLength, absoluteTimestamp);
}

bool OutNetRTMP4TSStream::FeedAudioData(uint8_t *pData, uint32_t dataLength,
		double absoluteTimestamp) {
	//the payload here respects this format:
	//6.2  Audio Data Transport Stream, ADTS
	//iso13818-7 page 26/206

	//1. Send the audio codec setup if necessary
	if (!_audioCodecSent) {
		if (!SendAudioCodec(absoluteTimestamp)) {
			FATAL("Unable to send video codec");
			return false;
		}
	}

	if (_audioIsG711) {
		return BaseOutNetRTMPStream::FeedData(
				pData, //pData
				dataLength, //dataLength
				0, //processedLength
				dataLength, //totalLength
				absoluteTimestamp, //absoluteTimestamp
				true //isAudio
				);
	} else {
		if (_inboundStreamIsRTP) {
			pData[0] = 0xaf;
			pData[1] = 0x01;
			return BaseOutNetRTMPStream::FeedData(
					pData, //pData
					dataLength, //dataLength
					0, //processedLength
					dataLength, //totalLength
					absoluteTimestamp, //absoluteTimestamp
					true //isAudio
					);
		} else {
			//2. Skip the ADTS headers and re-position the buffer
			uint32_t totalADTSHeaderLength = 0;
			if (((pData[1])&0x01) == 0)
				totalADTSHeaderLength = 9;
			else
				totalADTSHeaderLength = 7;
			pData += totalADTSHeaderLength - 2;

			//3. Setup the RTMP header
			uint8_t b1 = pData[0];
			uint8_t b2 = pData[1];
			pData[0] = 0xaf;
			pData[1] = 0x01;

			//4. Feed
			if (!BaseOutNetRTMPStream::FeedData(
					pData, //pData
					dataLength - totalADTSHeaderLength + 2, //dataLength
					0, //processedLength
					dataLength - totalADTSHeaderLength + 2, //totalLength
					absoluteTimestamp, //absoluteTimestamp
					true //isAudio
					)) {
				FATAL("BaseOutNetRTMPStream::FeedData failed");
				return false;
			}
			pData[0] = b1;
			pData[1] = b2;
			return true;
		}
	}
}

//void checkData(IOBuffer &buffer);

bool OutNetRTMP4TSStream::FeedVideoData(uint8_t *pData, uint32_t dataLength,
		double absoluteTimestamp) {
	uint8_t nalType = NALU_TYPE(pData[0]);

	//1. Create timestamp reference
	if (_lastVideoTimestamp < 0)
		_lastVideoTimestamp = absoluteTimestamp;

	//2. Send over the accumulated stuff if this is a new packet from a
	//brand new sequence of packets
	if (_lastVideoTimestamp != absoluteTimestamp) {
		if (!_videoCodecSent) {
			if (!SendVideoCodec(_lastVideoTimestamp)) {
				FATAL("Unable to send video codec");
				return false;
			}
		}
		if (!BaseOutNetRTMPStream::FeedData(
				GETIBPOINTER(_videoBuffer), //pData
				GETAVAILABLEBYTESCOUNT(_videoBuffer), //dataLength
				0, //processedLength
				GETAVAILABLEBYTESCOUNT(_videoBuffer), //totalLength
				_lastVideoTimestamp, //absoluteTimestamp
				false //isAudio
				)) {
			FATAL("Unable to send video");
			return false;
		}
		_videoBuffer.IgnoreAll();
		_isKeyFrame = false;
	}
	_lastVideoTimestamp = absoluteTimestamp;

	uint8_t *pTemp = NULL;

	//put the 5 bytes header
	if (GETAVAILABLEBYTESCOUNT(_videoBuffer) == 0) {
		_videoBuffer.ReadFromRepeat(0, 5);
		pTemp = GETIBPOINTER(_videoBuffer);
		pTemp[1] = 0x01;
		pTemp[2] = pTemp[3] = pTemp[4] = 0;
	}

	if ((nalType == NALU_TYPE_IDR)
			|| (nalType == NALU_TYPE_SLICE)
			|| (nalType == NALU_TYPE_SEI)
			) {
		//put the length
		_videoBuffer.ReadFromRepeat(0, 4);
		pTemp = GETIBPOINTER(_videoBuffer) + GETAVAILABLEBYTESCOUNT(_videoBuffer) - 4;
		EHTONLP(pTemp, dataLength);

		//put the data
		_videoBuffer.ReadFromBuffer(pData, dataLength);

		//setup the frame type
		_isKeyFrame |= (nalType == NALU_TYPE_IDR);
		if (_isKeyFrame) {
			GETIBPOINTER(_videoBuffer)[0] = 0x17;
		} else {
			GETIBPOINTER(_videoBuffer)[0] = 0x27;
		}
	}

	//6. make sure the packet doesn't grow too big
	if (GETAVAILABLEBYTESCOUNT(_videoBuffer) >= 4 * 1024 * 1024) {
		WARN("Big video frame. Discard it");
		_videoBuffer.IgnoreAll();
		_isKeyFrame = false;
		_lastVideoTimestamp = -1;
	}

	//done
	return true;
}

bool OutNetRTMP4TSStream::SendVideoCodec(double absoluteTimestamp) {
	StreamCapabilities *pCapabilities = GetCapabilities();
	if (pCapabilities == NULL || pCapabilities->videoCodecId != CODEC_VIDEO_AVC) {
		return true;
	}

	IOBuffer result;

	result.ReadFromByte(0x17); //0x10 - key frame; 0x07 - H264_CODEC_ID
	result.ReadFromByte(0); //0: AVC sequence header; 1: AVC NALU; 2: AVC end of sequence
	result.ReadFromByte(0); //CompositionTime
	result.ReadFromByte(0); //CompositionTime
	result.ReadFromByte(0); //CompositionTime
	result.ReadFromByte(1); //version
	result.ReadFromBuffer(pCapabilities->avc._pSPS + 1, 3); //profile,profile compat,level
	result.ReadFromByte(0xff); //6 bits reserved (111111) + 2 bits nal size length - 1 (11)
	result.ReadFromByte(0xe1); //3 bits reserved (111) + 5 bits number of sps (00001)
	uint16_t temp16 = EHTONS(pCapabilities->avc._spsLength);
	result.ReadFromBuffer((uint8_t *) & temp16, 2); //SPS length
	result.ReadFromBuffer(pCapabilities->avc._pSPS, pCapabilities->avc._spsLength);


	result.ReadFromByte(1);
	temp16 = EHTONS(pCapabilities->avc._ppsLength);
	result.ReadFromBuffer((uint8_t *) & temp16, 2); //PPS length
	result.ReadFromBuffer(pCapabilities->avc._pPPS, pCapabilities->avc._ppsLength);


	//3. Send the video codec
	if (!BaseOutNetRTMPStream::FeedData(
			GETIBPOINTER(result), //pData
			GETAVAILABLEBYTESCOUNT(result), //dataLength
			0, //processedLength
			GETAVAILABLEBYTESCOUNT(result), //totalLength
			absoluteTimestamp, //absoluteTimestamp
			false //isAudio
			)) {
		FATAL("Unable to send video codec setup");
		return false;
	}

	_videoCodecSent = true;

	return true;
}

bool OutNetRTMP4TSStream::SendAudioCodec(double absoluteTimestamp) {
	StreamCapabilities *pCapabilities = GetCapabilities();
	if ((pCapabilities == NULL)
			|| ((pCapabilities->audioCodecId != CODEC_AUDIO_AAC)
			&& (pCapabilities->audioCodecId != CODEC_AUDIO_G711))) {
		return true;
	}
	if (pCapabilities->audioCodecId == CODEC_AUDIO_G711) {
		_audioCodecSent = true;
		_audioIsG711 = true;
		return true;
	}
	IOBuffer result;
	result.ReadFromRepeat(0xaf, 1);
	result.ReadFromRepeat(0x00, 1);
	result.ReadFromBuffer(pCapabilities->aac._pAAC,
			pCapabilities->aac._aacLength);

	if (!BaseOutNetRTMPStream::FeedData(
			GETIBPOINTER(result), //pData
			GETAVAILABLEBYTESCOUNT(result), //dataLength
			0, //processedLength
			GETAVAILABLEBYTESCOUNT(result), //totalLength
			absoluteTimestamp, //absoluteTimestamp
			true //isAudio
			)) {
		FATAL("Unable to send audio codec setup");
		return false;
	}

	_audioCodecSent = true;
	_audioIsG711 = false;
	return true;
}
#endif /* HAS_PROTOCOL_RTMP */

