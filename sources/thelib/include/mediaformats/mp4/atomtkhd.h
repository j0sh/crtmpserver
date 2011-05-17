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
#ifndef _ATOMTKHD_H
#define _ATOMTKHD_H

#include "mediaformats/mp4/versionedatom.h"

class AtomTKHD
: public VersionedAtom {
private:
	uint32_t _creationTime;
	uint32_t _modificationTime;
	uint32_t _trackId;
	uint8_t _reserved1[4];
	uint32_t _duration;
	uint8_t _reserved2[8];
	uint16_t _layer;
	uint16_t _alternateGroup;
	uint16_t _volume;
	uint8_t _reserved3[2];
	uint8_t _matrixStructure[36];
	uint32_t _trackWidth;
	uint32_t _trackHeight;
public:
	AtomTKHD(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start);
	virtual ~AtomTKHD();

	uint32_t GetTrackId();
	uint32_t GetWidth();
	uint32_t GetHeight();
protected:
	virtual bool ReadData();
};

#endif	/* _ATOMTKHD_H */


#endif /* HAS_MEDIA_MP4 */
