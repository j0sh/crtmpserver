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

#ifdef HAS_PROTOCOL_VAR
#include "protocols/variant/jsonvariantprotocol.h"

JsonVariantProtocol::JsonVariantProtocol()
: BaseVariantProtocol(PT_JSON_VAR) {

}

JsonVariantProtocol::~JsonVariantProtocol() {
}

bool JsonVariantProtocol::Serialize(string &rawData, Variant &variant) {
	return variant.SerializeToJSON(rawData);
}

bool JsonVariantProtocol::Deserialize(uint8_t *pBuffer, uint32_t bufferLength,
		Variant &result) {
	string raw = string((char *) pBuffer, bufferLength);
	uint32_t start = 0;
	return Variant::DeserializeFromJSON(raw, result, start);
}
#endif	/* HAS_PROTOCOL_VAR */

