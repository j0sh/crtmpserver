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
#include "mediaformats/mp4/atomilst.h"
#include "mediaformats/mp4/mp4document.h"
#include "mediaformats/mp4/atommetafield.h"

AtomILST::AtomILST(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start)
: BoxAtom(pDocument, type, size, start) {
	_metadata.IsArray(false);
}

AtomILST::~AtomILST() {
}

Variant &AtomILST::GetMetadata() {
	return _metadata;
}

bool AtomILST::AtomCreated(BaseAtom *pAtom) {
	if ((pAtom->GetTypeNumeric() >> 24) == 0xa9) {
		AtomMetaField *pField = (AtomMetaField *) pAtom;
		_metadata[pField->GetName()] = pField->GetValue();
		return true;
	}
	switch (pAtom->GetTypeNumeric()) {
		case A_AART:
		case A_COVR:
		case A_CPIL:
		case A_DESC:
		case A_DISK:
		case A_GNRE:
		case A_PGAP:
		case A_TMPO:
		case A_TRKN:
		case A_TVEN:
		case A_TVES:
		case A_TVSH:
		case A_TVSN:
		case A_SONM:
		case A_SOAL:
		case A_SOAR:
		case A_SOAA:
		case A_SOCO:
		case A_SOSN:
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
