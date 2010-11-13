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


#ifndef _BASEEVENTSINK_H
#define	_BASEEVENTSINK_H

#include "common.h"

#define EVENT_SYNC_RTMP		MAKE_TAG4('R','T','M','P')
#define EVENT_SYNC_VARIANT	MAKE_TAG3('V','A','R')
namespace app_applestreamingclient {
	class ClientContext;

	class BaseEventSink {
	private:
		uint64_t _type;
		uint32_t _contextId;
	public:
		BaseEventSink(uint64_t type, uint32_t contextId);
		virtual ~BaseEventSink();

		uint64_t GetType();

		static BaseEventSink * GetInstance(uint64_t type, uint32_t contextId);

		virtual bool SignalStreamRegistered(string streamName) = 0;
		virtual bool SignalStreamUnRegistered(string streamName) = 0;
		virtual bool SignalUpgradeBandwidth(uint32_t oldBw, uint32_t newBw) = 0;
		virtual bool SignalDowngradeBandwidth(uint32_t oldBw, uint32_t newBw) = 0;
	protected:
		ClientContext *GetContext();
	};
}

#endif	/* _BASEEVENTSINK_H */

