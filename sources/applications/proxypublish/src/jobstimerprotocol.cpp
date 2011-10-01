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

#include "jobstimerprotocol.h"
#include "proxypublishapplication.h"
#include "streaming/streamsmanager.h"
#include "streaming/streamstypes.h"
#include "streaming/basestream.h"

using namespace app_proxypublish;

JobsTimerProtocol::JobsTimerProtocol() {
}

JobsTimerProtocol::~JobsTimerProtocol() {
}

bool JobsTimerProtocol::TimePeriodElapsed() {
	DoPulls();
	DoPushes();
	_pullQueue.clear();
	_pushQueue.clear();
	return true;
}

void JobsTimerProtocol::EnqueuePush(Variant &push) {
	ADD_VECTOR_END(_pushQueue, push);
}

void JobsTimerProtocol::EnqueuePull(Variant &pull) {
	ADD_VECTOR_END(_pullQueue, pull);
}

void JobsTimerProtocol::DoPulls() {
	for (uint32_t i = 0; i < _pullQueue.size(); i++) {
		Variant &parameters = _pullQueue[i];
		GetApplication()->PullExternalStream(parameters);
	}
}

void JobsTimerProtocol::DoPushes() {
	for (uint32_t i = 0; i < _pushQueue.size(); i++) {
		Variant &parameters = _pushQueue[i];
		GetApplication()->PushLocalStream(parameters);
	}
}
