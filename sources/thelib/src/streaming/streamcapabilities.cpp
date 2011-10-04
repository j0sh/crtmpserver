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


#include "streaming/streamcapabilities.h"

_VIDEO_AVC::_VIDEO_AVC() {
	_pSPS = NULL;
	_spsLength = 0;
	_pPPS = NULL;
	_ppsLength = 0;
	_width = 0;
	_height = 0;
	_widthOverride = 0;
	_heightOverride = 0;
}

_VIDEO_AVC::~_VIDEO_AVC() {
	Clear();
}

//#define DUMP_VAL(name,type,length) if(length!=0) FINEST("%8s %50s: %4.0f; l: %2u; ba: %4u",#type,name,(double)v[name],length,ba.AvailableBits()); else WARN("%8s %50s: %4.0f; l: %2u; ba: %4u",#type,name,(double)v[name],length,ba.AvailableBits());
#define DUMP_VAL(name,type,length)

#define CHECK_BA_LIMITS(name,length) \
if(ba.AvailableBits()<length) { \
	FATAL("Unable to read `"name"` value. Not enough bits. Wanted: %u; Have: %u", \
		(uint32_t)length, ba.AvailableBits()); \
	return false; \
}

#define READ_INT(name,type,length) \
{ \
	CHECK_BA_LIMITS(name,length); \
	v[name]=(type)ba.ReadBits<type>(length); \
	DUMP_VAL(name,type,length); \
}
#define READ_EG(name,type) \
{ \
	uint64_t ___value___=0; \
	if(!ba.ReadExpGolomb(___value___)) { \
		FATAL("Unable to read `"name"` value"); \
		return false; \
	} \
	v[name]=(type)___value___; \
	DUMP_VAL(name,type,0); \
}

#define READ_BOOL(name) \
{ \
	CHECK_BA_LIMITS(name,1); \
	v[name]=(bool)(ba.ReadBits<bool>(1)); \
	DUMP_VAL(name,bool,1); \
}

bool ReadSPSVUIHRD(BitArray &ba, Variant &v) {
	//E.1.2 HRD parameters syntax
	//14496-10.pdf 268/280
	READ_EG("cpb_cnt_minus1", uint64_t);
	READ_INT("bit_rate_scale", uint8_t, 4);
	READ_INT("cpb_size_scale", uint8_t, 4);
	for (uint64_t i = 0; i <= (uint64_t) v["cpb_cnt_minus1"]; i++) {
		uint64_t val = 0;
		if (!ba.ReadExpGolomb(val)) {
			FATAL("Unable to read bit_rate_value_minus1 value");
			return false;
		}
		v["bit_rate_value_minus1"].PushToArray(val);

		if (!ba.ReadExpGolomb(val)) {
			FATAL("Unable to read cpb_size_value_minus1 value");
			return false;
		}
		v["cpb_size_value_minus1"].PushToArray(val);

		CHECK_BA_LIMITS("cbr_flag", 1);
		v["cbr_flag"].PushToArray((bool)ba.ReadBits<bool>(1));
	}
	READ_INT("initial_cpb_removal_delay_length_minus1", uint8_t, 5);
	READ_INT("cpb_removal_delay_length_minus1", uint8_t, 5);
	READ_INT("dpb_output_delay_length_minus1", uint8_t, 5);
	READ_INT("time_offset_length", uint8_t, 5);
	return true;
}

bool ReadSPSVUI(BitArray &ba, Variant &v) {
	//E.1.1 VUI parameters syntax
	//14496-10.pdf 267/280
	READ_BOOL("aspect_ratio_info_present_flag");
	if ((bool)v["aspect_ratio_info_present_flag"]) {
		READ_INT("aspect_ratio_idc", uint8_t, 8);
		if ((uint8_t) v["aspect_ratio_idc"] == 255) {
			READ_INT("sar_width", uint16_t, 16);
			READ_INT("sar_height", uint16_t, 16);
		}
	}
	READ_BOOL("overscan_info_present_flag");
	if ((bool)v["overscan_info_present_flag"])
		READ_BOOL("overscan_appropriate_flag");
	READ_BOOL("video_signal_type_present_flag");
	if ((bool)v["video_signal_type_present_flag"]) {
		READ_INT("video_format", uint8_t, 3);
		READ_BOOL("video_full_range_flag");
		READ_BOOL("colour_description_present_flag");
		if ((bool)v["colour_description_present_flag"]) {
			READ_INT("colour_primaries", uint8_t, 8);
			READ_INT("transfer_characteristics", uint8_t, 8);
			READ_INT("matrix_coefficients", uint8_t, 8);
		}
	}
	READ_BOOL("chroma_loc_info_present_flag");
	if ((bool)v["chroma_loc_info_present_flag"]) {
		READ_EG("chroma_sample_loc_type_top_field", uint64_t);
		READ_EG("chroma_sample_loc_type_bottom_field", uint64_t);
	}
	READ_BOOL("timing_info_present_flag");
	if ((bool)v["timing_info_present_flag"]) {
		READ_INT("num_units_in_tick", uint32_t, 32);
		READ_INT("time_scale", uint32_t, 32);
		READ_BOOL("fixed_frame_rate_flag");
	}
	READ_BOOL("nal_hrd_parameters_present_flag");
	if ((bool)v["nal_hrd_parameters_present_flag"]) {
		if (!ReadSPSVUIHRD(ba, v["nal_hrd"])) {
			FATAL("Unable to read VUIHRD");
			return false;
		}
	}
	READ_BOOL("vcl_hrd_parameters_present_flag");
	if ((bool)v["vcl_hrd_parameters_present_flag"]) {
		if (!ReadSPSVUIHRD(ba, v["vcl_hrd"])) {
			FATAL("Unable to read VUIHRD");
			return false;
		}
	}
	if (((bool)v["nal_hrd_parameters_present_flag"])
			|| ((bool)v["vcl_hrd_parameters_present_flag"]))
		READ_BOOL("low_delay_hrd_flag");
	READ_BOOL("pic_struct_present_flag");
	READ_BOOL("bitstream_restriction_flag");
	if ((bool)v["bitstream_restriction_flag"]) {
		READ_BOOL("motion_vectors_over_pic_boundaries_flag");
		READ_EG("max_bytes_per_pic_denom", uint64_t);
		READ_EG("max_bits_per_mb_denom", uint64_t);
		READ_EG("log2_max_mv_length_horizontal", uint64_t);
		READ_EG("log2_max_mv_length_vertical", uint64_t);
		READ_EG("num_reorder_frames", uint64_t);
		READ_EG("max_dec_frame_buffering", uint64_t);
	}
	return true;
}

bool scaling_list(BitArray &ba, uint8_t sizeOfScalingList) {
	uint32_t nextScale = 8;
	uint32_t lastScale = 8;
	uint64_t delta_scale = 0;
	for (uint8_t j = 0; j < sizeOfScalingList; j++) {
		if (nextScale != 0) {
			if (!ba.ReadExpGolomb(delta_scale))
				return false;
			nextScale = (lastScale + delta_scale + 256) % 256;
		}
		lastScale = (nextScale == 0) ? lastScale : nextScale;
	}
	return true;
}

bool ReadSPS(BitArray &ba, Variant &v) {
	//7.3.2.1 Sequence parameter set RBSP syntax
	//14496-10.pdf 43/280
	READ_INT("profile_idc", uint8_t, 8);
	READ_BOOL("constraint_set0_flag");
	READ_BOOL("constraint_set1_flag");
	READ_BOOL("constraint_set2_flag");
	READ_INT("reserved_zero_5bits", uint8_t, 5);
	READ_INT("level_idc", uint8_t, 8);
	READ_EG("seq_parameter_set_id", uint64_t);
	if ((uint64_t) v["profile_idc"] >= 100) {
		READ_EG("chroma_format_idc", uint64_t);
		if ((uint64_t) v["chroma_format_idc"] == 3)
			READ_BOOL("residual_colour_transform_flag");
		READ_EG("bit_depth_luma_minus8", uint64_t);
		READ_EG("bit_depth_chroma_minus8", uint64_t);
		READ_BOOL("qpprime_y_zero_transform_bypass_flag");
		READ_BOOL("seq_scaling_matrix_present_flag");
		if ((bool)v["seq_scaling_matrix_present_flag"]) {
			for (uint8_t i = 0; i < 8; i++) {
				uint8_t flag = 0;
				CHECK_BA_LIMITS("seq_scaling_list_present_flag", 1);
				flag = ba.ReadBits<uint8_t > (1);
				if (flag) {
					if (i < 6) {
						if (!scaling_list(ba, 16)) {
							FATAL("scaling_list failed");
							return false;
						}
					} else {
						if (!scaling_list(ba, 64)) {
							FATAL("scaling_list failed");
							return false;
						}
					}
				}
			}
		}
	}
	READ_EG("log2_max_frame_num_minus4", uint64_t);
	READ_EG("pic_order_cnt_type", uint64_t);
	if ((uint64_t) v["pic_order_cnt_type"] == 0) {
		READ_EG("log2_max_pic_order_cnt_lsb_minus4", uint64_t);
	} else if ((uint64_t) v["pic_order_cnt_type"] == 1) {
		READ_BOOL("delta_pic_order_always_zero_flag");
		READ_EG("offset_for_non_ref_pic", int64_t);
		READ_EG("offset_for_top_to_bottom_field", int64_t);
		READ_EG("num_ref_frames_in_pic_order_cnt_cycle", uint64_t);
		for (uint64_t i = 0; i < (uint64_t) v["num_ref_frames_in_pic_order_cnt_cycle"]; i++) {
			uint64_t val = 0;
			if (!ba.ReadExpGolomb(val)) {
				FATAL("Unable to read offset_for_ref_frame value");
				return false;
			}
			v["offset_for_ref_frame"].PushToArray((int64_t) val);
		}
	}
	READ_EG("num_ref_frames", uint64_t);
	READ_BOOL("gaps_in_frame_num_value_allowed_flag");
	READ_EG("pic_width_in_mbs_minus1", uint64_t);
	READ_EG("pic_height_in_map_units_minus1", uint64_t);
	READ_BOOL("frame_mbs_only_flag");
	if (!((bool)v["frame_mbs_only_flag"]))
		READ_BOOL("mb_adaptive_frame_field_flag");
	READ_BOOL("direct_8x8_inference_flag");
	READ_BOOL("frame_cropping_flag");
	if ((bool)v["frame_cropping_flag"]) {
		READ_EG("frame_crop_left_offset", uint64_t);
		READ_EG("frame_crop_right_offset", uint64_t);
		READ_EG("frame_crop_top_offset", uint64_t);
		READ_EG("frame_crop_bottom_offset", uint64_t);
	}
	READ_BOOL("vui_parameters_present_flag");
	if ((bool)v["vui_parameters_present_flag"]) {
		if (!ReadSPSVUI(ba, v["vui_parameters"])) {
			FATAL("Unable to read VUI");
			return false;
		}
	}
	return true;
}

bool ReadPPS(BitArray &ba, Variant &v) {
	//7.3.2.2 Picture parameter set RBSP syntax
	//14496-10.pdf 44/280
	READ_EG("pic_parameter_set_id", uint64_t);
	READ_EG("seq_parameter_set_id", uint64_t);
	READ_BOOL("entropy_coding_mode_flag");
	READ_BOOL("pic_order_present_flag");
	READ_EG("num_slice_groups_minus1", int64_t);
	if ((int64_t) v["num_slice_groups_minus1"] > 0) {
		READ_EG("slice_group_map_type", uint64_t);
		if ((int64_t) v["slice_group_map_type"] == 0) {
			for (int64_t i = 0; i < (int64_t) v["num_slice_groups_minus1"]; i++) {
				uint64_t val = 0;
				if (!ba.ReadExpGolomb(val)) {
					FATAL("Unable to read run_length_minus1 value");
					return false;
				}
				v["run_length_minus1"].PushToArray(val);
			}
		} else if ((int64_t) v["slice_group_map_type"] == 2) {
			for (int64_t i = 0; i < (int64_t) v["num_slice_groups_minus1"]; i++) {
				uint64_t val = 0;
				if (!ba.ReadExpGolomb(val)) {
					FATAL("Unable to read top_left value");
					return false;
				}
				v["top_left"].PushToArray(val);

				if (!ba.ReadExpGolomb(val)) {
					FATAL("Unable to read bottom_right value");
					return false;
				}
				v["bottom_right"].PushToArray(val);
			}
		} else if (((int64_t) v["slice_group_map_type"] == 3)
				|| ((int64_t) v["slice_group_map_type"] == 4)
				|| ((int64_t) v["slice_group_map_type"] == 5)) {
			READ_BOOL("slice_group_change_direction_flag");
			READ_EG("slice_group_change_rate_minus1", uint64_t);
		} else if ((int64_t) v["slice_group_map_type"] == 6) {
			READ_EG("pic_size_in_map_units_minus1", uint64_t);
			for (uint64_t i = 0; i <= (uint64_t) v["pic_size_in_map_units_minus1"]; i++) {
				uint64_t val = 0;
				if (!ba.ReadExpGolomb(val)) {
					FATAL("Unable to read slice_group_id value");
					return false;
				}
				v["slice_group_id"].PushToArray((int64_t) val);
			}
		}
	}
	READ_EG("num_ref_idx_l0_active_minus1", uint64_t);
	READ_EG("num_ref_idx_l1_active_minus1", uint64_t);
	READ_BOOL("weighted_pred_flag");
	READ_INT("weighted_bipred_idc", uint8_t, 2);
	READ_EG("pic_init_qp_minus26", int64_t);
	READ_EG("pic_init_qs_minus26", int64_t);
	READ_EG("chroma_qp_index_offset", int64_t);
	READ_BOOL("deblocking_filter_control_present_flag");
	READ_BOOL("constrained_intra_pred_flag");
	READ_BOOL("redundant_pic_cnt_present_flag");
	return true;
}

bool _VIDEO_AVC::Init(uint8_t *pSPS, uint32_t spsLength, uint8_t *pPPS,
		uint32_t ppsLength) {
	Clear();
	if ((spsLength <= 0)
			|| (spsLength > 65535)
			|| (ppsLength <= 0)
			|| (ppsLength > 65535)) {
		FATAL("Invalid SPS/PPS lengths");
		return false;
	}
	_spsLength = (uint16_t) spsLength;
	_pSPS = new uint8_t[_spsLength];
	memcpy(_pSPS, pSPS, _spsLength);

	_ppsLength = (uint16_t) ppsLength;
	_pPPS = new uint8_t[_ppsLength];
	memcpy(_pPPS, pPPS, _ppsLength);

	_rate = 90000;

	BitArray spsBa;
	//remove emulation_prevention_three_byte
	for (uint16_t i = 1; i < _spsLength; i++) {
		if (((i + 2)<(_spsLength - 1))
				&& (_pSPS[i + 0] == 0)
				&& (_pSPS[i + 1] == 0)
				&& (_pSPS[i + 2] == 3)) {
			spsBa.ReadFromRepeat(0, 2);
			i += 2;
		} else {
			spsBa.ReadFromRepeat(_pSPS[i], 1);
		}
	}

	if (!ReadSPS(spsBa, _SPSInfo)) {
		WARN("Unable to parse SPS");
	} else {
		_SPSInfo.Compact();
		_width = ((uint32_t) _SPSInfo["pic_width_in_mbs_minus1"] + 1)*16;
		_height = ((uint32_t) _SPSInfo["pic_height_in_map_units_minus1"] + 1)*16;
		//		FINEST("_width: %u (%u); _height: %u (%u)",
		//				_width, (uint32_t) _SPSInfo["pic_width_in_mbs_minus1"],
		//				_height, (uint32_t) _SPSInfo["pic_height_in_map_units_minus1"]);
	}

	BitArray ppsBa;
	//remove emulation_prevention_three_byte
	for (uint16_t i = 1; i < _ppsLength; i++) {
		if (((i + 2)<(_ppsLength - 1))
				&& (_pPPS[i + 0] == 0)
				&& (_pPPS[i + 1] == 0)
				&& (_pPPS[i + 2] == 3)) {
			ppsBa.ReadFromRepeat(0, 2);
			i += 2;
		} else {
			ppsBa.ReadFromRepeat(_pPPS[i], 1);
		}
	}
	
	if (!ReadPPS(ppsBa, _PPSInfo)) {
		WARN("Unable to read PPS info");
	}

	return true;
}

void _VIDEO_AVC::Clear() {
	if (_pSPS != NULL) {
		delete[] _pSPS;
		_pSPS = NULL;
	}
	_spsLength = 0;
	if (_pPPS != NULL) {
		delete[] _pPPS;
		_pPPS = NULL;
	}
	_ppsLength = 0;
	_rate = 0;
}

bool _VIDEO_AVC::Serialize(IOBuffer &dest) {
	uint8_t temp[sizeof (_spsLength) + sizeof (_ppsLength) + sizeof (uint32_t)];
	EHTONSP(temp, _spsLength);
	dest.ReadFromBuffer(temp, sizeof (_spsLength));
	dest.ReadFromBuffer(_pSPS, _spsLength);
	EHTONSP(temp, _ppsLength);
	dest.ReadFromBuffer(temp, sizeof (_ppsLength));
	dest.ReadFromBuffer(_pPPS, _ppsLength);
	EHTONLP(temp, _widthOverride);
	dest.ReadFromBuffer(temp, sizeof (uint32_t));
	EHTONLP(temp, _heightOverride);
	dest.ReadFromBuffer(temp, sizeof (uint32_t));
	return true;
}

bool _VIDEO_AVC::Deserialize(IOBuffer &src, _VIDEO_AVC &dest) {
	dest.Clear();
	uint8_t *pBuffer = GETIBPOINTER(src);
	uint32_t length = GETAVAILABLEBYTESCOUNT(src);
	if (length<sizeof (dest._spsLength)) {
		FATAL("Not enough data");
		return false;
	}
	dest._spsLength = ENTOHSP(pBuffer);
	if (length < (
			sizeof (dest._spsLength)
			+ dest._spsLength
			+ sizeof (dest._ppsLength)
			+ 2 * sizeof (uint32_t))) {
		FATAL("Not enough data");
		return false;
	}
	dest._ppsLength = ENTOHSP(pBuffer + sizeof (dest._spsLength) + dest._spsLength);
	if (length < (
			sizeof (dest._spsLength)
			+ dest._spsLength
			+ sizeof (dest._ppsLength)
			+ dest._ppsLength
			+ 2 * sizeof (uint32_t))) {
		FATAL("Not enough data");
		return false;
	}
	if (!dest.Init(
			pBuffer + sizeof (dest._spsLength), dest._spsLength,
			pBuffer + sizeof (dest._spsLength) + dest._spsLength + sizeof (dest._ppsLength), dest._ppsLength)) {
		FATAL("Unable to init AVC");
		return false;
	}
	dest._widthOverride = ENTOHLP(pBuffer + sizeof (dest._spsLength) + dest._spsLength + sizeof (dest._ppsLength) + dest._ppsLength);
	dest._heightOverride = ENTOHLP(pBuffer + sizeof (dest._spsLength) + dest._spsLength + sizeof (dest._ppsLength) + dest._ppsLength + sizeof (uint32_t));

	return src.Ignore(sizeof (dest._spsLength) + dest._spsLength + sizeof (dest._ppsLength) + dest._ppsLength + sizeof (uint32_t) + sizeof (uint32_t));
}

_VIDEO_AVC::operator string() {
	string result;
	result += format("_spsLength: %"PRIu16"\n", _spsLength);
	result += format("_ppsLength: %"PRIu16"\n", _ppsLength);
	result += format("_rate: %"PRIu32"\n", _rate);
	result += format("WxH: %"PRIu32"x%"PRIu32, _width, _height);
	return result;
}

_AUDIO_AAC::_AUDIO_AAC() {
	_pAAC = NULL;
	_aacLength = 0;
	_audioObjectType = 0;
	_sampleRateIndex = 0;
	_sampleRate = 0;
	_channelConfigurationIndex = 0;
}

_AUDIO_AAC::~_AUDIO_AAC() {
	Clear();
}

bool _AUDIO_AAC::Init(uint8_t *pBuffer, uint32_t length) {
	Clear();
	//http://wiki.multimedia.cx/index.php?title=MP4A#Audio_Specific_Config

	if (length < 2) {
		FATAL("Invalid length: %u", length);
		return false;
	}

	//1. Prepare the bit array
	BitArray ba;
	ba.ReadFromBuffer(pBuffer, length);

	//2. Read the audio object type
	_audioObjectType = ba.ReadBits<uint8_t > (5);
	if ((_audioObjectType != 1)
			&& (_audioObjectType != 2)
			&& (_audioObjectType != 3)
			&& (_audioObjectType != 4)
			&& (_audioObjectType != 6)
			&& (_audioObjectType != 17)
			&& (_audioObjectType != 19)
			&& (_audioObjectType != 20)
			&& (_audioObjectType != 23)
			&& (_audioObjectType != 39)) {
		FATAL("Invalid _audioObjectType: %hhu", _audioObjectType);
		return false;
	}

	//3. Read the sample rate index
	_sampleRateIndex = ba.ReadBits<uint8_t > (4);
	if ((_sampleRateIndex == 13)
			|| (_sampleRateIndex == 14)) {
		FATAL("Invalid sample rate: %hhu", _sampleRateIndex);
		return false;
	}
	if (_sampleRateIndex == 15) {
		if (length < 5) {
			FATAL("Invalid length: %u", length);
			return false;
		}
		_sampleRate = ba.ReadBits<uint32_t > (24);
	} else {
		uint32_t rates[] = {
			96000, 88200, 64000, 48000, 44100, 32000, 24000, 22050, 16000,
			12000, 11025, 8000, 7350
		};
		_sampleRate = rates[_sampleRateIndex];
	}

	//4. read the channel configuration index
	_channelConfigurationIndex = ba.ReadBits<uint8_t > (4);
	if ((_channelConfigurationIndex == 0)
			|| (_channelConfigurationIndex >= 8)) {
		FATAL("Invalid _channelConfigurationIndex: %hhu", _channelConfigurationIndex);
		return false;
	}

	_pAAC = new uint8_t[length];
	memcpy(_pAAC, pBuffer, length);
	_aacLength = length;


	return true;
}

void _AUDIO_AAC::Clear() {
	if (_pAAC != NULL) {
		delete[] _pAAC;
		_pAAC = NULL;
	}
	_aacLength = 0;
	_audioObjectType = 0;
	_sampleRateIndex = 0;
	_sampleRate = 0;
	_channelConfigurationIndex = 0;
}

string _AUDIO_AAC::GetRTSPFmtpConfig() {
	string result;
	for (uint32_t i = 0; i < _aacLength; i++) {
		result += format("%02hhx", _pAAC[i]);
	}
	return "config=" + result;
}

bool _AUDIO_AAC::Serialize(IOBuffer &dest) {
	uint8_t temp[sizeof (_aacLength)];
	EHTONLP(temp, _aacLength);
	dest.ReadFromBuffer(temp, sizeof (_aacLength));
	dest.ReadFromBuffer(_pAAC, _aacLength);
	return true;
}

bool _AUDIO_AAC::Deserialize(IOBuffer &src, _AUDIO_AAC &dest) {
	dest.Clear();
	uint8_t *pBuffer = GETIBPOINTER(src);
	uint32_t length = GETAVAILABLEBYTESCOUNT(src);
	if (length<sizeof (dest._aacLength)) {
		FATAL("Not enough data");
		return false;
	}
	dest._aacLength = ENTOHLP(pBuffer);
	if (length<sizeof (dest._aacLength) + dest._aacLength) {
		FATAL("Not enough data");
		return false;
	}
	if (!dest.Init(pBuffer + sizeof (dest._aacLength), dest._aacLength)) {
		FATAL("Unable to init AAC");
		return false;
	}
	return src.Ignore(sizeof (dest._aacLength) + dest._aacLength);
}

_AUDIO_AAC::operator string() {
	string result;
	result += format("_aacLength: %u\n", _aacLength);
	result += format("_audioObjectType: %hhu\n", _audioObjectType);
	result += format("_sampleRateIndex: %hhu\n", _sampleRateIndex);
	result += format("_sampleRate: %u\n", _sampleRate);
	result += format("_channelConfigurationIndex: %hhu", _channelConfigurationIndex);
	return result;
}

StreamCapabilities::StreamCapabilities() {
	videoCodecId = CODEC_VIDEO_UNKNOWN;
	audioCodecId = CODEC_AUDIO_UNKNOWN;
	bandwidthHint = 0;
}

StreamCapabilities::~StreamCapabilities() {
	Clear();
}

bool StreamCapabilities::InitAudioAAC(uint8_t *pBuffer, uint32_t length) {
	ClearAudio();
	if (!aac.Init(pBuffer, length)) {
		ClearAudio();
		return false;
	}
	audioCodecId = CODEC_AUDIO_AAC;
	return true;
}

bool StreamCapabilities::InitAudioADTS() {
	ClearAudio();
	audioCodecId = CODEC_AUDIO_ADTS;
	return true;
}

bool StreamCapabilities::InitAudioMP3() {
	ClearAudio();
	audioCodecId = CODEC_AUDIO_MP3;
	return true;
}

bool StreamCapabilities::InitVideoH264(uint8_t *pSPS, uint32_t spsLength,
		uint8_t *pPPS, uint32_t ppsLength) {
	ClearVideo();
	if (!avc.Init(pSPS, spsLength, pPPS, ppsLength)) {
		ClearVideo();
		return false;
	}
	videoCodecId = CODEC_VIDEO_AVC;
	return true;
}

void StreamCapabilities::ClearVideo() {
	switch (videoCodecId) {
		case CODEC_VIDEO_AVC:
		{
			avc.Clear();
			break;
		}
		default:
		{
			break;
		}
	}
	videoCodecId = 0;
}

void StreamCapabilities::ClearAudio() {
	switch (audioCodecId) {
		case CODEC_AUDIO_AAC:
		{
			aac.Clear();
			break;
		}
		default:
		{
			break;
		}
	}
	audioCodecId = 0;
}

void StreamCapabilities::Clear() {
	ClearVideo();
	ClearAudio();
	bandwidthHint = 0;
}

#define __STREAM_CAPABILITIES_VERSION MAKE_TAG4('V','E','R','3')

bool StreamCapabilities::Serialize(IOBuffer &dest) {
	uint8_t temp[28];
	EHTONLLP(temp, __STREAM_CAPABILITIES_VERSION);
	EHTONLLP(temp + 8, videoCodecId);
	EHTONLLP(temp + 16, audioCodecId);
	EHTONLP(temp + 24, bandwidthHint);
	dest.ReadFromBuffer(temp, 28);
	switch (videoCodecId) {
		case CODEC_VIDEO_AVC:
		{
			if (!avc.Serialize(dest)) {
				FATAL("Unable to serialize avc");
				return false;
			}
			break;
		}
		default:
		{
			break;
		}
	}
	switch (audioCodecId) {
		case CODEC_AUDIO_AAC:
		{
			if (!aac.Serialize(dest)) {
				FATAL("Unable to serialize aac");
				return false;
			}
			break;
		}
		default:
		{
			break;
		}
	}
	return true;
}

bool StreamCapabilities::Deserialize(string seekFilePath, StreamCapabilities &capabilities) {
	File file;
	if (!file.Initialize(seekFilePath, FILE_OPEN_MODE_READ)) {
		FATAL("Unable to open seek file %s", STR(seekFilePath));
		return false;
	}

	uint32_t length = 0;
	if (!file.ReadUI32(&length, false)) {
		FATAL("Unable to read stream capabilities length from file %s", STR(seekFilePath));
		return false;
	}
	if (length > 1024 * 1024) {
		FATAL("Invalid stream capabilities length in file %s: %"PRIu32, STR(seekFilePath), length);
		return false;
	}

	IOBuffer buffer;
	buffer.ReadFromRepeat(0, length);
	if (!file.ReadBuffer(GETIBPOINTER(buffer), length)) {
		FATAL("Unable to read stream capabilities payload from file %s", STR(seekFilePath));
		return false;
	}

	file.Close();

	if (!Deserialize(buffer, capabilities)) {
		FATAL("Unable to deserialize stream capabilities from file %s", STR(seekFilePath));
		return false;
	}

	return true;
}

bool StreamCapabilities::Deserialize(IOBuffer &src, StreamCapabilities &capabilities) {
	uint8_t *pBuffer = GETIBPOINTER(src);
	uint32_t length = GETAVAILABLEBYTESCOUNT(src);
	if (length < 28) {
		FATAL("Not enough data");
		return false;
	}
	uint64_t ver = ENTOHLLP(pBuffer);
	if (ver != __STREAM_CAPABILITIES_VERSION) {
		FATAL("Invalid stream capabilities version. Wanted: %"PRIu64"; Got: %"PRIu64,
				__STREAM_CAPABILITIES_VERSION, ver);
		return false;
	}
	capabilities.Clear();
	capabilities.videoCodecId = ENTOHLLP(pBuffer + 8);
	capabilities.audioCodecId = ENTOHLLP(pBuffer + 16);
	capabilities.bandwidthHint = ENTOHLP(pBuffer + 24);
	src.Ignore(28);
	switch (capabilities.videoCodecId) {
		case CODEC_VIDEO_AVC:
		{
			if (!_VIDEO_AVC::Deserialize(src, capabilities.avc)) {
				FATAL("Unable to deserialize avc");
				return false;
			}
			break;
		}
		default:
		{
			break;
		}
	}
	switch (capabilities.audioCodecId) {
		case CODEC_AUDIO_AAC:
		{
			if (!_AUDIO_AAC::Deserialize(src, capabilities.aac)) {
				FATAL("Unable to deserialize aac");
				return false;
			}
			break;
		}
		default:
		{
			break;
		}
	}
	return true;
}
