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
#include "protocols/ts/streamdescriptors.h"
#include "protocols/ts/tsboundscheck.h"

bool ReadStreamDescriptor(StreamDescriptor &descriptor,
		uint8_t *pBuffer, uint32_t &cursor, uint32_t maxCursor) {
	CHECK_BOUNDS(2);
	descriptor.type = pBuffer[cursor++];
	descriptor.length = pBuffer[cursor++];
	CHECK_BOUNDS(descriptor.length);

	//iso13818-1.pdf Table 2-39, page 81/174
	switch (descriptor.type) {
		case 14://Maximum_bitrate_descriptor
		{
			CHECK_BOUNDS(3);
			descriptor.payload.maximum_bitrate_descriptor.maximum_bitrate =
					(((pBuffer[cursor] << 16) | (pBuffer[cursor + 1] << 8) | (pBuffer[cursor + 2]))&0x3fffff) * 50 * 8 / 1024;
			break;
		}
		default:
			break;
	}

	cursor += descriptor.length;
	return true;
}
#endif	/* HAS_PROTOCOL_TS */

