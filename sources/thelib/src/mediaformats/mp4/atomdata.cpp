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
#include "mediaformats/mp4/atomdata.h"

AtomDATA::AtomDATA(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start)
: BaseAtom(pDocument, type, size, start) {

}

AtomDATA::~AtomDATA() {
}

Variant AtomDATA::GetVariant() {
	switch (_type) {
		case 1:
		{
			//Single string
			return _dataString;
		}
		case 0:
		{
			//many uint16_t
			Variant result;

			FOR_VECTOR(_dataUI16, i) {
				result[i] = _dataUI16[i];
			}
			result.IsArray(true);
			return result;
		}
		case 21:
		{
			//many uint8_t
			Variant result;

			FOR_VECTOR(_dataUI8, i) {
				result[i] = _dataUI8[i];
			}
			result.IsArray(true);
			return result;
		}
		case 14:
		case 15:
		{
			//jpg/png image data
			Variant result = _dataImg;
			result.IsByteArray(true);
			return result;
			//TODO: for now, return null
		}
		default:
		{
			FATAL("Type %u not yet implemented", _type);
			return false;
		}
	}
}

bool AtomDATA::Read() {
	//1. Read the type
	if (!ReadUInt32(_type)) {
		FATAL("Unable to read type");
		return false;
	}

	//2. Read unknown 4 bytes
	if (!ReadUInt32(_unknown)) {
		FATAL("Unable to read type");
		return false;
	}

	switch (_type) {
		case 1:
		{
			//Single string
			if (!ReadString(_dataString, GetSize() - 8 - 8)) {
				FATAL("Unable to read string");
				return false;
			}
			return true;
		}
		case 0:
		{
			//many uint16_t
			uint64_t count = (GetSize() - 8 - 8) / 2;
			for (uint64_t i = 0; i < count; i++) {
				uint16_t val;
				if (!ReadUInt16(val)) {
					FATAL("Unable to read value");
					return false;
				}
				ADD_VECTOR_END(_dataUI16, val);
			}
			return true;
		}
		case 21:
		{
			//many uint8_t
			uint64_t count = GetSize() - 8 - 8;
			for (uint64_t i = 0; i < count; i++) {
				uint8_t val;
				if (!ReadUInt8(val)) {
					FATAL("Unable to read value");
					return false;
				}
				ADD_VECTOR_END(_dataUI8, val);
			}
			return true;
		}
		case 14:
		case 15:
		{
			if (!ReadString(_dataImg, GetSize() - 8 - 8)) {
				FATAL("Unable to read data");
				return false;
			}
			return true;
		}
		default:
		{
			FATAL("Type %u not yet implemented", _type);
			return false;
		}
	}
}

std::string AtomDATA::Hierarchy(uint32_t indent) {
	return std::string(4 * indent, ' ') + GetTypeString();
}


#endif /* HAS_MEDIA_MP4 */
