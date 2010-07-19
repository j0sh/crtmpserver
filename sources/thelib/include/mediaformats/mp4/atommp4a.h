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
#ifndef _ATOMMP4A_H
#define _ATOMMP4A_H

class AtomESDS;
class AtomWAVE;
class AtomCHAN;

#include "mediaformats/mp4/versionedboxatom.h"

class AtomMP4A
: public VersionedBoxAtom {
protected:
	AtomESDS *_pESDS;
	AtomWAVE *_pWAVE;
	AtomCHAN *_pCHAN;
	uint16_t _dataReferenceIndex;
	uint16_t _innerVersion;
	uint16_t _revisionLevel;
	uint32_t _vendor;
	uint16_t _numberOfChannels;
	uint16_t _sampleSizeInBits;
	int16_t _compressionId;
	uint16_t _packetSize;
	uint32_t _sampleRate;
	uint32_t _samplesPerPacket;
	uint32_t _bytesPerPacket;
	uint32_t _bytesPerFrame;
	uint32_t _bytesPerSample;
public:
	AtomMP4A(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start);
	virtual ~AtomMP4A();


protected:
	virtual bool ReadData();
	virtual bool AtomCreated(BaseAtom *pAtom);
};

#endif	/* _ATOMMP4A_H */


#endif /* HAS_MEDIA_MP4 */
