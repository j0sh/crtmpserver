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


#ifdef HAS_PROTOCOL_VAR
#include "protocols/variant/basevariantprotocol.h"
#include "protocols/variant/basevariantappprotocolhandler.h"
#include "protocols/http/outboundhttpprotocol.h"
#include "application/baseclientapplication.h"
#include "protocols/protocolmanager.h"

BaseVariantProtocol::BaseVariantProtocol(uint64_t type)
: BaseProtocol(type) {
	_pProtocolHandler = NULL;
}

BaseVariantProtocol::~BaseVariantProtocol() {
}

bool BaseVariantProtocol::Initialize(Variant &parameters) {
	return true;
}

void BaseVariantProtocol::SetApplication(BaseClientApplication *pApplication) {
	if (pApplication == NULL)
		return;
	BaseProtocol::SetApplication(pApplication);
	_pProtocolHandler = (BaseVariantAppProtocolHandler *)
			_pApplication->GetProtocolHandler(this);
}

IOBuffer * BaseVariantProtocol::GetOutputBuffer() {
	if (GETAVAILABLEBYTESCOUNT(_outputBuffer) == 0)
		return NULL;
	return &_outputBuffer;
}

bool BaseVariantProtocol::AllowFarProtocol(uint64_t type) {
	return type == PT_TCP
			|| type == PT_OUTBOUND_HTTP
			|| type == PT_INBOUND_HTTP;
}

bool BaseVariantProtocol::AllowNearProtocol(uint64_t type) {
	ASSERT("This is an endpoint protocol");
	return false;
}

bool BaseVariantProtocol::SignalInputData(int32_t recvAmount) {
	ASSERT("OPERATION NOT SUPPORTED");
	return false;
}

bool BaseVariantProtocol::SignalInputData(IOBuffer &buffer) {
	if (_pProtocolHandler == NULL) {
		FATAL("This protocol is not registered to any application yet");
		return false;
	}

	if (_pFarProtocol->GetType() == PT_OUTBOUND_HTTP
			|| _pFarProtocol->GetType() == PT_INBOUND_HTTP) {
#ifdef HAS_PROTOCOL_HTTP
		//1. This is a HTTP based transfer. We only start doing stuff
		//after a complete request is made.
		if (!((BaseHTTPProtocol *) _pFarProtocol)->TransferCompleted())
			return true;

		string rawData = string((char *) GETIBPOINTER(buffer), GETAVAILABLEBYTESCOUNT(buffer));
		buffer.IgnoreAll();
		if (!Deserialize(rawData, _lastReceived)) {
			FATAL("Unable to deserialize content");
			return false;
		}
#else
		FATAL("HTTP protocol not supported");
		return false;
#endif /* HAS_PROTOCOL_HTTP */
	} else {
		NYIR;
	}

	_lastReceived.Compact();

	return _pProtocolHandler->ProcessMessage(this, _lastSent, _lastReceived);
}

bool BaseVariantProtocol::Send(Variant &variant) {
	//1. Do we have a protocol?
	if (_pFarProtocol == NULL) {
		FATAL("This protocol is not linked");
		return false;
	}

	//2. Save the variant
	_lastSent = variant;

	//3. Depending on the far protocol, we do different stuff
	string rawContent = "";
	switch (_pFarProtocol->GetType()) {
		case PT_TCP:
		{
			//5. Serialize it
			if (!Serialize(rawContent, variant)) {
				FATAL("Unable to serialize variant");
				return false;
			}

			_outputBuffer.ReadFromString(rawContent);

			//6. enqueue for outbound
			return EnqueueForOutbound();
		}
		case PT_OUTBOUND_HTTP:
		{
#ifdef HAS_PROTOCOL_HTTP
			//7. This is a HTTP request. So, first things first: get the http protocol
			OutboundHTTPProtocol *pHTTP = (OutboundHTTPProtocol *) _pFarProtocol;

			//8. We wish to disconnect after the transfer is complete
			pHTTP->SetDisconnectAfterTransfer(true);

			//9. This will always be a POST
			pHTTP->Method(HTTP_METHOD_POST);

			//10. Our document and the host
			pHTTP->Document(variant["document"]);
			pHTTP->Host(variant["host"]);

			//11. Serialize it
			if (!Serialize(rawContent, variant["payload"])) {
				FATAL("Unable to serialize variant");
				return false;
			}

			_outputBuffer.ReadFromString(rawContent);

			//12. enqueue for outbound
			return EnqueueForOutbound();
#else
			FATAL("HTTP protocol not supported");
			return false;
#endif /* HAS_PROTOCOL_HTTP */
		}
		case PT_INBOUND_HTTP:
		{
#ifdef HAS_PROTOCOL_HTTP
			if (!Serialize(rawContent, variant)) {
				FATAL("Unable to serialize variant");
				return false;
			}

			_outputBuffer.ReadFromString(rawContent);

			return EnqueueForOutbound();
#else
			FATAL("HTTP protocol not supported");
			return false;
#endif /* HAS_PROTOCOL_HTTP */
		}
		default:
		{
			ASSERT("We should not be here");
			return false;
		}
	}
}
#endif	/* HAS_PROTOCOL_VAR */

