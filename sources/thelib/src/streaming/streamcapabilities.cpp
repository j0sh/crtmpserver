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
}

_VIDEO_AVC::~_VIDEO_AVC() {
	Clear();
}

bool _VIDEO_AVC::Init(uint8_t *pSPS, uint32_t spsLength, uint8_t *pPPS,
		uint32_t ppsLength) {
	Clear();
	_spsLength = spsLength;
	_pSPS = new uint8_t[_spsLength];
	memcpy(_pSPS, pSPS, _spsLength);

	_ppsLength = ppsLength;
	_pPPS = new uint8_t[_ppsLength];
	memcpy(_pPPS, pPPS, _ppsLength);

	_rate = 90000;

	//FINEST("H264 codec:\n%s", STR(*this));
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

_VIDEO_AVC::operator string() {
	string result;
	result += format("_spsLength: %d\n", _spsLength);
	result += format("_ppsLength: %d\n", _ppsLength);
	result += format("_rate: %d", _rate);
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
		FATAL("Invalid length: %d", length);
		return false;
	}

	//1. Prepare the bit array
	//	uint8_t temp[] = {0x13, 0x10, 0x56, 0xE5, 0x98};
	//	length = sizeof (temp);
	//	pBuffer = temp;
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
		FATAL("Invalid _audioObjectType: %d", _audioObjectType);
		return false;
	}
	//FINEST("_audioObjectType: %d", _audioObjectType);

	//3. Read the sample rate index
	_sampleRateIndex = ba.ReadBits<uint8_t > (4);
	if ((_sampleRateIndex == 13)
			|| (_sampleRateIndex == 14)) {
		FATAL("Invalid sample rate: %d", _sampleRateIndex);
		return false;
	}
	//FINEST("_sampleRateIndex: %d", _sampleRateIndex);
	if (_sampleRateIndex == 15) {
		if (length < 5) {
			FATAL("Invalid length: %d", length);
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
	//FINEST("_sampleRate: %d", _sampleRate);

	//4. read the channel configuration index
	_channelConfigurationIndex = ba.ReadBits<uint8_t > (4);
	if ((_channelConfigurationIndex == 0)
			|| (_channelConfigurationIndex >= 8)) {
		FATAL("Invalid _channelConfigurationIndex: %d", _channelConfigurationIndex);
		return false;
	}
	//FINEST("_channelConfigurationIndex: %d", _channelConfigurationIndex);

	_pAAC = new uint8_t[length];
	memcpy(_pAAC, pBuffer, length);
	_aacLength = length;

	//FINEST("AAC codec:\n%s", STR(*this));

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
		result += format("%02x", _pAAC[i]);
	}
	return "config=" + result;
}

_AUDIO_AAC::operator string() {
	string result;
	result += format("_aacLength: %d\n", _aacLength);
	result += format("_audioObjectType: %d\n", _audioObjectType);
	result += format("_sampleRateIndex: %d\n", _sampleRateIndex);
	result += format("_sampleRate: %d\n", _sampleRate);
	result += format("_channelConfigurationIndex: %d", _channelConfigurationIndex);
	return result;
}

StreamCapabilities::StreamCapabilities() {
	videoCodecId = CODEC_VIDEO_UNKNOWN;
	audioCodecId = CODEC_AUDIO_UNKNOWN;
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
}
