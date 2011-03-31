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

#include "protocols/m3u8/childm3u8protocol.h"
#include "protocols/protocolfactory.h"
#include "clientcontext.h"
using namespace app_applestreamingclient;

ChildM3U8Protocol::ChildM3U8Protocol()
: BaseM3U8Protocol(PT_INBOUND_CHILD_M3U8) {
}

ChildM3U8Protocol::~ChildM3U8Protocol() {
}

bool ChildM3U8Protocol::Initialize(Variant &parameters) {
	if (!BaseM3U8Protocol::Initialize(parameters)) {
		FATAL("Unable to initialize child playlist protocol");
		return false;
	}
	_bw = parameters["payload"]["bw"];
	if (_bw == 0) {
		FATAL("Invalid bandwidth: %u", _bw);
		return false;
	}
	return true;
}

Playlist *ChildM3U8Protocol::GetPlaylist() {
	ClientContext *pContext = GetContext();
	if (pContext == NULL) {
		FATAL("Unable to get the context");
		return NULL;
	}
	return pContext->ChildPlaylist(_bw);
}

bool ChildM3U8Protocol::SignalPlaylistAvailable() {
	//1. Get the context
	ClientContext *pContext = GetContext();
	if (pContext == NULL) {
		FATAL("Unable to get the context");
		return false;
	}

	//2. Signal the context about the new playlist
	if (!pContext->SignalChildPlaylistAvailable(_bw)) {
		FATAL("Unable to signal master M3U8 playlist available");
		return false;
	}

	//3. Done
	return true;
}

bool ChildM3U8Protocol::SignalPlaylistFailed() {
	//1. Get the context
	ClientContext *pContext = GetContext();
	if (pContext == NULL) {
		FATAL("Unable to get the context");
		return false;
	}

	//2. Signal the context about the new playlist
	if (!pContext->SignalChildPlaylistNotAvailable(_bw)) {
		FATAL("Unable to signal master M3U8 playlist available");
		return false;
	}

	//3. Done
	return true;
}
