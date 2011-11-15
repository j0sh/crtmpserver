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


#ifndef _PROTOCOLTYPES_H
#define	_PROTOCOLTYPES_H

#include "common.h"

//carrier protocols
#define PT_TCP					MAKE_TAG3('T','C','P')
#define PT_UDP					MAKE_TAG3('U','D','P')

//variant protocols
#define PT_BIN_VAR				MAKE_TAG4('B','V','A','R')
#define PT_XML_VAR				MAKE_TAG4('X','V','A','R')

//RTMP protocols
#define PT_INBOUND_RTMP			MAKE_TAG2('I','R')
#define PT_INBOUND_RTMPS_DISC	MAKE_TAG3('I','R','S')
#define PT_OUTBOUND_RTMP		MAKE_TAG2('O','R')
#define PT_MONITOR_RTMP			MAKE_TAG2('M','R')

//encryption protocols
#define PT_RTMPE				MAKE_TAG2('R','E')
#define PT_INBOUND_SSL			MAKE_TAG4('I','S','S','L')
#define PT_OUTBOUND_SSL			MAKE_TAG4('O','S','S','L')

//Async DNS protocols
#define PT_INBOUND_DNS			MAKE_TAG4('I','D','N','S')
#define PT_OUTBOUND_DNS			MAKE_TAG4('O','D','N','S')

//MPEG-TS protocol
#define PT_INBOUND_TS			MAKE_TAG3('I','T','S')

//HTTP protocols
#define PT_INBOUND_HTTP				MAKE_TAG4('I','H','T','T')
#define PT_INBOUND_HTTP_FOR_RTMP	MAKE_TAG4('I','H','4','R')
#define PT_OUTBOUND_HTTP			MAKE_TAG4('O','H','T','T')
#define PT_OUTBOUND_HTTP_FOR_RTMP	MAKE_TAG4('O','H','4','R')

//Timer protocol
#define PT_TIMER				MAKE_TAG3('T','M','R')

//Live FLV protocols
#define PT_INBOUND_LIVE_FLV		MAKE_TAG4('I','L','F','L')
#define PT_OUTBOUND_LIVE_FLV	MAKE_TAG4('O','L','F','L')

//RTP/RTPS protocols
#define PT_RTSP					MAKE_TAG4('R','T','S','P')
#define PT_RTCP					MAKE_TAG4('R','T','C','P')
#define PT_INBOUND_RTP			MAKE_TAG4('I','R','T','P')
#define PT_OUTBOUND_RTP			MAKE_TAG4('O','R','T','P')
#define PT_RTP_NAT_TRAVERSAL	MAKE_TAG5('R','N','A','T','T')

//MMS
#define PT_OUTBOUND_MMS			MAKE_TAG3('M','M','S')

//CLI protocols
#define PT_INBOUND_JSONCLI		MAKE_TAG8('I','J','S','O','N','C','L','I')
#define PT_HTTP_4_CLI			MAKE_TAG3('H','4','C')

//Raw HTTP stream
#define PT_INBOUND_RAW_HTTP_STREAM		MAKE_TAG4('I','R','H','S')

#endif	/* _PROTOCOLTYPES_H */


