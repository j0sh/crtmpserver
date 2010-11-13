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


#ifndef _BASEM3U8PROTOCOL_H
#define	_BASEM3U8PROTOCOL_H

#include "protocols/genericprotocol.h"

namespace app_applestreamingclient {
	class Playlist;

	class BaseM3U8Protocol
	: public GenericProtocol {
	public:
		BaseM3U8Protocol(uint64_t type);
		virtual ~BaseM3U8Protocol();

		virtual bool AllowFarProtocol(uint64_t type);
		virtual bool AllowNearProtocol(uint64_t type);
		virtual bool SignalInputData(int32_t recvAmount);
		virtual bool SignalInputData(IOBuffer &buffer);
	protected:
		bool ParsePlaylist(string fullUri, const uint8_t *pBuffer, uint32_t length);
		virtual Playlist *GetPlaylist() = 0;
		virtual bool SignalPlaylistAvailable() = 0;
		virtual bool SignalPlaylistFailed() = 0;
	};
}

#endif	/* _BASEM3U8PROTOCOL_H */

