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
#include "rtmpappprotocolhandler.h"
#include "application/baseclientapplication.h"
#include "protocols/rtmp/outboundrtmpprotocol.h"
#include "protocols/rtmp/messagefactories/connectionmessagefactory.h"
#include "protocols/rtmp/messagefactories/streammessagefactory.h"
using namespace app_stresstest;

RTMPAppProtocolHandler::RTMPAppProtocolHandler(Variant &configuration)
: BaseRTMPAppProtocolHandler(configuration) {
	_activeConnections = 0;
}

RTMPAppProtocolHandler::~RTMPAppProtocolHandler() {
}

void RTMPAppProtocolHandler::RegisterProtocol(BaseProtocol *pProtocol) {
	BaseRTMPAppProtocolHandler::RegisterProtocol(pProtocol);
	_activeConnections++;
	SpawnConnections();
}

void RTMPAppProtocolHandler::UnRegisterProtocol(BaseProtocol *pProtocol) {
	BaseRTMPAppProtocolHandler::UnRegisterProtocol(pProtocol);
	_activeConnections--;
	SpawnConnections();
}

void RTMPAppProtocolHandler::SpawnConnections() {
	string targetServer = _configuration["targetServer"];
	string targetApp = _configuration["targetApp"];
	uint32_t numberOfConnections = (uint32_t) _configuration["numberOfConnections"];
	if (_activeConnections >= numberOfConnections)
		return;
	bool randomAccessStreams = (bool)_configuration["randomAccessStreams"];
	string streamName = GetStreamName(_activeConnections, randomAccessStreams);

	string fullUri = format("rtmp://%s/%s/%s",
			STR(targetServer),
			STR(targetApp),
			STR(streamName));

	URI uri;
	if (!URI::FromString(fullUri, true, uri)) {
		FATAL("Unable to parse uri: %s", STR(fullUri));
		return;
	}

	Variant streamConfig;
	streamConfig["uri"] = uri;
	streamConfig["localStreamName"] = generateRandomString(8);

	if (!PullExternalStream(uri, streamConfig)) {
		FATAL("Unable to pull external stream %s", STR(fullUri));
		return;
	}
}

string RTMPAppProtocolHandler::GetStreamName(uint32_t index, bool randomAccessStreams) {
	if (randomAccessStreams) {
		return _configuration["streams"][rand() % _configuration["streams"].MapSize()];
	} else {
		return _configuration["streams"][index % _configuration["streams"].MapSize()];
	}
}
#endif /* HAS_PROTOCOL_RTMP */

