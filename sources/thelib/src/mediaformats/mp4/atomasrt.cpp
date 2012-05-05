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
#include "mediaformats/mp4/atomasrt.h"

AtomASRT::AtomASRT(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start)
: VersionedAtom(pDocument, type, size, start) {
	_qualityEntryCount = 0;
	_segmentRunEntryCount = 0;
}

AtomASRT::~AtomASRT() {
}

bool AtomASRT::ReadData() {
	//	FINEST("ASRT");
	if (!ReadUInt8(_qualityEntryCount)) {
		FATAL("Unable to read _qualityEntryCount");
		return false;
	}
	//	FINEST("_qualityEntryCount: %"PRIu8, _qualityEntryCount);

	for (uint8_t i = 0; i < _qualityEntryCount; i++) {
		string temp;
		if (!ReadNullTerminatedString(temp)) {
			FATAL("Unable to read _qualitySegmentUrlModifiers");
			return false;
		}
		//		FINEST("%"PRIu8": _qualitySegmentUrlModifiers: %s", i, STR(temp));
		ADD_VECTOR_END(_qualitySegmentUrlModifiers, temp);
	}

	if (!ReadUInt32(_segmentRunEntryCount)) {
		FATAL("Unable to read _segmentRunEntryCount");
		return false;
	}
	//	FINEST("_segmentRunEntryCount: %"PRIu32, _segmentRunEntryCount);

	for (uint32_t i = 0; i < _segmentRunEntryCount; i++) {
		SEGMENTRUNENTRY temp;
		if (!ReadUInt32(temp.firstSegment)) {
			FATAL("Unable to read SEGMENTRUNENTRY.FirstSegment");
			return false;
		}
		if (!ReadUInt32(temp.fragmentsPerSegment)) {
			FATAL("Unable to read SEGMENTRUNENTRY.FragmentsPerSegment");
			return false;
		}
		//		FINEST("%"PRIu32": SEGMENTRUNENTRY.FirstSegment: %"PRIu32"; SEGMENTRUNENTRY.FragmentsPerSegment: %"PRIu32,
		//				i, temp.firstSegment, temp.fragmentsPerSegment);
		ADD_VECTOR_END(_segmentRunEntryTable, temp);
	}

	return true;
}
#endif /* HAS_MEDIA_MP4 */
