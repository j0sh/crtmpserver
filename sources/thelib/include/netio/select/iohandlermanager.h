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


#ifdef NET_SELECT
#ifndef _IOHANDLERMANAGER_H
#define	_IOHANDLERMANAGER_H

#include "new/common.h"

class IOHandler;

class DLLEXP IOHandlerManager {
	static map<uint32_t, IOHandler *> _activeIOHandlers;
	static map<uint32_t, IOHandler *> _deadIOHandlers;
	static fd_set _readFds;
	static fd_set _writeFds;
	static fd_set _readFdsCopy;
	static fd_set _writeFdsCopy;
	static select_event _currentEvent;
	static struct timeval _timeout;
	static TimersManager *_pTimersManager;
	static map<int32_t, map<uint32_t, uint8_t> > _fdState;
	static bool _isShuttingDown;
public:
	static map<uint32_t, IOHandler *> & GetActiveHandlers();
	static map<uint32_t, IOHandler *> & GetDeadHandlers();
	static void Initialize();
	static void SignalShutdown();
	static void ShutdownIOHandlers();
	static void Shutdown();
	static void RegisterIOHandler(IOHandler *pIOHandler);
	static void UnRegisterIOHandler(IOHandler *pIOHandler);
	static bool EnableReadData(IOHandler *pIOHandler);
	static bool DisableReadData(IOHandler *pIOHandler);
	static bool EnableWriteData(IOHandler *pIOHandler);
	static bool DisableWriteData(IOHandler *pIOHandler);
	static bool EnableAcceptConnections(IOHandler *pIOHandler);
	static bool DisableAcceptConnections(IOHandler *pIOHandler);
	static bool EnableTimer(IOHandler *pIOHandler, uint32_t seconds);
	static bool DisableTimer(IOHandler *pIOHandler);
	static bool Pulse();
	static void EnqueueForDelete(IOHandler *pIOHandler);
	static void DeleteDeadHandlers();
private:
	static bool UpdateFdSets(int32_t fd);
	static void ProcessTimer(TimerEvent &event);
};

#endif	/* _IOHANDLERMANAGER_H */
#endif /* NET_SELECT */


