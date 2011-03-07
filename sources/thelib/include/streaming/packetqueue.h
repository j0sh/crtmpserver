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

#ifndef _PACKETQUEUE_H
#define	_PACKETQUEUE_H

#include "common.h"

struct Packet {
	IOBuffer buffer;
	double ts;
	bool isAudio;

	Packet() {
		ts = 0;
		isAudio = false;
	}

	virtual ~Packet() {
	}
};

class PacketQueue {
private:
	vector<Packet *> _allPackets;
	vector<Packet *> _free;
	map<double, vector<Packet *> > _queue;
	uint32_t _initialBuffer;

	bool _hasAudio;
	double _startAudio;

	bool _hasVideo;
	double _startVideo;
public:
	PacketQueue();
	virtual ~PacketQueue();
	void HasAudioVideo(bool hasAudio, bool hasVideo);
	vector<Packet *> PushPacket(uint8_t *pData, uint32_t dataLength,
			double absoluteTimestamp, bool isAudio);
private:
	Packet *GetPacket(uint8_t *pData, uint32_t dataLength,
			double absoluteTimestamp, bool isAudio);
};


#endif	/* _PACKETQUEUE_H */

