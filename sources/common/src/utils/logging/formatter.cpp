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

#include "utils/logging/formatter.h"

Formatter::Formatter() {

}

Formatter::~Formatter() {
	for (uint32_t i = 0; i < _fields.size(); i++) {
		delete _fields[i];
	}
	_fields.clear();
}

Formatter* Formatter::GetInstance(string formatString) {
	Formatter *pResult = new Formatter();
	if (!pResult->Init(formatString)) {
		delete pResult;
		return NULL;
	}
	return pResult;
}

bool Formatter::Init(string formatString) {
	return true; //there is a mem leak somewhere here
	string replacement = format("##__%s__##", STR(generateRandomString(5)));
	_formatString = formatString;

	//1. escape %%
	replace(_formatString, "%%", replacement);

	bool insideField = false;
	LogField *pTemp = new LogField();
	for (uint32_t i = 0; i < _formatString.length(); i++) {
		if (pTemp == NULL) {
			pTemp = new LogField;
		}
		if (_formatString[i] == '%') {
			if (insideField) {
				pTemp->isField = true;
				replace(pTemp->value, replacement, "%%");
				if (pTemp->value != "") {
					ADD_VECTOR_END(_fields, pTemp);
				}
				pTemp = NULL;
				insideField = false;
			} else {
				pTemp->isField = false;
				replace(pTemp->value, replacement, "%%");
				if (pTemp->value != "") {
					ADD_VECTOR_END(_fields, pTemp);
				}
				pTemp = NULL;
				insideField = true;
			}
		} else {
			pTemp->value += _formatString[i];
		}
	}
	if (pTemp != NULL) {
		pTemp->isField = false;
		replace(pTemp->value, replacement, "%%");
		if (pTemp->value != "") {
			ADD_VECTOR_END(_fields, pTemp);
		}
		pTemp = NULL;
	}

	return true;
}

string Formatter::Format(Variant &le) {
	string result;
	for (uint32_t i = 0; i < _fields.size(); i++) {
		LogField *pTemp = _fields[i];
		if (!pTemp->isField) {
			result += pTemp->value;
		} else {
			if (le.HasKey(pTemp->value)) {
				if (le[pTemp->value] != V_NULL)
					result += (string) (le[pTemp->value]);
			} else if (le["carrier"].HasKey(pTemp->value)) {
				if (le["carrier"][pTemp->value] != V_NULL)
					result += (string) (le["carrier"][pTemp->value]);
			} else if (le["fields"].HasKey(pTemp->value)) {
				if (le["fields"][pTemp->value] != V_NULL)
					result += (string) (le["fields"][pTemp->value]);
			}
		}
	}
	return result;
}
