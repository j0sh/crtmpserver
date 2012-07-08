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

#ifndef _ATOMUUID_H
#define _ATOMUUID_H

#include "mediaformats/mp4/baseatom.h"

class AtomUUID
: public BaseAtom {
private:
	Variant _metadata;
public:
	AtomUUID(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start);
	virtual ~AtomUUID();

	virtual bool Read();
	string Hierarchy(uint32_t indent);

	Variant &GetMetadata();
};

#endif	/* _ATOMUUID_H */

