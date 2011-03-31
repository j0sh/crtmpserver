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


#ifdef HAS_MEDIA_NSV

#include "mediaformats/nsv/mp3media.h"

#define LAYER_3 1
#define LAYER_2 2
#define LAYER_1 3

int32_t MP3Media::_bitRates[4][4][16] = {
	{ //MPEG Version 2.5
		//reserved layer
		{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},

		//Layer III
		{-1, 8000, 16000, 24000, 32000, 40000, 48000, 56000, 64000, 80000,
			96000, 112000, 128000, 144000, 160000, -1},

		//Layer II
		{-1, 8000, 16000, 24000, 32000, 40000, 48000, 56000, 64000, 80000,
			96000, 112000, 128000, 144000, 160000, -1},

		//Layer I
		{-1, 32000, 48000, 56000, 64000, 80000, 96000, 112000, 128000, 144000,
			160000, 176000, 192000, 224000, 256000, -1},
	},
	{ // Reserved
		{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
	},
	{ //MPEG Version 2 (ISO/IEC 13818-3)
		//reserved layer
		{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},

		//Layer III
		{-1, 8000, 16000, 24000, 32000, 40000, 48000, 56000, 64000, 80000,
			96000, 112000, 128000, 144000, 160000, -1},

		//Layer II
		{-1, 8000, 16000, 24000, 32000, 40000, 48000, 56000, 64000, 80000,
			96000, 112000, 128000, 144000, 160000, -1},

		//Layer I
		{-1, 32000, 48000, 56000, 64000, 80000, 96000, 112000, 128000, 144000,
			160000, 176000, 192000, 224000, 256000, -1},
	},
	{ //MPEG Version 1 (ISO/IEC 11172-3)
		//reserved layer
		{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},

		//Layer III
		{-1, 32000, 40000, 48000, 56000, 64000, 80000, 96000, 112000, 128000,
			160000, 192000, 224000, 256000, 320000, -1},

		//Layer II
		{-1, 32000, 48000, 56000, 64000, 80000, 96000, 112000, 128000,
			160000, 192000, 224000, 256000, 320000, 384000 - 1},

		//Layer I
		{-1, 32000, 64000, 96000, 128000, 160000, 192000, 224000, 256000, 288000,
			320000, 352000, 384000, 416000, 448000 - 1},
	}
};

int32_t MP3Media::_samplingRates[4][4] = {
	{//MPEG Version 2.5
		11025, 12000, 8000, -1
	},
	{// Reserved
		-1, -1, -1, -1
	},
	{//MPEG Version 2 (ISO/IEC 13818-3)
		22050, 24000, 16000, -1
	},
	{//MPEG Version 1 (ISO/IEC 11172-3)
		44100, 48000, 32000, -1
	}
};

string MP3Media::_versionNames[4] = {
	"MPEG Version 2.5",
	"reserved",
	"MPEG Version 2 (ISO/IEC 13818-3)",
	"MPEG Version 1 (ISO/IEC 11172-3)"
};
string MP3Media::_layerNames[4] = {
	"reserved",
	"Layer III",
	"Layer II",
	"Layer I",
};

map<uint8_t, map<uint8_t, map<uint8_t, map<uint8_t, map<uint8_t, uint64_t > > > > > MP3Media::_frameSizes;

MP3Media::MP3Media() {

}

MP3Media::~MP3Media() {
}

void MP3Media::InitFrameSizes() {
	if (_frameSizes.size() > 0)
		return;
	//ver/layer/bitrate/samplerate/padding
	for (uint8_t ver = 0; ver < 4; ver++) {
		if (ver == 1)
			continue;
		for (uint8_t layer = 0; layer < 4; layer++) {
			if (layer == 0)
				continue;
			for (uint8_t bitRateIndex = 1; bitRateIndex < 15; bitRateIndex++) {
				for (uint8_t sampleRateIndex = 0; sampleRateIndex < 3; sampleRateIndex++) {
					for (uint8_t padding = 0; padding < 2; padding++) {
						int32_t bitRate = _bitRates[ver][layer][bitRateIndex];
						int32_t sampleRate = _samplingRates[ver][sampleRateIndex];
						uint32_t length = 0;
						if (layer == LAYER_1) {
							length = (12 * bitRate / sampleRate + padding)*4;
						} else {
							length = 144 * bitRate / sampleRate + padding;
						}
						_frameSizes[ver][layer][bitRateIndex][sampleRateIndex][padding] = length;
					}
				}
			}
		}
	}
}

bool MP3Media::GetAudioFrames(uint8_t *pBuffer, uint32_t &audioSamplesCount, MediaFrame &mediaFrame) {

	InitFrameSizes();

	uint8_t firstBytes[4];

	memcpy(firstBytes, pBuffer, 4);

	mediaFrame.type = MEDIAFRAME_TYPE_AUDIO;
	mediaFrame.isKeyFrame = true;
	mediaFrame.deltaTime = 0;
	mediaFrame.isBinaryHeader = false;
	//Read the first 4 bytes
	//firstBytes

	if ((firstBytes[0] == 0xff) &&
			((firstBytes[1] >> 5) == 7)) {
		//4. Possible frame. Read the header
		uint8_t version = (firstBytes[1] >> 3)&0x03;
		uint8_t layer = (firstBytes[1] >> 1)&0x03;
		//uint8_t protectBit = firstBytes[1]&0x01;
		uint8_t bitRateIndex = firstBytes[2] >> 4;
		uint8_t sampleRateIndex = (firstBytes[2] >> 2)&0x03;
		uint8_t paddingBit = (firstBytes[2] >> 1)&0x01;

		//5. get the freame length
		mediaFrame.length = _frameSizes[version][layer][bitRateIndex]
				[sampleRateIndex][paddingBit];

		if (mediaFrame.length == 0) {
			FATAL("Invalid frame length: %hhu:%hhu:%hhu:%hhu:%hhu;",
					version, layer, bitRateIndex, sampleRateIndex,
					paddingBit); //, _mediaFile.Cursor());
			return false;
		}

		//6. Compute the frame duration and save the frame start
		uint32_t samplesCount = 0;
		if (layer == LAYER_1)
			samplesCount = 384;
		else
			samplesCount = 1152;

		mediaFrame.absoluteTime = ((double) audioSamplesCount /
				(double) (_samplingRates[version][sampleRateIndex])) * 1000.0;

		audioSamplesCount += samplesCount;


		//8. All good. Save the frame
	}
	return true;
}

#endif /*HAS_MEDIA_NSV*/

