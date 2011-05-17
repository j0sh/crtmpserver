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
#ifndef _ATOMTFHD_H
#define	_ATOMTFHD_H

#include "mediaformats/mp4/versionedatom.h"

class AtomTFHD
: public VersionedAtom {
private:
	int32_t _trackID;
	int64_t _baseDataOffset;
	int32_t _sampleDescriptionIndex;
	int32_t _defaultSampleDuration;
	int32_t _defaultSampleSize;
	int32_t _defaultSampleFlags;
public:
	AtomTFHD(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start);
	virtual ~AtomTFHD();
	uint32_t GetTrackId();
	int64_t GetBaseDataOffset();
	bool HasBaseDataOffset();
	bool HasSampleDescriptionIndex();
	bool HasDefaultSampleDuration();
	bool HasDefaultSampleSize();
	bool HasDefaultSampleFlags();
	bool DurationIsEmpty();
protected:
	virtual bool ReadData();
};


#endif	/* _ATOMTFHD_H */
#endif /* HAS_MEDIA_MP4 */
