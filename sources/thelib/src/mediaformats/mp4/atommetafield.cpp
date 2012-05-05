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
#include "mediaformats/mp4/atommetafield.h"
#include "mediaformats/mp4/mp4document.h"
#include "mediaformats/mp4/atomdata.h"

AtomMetaField::AtomMetaField(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start)
: BaseAtom(pDocument, type, size, start) {
}

AtomMetaField::~AtomMetaField() {
}

string &AtomMetaField::GetName() {
	return _name;
}

Variant &AtomMetaField::GetValue() {
	return _value;
}

bool AtomMetaField::Read() {
	if (GetSize() - 8 >= 8) {
		if (!GetDoc()->GetMediaFile().SeekAhead(4)) {
			FATAL("Unable to seek 4 bytes");
			return false;
		}
		uint32_t type;
		if (!ReadUInt32(type)) {
			FATAL("Unable to read type");
			return false;
		}
		if (type == A_DATA) {
			if (!GetDoc()->GetMediaFile().SeekBehind(8)) {
				FATAL("Unable to go back 8 bytes");
				return false;
			}
			AtomDATA *pAtom = (AtomDATA *) GetDoc()->ReadAtom(NULL);
			if (pAtom == NULL) {
				FATAL("Unable to read data atom");
				return false;
			}
			if ((GetTypeNumeric() >> 24) == 0xa9) {
				_name = GetTypeString().substr(1, 3);
			} else {
				_name = GetTypeString();
			}
			_value = pAtom->GetVariant();
			return GetDoc()->GetMediaFile().SeekTo(
					GetStart() + GetSize());
		} else {
			if (!GetDoc()->GetMediaFile().SeekBehind(8)) {
				FATAL("Unable to seek 8 bytes");
				return false;
			}
			return ReadSimpleString();
		}
	} else {
		return ReadSimpleString();
	}
}

string AtomMetaField::Hierarchy(uint32_t indent) {
	return string(4 * indent, ' ') + GetTypeString();
}

bool AtomMetaField::ReadSimpleString() {
	if ((GetTypeNumeric() >> 24) == 0xa9) {
		//1. Read the size
		uint16_t size;
		if (!ReadUInt16(size)) {
			FATAL("Unable to read the size");
			return false;
		}

		//2. Skip the locale
		if (!SkipBytes(2)) {
			FATAL("Unable to skip 2 bytes");
			return false;
		}

		//3. Read the value
		string val;
		if (!ReadString(val, size)) {
			FATAL("Unable to read string");
			return false;
		}

		//4. Setup the name/value
		_name = GetTypeString().substr(1, 3);
		_value = val;

		//5. Done
		return true;
	} else {
		//3. Read the value
		string val;
		if (!ReadString(val, GetSize() - 8)) {
			FATAL("Unable to read string");
			return false;
		}

		//4. Setup the name/value
		_name = GetTypeString();
		_value = val;

		//5. Done
		return true;
	}
}

#endif /* HAS_MEDIA_MP4 */
