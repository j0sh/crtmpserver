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
#include "mediaformats/mp4/atomafra.h"

AtomAFRA::AtomAFRA(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start)
: VersionedAtom(pDocument, type, size, start) {
	_flags = 0;
	_timeScale = 0;
	_entryCount = 0;
	_globalEntryCount = 0;
}

AtomAFRA::~AtomAFRA() {
}

bool AtomAFRA::ReadData() {
	if (!ReadUInt8(_flags)) {
		FATAL("Unable to read flags");
		return false;
	}

	if (!ReadUInt32(_timeScale)) {
		FATAL("Unable to read timeScale");
		return false;
	}


	if (!ReadUInt32(_entryCount)) {
		FATAL("Unable to read entryCount");
		return false;
	}

	bool longOffsets = (_flags >> 6)&0x01;
	bool longIds = (_flags >> 7)&0x01;
	bool globalEntries = (_flags >> 5)&0x01;

	for (uint32_t i = 0; i < _entryCount; i++) {
		AFRAENTRY temp = {0, 0};
		if (!ReadUInt64(temp.time)) {
			FATAL("Unable to read AFRAENTRY.Time");
			return false;
		}
		if (longOffsets) {
			if (!ReadUInt64(temp.offset)) {
				FATAL("Unable to read AFRAENTRY.Offset");
				return false;
			}
		} else {
			uint32_t ui32;
			if (!ReadUInt32(ui32)) {
				FATAL("Unable to read AFRAENTRY.Offset");
				return false;
			}
			temp.offset = ui32;
		}
		//		FINEST("%"PRIu32": AFRAENTRY.Time: %"PRIu64"; AFRAENTRY.Offset: %"PRIu64, i, temp.time, temp.offset);
		ADD_VECTOR_END(_localAccessEntries, temp);
	}

	if (globalEntries) {
		if (!ReadUInt32(_globalEntryCount)) {
			FATAL("Unable to read globalEntryCount");
			return false;
		}
		for (uint32_t i = 0; i < _entryCount; i++) {
			GLOBALAFRAENTRY temp = {0, 0, 0, 0, 0};
			if (!ReadUInt64(temp.time)) {
				FATAL("Unable to read AFRAENTRY.Time");
				return false;
			}

			//Segment,fragment
			if (longIds) {
				if (!ReadUInt32(temp.segment)) {
					FATAL("Unable to read GLOBALAFRAENTRY.Segment");
					return false;
				}
				if (!ReadUInt32(temp.fragment)) {
					FATAL("Unable to read GLOBALAFRAENTRY.Fragment");
					return false;
				}
			} else {
				uint16_t ui16;
				if (!ReadUInt16(ui16)) {
					FATAL("Unable to read GLOBALAFRAENTRY.Segment");
					return false;
				}
				temp.segment = ui16;

				if (!ReadUInt16(ui16)) {
					FATAL("Unable to read GLOBALAFRAENTRY.Fragment");
					return false;
				}
				temp.fragment = ui16;
			}

			//AfraOffset, OffsetFromAfra
			if (longOffsets) {
				if (!ReadUInt64(temp.afraOffset)) {
					FATAL("Unable to read GLOBALAFRAENTRY.AfraOffset");
					return false;
				}
				if (!ReadUInt64(temp.offsetFromAfra)) {
					FATAL("Unable to read GLOBALAFRAENTRY.OffsetFromAfra");
					return false;
				}
			} else {
				uint32_t ui32;
				if (!ReadUInt32(ui32)) {
					FATAL("Unable to read GLOBALAFRAENTRY.AfraOffset");
					return false;
				}
				temp.afraOffset = ui32;

				if (!ReadUInt32(ui32)) {
					FATAL("Unable to read GLOBALAFRAENTRY.OffsetFromAfra");
					return false;
				}
				temp.offsetFromAfra = ui32;
			}


			//			FINEST("%"PRIu32": GLOBALAFRAENTRY.Time: %"PRIu64"; GLOBALAFRAENTRY.Segment: %"PRIu32"; GLOBALAFRAENTRY.Fragment: %"PRIu32"; GLOBALAFRAENTRY.AfraOffset: %"PRIu64"; GLOBALAFRAENTRY.OffsetFromAfra: %"PRIu64,
			//					i, temp.time, temp.segment, temp.fragment, temp.afraOffset, temp.offsetFromAfra);
			ADD_VECTOR_END(_globalAccessEntries, temp);
		}
	}

	//	FINEST("%02"PRIx8"; timeScale: %"PRIu32"; entryCount: %"PRIu32"; globalEntryCount: %"PRIu32,
	//			_flags, _timeScale, _entryCount, _globalEntryCount);

	return true;
}
#endif /* HAS_MEDIA_MP4 */
