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

#ifdef NET_EPOLL
#ifndef _IOHANDLERMANAGER_H
#define	_IOHANDLERMANAGER_H

#include "common.h"
#include "netio/epoll/iohandlermanagertoken.h"
#include "netio/fdstats.h"

class IOHandler;

#define EPOLL_QUERY_SIZE 1024

class IOHandlerManager {
private:
	static int32_t _eq;
	static map<uint32_t, IOHandler *> _activeIOHandlers;
	static map<uint32_t, IOHandler *> _deadIOHandlers;
	static struct epoll_event _query[EPOLL_QUERY_SIZE];
	static vector<IOHandlerManagerToken *> _tokensVector1;
	static vector<IOHandlerManagerToken *> _tokensVector2;
	static vector<IOHandlerManagerToken *> *_pAvailableTokens;
	static vector<IOHandlerManagerToken *> *_pRecycledTokens;
#ifndef HAS_EPOLL_TIMERS
	static TimersManager *_pTimersManager;
#endif /* HAS_EPOLL_TIMERS */
	static struct epoll_event _dummy;
	static FdStats _fdStats;
public:
	static map<uint32_t, IOHandler *> & GetActiveHandlers();
	static map<uint32_t, IOHandler *> & GetDeadHandlers();
	static FdStats &GetStats(bool updateSpeeds);
	static void Initialize();
	static void Start();
	static void SignalShutdown();
	static void ShutdownIOHandlers();
	static void Shutdown();
	static void RegisterIOHandler(IOHandler *pIOHandler);
	static void UnRegisterIOHandler(IOHandler *pIOHandler);
	static int CreateRawUDPSocket();
	static void CloseRawUDPSocket(int socket);
#ifdef GLOBALLY_ACCOUNT_BYTES
	static void AddInBytesManaged(IOHandlerType type, uint64_t bytes);
	static void AddOutBytesManaged(IOHandlerType type, uint64_t bytes);
	static void AddInBytesRawUdp(uint64_t bytes);
	static void AddOutBytesRawUdp(uint64_t bytes);
#endif /* GLOBALLY_ACCOUNT_BYTES */
	static bool EnableReadData(IOHandler *pIOHandler);
	static bool DisableReadData(IOHandler *pIOHandler, bool ignoreError = false);
	static bool EnableWriteData(IOHandler *pIOHandler);
	static bool DisableWriteData(IOHandler *pIOHandler, bool ignoreError = false);
	static bool EnableAcceptConnections(IOHandler *pIOHandler);
	static bool DisableAcceptConnections(IOHandler *pIOHandler, bool ignoreError = false);
	static bool EnableTimer(IOHandler *pIOHandler, uint32_t seconds);
	static bool DisableTimer(IOHandler *pIOHandler, bool ignoreError = false);
	static void EnqueueForDelete(IOHandler *pIOHandler);
	static uint32_t DeleteDeadHandlers();
	static bool Pulse();
private:
	static void SetupToken(IOHandler *pIOHandler);
	static void FreeToken(IOHandler *pIOHandler);
#ifndef HAS_EPOLL_TIMERS
	static void ProcessTimer(TimerEvent &event);
#endif /* HAS_EPOLL_TIMERS */
};


#endif	/* _IOHANDLERMANAGER_H */
#endif /* NET_EPOLL */


