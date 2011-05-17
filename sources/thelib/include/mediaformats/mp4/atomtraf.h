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
#ifndef _ATOMTRAF_H
#define	_ATOMTRAF_H

#include "mediaformats/mp4/boxatom.h"

class AtomTFHD;
class AtomTRUN;

class AtomTRAF
: public BoxAtom {
private:
	AtomTFHD *_pTFHD;
	vector<AtomTRUN *> _runs;
public:
	AtomTRAF(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start);
	virtual ~AtomTRAF();
	uint32_t GetId();
	vector<AtomTRUN *> &GetRuns();
protected:
	virtual bool AtomCreated(BaseAtom *pAtom);
};


#endif	/* _ATOMTRAF_H */
#endif /* HAS_MEDIA_MP4 */
