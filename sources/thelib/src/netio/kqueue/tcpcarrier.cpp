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
#include "netio/kqueue/tcpcarrier.h"
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
	_pProtocol->ReadyForSend(); \
	if(_pProtocol->GetOutputBuffer()==NULL) {\
		_writeDataEnabled = false; \
		IOHandlerManager::DisableWriteData(this); \
	} \
}

TCPCarrier::TCPCarrier(int32_t fd)
: IOHandler(fd, fd, IOHT_TCP_CARRIER) {
	IOHandlerManager::EnableReadData(this);
	_writeDataEnabled = false;
	memset(&_farAddress, 0, sizeof (sockaddr_in));
	_farIp = "";
	_farPort = 0;
	memset(&_nearAddress, 0, sizeof (sockaddr_in));
	_nearIp = "";
	_nearPort = 0;
	GetEndpointsInfo();
	_rx = 0;
	_tx = 0;
	//	uint32_t sendBufferSize = 1024 * 1024;
	//	if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &sendBufferSize, sizeof (uint32_t)) != 0) {
	//		ASSERT("Unable to determine the send buffer size");
	//	}
}

TCPCarrier::~TCPCarrier() {
	//FINEST("Delete tcp carrier %p", this);
	close(_inboundFd);
	//FINEST("Done delete tcp carrier %p", this);
}

bool TCPCarrier::OnEvent(struct kevent &event) {
	int32_t readAmount = 0;
	int32_t writeAmount = 0;

	//3. Do the I/O
	switch (event.filter) {
		case EVFILT_READ:
		{
			IOBuffer *pInputBuffer = _pProtocol->GetInputBuffer();
			assert(pInputBuffer != NULL);
			if (!pInputBuffer->ReadFromTCPFd(event.ident, event.data, readAmount)) {
				FATAL("Unable to read data. %s:%d -> %s:%d",
						STR(_farIp), _farPort,
						STR(_nearIp), _nearPort);
				return false;
			}
			_rx += readAmount;
			return _pProtocol->SignalInputData(readAmount);
		}
		case EVFILT_WRITE:
		{
			IOBuffer *pOutputBuffer = NULL;

			if ((pOutputBuffer = _pProtocol->GetOutputBuffer()) != NULL) {
				//uint32_t initial = GETAVAILABLEBYTESCOUNT(*pOutputBuffer);
				//FINEST("Try to send buffer:\n%s", STR(*pOutputBuffer));
				if (!pOutputBuffer->WriteToTCPFd(event.ident, event.data, writeAmount)) {
					FATAL("Unable to send data. %s:%d -> %s:%d",
							STR(_farIp), _farPort,
							STR(_nearIp), _nearPort);
					IOHandlerManager::EnqueueForDelete(this);
					return false;
				}
				_tx += writeAmount;
				if (GETAVAILABLEBYTESCOUNT(*pOutputBuffer) == 0) {
					DISABLE_WRITE_DATA;
				}
				//				else {
				//					FINEST("Advertised: %d; sent: %d; initial :%d; leftovers: %d;",
				//							event.data,
				//							writeAmount,
				//							initial,
				//							GETAVAILABLEBYTESCOUNT(*pOutputBuffer));
				//				}
			} else {
				DISABLE_WRITE_DATA;
			}
			return true;
		}
		default:
		{
			ASSERT("Invalid state: %d", event.filter);

			return false;
		}
	}
}

bool TCPCarrier::SignalOutputData() {
	ENABLE_WRITE_DATA;
	return true;
}

TCPCarrier::operator string() {
	if (_pProtocol != NULL)
		return STR(*_pProtocol);
	return format("TCP(%d)", _inboundFd);
}

void TCPCarrier::GetStats(Variant &info) {
	if (!GetEndpointsInfo()) {
		FATAL("Unable to get endpoints info");
		info = "unable to get endpoints info";
		return;
	}
	info["type"] = "IOHT_TCP_CARRIER";
	info["farIP"] = _farIp;
	info["farPort"] = _farPort;
	info["nearIP"] = _nearIp;
	info["nearPort"] = _nearPort;
	info["rx"] = _rx;
	info["tx"] = _tx;
}

sockaddr_in &TCPCarrier::GetFarEndpointAddress() {
	if ((_farIp == "") || (_farPort == 0))
		GetEndpointsInfo();
	return _farAddress;
}

string TCPCarrier::GetFarEndpointAddressIp() {
	if (_farIp != "")
		return _farIp;
	GetEndpointsInfo();
	return _farIp;
}

uint16_t TCPCarrier::GetFarEndpointPort() {
	if (_farPort != 0)
		return _farPort;
	GetEndpointsInfo();
	return _farPort;
}

sockaddr_in &TCPCarrier::GetNearEndpointAddress() {
	if ((_nearIp == "") || (_nearPort == 0))
		GetEndpointsInfo();
	return _nearAddress;
}

string TCPCarrier::GetNearEndpointAddressIp() {
	if (_nearIp != "")
		return _nearIp;
	GetEndpointsInfo();
	return _nearIp;
}

uint16_t TCPCarrier::GetNearEndpointPort() {
	if (_nearPort != 0)
		return _nearPort;
	GetEndpointsInfo();
	return _nearPort;
}

bool TCPCarrier::GetEndpointsInfo() {
	socklen_t len = sizeof (sockaddr);
	if (getpeername(_inboundFd, (sockaddr *) & _farAddress, &len) != 0) {
		FATAL("Unable to get peer's address");
		return false;
	}
	_farIp = format("%s", inet_ntoa(((sockaddr_in *) & _farAddress)->sin_addr));
	_farPort = ENTOHS(((sockaddr_in *) & _farAddress)->sin_port); //----MARKED-SHORT----

	if (getsockname(_inboundFd, (sockaddr *) & _nearAddress, &len) != 0) {
		FATAL("Unable to get peer's address");
		return false;
	}
	_nearIp = format("%s", inet_ntoa(((sockaddr_in *) & _nearAddress)->sin_addr));
	_nearPort = ENTOHS(((sockaddr_in *) & _nearAddress)->sin_port); //----MARKED-SHORT----
	//FINEST("_nearAddress: %s; _nearPort: %d", STR(_nearAddress), _nearPort);
	return true;
}

#endif /* NET_KQUEUE */

