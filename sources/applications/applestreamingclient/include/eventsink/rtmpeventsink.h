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

#ifdef HAS_PROTOCOL_RTMP
#ifndef _RTMPEVENTSINK_H
#define	_RTMPEVENTSINK_H

#include "eventsink/baseeventsink.h"
namespace app_applestreamingclient {

	class RTMPEventSink
	: public BaseEventSink {
	private:
		uint32_t _protocolId;
		string _streamName;
	public:
		RTMPEventSink(uint32_t contextId);
		virtual ~RTMPEventSink();

		void SetProtocolId(uint32_t protocolId);

		virtual bool SignalStreamRegistered(string streamName);
		virtual bool SignalStreamUnRegistered(string streamName);
		virtual bool SignalUpgradeBandwidth(uint32_t oldBw, uint32_t newBw);
		virtual bool SignalDowngradeBandwidth(uint32_t oldBw, uint32_t newBw);
	};
}
#endif	/* _RTMPEVENTSINK_H */
#endif /* HAS_PROTOCOL_RTMP */
