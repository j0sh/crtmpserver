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
#include "mediaformats/mp4/atomtfhd.h"

AtomTFHD::AtomTFHD(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start)
: VersionedAtom(pDocument, type, size, start) {
	_trackID = 0;
	_baseDataOffset = 0;
	_sampleDescriptionIndex = 0;
	_defaultSampleDuration = 0;
	_defaultSampleSize = 0;
	_defaultSampleFlags = 0;
}

AtomTFHD::~AtomTFHD() {
}

uint32_t AtomTFHD::GetTrackId() {
	return _trackID;
}

int64_t AtomTFHD::GetBaseDataOffset() {
	return _baseDataOffset;
}

bool AtomTFHD::HasBaseDataOffset() {
	return (_flags[2]&0x01) != 0;
}

bool AtomTFHD::HasSampleDescriptionIndex() {
	return (_flags[2]&0x02) != 0;
}

bool AtomTFHD::HasDefaultSampleDuration() {
	return (_flags[2]&0x08) != 0;
}

bool AtomTFHD::HasDefaultSampleSize() {
	return (_flags[2]&0x10) != 0;
}

bool AtomTFHD::HasDefaultSampleFlags() {
	return (_flags[2]&0x20) != 0;
}

bool AtomTFHD::DurationIsEmpty() {
	return (_flags[0]&0x01) != 0;
}

bool AtomTFHD::ReadData() {
	if (!ReadInt32(_trackID)) {
		FATAL("Unable to read track ID");
		return false;
	}
	if (HasBaseDataOffset()) {
		if (!ReadInt64(_baseDataOffset)) {
			FATAL("Unable to read base data offset");
			return false;
		}
	}
	if (HasSampleDescriptionIndex()) {
		if (!ReadInt32(_sampleDescriptionIndex)) {
			FATAL("Unable to read sample description index");
			return false;
		}
	}
	if (HasDefaultSampleDuration()) {
		if (!ReadInt32(_defaultSampleDuration)) {
			FATAL("Unable to read default sample duration");
			return false;
		}
	}
	if (HasDefaultSampleSize()) {
		if (!ReadInt32(_defaultSampleSize)) {
			FATAL("Unable to read default sample size");
			return false;
		}
	}
	if (HasDefaultSampleFlags()) {
		if (!ReadInt32(_defaultSampleFlags)) {
			FATAL("Unable to read default sample flags");
			return false;
		}
	}
	return true;
}
#endif /* HAS_MEDIA_MP4 */
