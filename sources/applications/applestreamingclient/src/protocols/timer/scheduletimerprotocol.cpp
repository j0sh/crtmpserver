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

#include "protocols/timer/scheduletimerprotocol.h"
#include "clientcontext.h"
#include "applestreamingclient.h"
#include "applestreamingclientapplication.h"
#include "jnihelpers.h"
using namespace app_applestreamingclient;

ScheduleTimerProtocol::ScheduleTimerProtocol(uint32_t contextId) {
	_contextId = contextId;
	_pInputQueue = &_queue1;
	_pRunningQueue = &_queue2;
}

ScheduleTimerProtocol::~ScheduleTimerProtocol() {
}

bool ScheduleTimerProtocol::TimePeriodElapsed() {
	//1. First, switch te queues
	vector<Variant> *pTemp = _pInputQueue;
	_pInputQueue = _pRunningQueue;
	_pRunningQueue = pTemp;

	//2. Get the context
	ClientContext *pContext = ClientContext::GetContext(_contextId, 0, 0);
	if (pContext == NULL) {
		FATAL("Unable to get context with id %u", _contextId);
		return false;
	}

	//3. Process all the jobs in the queue
	for (uint32_t i = 0; i < _pRunningQueue->size(); i++) {
		if (!ProcessJob(pContext, (*_pRunningQueue)[i])) {
			FATAL("Unable to process job\n%s", STR((*_pRunningQueue)[i].ToString()));
			return false;
		}
		if ((bool)(*_pRunningQueue)[i]["recurring"]) {
			ADD_VECTOR_END((*_pInputQueue), (*_pRunningQueue)[i]);
		}
	}

	//4. Clear the running queue
	_pRunningQueue->clear();

	//5. Done
	return true;
}

void ScheduleTimerProtocol::AddJob(Variant &job, bool recurring) {
	job["recurring"] = (bool)recurring;
	ADD_VECTOR_END((*_pInputQueue), job);
}

bool ScheduleTimerProtocol::ProcessJob(ClientContext *pContext, Variant &job) {
	if (job["type"] == "startFeeding") {
		return ProcessJobStartFeeding(pContext, job);
	} else if (job["type"] == "fetchChildPlaylist") {
		return ProcessJobFetchChildPlaylist(pContext, job);
	} else if (job["type"] == "consumeAVBuffer") {
		return ProcessJobConsumeAVBuffer(pContext, job);
	} else if (job["type"] == "testJNICallback") {
		return ProcessJobTestJNICallback(pContext, job);
	} else {
		ASSERT("Invalid job:\n%s", STR(job.ToString()));
		return false;
	}
}

bool ScheduleTimerProtocol::ProcessJobStartFeeding(ClientContext *pContext,
		Variant &job) {
	return pContext->StartFeeding();
}

bool ScheduleTimerProtocol::ProcessJobFetchChildPlaylist(ClientContext *pContext, Variant &job) {
	return pContext->FetchChildPlaylist(job["uri"], (uint32_t) job["bw"]);
}

bool ScheduleTimerProtocol::ProcessJobConsumeAVBuffer(ClientContext *pContext, Variant &job) {
	return pContext->ConsumeAVBuffer();
}

bool ScheduleTimerProtocol::ProcessJobTestJNICallback(ClientContext *pContext, Variant &job) {
#ifdef ANDROID
	AppleStreamingClientApplication *pApp = pContext->GetApplication();
	if (pApp == NULL) {
		FATAL("Unable to get the application");
		return false;
	}
	CallBackInfo &ci = pApp->GetJavaCallBackInterface();

	Variant parameters;
	parameters["gigi"] = "spaima";
	parameters["fane"]["brici"] = 123.456;
	return CallJava(ci, parameters);
#else
	NYIR;
#endif
}
