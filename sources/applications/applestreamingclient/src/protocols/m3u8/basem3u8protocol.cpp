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

#include "protocols/m3u8/basem3u8protocol.h"
#include "protocols/http/outboundhttpprotocol.h"
#include "clientcontext.h"
using namespace app_applestreamingclient;

BaseM3U8Protocol::BaseM3U8Protocol(uint64_t type)
: GenericProtocol(type) {
}

BaseM3U8Protocol::~BaseM3U8Protocol() {
}

bool BaseM3U8Protocol::AllowFarProtocol(uint64_t type) {
	return type == PT_OUTBOUND_HTTP;
}

bool BaseM3U8Protocol::AllowNearProtocol(uint64_t type) {
	return false;
}

bool BaseM3U8Protocol::SignalInputData(int32_t recvAmount) {
	NYIR;
}

bool BaseM3U8Protocol::SignalInputData(IOBuffer &buffer) {
	//1. Get the context
	ClientContext *pContext = GetContext();
	if (pContext == NULL) {
		FATAL("Unable to get the context");
		return false;
	}

	//1. Get the transport
	OutboundHTTPProtocol *pHttpProtocol = (OutboundHTTPProtocol *) GetFarProtocol();
	assert(pHttpProtocol != NULL);

	//2. See if we have the 200 OK
	if (!pHttpProtocol->Is200OK()) {
		FATAL("The HTTP request failed. Response headers\n%s",
				STR(pHttpProtocol->GetHeaders().ToString()));
		FATAL("Additional data:\n%s", STR(GetCustomParameters().ToString()));
		if (!SignalPlaylistFailed()) {
			FATAL("Unable to signal playlist available");
			return false;
		}
		buffer.IgnoreAll();
		EnqueueForDelete();
		return true;
	}

	//3. See if we have a complete transfer
	if (!pHttpProtocol->TransferCompleted()) {
		return true;
	}

	//4. Parse the playlist
	if (!ParsePlaylist(GetCustomParameters()["fullUri"], GETIBPOINTER(buffer),
			GETAVAILABLEBYTESCOUNT(buffer))) {
		FATAL("Unable to parse playlist");
		return false;
	}
	buffer.IgnoreAll();

	//5. Signal that a new playlist is available
	if (!SignalPlaylistAvailable()) {
		FATAL("Unable to signal playlist available");
		return false;
	}

	//6. Job done. Disconnect
	EnqueueForDelete();

	return true;
}

bool BaseM3U8Protocol::ParsePlaylist(string fullUri, const uint8_t *pBuffer, uint32_t length) {
	Playlist *pPlaylist = GetPlaylist();
	pPlaylist->SetPlaylistUri(fullUri);
	if (pPlaylist == NULL) {
		FATAL("Unable to get the playlist");
		return false;
	}
	pPlaylist->Clear();
	pPlaylist->GetBuffer()->ReadFromBuffer(pBuffer, length);
	pPlaylist->GetBuffer()->ReadFromRepeat(0x0a, 1);
	pPlaylist->Parse();

	return true;
}
