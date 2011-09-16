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
#include "netio/kqueue/iotimer.h"
#include "protocols/baseprotocol.h"
#include "netio/kqueue/iohandlermanager.h"

int32_t IOTimer::_idGenerator;

IOTimer::IOTimer()
: IOHandler(0, 0, IOHT_TIMER) {
	_outboundFd = _inboundFd = ++_idGenerator;
}

IOTimer::~IOTimer() {
	IOHandlerManager::DisableTimer(this, true);
}

bool IOTimer::SignalOutputData() {
	ASSERT("Operation not supported");
	return false;
}

bool IOTimer::OnEvent(struct kevent &event) {
	switch (event.filter) {
		case EVFILT_TIMER:
		{
			if (!_pProtocol->IsEnqueueForDelete()) {
				if (!_pProtocol->TimePeriodElapsed()) {
					FATAL("Unable to handle TimeElapsed event");
					IOHandlerManager::EnqueueForDelete(this);
					return false;
				}
			}
			return true;
		}
		default:
		{
			ASSERT("Invalid state: %hu", event.filter);

			return false;
		}
	}
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

#endif /* NET_KQUEUE */

