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

#ifdef HAS_MEDIA_MKV

#include "mediaformats/mkv/elementdummy.h"

ElementDummy::ElementDummy() {

}

ElementDummy::~ElementDummy() {
}

string ElementDummy::GetHierarchyString(uint32_t depth) {
	return string(depth * 2, ' ') + format("<%s id=\"%llx\"/>",
			STR(MKVElemToString(_id)), _id);
}

bool ElementDummy::Read() {
	WARN("Element id %llx not parsed: _elementStart: %llx; _elementSize: %llx",
			_id,
			_elementStart,
			_elementSize);
	assert(_id != 0x1f43b675);
	return true;
}

#endif /* HAS_MEDIA_MKV */
