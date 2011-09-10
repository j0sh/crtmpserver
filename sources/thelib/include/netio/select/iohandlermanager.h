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

#include "common.h"

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

	/*!
		@brief Initializes the handler manager.
	 */
	static void Initialize();

	static void Start();

	/*!
		@brief Sets the shutdown signal to true.
	 */
	static void SignalShutdown();

	/*!
		@brief Enqueues all active handlers for delete
	 */
	static void ShutdownIOHandlers();

	/*!
		@brief Shuts down the IO handlers by deleting the TimersManager
	 */
	static void Shutdown();

	/*!
		@brief Registers the IO handler. It is considered active once successfully registered.
		@param pIOHandler: Pointer to the IO handler to be registered
		@discussion If the IO handler being registered is already active, this function will do nothing but display a log message about it.
	 */
	static void RegisterIOHandler(IOHandler *pIOHandler);

	/*!
		@brief Erases the IO handler.
		@param pIOHandler: Pointer to the IO handler to be erased.
		@discussion: When this function runs, it disables this IO handler's timer and prohibits read/write data and connection acceptance.
	 */
	static void UnRegisterIOHandler(IOHandler *pIOHandler);

	/*!
		@brief Enables the IO handler to read data. 
		@discussion This assigns a FDSTATE_READ_ENABLED to the inbound file descriptor's state
	 */
	static bool EnableReadData(IOHandler *pIOHandler);

	/*!
		@brief Prohibits the IO handler to read data
	 */
	static bool DisableReadData(IOHandler *pIOHandler);

	/*!
		@brief Enables the IO handler to write data. 
		@discussion This assigns a FDSTATE_WRITE_ENABLED to the inbound file descriptor's state
	 */
	static bool EnableWriteData(IOHandler *pIOHandler);

	/*!
		@brief  Prohibits the IO handler to write data
	 */
	static bool DisableWriteData(IOHandler *pIOHandler);

	/*!
		@brief Enables the IO handler to accept connections
		@discussion This assigns a FDSTATE_READ_ENABLED to the outbound file descriptor's state
	 */
	static bool EnableAcceptConnections(IOHandler *pIOHandler);

	/*!
		@brief  Prohibits the IO handler to accept connections
	 */
	static bool DisableAcceptConnections(IOHandler *pIOHandler);

	/*!
		@brief  Adds a timer event to the TimersManager.
		@param pIOHandler: The function uses this IO handler's id to set the valu of the event's id.
		@param seconds: The timer's period is set to seconds
	 */
	static bool EnableTimer(IOHandler *pIOHandler, uint32_t seconds);

	/*!
		@brief  Removes the timer to the TimersManager
	 */
	static bool DisableTimer(IOHandler *pIOHandler);
	static bool Pulse();

	/*!
		@brief  Enqueues the IO handler for delete by treating it as a dead handler
	 */
	static void EnqueueForDelete(IOHandler *pIOHandler);

	/*!
		@brief  Erases all dead IO handlers
	 */
	static uint32_t DeleteDeadHandlers();
private:
	static bool UpdateFdSets(int32_t fd);
	static void ProcessTimer(TimerEvent &event);
};

#endif	/* _IOHANDLERMANAGER_H */
#endif /* NET_SELECT */


