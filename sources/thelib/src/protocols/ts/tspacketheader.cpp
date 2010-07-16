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


#ifdef HAS_PROTOCOL_TS
#include "protocols/ts/tspacketheader.h"

#include "utils/core.h"

TSPacketHeader::TSPacketHeader() {
    *((uint32_t *) _header.raw) = 0;
}

TSPacketHeader::~TSPacketHeader() {
}

bool TSPacketHeader::Read(IOBuffer &buffer) {

    _header.raw[0] = GETIBPOINTER(buffer)[0];
    _header.raw[1] = GETIBPOINTER(buffer)[2];
    _header.raw[2] = GETIBPOINTER(buffer)[1];
    _header.raw[3] = GETIBPOINTER(buffer)[3];

    return true;
}

Variant TSPacketHeader::GetVariant() {
    Variant result;

    result["header"]["syncByte"] = (uint8_t) _header.data.syncByte;
    result["header"]["transportErrorIndicator"] = _header.data.transportErrorIndicator;
    result["header"]["payloadUnitStartIndicator"] = _header.data.payloadUnitStartIndicator;
    result["header"]["transportPriority"] = _header.data.transportPriority;
    result["header"]["pid"] = _header.data.pid;
    result["header"]["transportScramblingControl"] = _header.data.transportScramblingControl;
    result["header"]["hasAdaptationField"] = _header.data.hasAdaptationField;
    result["header"]["hasPayload"] = _header.data.hasPayload;
    result["header"]["continuityCounter"] = _header.data.continuityCounter;

    return result;
}

TSPacketHeader::operator string() {
    return GetVariant().ToString();
}

bool TSPacketHeader::IsPayloadStart() {
    return _header.data.payloadUnitStartIndicator == 1;
}

bool TSPacketHeader::HasAdaptationField() {
    return _header.data.hasAdaptationField;
}
#endif	/* HAS_PROTOCOL_TS */

