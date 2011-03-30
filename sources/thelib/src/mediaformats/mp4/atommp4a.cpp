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
#include "mediaformats/mp4/atommp4a.h"
#include "mediaformats/mp4/mp4document.h"

AtomMP4A::AtomMP4A(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start)
: VersionedBoxAtom(pDocument, type, size, start) {
	_pESDS = NULL;
	_pWAVE = NULL;
	_pCHAN = NULL;
	_dataReferenceIndex = 0;
	_innerVersion = 0;
	_revisionLevel = 0;
	_vendor = 0;
	_numberOfChannels = 0;
	_sampleSizeInBits = 0;
	_compressionId = 0;
	_packetSize = 0;
	_sampleRate = 0;
	_samplesPerPacket = 0;
	_bytesPerPacket = 0;
	_bytesPerFrame = 0;
	_bytesPerSample = 0;
}

AtomMP4A::~AtomMP4A() {
}

bool AtomMP4A::ReadData() {
	//qtff.pdf: General StructureofaSampleDescription (76/304)

	//TODO: there seems to be 2 kinds of DISTINCT mp4a atoms. If the size
	// of the atom is 0x0c (size+'mp4a'+4 zeros) we should ignore it for now...
	if (_size == 0x0c) {
		WARN("Another strange mp4a atom....");
		return true;
	}

	//1. Skip reserved 6 bytes. Actually, slip the next 2, because the prev 4
	//are already skipped by the versioned atom
	if (!SkipBytes(2)) {
		FATAL("Unable to skip 2 bytes");
		return false;
	}

	if (!ReadUInt16(_dataReferenceIndex)) {
		FATAL("Unable to read count");
		return false;
	}

	if (!ReadUInt16(_innerVersion)) {
		FATAL("Unable to read count");
		return false;
	}

	if (!ReadUInt16(_revisionLevel)) {
		FATAL("Unable to read count");
		return false;
	}

	if (!ReadUInt32(_vendor)) {
		FATAL("Unable to read count");
		return false;
	}

	if (!ReadUInt16(_numberOfChannels)) {
		FATAL("Unable to read count");
		return false;
	}

	if (!ReadUInt16(_sampleSizeInBits)) {
		FATAL("Unable to read count");
		return false;
	}

	if (!ReadInt16(_compressionId)) {
		FATAL("Unable to read count");
		return false;
	}

	if (!ReadUInt16(_packetSize)) {
		FATAL("Unable to read count");
		return false;
	}

	if (!ReadUInt32(_sampleRate)) {
		FATAL("Unable to read count");
		return false;
	}

	if (_innerVersion == 0) {
		return true;
	}


	if (!ReadUInt32(_samplesPerPacket)) {
		FATAL("Unable to read count");
		return false;
	}

	if (!ReadUInt32(_bytesPerPacket)) {
		FATAL("Unable to read count");
		return false;
	}

	if (!ReadUInt32(_bytesPerFrame)) {
		FATAL("Unable to read count");
		return false;
	}

	if (!ReadUInt32(_bytesPerSample)) {
		FATAL("Unable to read count");
		return false;
	}

	return true;
}

bool AtomMP4A::AtomCreated(BaseAtom *pAtom) {
	switch (pAtom->GetTypeNumeric()) {
		case A_ESDS:
			_pESDS = (AtomESDS *) pAtom;
			return true;
		case A_WAVE:
			_pWAVE = (AtomWAVE *) pAtom;
			return true;
		case A_CHAN:
			_pCHAN = (AtomCHAN *) pAtom;
			return true;
		default:
		{
			FATAL("Invalid atom type: %s", STR(pAtom->GetTypeString()));
			return false;
		}
	}
}

#endif /* HAS_MEDIA_MP4 */
