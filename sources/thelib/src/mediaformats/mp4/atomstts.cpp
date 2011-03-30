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
#include "mediaformats/mp4/atomstts.h"

AtomSTTS::AtomSTTS(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start)
: VersionedAtom(pDocument, type, size, start) {
}

AtomSTTS::~AtomSTTS() {
}

vector<uint32_t> AtomSTTS::GetEntries() {
	if (_normalizedEntries.size() != 0)
		return _normalizedEntries;

	FOR_VECTOR_ITERATOR(STTSEntry, _sttsEntries, i) {
		for (uint32_t j = 0; j < VECTOR_VAL(i).count; j++) {
			ADD_VECTOR_END(_normalizedEntries, VECTOR_VAL(i).delta);
		}
	}
	return _normalizedEntries;
}

bool AtomSTTS::ReadData() {
	uint32_t entryCount;
	if (!ReadUInt32(entryCount)) {
		FATAL("Unable to read entry count");
		return false;
	}

	for (uint32_t i = 0; i < entryCount; i++) {
		STTSEntry entry;

		if (!ReadUInt32(entry.count)) {
			FATAL("Unable to read count");
			return false;
		}

		if (!ReadUInt32(entry.delta)) {
			FATAL("Unable to read delta");
			return false;
		}

		ADD_VECTOR_END(_sttsEntries, entry);
	}
	return true;
}


#endif /* HAS_MEDIA_MP4 */
