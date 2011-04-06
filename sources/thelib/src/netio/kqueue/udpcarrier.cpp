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

#include "netio/kqueue/udpcarrier.h"
#include "netio/kqueue/iohandlermanager.h"
#include "protocols/baseprotocol.h"

UDPCarrier::UDPCarrier(int32_t fd)
: IOHandler(fd, fd, IOHT_UDP_CARRIER) {
	IOHandlerManager::EnableReadData(this);
	memset(&_peerAddress, 0, sizeof (sockaddr_in));
	memset(&_nearAddress, 0, sizeof (sockaddr_in));
	_nearIp = "";
	_nearPort = 0;
	_rx = 0;
	_tx = 0;
}

UDPCarrier::~UDPCarrier() {
	close(_inboundFd);
}

bool UDPCarrier::OnEvent(struct kevent &event) {
	int32_t recvAmount = 0;

	//3. Do the I/O
	switch (event.filter) {
		case EVFILT_READ:
		{
			IOBuffer *pInputBuffer = _pProtocol->GetInputBuffer();
			assert(pInputBuffer != NULL);
			if (!pInputBuffer->ReadFromUDPFd(event.ident, recvAmount, _peerAddress)) {
				FATAL("Unable to read data");
				return false;
			}
			_rx += recvAmount;
			return _pProtocol->SignalInputData(recvAmount, &_peerAddress);
		}
		case EVFILT_WRITE:
		{
			NYIR;
		}
		default:
		{
			ASSERT("Invalid state: %hu", event.filter);
			return false;
		}
	}
}

bool UDPCarrier::SignalOutputData() {
	NYIR;
}

UDPCarrier::operator string() {
	if (_pProtocol != NULL)
		return STR(*_pProtocol);
	return format("UDP(%d)", _inboundFd);
}

void UDPCarrier::GetStats(Variant &info) {
	if (!GetEndpointsInfo()) {
		FATAL("Unable to get endpoints info");
		info = "unable to get endpoints info";
		return;
	}
	info["type"] = "IOHT_UDP_CARRIER";
	info["nearIP"] = _nearIp;
	info["nearPort"] = _nearPort;
	info["rx"] = _rx;
}

string UDPCarrier::GetFarEndpointAddress() {
	ASSERT("Operation not supported");
	return "";
}

uint16_t UDPCarrier::GetFarEndpointPort() {
	ASSERT("Operation not supported");
	return 0;
}

string UDPCarrier::GetNearEndpointAddress() {
	if (_nearIp != "")
		return _nearIp;
	GetEndpointsInfo();
	return _nearIp;
}

uint16_t UDPCarrier::GetNearEndpointPort() {
	if (_nearPort != 0)
		return _nearPort;
	GetEndpointsInfo();
	return _nearPort;
}

UDPCarrier* UDPCarrier::Create(string bindIp, uint16_t bindPort) {

	//1. Create the socket
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		FATAL("Unable to create socket: %s(%d)", strerror(errno), errno);
		return NULL;
	}

	//2. No SIGPIPE
	if (!setFdNoSIGPIPE(sock)) {
		FATAL("Unable to set SO_NOSIGPIPE");
		close(sock);
		return NULL;
	}

	//3. bind if necessary
	sockaddr_in bindAddress;
	memset(&bindAddress, 0, sizeof (bindAddress));
	if (bindIp != "") {
		bindAddress.sin_family = PF_INET;
		bindAddress.sin_addr.s_addr = inet_addr(bindIp.c_str());
		bindAddress.sin_port = EHTONS(bindPort); //----MARKED-SHORT----
		if (bindAddress.sin_addr.s_addr == INADDR_NONE) {
			FATAL("Unable to bind on address %s:%hu", STR(bindIp), bindPort);
			close(sock);
			return NULL;
		}
		if (bind(sock, (sockaddr *) & bindAddress, sizeof (sockaddr)) != 0) {
			int error = errno;
			FATAL("Unable to bind on address: udp://%s:%hu; Error was: %s (%d)",
					STR(bindIp), bindPort, strerror(error), error);
			close(sock);
			return NULL;
		}
	}

	//4. Create the carrier
	UDPCarrier *pResult = new UDPCarrier(sock);
	pResult->_nearAddress = bindAddress;

	return pResult;
}

UDPCarrier* UDPCarrier::Create(string bindIp, uint16_t bindPort, BaseProtocol *pProtocol) {
	if (pProtocol == NULL) {
		FATAL("Protocol can't be null");
		return NULL;
	}

	UDPCarrier *pResult = Create(bindIp, bindPort);
	if (pResult == NULL) {
		FATAL("Unable to create UDP carrier");
		return NULL;
	}

	pResult->SetProtocol(pProtocol->GetFarEndpoint());
	pProtocol->GetFarEndpoint()->SetIOHandler(pResult);

	return pResult;
}

bool UDPCarrier::Setup(Variant &settings) {
	NYIR;
}

bool UDPCarrier::GetEndpointsInfo() {
	socklen_t len = sizeof (sockaddr);
	if (getsockname(_inboundFd, (sockaddr *) & _nearAddress, &len) != 0) {
		FATAL("Unable to get peer's address");
		return false;
	}
	_nearIp = format("%s", inet_ntoa(((sockaddr_in *) & _nearAddress)->sin_addr));
	_nearPort = ENTOHS(((sockaddr_in *) & _nearAddress)->sin_port); //----MARKED-SHORT----
	return true;
}

#endif /* NET_KQUEUE */

