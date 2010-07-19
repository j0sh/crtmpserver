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
: BoxAtom(pDocument, type, size, start) {
	_pDATA = NULL;
}

AtomMetaField::~AtomMetaField() {
}

bool AtomMetaField::Read() {
	if (GetSize() >= 8) {
		if (!GetDoc()->GetMediaFile().SeekAhead(4)) {
			FATAL("Unable to seek 4 bytes");
			return false;
		}
		uint32_t type;
		if (!ReadUInt32(type, false)) {
			FATAL("Unable to read type");
			return false;
		}
		if (type != A_DATA) {
			if (!GetDoc()->GetMediaFile().SeekBehind(4)) {
				FATAL("Unable to go back 4 bytes");
				return false;
			}
			if (!ReadString(_stringData, GetSize() - 8 - 4)) {
				FATAL("Unable to read string");
				return false;
			}
			return true;
		} else {
			if (!GetDoc()->GetMediaFile().SeekBehind(8)) {
				FATAL("Unable to go back 8 bytes");
				return false;
			}
			return BoxAtom::Read();
		}
	} else {
		return BoxAtom::Read();
	}
}

bool AtomMetaField::AtomCreated(BaseAtom *pAtom) {
	switch (pAtom->GetTypeNumeric()) {
		case A_DATA:
			_pDATA = (AtomDATA *) pAtom;
			return true;
		default:
		{
			FATAL("Invalid atom type: %s", STR(pAtom->GetTypeString()));
			return false;
		}
	}
}

Variant AtomMetaField::GetVariant() {
	if (_pDATA != NULL)
		return _pDATA->GetVariant();
	return _stringData;
}

#endif /* HAS_MEDIA_MP4 */
