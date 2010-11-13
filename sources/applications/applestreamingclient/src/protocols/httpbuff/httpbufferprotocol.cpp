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

#include "protocols/httpbuff/httpbufferprotocol.h"
#include "protocols/protocolfactory.h"
#include "protocols/http/outboundhttpprotocol.h"
#include "clientcontext.h"
#include "protocols/tcpprotocol.h"
using namespace app_applestreamingclient;

HTTPBufferProtocol::HTTPBufferProtocol()
: GenericProtocol(PT_HTTP_BUFF) {
	_lastTimestamp = 0;
	_lastAmount = 0;
	_isEncrypted = false;
}

HTTPBufferProtocol::~HTTPBufferProtocol() {
}

bool HTTPBufferProtocol::AllowFarProtocol(uint64_t type) {
	return type == PT_OUTBOUND_HTTP;
}

bool HTTPBufferProtocol::AllowNearProtocol(uint64_t type) {
	_isEncrypted = (type == PT_INBOUND_AES);
	return true;
}

bool HTTPBufferProtocol::SignalInputData(int32_t recvAmount) {
	NYIR;
}

bool HTTPBufferProtocol::SignalInputData(IOBuffer &buffer) {
	//1. Get the context
	ClientContext *pContext = GetContext();
	if (pContext == NULL) {
		FATAL("Unable to get context");
		return false;
	}

	//2. Compute the speed
	double currentTimestamp = 0;
	GETCLOCKS(currentTimestamp);

	if (_lastTimestamp != 0) {
		double instantTime = currentTimestamp - _lastTimestamp;
		double instantAmount = ((TCPProtocol *) GetFarEndpoint())->GetDecodedBytesCount() - _lastAmount;

		pContext->SignalSpeedDetected(instantAmount, instantTime / CLOCKS_PER_SECOND);
	}

	_lastAmount = ((TCPProtocol *) GetFarEndpoint())->GetDecodedBytesCount();
	GETCLOCKS(_lastTimestamp);

	if (_isEncrypted) {
		if (!GetNearProtocol()->SignalInputData(buffer)) {
			FATAL("Unable to signal upper protocols");
			return false;
		}
	} else {
		ClientContext *pContext = GetContext();
		if (pContext == NULL) {
			FATAL("Unable to get context");
			return false;
		}

		if (!pContext->SignalAVDataAvailable(buffer)) {
			FATAL("Unable to signal ts A/V data available");
			return false;
		}
	}

	//10. Signal the context about this finished chunk
	if (TransferCompleted()) {
		EnqueueForDelete();
		ClientContext *pContext = GetContext();
		if (pContext == NULL) {
			FATAL("Unable to get context");
			return false;
		}

		if (!pContext->SignalTSChunkComplete(GetNearProtocol()->GetId())) {
			FATAL("Unable to signal ts chunk complete");
			return false;
		}
	}

	return true;
}

bool HTTPBufferProtocol::TransferCompleted() {
	return ((OutboundHTTPProtocol *) GetFarProtocol())->TransferCompleted();
}
