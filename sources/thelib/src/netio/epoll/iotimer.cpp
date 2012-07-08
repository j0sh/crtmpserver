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
#include "netio/epoll/iotimer.h"
#include "netio/epoll/iohandlermanager.h"
#include "protocols/baseprotocol.h"

#ifdef HAS_EPOLL_TIMERS
#include <sys/timerfd.h>
#else /* HAS_EPOLL_TIMERS */
int32_t IOTimer::_idGenerator;
#endif /* HAS_EPOLL_TIMERS */

IOTimer::IOTimer()
: IOHandler(0, 0, IOHT_TIMER) {
#ifdef HAS_EPOLL_TIMERS
	_count = 0;
	_inboundFd = _outboundFd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
	if (_inboundFd < 0) {
		int err = errno;
		ASSERT("timerfd_create failed with error (%d) %s", err, strerror(err));
	}
#else /* HAS_EPOLL_TIMERS */
	_inboundFd = _outboundFd = ++_idGenerator;
#endif /* HAS_EPOLL_TIMERS */
}

IOTimer::~IOTimer() {
	IOHandlerManager::DisableTimer(this, true);
#ifdef HAS_EPOLL_TIMERS
	CLOSE_SOCKET(_inboundFd);
#endif /* HAS_EPOLL_TIMERS */
}

bool IOTimer::SignalOutputData() {
	ASSERT("Operation not supported");
	return false;
}

bool IOTimer::OnEvent(struct epoll_event &/*ignored*/) {
#ifdef HAS_EPOLL_TIMERS
	if (read(_inboundFd, &_count, 8) != 8) {
		FATAL("Timer failed!");
		return false;
	}
#endif /* HAS_EPOLL_TIMERS */
	if (!_pProtocol->IsEnqueueForDelete()) {
		if (!_pProtocol->TimePeriodElapsed()) {
			FATAL("Unable to handle TimeElapsed event");
			IOHandlerManager::EnqueueForDelete(this);
			return false;
		}
	}
	return true;
}

bool IOTimer::EnqueueForTimeEvent(uint32_t seconds) {
	return IOHandlerManager::EnableTimer(this, seconds);
}

IOTimer::operator string() {
	if (_pProtocol != NULL)
		return STR(*_pProtocol);
	return format("T(%d)", _inboundFd);
}

void IOTimer::GetStats(Variant &info, uint32_t namespaceId) {

}

#endif /* NET_EPOLL */

