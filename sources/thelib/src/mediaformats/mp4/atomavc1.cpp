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
#include "mediaformats/mp4/atomavc1.h"
#include "mediaformats/mp4/mp4document.h"
#include "mediaformats/mp4/atomavcc.h"

AtomAVC1::AtomAVC1(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start)
: VersionedBoxAtom(pDocument, type, size, start) {
	_pAVCC = NULL;
	_reserved = 0;
	_referenceIndex = 0;
	_qtVideoEncodingVersion = 0;
	_qtVideoEncodingRevisionLevel = 0;
	_qtVideoEncodingVendor = 0;
	_qtVideoTemporalQuality = 0;
	_qtVideoSpatialQuality = 0;
	_videoFramePixelSize = 0;
	_horizontalDpi = 0;
	_verticalDpi = 0;
	_qtVideoDataSize = 0;
	_videoFrameCount = 0;
	_videoEncoderNameLength = 0;
	_videoPixelDepth = 0;
	_qtVideoColorTableId = 0;
}

AtomAVC1::~AtomAVC1() {
}

bool AtomAVC1::ReadData() {
	if (!ReadUInt16(_reserved)) {
		FATAL("Unable to read _reserved");
		return false;
	}

	if (!ReadUInt16(_referenceIndex)) {
		FATAL("Unable to read _referenceIndex");
		return false;
	}

	if (!ReadUInt16(_qtVideoEncodingVersion)) {
		FATAL("Unable to read _qtVideoEncodingVersion");
		return false;
	}

	if (!ReadUInt16(_qtVideoEncodingRevisionLevel)) {
		FATAL("Unable to read _qtVideoEncodingRevisionLevel");
		return false;
	}

	if (!ReadUInt32(_qtVideoEncodingVendor)) {
		FATAL("Unable to read _qtVideoEncodingVendor");
		return false;
	}

	if (!ReadUInt32(_qtVideoTemporalQuality)) {
		FATAL("Unable to read _qtVideoTemporalQuality");
		return false;
	}

	if (!ReadUInt32(_qtVideoSpatialQuality)) {
		FATAL("Unable to read _qtVideoSpatialQuality");
		return false;
	}

	if (!ReadUInt32(_videoFramePixelSize)) {
		FATAL("Unable to read _videoFramePixelSize");
		return false;
	}

	if (!ReadUInt32(_horizontalDpi)) {
		FATAL("Unable to read _horizontalDpi");
		return false;
	}

	if (!ReadUInt32(_verticalDpi)) {
		FATAL("Unable to read _verticalDpi");
		return false;
	}

	if (!ReadUInt32(_qtVideoDataSize)) {
		FATAL("Unable to read _qtVideoDataSize");
		return false;
	}

	if (!ReadUInt16(_videoFrameCount)) {
		FATAL("Unable to read _videoFrameCount");
		return false;
	}

	if (!ReadUInt8(_videoEncoderNameLength)) {
		FATAL("Unable to read _videoEncoderNameLength");
		return false;
	}

	if (_videoEncoderNameLength < 31)
		_videoEncoderNameLength = 31;

	uint8_t *pTemp = new uint8_t[_videoEncoderNameLength];
	if (!ReadArray(pTemp, _videoEncoderNameLength)) {
		FATAL("Unable to read _videoEncoderNameLength");
		delete[] pTemp;
		return false;
	}
	_videoEncoderName = string((char *) pTemp, _videoEncoderNameLength);
	delete[] pTemp;

	if (!ReadUInt16(_videoPixelDepth)) {
		FATAL("Unable to read _videoPixelDepth");
		return false;
	}

	if (!ReadUInt16(_qtVideoColorTableId)) {
		FATAL("Unable to read _qtVideoColorTableId");
		return false;
	}
	if (_qtVideoColorTableId != 0xffff) {
		FATAL("_qtVideoColorTableId not supported yet");
		return false;
	}

	return true;
}

bool AtomAVC1::AtomCreated(BaseAtom *pAtom) {
	switch (pAtom->GetTypeNumeric()) {
			//TODO: What is the deal with all this order stuff!?
		case A_AVCC:
			_pAVCC = (AtomAVCC *) pAtom;
			return true;
		default:
		{
			FATAL("Invalid atom type: %s", STR(pAtom->GetTypeString()));
			return false;
		}
	}
}

#endif /* HAS_MEDIA_MP4 */
