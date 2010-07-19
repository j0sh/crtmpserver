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
#ifndef _ATOMAVC1_H
#define _ATOMAVC1_H

#include "mediaformats/mp4/versionedboxatom.h"

class AtomAVCC;

class AtomAVC1
: public VersionedBoxAtom {
private:
	AtomAVCC *_pAVCC;
	uint16_t _reserved;
	uint16_t _referenceIndex;
	uint16_t _qtVideoEncodingVersion;
	uint16_t _qtVideoEncodingRevisionLevel;
	uint32_t _qtVideoEncodingVendor;
	uint32_t _qtVideoTemporalQuality;
	uint32_t _qtVideoSpatialQuality;
	uint32_t _videoFramePixelSize;
	uint32_t _horizontalDpi;
	uint32_t _verticalDpi;
	uint32_t _qtVideoDataSize;
	uint16_t _videoFrameCount;
	uint8_t _videoEncoderNameLength;
	string _videoEncoderName;
	uint16_t _videoPixelDepth;
	uint16_t _qtVideoColorTableId;
public:
	AtomAVC1(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start);
	virtual ~AtomAVC1();

protected:
	virtual bool ReadData();
	virtual bool AtomCreated(BaseAtom *pAtom);
};

#endif	/* _ATOMAVC1_H */


#endif /* HAS_MEDIA_MP4 */
