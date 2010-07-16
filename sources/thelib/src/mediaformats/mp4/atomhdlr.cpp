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


#include "mediaformats/mp4/atomhdlr.h"

AtomHDLR::AtomHDLR(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start)
: VersionedAtom(pDocument, type, size, start) {
    _componentType = 0;
    _componentSubType = 0;
    _componentManufacturer = 0;
    _componentFlags = 0;
    _componentFlagsMask = 0;
    _componentName = "";
}

AtomHDLR::~AtomHDLR() {
}

uint32_t AtomHDLR::GetComponentSubType() {
    return _componentSubType;
}

bool AtomHDLR::ReadData() {
    if (!ReadUInt32(_componentType)) {
        FATAL("Unable to read component type");
        return false;
    }
    //FINEST("_componentType: %u", _componentType);

    if (!ReadUInt32(_componentSubType)) {
        FATAL("Unable to read component sub type");
        return false;
    }
    //    FINEST("_componentSubType: %s(%u)", STR(U32TOS(_componentSubType)),
    //            _componentSubType);

    if (!ReadUInt32(_componentManufacturer)) {
        FATAL("Unable to read component manufacturer");
        return false;
    }
    //FINEST("_componentManufacturer: %u", _componentManufacturer);

    if (!ReadUInt32(_componentFlags)) {
        FATAL("Unable to read component flags");
        return false;
    }
    //FINEST("_componentFlags: %u", _componentFlags);

    if (!ReadUInt32(_componentFlagsMask)) {
        FATAL("Unable to read component flags mask");
        return false;
    }
    //FINEST("_componentFlagsMask: %u", _componentFlagsMask);

    if (!ReadString(_componentName, _size - 32)) {
        FATAL("Unable to read component name");
        return false;
    }
    //FINEST("_componentFlagsMask: %s", STR(_componentName));

    return true;
}

string AtomHDLR::Hierarchy(uint32_t indent) {
    return string(4 * indent, ' ') + GetTypeString() + "(" + U32TOS(_componentSubType) + ")";
}


