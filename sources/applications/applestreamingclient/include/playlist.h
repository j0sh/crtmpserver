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


#ifndef _PLAYLIST_H
#define	_PLAYLIST_H

#include "common.h"

namespace app_applestreamingclient {

	class Playlist {
	private:
		string _playlistUri;
		string _partialUri;
		IOBuffer _buffer;
		uint32_t _lastMediaSequence;
		char *_pLastKeyUri;
		Variant _customData;
		map<uint32_t, vector<char *> > _items;
		map<uint32_t, uint32_t> _itemMediaSequences;
		map<uint32_t, char *> _itemKeyUris;
		map<uint32_t, char *> _itemUris;
		map<uint32_t, uint32_t> _itemBandwidths;
		double _startTime;
		double _endTime;
		double _meanDuration;
		uint32_t _meanDurationCount;
	public:
		Playlist();
		virtual ~Playlist();

		IOBuffer * GetBuffer();
		void Clear();
		void SetPlaylistUri(string playlistUri);
		string GetPlaylistUri();
		void Parse(uint32_t skipCount = 0);
		bool ParseBandwidthInfo();

		Variant & GetCustomData();
		uint32_t GetItemsCount();
		string GetItemUri(uint32_t &sequence);
		string GetItemKeyUri(uint32_t &sequence);
		uint32_t GetItemBandwidth(uint32_t &sequence);
		Variant GetItemVariant(uint32_t &sequence);
	private:
		uint32_t GetIndex(uint32_t &sequence);
	};
};

#endif	/* _PLAYLIST_H */

