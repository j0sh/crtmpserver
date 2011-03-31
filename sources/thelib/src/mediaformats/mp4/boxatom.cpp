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
#include "mediaformats/mp4/boxatom.h"
#include "mediaformats/mp4/mp4document.h"

BoxAtom::BoxAtom(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start)
: BaseAtom(pDocument, type, size, start) {

}

BoxAtom::~BoxAtom() {
}

bool BoxAtom::Read() {
	while (CurrentPosition() != _start + _size) {
		BaseAtom *pAtom = GetDoc()->ReadAtom(this);
		if (pAtom == NULL) {
			FATAL("Unable to read atom. Parent atom is %s",
					STR(GetTypeString()));
			return false;
		}
		if (!pAtom->IsIgnored()) {
			if (!AtomCreated(pAtom)) {
				FATAL("Unable to signal AtomCreated for atom %s (%"PRIx64")",
						STR(GetTypeString()), _start);
				return false;
			}
		}
		ADD_VECTOR_END(_subAtoms, pAtom);
	}
	return true;
}

string BoxAtom::Hierarchy(uint32_t indent) {
	string result = string(indent * 4, ' ') + GetTypeString() + "\n";
	if (_subAtoms.size() == 0) {
		result += string((indent + 1) * 4, ' ') + "[empty]";
		return result;
	}
	for (uint32_t i = 0; i < _subAtoms.size(); i++) {
		result += _subAtoms[i]->Hierarchy(indent + 1);
		if (i != _subAtoms.size() - 1)
			result += "\n";
	}
	return result;
}

BaseAtom * BoxAtom::GetPath(uint8_t depth, ...) {
	vector<uint32_t> path;
	va_list arguments;
	va_start(arguments, depth);
	for (uint8_t i = 0; i < depth; i++) {
		uint32_t pathElement = va_arg(arguments, uint32_t);
		ADD_VECTOR_END(path, pathElement);
	}
	va_end(arguments);
	if (path.size() == 0)
		return NULL;
	return GetPath(path);
}

BaseAtom * BoxAtom::GetPath(vector<uint32_t> path) {
	if (path.size() == 0)
		return NULL;

	uint32_t search = path[0];
	path.erase(path.begin());
	for (uint32_t i = 0; i < _subAtoms.size(); i++) {
		if (_subAtoms[i]->GetTypeNumeric() == search) {
			if (path.size() == 0)
				return _subAtoms[i];
			return _subAtoms[i]->GetPath(path);
		}
	}

	return NULL;
}

#endif /* HAS_MEDIA_MP4 */
