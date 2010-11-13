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

#ifndef _SCHEDULETIMERPROTOCOL_H
#define	_SCHEDULETIMERPROTOCOL_H

#include "protocols/timer/basetimerprotocol.h"

namespace app_applestreamingclient {
	class ClientContext;

	class ScheduleTimerProtocol
	: public BaseTimerProtocol {
	private:
		uint32_t _contextId;
		vector<Variant> _queue1;
		vector<Variant> _queue2;
		vector<Variant> *_pInputQueue;
		vector<Variant> *_pRunningQueue;
	public:
		ScheduleTimerProtocol(uint32_t contextId);
		virtual ~ScheduleTimerProtocol();

		virtual bool TimePeriodElapsed();

		void AddJob(Variant &job, bool recurring);
	private:
		bool ProcessJob(ClientContext *pContext, Variant &job);
		bool ProcessJobStartFeeding(ClientContext *pContext, Variant &job);
		bool ProcessJobFetchChildPlaylist(ClientContext *pContext, Variant &job);
		bool ProcessJobConsumeAVBuffer(ClientContext *pContext, Variant &job);
		bool ProcessJobTestJNICallback(ClientContext *pContext, Variant &job);
	};
}

#endif	/* _SCHEDULETIMERPROTOCOL_H */

