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
#include "mediaformats/mp4/atommdhd.h"

AtomMDHD::AtomMDHD(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start)
: VersionedAtom(pDocument, type, size, start) {
	_creationTime = 0;
	_modificationTime = 0;
	_timeScale = 0;
	_duration = 0;
	_language = 0;
	_quality = 0;
}

AtomMDHD::~AtomMDHD() {
}

uint32_t AtomMDHD::GetTimeScale() {
	return _timeScale;
}

bool AtomMDHD::ReadData() {
	if (_version == 1) {
		return ReadDataVersion1();
	} else {
		return ReadDataVersion0();
	}
}

bool AtomMDHD::ReadDataVersion0() {
	uint32_t temp = 0;
	if (!ReadUInt32(temp)) {
		FATAL("Unable to read creation time");
		return false;
	}
	_creationTime = temp;

	if (!ReadUInt32(temp)) {
		FATAL("Unable to read modification time");
		return false;
	}
	_modificationTime = temp;

	if (!ReadUInt32(_timeScale)) {
		FATAL("Unable to read time scale");
		return false;
	}

	if (!ReadUInt32(temp)) {
		FATAL("Unable to read duration");
		return false;
	}
	_duration = temp;

	if (!ReadUInt16(_language)) {
		FATAL("Unable to read language");
		return false;
	}

	if (!ReadUInt16(_quality)) {
		FATAL("Unable to read quality");
		return false;
	}

	return true;
}

bool AtomMDHD::ReadDataVersion1() {
	if (!ReadUInt64(_creationTime)) {
		FATAL("Unable to read creation time");
		return false;
	}

	if (!ReadUInt64(_modificationTime)) {
		FATAL("Unable to read modification time");
		return false;
	}

	if (!ReadUInt32(_timeScale)) {
		FATAL("Unable to read time scale");
		return false;
	}

	if (!ReadUInt64(_duration)) {
		FATAL("Unable to read duration");
		return false;
	}

	if (!ReadUInt16(_language)) {
		FATAL("Unable to read language");
		return false;
	}

	if (!ReadUInt16(_quality)) {
		FATAL("Unable to read quality");
		return false;
	}

	return true;
}

#endif /* HAS_MEDIA_MP4 */
