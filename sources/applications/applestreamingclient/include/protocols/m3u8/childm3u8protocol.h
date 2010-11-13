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


#ifndef _CHILDM3U8PROTOCOL_H
#define	_CHILDM3U8PROTOCOL_H

#include "protocols/m3u8/basem3u8protocol.h"

namespace app_applestreamingclient {

	class ChildM3U8Protocol
	: public BaseM3U8Protocol {
	private:
		uint32_t _bw;
	public:
		ChildM3U8Protocol();
		virtual ~ChildM3U8Protocol();

		virtual bool Initialize(Variant &parameters);
	protected:
		virtual Playlist *GetPlaylist();
		virtual bool SignalPlaylistAvailable();
		virtual bool SignalPlaylistFailed();
	};
}

#endif	/* _CHILDM3U8PROTOCOL_H */

