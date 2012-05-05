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
#ifndef _ATOMASRT_H
#define _ATOMASRT_H

#include "mediaformats/mp4/versionedatom.h"

struct SEGMENTRUNENTRY {
	uint32_t firstSegment;
	uint32_t fragmentsPerSegment;
};

class AtomASRT
: public VersionedAtom {
private:
	uint8_t _qualityEntryCount;
	vector<string> _qualitySegmentUrlModifiers;
	uint32_t _segmentRunEntryCount;
	vector<SEGMENTRUNENTRY> _segmentRunEntryTable;
public:
	AtomASRT(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start);
	virtual ~AtomASRT();
protected:
	virtual bool ReadData();
};

#endif	/* _ATOMASRT_H */
#endif /* HAS_MEDIA_MP4 */
