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

AtomUDTA::AtomUDTA(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start)
: BoxAtom(pDocument, type, size, start) {
	_pMETA = NULL;
}

AtomUDTA::~AtomUDTA() {
}

bool AtomUDTA::Read() {
	if (_pParent == NULL)
		return SkipRead(false);
	if (_pParent->GetTypeNumeric() != A_MOOV)
		return SkipRead(false);
	return BoxAtom::Read();
}

bool AtomUDTA::AtomCreated(BaseAtom *pAtom) {
	switch (pAtom->GetTypeNumeric()) {
		case A_META:
			_pMETA = (AtomMETA *) pAtom;
			return true;
		case A_NAME:
		case A__ALB:
		case A__ART1:
		case A__ART2:
		case A__PRT:
		case A__CMT:
		case A__CPY:
		case A__DES:
		case A__NAM:
		case A__COM:
		case A__GEN:
		{
			ADD_VECTOR_END(_metaFields, (AtomMetaField *) pAtom);
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
