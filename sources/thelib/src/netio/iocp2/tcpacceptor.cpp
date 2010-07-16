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
#include "netio/iocp2/tcpacceptor.h"
#include "netio/iocp2/iohandlermanager.h"
#include "netio/iocp2/tcpcarrier.h"
#include "protocols/protocolfactorymanager.h"
#include "protocols/baseprotocol.h"

TCPAcceptor::TCPAcceptor(string ipAddress, uint16_t port, Variant parameters, vector<uint64_t>/*&*/ protocolChain)
: IOHandler(0,0, IOHT_ACCEPTOR) {
	_pApplication=NULL;
    assert((port > 1024 && port < 65535) || (port == 80));
    memset(&_address, 0, sizeof (sockaddr_in));

    _address.sin_family = PF_INET;
    _address.sin_addr.s_addr = inet_addr(ipAddress.c_str());
    assert(_address.sin_addr.s_addr != INADDR_NONE);
    _address.sin_port = htons(port); //----MARKED-SHORT----

    _protocolChain = protocolChain;
    _pAcceptOV = NULL;
	_parameters = parameters;
}

TCPAcceptor::~TCPAcceptor() {
    closesocket(_inboundFd);
    FreeGenericOV(_pAcceptOV);
}

bool TCPAcceptor::StartAccept(BaseClientApplication *pApplication) {
	_pApplication = pApplication;

    //2. create the socket
    _inboundFd = (int) socket(PF_INET, SOCK_STREAM, 0);
    if (_inboundFd <= 0) {
        FATAL("Unable to create socket");
        return false;
    }

    int32_t one = 1;
    int32_t zero = 0;

    //3. Reuse address. Mostly used for debugging purposes.
    //if the server crashes, the listener will remain in TIME_WAIT
    //and you are forced to wait few seconds before restarting
    if (setsockopt(_inboundFd, SOL_SOCKET, SO_REUSEADDR, (char *) & one, sizeof (one)) != 0) {
        FATAL("Unable to reuse address");
        return false;
    }

    //4. Send data immediately
    if (setsockopt(_inboundFd, IPPROTO_TCP, TCP_NODELAY, (char *) & one, sizeof (one)) != 0) {
        FATAL("Unable to disable Nagle");
        return false;
    }

    //5. bind
    if (bind(_inboundFd, (sockaddr *) & _address, sizeof (sockaddr)) != 0) {
        int error = errno;
        FATAL("Unable to bind on address: \"PLEASE_FIX_THIS\"; Error was: %s (%d)",
                strerror(error),
                error);
        return false;
    }


    //6. Listen
    if (listen(_inboundFd, 100) != 0) {
        FATAL("Unable to put the socket in listening mode");
        return false;
    }

    //7. Start the accepting process
    return IOHandlerManager::EnableAcceptConnections(this);
}

bool TCPAcceptor::SignalOutputData() {
    ASSERT("Operation not supported");
    return false;
}

bool TCPAcceptor::OnEvent(GenericOV *pGenericOV) {
    return OnConnectionAvailable(pGenericOV);
}

bool TCPAcceptor::OnConnectionAvailable(GenericOV *pGenericOV) {
    //1. get the structure
    AcceptOV *pAcceptOV = (AcceptOV *) pGenericOV;

    //2. Get the accepted socket
    SOCKET accepted = pAcceptOV->acceptedSocket;

    //3. Get the peers address
    sockaddr *pLocalAddress = NULL;
    socklen_t localLen = sizeof (sockaddr);
    sockaddr *pRemoteAddress = NULL;
    socklen_t remoteLen = sizeof (sockaddr);
    GetAcceptExSockaddrs(pAcceptOV->rawBuffer, 0,
            sizeof (sockaddr_in) + 16, sizeof (sockaddr_in) + 16,
            &pLocalAddress, &localLen, &pRemoteAddress, &remoteLen);
    INFO("Client connected: %s:%d -> %s:%d",
            inet_ntoa(((sockaddr_in *) pRemoteAddress)->sin_addr),
            ntohs(((sockaddr_in *) pRemoteAddress)->sin_port), //----MARKED-SHORT----
            inet_ntoa(((sockaddr_in *) pLocalAddress)->sin_addr),
            ntohs(((sockaddr_in *) pLocalAddress)->sin_port)); //----MARKED-SHORT----

    int32_t one = 1;

    //4. set the no delay flag on the accepted socket
    if (setsockopt(accepted, IPPROTO_TCP, TCP_NODELAY,
            (const char*) & one, sizeof (one)) != 0) {
        FATAL("Unable to set TCP_NODELAY");
        closesocket(accepted);
        return false;
	}


	//5. Create the chain
    BaseProtocol *pProtocol = ProtocolFactoryManager::CreateProtocolChain(_protocolChain, _parameters);
    if (pProtocol == NULL) {
        FATAL("Unable to create protocol chain");
        closesocket(accepted);
        return false;
    }

	//6. Create the carrier and bind it
    TCPCarrier *pTCPCarrier = new TCPCarrier(accepted, pProtocol->GetFarEndpoint());
    pProtocol->GetFarEndpoint()->SetIOHandler(pTCPCarrier);

    //7. Register the protocol stack with an application
    if (_pApplication != NULL) {
        pProtocol = pProtocol->GetNearEndpoint();
        pProtocol->SetApplication(_pApplication);
    }

    //8. Issue a new accept
    return IOHandlerManager::EnableAcceptConnections(this);
}

AcceptOV * TCPAcceptor::GetAcceptOV() {
    //1. Allocate the AcceptOV and zero it if necessary
    if (_pAcceptOV == NULL) {
        _pAcceptOV = new AcceptOV;
        memset(_pAcceptOV, 0, sizeof (AcceptOV));
    }

    //2. Create the socket
    _pAcceptOV->acceptedSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (_pAcceptOV->acceptedSocket == INVALID_SOCKET) {
        FATAL("Unable to create socket. Error was: %d", WSAGetLastError());
        return NULL;
    }

    //3. Instruct the iocp to inherit all the flags set up on the acceptor
    if (setsockopt(_pAcceptOV->acceptedSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
            (char *) & _inboundFd, sizeof (_inboundFd)) == 0) {
        FATAL("Unable to set the inherit flag");
        return NULL;
    }

    //4. Set the rawBuffer length
    _pAcceptOV->rawBufferLength = sizeof (_pAcceptOV->rawBuffer);

    //5. Set the ownership and type
    _pAcceptOV->pToken = GetIOHandlerManagerToken();
    _pAcceptOV->ownerId = GetId();
    _pAcceptOV->type = GetType();

    //6. done
    return _pAcceptOV;
}

TCPAcceptor::operator string() {
    return format("A(%d)", _inboundFd);
}

Variant & TCPAcceptor::GetParameters() {
    return _parameters;
}

BaseClientApplication *TCPAcceptor::GetApplication() {
    return _pApplication;
}

vector<uint64_t> &TCPAcceptor::GetProtocolChain() {
    return _protocolChain;
} 

#endif /* NET_IOCP2 */

