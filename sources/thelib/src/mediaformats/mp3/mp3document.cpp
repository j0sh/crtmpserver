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

#ifdef HAS_MEDIA_MP3
#include "mediaformats/mp3/mp3document.h"
#include "protocols/baseprotocol.h"
#include "mediaformats/mp3/id3parser.h"

#define LAYER_3 1
#define LAYER_2 2
#define LAYER_1 3

int32_t MP3Document::_bitRates[4][4][16] = {
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

int32_t MP3Document::_samplingRates[4][4] = {
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

string MP3Document::_versionNames[4] = {
	"MPEG Version 2.5",
	"reserved",
	"MPEG Version 2 (ISO/IEC 13818-3)",
	"MPEG Version 1 (ISO/IEC 11172-3)"
};
string MP3Document::_layerNames[4] = {
	"reserved",
	"Layer III",
	"Layer II",
	"Layer I",
};

map<uint8_t, map<uint8_t, map<uint8_t, map<uint8_t, map<uint8_t, uint64_t > > > > > MP3Document::_frameSizes;

MP3Document::MP3Document(Variant &metadata)
: BaseMediaDocument(metadata) {
	_streamCapabilities.audioCodecId = CODEC_AUDIO_MP3;
	_streamCapabilities.videoCodecId = CODEC_VIDEO_UNKNOWN;
}

MP3Document::~MP3Document() {
}

bool MP3Document::ParseDocument() {
	return true;
}

bool MP3Document::BuildFrames() {
	//1. Build the map with frame sizes
	InitFrameSizes();

	//2. Go to the beginning of the file
	if (!_mediaFile.SeekBegin()) {
		FATAL("Unable to seek in file");
		return false;
	}

	if (!ParseMetadata()) {
		WARN("Invalid metadata");
		if (!FindFrameData()) {
			FATAL("Unable to position on frame data");
			return false;
		}
	}

	uint8_t firstBytes[4];

	double totalDuration = 0;
	MediaFrame frame = {0, 0, 0, 0, 0, 0, 0, 0};
	frame.type = MEDIAFRAME_TYPE_AUDIO;
	frame.isKeyFrame = true;
	frame.deltaTime = 0;
	frame.isBinaryHeader = false;

	while (_mediaFile.Cursor() != _mediaFile.Size()) {
		//3. Read the first 4 bytes
		if (!_mediaFile.ReadBuffer(firstBytes, 4)) {
			FATAL("Unable to read 4 byte");
			return false;
		}

		if ((firstBytes[0] == 0xff) &&
				((firstBytes[1] >> 5) == 7)) {
			//4. Possible frame. Read the header
			uint8_t version = (firstBytes[1] >> 3)&0x03;
			uint8_t layer = (firstBytes[1] >> 1)&0x03;
			uint8_t bitRateIndex = firstBytes[2] >> 4;
			uint8_t sampleRateIndex = (firstBytes[2] >> 2)&0x03;
			uint8_t paddingBit = (firstBytes[2] >> 1)&0x01;

			//5. get the freame length
			frame.start = _mediaFile.Cursor() - 4;
			frame.length = _frameSizes[version][layer][bitRateIndex]
					[sampleRateIndex][paddingBit];
			if (frame.length == 0) {
				FATAL("Invalid frame length: %hhu:%hhu:%hhu:%hhu:%hhu; Cusror: %"PRIx64,
						version, layer, bitRateIndex, sampleRateIndex,
						paddingBit, _mediaFile.Cursor());
				return false;
			}

			//6. Compute the frame duration and save the frame start
			uint32_t samplesCount = 0;
			if (layer == LAYER_1)
				samplesCount = 384;
			else
				samplesCount = 1152;
			frame.absoluteTime = totalDuration * 1000;
			totalDuration += (double) samplesCount /
					(double) (_samplingRates[version][sampleRateIndex]);

			//7. Seek to the next frame
			if (!_mediaFile.SeekTo(frame.start + frame.length)) {
				WARN("Unable to seek to %"PRIx64, frame.start + frame.length);
				break;
			}

			//8. All good. Save the frame
			ADD_VECTOR_END(_frames, frame);
		} else {
			break;
		}
	}


	return true;
}

Variant MP3Document::GetRTMPMeta() {
	return _tags;
}

bool MP3Document::FindFrameData() {
	if (!_mediaFile.SeekBegin()) {
		FATAL("Unable to seek to the beginning of the file");
		return false;
	}

	uint8_t firstBytes[4];
	while (true) {
		//1. Read the first 4 bytes
		if (!_mediaFile.PeekBuffer(firstBytes, 4)) {
			FATAL("Unable to read 4 bytes");
			return false;
		}
		if ((firstBytes[0] != 0xff) || ((firstBytes[1] >> 5) != 7)) {
			_mediaFile.SeekAhead(1);
			continue;
		}

		//2. Split the flags
		uint8_t version = (firstBytes[1] >> 3)&0x03;
		uint8_t layer = (firstBytes[1] >> 1)&0x03;
		uint8_t bitRateIndex = firstBytes[2] >> 4;
		uint8_t sampleRateIndex = (firstBytes[2] >> 2)&0x03;
		uint8_t paddingBit = (firstBytes[2] >> 1)&0x01;

		//3. Compute the frame length from the flags
		uint64_t length = _frameSizes[version][layer][bitRateIndex]
				[sampleRateIndex][paddingBit];
		if (length == 0) {
			_mediaFile.SeekAhead(1);
			continue;
		}

		//4. Save the cursor value and seek ahead to the next frame
		uint64_t cursor = _mediaFile.Cursor();
		_mediaFile.SeekTo(cursor + length);

		//5. Try to read 4 bytes again
		if (!_mediaFile.PeekBuffer(firstBytes, 4)) {
			FATAL("Unable to read 4 bytes");
			return false;
		}

		//6. Is this a frame start?
		if ((firstBytes[0] != 0xff) || ((firstBytes[1] >> 5) != 7)) {
			_mediaFile.SeekTo(cursor + 1);
			continue;
		}

		//7. Jack pot!
		return true;
	}
	return false;
}

bool MP3Document::ParseMetadata() {
	//1. pick up first 3 bytes they must be ID3
	uint8_t id3[3];
	if (!_mediaFile.ReadBuffer(id3, 3)) {
		FATAL("Unable to read 3 bytes");
		return false;
	}
	if ((id3[0] != 'I') || (id3[1] != 'D') || (id3[2] != '3')) {
		WARN("ID3 not found");
		return false;
	}

	//2. pick up the major version
	uint8_t majorVersion;
	uint8_t minorVersion;
	if (!_mediaFile.ReadBuffer(&majorVersion, 1)) {
		FATAL("Unable to read 1 byte");
		return false;
	}
	if (!_mediaFile.ReadBuffer(&minorVersion, 1)) {
		FATAL("Unable to read 1 byte");
		return false;
	}

	//3. Instantiate the proper parser
	ID3Parser *pParser = new ID3Parser(majorVersion, minorVersion);

	//4. Parse
	bool parseResult = pParser->Parse(_mediaFile);
	_tags["tags"] = pParser->GetMetadata();
	delete pParser;
	if (!parseResult) {
		return false;
	}

	//5. Process the metadata
	return true;
}

void MP3Document::InitFrameSizes() {
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

#endif /* HAS_MEDIA_MP3 */
