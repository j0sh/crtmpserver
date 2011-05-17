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
#include "mediaformats/mp4/atomtrak.h"

#include "mediaformats/mp4/mp4document.h"
#include "mediaformats/mp4/atomtkhd.h"

AtomTRAK::AtomTRAK(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start)
: BoxAtom(pDocument, type, size, start) {
	_pTKHD = NULL;
	_pMDIA = NULL;
	_pHDLR = NULL;
	_pMINF = NULL;
	_pDINF = NULL;
	_pSTBL = NULL;
	_pUDTA = NULL;
}

AtomTRAK::~AtomTRAK() {
}

uint32_t AtomTRAK::GetId() {
	if (_pTKHD != NULL)
		return _pTKHD->GetTrackId();
	return 0;
}

bool AtomTRAK::AtomCreated(BaseAtom *pAtom) {
	switch (pAtom->GetTypeNumeric()) {
		case A_TKHD:
			_pTKHD = (AtomTKHD *) pAtom;
			return true;
		case A_MDIA:
			_pMDIA = (AtomMDIA *) pAtom;
			return true;
		case A_HDLR:
			_pHDLR = (AtomHDLR *) pAtom;
			return true;
		case A_MINF:
			_pMINF = (AtomMINF *) pAtom;
			return true;
		case A_DINF:
			_pDINF = (AtomDINF *) pAtom;
			return true;
		case A_STBL:
			_pSTBL = (AtomSTBL *) pAtom;
			return true;
		case A_UDTA:
			_pUDTA = (AtomUDTA *) pAtom;
			return true;
		case A_META:
			_pMETA = (AtomMETA *) pAtom;
			return true;
		default:
		{
			FATAL("Invalid atom type: %s", STR(pAtom->GetTypeString()));
			return false;
		}
	}
}

#endif /* HAS_MEDIA_MP4 */
