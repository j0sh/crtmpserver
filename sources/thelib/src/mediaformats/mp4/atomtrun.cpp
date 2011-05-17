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
#include "mediaformats/mp4/atomtrun.h"

AtomTRUN::AtomTRUN(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start)
: VersionedAtom(pDocument, type, size, start) {

}

AtomTRUN::~AtomTRUN() {
	for (uint32_t i = 0; i < _samples.size(); i++) {
		delete _samples[i];
	}
	_samples.clear();
}

bool AtomTRUN::HasDataOffset() {
	return (_flags[2]&0x01) != 0;
}

bool AtomTRUN::HasFirstSampleFlags() {
	return (_flags[2]&0x04) != 0;
}

bool AtomTRUN::HasSampleDuration() {
	return (_flags[1]&0x01) != 0;
}

bool AtomTRUN::HasSampleSize() {
	return (_flags[1]&0x02) != 0;
}

bool AtomTRUN::HasSampleFlags() {
	return (_flags[1]&0x04) != 0;
}

bool AtomTRUN::HasSampleCompositionTimeOffsets() {
	return (_flags[1]&0x08) != 0;
}

uint32_t AtomTRUN::GetDataOffset() {
	return _dataOffset;
}

vector<TRUNSample *> &AtomTRUN::GetSamples() {
	return _samples;
}

bool AtomTRUN::ReadData() {
	if (!ReadUInt32(_sampleCount)) {
		FATAL("Unable to read sample count");
		return false;
	}
	if (HasDataOffset()) {
		if (!ReadInt32(_dataOffset)) {
			FATAL("Unable to read data offset");
			return false;
		}
	}
	if (HasFirstSampleFlags()) {
		if (!ReadUInt32(_firstSampleFlags)) {
			FATAL("Unable to read first sample flags");
			return false;
		}
	}
	for (uint32_t i = 0; i < _sampleCount; i++) {
		TRUNSample *pSample = new TRUNSample();
		if (HasSampleDuration()) {
			if (!ReadUInt32(pSample->duration)) {
				FATAL("Unable to read sample duration");
				return false;
			}
		}
		if (HasSampleSize()) {
			if (!ReadUInt32(pSample->size)) {
				FATAL("Unable to read sample size");
				return false;
			}
		}
		if (HasSampleFlags()) {
			if (!ReadUInt32(pSample->flags)) {
				FATAL("Unable to read sample flags");
				return false;
			}
		}
		if (HasSampleCompositionTimeOffsets()) {
			if (!ReadUInt32(pSample->compositionTimeOffset)) {
				FATAL("Unable to read sample composition time offset");
				return false;
			}
		}
		ADD_VECTOR_END(_samples, pSample);
	}
	return true;
}
#endif	/* HAS_MEDIA_MP4 */
