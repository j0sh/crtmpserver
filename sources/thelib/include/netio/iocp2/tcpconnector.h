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
#ifndef _TCPCONNECTOR_H
#define	_TCPCONNECTOR_H

#include "netio/iocp2/iohandler.h"

typedef struct _ConnectOV : public GenericOV {
    SOCKET connectSocket;
    sockaddr_in address;
    uint32_t bytesSent;
} ConnectOV;

template<class T>
class TCPConnector
: public IOHandler {
private:
    string _ip;
    uint16_t _port;
    vector<uint64_t> _protocolChain;
    bool _closeSocket;
    Variant _customParameters;
    ConnectOV *_pConnectOV;
    sockaddr_in _address;
public:

    TCPConnector(int32_t fd, string ip, uint16_t port,
            vector<uint64_t>& protocolChain, const Variant& customParameters)
    : IOHandler(fd,fd, IOHT_TCP_CONNECTOR) {
        _ip = ip;
        _port = port;
        _protocolChain = protocolChain;
        _closeSocket = true;
        _customParameters = customParameters;
        _pConnectOV = NULL;
    }

    virtual ~TCPConnector() {
        if (_closeSocket)
            CLOSE_SOCKET(_inboundFd);
        FreeGenericOV(_pConnectOV);
    }

	virtual bool SignalOutputData(){
		ASSERT("Operation not supported");
		return false;
	}

    virtual bool OnEvent(GenericOV *pGenericOV) {
        //1. Enqueue the connector for delete
        IOHandlerManager::EnqueueForDelete(this);

        //2. Get the connect result
        ConnectOV *pConnectOV = (ConnectOV *) pGenericOV;
        uint32_t bytesTransferred;
        BOOL connectResult = GetOverlappedResult((HANDLE) pConnectOV->connectSocket, pGenericOV, &bytesTransferred, FALSE);
        if (!connectResult) {
            FATAL("Connector failed");
            _closeSocket = true;
            return false;
        }

        //3. Create the protocol chain
		BaseProtocol *pProtocol = ProtocolFactoryManager::CreateProtocolChain(_protocolChain, _customParameters);
        if (pProtocol == NULL) {
            FATAL("Unable to create protocol chain");
            _closeSocket = true;
            return false;
        }

		TCPCarrier *pTCPCarrier = new TCPCarrier(_inboundFd, pProtocol->GetFarEndpoint());
        pProtocol->GetFarEndpoint()->SetIOHandler(pTCPCarrier);

		//TODO:
        //4. THIS IS A HACK. Mark the newly created IOHandler as already registered to IOCP
        //and also re-enable the Read thing
        pProtocol->GetIOHandler()->GetIOHandlerManagerToken()->registeredIntoIOCP = true;
        if (!IOHandlerManager::EnableReadData(pProtocol->GetIOHandler())) {
            FATAL("Unable toenable read data");
            delete pProtocol;
            _closeSocket = true;
            return false;
        }

        //5. Signal that the protocol stack is created
        if (!T::SignalProtocolCreated(pProtocol, _customParameters)) {
            FATAL("Unable to signal protocol created");
            delete pProtocol;
            _closeSocket = true;
            return false;
        }

        //6. We are done. Don't close the socket
        _closeSocket = false;
        return true;
    }

    static bool Connect(string ip, uint16_t port,
            vector<uint64_t>& protocolChain, Variant customParameters) {

        //2. create the socket
        int32_t fd = (int32_t) socket(PF_INET, SOCK_STREAM, 0);
        if (fd <= 0) {
            FATAL("Unable to create fd");
            return 0;
        }

        //3. Set it as non-blocking
        if (!SetFdNonBlock(fd)) {
            FATAL("Unable to put socket in non-blocking mode");
            return false;
        }

        //4. Bind it to ANY:ANY
        sockaddr_in address = {0};
        address.sin_family = PF_INET;
        address.sin_addr.s_addr = ADDR_ANY;
        address.sin_port = 0;

        if (bind(fd, (sockaddr *) & address, sizeof (sockaddr_in)) != 0) {
            FATAL("Unable to bind fd");
            return 0;
        }

        //5. Spawn the connector instance
        TCPConnector<T> *pTCPConnector = new TCPConnector(fd, ip, port,
                protocolChain, customParameters);

        //6. Issue the connect
        if (!pTCPConnector->Connect()) {
            IOHandlerManager::EnqueueForDelete(pTCPConnector);
            FATAL("Unable to connect");
            return false;
        }

        //7. We are done here...
        return true;
    }

    bool Connect() {
        //1. create the ConnectOV and zero it
        _pConnectOV = new ConnectOV;
        memset(_pConnectOV, 0, sizeof (ConnectOV));

        //2. setup the address
        _pConnectOV->address.sin_family = PF_INET;
        _pConnectOV->address.sin_addr.s_addr = inet_addr(_ip.c_str());
        if (_pConnectOV->address.sin_addr.s_addr == INADDR_NONE) {
            FATAL("Unable to translate string %s to a valid IP address", STR(_ip));
            return 0;
        }
        _pConnectOV->address.sin_port = htons(_port); //----MARKED-SHORT----

        //3. set up the socket
		_pConnectOV->connectSocket = _inboundFd;

        //4. setup the token, owner and type
        _pConnectOV->pToken = GetIOHandlerManagerToken();
        _pConnectOV->ownerId = GetId();
        _pConnectOV->type = GetType();

        if (!IOHandlerManager::EnableConnectEvent(this, _pConnectOV)) {
            FATAL("Unable to enable connect event");
            IOHandlerManager::EnqueueForDelete(this);
            return false;
        }

        return true;
    }

    operator string() {
        NYIA;
        return "";
    }
};


#endif	/* _TCPCONNECTOR_H */
#endif /* NET_IOCP2 */


