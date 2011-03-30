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
#ifndef _ATOMSTSC_H
#define _ATOMSTSC_H

#include "mediaformats/mp4/versionedatom.h"

typedef struct _STSCEntry {
	uint32_t firstChunk;
	uint32_t samplesPerChunk;
	uint32_t sampleDescriptionIndex;
} STSCEntry;

class AtomSTSC
: public VersionedAtom {
private:
	vector<STSCEntry> _stscEntries;
	vector<uint32_t> _normalizedEntries;
public:
	AtomSTSC(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start);
	virtual ~AtomSTSC();

	vector<uint32_t> GetEntries(uint32_t totalChunksCount);
protected:
	virtual bool ReadData();
};

#endif	/* _ATOMSTSC_H */


#endif /* HAS_MEDIA_MP4 */
