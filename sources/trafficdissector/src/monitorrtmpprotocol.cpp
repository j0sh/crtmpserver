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

#include "monitorrtmpprotocol.h"
#include "monitorstream.h"
#include "localdefines.h"
#include "application/baseclientapplication.h"
#include "streaming/streamsmanager.h"

MonitorRTMPProtocol::MonitorRTMPProtocol(bool isClient)
: BaseRTMPProtocol(PT_MONITORRTMP) {
	_isClient = isClient;
	_handshakeStage = HSS_UNDEF;
}

MonitorRTMPProtocol::~MonitorRTMPProtocol() {
}

uint8_t MonitorRTMPProtocol::GetLogLevel() {
	if (_isClient)
		return _DEBUG_;
	else
		return _WARNING_;
}

bool MonitorRTMPProtocol::Initialize(Variant &parameters) {
	for (uint32_t i = 0; i < MAX_STREAMS_COUNT; i++) {
		_streams[i] = new MonitorStream(this,
				GetApplication()->GetStreamsManager(), format("stream_%u", i));
	}
	return true;
}

bool MonitorRTMPProtocol::PerformHandshake(IOBuffer &buffer) {
	if (_isClient) {
		return PerformHandshakeClient(buffer);
	} else {
		return PerformHandshakeServer(buffer);
	}
}

bool MonitorRTMPProtocol::SetInboundChunkSize(uint32_t chunkSize) {
	_inboundChunkSize = chunkSize;
	return true;
}

bool MonitorRTMPProtocol::SetOutboundChunkSize(uint32_t chunkSize) {
	_outboundChunkSize = chunkSize;
	return true;
}

bool MonitorRTMPProtocol::PerformHandshakeClient(IOBuffer &buffer) {
	switch (_handshakeStage) {
		case HSS_UNDEF:
		{
			if (GETAVAILABLEBYTESCOUNT(buffer) < 1537) {
				return true;
			}
			if (!buffer.Ignore(1537)) {
				LOG(GetLogLevel(), "Unable to ignore 1737 bytes");
				return false;
			}
			_handshakeStage = HSS_HS;
			return true;
		}
		case HSS_HS:
		{
			if (GETAVAILABLEBYTESCOUNT(buffer) < 1536) {
				return true;
			}
			if (!buffer.Ignore(1536)) {
				LOG(GetLogLevel(), "Unable to ignore 1736 bytes");
				return false;
			}
			_handshakeStage = HSS_DONE;
			_handshakeCompleted = true;
			return true;
		}
		default:
		{
			LOG(GetLogLevel(), "Invalid handshake stage: %u", _handshakeStage);
			return false;
		}
	}
}

bool MonitorRTMPProtocol::PerformHandshakeServer(IOBuffer &buffer) {
	switch (_handshakeStage) {
		case HSS_UNDEF:
		{
			if (GETAVAILABLEBYTESCOUNT(buffer) < 3073) {
				return true;
			}
			if (!buffer.Ignore(3073)) {
				LOG(GetLogLevel(), "Unable to ignore 3073 bytes");
				return false;
			}
			_handshakeStage = HSS_DONE;
			_handshakeCompleted = true;
			return true;
		}
		default:
		{
			LOG(GetLogLevel(), "Invalid handshake stage: %u", _handshakeStage);
			return false;
		}
	}
}
