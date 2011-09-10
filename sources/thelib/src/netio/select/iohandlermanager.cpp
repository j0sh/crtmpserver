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
#include "netio/select/iohandlermanager.h"
#include "netio/select/iohandler.h"

#define FDSTATE_READ_ENABLED 0x01
#define FDSTATE_WRITE_ENABLED 0x02

map<uint32_t, IOHandler *> IOHandlerManager::_activeIOHandlers;
map<uint32_t, IOHandler *> IOHandlerManager::_deadIOHandlers;
fd_set IOHandlerManager::_readFds;
fd_set IOHandlerManager::_writeFds;
fd_set IOHandlerManager::_readFdsCopy;
fd_set IOHandlerManager::_writeFdsCopy;
map<int32_t, map<uint32_t, uint8_t> > IOHandlerManager::_fdState;
struct timeval IOHandlerManager::_timeout = {1, 0};
TimersManager *IOHandlerManager::_pTimersManager = NULL;
select_event IOHandlerManager::_currentEvent = {0};
bool IOHandlerManager::_isShuttingDown = false;

map<uint32_t, IOHandler *> & IOHandlerManager::GetActiveHandlers() {
	return _activeIOHandlers;
}

map<uint32_t, IOHandler *> & IOHandlerManager::GetDeadHandlers() {
	return _deadIOHandlers;
}

void IOHandlerManager::Initialize() {
	FD_ZERO(&_readFds);
	FD_ZERO(&_writeFds);
	_pTimersManager = new TimersManager(ProcessTimer);
	_isShuttingDown = false;
}

void IOHandlerManager::Start() {
}

void IOHandlerManager::SignalShutdown() {
	_isShuttingDown = true;
}

void IOHandlerManager::ShutdownIOHandlers() {

	FOR_MAP(_activeIOHandlers, uint32_t, IOHandler *, i) {
		EnqueueForDelete(MAP_VAL(i));
	}
}

void IOHandlerManager::Shutdown() {
	_isShuttingDown = false;

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
	size_t before = _activeIOHandlers.size();
	_activeIOHandlers[pIOHandler->GetId()] = pIOHandler;
	DEBUG("Handlers count changed: %"PRIz"u->%"PRIz"u %s", before, before + 1,
			STR(IOHandler::IOHTToString(pIOHandler->GetType())));
}

void IOHandlerManager::UnRegisterIOHandler(IOHandler *pIOHandler) {
	DisableAcceptConnections(pIOHandler);
	DisableReadData(pIOHandler);
	DisableWriteData(pIOHandler);
	DisableTimer(pIOHandler);
	if (MAP_HAS1(_activeIOHandlers, pIOHandler->GetId())) {
		size_t before = _activeIOHandlers.size();
		_activeIOHandlers.erase(pIOHandler->GetId());
		DEBUG("Handlers count changed: %"PRIz"u->%"PRIz"u %s", before, before - 1,
				STR(IOHandler::IOHTToString(pIOHandler->GetType())));
	}
}

bool IOHandlerManager::EnableReadData(IOHandler *pIOHandler) {
	_fdState[pIOHandler->GetInboundFd()][pIOHandler->GetId()] |= FDSTATE_READ_ENABLED;
	return UpdateFdSets(pIOHandler->GetInboundFd());
}

bool IOHandlerManager::DisableReadData(IOHandler *pIOHandler) {
	_fdState[pIOHandler->GetInboundFd()][pIOHandler->GetId()] &= ~FDSTATE_READ_ENABLED;
	return UpdateFdSets(pIOHandler->GetInboundFd());
}

bool IOHandlerManager::EnableWriteData(IOHandler *pIOHandler) {
	_fdState[pIOHandler->GetOutboundFd()][pIOHandler->GetId()] |= FDSTATE_WRITE_ENABLED;
	return UpdateFdSets(pIOHandler->GetOutboundFd());
}

bool IOHandlerManager::DisableWriteData(IOHandler *pIOHandler) {
	_fdState[pIOHandler->GetOutboundFd()][pIOHandler->GetId()] &= ~FDSTATE_WRITE_ENABLED;
	return UpdateFdSets(pIOHandler->GetOutboundFd());
}

bool IOHandlerManager::EnableAcceptConnections(IOHandler *pIOHandler) {
	_fdState[pIOHandler->GetInboundFd()][pIOHandler->GetId()] |= FDSTATE_READ_ENABLED;
	return UpdateFdSets(pIOHandler->GetInboundFd());
}

bool IOHandlerManager::DisableAcceptConnections(IOHandler *pIOHandler) {
	_fdState[pIOHandler->GetInboundFd()][pIOHandler->GetId()] &= ~FDSTATE_READ_ENABLED;
	return UpdateFdSets(pIOHandler->GetInboundFd());
}

bool IOHandlerManager::EnableTimer(IOHandler *pIOHandler, uint32_t seconds) {
	TimerEvent event = {0, 0, 0};
	event.id = pIOHandler->GetId();
	event.period = seconds;
	_pTimersManager->AddTimer(event);
	return true;
}

bool IOHandlerManager::DisableTimer(IOHandler *pIOHandler) {
	_pTimersManager->RemoveTimer(pIOHandler->GetId());
	return true;
}

void IOHandlerManager::EnqueueForDelete(IOHandler *pIOHandler) {
	DisableAcceptConnections(pIOHandler);
	DisableReadData(pIOHandler);
	DisableWriteData(pIOHandler);
	DisableTimer(pIOHandler);
	if (!MAP_HAS1(_deadIOHandlers, pIOHandler->GetId())) {
		_deadIOHandlers[pIOHandler->GetId()] = pIOHandler;
	}
}

uint32_t IOHandlerManager::DeleteDeadHandlers() {
	uint32_t result = 0;
	while (_deadIOHandlers.size() > 0) {
		IOHandler *pIOHandler = MAP_VAL(_deadIOHandlers.begin());
		_deadIOHandlers.erase(pIOHandler->GetId());
		delete pIOHandler;
		result++;
	}
	return result;
}

bool IOHandlerManager::Pulse() {
	if (_isShuttingDown)
		return false;
	//1. Create a copy of all fd sets
	FD_ZERO(&_readFdsCopy);
	FD_ZERO(&_writeFdsCopy);
	FD_ZERO(&_writeFdsCopy);
	FD_COPY(&_readFds, &_readFdsCopy);
	FD_COPY(&_writeFds, &_writeFdsCopy);

	//2. compute the max fd
	if (_activeIOHandlers.size() == 0)
		return true;

	//3. do the select
	RESET_TIMER(_timeout, 1, 0);
	int32_t count = select(MAP_KEY(--_fdState.end()) + 1, &_readFdsCopy, &_writeFdsCopy, NULL, &_timeout);
	if (count < 0) {
		FATAL("Unable to do select: %u", (uint32_t) LASTSOCKETERROR);
		return false;
	}

	_pTimersManager->TimeElapsed(time(NULL));

	if (count == 0) {
		return true;
	}

	//4. Start crunching the sets

	FOR_MAP(_activeIOHandlers, uint32_t, IOHandler *, i) {
		if (FD_ISSET(MAP_VAL(i)->GetInboundFd(), &_readFdsCopy)) {
			_currentEvent.type = SET_READ;
			if (!MAP_VAL(i)->OnEvent(_currentEvent))
				EnqueueForDelete(MAP_VAL(i));
		}
		if (FD_ISSET(MAP_VAL(i)->GetOutboundFd(), &_writeFdsCopy)) {
			_currentEvent.type = SET_WRITE;
			if (!MAP_VAL(i)->OnEvent(_currentEvent))
				EnqueueForDelete(MAP_VAL(i));
		}
	}

	return true;
}

bool IOHandlerManager::UpdateFdSets(int32_t fd) {
	uint8_t state = 0;

	FOR_MAP(_fdState[fd], uint32_t, uint8_t, i) {
		state |= MAP_VAL(i);
	}
	if ((state & FDSTATE_READ_ENABLED) != 0)
		FD_SET(fd, &_readFds);
	else
		FD_CLR(fd, &_readFds);

	if ((state & FDSTATE_WRITE_ENABLED) != 0)
		FD_SET(fd, &_writeFds);
	else
		FD_CLR(fd, &_writeFds);

	if (state == 0)
		_fdState.erase(fd);

	return true;
}

void IOHandlerManager::ProcessTimer(TimerEvent &event) {
	_currentEvent.type = SET_TIMER;
	if (MAP_HAS1(_activeIOHandlers, event.id)) {
		if (!_activeIOHandlers[event.id]->OnEvent(_currentEvent)) {
			EnqueueForDelete(_activeIOHandlers[event.id]);
		}
	}
}

#endif /* NET_SELECT */


