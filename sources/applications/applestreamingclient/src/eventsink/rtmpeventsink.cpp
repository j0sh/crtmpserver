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
#include "eventsink/rtmpeventsink.h"
#include "protocols/rtmp/basertmpprotocol.h"
#include "protocols/protocolmanager.h"
#include "protocols/rtmp/messagefactories/messagefactories.h"
using namespace app_applestreamingclient;

RTMPEventSink::RTMPEventSink(uint32_t contextId)
: BaseEventSink(EVENT_SYNC_RTMP, contextId) {
	_protocolId = 0;
	_streamName = "";
}

RTMPEventSink::~RTMPEventSink() {
}

void RTMPEventSink::SetProtocolId(uint32_t protocolId) {
	_protocolId = protocolId;
}

bool RTMPEventSink::SignalStreamRegistered(string streamName) {
	//1. Save the stream name
	if (_streamName == streamName)
		return true;
	else
		_streamName = streamName;


	//2. Get the RTMP protocol
	BaseRTMPProtocol *pProtocol = (BaseRTMPProtocol *) ProtocolManager::GetProtocol(_protocolId);
	if (pProtocol == NULL) {
		FATAL("Unable to get the RTMP protocol");
		return false;
	}

	//3. Prepare the invoke
	Variant parameters;
	parameters.PushToArray(Variant());
	parameters.PushToArray(streamName);
	Variant request = GenericMessageFactory::GetInvoke(3, 0, 0, false, 0,
			"streamAvailable", parameters);

	//4. Send it
	if (!pProtocol->SendMessage(request)) {
		FATAL("Unable to send RTMP message");
		pProtocol->EnqueueForDelete();
		return false;
	}

	//5. Done
	return true;
}

bool RTMPEventSink::SignalStreamUnRegistered(string streamName) {
	_streamName = "";
	return true;
}

bool RTMPEventSink::SignalUpgradeBandwidth(uint32_t oldBw, uint32_t newBw) {
	return true;
}

bool RTMPEventSink::SignalDowngradeBandwidth(uint32_t oldBw, uint32_t newBw) {
	return true;
}

#endif /* HAS_PROTOCOL_RTMP */
