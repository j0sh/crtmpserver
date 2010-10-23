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
#ifndef _BOXELEMENT_H
#define	_BOXELEMENT_H

#include "mediaformats/mkv/baseelement.h"

class BoxElement
: public BaseElement {
protected:
	vector<BaseElement *> _elements;
public:
	BoxElement();
	virtual ~BoxElement();

	virtual string GetHierarchyString(uint32_t depth);
protected:
	virtual bool Read();
	vector<BaseElement *> GetPath(vector<uint64_t> path);
};

class ClusterBoxElement
: public BoxElement {
public:
	ClusterBoxElement();
	virtual ~ClusterBoxElement();
protected:
	virtual bool Read();
};

#endif	/* _BOXELEMENT_H */
#endif	/* HAS_MEDIA_MKV */

