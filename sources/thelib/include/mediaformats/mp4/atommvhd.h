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
#ifndef _ATOMMVHD_H
#define _ATOMMVHD_H

#include "mediaformats/mp4/versionedatom.h"

class AtomMVHD
: public VersionedAtom {
private:
	uint32_t _creationTime;
	uint32_t _modificationTime;
	uint32_t _timeScale;
	uint32_t _duration;
	uint32_t _preferredRate;
	uint16_t _preferredVolume;
	uint8_t _reserved[10];
	uint32_t _matrixStructure[9];
	uint32_t _previewTime;
	uint32_t _previewDuration;
	uint32_t _posterTime;
	uint32_t _selectionTime;
	uint32_t _selectionDuration;
	uint32_t _currentTime;
	uint32_t _nextTrakId;
public:
	AtomMVHD(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start);
	virtual ~AtomMVHD();

protected:
	virtual bool ReadData();
};

#endif	/* _ATOMMVHD_H */


#endif /* HAS_MEDIA_MP4 */
