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


#ifdef NET_EPOLL
#include "netio/epoll/iohandlermanager.h"
#include "netio/epoll/inboundnamedpipecarrier.h"
#include "protocols/baseprotocol.h"

InboundNamedPipeCarrier::InboundNamedPipeCarrier(int32_t fd, string path)
: IOHandler(fd, fd, IOHT_INBOUNDNAMEDPIPE_CARRIER) {
	_path = path;
}

InboundNamedPipeCarrier::~InboundNamedPipeCarrier() {
	deleteFile(_path);
}

InboundNamedPipeCarrier *InboundNamedPipeCarrier::Create(string path,
		uint16_t mode) {
	if (mkfifo(STR(path), mode) != 0) {
		int err = errno;
		FATAL("Unable to create named pipe %s with mode %u: %s (%d)",
				STR(path), (uint32_t) mode, strerror(err), err);
		return NULL;
	}

	int32_t fd = open(STR(path), O_RDONLY | O_NONBLOCK);
	if (fd < 0) {
		int err = errno;
		FATAL("Unable to open named pipe %s:%s (%d)",
				STR(path), strerror(err), err);
		deleteFile(path);
		return NULL;
	}

	InboundNamedPipeCarrier *pResult = new InboundNamedPipeCarrier(fd, path);

	if (!IOHandlerManager::EnableReadData(pResult)) {
		FATAL("Unable to enable read event on the named pipe");
		delete pResult;
		return NULL;
	}

	return pResult;
}

bool InboundNamedPipeCarrier::SignalOutputData() {
	NYIR;
}

bool InboundNamedPipeCarrier::OnEvent(struct epoll_event &event) {
	if (_pProtocol == NULL) {
		ASSERT("This pipe has no upper protocols");
		return false;
	}

	int32_t recvBytes = 0;

	if ((event.events & EPOLLIN) != 0) {
		IOBuffer *pInputBuffer = _pProtocol->GetInputBuffer();
		assert(pInputBuffer != NULL);
		if (!pInputBuffer->ReadFromPipe(_inboundFd, FD_READ_CHUNK,
				recvBytes)) {
			FATAL("Unable to read data");
			return false;
		}

		return _pProtocol->SignalInputData(recvBytes);
	} else if (event.events & EPOLLHUP) {
		WARN("This is a HUP");
		if (_pProtocol != NULL)
			_pProtocol->EnqueueForDelete();
		return false;
	} else {
		ASSERT("Invalid state: %x", event.events);

		return false;
	}
}

InboundNamedPipeCarrier::operator string() {
	if (_pProtocol != NULL)
		return STR(*_pProtocol);
	return format("INP(%d)", _inboundFd);
}

void InboundNamedPipeCarrier::GetStats(Variant &info, uint32_t namespaceId) {

}

#endif /* NET_EPOLL */

