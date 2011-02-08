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

#include "streaming/packetqueue.h"

PacketQueue::PacketQueue() {
}

PacketQueue::~PacketQueue() {
	for (uint32_t i = 0; i < _allPackets.size(); i++) {
		delete _allPackets[i];
	}
	_allPackets.clear();
	_queue.clear();
	_free.clear();
}

vector<Packet *> PacketQueue::PushPacket(uint8_t *pData, uint32_t dataLength,
		double absoluteTimestamp, bool isAudio) {
	//FINEST("isAudio: %d; _lastIsAudio: %d", isAudio, _lastIsAudio);
	if (dataLength == 0)
		return vector<Packet *>();

	Packet *pPacket = GetPacket(pData, dataLength, absoluteTimestamp, isAudio);
	ADD_VECTOR_END(_queue[absoluteTimestamp], pPacket);

	if (_queue.size() < 10) {
		return vector<Packet *>();
	}
	vector<Packet *> result = MAP_VAL(_queue.begin());
	_queue.erase(_queue.begin());
	for (uint32_t i = 0; i < result.size(); i++) {
		ADD_VECTOR_END(_free, result[i]);
	}

	return result;
}

Packet *PacketQueue::GetPacket(uint8_t *pData, uint32_t dataLength,
		double absoluteTimestamp, bool isAudio) {
	if (dataLength == 0)
		return NULL;
	Packet *pPacket = NULL;
	if (_free.size() != 0) {
		//FINEST("cache");
		pPacket = _free[0];
		_free.erase(_free.begin());
	} else {
		//FINEST("new");
		pPacket = new Packet;
		ADD_VECTOR_END(_allPackets, pPacket);
	}
	pPacket->buffer.IgnoreAll();
	pPacket->buffer.ReadFromBuffer(pData, dataLength);
	pPacket->ts = absoluteTimestamp;
	pPacket->isAudio = isAudio;
	return pPacket;
}
