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
#include "mediaformats/mp4/atomstbl.h"
#include "mediaformats/mp4/mp4document.h"

AtomSTBL::AtomSTBL(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start)
: BoxAtom(pDocument, type, size, start) {
	_pSTSD = NULL;
	_pSTTS = NULL;
	_pSTSC = NULL;
	_pSTSZ = NULL;
	_pSTCO = NULL;
	_pCTTS = NULL;
	_pSTSS = NULL;
}

AtomSTBL::~AtomSTBL() {
}

bool AtomSTBL::AtomCreated(BaseAtom *pAtom) {
	switch (pAtom->GetTypeNumeric()) {
		case A_STSD:
			_pSTSD = (AtomSTSD *) pAtom;
			return true;
		case A_STTS:
			_pSTTS = (AtomSTTS *) pAtom;
			return true;
		case A_STSC:
			_pSTSC = (AtomSTSC *) pAtom;
			return true;
		case A_STSZ:
			_pSTSZ = (AtomSTSZ *) pAtom;
			return true;
		case A_STCO:
			_pSTCO = (AtomSTCO *) pAtom;
			return true;
		case A_CO64:
			_pCO64 = (AtomCO64 *) pAtom;
			return true;
		case A_CTTS:
			_pCTTS = (AtomCTTS *) pAtom;
			return true;
		case A_STSS:
			_pSTSS = (AtomSTSS *) pAtom;
			return true;
		default:
		{
			FATAL("Invalid atom type: %s", STR(pAtom->GetTypeString()));
			return false;
		}
	}
}

#endif /* HAS_MEDIA_MP4 */
