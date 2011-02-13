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
#ifndef _TSPACKETHEADER_H
#define	_TSPACKETHEADER_H

#include "common.h"

//iso13818-1.pdf page 36/174
//Table 2-2 – Transport packet of this Recommendation | International Standard
#define TS_TRANSPORT_PACKET_PID(x)					((uint16_t)(((x)>>8)&0x00001fff))
#define TS_TRANSPORT_PACKET_HAS_ADAPTATION_FIELD(x)	((bool)(((x)&(0x00000020))!=0))
#define TS_TRANSPORT_PACKET_HAS_PAYLOAD(x)			((bool)(((x)&(0x00000010))!=0))
#define TS_TRANSPORT_PACKET_IS_PAYLOAD_START(x)		((bool)(((x)&(0x00400000))!=0))

#endif	/* _TSPACKETHEADER_H */
#endif	/* HAS_PROTOCOL_TS */
