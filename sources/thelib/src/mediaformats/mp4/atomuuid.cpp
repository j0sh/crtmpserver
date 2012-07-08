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

#include "mediaformats/mp4/atomuuid.h"

AtomUUID::AtomUUID(MP4Document *pDocument, uint32_t type, uint64_t size, uint64_t start)
: BaseAtom(pDocument, type, size, start) {

}

AtomUUID::~AtomUUID() {
}

bool AtomUUID::Read() {
	uint8_t buffer[16];
	if (!ReadArray(buffer, 16)) {
		FATAL("Unable to read UUID");
		return false;
	}
	_metadata["uuid"] = format("%02"PRIx8"%02"PRIx8"%02"PRIx8"%02"PRIx8"-%02"PRIx8"%02"PRIx8"-%02"PRIx8"%02"PRIx8"-%02"PRIx8"%02"PRIx8"-%02"PRIx8"%02"PRIx8"%02"PRIx8"%02"PRIx8"%02"PRIx8"%02"PRIx8,
			buffer[0],
			buffer[1],
			buffer[2],
			buffer[3],
			buffer[4],
			buffer[5],
			buffer[6],
			buffer[7],
			buffer[8],
			buffer[9],
			buffer[10],
			buffer[11],
			buffer[12],
			buffer[13],
			buffer[14],
			buffer[15]
			);
	if (_metadata["uuid"] == "be7acfcb-97a9-42e8-9c71-999491e3afac") {
		//http://www.adobe.com/content/dam/Adobe/en/devnet/xmp/pdfs/XMPSpecificationPart3.pdf
		string payload;
		if (!ReadString(payload, _size - 16 - 8)) {
			FATAL("Unable to read XMP");
			return false;
		}
		_metadata["payload"] = payload;
	} else {
		if ((_size - 16 - 8) > 0) {
			uint32_t tempSize = (uint32_t) _size - 16 - 8;
			uint8_t *pTemp = new uint8_t[tempSize];
			if (!ReadArray(pTemp, tempSize)) {
				FATAL("Unable to read UUID raw content");
				delete[] pTemp;
				return false;
			}
			_metadata["payload"] = Variant(pTemp, tempSize);
			delete[] pTemp;
		} else {
			_metadata["payload"] = Variant();
		}
	}
	return true;
}

string AtomUUID::Hierarchy(uint32_t indent) {

	return string(4 * indent, ' ') + GetTypeString();
}

Variant &AtomUUID::GetMetadata() {
	return _metadata;
}
