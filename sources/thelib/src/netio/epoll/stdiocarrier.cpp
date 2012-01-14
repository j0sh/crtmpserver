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
#include "netio/epoll/stdiocarrier.h"
#include "netio/epoll/iohandlermanager.h"
#include "protocols/baseprotocol.h"

/*#define ENABLE_WRITE_DATA \
if (!_writeDataEnabled) { \
	_writeDataEnabled = true; \
	IOHandlerManager::EnableWriteData(this); \
}


#define DISABLE_WRITE_DATA \
if (_writeDataEnabled) { \
	_writeDataEnabled = false; \
	IOHandlerManager::DisableWriteData(this); \
	_pProtocol->ReadyForSend(); \
}*/

StdioCarrier *StdioCarrier::_pInstance = NULL;

StdioCarrier::StdioCarrier()
: IOHandler(fileno(stdin), fileno(stdout), IOHT_TCP_CARRIER) {
	IOHandlerManager::EnableReadData(this);
	_writeDataEnabled = false;
	_ioAmount = 0;
}

StdioCarrier *StdioCarrier::GetInstance(BaseProtocol *pProtocol) {
	if (_pInstance == NULL) {
		_pInstance = new StdioCarrier();
		_pInstance->SetProtocol(pProtocol);
		pProtocol->GetFarEndpoint()->SetIOHandler(_pInstance);
		return _pInstance;
	}
	o_assert(_pInstance->_pProtocol != NULL);
	o_assert(pProtocol != NULL);
	if (_pInstance->_pProtocol->GetId() != pProtocol->GetId()) {
		FATAL("Stdio carrier is already acquired");
		return NULL;
	}
	return _pInstance;
}

StdioCarrier::~StdioCarrier() {
	_pInstance = NULL;
}

bool StdioCarrier::OnEvent(struct epoll_event &event) {
	//1. Read data
	if ((event.events & EPOLLIN) != 0) {
		IOBuffer *pInputBuffer = _pProtocol->GetInputBuffer();
		o_assert(pInputBuffer != NULL);
		if (!pInputBuffer->ReadFromStdio(_inboundFd, FD_READ_CHUNK, _ioAmount)) {
			FATAL("Unable to read data");
			return false;
		}
		ADD_IN_BYTES_MANAGED(_type, _ioAmount);
		if (_ioAmount == 0) {
			FATAL("Connection closed");
			return false;
		}

		if (!_pProtocol->SignalInputData(_ioAmount)) {
			FATAL("Unable to signal data available");
			return false;
		}
	}

	/*//2. Write data
	if ((event.events & EPOLLOUT) != 0) {
		IOBuffer *pOutputBuffer = NULL;
		while ((pOutputBuffer = _pProtocol->GetOutputBuffer()) != NULL) {
			if (!pOutputBuffer->PutToStdio(_outboundFd, FD_WRITE_CHUNK,_ioAmount)) {
				FATAL("Unable to send data");
				IOHandlerManager::EnqueueForDelete(this);
				return false;
			}
			ADD_OUT_BYTES_MANAGED(_type, _ioAmount);
			if (GETAVAILABLEBYTESCOUNT(*pOutputBuffer) > 0) {
				ENABLE_WRITE_DATA;
				break;
			}
		}

		if (pOutputBuffer == NULL) {
			DISABLE_WRITE_DATA;
		}
	}*/

	return true;
}

bool StdioCarrier::SignalOutputData() {
	IOBuffer *pOutputBuffer = NULL;

	while ((pOutputBuffer = _pProtocol->GetOutputBuffer()) != NULL) {
		if (!pOutputBuffer->WriteToStdio(_outboundFd, FD_WRITE_CHUNK, _ioAmount)) {
			FATAL("Unable to send data");
			IOHandlerManager::EnqueueForDelete(this);
			return false;
		}
		ADD_OUT_BYTES_MANAGED(_type, _ioAmount);
	}
	return true;
}

StdioCarrier::operator string() {
	if (_pProtocol != NULL)
		return STR(*_pProtocol);
	return format("IO(%d,%d)", _inboundFd, _outboundFd);
}

void StdioCarrier::GetStats(Variant &info, uint32_t namespaceId) {

}

#endif /* NET_EPOLL */

