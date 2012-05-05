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
#ifndef _ATOMAFRA_H
#define _ATOMAFRA_H

#include "mediaformats/mp4/versionedatom.h"

struct AFRAENTRY {
	uint64_t time;
	uint64_t offset;
};

struct GLOBALAFRAENTRY {
	uint64_t time;
	uint32_t segment;
	uint32_t fragment;
	uint64_t afraOffset;
	uint64_t offsetFromAfra;
};

class AtomAFRA
: public VersionedAtom {
private:
	uint8_t _flags;
	uint32_t _timeScale;
	uint32_t _entryCount;
	uint32_t _globalEntryCount;
	vector<AFRAENTRY> _localAccessEntries;
	vector<GLOBALAFRAENTRY> _globalAccessEntries;
public:
	AtomAFRA(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start);
	virtual ~AtomAFRA();
protected:
	virtual bool ReadData();
};

#endif	/* _ATOMAFRA_H */
#endif /* HAS_MEDIA_MP4 */
