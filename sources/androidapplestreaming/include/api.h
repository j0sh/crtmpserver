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

#include "common.h"
#include "jniwrapper.h"

#ifdef ANDROID
void EnvRun(string ip, uint16_t port, CallBackInfo ci, int logLevel);
#else
void EnvRun(string ip, uint16_t port, int logLevel);
#endif /* ANDROID */
void WaitEnvReady();
void EnvStop();

Variant ContextCreate();
Variant ContextList();
Variant ContextClose(uint32_t contextId);
Variant ContextCloseAll();

Variant CommandSetBitrates(uint32_t contextId, string bitRates);
Variant CommandPlay(uint32_t contextId, string connectingString);
Variant CommandPlay(uint32_t contextId, string m3u8Uri, string httpSessionId,
		string keyPassword);
Variant CommandPause(uint32_t contextId);
Variant CommandResume(uint32_t contextId);
Variant CommandSelectBandwidth(uint32_t contextId, uint32_t bandwidth);
Variant CommandSeek(uint32_t contextId, double value);
Variant CommandSelectAVChannels(uint32_t contextId, uint32_t audioChannel,
		uint32_t videoChannel);

Variant InfoListStreams(uint32_t contextId);
Variant InfoListAllStreams();
Variant InfoBandwidth(uint32_t contextId);
Variant InfoPlayback(uint32_t contextId);

#endif	/* _API_H */

