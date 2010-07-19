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

#include "eventsink/baseeventsink.h"
#include "eventsink/rtmpeventsink.h"
#include "protocols/protocoltypes.h"

BaseEventSink::BaseEventSink() {

}

BaseEventSink::~BaseEventSink() {
}

BaseEventSink * BaseEventSink::GetInstance(uint64_t type) {
	switch (type) {
#ifdef HAS_PROTOCOL_RTMP
		case PT_INBOUND_RTMP:
		{
			return new RTMPEventSink();
		}
#endif /* HAS_PROTOCOL_RTMP */
		default:
		{
			ASSERT("Invalid event sync type %s", STR(tagToString(type)));
			return NULL;
		}
	}
}
