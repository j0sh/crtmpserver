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


#ifdef NET_SELECT

#include "netio/select/iohandlermanager.h"
#include "netio/select/inboundnamedpipecarrier.h"
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
	int32_t fd = open(STR(path), O_RDONLY/* | O_NONBLOCK*/);
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

bool InboundNamedPipeCarrier::OnEvent(select_event &event) {
	if (_pProtocol == NULL) {
		ASSERT("This pipe has no upper protocols");
		return false;
	}

	int32_t recvAmount = 0;

	switch (event.type) {
		case SET_READ:
		{
			IOBuffer *pInputBuffer = _pProtocol->GetInputBuffer();
			assert(pInputBuffer != NULL);
			if (!pInputBuffer->ReadFromPipe(_inboundFd,
					FD_READ_CHUNK, recvAmount)) {
				FATAL("Unable to read data");
				return false;
			}

			return _pProtocol->SignalInputData(recvAmount);
		}
		default:
		{
			ASSERT("Invalid state: %hhu", event.type);
			return false;
		}
	}
}

InboundNamedPipeCarrier::operator string() {
	if (_pProtocol != NULL)
		return STR(*_pProtocol);
	return format("INP(%d)", _inboundFd);
}

void InboundNamedPipeCarrier::GetStats(Variant &info, uint32_t namespaceId) {

}

#endif /* NET_SELECT */

