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
#ifndef _ATOMDATA_H
#define _ATOMDATA_H

#include "mediaformats/mp4/baseatom.h"

class AtomDATA
: public BaseAtom {
private:
	uint32_t _type;
	uint32_t _unknown;

	string _dataString;
	vector<uint16_t> _dataUI16;
	vector<uint8_t> _dataUI8;
	string _dataImg;
public:
	AtomDATA(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start);
	virtual ~AtomDATA();

	Variant GetVariant();

	virtual bool Read();
	std::string Hierarchy(uint32_t indent);
};

#endif	/* _ATOMDATA_H */


#endif /* HAS_MEDIA_MP4 */
