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
#include "netio/kqueue/tcpacceptor.h"
#include "netio/kqueue/iohandlermanager.h"
#include "utils/core.h"
#include "protocols/protocolfactorymanager.h"
#include "protocols/tcpprotocol.h"
#include "netio/kqueue/tcpcarrier.h"

TCPAcceptor::TCPAcceptor(string ipAddress, uint16_t port, Variant parameters,
        vector<uint64_t>/*&*/ protocolChain)
: IOHandler(0, 0, IOHT_ACCEPTOR) {
    _pApplication = NULL;
    //assert((port > 1024 && port < 65535) || (port == 80));
    memset(&_address, 0, sizeof (sockaddr_in));

    _address.sin_family = PF_INET;
    _address.sin_addr.s_addr = inet_addr(ipAddress.c_str());
    assert(_address.sin_addr.s_addr != INADDR_NONE);
    _address.sin_port = htons(port); //----MARKED-SHORT----

    _protocolChain = protocolChain;
    _parameters = parameters;
}

TCPAcceptor::~TCPAcceptor() {
    close(_inboundFd);
}

bool TCPAcceptor::StartAccept(BaseClientApplication *pApplication) {
    _pApplication = pApplication;

    _inboundFd = _outboundFd = (int) socket(PF_INET, SOCK_STREAM, 0);
    if (_inboundFd <= 0) {
        FATAL("Unable to create socket: %s(%d)", strerror(errno), errno);
        return false;
    }

    int32_t one = 1;

    if (setsockopt(_inboundFd, SOL_SOCKET, SO_REUSEADDR, (char *) & one, sizeof (one)) != 0) {
        FATAL("Unable to reuse address");
        return false;
    }

    one = 1;
    if (setsockopt(_inboundFd, IPPROTO_TCP, TCP_NODELAY, (char *) & one, sizeof (one)) != 0) {
        FATAL("Unable to disable Nagle");
        return false;
    }

    if (!SetFdNoSIGPIPE(_inboundFd)) {
        FATAL("Unable to set SO_NOSIGPIPE");
        return false;
    }

    if (bind(_inboundFd, (sockaddr *) & _address, sizeof (sockaddr)) != 0) {
        int error = errno;
        FATAL("Unable to bind on address: tcp://%s:%d; Error was: %s (%d)",
                inet_ntoa(((sockaddr_in *) & _address)->sin_addr),
                ntohs(((sockaddr_in *) & _address)->sin_port),
                strerror(error),
                error);
        return false;
    }

    if (listen(_inboundFd, 100) != 0) {
        FATAL("Unable to put the socket in listening mode");
        return false;
    }

    return IOHandlerManager::EnableAcceptConnections(this);
}

bool TCPAcceptor::SignalOutputData() {
    ASSERT("Operation not supported");
    return false;
}

bool TCPAcceptor::OnEvent(struct kevent &event) {
    if (!OnConnectionAvailable(event))
        return IsAlive();
    else
        return true;
}

bool TCPAcceptor::OnConnectionAvailable(struct kevent &event) {
    sockaddr address;
    memset(&address, 0, sizeof (sockaddr));
    socklen_t len = sizeof (sockaddr);
    int32_t fd;
    int32_t error;

    //1. Accept the connection
    fd = accept(_inboundFd, &address, &len);
    error = errno;
    if (fd <= 0) {
        FATAL("Unable to accept client connection: %s (%d)", strerror(error), error);
        return false;
    }
    INFO("Client connected: %s", inet_ntoa(((sockaddr_in *) & address)->sin_addr));

    //2. Disable Nagle's algorithm
    int32_t one = 1;
    if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY,
            (const char*) & one, sizeof (one)) != 0) {
        FATAL("Unable to set TCP_NODELAY");
        return false;
    }

    if (setsockopt(_inboundFd, SOL_SOCKET, SO_KEEPALIVE, (char *) & one, sizeof (one)) != 0) {
        FATAL("Unable to enable keepalive");
        return false;
    }

    if (!SetFdNoSIGPIPE(fd)) {
        FATAL("Unable to set SO_NOSIGPIPE");
        return false;
    }

    //3. Put the socket in non-blocking mode
    int flags;
    if ((flags = fcntl(fd, F_GETFL, 0)) < 0) {
        FATAL("Unable to get current flags");
        return false;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        FATAL("Unable to set O_NONBLOCK");
        return false;
    }

    //4. Create the chain
    BaseProtocol *pProtocol = ProtocolFactoryManager::CreateProtocolChain(
            _protocolChain, _parameters);
    if (pProtocol == NULL) {
        FATAL("Unable to create protocol chain");
        close(fd);
        return false;
    }

    //5. Create the carrier and bind it
    TCPCarrier *pTCPCarrier = new TCPCarrier(fd, pProtocol->GetFarEndpoint());
    pProtocol->GetFarEndpoint()->SetIOHandler(pTCPCarrier);

    //6. Register the protocol stack with an application
    if (_pApplication != NULL) {
        pProtocol = pProtocol->GetNearEndpoint();
        pProtocol->SetApplication(_pApplication);
    }

    //7. Done
    return true;
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

TCPAcceptor::operator string() {
    return format("A(%d)", _inboundFd);
}

bool TCPAcceptor::IsAlive() {
    //TODO: Implement this. It must return true
    //if this acceptor is operational
    NYI;
    return true;
}

#endif /* NET_KQUEUE */

