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

#include "playlist.h"
using namespace app_applestreamingclient;

Playlist::Playlist() {
	Clear();
}

Playlist::~Playlist() {
	Clear();
}

IOBuffer *Playlist::GetBuffer() {
	return &_buffer;
}

void Playlist::Clear() {
	_buffer.IgnoreAll();
	_lastMediaSequence = 0;
	_pLastKeyUri = NULL;
	_items.clear();
	_itemMediaSequences.clear();
	_itemKeyUris.clear();
	_itemUris.clear();
	_itemBandwidths.clear();
	_meanDuration = 0;
	_meanDurationCount = 0;
	_startTime = -1;
	_endTime = -1;
}

void Playlist::SetPlaylistUri(string playlistUri) {
	_playlistUri = playlistUri;
	string dummy;
	splitFileName(_playlistUri, _partialUri, dummy, '/');
	_partialUri += "/";
}

string Playlist::GetPlaylistUri() {
	return _playlistUri;
}

void Playlist::Parse(uint32_t skipCount) {
	_lastMediaSequence = 0;
	_pLastKeyUri = NULL;

	uint8_t *pBuffer = GETIBPOINTER(_buffer);
	uint32_t bufferLength = GETAVAILABLEBYTESCOUNT(_buffer);
	uint32_t lineStart = 0;
	uint32_t lineLength = 0;
	char *pLine = NULL;
	vector<char *> item;
	uint32_t itemIndex = 0;

	for (uint32_t i = 0; i < bufferLength; i++) {
		//1. Detect the new-line
		if (pBuffer[i] != 0x0d && pBuffer[i] != 0x0a)
			continue;

		//2. Over-write it with '\0'
		pBuffer[i] = 0;

		//3. Compute the line length and save the pointer
		lineLength = i - lineStart;
		if (lineLength != 0) {
			pLine = (char *) (pBuffer + lineStart);
		}
		lineStart = i + 1;

		//4. Skip this line if length is 0
		if (lineLength == 0)
			continue;

		//5. This line is part of an item.
		item.push_back(pLine);
		if (pLine[0] == '#') {
			//6. Detect EXT-X-MEDIA-SEQUENCE or EXT-X-KEY
			//The string must be at least 22 bytes length
			if (lineLength >= 22) {
				if (pLine[21] == ':') {
					//7. We have a possible winner for EXT-X-MEDIA-SEQUENCE
					//now the offort of strstr is motivated
					if (strstr(pLine, "#EXT-X-MEDIA-SEQUENCE:") == pLine) {
						//8. We have it
						_lastMediaSequence = atoi(pLine + 22);
					}
				} else if (pLine[10] == ':') {
					//9. We have a possible winner for EXT-X-KEY
					//now the offort of strstr is motivated
					if (strstr(pLine, "#EXT-X-KEY:") == pLine) {
						//10. We have it
						_pLastKeyUri = strstr(pLine, "URI=\"");
						if (_pLastKeyUri != NULL) {
							_pLastKeyUri += 5;
							strstr(_pLastKeyUri, "\"")[0] = 0;
						}
					}
				}
			}
		} else {
			if (skipCount == 0) {
				//11. If we get an non-comment
				//that means the item is finished. Save it
				_items[itemIndex] = item;
				_itemMediaSequences[_lastMediaSequence] = itemIndex;
				_itemKeyUris[itemIndex] = _pLastKeyUri;
				_itemUris[itemIndex] = pLine;

				//12. Increment indexes
				itemIndex++;
			} else {
				skipCount--;
			}
			_lastMediaSequence++;

			//13. Clear the current item
			item.clear();
		}
	}
}

bool Playlist::ParseBandwidthInfo() {
	for (map<uint32_t, vector<char *> >::iterator i = _items.begin(); i != _items.end(); i++) {
		vector<char *> &comments = MAP_VAL(i);
		bool bandwidthFound = false;
		for (uint32_t j = 0; j < comments.size() - 1; j++) {
			char *pComment = comments[j];
			if (pComment[0] != '#')
				continue;
			char *pBandwidth = strstr(pComment, "BANDWIDTH=");
			if (pBandwidth == NULL)
				continue;
			pBandwidth += 10;
			_itemBandwidths[MAP_KEY(i)] = atoi(pBandwidth);
			bandwidthFound = true;
			break;
		}
		if (!bandwidthFound) {
			FATAL("Item number %u doesn't have bandwidth info", MAP_KEY(i));
			return false;
		}
	}

	return true;
}

Variant &Playlist::GetCustomData() {
	return _customData;
}

uint32_t Playlist::GetItemsCount() {
	return _items.size();
}

string Playlist::GetItemUri(uint32_t &sequence) {
	uint32_t index = GetIndex(sequence);
	if (index >= _itemUris.size())
		return "";
	string result = _itemUris[index];
	if (result == "")
		return result;
	if (result.find("http://") == 0)
		return result;
	if (result[0] != '/')
		return _partialUri + result;
	NYIA;
	return "";
}

string Playlist::GetItemKeyUri(uint32_t &sequence) {
	uint32_t index = GetIndex(sequence);
	if (index >= _itemKeyUris.size())
		return "";
	if (_itemKeyUris[index] == NULL)
		return "";
	return _itemKeyUris[index];
}

uint32_t Playlist::GetItemBandwidth(uint32_t &sequence) {
	uint32_t index = GetIndex(sequence);
	if (index >= _itemBandwidths.size())
		return 0;
	return _itemBandwidths[index];
}

Variant Playlist::GetItemVariant(uint32_t &sequence) {
	uint32_t index = GetIndex(sequence);
	Variant result;
	result["encryptionKeyUri"] = GetItemKeyUri(index);
	result["itemUri"] = GetItemUri(index);

	return result;
}

uint32_t Playlist::GetIndex(uint32_t &sequence) {
	FINEST("---------------------");
	FINEST("sequence requested: %u", sequence);
	//1. We have it, we return it
	if (MAP_HAS1(_itemMediaSequences, sequence)) {
		FINEST("WE have it");
		FINEST("---------------------");
		return _itemMediaSequences[sequence];
	}

	//2. We don't have it, and is a 0 value, that means it was never initialized
	if (sequence == 0) {
		FINEST("sequence is 0");
		FINEST("_itemMediaSequences.size(): %"PRIz"u", _itemMediaSequences.size());
		if (_itemMediaSequences.size() != 0) {
			//3. We have stuff. Init and return
			FINEST("Return the first entry");
			sequence = MAP_KEY(_itemMediaSequences.begin());
			FINEST("---------------------");
			return MAP_VAL(_itemMediaSequences.begin());
		} else {
			//4. We don't have stuff. Don't do nothing and return
			FINEST("empty sequences");
			FINEST("---------------------");
			return 0xffffffff;
		}
	}

	//5. Ok, sequence is initialized but we don't have that value.
	//So, we search the next one, init and return;
	FINEST("DISC????");
	if (_itemMediaSequences.size() > 0) {
		FINEST("So, we have some items");
		if (MAP_KEY(_itemMediaSequences.end()) >= sequence) {
			FINEST("Our sequence is somewhere there...");

			FOR_MAP(_itemMediaSequences, uint32_t, uint32_t, i) {
				FINEST("sequence: %u; MAP_KEY(i): %u", sequence, MAP_KEY(i));
				if (MAP_KEY(i) >= sequence) {
					sequence = MAP_KEY(i);
					FINEST("JACKPOT");
					FINEST("---------------------");
					return MAP_VAL(i);
				}
			}
		} else {
			FINEST("sequence is bigger than the last item");
		}
	}

	//6. Nada. This is the bottom
	FINEST("Nothing left to do....");
	FINEST("---------------------");
	return 0xffffffff;
}
