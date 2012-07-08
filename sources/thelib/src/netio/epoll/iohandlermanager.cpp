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
#include "netio/epoll/iohandlermanager.h"
#include "netio/epoll/iohandler.h"
#ifdef HAS_EPOLL_TIMERS
#include <sys/timerfd.h>
#endif /* HAS_EPOLL_TIMERS */

int32_t IOHandlerManager::_eq = 0;
map<uint32_t, IOHandler *> IOHandlerManager::_activeIOHandlers;
map<uint32_t, IOHandler *> IOHandlerManager::_deadIOHandlers;
struct epoll_event IOHandlerManager::_query[EPOLL_QUERY_SIZE];
vector<IOHandlerManagerToken *> IOHandlerManager::_tokensVector1;
vector<IOHandlerManagerToken *> IOHandlerManager::_tokensVector2;
vector<IOHandlerManagerToken *> *IOHandlerManager::_pAvailableTokens;
vector<IOHandlerManagerToken *> *IOHandlerManager::_pRecycledTokens;
#ifndef HAS_EPOLL_TIMERS
TimersManager *IOHandlerManager::_pTimersManager = NULL;
#endif /* HAS_EPOLL_TIMERS */

FdStats IOHandlerManager::_fdStats;
struct epoll_event IOHandlerManager::_dummy = {0,
	{0}};

map<uint32_t, IOHandler *> & IOHandlerManager::GetActiveHandlers() {
	return _activeIOHandlers;
}

map<uint32_t, IOHandler *> & IOHandlerManager::GetDeadHandlers() {
	return _deadIOHandlers;
}

FdStats &IOHandlerManager::GetStats(bool updateSpeeds) {
#ifdef GLOBALLY_ACCOUNT_BYTES
	if (updateSpeeds)
		_fdStats.UpdateSpeeds();
#endif /* GLOBALLY_ACCOUNT_BYTES */
	return _fdStats;
}

void IOHandlerManager::Initialize() {
	_fdStats.Reset();
	_eq = 0;
	_pAvailableTokens = &_tokensVector1;
	_pRecycledTokens = &_tokensVector2;
#ifndef HAS_EPOLL_TIMERS
	_pTimersManager = new TimersManager(ProcessTimer);
#endif /* HAS_EPOLL_TIMERS */
	memset(&_dummy, 0, sizeof (_dummy));
}

void IOHandlerManager::Start() {
	_eq = epoll_create(EPOLL_QUERY_SIZE);
	o_assert(_eq > 0);
}

void IOHandlerManager::SignalShutdown() {
	close(_eq);
}

void IOHandlerManager::ShutdownIOHandlers() {

	FOR_MAP(_activeIOHandlers, uint32_t, IOHandler *, i) {
		EnqueueForDelete(MAP_VAL(i));
	}

}

void IOHandlerManager::Shutdown() {
	close(_eq);

	for (uint32_t i = 0; i < _tokensVector1.size(); i++)
		delete _tokensVector1[i];
	_tokensVector1.clear();
	_pAvailableTokens = &_tokensVector1;

	for (uint32_t i = 0; i < _tokensVector2.size(); i++)
		delete _tokensVector2[i];
	_tokensVector2.clear();
	_pRecycledTokens = &_tokensVector2;
#ifndef HAS_EPOLL_TIMERS
	delete _pTimersManager;
	_pTimersManager = NULL;
#endif /* HAS_EPOLL_TIMERS */

	if (_activeIOHandlers.size() != 0 || _deadIOHandlers.size() != 0) {
		FATAL("Incomplete shutdown!");
	}

}

void IOHandlerManager::RegisterIOHandler(IOHandler* pIOHandler) {
	if (MAP_HAS1(_activeIOHandlers, pIOHandler->GetId())) {
		ASSERT("IOHandler already registered");
	}
	SetupToken(pIOHandler);
	size_t before = _activeIOHandlers.size();
	_activeIOHandlers[pIOHandler->GetId()] = pIOHandler;
	_fdStats.RegisterManaged(pIOHandler->GetType());
	DEBUG("Handlers count changed: %"PRIz"u->%"PRIz"u %s", before, before + 1,
			STR(IOHandler::IOHTToString(pIOHandler->GetType())));
}

void IOHandlerManager::UnRegisterIOHandler(IOHandler *pIOHandler) {
	if (MAP_HAS1(_activeIOHandlers, pIOHandler->GetId())) {
		_fdStats.UnRegisterManaged(pIOHandler->GetType());
		FreeToken(pIOHandler);
		size_t before = _activeIOHandlers.size();
		_activeIOHandlers.erase(pIOHandler->GetId());
		DEBUG("Handlers count changed: %"PRIz"u->%"PRIz"u %s", before, before - 1,
				STR(IOHandler::IOHTToString(pIOHandler->GetType())));
	}
}

int IOHandlerManager::CreateRawUDPSocket() {
	int result = socket(AF_INET, SOCK_DGRAM, 0);
	if (result >= 0) {
		_fdStats.RegisterRawUdp();
	} else {
		int err = errno;
		FATAL("Unable to create raw udp socket. Error code was: (%d) %s",
				err, strerror(err));
	}
	return result;
}

void IOHandlerManager::CloseRawUDPSocket(int socket) {
	if (socket > 0) {
		_fdStats.UnRegisterRawUdp();
	}
	CLOSE_SOCKET(socket);
}

#ifdef GLOBALLY_ACCOUNT_BYTES

void IOHandlerManager::AddInBytesManaged(IOHandlerType type, uint64_t bytes) {
	_fdStats.AddInBytesManaged(type, bytes);
}

void IOHandlerManager::AddOutBytesManaged(IOHandlerType type, uint64_t bytes) {
	_fdStats.AddOutBytesManaged(type, bytes);
}

void IOHandlerManager::AddInBytesRawUdp(uint64_t bytes) {
	_fdStats.AddInBytesRawUdp(bytes);
}

void IOHandlerManager::AddOutBytesRawUdp(uint64_t bytes) {
	_fdStats.AddOutBytesRawUdp(bytes);
}
#endif /* GLOBALLY_ACCOUNT_BYTES */

bool IOHandlerManager::EnableReadData(IOHandler *pIOHandler) {
	struct epoll_event evt = {0,
		{0}};
	evt.events = EPOLLIN;
	evt.data.ptr = pIOHandler->GetIOHandlerManagerToken();
	if (epoll_ctl(_eq, EPOLL_CTL_ADD, pIOHandler->GetInboundFd(), &evt) != 0) {
		int err = errno;
		FATAL("Unable to enable read data: (%d) %s", err, strerror(err));
		return false;
	}
	return true;
}

bool IOHandlerManager::DisableReadData(IOHandler *pIOHandler, bool ignoreError) {
	struct epoll_event evt = {0,
		{0}};
	evt.events = EPOLLIN;
	evt.data.ptr = pIOHandler->GetIOHandlerManagerToken();
	if (epoll_ctl(_eq, EPOLL_CTL_DEL, pIOHandler->GetInboundFd(), &evt) != 0) {
		if (!ignoreError) {
			int err = errno;
			FATAL("Unable to disable read data: (%d) %s", err, strerror(err));
			return false;
		}
	}
	return true;
}

bool IOHandlerManager::EnableWriteData(IOHandler *pIOHandler) {
	struct epoll_event evt = {0,
		{0}};
	evt.events = EPOLLIN | EPOLLOUT;
	evt.data.ptr = pIOHandler->GetIOHandlerManagerToken();

	//TODO: Solve this ugly hack
	int32_t operation;
	if (pIOHandler->GetType() == IOHT_TCP_CONNECTOR)
		operation = EPOLL_CTL_ADD;
	else
		operation = EPOLL_CTL_MOD;

	if (epoll_ctl(_eq, operation, pIOHandler->GetOutboundFd(), &evt) != 0) {
		int err = errno;
		FATAL("Unable to enable read data: (%d) %s", err, strerror(err));
		return false;
	}

	return true;
}

bool IOHandlerManager::DisableWriteData(IOHandler *pIOHandler, bool ignoreError) {
	struct epoll_event evt = {0,
		{0}};
	evt.events = EPOLLIN;
	evt.data.ptr = pIOHandler->GetIOHandlerManagerToken();
	if (epoll_ctl(_eq, EPOLL_CTL_MOD, pIOHandler->GetOutboundFd(), &evt) != 0) {
		if (!ignoreError) {
			int err = errno;
			FATAL("Unable to disable write data: (%d) %s", err, strerror(err));
			return false;
		}
	}
	return true;
}

bool IOHandlerManager::EnableAcceptConnections(IOHandler *pIOHandler) {
	struct epoll_event evt = {0,
		{0}};
	evt.events = EPOLLIN;
	evt.data.ptr = pIOHandler->GetIOHandlerManagerToken();
	if (epoll_ctl(_eq, EPOLL_CTL_ADD, pIOHandler->GetInboundFd(), &evt) != 0) {
		int err = errno;
		if (err == EEXIST)
			return true;
		FATAL("Unable to enable accept connections: (%d) %s", err, strerror(err));
		return false;
	}
	return true;
}

bool IOHandlerManager::DisableAcceptConnections(IOHandler *pIOHandler, bool ignoreError) {
	struct epoll_event evt = {0,
		{0}};
	evt.events = EPOLLIN;
	evt.data.ptr = pIOHandler->GetIOHandlerManagerToken();
	if (epoll_ctl(_eq, EPOLL_CTL_DEL, pIOHandler->GetInboundFd(), &evt) != 0) {
		if (!ignoreError) {
			int err = errno;
			FATAL("Unable to disable accept connections: (%d) %s", err, strerror(err));
			return false;
		}
	}
	return true;
}

bool IOHandlerManager::EnableTimer(IOHandler *pIOHandler, uint32_t seconds) {
#ifdef HAS_EPOLL_TIMERS
	itimerspec tmp;
	itimerspec dummy;
	memset(&tmp, 0, sizeof (tmp));
	tmp.it_interval.tv_nsec = 0;
	tmp.it_interval.tv_sec = seconds;
	tmp.it_value.tv_nsec = 0;
	tmp.it_value.tv_sec = seconds;
	if (timerfd_settime(pIOHandler->GetInboundFd(), 0, &tmp, &dummy) != 0) {
		int err = errno;
		FATAL("timerfd_settime failed with error (%d) %s", err, strerror(err));
		return false;
	}
	struct epoll_event evt = {0,
		{0}};
	evt.events = EPOLLIN;
	evt.data.ptr = pIOHandler->GetIOHandlerManagerToken();
	if (epoll_ctl(_eq, EPOLL_CTL_ADD, pIOHandler->GetInboundFd(), &evt) != 0) {
		int err = errno;
		FATAL("Unable to enable read data: (%d) %s", err, strerror(err));
		return false;
	}
	return true;
#else /* HAS_EPOLL_TIMERS */
	TimerEvent event = {0, 0, 0};
	event.id = pIOHandler->GetId();
	event.period = seconds;
	event.pUserData = pIOHandler->GetIOHandlerManagerToken();
	_pTimersManager->AddTimer(event);
	return true;
#endif /* HAS_EPOLL_TIMERS */
}

bool IOHandlerManager::DisableTimer(IOHandler *pIOHandler, bool ignoreError) {
#ifdef HAS_EPOLL_TIMERS
	itimerspec tmp;
	itimerspec dummy;
	memset(&tmp, 0, sizeof (tmp));
	timerfd_settime(pIOHandler->GetInboundFd(), 0, &tmp, &dummy);
	struct epoll_event evt = {0,
		{0}};
	evt.events = EPOLLIN;
	evt.data.ptr = pIOHandler->GetIOHandlerManagerToken();
	if (epoll_ctl(_eq, EPOLL_CTL_DEL, pIOHandler->GetInboundFd(), &evt) != 0) {
		if (!ignoreError) {
			int err = errno;
			FATAL("Unable to disable read data: (%d) %s", err, strerror(err));
			return false;
		}
	}
	return true;
#else /* HAS_EPOLL_TIMERS */
	_pTimersManager->RemoveTimer(pIOHandler->GetId());
	return true;
#endif /* HAS_EPOLL_TIMERS */
}

void IOHandlerManager::EnqueueForDelete(IOHandler *pIOHandler) {
	DisableWriteData(pIOHandler, true);
	DisableAcceptConnections(pIOHandler, true);
	DisableReadData(pIOHandler, true);
	DisableTimer(pIOHandler, true);
	if (!MAP_HAS1(_deadIOHandlers, pIOHandler->GetId()))
		_deadIOHandlers[pIOHandler->GetId()] = pIOHandler;
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
	int32_t eventsCount = 0;
#ifdef HAS_EPOLL_TIMERS
	if ((eventsCount = epoll_wait(_eq, _query, EPOLL_QUERY_SIZE, -1)) < 0) {
		int err = errno;
		if (err == EINTR)
			return true;
		FATAL("Unable to execute epoll_wait: (%d) %s", err, strerror(err));
		return false;
	}
#else /* HAS_EPOLL_TIMERS */
	if ((eventsCount = epoll_wait(_eq, _query, EPOLL_QUERY_SIZE, 1000)) < 0) {
		int err = errno;
		if (err == EINTR)
			return true;
		FATAL("Unable to execute epoll_wait: (%d) %s", err, strerror(err));
		return false;
	}

	_pTimersManager->TimeElapsed(time(NULL));
#endif /* HAS_EPOLL_TIMERS */

	for (int32_t i = 0; i < eventsCount; i++) {
		//1. Get the token
		IOHandlerManagerToken *pToken =
				(IOHandlerManagerToken *) _query[i].data.ptr;

		//2. Test the fd
		if ((_query[i].events & EPOLLERR) != 0) {
			if (pToken->validPayload) {
				if ((_query[i].events & EPOLLHUP) != 0) {
					DEBUG("***Event handler HUP: %p", (IOHandler *) pToken->pPayload);
					((IOHandler *) pToken->pPayload)->OnEvent(_query[i]);
				} else {
					DEBUG("***Event handler ERR: %p", (IOHandler *) pToken->pPayload);
				}
				IOHandlerManager::EnqueueForDelete((IOHandler *) pToken->pPayload);
			}
			continue;
		}

		//3. Do the damage
		if (pToken->validPayload) {
			if (!((IOHandler *) pToken->pPayload)->OnEvent(_query[i])) {
				EnqueueForDelete((IOHandler *) pToken->pPayload);
			}
		} else {
			FATAL("Invalid token");
		}
	}

	if (_tokensVector1.size() > _tokensVector2.size()) {
		_pAvailableTokens = &_tokensVector1;
		_pRecycledTokens = &_tokensVector2;
	} else {
		_pAvailableTokens = &_tokensVector2;
		_pRecycledTokens = &_tokensVector1;
	}

	return true;
}

void IOHandlerManager::SetupToken(IOHandler *pIOHandler) {
	IOHandlerManagerToken *pResult = NULL;
	if (_pAvailableTokens->size() == 0) {
		pResult = new IOHandlerManagerToken();
	} else {
		pResult = (*_pAvailableTokens)[0];
		_pAvailableTokens->erase(_pAvailableTokens->begin());
	}
	pResult->pPayload = pIOHandler;
	pResult->validPayload = true;
	pIOHandler->SetIOHandlerManagerToken(pResult);
}

void IOHandlerManager::FreeToken(IOHandler *pIOHandler) {
	IOHandlerManagerToken *pToken = pIOHandler->GetIOHandlerManagerToken();
	pIOHandler->SetIOHandlerManagerToken(NULL);
	pToken->pPayload = NULL;
	pToken->validPayload = false;
	ADD_VECTOR_END((*_pRecycledTokens), pToken);
}

#ifndef HAS_EPOLL_TIMERS

void IOHandlerManager::ProcessTimer(TimerEvent &event) {
	IOHandlerManagerToken *pToken =
			(IOHandlerManagerToken *) event.pUserData;
	_dummy.data.ptr = &event;
	if (pToken->validPayload) {
		if (!((IOHandler *) pToken->pPayload)->OnEvent(_dummy)) {
			EnqueueForDelete((IOHandler *) pToken->pPayload);
		}
	} else {
		FATAL("Invalid token");
	}
}
#endif /* HAS_EPOLL_TIMERS */
#endif /* NET_EPOLL */


