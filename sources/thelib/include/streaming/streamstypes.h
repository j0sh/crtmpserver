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


#ifndef _STREAMSTYPES_H
#define	_STREAMSTYPES_H

#define ST_NEUTRAL_RTMP			MAKE_TAG2('N','R')
#define ST_IN					MAKE_TAG1('I')
#define ST_IN_NET				MAKE_TAG2('I','N')
#define ST_IN_NET_RTMP			MAKE_TAG3('I','N','R')
#define ST_IN_NET_LIVEFLV		MAKE_TAG6('I','N','L','F','L','V')
#define ST_IN_NET_TS			MAKE_TAG4('I','N','T','S')
#define ST_IN_NET_RTP			MAKE_TAG3('I','N','P')
#define ST_IN_NET_RAW			MAKE_TAG3('I','N','W')
#define ST_IN_NET_AAC			MAKE_TAG3('I','N','A')
#define ST_IN_NET_MP3			MAKE_TAG3('I','N','M')
#define ST_IN_FILE				MAKE_TAG2('I','F')
#define ST_IN_FILE_RTMP			MAKE_TAG3('I','F','R')
#define ST_OUT					MAKE_TAG1('O')
#define ST_OUT_NET				MAKE_TAG2('O','N')
#define ST_OUT_NET_RTMP			MAKE_TAG3('O','N','R')
#define ST_OUT_NET_RTMP_4_TS	MAKE_TAG6('O','N','R','4','T','S')
#define ST_OUT_NET_RTMP_4_RTMP	MAKE_TAG5('O','N','R','4','R')
#define ST_OUT_NET_RTP			MAKE_TAG3('O','N','P')
#define ST_OUT_NET_RAW			MAKE_TAG3('O','N','W')
#define ST_OUT_FILE				MAKE_TAG2('O','F')
#define ST_OUT_FILE_RTMP		MAKE_TAG3('O','F','R')
#define ST_OUT_FILE_RTMP_FLV	MAKE_TAG6('O','F','R','F','L','V')
#define ST_OUT_FILE_HLS			MAKE_TAG5('O','F','H','L','S')

#endif	/* _STREAMSTYPES_H */


