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

#include "mediaformats/mkv/boxelement.h"
#include "mediaformats/mkv/mkvdocument.h"

//#define DEBUG_GET_PATH(...) FINEST(__VA_ARGS__)
#define DEBUG_GET_PATH(...)

BoxElement::BoxElement() {

}

BoxElement::~BoxElement() {

	FOR_VECTOR(_elements, i) {
		delete _elements[i];
	}
	_elements.clear();
}

string BoxElement::GetHierarchyString(uint32_t depth) {
	string result = string(depth * 2, ' ') +
			format("<%s id=\"%llx\" elementsCount=\"%llu\">\n",
			STR(MKVElemToString(_id)), _id, _elements.size());

	FOR_VECTOR(_elements, i) {
		result += _elements[i]->GetHierarchyString(depth + 1) + "\n";
	}
	result += string(depth * 2, ' ') + format("</%s>", STR(MKVElemToString(_id)));
	return result;
}

bool BoxElement::Read() {
	//	if (_id == MKV_ID_Cluster)
	//		return true;
	uint64_t start = _payloadStart;

	//2. Read the elements
	while (start < _payloadStart + _payloadSize) {
		BaseElement *pElement = BaseElement::ReadElement(_pDoc, this, start, _payloadStart + _payloadSize);
		if (pElement == NULL) {
			FATAL("Unable to read element");
			return false;
		}
		ADD_VECTOR_END(_elements, pElement);
		start = pElement->ElementStart() + pElement->ElementSize();
	}

	return true;
}

vector<BaseElement *> BoxElement::GetPath(vector<uint64_t> path) {
	DEBUG_GET_PATH("********");
	if (path.size() == 0) {
		DEBUG_GET_PATH("Nothing left to search");
		DEBUG_GET_PATH("********");
		return vector<BaseElement *>();
	}

	uint64_t search = path[0];
	path.erase(path.begin());

	DEBUG_GET_PATH("Searching for %s in %s",
			STR(MKVElemToString(search)),
			STR(MKVElemToString(Id())));
	vector<BaseElement *> result;
	vector<BaseElement *> subResult;
	for (uint32_t i = 0; i < _elements.size(); i++) {
		subResult.clear();
		DEBUG_GET_PATH("Wanted: %s; Got: %s",
				STR(MKVElemToString(search)),
				STR(MKVElemToString(_elements[i]->Id()))
				);
		if (_elements[i]->Id() == search) {
			DEBUG_GET_PATH("Got it");
			if (path.size() == 0) {
				DEBUG_GET_PATH("End of path");
				ADD_VECTOR_END(subResult, _elements[i]);
			} else {
				DEBUG_GET_PATH("Continue...");
				subResult = _elements[i]->GetPath(path);
			}
		}

		FOR_VECTOR(subResult, j) {
			ADD_VECTOR_END(result, subResult[j]);
		}
	}

	DEBUG_GET_PATH("End Searching for %s in %s",
			STR(MKVElemToString(search)),
			STR(MKVElemToString(Id())));
	DEBUG_GET_PATH("********");
	return result;
}

ClusterBoxElement::ClusterBoxElement() {

}

ClusterBoxElement::~ClusterBoxElement() {
}

bool ClusterBoxElement::Read() {
	//	if (_id == MKV_ID_Cluster)
	//		return true;
	uint64_t start = _payloadStart;

	//2. Read the elements
	while (start < _payloadStart + _payloadSize) {
		BaseElement *pElement = BaseElement::ReadElement(_pDoc, this, start, _payloadStart + _payloadSize);
		if (pElement == NULL) {
			FATAL("Unable to read element");
			return false;
		}
		ADD_VECTOR_END(_elements, pElement);
		start = pElement->ElementStart() + pElement->ElementSize();
		//		if (pElement->Id() == MKV_ID_Timecode)
		//			break;
	}

	return true;
}

#endif	/* HAS_MEDIA_MKV */
