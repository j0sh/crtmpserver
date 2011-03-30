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
#include "protocols/rtmp/inboundrtmpsdiscriminatorprotocol.h"
#include "protocols/http/inboundhttpprotocol.h"
#include "protocols/rtmp/inboundhttp4rtmp.h"
#include "protocols/rtmp/inboundrtmpprotocol.h"

InboundRTMPSDiscriminatorProtocol::InboundRTMPSDiscriminatorProtocol()
: BaseProtocol(PT_INBOUND_RTMPS_DISC) {

}

InboundRTMPSDiscriminatorProtocol::~InboundRTMPSDiscriminatorProtocol() {
}

bool InboundRTMPSDiscriminatorProtocol::Initialize(Variant &parameters) {
	GetCustomParameters() = parameters;
	return true;
}

bool InboundRTMPSDiscriminatorProtocol::AllowFarProtocol(uint64_t type) {
	return type == PT_INBOUND_SSL;
}

bool InboundRTMPSDiscriminatorProtocol::AllowNearProtocol(uint64_t type) {
	return false;
}

bool InboundRTMPSDiscriminatorProtocol::SignalInputData(int32_t recvAmount) {
	NYIR;
}

bool InboundRTMPSDiscriminatorProtocol::SignalInputData(IOBuffer &buffer) {
	//1. Do we have enough data?
	if (GETAVAILABLEBYTESCOUNT(buffer) < 4)
		return true;

	//2. Get the first 4 bytes in a string
	string method = string((char *) GETIBPOINTER(buffer), 4);

	//3. Create the proper RTMP stack
	if (method == HTTP_METHOD_POST) {
#ifdef HAS_PROTOCOL_HTTP
		return BindHTTP(buffer);
#else
		FATAL("No HTTP protocol support");
		return false;
#endif /* HAS_PROTOCOL_HTTP */
	} else {
		return BindSSL(buffer);
	}
}

#ifdef HAS_PROTOCOL_HTTP

bool InboundRTMPSDiscriminatorProtocol::BindHTTP(IOBuffer &buffer) {
	//1. Create the HTTP protocol
	BaseProtocol *pHTTP = new InboundHTTPProtocol();
	if (!pHTTP->Initialize(GetCustomParameters())) {
		FATAL("Unable to create HTTP protocol");
		pHTTP->EnqueueForDelete();
		return false;
	}

	//2. Create the HTTP4RTMP protocol
	BaseProtocol *pHTTP4RTMP = new InboundHTTP4RTMP();
	if (!pHTTP4RTMP->Initialize(GetCustomParameters())) {
		FATAL("Unable to create HTTP4RTMP protocol");
		pHTTP->EnqueueForDelete();
		pHTTP4RTMP->EnqueueForDelete();
		return false;
	}

	//3. Destroy the link
	BaseProtocol *pFar = _pFarProtocol;
	pFar->ResetNearProtocol();
	ResetFarProtocol();

	//4. Create the new links
	pFar->SetNearProtocol(pHTTP);
	pHTTP->SetFarProtocol(pFar);
	pHTTP->SetNearProtocol(pHTTP4RTMP);
	pHTTP4RTMP->SetFarProtocol(pHTTP);

	//5. Set the application
	pHTTP4RTMP->SetApplication(GetApplication());

	//6. Enqueue for delete this protocol
	EnqueueForDelete();

	//7. Process the data
	if (!pHTTP->SignalInputData(buffer)) {
		FATAL("Unable to process data");
		pHTTP4RTMP->EnqueueForDelete();
	}

	return true;
}
#endif /* HAS_PROTOCOL_HTTP */

bool InboundRTMPSDiscriminatorProtocol::BindSSL(IOBuffer &buffer) {
	//1. Create the RTMP protocol
	BaseProtocol *pRTMP = new InboundRTMPProtocol();
	if (!pRTMP->Initialize(GetCustomParameters())) {
		FATAL("Unable to create RTMP protocol");
		pRTMP->EnqueueForDelete();
		return false;
	}

	//2. Destroy the link
	BaseProtocol *pFar = _pFarProtocol;
	pFar->ResetNearProtocol();
	ResetFarProtocol();

	//3. Create the new links
	pFar->SetNearProtocol(pRTMP);
	pRTMP->SetFarProtocol(pFar);

	//4. Set the application
	pRTMP->SetApplication(GetApplication());

	//5. Enqueue for delete this protocol
	EnqueueForDelete();

	//6. Process the data
	if (!pRTMP->SignalInputData(buffer)) {
		FATAL("Unable to process data");
		pRTMP->EnqueueForDelete();
	}

	return true;
}

#endif /* HAS_PROTOCOL_RTMP */
