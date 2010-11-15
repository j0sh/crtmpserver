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

#include "mediaformats/mkv/intelement.h"
#include "mediaformats/mkv/mkvdocument.h"

IntElement::IntElement() {
	_value = 0;
}

IntElement::~IntElement() {
}

string IntElement::GetHierarchyString(uint32_t depth) {
	return string(depth * 2, ' ') + format("<%s id=\"%llx\" value=\"%llx\"/>",
			STR(MKVElemToString(_id)), _id, _value);
}

uint64_t IntElement::UnsignedValue() {
	return _value;
}

int64_t IntElement::SignedValue() {
	return (int64_t) _value;
}

bool IntElement::Read() {
	if (_payloadSize > 8) {
		FATAL("Invalid payload size for integer: %d", _payloadSize);
		return false;
	}
	_value = 0;
	_pDoc->GetMediaFile().SeekTo(_payloadStart);
	if (!_pDoc->GetMediaFile().ReadBuffer(((uint8_t *) & _value) + 8 - _payloadSize, _payloadSize)) {
		FATAL("Unable to read buffer");
		return false;
	}
	_value = ENTOHLLP((uint8_t *) & _value);
	return true;
}

#endif	/* HAS_MEDIA_MKV */
