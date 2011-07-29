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
#ifndef _STREAMDESCRIPTORS_H
#define	_STREAMDESCRIPTORS_H

#include "common.h"

//frame rates
//ISO13818-2 table 6-4  (61/255)
#define FRAME_RATE_CODE_FORBIDEN 0
//23, 976
#define FRAME_RATE_CODE_24_PARTIAL 1
#define FRAME_RATE_CODE_24 2
#define FRAME_RATE_CODE_25 3
//29, 97
#define FRAME_RATE_CODE_30_PARTIAL 4
#define FRAME_RATE_CODE_30 5
#define FRAME_RATE_CODE_50 6
//59, 94
#define FRAME_RATE_CODE_60_PARTIAL 7
#define FRAME_RATE_CODE_60 8

//data stream alignaments
//ISO13818-1 Table 2-47 : Video stream alignment values (85/174)
#define DATA_STREAM_ALIGNMENT_SLICE 1
#define DATA_STREAM_ALIGNMENT_VIDEO_ACCESS_UNIT 2
#define DATA_STREAM_ALIGNMENT_GOP_SEQ 3
#define DATA_STREAM_ALIGNMENT_SEQ 4

//udio types values
//ISO13818-1 Table 2-53 : Audio type values (88/174)
#define AUDIO_TYPE_UNDEFINED 0x00
#define AUDIO_TYPE_CLEAN_EFFECTS 0x01
#define AUDIO_TYPE_HEARING_IMPAIRED 0x02
#define AUDIO_TYPE_VISUALY_IMPAIRED 0x03

//descriptors types
//ISO13818-1 Table 2-39 : Program and program element descriptors (81/174)
#define DESCRIPTOR_TYPE_USER_PRIVATE 255
#define DESCRIPTOR_TYPE_VIDEO 2
#define DESCRIPTOR_TYPE_REGISTRATION 5
#define DESCRIPTOR_TYPE_DATA_STREAM_ALIGNMENT 6
#define DESCRIPTOR_TYPE_ISO_639_LANGUAGE 10
#define DESCRIPTOR_TYPE_SMOOTHING_BUFFER 16
#define DESCRIPTOR_TYPE_IOD_DESCRIPTOR 29
#define DESCRIPTOR_TYPE_FMC_DESCRIPTOR 31

typedef struct _StreamDescriptor {
	uint8_t type;
	uint8_t length;

	union {

		struct {
			uint32_t maximum_bitrate;
		} maximum_bitrate_descriptor;
	} payload;

	operator string() {
		return format("type: %hhu; length: %hhu", type, length);
	};
} StreamDescriptor;

bool ReadStreamDescriptor(StreamDescriptor &descriptor,
		uint8_t *pBuffer, uint32_t &cursor, uint32_t maxCursor);


#endif	/* _STREAMDESCRIPTORS_H */
#endif	/* HAS_PROTOCOL_TS */

