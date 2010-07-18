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

#ifndef _API_H
#define	_API_H

#include "utils/core.h"

#define AS_REQUEST_PARAMS(v)				(v)["request"]
#define AS_REQUEST_PARAM(v,key)				AS_REQUEST_PARAMS(v)[(key)]
#define AS_RESPONSE(v)						(v)["response"]
#define AS_RESPONSE_ERROR_CODE(v)			AS_RESPONSE(v)["errorCode"]
#define AS_RESPONSE_ERROR_DESC(v)			AS_RESPONSE(v)["errorCodeDesc"]
#define AS_RESPONSE_PAYLOAD(v)				AS_RESPONSE(v)["payload"]
#define AS_RESPONSE_PAYLOAD_VALUE(v,key)	AS_RESPONSE_PAYLOAD(v)[(key)]
#define AS_RESPONSE_IS_OK(v)				((bool)(AS_RESPONSE_ERROR_CODE(v)==((uint32_t)0)))

void EnvRun(string ip, uint16_t port);
void EnvStop();

Variant ContextCreate();
Variant ContextList();
Variant ContextClose(uint32_t contextId);
Variant ContextCloseAll();

Variant CommandPlay(uint32_t contextId, string m3u8Uri, string httpSessionId,
		string keyPassword);
Variant CommandPause(uint32_t contextId);
Variant CommandResume(uint32_t contextId);

#endif	/* _API_H */

