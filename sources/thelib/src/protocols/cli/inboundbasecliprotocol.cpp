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

#include "netio/kqueue/iohandlermanager.h"


#ifdef HAS_PROTOCOL_CLI
#include "protocols/cli/inboundbasecliprotocol.h"
#include "application/clientapplicationmanager.h"
#include "protocols/cli/basecliappprotocolhandler.h"

InboundBaseCLIProtocol::InboundBaseCLIProtocol(uint64_t type)
: BaseProtocol(type) {
	_pProtocolHandler = NULL;
}

InboundBaseCLIProtocol::~InboundBaseCLIProtocol() {
}

bool InboundBaseCLIProtocol::Initialize(Variant &parameters) {
	GetCustomParameters() = parameters;
	return true;
}

void InboundBaseCLIProtocol::SetApplication(BaseClientApplication *pApplication) {
	BaseProtocol::SetApplication(pApplication);
	if (pApplication != NULL) {
		_pProtocolHandler = (BaseCLIAppProtocolHandler *)
				pApplication->GetProtocolHandler(this);
	} else {
		_pProtocolHandler = NULL;
	}
}

bool InboundBaseCLIProtocol::AllowFarProtocol(uint64_t type) {
	return (type == PT_TCP) || (type == PT_HTTP_4_CLI);
}

bool InboundBaseCLIProtocol::AllowNearProtocol(uint64_t type) {
	ASSERT("Operation not supported");
	return false;
}

IOBuffer * InboundBaseCLIProtocol::GetOutputBuffer() {
	if (GETAVAILABLEBYTESCOUNT(_outputBuffer) != 0)
		return &_outputBuffer;
	return NULL;
}

bool InboundBaseCLIProtocol::SignalInputData(int32_t recvAmount) {
	ASSERT("Operation not supported");
	return false;
}

bool InboundBaseCLIProtocol::ProcessMessage(Variant &message) {
	if (_pProtocolHandler == NULL) {
		FATAL("No handler available yet");
		return false;
	}
	return _pProtocolHandler->ProcessMessage(this, message);
}

#endif /* HAS_PROTOCOL_CLI */
