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
#ifndef _ATOMESDS_H
#define _ATOMESDS_H

#include "mediaformats/mp4/versionedatom.h"

class AtomESDS
: public VersionedAtom {
private:
	uint16_t _MP4ESDescrTag_ID;
	uint8_t _MP4ESDescrTag_Priority;
	uint8_t _MP4DecConfigDescrTag_ObjectTypeID;
	uint8_t _MP4DecConfigDescrTag_StreamType;
	uint32_t _MP4DecConfigDescrTag_BufferSizeDB;
	uint32_t _MP4DecConfigDescrTag_MaxBitRate;
	uint32_t _MP4DecConfigDescrTag_AvgBitRate;
	uint64_t _extraDataStart;
	uint64_t _extraDataLength;
#ifdef DEBUG_ESDS_ATOM
	uint8_t _objectType;
	uint8_t _sampleRate;
	uint8_t _channels;
	uint8_t _extObjectType;
	uint8_t _sbr;
	uint8_t _extSampleRate;
#endif /* DEBUG_ESDS_ATOM */
public:
	AtomESDS(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start);
	virtual ~AtomESDS();
	uint64_t GetExtraDataStart();
	uint64_t GetExtraDataLength();
protected:
	virtual bool ReadData();
private:
	bool ReadTagLength(uint32_t &length);
	bool ReadTagAndLength(uint8_t &tagType, uint32_t &length);
};

#endif	/* _ATOMESDS_H */


#endif /* HAS_MEDIA_MP4 */
