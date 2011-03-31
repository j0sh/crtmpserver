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

#ifdef HAS_MS_TIMER
#include "protocols/timer/finetimer.h"
#include "protocols/protocolfactory.h"
#include "protocols/genericprotocol.h"
#include "clientcontext.h"
using namespace app_applestreamingclient;

FineTimer::FineTimer()
: BaseProtocol(PT_FINE_TIMER) {
	memset(&_address, 0, sizeof (_address));
	_fd = -1;
	_period = 1000000;
}

FineTimer::~FineTimer() {
	pthread_kill(_thread, SIGKILL);
	CLOSE_SOCKET(_fd);
}

bool FineTimer::Initialize(Variant &parameters) {
	GetCustomParameters() = parameters;
	int32_t error = 0;
	inet_aton(STR(parameters[CONF_IP]), &_address.sin_addr);
	_address.sin_family = AF_INET;
	_address.sin_port = EHTONS((uint16_t) parameters[CONF_PORT]);
	_fd = socket(AF_INET, SOCK_DGRAM, 0);
	_period = (double) parameters["FineTimerPeriod"]*1000000.00;
	if ((error = pthread_create(&_thread, NULL, FineTimerWorker, this)) != 0) {
		return -1;
	}
	return true;
}

bool FineTimer::AllowFarProtocol(uint64_t type) {
	return PT_UDP;
}

bool FineTimer::AllowNearProtocol(uint64_t type) {
	return false;
}

bool FineTimer::SignalInputData(int32_t recvAmount) {
	NYIR;
}

bool FineTimer::SignalInputData(IOBuffer &buffer) {
	buffer.IgnoreAll();

	FOR_MAP(_contextIds, uint32_t, uint32_t, i) {
		ClientContext *pContext = ClientContext::GetContext(MAP_VAL(i), 0, 0);
		if (pContext == NULL) {
			WARN("Unable to get context with id %u", MAP_VAL(i));
			continue;
		}
		pContext->ConsumeAVBuffer();
	}

	return true;
}

bool FineTimer::SignalInputData(IOBuffer &buffer, sockaddr_in *pPeerAddress) {
	return SignalInputData(buffer);
}

void FineTimer::RegisterContextId(uint32_t contextId) {
	FATAL("Register context");
	if (!MAP_HAS1(_contextIds, contextId))
		_contextIds[contextId] = contextId;
}

void FineTimer::UnRegisterContextId(uint32_t contextId) {
	FATAL("UnRegister context");
	_contextIds.erase(contextId);
}

void* FineTimer::FineTimerWorker(void *pArg) {
	FineTimer *pTimer = (FineTimer *) pArg;
	char c = 0;
	sockaddr *pAddress = (sockaddr *) & pTimer->_address;
	uint32_t period = pTimer->_period;
	while (true) {
		sendto(pTimer->_fd, &c, 1, 0, pAddress, sizeof (sockaddr));
		usleep(period);
	}
	return NULL;
}
#endif /* HAS_MS_TIMER */
