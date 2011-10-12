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
}

AtomILST::~AtomILST() {
}

Variant AtomILST::GetVariant() {
	Variant result;

	FOR_VECTOR(_metaFields, i) {
		result[_metaFields[i]->GetTypeString()] = _metaFields[i]->GetVariant();
	}
	return result;
}

bool AtomILST::AtomCreated(BaseAtom *pAtom) {
	switch (pAtom->GetTypeNumeric()) {
		case A__NAM:
		case A_CPIL:
		case A_PGAP:
		case A_TMPO:
		case A__TOO:
		case A__ART1:
		case A__ART2:
		case A__PRT:
		case A__ALB:
		case A_GNRE:
		case A_TRKN:
		case A__DAY:
		case A_DISK:
		case A__CMT:
		case A_COVR:
		case A_AART:
		case A__WRT:
		case A__GRP:
		case A__LYR:
		case A_DESC:
		case A_TVSH:
		case A_TVEN:
		case A_TVSN:
		case A_TVES:
		case A__DES:
			ADD_VECTOR_END(_metaFields, (AtomMetaField *) pAtom);
			return true;
		default:
		{
			FATAL("Invalid atom type: %s", STR(pAtom->GetTypeString()));
			return false;
		}
	}
}

#endif /* HAS_MEDIA_MP4 */
