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


#ifdef HAS_PROTOCOL_HTTP
#ifdef HAS_PROTOCOL_RTMP
#include "protocols/rtmp/inboundhttp4rtmp.h"
#include "protocols/http/inboundhttpprotocol.h"
#include "netio/netio.h"
#include "protocols/rtmp/inboundrtmpprotocol.h"
#include "protocols/protocolmanager.h"

map<string, string> InboundHTTP4RTMP::_generatedSids;
map<string, uint32_t> InboundHTTP4RTMP::_protocolsBySid;

InboundHTTP4RTMP::InboundHTTP4RTMP()
: BaseProtocol(PT_INBOUND_HTTP_FOR_RTMP) {

}

InboundHTTP4RTMP::~InboundHTTP4RTMP() {
}

bool InboundHTTP4RTMP::Initialize(Variant &parameters) {
	GetCustomParameters()=parameters;
	return true;
}

bool InboundHTTP4RTMP::AllowFarProtocol(uint64_t type) {
	return type == PT_INBOUND_HTTP;
}

bool InboundHTTP4RTMP::AllowNearProtocol(uint64_t type) {
	return (type == PT_INBOUND_RTMP) || (type == PT_RTMPE);
}

IOBuffer * InboundHTTP4RTMP::GetOutputBuffer() {
	if (GETAVAILABLEBYTESCOUNT(_outputBuffer) != 0) {
		return &_outputBuffer;
	}
	return NULL;
}

bool InboundHTTP4RTMP::SignalInputData(int32_t recvAmount) {
	ASSERT("OPERATION NOT SUPPORTED");
	return false;
}

bool InboundHTTP4RTMP::SignalInputData(IOBuffer &buffer) {
	//1. Get the HTTP far protool and test to see if it has ContentLength
	InboundHTTPProtocol *pHTTP = (InboundHTTPProtocol *) _pFarProtocol;
	if (pHTTP == NULL || pHTTP->GetContentLength() == 0) {
		FATAL("Invalid HTTP request");
		return false;
	}

	//2. Test it and see if all the data was transfered
	if (!pHTTP->TransferCompleted()) {
		return true;
	}

	//3. Get the HTTP request
	Variant request = pHTTP->GetHeaders();

	//4. Is this a keep-alive?
	pHTTP->SetDisconnectAfterTransfer(
			request[HTTP_HEADERS][HTTP_HEADERS_CONNECTION]
			!= HTTP_HEADERS_CONNECTION_KEEP_ALIVE);
	DeleteNearProtocol(false);

	//4. Get the URL
	string url = request[HTTP_FIRST_LINE][HTTP_URL];

	//5. split it in meaningful parts
	vector<string> parts;
	split(url, "/", parts);
	if (parts.size() < 2) {
		FATAL("Invalid request:\n%s", STR(request.ToString()));
		return false;
	}

	//7. Do the dammage
	bool result;
	if (parts[1] == "fcs") {
		result = ProcessFcs(parts);
		buffer.Ignore(pHTTP->GetContentLength());
	} else if (parts[1] == "open") {
		result = ProcessOpen(parts);
		buffer.Ignore(pHTTP->GetContentLength());
	} else if (parts[1] == "idle") {
		result = ProcessIdle(parts);
		buffer.Ignore(pHTTP->GetContentLength());
	} else if (parts[1] == "send") {
		if (GETAVAILABLEBYTESCOUNT(buffer) < 1)
			return false;
		_inputBuffer.ReadFromBuffer(GETIBPOINTER(buffer), pHTTP->GetContentLength());
		buffer.Ignore(pHTTP->GetContentLength());
		result = ProcessSend(parts);
	} else {
		FATAL("Invalid command: %s", STR(parts[1]));
		result = false;
	}

	//8. Cleanup
	if (!result) {
		DeleteNearProtocol(true);
		EnqueueForDelete();
	}

	//9. Done
	return result;
}

bool InboundHTTP4RTMP::EnqueueForOutbound() {
	return true;
}

void InboundHTTP4RTMP::ReadyForSend() {
	if (_pNearProtocol != NULL) {
		_pNearProtocol->ReadyForSend();
	}
}

BaseProtocol *InboundHTTP4RTMP::Bind(string sid) {
	BaseProtocol *pResult = NULL;
	if (_pNearProtocol == NULL) {
		//14. This might be a new connection. Do we have that sid generated?
		if (!MAP_HAS1(_generatedSids, sid)) {
			FATAL("Invalid sid: %s", STR(sid));
			return false;
		}

		//15. See if we have to generate a new connection or we just pick up
		//a disconnected one
		if (MAP_HAS1(_protocolsBySid, sid)) {
			pResult = ProtocolManager::GetProtocol(_protocolsBySid[sid]);
		} else {
			pResult = new InboundRTMPProtocol();
			pResult->Initialize(GetCustomParameters());
			pResult->SetApplication(GetApplication());
			_protocolsBySid[sid] = pResult->GetId();
			SetNearProtocol(pResult);
			pResult->SetFarProtocol(this);
		}
	} else {
		pResult = _pNearProtocol;
	}

	return pResult;
}

bool InboundHTTP4RTMP::ProcessFcs(vector<string> &parts) {
	_outputBuffer.ReadFromString(((TCPCarrier *) GetIOHandler())->GetNearEndpointAddressIp() + "\n");
	return BaseProtocol::EnqueueForOutbound();
}

bool InboundHTTP4RTMP::ProcessOpen(vector<string> &parts) {
	if (parts.size() != 3) {
		return false;
	}
	if (parts[2] != "1") {
		return false;
	}

	//2. create a SID, save it, and send it after that
	string sid = md5(format("%u", GetId()), true).substr(0, 16);
	_generatedSids[sid] = sid;
	_outputBuffer.ReadFromString(sid + "\n");

	//3. We are done
	return BaseProtocol::EnqueueForOutbound();
}

bool InboundHTTP4RTMP::ProcessIdle(vector<string> &parts) {

	BaseProtocol *pProtocol = Bind(parts[2]);
	if (pProtocol == NULL) {
		FATAL("Unable to bind protocol");
		return false;
	}

	_outputBuffer.ReadFromByte(1);
	IOBuffer *pBuffer = pProtocol->GetOutputBuffer();
	if (pBuffer != NULL) {
		_outputBuffer.ReadFromBuffer(GETIBPOINTER(*pBuffer), GETAVAILABLEBYTESCOUNT(*pBuffer));
		pBuffer->IgnoreAll();
	}

	return BaseProtocol::EnqueueForOutbound();
}

bool InboundHTTP4RTMP::ProcessSend(vector<string> &parts) {
	BaseProtocol *pProtocol = Bind(parts[2]);
	if (pProtocol == NULL) {
		FATAL("Unable to bind protocol");
		return false;
	}

	if (!pProtocol->SignalInputData(_inputBuffer)) {
		FATAL("Unable to call upper protocol");
		return false;
	}

	_outputBuffer.ReadFromByte(1);
	IOBuffer *pBuffer = pProtocol->GetOutputBuffer();
	if (pBuffer != NULL) {
		_outputBuffer.ReadFromBuffer(GETIBPOINTER(*pBuffer), GETAVAILABLEBYTESCOUNT(*pBuffer));
		pBuffer->IgnoreAll();
	}

	return BaseProtocol::EnqueueForOutbound();
}
#endif /* HAS_PROTOCOL_RTMP */
#endif /* HAS_PROTOCOL_HTTP */


