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

#ifdef HAS_MEDIA_MP4
#include "mediaformats/mp4/atomesds.h"

#define MP4ESDescrTag                   0x03
#define MP4DecConfigDescrTag            0x04
#define MP4DecSpecificDescrTag          0x05
#define MP4UnknownTag                   0x06

AtomESDS::AtomESDS(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start)
: VersionedAtom(pDocument, type, size, start) {
	_MP4ESDescrTag_ID = 0;
	_MP4ESDescrTag_Priority = 0;
	_MP4DecConfigDescrTag_ObjectTypeID = 0;
	_MP4DecConfigDescrTag_StreamType = 0;
	_MP4DecConfigDescrTag_BufferSizeDB = 0;
	_MP4DecConfigDescrTag_MaxBitRate = 0;
	_MP4DecConfigDescrTag_AvgBitRate = 0;
	_extraDataStart = 0;
	_extraDataLength = 0;
#ifdef DEBUG_ESDS_ATOM
	_objectType = 0;
	_sampleRate = 0;
	_channels = 0;
	_extObjectType = 0;
	_sbr = 0;
	_extSampleRate = 0;
#endif /* DEBUG_ESDS_ATOM */
}

AtomESDS::~AtomESDS() {
}

uint64_t AtomESDS::GetExtraDataStart() {
	return _extraDataStart;
}

uint64_t AtomESDS::GetExtraDataLength() {
	return _extraDataLength;
}

bool AtomESDS::ReadTagLength(uint32_t &length) {
	length = 0;
	uint32_t count = 4;
	while (count--) {
		uint8_t c = 0;
		if (!ReadUInt8(c))
			return false;

		length = (length << 7) | (c & 0x7f);
		if (!(c & 0x80))
			break;
	}
	return true;
}

bool AtomESDS::ReadTagAndLength(uint8_t &tagType, uint32_t &length) {
	if (!ReadUInt8(tagType))
		return false;
	if (!ReadTagLength(length))
		return false;
	return true;
}

bool AtomESDS::ReadData() {
	uint8_t tagType = 0;
	uint32_t length = 0;

	if (!ReadTagAndLength(tagType, length)) {
		FATAL("Unable to read tag type and length");
		return false;
	}

	if (tagType == MP4ESDescrTag) {
		if (!ReadUInt16(_MP4ESDescrTag_ID)) {
			FATAL("Unable to read _MP4ESDescrTag_ID");
			return false;
		}

		if (!ReadUInt8(_MP4ESDescrTag_Priority)) {
			FATAL("Unable to read _MP4ESDescrTag_Priority");
			return false;
		}
	} else {
		if (!ReadUInt16(_MP4ESDescrTag_ID)) {
			FATAL("Unable to read _MP4ESDescrTag_ID");
			return false;
		}
	}

	if (!ReadTagAndLength(tagType, length)) {
		FATAL("Unable to read tag type and length");
		return false;
	}

	if (tagType == MP4DecConfigDescrTag) {
		if (!ReadUInt8(_MP4DecConfigDescrTag_ObjectTypeID)) {
			FATAL("Unable to read _MP4DecConfigDescrTag_ObjectTypeID");
			return false;
		}

		if (!ReadUInt8(_MP4DecConfigDescrTag_StreamType)) {
			FATAL("Unable to read _MP4DecConfigDescrTag_StreamType");
			return false;
		}

		if (!ReadUInt24(_MP4DecConfigDescrTag_BufferSizeDB)) {
			FATAL("Unable to read _MP4DecConfigDescrTag_BufferSizeDB");
			return false;
		}

		if (!ReadUInt32(_MP4DecConfigDescrTag_MaxBitRate)) {
			FATAL("Unable to read _MP4DecConfigDescrTag_MaxBitRate");
			return false;
		}

		if (!ReadUInt32(_MP4DecConfigDescrTag_AvgBitRate)) {
			FATAL("Unable to read _MP4DecConfigDescrTag_AvgBitRate");
			return false;
		}

		if (!ReadTagAndLength(tagType, length)) {
			FATAL("Unable to read tag type and length");
			return false;
		}

		if (tagType == MP4UnknownTag) {
			uint8_t unknownValue;
			if (!ReadUInt8(unknownValue)) {
				FATAL("Unable to read unknownValue");
				return false;
			}
			
			if (!ReadTagAndLength(tagType, length)) {
				FATAL("Unable to read tag type and length");
				return false;
			}
		}

		if (tagType == MP4DecSpecificDescrTag) {
			//iso14496-3
			//http://wiki.multimedia.cx/index.php?title=MPEG-4_Audio
			_extraDataStart = CurrentPosition();
			_extraDataLength = length;
			return SkipRead(false);
		}
	}
	FATAL("No MP4DecSpecificDescrTag found");
	return false;
}

#endif /* HAS_MEDIA_MP4 */
