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
#ifndef _FINETIMER_H
#define	_FINETIMER_H

#include "protocols/baseprotocol.h"

namespace app_applestreamingclient {

	class FineTimer
	: public BaseProtocol {
	private:
		map<uint32_t, uint32_t> _contextIds;
		pthread_t _thread;
		sockaddr_in _address;
		int32_t _fd;
		uint32_t _period;
	public:
		FineTimer();
		virtual ~FineTimer();

		virtual bool Initialize(Variant &parameters);
		virtual bool AllowFarProtocol(uint64_t type);
		virtual bool AllowNearProtocol(uint64_t type);
		virtual bool SignalInputData(int32_t recvAmount);
		virtual bool SignalInputData(IOBuffer &buffer);
		virtual bool SignalInputData(IOBuffer &buffer, sockaddr_in *pPeerAddress);

		void RegisterContextId(uint32_t contextId);
		void UnRegisterContextId(uint32_t contextId);
	private:
		static void* FineTimerWorker(void *pArg);
	};
}
#endif	/* _FINETIMER_H */
#endif /* HAS_MS_TIMER */
