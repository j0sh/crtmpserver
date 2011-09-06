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
#ifndef _CHANNEL_H
#define	_CHANNEL_H

#include "protocols/rtmp/header.h"

#define CS_HEADER 0
#define CS_PAYLOAD 1

typedef struct _Channel {
	uint32_t id;
	uint32_t state;
	IOBuffer inputData;

	Header lastInHeader;
	int8_t lastInHeaderType;
	uint32_t lastInProcBytes;
	double lastInAbsTs;
	uint32_t lastInStreamId;

	Header lastOutHeader;
	int8_t lastOutHeaderType;
	uint32_t lastOutProcBytes;
	double lastOutAbsTs;
	uint32_t lastOutStreamId;

	void Reset() {
		state = CS_HEADER;
		inputData.IgnoreAll();

		memset(&lastInHeader, 0, sizeof (lastInHeader));
		lastInHeaderType = 0;
		lastInProcBytes = 0;
		lastInAbsTs = 0;
		lastInStreamId = 0xffffffff;

		memset(&lastOutHeader, 0, sizeof (lastOutHeader));
		lastOutHeaderType = 0;
		lastOutProcBytes = 0;
		lastOutAbsTs = 0;
		lastOutStreamId = 0xffffffff;
	}
} Channel;


#endif	/* _CHANNEL_H */

#endif /* HAS_PROTOCOL_RTMP */

