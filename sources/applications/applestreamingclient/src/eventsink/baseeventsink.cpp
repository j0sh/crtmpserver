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
#include "eventsink/varianteventsink.h"
#include "clientcontext.h"
using namespace app_applestreamingclient;

BaseEventSink::BaseEventSink(uint64_t type, uint32_t contextId) {
	_type = type;
	_contextId = contextId;
}

BaseEventSink::~BaseEventSink() {
}

uint64_t BaseEventSink::GetType() {
	return _type;
}

BaseEventSink * BaseEventSink::GetInstance(uint64_t type, uint32_t contextId) {
	switch (type) {
#ifdef HAS_PROTOCOL_RTMP
		case PT_INBOUND_RTMP:
		{
			return new RTMPEventSink(contextId);
		}
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_VAR
		case PT_XML_VAR:
		case PT_BIN_VAR:
		{
			return new VariantEventSink(contextId);
		}
#endif /* HAS_PROTOCOL_VAR */
		default:
		{
			ASSERT("Invalid event sync type %s", STR(tagToString(type)));
			return NULL;
		}
	}
}

ClientContext *BaseEventSink::GetContext() {
	return ClientContext::GetContext(_contextId, 0, 0);
}
