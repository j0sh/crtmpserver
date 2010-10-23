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

#include "mediaformats/mkv/floatelement.h"
#include "mediaformats/mkv/mkvdocument.h"

FloatElement::FloatElement() {
	_value = 0;
}

FloatElement::~FloatElement() {
}

string FloatElement::GetHierarchyString(uint32_t depth) {
	return string(depth * 2, ' ') + format("<%s id=\"%llx\" value=\"%.4f\"/>",
			STR(MKVElemToString(_id)), _id, _value);
}

double FloatElement::Value() {
	return _value;
}

bool FloatElement::Read() {
	if (_payloadSize == 4) {
		uint32_t temp;
		if (!_pDoc->GetMediaFile().ReadBuffer(((uint8_t *) & temp), 4)) {
			FATAL("Unable to read buffer");
			return false;
		}
		temp = ntohl(temp);
		_value = (double) (*((float *) &temp));
		return true;
	} else if (_payloadSize == 8) {
		uint64_t temp;
		if (!_pDoc->GetMediaFile().ReadBuffer(((uint8_t *) & temp), 8)) {
			FATAL("Unable to read buffer");
			return false;
		}
		temp = ntohll(temp);
		_value = *((double *) &temp);
		return true;
	} else {
		FATAL("Invalid payload size");
		return false;
	}
}

#endif	/* HAS_MEDIA_MKV */