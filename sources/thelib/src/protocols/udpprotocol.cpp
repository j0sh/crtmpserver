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


#include "protocols/udpprotocol.h"
#include "netio/netio.h"

UDPProtocol::UDPProtocol()
: BaseProtocol(PT_UDP) {
	_decodedBytesCount = 0;
	_pCarrier = NULL;
}

UDPProtocol::~UDPProtocol() {
	if (_pCarrier != NULL) {
		IOHandler *pCarrier = _pCarrier;
		_pCarrier = NULL;
		pCarrier->SetProtocol(NULL);
		delete pCarrier;
	}
}

bool UDPProtocol::Initialize(Variant &parameters) {
	return true;
}

IOHandler *UDPProtocol::GetIOHandler() {
	return _pCarrier;
}

void UDPProtocol::SetIOHandler(IOHandler *pIOHandler) {
	if (pIOHandler != NULL) {
		if (pIOHandler->GetType() != IOHT_UDP_CARRIER) {
			ASSERT("This protocol accepts only UDP carrier");
		}
	}
	_pCarrier = pIOHandler;
}

bool UDPProtocol::AllowFarProtocol(uint64_t type) {
	WARN("This protocol doesn't accept any far protocol");
	return false;
}

bool UDPProtocol::AllowNearProtocol(uint64_t type) {
	return true;
}

IOBuffer * UDPProtocol::GetInputBuffer() {
	return &_inputBuffer;
}

bool UDPProtocol::SignalInputData(int32_t recvAmount) {
	ASSERT("Operation not supported");
	return false;
}

bool UDPProtocol::SignalInputData(int32_t recvAmount, sockaddr_in *pPeerAddress) {
	_decodedBytesCount += recvAmount;
	return _pNearProtocol->SignalInputData(_inputBuffer, pPeerAddress);
}

bool UDPProtocol::SignalInputData(IOBuffer & /* ignored */) {
	ASSERT("OPERATION NOT SUPPORTED");
	return false;
}

bool UDPProtocol::EnqueueForOutbound() {
	if (_pCarrier == NULL) {
		ASSERT("TCPProtocol has no carrier");
		return false;
	}
	return _pCarrier->SignalOutputData();
}

uint64_t UDPProtocol::GetDecodedBytesCount() {
	return _decodedBytesCount;
}


