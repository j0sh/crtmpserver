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
#include "mediaformats/mkv/binaryelement.h"
#include "mediaformats/mkv/mkvdocument.h"

BinaryElement::BinaryElement() {

}

BinaryElement::~BinaryElement() {
}

string BinaryElement::GetHierarchyString(uint32_t depth) {
	return string(depth * 2, ' ') + format("<%s id=\"%llx\" size=\"%llu\"/>",
			STR(MKVElemToString(_id)),
			_id, _payloadSize);
}

bool BinaryElement::ReadBuffer(IOBuffer &buffer) {
	buffer.ReadFromRepeat(0, _payloadSize);
	_pDoc->GetMediaFile().SeekTo(_payloadStart);
	return _pDoc->GetMediaFile().ReadBuffer(GETIBPOINTER(buffer), _payloadSize);
}

bool BinaryElement::Read() {
	return true;
}
#endif	/* HAS_MEDIA_MKV */
