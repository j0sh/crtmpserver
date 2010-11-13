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

#include "protocols/genericprotocol.h"
#include "protocols/http/outboundhttpprotocol.h"
#include "clientcontext.h"
using namespace app_applestreamingclient;

GenericProtocol::GenericProtocol(uint64_t type)
: BaseProtocol(type) {
	_contextId = 0;
}

GenericProtocol::~GenericProtocol() {
}

bool GenericProtocol::Initialize(Variant &parameters) {
	GetCustomParameters() = parameters;
	_contextId = parameters["contextId"];
	if (_contextId == 0) {
		FATAL("Invalid context id");
		return false;
	}
	return true;
}

ClientContext *GenericProtocol::GetContext() {
	return ClientContext::GetContext(_contextId, 0, 0);
}

bool GenericProtocol::DoHTTPRequest() {
	//1. Get the paramaters
	Variant &parameters = GetCustomParameters();

	//2. Get the http protocol
	OutboundHTTPProtocol *pHTTP = NULL;
	BaseProtocol *pTemp = this;
	while (pTemp != NULL) {
		if (pTemp->GetType() == PT_OUTBOUND_HTTP) {
			pHTTP = (OutboundHTTPProtocol *) pTemp;
			break;
		}
		pTemp = pTemp->GetFarProtocol();
	}
	if (pHTTP == NULL) {
		FATAL("This is not a HTTP based protocol chain");
		return false;
	}

	//3. We wish to disconnect after the transfer is complete
	pHTTP->SetDisconnectAfterTransfer(true);

	//4. This is a GET request
	pHTTP->Method(HTTP_METHOD_GET);

	//5. Our document and the host
	pHTTP->Document(parameters["document"]);
	pHTTP->Host(parameters["host"]);

	//6. Done
	return pHTTP->EnqueueForOutbound();
}
