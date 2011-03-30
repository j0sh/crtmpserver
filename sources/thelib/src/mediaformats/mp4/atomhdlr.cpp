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

	if (!ReadUInt32(_componentSubType)) {
		FATAL("Unable to read component sub type");
		return false;
	}

	if (!ReadUInt32(_componentManufacturer)) {
		FATAL("Unable to read component manufacturer");
		return false;
	}

	if (!ReadUInt32(_componentFlags)) {
		FATAL("Unable to read component flags");
		return false;
	}

	if (!ReadUInt32(_componentFlagsMask)) {
		FATAL("Unable to read component flags mask");
		return false;
	}

	if (!ReadString(_componentName, _size - 32)) {
		FATAL("Unable to read component name");
		return false;
	}

	return true;
}

string AtomHDLR::Hierarchy(uint32_t indent) {
	return string(4 * indent, ' ') + GetTypeString() + "(" + U32TOS(_componentSubType) + ")";
}


#endif /* HAS_MEDIA_MP4 */
