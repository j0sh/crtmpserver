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
#ifndef _ATOMTRUN_H
#define	_ATOMTRUN_H

#include "mediaformats/mp4/versionedatom.h"

typedef struct _TRUNSample {
	uint32_t duration;
	uint32_t size;
	uint32_t flags;
	uint32_t compositionTimeOffset;
	int64_t absoluteOffset;

	_TRUNSample() {
		duration = size = flags = compositionTimeOffset = 0;
		absoluteOffset = 0;
	}

	operator string() {
		return format("duration: %u; size: %u; flags: %u; CTO: %u",
				duration, size, flags, compositionTimeOffset);
	}
} TRUNSample;

class AtomTRUN
: public VersionedAtom {
private:
	uint32_t _sampleCount;
	int32_t _dataOffset;
	uint32_t _firstSampleFlags;
	vector<TRUNSample *> _samples;
public:
	AtomTRUN(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start);
	virtual ~AtomTRUN();
	bool HasDataOffset();
	bool HasFirstSampleFlags();
	bool HasSampleDuration();
	bool HasSampleSize();
	bool HasSampleFlags();
	bool HasSampleCompositionTimeOffsets();
	uint32_t GetDataOffset();
	vector<TRUNSample *> &GetSamples();
protected:
	virtual bool ReadData();
};


#endif	/* _ATOMTRUN_H */
#endif	/* HAS_MEDIA_MP4 */
