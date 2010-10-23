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
#ifndef _BASEELEMENT_H
#define	_BASEELEMENT_H

#include "common.h"
#include "mediaformats/mediafile.h"
#include "mediaformats/mkv/elementids.h"

class MKVDocument;
class BoxElement;

class BaseElement {
protected:
	MKVDocument *_pDoc;
	BoxElement *_pParent;
	uint64_t _elementStart;
	uint64_t _elementSize;
	uint64_t _payloadStart;
	uint64_t _payloadSize;
	uint64_t _id;
public:
	BaseElement();
	virtual ~BaseElement();

	BoxElement *Parent();
	uint64_t Id();
	uint64_t ElementStart();
	uint64_t ElementSize();
	uint64_t PayloadStart();
	uint64_t PayloadSize();

	operator string();

	virtual vector<BaseElement *> GetPath(uint32_t depth, ...);
	virtual vector<BaseElement *> GetPath(vector<uint64_t> path);

	virtual string GetHierarchyString(uint32_t depth) = 0;

	static BaseElement *ReadElement(MKVDocument *pDoc, BoxElement *pParent, uint64_t start, uint64_t max);
protected:
	static bool ReadCUI(MediaFile &mf, uint64_t &value, bool removeLeadingBit);
	virtual bool Read() = 0;
};

#endif	/* _BASEELEMENT_H */
#endif /* HAS_MEDIA_MKV */
