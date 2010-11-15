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

#define SPSPPS_MAX_LENGTH 1024

OutNetRTMP4TSStream::OutNetRTMP4TSStream(BaseProtocol *pProtocol,
		StreamsManager *pStreamsManager, string name, uint32_t rtmpStreamId,
		uint32_t chunkSize)
: BaseOutNetRTMPStream(pProtocol, pStreamsManager, ST_OUT_NET_RTMP_4_TS,
name, rtmpStreamId, chunkSize) {
	_audioCodecSent = false;
	_videoCodecSent = false;
	_spsAvailable = false;
	_pSPSPPS = new uint8_t[SPSPPS_MAX_LENGTH];
	_SPSPPSLength = 0;
	_PPSStart = 0;
	CanDropFrames(false);


	_pSPSPPS[0] = 0x17; //0x10 - key frame; 0x07 - H264_CODEC_ID
	_pSPSPPS[1] = 0; //0: AVC sequence header; 1: AVC NALU; 2: AVC end of sequence
	_pSPSPPS[2] = 0; //CompositionTime
	_pSPSPPS[3] = 0; //CompositionTime
	_pSPSPPS[4] = 0; //CompositionTime
	_pSPSPPS[5] = 1; //version
	_pSPSPPS[9] = 0xff; //6 bits reserved (111111) + 2 bits nal size length - 1 (11)
	_pSPSPPS[10] = 0xe1; //3 bits reserved (111) + 5 bits number of sps (00001)
}

OutNetRTMP4TSStream::~OutNetRTMP4TSStream() {
	delete[] _pSPSPPS;
}

bool OutNetRTMP4TSStream::IsCompatibleWithType(uint64_t type) {
	return TAG_KIND_OF(type, ST_IN_NET_TS)
			|| TAG_KIND_OF(type, ST_IN_NET_RTP);
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
	if (!_videoCodecSent)
		return true;
	//the payload here respects this format:
	//6.2  Audio Data Transport Stream, ADTS
	//iso13818-7 page 26/206

	//1. Send the audio codec setup if necessary
	if (!_audioCodecSent) {
		StreamCapabilities *pCapabilities = GetCapabilities();
		if ((pCapabilities != NULL)
				&& (pCapabilities->audioCodecId == CODEC_AUDIO_AAC)) {
			IOBuffer codecSetup;
			codecSetup.ReadFromRepeat(0xaf, 1);
			codecSetup.ReadFromRepeat(0x00, 1);
			codecSetup.ReadFromBuffer(pCapabilities->aac._pAAC,
					pCapabilities->aac._aacLength);

			if (!BaseOutNetRTMPStream::FeedData(
					GETIBPOINTER(codecSetup), //pData
					GETAVAILABLEBYTESCOUNT(codecSetup), //dataLength
					0, //processedLength
					GETAVAILABLEBYTESCOUNT(codecSetup), //totalLength
					absoluteTimestamp, //absoluteTimestamp
					true //isAudio
					)) {
				FATAL("Unable to send audio codec setup");
				return false;
			}
		}

		_audioCodecSent = true;
	}

	//2. Skip the ADTS headers and re-position the buffer
	uint32_t totalADTSHeaderLength = 0;
	if (((pData[1])&0x01) == 0)
		totalADTSHeaderLength = 9;
	else
		totalADTSHeaderLength = 7;
	pData += totalADTSHeaderLength - 2;

	//3. Setup the RTMP header
	pData[0] = 0xaf;
	pData[1] = 0x01;

	//4. Feed
	return BaseOutNetRTMPStream::FeedData(
			pData, //pData
			dataLength - totalADTSHeaderLength + 2, //dataLength
			0, //processedLength
			dataLength - totalADTSHeaderLength + 2, //totalLength
			absoluteTimestamp, //absoluteTimestamp
			true //isAudio
			);
}

bool OutNetRTMP4TSStream::FeedVideoData(uint8_t *pData, uint32_t dataLength,
		double absoluteTimestamp) {
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
			_spsAvailable = false;

			//3. Send the video codec
			if (!BaseOutNetRTMPStream::FeedData(
					_pSPSPPS, //pData
					_PPSStart + 1 + 2 + dataLength, //dataLength
					0, //processedLength
					_PPSStart + 1 + 2 + dataLength, //totalLength
					absoluteTimestamp, //absoluteTimestamp
					false //isAudio
					)) {
				FATAL("Unable to send video codec setup");
				return false;
			}

			_videoCodecSent = true;

			return true;
		}
		case NALU_TYPE_IDR:
		case NALU_TYPE_SLICE:
		{
			//10. Make room for the RTMP header
			_videoBuffer.ReadFromRepeat(0, 9);

			//11. Add the raw data
			_videoBuffer.ReadFromBuffer(pData, dataLength);

			uint8_t *pBuffer = GETIBPOINTER(_videoBuffer);

			//12. Setup the RTMP header
			pBuffer[0] = (NALU_TYPE(pData[0]) == NALU_TYPE_IDR) ? 0x17 : 0x27;
			pBuffer[1] = 0x01;
			pBuffer[2] = pBuffer[3] = pBuffer[4] = 0;
			EHTONLP(pBuffer + 5, dataLength); //----MARKED-LONG---

			//13. Send it
			if (!BaseOutNetRTMPStream::FeedData(
					pBuffer, //pData
					dataLength + 9, //dataLength
					0, //processedLength
					dataLength + 9, //totalLength
					absoluteTimestamp, //absoluteTimestamp
					false //isAudio
					)) {
				FATAL("Unable to send video");
				return false;
			}

			//14. Cleanup
			_videoBuffer.IgnoreAll();

			return true;
		}
		case NALU_TYPE_PD:
		case NALU_TYPE_SEI:
		case NALU_TYPE_FILL:
		{
			return true;
		}
		default:
		{
			WARN("Ignoring NAL: %s", STR(NALUToString(pData[0])));
			return true;
		}
	}
}

Variant OutNetRTMP4TSStream::SPSToVariant() {
	//	Variant result;
	//	BitArray ba;
	//	ba.ReadFromBuffer(GETIBPOINTER(_SPS), GETAVAILABLEBYTESCOUNT(_SPS));
	//	ba.ReadBits<uint8_t > (8);
	//	result["00_profile_idc"] = ba.ReadBits<uint8_t > (8);
	//	result["01_constraint_set0_flag"] = ba.ReadBits<bool>(1);
	//	result["02_constraint_set1_flag"] = ba.ReadBits<bool>(1);
	//	result["03_constraint_set2_flag"] = ba.ReadBits<bool>(1);
	//	result["04_reserved_zero_5bits"] = ba.ReadBits<uint8_t > (5);
	//	result["05_level_idc"] = ba.ReadBits<uint8_t > (8);
	//	result["06_seq_parameter_set_id"] = ba.ReadExpGolomb();
	//	result["07_log2_max_frame_num_minus4"] = ba.ReadExpGolomb();
	//	result["08_pic_order_cnt_type"] = ba.ReadExpGolomb();
	//	if ((uint32_t) result["08_pic_order_cnt_type"] == 0) {
	//		result["09_log2_max_pic_order_cnt_lsb_minus4"] = ba.ReadExpGolomb();
	//	} else if ((uint32_t) result["08_pic_order_cnt_type"] == 1) {
	//		result["10_delta_pic_order_always_zero_flag"] = ba.ReadBits<bool>(1);
	//		result["11_offset_for_non_ref_pic"] = (int32_t) ba.ReadExpGolomb();
	//		result["12_offset_for_top_to_bottom_field"] = (int32_t) ba.ReadExpGolomb();
	//		result["13_num_ref_frames_in_pic_order_cnt_cycle"] = ba.ReadExpGolomb();
	//		for (uint32_t i = 0; i < (uint32_t) result["13_num_ref_frames_in_pic_order_cnt_cycle"]; i++)
	//			result["14_offset_for_ref_frame"].PushToArray((int32_t) ba.ReadExpGolomb());
	//	}
	//	result["15_num_ref_frames"] = ba.ReadExpGolomb();
	//	result["16_gaps_in_frame_num_value_allowed_flag"] = ba.ReadBits<bool>(1);
	//	result["17_pic_width_in_mbs_minus1"] = ba.ReadExpGolomb();
	//	result["18_pic_height_in_map_units_minus1"] = ba.ReadExpGolomb();
	//	result["19_frame_mbs_only_flag"] = ba.ReadBits<bool>(1);
	//	if (!(bool)result["19_frame_mbs_only_flag"])
	//		result["20_mb_adaptive_frame_field_flag"] = ba.ReadBits<bool>(1);
	//	result["21_direct_8x8_inference_flag"] = ba.ReadBits<bool>(1);
	//	result["22_frame_cropping_flag"] = ba.ReadBits<bool>(1);
	//	if ((bool)result["22_frame_cropping_flag"]) {
	//		result["23_frame_crop_left_offset"] = ba.ReadExpGolomb();
	//		result["24_frame_crop_right_offset"] = ba.ReadExpGolomb();
	//		result["25_frame_crop_top_offset"] = ba.ReadExpGolomb();
	//		result["26_frame_crop_bottom_offset"] = ba.ReadExpGolomb();
	//	}
	//	result["27_vui_parameters_present_flag"] = ba.ReadBits<bool>(1);
	//	//if( (bool)result["00_vui_parameters_present_flag"] )
	//	//vui_parameters( )
	//	//rbsp_trailing_bits( )
	//
	//	return result;
	return Variant();
}

Variant OutNetRTMP4TSStream::PPSToVariant() {
	Variant result;
	return result;
}
#endif /* HAS_PROTOCOL_RTMP */

