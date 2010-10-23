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

#ifdef HAS_MEDIA_MKV
#ifndef _INTELEMENT_H
#define	_INTELEMENT_H

#include "mediaformats/mkv/baseelement.h"

class IntElement
: public BaseElement {
private:
	uint64_t _value;
public:
	IntElement();
	virtual ~IntElement();

	virtual string GetHierarchyString(uint32_t depth);
	
	uint64_t UnsignedValue();
	int64_t SignedValue();
protected:
	virtual bool Read();
};

#endif	/* _INTELEMENT_H */
#endif	/* HAS_MEDIA_MKV */
