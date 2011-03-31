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
#include "mediaformats/mp3/id3parser.h"
#include "mediaformats/mediaframe.h"

#define TAG_USLT MAKE_TAG4('U','S','L','T')
#define TAG_APIC MAKE_TAG4('A','P','I','C')
#define TAG_COMM MAKE_TAG4('C','O','M','M')
#define TAG_TXXX MAKE_TAG4('T','X','X','X')
#define TAG_WXXX MAKE_TAG4('W','X','X','X')
#define TAG_START_TEXT MAKE_TAG4('T','0','0','0')
#define TAG_END_TEXT MAKE_TAG4('T','Z','Z','Z')
#define TAG_START_URL MAKE_TAG4('W','0','0','0')
#define TAG_END_URL MAKE_TAG4('W','Z','Z','Z')

ID3Parser::ID3Parser(uint32_t majorVersion, uint32_t minorVersion) {
	_majorVersion = majorVersion;
	_minorVersion = minorVersion;
	_unsynchronisation = false;
	_compression = false;
	_extendedHeader = false;
	_experimentalIndicator = false;
	_footerPresent = false;
}

ID3Parser::~ID3Parser() {
}

Variant ID3Parser::GetMetadata() {
	return _metadata;
}

bool ID3Parser::Parse(MediaFile &file) {
	_metadata.Reset();

	//http://www.id3.org/d3v2.3.0
	//1. Read the flags
	uint8_t flags;
	if (!file.ReadBuffer(&flags, 1)) {
		WARN("Unable to read 1 byte");
		return false;
	}
	switch (_majorVersion) {
		case 3:
		{
			_unsynchronisation = ((flags >> 7) != 0);
			_extendedHeader = (((flags >> 6)&0x01) != 0);
			_experimentalIndicator = (((flags >> 5)&0x01) != 0);
			break;
		}
		case 4:
		{
			_unsynchronisation = ((flags >> 7) != 0);
			_extendedHeader = (((flags >> 6)&0x01) != 0);
			_experimentalIndicator = (((flags >> 5)&0x01) != 0);
			_footerPresent = (((flags >> 4)&0x01) != 0);
			break;
		}
		default:
		{
			WARN("Invalid version: ID3v%u.%u", _majorVersion, _minorVersion);
			return false;
		}
	}


	//2. Read the size
	uint8_t tempBuffer[4];
	if (!file.ReadBuffer(tempBuffer, 4)) {
		WARN("Unable to read 4 bytes");
		return false;
	}
	uint32_t totalSize = (tempBuffer[0]&0x7f);
	totalSize = (totalSize << 7) | (tempBuffer[1]&0x7f);
	totalSize = (totalSize << 7) | (tempBuffer[2]&0x7f);
	totalSize = (totalSize << 7) | (tempBuffer[3]&0x7f);

	//3. See if we go to the end of the ID3 we have a valid media frame
	//After that we go back to where we left (end of the header, offset 10)
	if (!file.SeekTo(totalSize + 10)) {
		WARN("Unable to seek to 0x%x offset", totalSize);
		return false;
	}

	if (!file.ReadBuffer(tempBuffer, 2)) {
		WARN("Unable to read 2 bytes");
		return false;
	}
	if ((tempBuffer[0] != 0xff) || ((tempBuffer[1] >> 5) != 7)) {
		WARN("Unable to position on frame data.");
		return false;
	}
	if (!file.SeekTo(10)) {
		WARN("Unable to seek to 0x0a offset");
		return false;
	}

	//4. Skip the extended header
	if (_extendedHeader) {
		NYIR;
	}

	//5. Store the tags inside a IOBuffer
	IOBuffer buffer;
	if (!buffer.ReadFromFs(file, totalSize)) {
		WARN("Unable to read %u bytes", totalSize);
		return false;
	}

	if (!ParseTags(buffer)) {
		WARN("Unable to parse the tags. Metadata might be incomplete");
	}

	return true;
}

#define CHECK_BUFFER_SIZE(x,y) \
if (GETAVAILABLEBYTESCOUNT((x)) < (y)){ \
    WARN("Not enough data (%u - %u)", \
        GETAVAILABLEBYTESCOUNT((x)),(uint32_t)(y)); \
    return false; \
} 

bool ID3Parser::ParseTags(IOBuffer &buffer) {
	IOBuffer tagBuffer;
	Variant tag;
	while (GETAVAILABLEBYTESCOUNT(buffer) != 0) {

		//1. Read the tag type
		CHECK_BUFFER_SIZE(buffer, 4);
		string name = string((char *) GETIBPOINTER(buffer), 4);
		if (
				(!(((name[0] >= 'A') && (name[0] <= 'Z')) || ((name[0] >= '0') && (name[0] <= '9')))) ||
				(!(((name[1] >= 'A') && (name[1] <= 'Z')) || ((name[1] >= '0') && (name[1] <= '9')))) ||
				(!(((name[2] >= 'A') && (name[2] <= 'Z')) || ((name[2] >= '0') && (name[2] <= '9')))) ||
				(!(((name[3] >= 'A') && (name[3] <= 'Z')) || ((name[3] >= '0') && (name[3] <= '9'))))
				) {
			if ((name[0] == 0) && (name[1] == 0) && (name[2] == 0) && (name[3] == 0)) {
				return true;
			} else {
				WARN("Invalid tag name: %hhu %hhu %hhu %hhu",
						name[0], name[1], name[2], name[3]);
				return false;
			}
		}
		uint64_t id = MAKE_TAG4(name[0], name[1], name[2], name[3]);
		buffer.Ignore(4);



		//2. Read the size and the flags
		CHECK_BUFFER_SIZE(buffer, 4);
		uint32_t size = 0;
		if (_majorVersion == 4) {
			size = (GETIBPOINTER(buffer)[0]&0x7f);
			size = (size << 7) | (GETIBPOINTER(buffer)[1]&0x7f);
			size = (size << 7) | (GETIBPOINTER(buffer)[2]&0x7f);
			size = (size << 7) | (GETIBPOINTER(buffer)[3]&0x7f);
		} else {
			size = ENTOHLP(GETIBPOINTER(buffer));
		}
		buffer.Ignore(4);

		CHECK_BUFFER_SIZE(buffer, 2);
		buffer.Ignore(2);
		if (size == 0 || GETAVAILABLEBYTESCOUNT(buffer) < size) {
			WARN("Invalid tagSize");
			return false;
		}

		//3. Get a copy of data
		tagBuffer.IgnoreAll();
		tagBuffer.ReadFromBuffer(GETIBPOINTER(buffer), size);
		buffer.Ignore(size);

		//4. Parse
		tag.Reset();
		if (id == TAG_USLT) {
			if (!ParseUSLT(tagBuffer, tag)) {
				WARN("Unable to parse tag %s", STR(name));
			}
			_metadata[name] = tag;
		} else if (id == TAG_APIC) {
			if (!ParseAPIC(tagBuffer, tag)) {
				WARN("Unable to parse tag %s", STR(name));
			}
			_metadata[name] = tag;
		} else if (id == TAG_COMM) {
			if (!ParseCOMM(tagBuffer, tag)) {
				WARN("Unable to parse tag %s", STR(name));
			}
			_metadata[name] = tag;
		} else if (id == TAG_TXXX) {
			if (!ParseTXXX(tagBuffer, tag)) {
				WARN("Unable to parse tag %s", STR(name));
			}
			_metadata[name] = tag;
		} else if (id == TAG_WXXX) {
			if (!ParseWXXX(tagBuffer, tag)) {
				WARN("Unable to parse tag %s", STR(name));
			}
			_metadata[name] = tag;
		} else if (id >= TAG_START_TEXT && id <= TAG_END_TEXT) {
			if (!ParseTextTag(tagBuffer, tag)) {
				WARN("Unable to parse tag %s", STR(name));
			}
			_metadata[name] = tag;
		} else if (id >= TAG_START_URL && id <= TAG_END_URL) {
			if (!ParseUrlTag(tagBuffer, tag)) {
				WARN("Unable to parse tag %s", STR(name));
			}
			_metadata[name] = tag;
		} else {
			WARN("tag %s not yet implemented", STR(name));
			_metadata[name] = string((char *) GETIBPOINTER(tagBuffer),
					GETAVAILABLEBYTESCOUNT(tagBuffer));
			_metadata[name].IsByteArray(true);
		}
	}
	return true;
}

bool ID3Parser::ReadStringWithSize(IOBuffer &buffer, Variant &value,
		uint32_t size, bool hasEncoding) {
	CHECK_BUFFER_SIZE(buffer, size);
	value = string(
			(char *) (GETIBPOINTER(buffer)+(hasEncoding ? 1 : 0)),
			size - (hasEncoding ? 1 : 0));
	buffer.Ignore(size);
	return true;
}

bool ID3Parser::ReadStringNullTerminated(IOBuffer &buffer, Variant &value, bool unicode) {
	string temp = "";
	while (GETAVAILABLEBYTESCOUNT(buffer) >= (uint32_t) (unicode ? 2 : 1)) {
		if (unicode) {
			if ((GETIBPOINTER(buffer)[0] == 0) && (GETIBPOINTER(buffer)[1] == 0)) {
				value = temp;
				buffer.Ignore(2);
				return true;
			}
		} else {
			if (GETIBPOINTER(buffer)[0] == 0) {
				value = temp;
				buffer.Ignore(1);
				return true;
			}
		}
		temp += (char) GETIBPOINTER(buffer)[0];
		buffer.Ignore(1);
	}
	value = "";
	return false;
}

bool ID3Parser::ParseTextTag(IOBuffer &buffer, Variant &tag) {
	return ReadStringWithSize(buffer, tag, GETAVAILABLEBYTESCOUNT(buffer), true);
}

bool ID3Parser::ParseUSLT(IOBuffer &buffer, Variant &tag) {
	CHECK_BUFFER_SIZE(buffer, 1);
	bool unicode = GETIBPOINTER(buffer)[0];
	tag["unicode"] = (bool)unicode != 0;
	buffer.Ignore(1);

	CHECK_BUFFER_SIZE(buffer, 3);
	if (!ReadStringWithSize(buffer, tag["language"], 3, false)) {
		WARN("Unable to read string");
		return false;
	}

	if (!ReadStringNullTerminated(buffer, tag["contentDescriptor"], unicode)) {
		WARN("Unable to read string");
		return false;
	}

	if (!ReadStringWithSize(buffer, tag["lyrics"], GETAVAILABLEBYTESCOUNT(buffer), false)) {
		WARN("Unable to read string");
		return false;
	}

	return true;
}

bool ID3Parser::ParseAPIC(IOBuffer &buffer, Variant &tag) {
	CHECK_BUFFER_SIZE(buffer, 1);
	bool unicode = GETIBPOINTER(buffer)[0];
	tag["unicode"] = (bool)unicode != 0;
	buffer.Ignore(1);

	if (!ReadStringNullTerminated(buffer, tag["mimeType"], false)) {
		WARN("Unable to read string");
		return false;
	}

	CHECK_BUFFER_SIZE(buffer, 1);
	tag["pictureType"] = GETIBPOINTER(buffer)[0];
	buffer.Ignore(1);

	if (!ReadStringNullTerminated(buffer, tag["description"], unicode)) {
		WARN("Unable to read string");
		return false;
	}

	tag["pictureData"] = string((char *) GETIBPOINTER(buffer),
			GETAVAILABLEBYTESCOUNT(buffer));
	tag["pictureData"].IsByteArray(true);

	return true;
}

bool ID3Parser::ParseCOMM(IOBuffer &buffer, Variant &tag) {
	CHECK_BUFFER_SIZE(buffer, 1);
	bool unicode = GETIBPOINTER(buffer)[0];
	tag["unicode"] = (bool)unicode != 0;
	buffer.Ignore(1);

	CHECK_BUFFER_SIZE(buffer, 3);
	if (!ReadStringWithSize(buffer, tag["language"], 3, false)) {
		WARN("Unable to read string");
		return false;
	}

	if (!ReadStringNullTerminated(buffer, tag["shortContentDescription"], unicode)) {
		WARN("Unable to read string");
		return false;
	}

	if (!ReadStringWithSize(buffer, tag["text"], GETAVAILABLEBYTESCOUNT(buffer), false)) {
		WARN("Unable to read string");
		return false;
	}

	return true;
}

bool ID3Parser::ParseUrlTag(IOBuffer &buffer, Variant &tag) {
	if (!ReadStringWithSize(buffer, tag, GETAVAILABLEBYTESCOUNT(buffer), false)) {
		WARN("Unable to read string");
		return false;
	}
	return true;
}

bool ID3Parser::ParseWXXX(IOBuffer &buffer, Variant &tag) {
	CHECK_BUFFER_SIZE(buffer, 1);
	bool unicode = GETIBPOINTER(buffer)[0];
	tag["unicode"] = (bool)unicode != 0;
	buffer.Ignore(1);

	if (!ReadStringNullTerminated(buffer, tag["description"], unicode)) {
		WARN("Unable to read string");
		return false;
	}

	if (!ReadStringWithSize(buffer, tag["url"], GETAVAILABLEBYTESCOUNT(buffer), false)) {
		WARN("Unable to read string");
		return false;
	}

	return true;
}

bool ID3Parser::ParseTXXX(IOBuffer &buffer, Variant &tag) {
	CHECK_BUFFER_SIZE(buffer, 1);
	bool unicode = GETIBPOINTER(buffer)[0];
	tag["unicode"] = (bool)unicode != 0;
	buffer.Ignore(1);

	if (!ReadStringNullTerminated(buffer, tag["description"], unicode)) {
		WARN("Unable to read string");
		return false;
	}

	if (!ReadStringWithSize(buffer, tag["value"], GETAVAILABLEBYTESCOUNT(buffer), false)) {
		WARN("Unable to read string");
		return false;
	}

	return true;
}

#endif /* HAS_MEDIA_MP3 */
