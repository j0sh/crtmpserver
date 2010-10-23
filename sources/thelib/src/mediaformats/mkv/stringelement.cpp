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
#include "mediaformats/mkv/stringelement.h"
#include "mediaformats/mkv/mkvdocument.h"

StringElement::StringElement() {
	_value = "";
}

StringElement::~StringElement() {
	_value = "";
}

string StringElement::GetHierarchyString(uint32_t depth) {
	return string(depth * 2, ' ') + format("<%s id=\"%llx\" value=\"%s\"/>",
			STR(MKVElemToString(_id)), _id, STR(_value));
}

string StringElement::Value() {
	return _value;
}

bool StringElement::Read() {
	char *pTemp = new char[_payloadSize];
	_pDoc->GetMediaFile().SeekTo(_payloadStart);
	if (!_pDoc->GetMediaFile().ReadBuffer((uint8_t *) pTemp, _payloadSize)) {
		FATAL("Unable to read %d bytes", _payloadSize);
		delete[] pTemp;
		return false;
	}
	_value = string(pTemp, _payloadSize);
	delete[] pTemp;
	return true;
}

#endif	/* HAS_MEDIA_MKV */
