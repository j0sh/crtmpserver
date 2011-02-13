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
	cursor += descriptor.length;
	return true;
	//	switch (descriptor.type) {
	//		case DESCRIPTOR_TYPE_VIDEO:
	//		{
	//			CHECK_BOUNDS(1);
	//			uint8_t flags = pBuffer[cursor];
	//			descriptor.info.video.multipleFrameFate = (flags & 0x80) != 0;
	//			descriptor.info.video.frameRateCode = (flags & 0x78) >> 3;
	//			descriptor.info.video.MPEG1Only = (flags & 0x04) != 0;
	//			descriptor.info.video.constrainedParameter = (flags & 0x02) != 0;
	//			descriptor.info.video.stillPicture = flags & 0x01;
	//			if (descriptor.info.video.MPEG1Only) {
	//				CHECK_BOUNDS(1);
	//				descriptor.info.video.profileAndLevelIndication = pBuffer[cursor + 1];
	//				CHECK_BOUNDS(1);
	//				flags = pBuffer[cursor + 2];
	//				descriptor.info.video.chromaFormat = flags >> 6;
	//				descriptor.info.video.frameRateExtension = (flags & 0x20) != 0;
	//			}
	//			CHECK_BOUNDS(descriptor.length);
	//			cursor += descriptor.length;
	//			return true;
	//		}
	//		case DESCRIPTOR_TYPE_REGISTRATION:
	//		{
	//			CHECK_BOUNDS(4);
	//			descriptor.info.registration.formatIdentifier = ENTOHLP((pBuffer + cursor)); //----MARKED-LONG---
	//			descriptor.info.registration.additionalInfoLength = descriptor.length - 4;
	//			memcpy(descriptor.info.registration.additionalInfo,
	//					pBuffer + cursor,
	//					descriptor.info.registration.additionalInfoLength);
	//			cursor += descriptor.length;
	//			return true;
	//		}
	//		case DESCRIPTOR_TYPE_DATA_STREAM_ALIGNMENT:
	//		{
	//			CHECK_BOUNDS(1);
	//			descriptor.info.dataStreamAlignment.alignmentType = pBuffer[cursor++];
	//			return true;
	//		}
	//		case DESCRIPTOR_TYPE_ISO_639_LANGUAGE:
	//		{
	//			for (uint8_t i = 0; i < descriptor.length / 4; i++) {
	//				CHECK_BOUNDS(3);
	//				memcpy(descriptor.info.iso639Language.definition[i].languageCode,
	//						pBuffer + cursor, 3);
	//				cursor += 3;
	//				CHECK_BOUNDS(1);
	//				descriptor.info.iso639Language.definition[i].audioType = pBuffer[cursor++];
	//			}
	//			return true;
	//		}
	//		case DESCRIPTOR_TYPE_IOD_DESCRIPTOR:
	//		{
	//			WARN("DESCRIPTOR_TYPE_IOD_DESCRIPTOR not yet implemented");
	//			CHECK_BOUNDS(descriptor.length);
	//			cursor += descriptor.length;
	//			return true;
	//		}
	//		case DESCRIPTOR_TYPE_SMOOTHING_BUFFER:
	//		{
	//			WARN("DESCRIPTOR_TYPE_SMOOTHING_BUFFER not yet implemented");
	//			CHECK_BOUNDS(descriptor.length);
	//			cursor += descriptor.length;
	//			return true;
	//		}
	//		case DESCRIPTOR_TYPE_FMC_DESCRIPTOR:
	//		{
	//			WARN("DESCRIPTOR_TYPE_SMOOTHING_BUFFER not yet implemented");
	//			CHECK_BOUNDS(descriptor.length);
	//			cursor += descriptor.length;
	//			return true;
	//		}
	//		default:
	//		{
	//			if (descriptor.type < 36) {
	//				FATAL("Unknown descriptor type %d ", descriptor.type);
	//				return false;
	//			} else {
	//				WARN("Descriptor type %d defaulted to USER PRIVATE", descriptor.type);
	//				descriptor.type = DESCRIPTOR_TYPE_USER_PRIVATE;
	//				CHECK_BOUNDS(descriptor.length);
	//				cursor += descriptor.length;
	//				return true;
	//			}
	//		}
	//	}
}
#endif	/* HAS_PROTOCOL_TS */

