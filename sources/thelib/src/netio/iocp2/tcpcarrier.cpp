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


#ifdef NET_IOCP2
#include "netio/iocp2/tcpcarrier.h"
#include "netio/iocp2/iohandlermanager.h"
#include "protocols/baseprotocol.h"
#include "buffering/iobuffer.h"

TCPCarrier::TCPCarrier(int32_t fd, BaseProtocol *pProtocol)
: IOHandler(fd,fd, IOHT_TCP_CARRIER) {
    _pProtocol = pProtocol;
    _writeDataEnabled = false;
    _pReadOV = NULL;
    _pWriteOV = NULL;
    _pProtocol->SetIOHandler(this);
    IOHandlerManager::EnableReadData(this);
    _farAddress = "";
    _farPort = 0; 
}

TCPCarrier::~TCPCarrier() {
    closesocket(_inboundFd);
    FreeGenericOV(_pReadOV);
    FreeGenericOV(_pWriteOV);
    if (_pProtocol != NULL) {
        _pProtocol->SetIOHandler(NULL);
        delete _pProtocol;
    }
}

void TCPCarrier::ResetProtocol() {
    _pProtocol = NULL;
}

bool TCPCarrier::OnEvent(GenericOV *pGenericOV) {
    ReadWriteOV *pReadWriteOV = (ReadWriteOV *) pGenericOV;

    //1. Get the result
    uint32_t transferCount;
    BOOL overlappedResult = GetOverlappedResult((HANDLE) GetInboundFd(), pGenericOV, &transferCount, FALSE);
    if (!overlappedResult) {
        if (GetLastError() != ERROR_IO_INCOMPLETE) {
            FATAL("Unable to get overlapped result");
            return false;
        }
        return true;
    }

    if (pReadWriteOV->isRead) {
        //3. get the input buffer
        IOBuffer *pInputBuffer = _pProtocol->GetInputBuffer();
        assert(pInputBuffer != NULL);

        //4. read data
        int32_t recvBytes = 0;
        if (!pInputBuffer->ReadFromTCPFd(_inboundFd, FD_READ_CHUNK, recvBytes)) {
            FATAL("Unable to read data");
            return false;
        }

        //6. Signal the stack that there is data to be processed
        if (!_pProtocol->SignalInputData(recvBytes)) {
            FATAL("Unable to signal data available");
            return false;
        }

        //7. Re-enable the read event
        if (!IOHandlerManager::EnableReadData(this)) {
            FATAL("Unable to reactivate the read");
            return false;
        }
    } else {
        _writeDataEnabled = false;
        IOBuffer *pOutputBuffer = NULL;

        while ((pOutputBuffer = _pProtocol->GetOutputBuffer()) != NULL) {
            //FINEST("Try to send buffer:\n%s", STR(*pOutputBuffer));
            if (!pOutputBuffer->WriteToTCPFd(_inboundFd, FD_WRITE_CHUNK)) {
                FATAL("Unable to send data");
                IOHandlerManager::EnqueueForDelete(this);
                return false;
            }
            if (GETAVAILABLEBYTESCOUNT(*pOutputBuffer) > 0) {
                IOHandlerManager::EnableWriteData(this);
                break;
            }
        }

        if (pOutputBuffer == NULL) {
            _pProtocol->ReadyForSend();
        }
    }

    return true;
}

bool TCPCarrier::SignalOutputData() {
    if (!_writeDataEnabled) {
        _writeDataEnabled = true;
        return IOHandlerManager::EnableWriteData(this);
    }
    return true;
}

ReadWriteOV *TCPCarrier::GetReadWriteOV(bool isRead) {
    //1. Allocate the AcceptOV and zero it if necessary
    ReadWriteOV *_pResult;

    if (isRead) {
        if (_pReadOV == NULL) {
            _pReadOV = new ReadWriteOV;
            memset(_pReadOV, 0, sizeof (ReadWriteOV));
        }
        _pResult = _pReadOV;
    } else {
        if (_pWriteOV == NULL) {
            _pWriteOV = new ReadWriteOV;
            memset(_pWriteOV, 0, sizeof (ReadWriteOV));
        }
        _pResult = _pWriteOV;
    }

    //2. Set the ownership and type
    _pResult->isRead = isRead;
    _pResult->pToken = GetIOHandlerManagerToken();
    _pResult->ownerId = GetId();
    _pResult->type = GetType();

    //3. done
    return _pResult;
}

TCPCarrier::operator string() {
    if (_pProtocol != NULL)
        return STR(*_pProtocol);
    return format("C(%d)", _inboundFd);
}

string TCPCarrier::GetFarEndpointAddress() {
    if (_farAddress != "")
        return _farAddress;
    GetEndpointsInfo();
    return _farAddress;
}

uint16_t TCPCarrier::GetFarEndpointPort() {
    if (_farPort != 0)
        return _farPort;
    GetEndpointsInfo();
    return _farPort;
}

string TCPCarrier::GetNearEndpointAddress() {
    if (_nearAddress != "")
        return _nearAddress;
    GetEndpointsInfo();
    return _nearAddress;
}

uint16_t TCPCarrier::GetNearEndpointPort() {
    if (_nearPort != 0)
        return _nearPort;
    GetEndpointsInfo();
    return _nearPort;
}

bool TCPCarrier::GetEndpointsInfo() {
    sockaddr_in address;
    socklen_t len = sizeof (sockaddr);
    if (getpeername(_inboundFd, (sockaddr *) & address, &len) != 0) {
        FATAL("Unable to get peer's address");
        return false;
    }
    _farAddress = format("%s", inet_ntoa(((sockaddr_in *) & address)->sin_addr));
    _farPort = ntohs(((sockaddr_in *) & address)->sin_port); //----MARKED-SHORT----

    if (getsockname(_inboundFd, (sockaddr *) & address, &len) != 0) {
        FATAL("Unable to get peer's address");
        return false;
    }
    _nearAddress = format("%s", inet_ntoa(((sockaddr_in *) & address)->sin_addr));
    _nearPort = ntohs(((sockaddr_in *) & address)->sin_port); //----MARKED-SHORT----
    //FINEST("_nearAddress: %s; _nearPort: %d", STR(_nearAddress), _nearPort);
    return true;
}


#endif /* NET_IOCP2 */

