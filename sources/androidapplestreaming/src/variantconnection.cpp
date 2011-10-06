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


#include "variantconnection.h"

VariantConnection::VariantConnection() {
	_fd = -1;
}

VariantConnection::~VariantConnection() {
	Close();
}

bool VariantConnection::Connect(sockaddr_in &address) {
	Close();
	_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (_fd < 0) {
		FATAL("Unable to create fd: (%d)%s", errno, strerror(errno));
		return false;
	}

	if (connect(_fd, (sockaddr *) & address, sizeof (address)) != 0) {
		FATAL("Unable to connect: (%d)%s", errno, strerror(errno));
		return false;
	}
	return true;
}

bool VariantConnection::SendMessage(Variant &message, Variant &response) {
	string rawContent = "    ";
	if (!message.SerializeToBin(rawContent)) {
		FATAL("Unable to serialize message");
		return false;
	}
	uint8_t *pBuffer = (uint8_t *) rawContent.c_str();
	uint32_t size = rawContent.size();
	EHTONLP(pBuffer, size - 4);

	int32_t totalSentAmount = 0;
	int32_t sentAmount = 0;

	while (totalSentAmount < (int32_t) size) {
		sentAmount = send(_fd, pBuffer + totalSentAmount, size - totalSentAmount, 0);
		if (sentAmount <= 0) {
			return false;
		}
		totalSentAmount += sentAmount;
	}

	_buffer.IgnoreAll();
	int32_t recvAmount = 0;
	for (;;) {
		if (!_buffer.ReadFromTCPFd(_fd, 1024 * 1024, recvAmount)) {
			FATAL("Unable to read from socket");
			return false;
		}
		size = GETAVAILABLEBYTESCOUNT(_buffer);
		pBuffer = GETIBPOINTER(_buffer);
		if (size < 4) {
			WARN("not enough data. Wait for more...");
			continue;
		}
		uint32_t variantSize = ENTOHLP(pBuffer);
		if (variantSize + 4 > size) {
			WARN("not enough data. Wait for more...");
			continue;
		}
		if (!Variant::DeserializeFromBin(pBuffer + 4, variantSize, response)) {
			FATAL("Unable to deserialize buffer");
			return false;
		}
		return true;
	}
}

void VariantConnection::Close() {
	if (_fd > 0) {
		CLOSE_SOCKET(_fd);
		_fd = 0;
	}
}
