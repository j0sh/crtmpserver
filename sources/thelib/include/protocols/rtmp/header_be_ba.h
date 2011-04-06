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


#ifdef HAS_PROTOCOL_RTMP
#ifdef BIG_ENDIAN_BYTE_ALIGNED
#ifndef _HEADER_BE_BA_H
#define	_HEADER_BE_BA_H

#include "common.h"

class IOBuffer;
struct _Channel;

#define HT_FULL 0
#define HT_SAME_STREAM 1
#define HT_SAME_LENGTH_AND_STREAM 2
#define HT_CONTINUATION 3

#define H_HT(x) ((x).ht)
#define H_CI(x) ((x).ci)
#define H_TS(x) ((x).hf.s.ts)
#define H_ML(x) ((x).hf.s.ml)
#define H_MT(x) ((x).hf.s.mt)
#define H_SI(x) ((x).hf.s.si)
#define H_IA(x) ((x).isAbsolute)

typedef struct DLLEXP _Header {
	uint32_t ci;
	uint8_t ht;

	union _hf {

		struct _s {
			uint32_t ts : 32;
			uint32_t ml : 24;
			uint32_t mt : 8;
			uint32_t si : 32;
		} s;
		uint8_t datac[12];
		uint32_t dataw[3];
	} hf;
	bool readCompleted;
	bool isAbsolute;
	bool skip4bytes;

	bool Read(uint32_t channelId, uint8_t type, IOBuffer &buffer,
			uint32_t availableBytes);

	Variant GetVariant();
	static bool GetFromVariant(struct _Header &header, Variant & variant);

	bool Write(struct _Channel &channel, IOBuffer & buffer);
	bool Write(IOBuffer & buffer);

	operator string();
} Header;

#endif	/* _HEADER_BE_BA_H */
#endif /* BIG_ENDIAN_BYTE_ALIGNED */
#endif /* HAS_PROTOCOL_RTMP */

