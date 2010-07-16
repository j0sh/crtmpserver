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


#include "mediaformats/mp4/atommvhd.h"

AtomMVHD::AtomMVHD(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start)
: VersionedAtom(pDocument, type, size, start) {
    _creationTime = 0;
    _modificationTime = 0;
    _timeScale = 0;
    _duration = 0;
    _preferredRate = 0;
    _preferredVolume = 0;
    memset(_reserved, 0, 10);
    memset(_matrixStructure, 0, 36);
    _previewTime = 0;
    _previewDuration = 0;
    _posterTime = 0;
    _selectionTime = 0;
    _selectionDuration = 0;
    _currentTime = 0;
    _nextTrakId = 0;
}

AtomMVHD::~AtomMVHD() {
}

bool AtomMVHD::ReadData() {
    if (!ReadUInt32(_creationTime)) {
        FATAL("Unable to read creation time");
        return false;
    }
    //FINEST("_creationTime: %u", _creationTime);

    if (!ReadUInt32(_modificationTime)) {
        FATAL("Unable to read modification time");
        return false;
    }
    //FINEST("_modificationTime: %u", _modificationTime);

    if (!ReadUInt32(_timeScale)) {
        FATAL("Unable to read time scale");
        return false;
    }
    //FINEST("_timeScale: %u", _timeScale);

    if (!ReadUInt32(_duration)) {
        FATAL("Unable to read duration");
        return false;
    }
    //FINEST("_duration: %u", _duration);

    if (!ReadUInt32(_preferredRate)) {
        FATAL("Unable to read preferred rate");
        return false;
    }
    //FINEST("_preferredRate: %u", _preferredRate);

    if (!ReadUInt16(_preferredVolume)) {
        FATAL("Unable to read preferred volume");
        return false;
    }
    //FINEST("_preferredVolume: %u", _preferredVolume);

    if (!ReadArray(_reserved, 10)) {
        FATAL("Unable to read reserved");
        return false;
    }
    //    FINEST("_reserved: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x",
    //            _reserved[0], _reserved[1], _reserved[2], _reserved[3],
    //    _reserved[4], _reserved[5], _reserved[6], _reserved[7],
    //            _reserved[8], _reserved[9]);

    if (!ReadArray((uint8_t *) _matrixStructure, 36)) {
        FATAL("Unable to read matrix structure");
        return false;
    }

    if (!ReadUInt32(_previewTime)) {
        FATAL("Unable to read preview time");
        return false;
    }
    //FINEST("_previewTime: %u", _previewTime);

    if (!ReadUInt32(_previewDuration)) {
        FATAL("Unable to read preview duration");
        return false;
    }
    //FINEST("_previewDuration: %u", _previewDuration);

    if (!ReadUInt32(_posterTime)) {
        FATAL("Unable to read poster time");
        return false;
    }
    //FINEST("_posterTime: %u", _posterTime);

    if (!ReadUInt32(_selectionTime)) {
        FATAL("Unable to read selection time");
        return false;
    }
    //FINEST("_selectionTime: %u", _selectionTime);

    if (!ReadUInt32(_selectionDuration)) {
        FATAL("Unable to read selection duration");
        return false;
    }
    //FINEST("_selectionDuration: %u", _selectionDuration);

    if (!ReadUInt32(_currentTime)) {
        FATAL("Unable to read current time");
        return false;
    }
    //FINEST("_currentTime: %u", _currentTime);

    if (!ReadUInt32(_nextTrakId)) {
        FATAL("Unable to read next track ID");
        return false;
    }
    //FINEST("_nextTrakId: %u", _nextTrakId);

    return true;
}


