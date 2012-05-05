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
#include "mediaformats/mp4/atomabst.h"
#include "mediaformats/mp4/mp4document.h"

AtomABST::AtomABST(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start)
: VersionedAtom(pDocument, type, size, start) {
	_bootstrapInfoVersion = 0;
	_profile = 0;
	_live = false;
	_update = false;
	_timeScale = 0;
	_currentMediaTime = 0;
	_smpteTimeCodeOffset = 0;
	_serverEntryCount = 0;
	_qualityEntryCount = 0;
	_segmentRunTableCount = 0;
	_fragmentRunTableCount = 0;
}

AtomABST::~AtomABST() {
}

string AtomABST::Hierarchy(uint32_t indent) {
	string result = string(4 * indent, ' ') + GetTypeString();
	if (_segmentRunTableCount > 0) {
		result += "\n";
		for (uint8_t i = 0; i < _segmentRunTableCount; i++) {
			result += _segmentRunTableEntries[i]->Hierarchy(indent + 1);
			if (i != (_segmentRunTableCount - 1))
				result += "\n";
		}
	}
	if (_fragmentRunTableCount > 0) {
		result += "\n";
		for (uint8_t i = 0; i < _fragmentRunTableCount; i++) {
			result += _fragmentRunTableEntries[i]->Hierarchy(indent + 1);
			if (i != (_fragmentRunTableCount - 1))
				result += "\n";
		}
	}
	return result;
}

bool AtomABST::ReadData() {
	if (!ReadUInt32(_bootstrapInfoVersion)) {
		FATAL("Unable to read _bootstrapInfoVersion");
		return false;
	}
	//	FINEST("_bootstrapInfoVersion: %"PRIu32, _bootstrapInfoVersion);

	uint8_t ui8;
	if (!ReadUInt8(ui8)) {
		FATAL("Unable to read flags");
		return false;
	}

	_profile = (ui8 >> 6)&0x03;
	_live = (ui8 >> 5)&0x01;
	_update = (ui8 >> 4)&0x01;
	//	FINEST("_profile: %"PRIu8"; _live: %d; _update: %d", _profile, _live, _update);

	if (!ReadUInt32(_timeScale)) {
		FATAL("Unable to read _timeScale");
		return false;
	}
	//	FINEST("_timeScale: %"PRIu32, _timeScale);

	if (!ReadUInt64(_currentMediaTime)) {
		FATAL("Unable to read _currentMediaTime");
		return false;
	}
	//	FINEST("_currentMediaTime: %"PRIu64, _currentMediaTime);

	if (!ReadUInt64(_smpteTimeCodeOffset)) {
		FATAL("Unable to read _smpteTimeCodeOffset");
		return false;
	}
	//	FINEST("_smpteTimeCodeOffset: %"PRIu64, _smpteTimeCodeOffset);

	if (!ReadNullTerminatedString(_movieIdentifier)) {
		FATAL("Unable to read _movieIdentifier");
		return false;
	}
	//	FINEST("_movieIdentifier: %s", STR(_movieIdentifier));

	if (!ReadUInt8(_serverEntryCount)) {
		FATAL("Unable to read _serverEntryCount");
		return false;
	}
	//	FINEST("_serverEntryCount: %"PRIu8, _serverEntryCount);

	for (uint8_t i = 0; i < _serverEntryCount; i++) {
		string temp;
		if (!ReadNullTerminatedString(temp)) {
			FATAL("Unable to read SERVERENTRY.serverBaseURL");
			return false;
		}
		//		FINEST("%"PRIu8"; SERVERENTRY.serverBaseURL: %s", i, STR(temp));
		ADD_VECTOR_END(_serverEntryTable, temp);
	}

	if (!ReadUInt8(_qualityEntryCount)) {
		FATAL("Unable to read _qualityEntryCount");
		return false;
	}
	//	FINEST("_qualityEntryCount: %"PRIu8, _qualityEntryCount);

	for (uint8_t i = 0; i < _qualityEntryCount; i++) {
		string temp;
		if (!ReadNullTerminatedString(temp)) {
			FATAL("Unable to read QUALITYENTRY.qualitySegmentUrlModifier");
			return false;
		}
		//		FINEST("%"PRIu8"; QUALITYENTRY.qualitySegmentUrlModifier: %s", i, STR(temp));
		ADD_VECTOR_END(_qualityEntryTable, temp);
	}

	if (!ReadNullTerminatedString(_drmData)) {
		FATAL("Unable to read _drmData");
		return false;
	}
	//	FINEST("_drmData: %s", STR(_drmData));

	if (!ReadNullTerminatedString(_metaData)) {
		FATAL("Unable to read _metaData");
		return false;
	}
	//	FINEST("_metaData: %s", STR(_metaData));

	if (!ReadUInt8(_segmentRunTableCount)) {
		FATAL("Unable to read _segmentRunTableCount");
		return false;
	}
	//	FINEST("_segmentRunTableCount: %"PRIu8, _segmentRunTableCount);

	for (uint8_t i = 0; i < _segmentRunTableCount; i++) {
		BaseAtom *pAtom = GetDoc()->ReadAtom(this);
		if (pAtom == NULL) {
			FATAL("Unable to read atoms");
			return false;
		}
		//		FINEST("%"PRIu8"; _segmentRunTableEntries: %p", i, pAtom);
		ADD_VECTOR_END(_segmentRunTableEntries, pAtom);
	}

	if (!ReadUInt8(_fragmentRunTableCount)) {
		FATAL("Unable to read _fragmentRunTableCount");
		return false;
	}
	//	FINEST("_fragmentRunTableCount: %"PRIu8, _fragmentRunTableCount);

	for (uint8_t i = 0; i < _fragmentRunTableCount; i++) {
		BaseAtom *pAtom = GetDoc()->ReadAtom(this);
		if (pAtom == NULL) {
			FATAL("Unable to read atoms");
			return false;
		}
		//		FINEST("%"PRIu8"; _fragmentRunTableEntries: %p", i, pAtom);
		ADD_VECTOR_END(_fragmentRunTableEntries, pAtom);
	}

	return true;
}

#endif /* HAS_MEDIA_MP4 */

