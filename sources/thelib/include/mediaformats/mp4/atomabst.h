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
#ifndef _ATOMABST_H
#define _ATOMABST_H

#include "mediaformats/mp4/versionedatom.h"

class AtomABST
: public VersionedAtom {
private:
	uint32_t _bootstrapInfoVersion;
	uint8_t _profile;
	bool _live;
	bool _update;
	uint32_t _timeScale;
	uint64_t _currentMediaTime;
	uint64_t _smpteTimeCodeOffset;
	string _movieIdentifier;
	uint8_t _serverEntryCount;
	vector<string> _serverEntryTable;
	uint8_t _qualityEntryCount;
	vector<string> _qualityEntryTable;
	string _drmData;
	string _metaData;
	uint8_t _segmentRunTableCount;
	vector<BaseAtom*> _segmentRunTableEntries;
	uint8_t _fragmentRunTableCount;
	vector<BaseAtom*> _fragmentRunTableEntries;
public:
	AtomABST(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start);
	virtual ~AtomABST();
	virtual string Hierarchy(uint32_t indent);
protected:
	virtual bool ReadData();
};

#endif	/* _ATOMABST_H */
#endif /* HAS_MEDIA_MP4 */
