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
#include "protocols/protocolfactorymanager.h"
#include "protocols/tcpprotocol.h"
#include "netio/kqueue/tcpcarrier.h"
#include "application/baseclientapplication.h"

TCPAcceptor::TCPAcceptor(string ipAddress, uint16_t port, Variant parameters,
		vector<uint64_t>/*&*/ protocolChain)
: IOHandler(0, 0, IOHT_ACCEPTOR) {
	_pApplication = NULL;
	memset(&_address, 0, sizeof (sockaddr_in));

	_address.sin_family = PF_INET;
	_address.sin_addr.s_addr = inet_addr(ipAddress.c_str());
	assert(_address.sin_addr.s_addr != INADDR_NONE);
	_address.sin_port = EHTONS(port); //----MARKED-SHORT----

	_protocolChain = protocolChain;
	_parameters = parameters;
	_enabled = false;
	_acceptedCount = 0;
	_droppedCount = 0;
	_ipAddress = ipAddress;
	_port = port;
}

TCPAcceptor::~TCPAcceptor() {
	CLOSE_SOCKET(_inboundFd);
}

bool TCPAcceptor::Bind() {
	_inboundFd = _outboundFd = (int) socket(PF_INET, SOCK_STREAM, 0);
	if (_inboundFd < 0) {
		FATAL("Unable to create socket: %s(%d)", strerror(errno), errno);
		return false;
	}

	if (!setFdOptions(_inboundFd)) {
		FATAL("Unable to set socket options");
		return false;
	}

	if (bind(_inboundFd, (sockaddr *) & _address, sizeof (sockaddr)) != 0) {
		int error = errno;
		FATAL("Unable to bind on address: tcp://%s:%hu; Error was: %s (%d)",
				inet_ntoa(((sockaddr_in *) & _address)->sin_addr),
				ENTOHS(((sockaddr_in *) & _address)->sin_port),
				strerror(error),
				error);
		return false;
	}

	if (_port == 0) {
		socklen_t tempSize = sizeof (sockaddr);
		if (getsockname(_inboundFd, (sockaddr *) & _address, &tempSize) != 0) {
			FATAL("Unable to extract the random port");
			return false;
		}
		_parameters[CONF_PORT] = (uint16_t) ENTOHS(_address.sin_port);
	}

	if (listen(_inboundFd, 100) != 0) {
		FATAL("Unable to put the socket in listening mode");
		return false;
	}

	_enabled = true;
	return true;
}

void TCPAcceptor::SetApplication(BaseClientApplication *pApplication) {
	assert(_pApplication == NULL);
	_pApplication = pApplication;
}

bool TCPAcceptor::StartAccept() {
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
	if (_pApplication == NULL)
		return Accept();
	return _pApplication->AcceptTCPConnection(this);
}

bool TCPAcceptor::Accept() {
	sockaddr address;
	memset(&address, 0, sizeof (sockaddr));
	socklen_t len = sizeof (sockaddr);
	int32_t fd;
	int32_t error;

	//1. Accept the connection
	fd = accept(_inboundFd, &address, &len);
	error = errno;
	if (fd < 0) {
		FATAL("Unable to accept client connection: %s (%d)", strerror(error), error);
		return false;
	}
	if (!_enabled) {
		CLOSE_SOCKET(fd);
		_droppedCount++;
		WARN("Acceptor is not enabled. Client dropped: %s:%hu -> %s:%hu",
				inet_ntoa(((sockaddr_in *) & address)->sin_addr),
				ENTOHS(((sockaddr_in *) & address)->sin_port),
				STR(_ipAddress),
				_port);
		return true;
	}
	INFO("Client connected: %s:%hu -> %s:%hu",
			inet_ntoa(((sockaddr_in *) & address)->sin_addr),
			ENTOHS(((sockaddr_in *) & address)->sin_port),
			STR(_ipAddress),
			_port);

	if (!setFdOptions(fd)) {
		FATAL("Unable to set socket options");
		CLOSE_SOCKET(fd);
		return false;
	}

	//4. Create the chain
	BaseProtocol *pProtocol = ProtocolFactoryManager::CreateProtocolChain(
			_protocolChain, _parameters);
	if (pProtocol == NULL) {
		FATAL("Unable to create protocol chain");
		CLOSE_SOCKET(fd);
		return false;
	}

	//5. Create the carrier and bind it
	TCPCarrier *pTCPCarrier = new TCPCarrier(fd);
	pTCPCarrier->SetProtocol(pProtocol->GetFarEndpoint());
	pProtocol->GetFarEndpoint()->SetIOHandler(pTCPCarrier);

	//6. Register the protocol stack with an application
	if (_pApplication != NULL) {
		pProtocol = pProtocol->GetNearEndpoint();
		pProtocol->SetApplication(_pApplication);
	}

	if (pProtocol->GetNearEndpoint()->GetOutputBuffer() != NULL)
		pProtocol->GetNearEndpoint()->EnqueueForOutbound();

	_acceptedCount++;

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

void TCPAcceptor::GetStats(Variant &info, uint32_t namespaceId) {
	info = _parameters;
	info["id"] = (((uint64_t) namespaceId) << 32) | GetId();
	info["enabled"] = (bool)_enabled;
	info["acceptedConnectionsCount"] = _acceptedCount;
	info["droppedConnectionsCount"] = _droppedCount;
	if (_pApplication != NULL) {
		info["appId"] = (((uint64_t) namespaceId) << 32) | _pApplication->GetId();
		info["appName"] = _pApplication->GetName();
	} else {
		info["appId"] = (((uint64_t) namespaceId) << 32);
		info["appName"] = "";
	}
}

bool TCPAcceptor::Enable() {
	return _enabled;
}

void TCPAcceptor::Enable(bool enabled) {
	_enabled = enabled;
}

bool TCPAcceptor::IsAlive() {
	//TODO: Implement this. It must return true
	//if this acceptor is operational
	NYI;
	return true;
}

#endif /* NET_KQUEUE */

