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


#include "protocols/tcpprotocol.h"
#include "netio/netio.h"

TCPProtocol::TCPProtocol()
: BaseProtocol(PT_TCP) {
	_decodedBytesCount = 0;
	_pCarrier = NULL;
}

TCPProtocol::~TCPProtocol() {
	if (_pCarrier != NULL) {
		IOHandler *pCarrier = _pCarrier;
		_pCarrier = NULL;
		pCarrier->SetProtocol(NULL);
		delete pCarrier;
	}
}

bool TCPProtocol::Initialize(Variant &parameters) {
	return true;
}

IOHandler *TCPProtocol::GetIOHandler() {
	return _pCarrier;
}

void TCPProtocol::SetIOHandler(IOHandler *pIOHandler) {
	if (pIOHandler != NULL) {
		if ((pIOHandler->GetType() != IOHT_TCP_CARRIER)
				&& (pIOHandler->GetType() != IOHT_STDIO)) {
			ASSERT("This protocol accepts only TCP carriers");
		}
	}
	_pCarrier = pIOHandler;
}

bool TCPProtocol::AllowFarProtocol(uint64_t type) {
	WARN("This protocol doesn't accept any far protocol");
	return false;
}

bool TCPProtocol::AllowNearProtocol(uint64_t type) {
	return true;
}

IOBuffer * TCPProtocol::GetInputBuffer() {
	return &_inputBuffer;
}

bool TCPProtocol::SignalInputData(int32_t recvAmount) {
	_decodedBytesCount += recvAmount;
	return _pNearProtocol->SignalInputData(_inputBuffer);
}

bool TCPProtocol::SignalInputData(IOBuffer & /* ignored */) {
	ASSERT("OPERATION NOT SUPPORTED");
	return false;
}

bool TCPProtocol::EnqueueForOutbound() {
	if (_pCarrier == NULL) {
		FATAL("TCPProtocol has no carrier");
		return false;
	}
	return _pCarrier->SignalOutputData();
}

uint64_t TCPProtocol::GetDecodedBytesCount() {
	return _decodedBytesCount;
}

