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
#include "mediaformats/mp4/atomudta.h"
#include "mediaformats/mp4/mp4document.h"
#include "mediaformats/mp4/atommetafield.h"

AtomUDTA::AtomUDTA(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start)
: BoxAtom(pDocument, type, size, start) {
	_metadata.IsArray(false);
}

AtomUDTA::~AtomUDTA() {
}

Variant &AtomUDTA::GetMetadata() {
	return _metadata;
}

bool AtomUDTA::AtomCreated(BaseAtom *pAtom) {
	if ((pAtom->GetTypeNumeric() >> 24) == 0xa9) {
		AtomMetaField *pField = (AtomMetaField *) pAtom;
		_metadata[pField->GetName()] = pField->GetValue();
		return true;
	}
	switch (pAtom->GetTypeNumeric()) {
		case A_META:
			return true;
		case A_NAME:
		{
			AtomMetaField *pField = (AtomMetaField *) pAtom;
			_metadata[pField->GetName()] = pField->GetValue();
			return true;
		}
		default:
		{
			FATAL("Invalid atom type: %s", STR(pAtom->GetTypeString()));
			return false;
		}
	}
}

#endif /* HAS_MEDIA_MP4 */
