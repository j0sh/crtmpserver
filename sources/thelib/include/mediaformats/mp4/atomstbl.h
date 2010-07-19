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
#ifndef _ATOMSTBL_H
#define _ATOMSTBL_H

#include "mediaformats/mp4/boxatom.h"

class AtomSTSD;
class AtomSTTS;
class AtomSTSC;
class AtomSTSZ;
class AtomSTCO;
class AtomCO64;
class AtomCTTS;
class AtomSTSS;

class AtomSTBL
: public BoxAtom {
private:
	AtomSTSD *_pSTSD;
	AtomSTTS *_pSTTS;
	AtomSTSC *_pSTSC;
	AtomSTSZ *_pSTSZ;
	AtomSTCO *_pSTCO;
	AtomCO64 *_pCO64;
	AtomCTTS *_pCTTS;
	AtomSTSS *_pSTSS;
public:
	AtomSTBL(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start);
	virtual ~AtomSTBL();


protected:
	virtual bool AtomCreated(BaseAtom *pAtom);
};

#endif	/* _ATOMSTBL_H */


#endif /* HAS_MEDIA_MP4 */
