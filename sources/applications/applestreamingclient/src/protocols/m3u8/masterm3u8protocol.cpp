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

#include "protocols/m3u8/masterm3u8protocol.h"
#include "protocols/protocolfactory.h"
#include "clientcontext.h"
using namespace app_applestreamingclient;

MasterM3U8Protocol::MasterM3U8Protocol()
: BaseM3U8Protocol(PT_INBOUND_MASTER_M3U8) {
}

MasterM3U8Protocol::~MasterM3U8Protocol() {
}

Playlist *MasterM3U8Protocol::GetPlaylist() {
	ClientContext *pContext = GetContext();
	if (pContext == NULL) {
		FATAL("Unable to get the context");
		return NULL;
	}
	return pContext->MasterPlaylist();
}

bool MasterM3U8Protocol::SignalPlaylistAvailable() {
	//1. Get the context
	ClientContext *pContext = GetContext();
	if (pContext == NULL) {
		FATAL("Unable to get the context");
		return false;
	}

	//2. Validate the playlist
	if (!GetPlaylist()->ParseBandwidthInfo()) {
		WARN("Unable to parse bandwidth info inside master playlist");
		string content = "#EXT-X-STREAM-INF:PROGRAM-ID=1, BANDWIDTH=500000\r\n";
		content += (string) GetCustomParameters()["fullUri"];
		if (!ParsePlaylist(GetCustomParameters()["fullUri"], (const uint8_t *) STR(content), content.size())) {
			ASSERT("Unable to parse master playlist");
			return false;
		}
		if (!GetPlaylist()->ParseBandwidthInfo()) {
			FATAL("Unable to parse bandwidth info inside master playlist");
			return false;
		}
	}

	//3. Signal the context about the new playlist
	if (!pContext->SignalMasterPlaylistAvailable()) {
		FATAL("Unable to signal master M3U8 playlist available");
		return false;
	}

	//4. Done
	return true;
}

bool MasterM3U8Protocol::SignalPlaylistFailed() {
	NYIR;
}
