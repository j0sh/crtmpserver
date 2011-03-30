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
	_initialBuffer = 64 * 1024;
	_hasAudio = false;
	_startAudio = -1;
	_hasVideo = false;
	_startVideo = 0;
}

PacketQueue::~PacketQueue() {
	for (uint32_t i = 0; i < _allPackets.size(); i++) {
		delete _allPackets[i];
	}
	_allPackets.clear();
	_queue.clear();
	_free.clear();
}

void PacketQueue::HasAudioVideo(bool hasAudio, bool hasVideo) {
	_hasAudio = hasAudio;
	_hasVideo = hasVideo;
}

vector<Packet *> PacketQueue::PushPacket(uint8_t *pData, uint32_t dataLength,
		double absoluteTimestamp, bool isAudio) {
	//1. Do we have any data?
	if (dataLength == 0)
		return vector<Packet *>();

	//2. Check if this is a on-track stream and return
	//immediatly if so
	if (!(_hasAudio && _hasVideo)) {
		Packet *pPacket = GetPacket(pData, dataLength, absoluteTimestamp, isAudio);
		vector<Packet *> result;
		ADD_VECTOR_END(_free, pPacket);
		ADD_VECTOR_END(result, pPacket);
		return result;
	}

	//3. Create the packet and add it to the queue
	Packet *pPacket = GetPacket(pData, dataLength, absoluteTimestamp, isAudio);
	ADD_VECTOR_END(_queue[absoluteTimestamp], pPacket);

	//4. Wait untill the queue has some meaningfull number
	//of packets in it
	//TODO: This must be re-implemented using a more
	//deterministic way instead of blindly waiting
	//for the queue to accumulate a number of packets
	if (_queue.size() < 100) {
		return vector<Packet *>();
	}

	//5. Pop the head of the queue. This is our result
	vector<Packet *> result = MAP_VAL(_queue.begin());
	_queue.erase(_queue.begin());

	//6. Add the ready-to-return packets to the
	//packets cache for re-use
	for (uint32_t i = 0; i < result.size(); i++) {
		ADD_VECTOR_END(_free, result[i]);
	}

	//7. Done!
	return result;
}

Packet *PacketQueue::GetPacket(uint8_t *pData, uint32_t dataLength,
		double absoluteTimestamp, bool isAudio) {
	if (dataLength == 0)
		return NULL;
	Packet *pPacket = NULL;
	if (_free.size() != 0) {
		pPacket = _free[0];
		_free.erase(_free.begin());
	} else {
		pPacket = new Packet;
		pPacket->buffer.ReadFromRepeat(0, _initialBuffer);
		pPacket->buffer.IgnoreAll();
		ADD_VECTOR_END(_allPackets, pPacket);
	}
	pPacket->buffer.IgnoreAll();
	pPacket->buffer.ReadFromBuffer(pData, dataLength);
	pPacket->ts = absoluteTimestamp;
	pPacket->isAudio = isAudio;
	return pPacket;
}
