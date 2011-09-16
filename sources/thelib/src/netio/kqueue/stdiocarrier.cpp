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


#ifdef NET_KQUEUE
#include "common.h"
#include "netio/kqueue/stdiocarrier.h"
#include "netio/kqueue/iohandlermanager.h"
#include "protocols/baseprotocol.h"

#define ENABLE_WRITE_DATA \
if (!_writeDataEnabled) { \
    _writeDataEnabled = true; \
    IOHandlerManager::EnableWriteData(this); \
} \
/*else{ \
    FINEST("Write data already enabled"); \
}*/

#define DISABLE_WRITE_DATA \
if (_writeDataEnabled) { \
    _writeDataEnabled = false; \
    IOHandlerManager::DisableWriteData(this); \
    _pProtocol->ReadyForSend(); \
} \
/*else { \
    FINEST("Write data already disabled"); \
}*/

StdioCarrier *StdioCarrier::_pInstance = NULL;

StdioCarrier::StdioCarrier()
: IOHandler(fileno(stdin), fileno(stdout), IOHT_STDIO) {
	IOHandlerManager::EnableReadData(this);
	_writeDataEnabled = false;
}

StdioCarrier *StdioCarrier::GetInstance(BaseProtocol *pProtocol) {
	if (_pInstance == NULL) {
		_pInstance = new StdioCarrier();
		_pInstance->SetProtocol(pProtocol);
		pProtocol->GetFarEndpoint()->SetIOHandler(_pInstance);
		return _pInstance;
	}
	assert(_pInstance->_pProtocol != NULL);
	assert(pProtocol != NULL);
	if (_pInstance->_pProtocol->GetId() != pProtocol->GetId()) {
		FATAL("Stdio carrier is already acquired");
		return NULL;
	}
	return _pInstance;
}

StdioCarrier::~StdioCarrier() {
	_pInstance = NULL;
}

bool StdioCarrier::OnEvent(struct kevent &event) {
	int32_t recvAmount = 0;

	//3. Do the I/O
	switch (event.filter) {
		case EVFILT_READ:
		{
			IOBuffer *pInputBuffer = _pProtocol->GetInputBuffer();
			assert(pInputBuffer != NULL);
			if (!pInputBuffer->ReadFromStdio(event.ident, event.data, recvAmount)) {
				FATAL("Unable to read data");
				return false;
			}

			return _pProtocol->SignalInputData(recvAmount);
		}
		case EVFILT_WRITE:
		{
			IOBuffer *pOutputBuffer = NULL;

			while ((pOutputBuffer = _pProtocol->GetOutputBuffer()) != NULL) {
				if (!pOutputBuffer->WriteToStdio(event.ident, event.data)) {
					FATAL("Unable to send data");
					IOHandlerManager::EnqueueForDelete(this);
					return false;
				}
				if (GETAVAILABLEBYTESCOUNT(*pOutputBuffer) > 0) {
					ENABLE_WRITE_DATA;
					break;
				}
			}
			if (pOutputBuffer == NULL) {
				DISABLE_WRITE_DATA;
			}
			return true;
		}
		default:
		{
			ASSERT("Invalid state: %hu", event.filter);

			return false;
		}
	}
}

bool StdioCarrier::SignalOutputData() {
	ENABLE_WRITE_DATA;
	return true;
}

StdioCarrier::operator string() {
	if (_pProtocol != NULL)
		return STR(*_pProtocol);
	return format("IO(%d,%d)", _inboundFd, _outboundFd);
}

void StdioCarrier::GetStats(Variant &info, uint32_t namespaceId) {

}

#endif /* NET_KQUEUE */

