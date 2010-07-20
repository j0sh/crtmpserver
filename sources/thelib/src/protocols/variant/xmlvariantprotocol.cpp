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
#include "protocols/variant/xmlvariantprotocol.h"

XmlVariantProtocol::XmlVariantProtocol()
: BaseVariantProtocol(PT_XML_VAR) {

}

XmlVariantProtocol::~XmlVariantProtocol() {
}

bool XmlVariantProtocol::Serialize(string &rawData, Variant &variant) {
	return variant.SerializeToXml(rawData, false);
}

bool XmlVariantProtocol::Deserialize(uint8_t *pBuffer, uint32_t bufferLength,
		Variant &result) {
	return Variant::DeserializeFromXml(pBuffer, bufferLength, result);
}
#endif	/* HAS_PROTOCOL_VAR */

