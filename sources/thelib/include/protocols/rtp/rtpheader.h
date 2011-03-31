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


#ifdef HAS_PROTOCOL_RTP
#ifndef _RTPHEADER_H
#define _RTPHEADER_H

#include "common.h"

#define GET_RTP_V(x)   ((uint8_t )((((x)._flags)>>30)))
#define GET_RTP_P(x)   ((bool    )((((x)._flags)>>29)&0x01))
#define GET_RTP_X(x)   ((bool    )((((x)._flags)>>28)&0x01))
#define GET_RTP_CC(x)  ((uint8_t )((((x)._flags)>>24)&0x0f))
#define GET_RTP_M(x)   ((bool    )((((x)._flags)>>23)&0x01))
#define GET_RTP_PT(x)  ((uint8_t )((((x)._flags)>>16)&0x7f))
#define GET_RTP_SEQ(x) ((uint16_t)(((x)._flags)&0xffff))

typedef struct DLLEXP _RTPHeader {
	uint32_t _flags;
	uint32_t _timestamp;
	uint32_t _ssrc;

	operator string() {
		return format("f: %hhx; V: %hhu; P: %hhu; X: %hhu; CC: %hhu; M: %hhu; PT: %hhu; SEQ: %hu; TS: %u; SSRC: %x",
				_flags,
				GET_RTP_V(*this),
				GET_RTP_P(*this),
				GET_RTP_X(*this),
				GET_RTP_CC(*this),
				GET_RTP_M(*this),
				GET_RTP_PT(*this),
				GET_RTP_SEQ(*this),
				_timestamp,
				_ssrc);
	};
} RTPHeader;

#endif  /* _RTPHEADER_H */
#endif /* HAS_PROTOCOL_RTP */

