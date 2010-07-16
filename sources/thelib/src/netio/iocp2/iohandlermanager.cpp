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
#include "netio/iocp2/iohandlermanager.h"
#include "netio/iocp2/iohandler.h"
#include "netio/iocp2/tcpacceptor.h"
#include "netio/iocp2/tcpcarrier.h"
#include "netio/iocp2/tcpconnector.h"

bool IOHandlerManager::_signalShutdown = false;
HANDLE IOHandlerManager::_iocp = NULL;
uint32_t IOHandlerManager::_numberOfThreads = 0;
map<uint32_t, IOHandler *> IOHandlerManager::_activeIOHandlers;
map<uint32_t, IOHandler *> IOHandlerManager::_deadIOHandlers;
vector<IOHandlerManagerToken *> IOHandlerManager::_tokensVector1;
vector<IOHandlerManagerToken *> IOHandlerManager::_tokensVector2;
vector<IOHandlerManagerToken *> *IOHandlerManager::_pAvailableTokens;
vector<IOHandlerManagerToken *> *IOHandlerManager::_pRecycledTokens;
TimersManager *IOHandlerManager::_pTimersManager = NULL;

map<uint32_t, IOHandler *> & IOHandlerManager::GetActiveHandlers() {
    return _activeIOHandlers;
}

map<uint32_t, IOHandler *> & IOHandlerManager::GetDeadHandlers() {
    return _deadIOHandlers;
}

void IOHandlerManager::Initialize() {
    //1. Initialize the socket system
    WSADATA wsaData = {0};
    WSAStartup(0, &wsaData); //get the highest version available
    if (WSAStartup(wsaData.wHighVersion, &wsaData) != 0) {
        ASSERT("Unable to call WSAStartup. Error was: %d", WSAGetLastError());
    }

    //2. Compute the number of threads based on the number of processors.
    //TODO: now is hardcoded to 2
    _numberOfThreads = 2;

    //3. Create the IOCP
    _iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, _numberOfThreads);
    if (_iocp == NULL) {
        ASSERT("Unable to create IOCP. Error was: %d", GetLastError());
    }

    //4. assign the tokens pool configuration
    _pAvailableTokens = &_tokensVector1;
    _pRecycledTokens = &_tokensVector2;

    //5. Create the timers manager
    _pTimersManager = new TimersManager(ProcessTimer);
}

void IOHandlerManager::SignalShutdown() {
    _signalShutdown = true;
}

void IOHandlerManager::ShutdownIOHandlers() {

    FOR_MAP(_activeIOHandlers, uint32_t, IOHandler *, i) {
        EnqueueForDelete(MAP_VAL(i));
    }
}

void IOHandlerManager::Shutdown() {
    CloseHandle(_iocp);
    _iocp = NULL;

    for (uint32_t i = 0; i < _tokensVector1.size(); i++)
        delete _tokensVector1[i];
    _tokensVector1.clear();
    _pAvailableTokens = &_tokensVector1;

    for (uint32_t i = 0; i < _tokensVector2.size(); i++)
        delete _tokensVector2[i];
    _tokensVector2.clear();
    _pRecycledTokens = &_tokensVector2;

    delete _pTimersManager;
    _pTimersManager = NULL;

    if (_activeIOHandlers.size() != 0 || _deadIOHandlers.size() != 0) {
        FATAL("Incomplete shutdown!!!");
    }
}

void IOHandlerManager::RegisterIOHandler(IOHandler* pIOHandler) {
    if (MAP_HAS1(_activeIOHandlers, pIOHandler->GetId())) {
        ASSERT("IOHandler already registered");
    }
    SetupToken(pIOHandler);
    uint32_t before = (uint32_t) _activeIOHandlers.size();
    _activeIOHandlers[pIOHandler->GetId()] = pIOHandler;
    DEBUG("Handlers count changed: %d->%d %s", before, before + 1,
            STR(IOHandler::IOHTToString(pIOHandler->GetType())));
}

void IOHandlerManager::UnRegisterIOHandler(IOHandler *pIOHandler) {
    if (MAP_HAS1(_activeIOHandlers, pIOHandler->GetId())) {
        FreeToken(pIOHandler);
        uint32_t before = (uint32_t) _activeIOHandlers.size();
        _activeIOHandlers.erase(pIOHandler->GetId());
        DEBUG("Handlers count changed: %d->%d %s", before, before - 1,
                STR(IOHandler::IOHTToString(pIOHandler->GetType())));
    }
}

bool IOHandlerManager::EnableReadData(IOHandler *pIOHandler) {
    //1. Test the type of IOhandler. It can be removed after debugging though...
    if (pIOHandler->GetType() != IOHT_TCP_CARRIER) {
        ASSERT("This is not an acceptor");
        return false;
    }

    //2. register the fd with the IOCP if necessary
    IOHandlerManagerToken *pToken = pIOHandler->GetIOHandlerManagerToken();
    if (!pToken->registeredIntoIOCP) {
        if (CreateIoCompletionPort((HANDLE) pIOHandler->GetInboundFd(), _iocp, NULL, _numberOfThreads) == NULL) {
            FATAL("Unable to register fd to the iocp. Error was: %d", GetLastError());
            return false;
        }
        pToken->registeredIntoIOCP = true;
    }

    //3. Create the overlapped structure
    ReadWriteOV *pReadWriteOV = ((TCPCarrier *) pIOHandler)->GetReadWriteOV(true);
    if (pReadWriteOV == NULL) {
        FATAL("Unable to create overlapped structure");
        return false;
    }

    //4. Do the ghost read
    BOOL readResult = ReadFile(
            (HANDLE) pIOHandler->GetInboundFd(),
            &pReadWriteOV->dummyBuffer,
            0,
            NULL,
            pReadWriteOV
            );

    if (!readResult) {
        //data might not be immediately available
        uint32_t error = GetLastError();
        if (error != ERROR_IO_PENDING) {
            FATAL("ReadFile failed with error: %d", error);
            return false;
        } else {
            INCREMENT_GENERICOV_USAGE(*pReadWriteOV);
            return true;
        }
    } else {
        //data is available
        INCREMENT_GENERICOV_USAGE(*pReadWriteOV);
        return true; //pIOHandler->OnEvent(pReadWriteOV);
    }
}

bool IOHandlerManager::DisableReadData(IOHandler *pIOHandler, bool ignoreError) {
    NYIA;
    return false;
}

bool IOHandlerManager::EnableWriteData(IOHandler *pIOHandler) {
    //1. Test the type of IOhandler. It can be removed after debugging though...
    if (pIOHandler->GetType() != IOHT_TCP_CARRIER) {
        ASSERT("This is not a TCP carrier");
        return false;
    }

    //2. register the fd with the IOCP if necessary
    IOHandlerManagerToken *pToken = pIOHandler->GetIOHandlerManagerToken();
    if (!pToken->registeredIntoIOCP) {
        if (CreateIoCompletionPort((HANDLE) pIOHandler->GetOutboundFd(), _iocp, NULL, _numberOfThreads) == NULL) {
            FATAL("Unable to register fd to the iocp. Error was: %d", GetLastError());
            return false;
        }
        pToken->registeredIntoIOCP = true;
    }

    //3. Create the overlapped structure
    ReadWriteOV *pReadWriteOV = ((TCPCarrier *) pIOHandler)->GetReadWriteOV(false);
    if (pReadWriteOV == NULL) {
        FATAL("Unable to create overlapped structure");
        return false;
    }

    if (!PostQueuedCompletionStatus(_iocp, 0, NULL, pReadWriteOV)) {
        FATAL("Unable to simulate the write event");
        return false;
    }
    INCREMENT_GENERICOV_USAGE(*pReadWriteOV);
    return true;

    // 	4. Do the ghost write
    // 		BOOL writeResult=WriteFile(
    // 			(HANDLE)pIOHandler->GetFd(),
    // 			&pReadWriteOV->dummyBuffer,
    // 			0,
    // 			NULL,
    // 			pReadWriteOV
    // 			);
    //
    // 		if(!writeResult){
    // 			//the socket might not be immediately available for writing
    // 			uint32_t error=GetLastError();
    // 			if(error!=ERROR_IO_PENDING){
    // 				FATAL("WriteFile failed with error: %d",error);
    // 				return false;
    // 			}else{
    // 				INCREMENT_GENERICOV_USAGE(*pReadWriteOV);
    // 				return true;
    // 			}
    // 		} else {
    // 			//socket is ready for write
    // 			INCREMENT_GENERICOV_USAGE(*pReadWriteOV);
    // 			return pIOHandler->OnEvent(pReadWriteOV);
    // 		}
}

bool IOHandlerManager::DisableWriteData(IOHandler *pIOHandler, bool ignoreError) {
    NYIA;
    return false;
}

bool IOHandlerManager::EnableAcceptConnections(IOHandler *pIOHandler) {
    //1. Test the type of IOhandler. It can be removed after debugging though...
    if (pIOHandler->GetType() != IOHT_ACCEPTOR) {
        ASSERT("This is not an acceptor");
        return false;
    }

    //2. register the fd with the IOCP if necessary
    IOHandlerManagerToken *pToken = pIOHandler->GetIOHandlerManagerToken();
    if (!pToken->registeredIntoIOCP) {
        if (CreateIoCompletionPort((HANDLE) pIOHandler->GetInboundFd(), _iocp, NULL, _numberOfThreads) == NULL) {
            FATAL("Unable to register fd to the iocp. Error was: %d", GetLastError());
            return false;
        }
        pToken->registeredIntoIOCP = true;
    }

    //3. Create the overlapped structure
    AcceptOV *pAcceptOV = ((TCPAcceptor *) pIOHandler)->GetAcceptOV();
    if (pAcceptOV == NULL) {
        FATAL("Unable to create overlapped structure");
        return false;
    }

    //4. Do the accept
    BOOL acceptResult = AcceptEx(
            //A descriptor identifying a socket that has already been
            //called with the listen function. A server application
            //waits for attempts to connect on this socket
            pIOHandler->GetInboundFd(),

            //A descriptor identifying a socket on
            //which to accept an incoming connection.
            //This socket must not be bound or connected.
            pAcceptOV->acceptedSocket,

            //A pointer to a buffer that receives the first block of data
            //sent on a new connection, the local address of the server,
            //and the remote address of the client. The receive data is
            //written to the first part of the buffer starting at offset
            //zero, while the addresses are written to the latter part of
            //the buffer. This parameter must be specified.
            pAcceptOV->rawBuffer,

            //The number of bytes in lpOutputBuffer that will be used
            //for actual receive data at the beginning of the buffer.
            //This size should not include the size of the local address
            //of the server, nor the remote address of the client; they
            //are appended to the output buffer. If dwReceiveDataLength
            //is zero, accepting the connection will not result in a receive
            //operation. Instead, AcceptEx completes as soon as a connection
            //arrives, without waiting for any data.
            0,

            //The number of bytes reserved for the local address information.
            //This value must be at least 16 bytes more than the maximum
            //address length for the transport protocol in use
            sizeof (sockaddr_in) + 16,

            //The number of bytes reserved for the remote address information.
            //This value must be at least 16 bytes more than the maximum
            //address length for the transport protocol in use. Cannot be zero.
            sizeof (sockaddr_in) + 16,

            //A pointer to a DWORD that receives the count of bytes received.
            //This parameter is set only if the operation completes synchronously.
            //If it returns ERROR_IO_PENDING and is completed later, then this
            //DWORD is never set and you must obtain the number of bytes
            //read from the completion notification mechanism.
            &pAcceptOV->rawBufferWritten,

            //An OVERLAPPED structure that is used to process the request.
            //This parameter must be specified; it cannot be NULL.
            pAcceptOV
            );


    if (!acceptResult) {
        uint32_t error = WSAGetLastError();
        if (error != ERROR_IO_PENDING) {
            FATAL("AcceptEx failed with error: %d", error);
            return false;
        } else {
            INCREMENT_GENERICOV_USAGE(*pAcceptOV);
            return true;
        }
    }

    NYIA;
    return false;
}

bool IOHandlerManager::DisableAcceptConnections(IOHandler *pIOHandler, bool ignoreError) {
    NYIA;
    return false;
}

bool IOHandlerManager::EnableTimer(IOHandler *pIOHandler, uint32_t seconds) {
    TimerEvent event = {0};
    event.id = pIOHandler->GetId();
    event.period = seconds;
    event.pUserData = pIOHandler->GetIOHandlerManagerToken();
    _pTimersManager->AddTimer(event);
    return true;
}

bool IOHandlerManager::DisableTimer(IOHandler *pIOHandler, bool ignoreError) {
    _pTimersManager->RemoveTimer(pIOHandler->GetId());
    return true;
}

bool IOHandlerManager::EnableConnectEvent(IOHandler *pIOHandler, ConnectOV *pConnectOV) {
    //1. Test the type of IOhandler. It can be removed after debugging though...
    if (pIOHandler->GetType() != IOHT_TCP_CONNECTOR) {
        ASSERT("This is not a TCP connector");
        return false;
    }

    //2. register the fd with the IOCP if necessary
    IOHandlerManagerToken *pToken = pIOHandler->GetIOHandlerManagerToken();
    if (!pToken->registeredIntoIOCP) {
        if (CreateIoCompletionPort((HANDLE) pIOHandler->GetInboundFd(), _iocp, NULL, _numberOfThreads) == NULL) {
            FATAL("Unable to register fd to the iocp. Error was: %d", GetLastError());
            return false;
        }
        pToken->registeredIntoIOCP = true;
    }

    //3. get our hands on ConnectEx function
    LPFN_CONNECTEX connectEx;
    GUID connectExGUID = WSAID_CONNECTEX;
    uint32_t loadFncreadBytes = 0;
    if (WSAIoctl(pConnectOV->connectSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, &connectExGUID,
            sizeof (GUID), &connectEx, sizeof (LPFN_CONNECTEX), &loadFncreadBytes, NULL, NULL) != 0) {
        FATAL("Unable to get the ConnectEx function pointer. Error was: %d", WSAGetLastError());
        return false;
    }

    //4. Use it now...
    bool connectResult = connectEx(pConnectOV->connectSocket, (sockaddr *) & pConnectOV->address,
            sizeof (sockaddr_in), NULL, 0, &pConnectOV->bytesSent, pConnectOV);

    //5. Test the result
    if (!connectResult) {
        uint32_t error = WSAGetLastError();
        if (error != ERROR_IO_PENDING) {
            FATAL("ConnectEx failed with error: %d", error);
            return false;
        } else {
            INCREMENT_GENERICOV_USAGE(*pConnectOV);
            return true;
        }
    } else {
        //the connection succeeded. Do stuff
        INCREMENT_GENERICOV_USAGE(*pConnectOV);
        return pIOHandler->OnEvent(pConnectOV);
    }
}

void IOHandlerManager::EnqueueForDelete(IOHandler *pIOHandler) {
    if (!MAP_HAS1(_deadIOHandlers, pIOHandler->GetId()))
        _deadIOHandlers[pIOHandler->GetId()] = pIOHandler;
}

void IOHandlerManager::DeleteDeadHandlers() {
    while (_deadIOHandlers.size() > 0) {
        IOHandler *pIOHandler = MAP_VAL(_deadIOHandlers.begin());
        //FINEST("Delete IOH %p", pIOHandler);
        _deadIOHandlers.erase(pIOHandler->GetId());
        delete pIOHandler;
        //FINEST("Done delete IOH %p", pIOHandler);
    }
}

bool IOHandlerManager::Pulse() {
    if (_signalShutdown)
        return false;
    uint32_t numberOfBytesTransferred;
    ULONG_PTR __ignored__;
    OVERLAPPED *pOVERLAPPED = NULL;

    //1. Do the IO
    BOOL result = GetQueuedCompletionStatus(_iocp, &numberOfBytesTransferred, &__ignored__, &pOVERLAPPED, 1000);

    //2. take care of timers
    _pTimersManager->TimeElapsed(time(NULL));

    //3. test to see if we have events
    if (result == 0 && pOVERLAPPED == NULL) {
        return true;
    }

    //4. test to see if we are ok with the overlapped structure
    if (pOVERLAPPED == NULL) {
        ASSERT("WTF!? result!=0 but no overlapped structure!?");
        return false;
    }

    //5. get the GenericOV
    GenericOV *pGenericOV = (GenericOV *) pOVERLAPPED;
    DECREMENT_GENERICOV_USAGE(*pGenericOV);

    //6. Test to see if this has the right ownership or if it is valid
    if (pGenericOV->ownerId != pGenericOV->pToken->ownerId) {
        FINEST("Someone else has this token");
        FreeGenericOV(pGenericOV);
        return true;
    }
    if (!pGenericOV->pToken->validPayload) {
        FINEST("Long dead thingy");
        FreeGenericOV(pGenericOV);
        return true;
    }

    //7. Do the damage
    if (!((IOHandler *) pGenericOV->pToken->pPayload)->OnEvent(pGenericOV)) {
        EnqueueForDelete(((IOHandler *) pGenericOV->pToken->pPayload));
    }

    //8. Done
    return true;
}

void IOHandlerManager::SetupToken(IOHandler *pIOHandler) {
    IOHandlerManagerToken *pResult = NULL;
    if (_pAvailableTokens->size() == 0) {
        pResult = new IOHandlerManagerToken();
        //WARN("Token created: %p", pResult);
    } else {
        pResult = (*_pAvailableTokens)[0];
        //WARN("Token %p served from %p", pResult, _pAvailableTokens);
        _pAvailableTokens->erase(_pAvailableTokens->begin());
    }
    pResult->ownerId = pIOHandler->GetId();
    pResult->pPayload = pIOHandler;
    pResult->validPayload = true;
    pResult->registeredIntoIOCP = false;
    pIOHandler->SetIOHandlerManagerToken(pResult);
}

void IOHandlerManager::FreeToken(IOHandler *pIOHandler) {
    IOHandlerManagerToken *pToken = pIOHandler->GetIOHandlerManagerToken();
    //FATAL("Token %p returned to %p", pToken, _pRecycledTokens);
    pIOHandler->SetIOHandlerManagerToken(NULL);
    pToken->ownerId = 0;
    pToken->pPayload = NULL;
    pToken->validPayload = false;
    pToken->registeredIntoIOCP = false;
    ADD_VECTOR_END((*_pRecycledTokens), pToken);
}

void IOHandlerManager::ProcessTimer(TimerEvent &event) {
    IOHandlerManagerToken *pToken =
            (IOHandlerManagerToken *) event.pUserData;
    if (pToken->validPayload) {
        if (!((IOHandler *) pToken->pPayload)->OnEvent(NULL)) {
            EnqueueForDelete((IOHandler *) pToken->pPayload);
        }
    } else {
        FATAL("Invalid token");
    }
}


#endif /* NET_IOCP2 */


