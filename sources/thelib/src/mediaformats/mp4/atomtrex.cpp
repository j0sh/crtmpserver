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
#include "mediaformats/mp4/atomtrex.h"

AtomTREX::AtomTREX(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start)
: VersionedAtom(pDocument, type, size, start) {
	_trackID = 0;
	_defaultSampleDescriptionIndex = 0;
	_defaultSampleDuration = 0;
	_defaultSampleSize = 0;
	_defaultSampleFlags = 0;
}

AtomTREX::~AtomTREX() {
}

uint32_t AtomTREX::GetTrackID() {
	return _trackID;
}

uint32_t AtomTREX::GetDefaultSampleDescriptionIndex() {
	return _defaultSampleDescriptionIndex;
}

uint32_t AtomTREX::GetDefaultSampleDuration() {
	return _defaultSampleDuration;
}

uint32_t AtomTREX::GetDefaultSampleSize() {
	return _defaultSampleSize;
}

uint32_t AtomTREX::GetDefaultSampleFlags() {
	return _defaultSampleFlags;
}

bool AtomTREX::ReadData() {
	if (!ReadUInt32(_trackID)) {
		FATAL("Unable to read count");
		return false;
	}
	if (!ReadUInt32(_defaultSampleDescriptionIndex)) {
		FATAL("Unable to read count");
		return false;
	}
	if (!ReadUInt32(_defaultSampleDuration)) {
		FATAL("Unable to read count");
		return false;
	}
	if (!ReadUInt32(_defaultSampleSize)) {
		FATAL("Unable to read count");
		return false;
	}
	if (!ReadUInt32(_defaultSampleFlags)) {
		FATAL("Unable to read count");
		return false;
	}
	return true;
}
#endif /* HAS_MEDIA_MP4 */
