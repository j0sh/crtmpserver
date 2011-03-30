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
#include "mediaformats/mp4/atomstss.h"

AtomSTSS::AtomSTSS(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start)
: VersionedAtom(pDocument, type, size, start) {

}

AtomSTSS::~AtomSTSS() {
}

vector<uint32_t> AtomSTSS::GetEntries() {
	return _entries;
}

bool AtomSTSS::ReadData() {
	uint32_t count;
	if (!ReadUInt32(count)) {
		FATAL("Unable to read count");
		return false;
	}

	for (uint32_t i = 0; i < count; i++) {
		uint32_t sampleNumber;
		if (!ReadUInt32(sampleNumber)) {
			FATAL("Unable to read sample number");
			return false;
		}

		ADD_VECTOR_END(_entries, sampleNumber);
	}
	return true;
}

#endif /* HAS_MEDIA_MP4 */
