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

#include "protocols/ts/tsappprotocolhandler.h"
#include "protocols/baseprotocol.h"
#include "protocols/http/outboundhttpprotocol.h"
#include "clientcontext.h"
#include "protocols/ts/inboundtsprotocol.h"
using namespace app_applestreamingclient;

TSAppProtocolHandler::TSAppProtocolHandler(Variant &configuration)
: BaseTSAppProtocolHandler(configuration) {

}

TSAppProtocolHandler::~TSAppProtocolHandler() {

}

void TSAppProtocolHandler::RegisterProtocol(BaseProtocol *pProtocol) {
	//1. Run the default behaviour
	BaseTSAppProtocolHandler::RegisterProtocol(pProtocol);

	//2. Get the context
	uint32_t contextId = pProtocol->GetCustomParameters()["payload"]["contextId"];
	ClientContext *pContext = ClientContext::GetContext(contextId, 0, 0);
	if (pContext == NULL) {
		FATAL("Unable to get the context with id: %u", contextId);
		pProtocol->EnqueueForDelete();
		return;
	}

	//3. Mark this protocol as a survivor upont chain tear-down
	pProtocol->GetFarProtocol()->DeleteNearProtocol(false);

	//4. This is a step-by-step feed process
	((InboundTSProtocol *) pProtocol)->SetStepByStep(true);

	//5. Tell the context about this new TS protocol
	uint32_t bw = pProtocol->GetCustomParameters()["payload"]["bw"];
	if (!pContext->SignalTSProtocolAvailable(pProtocol->GetId(), bw)) {
		FATAL("Unable to signal the context about new TS protocol");
		pProtocol->EnqueueForDelete();
		return;
	}

	//6. Do the HTTP request
	if (!DoHTTPRequest(pProtocol)) {
		FATAL("Unable to do the HTTP request");
		pProtocol->EnqueueForDelete();
	}

	//7. Done
	FINEST("%s", STR(*pProtocol));
}

bool TSAppProtocolHandler::DoHTTPRequest(BaseProtocol *pProtocol) {
	//1. Get the paramaters
	Variant &parameters = pProtocol->GetCustomParameters();

	//2. Get the http protocol
	OutboundHTTPProtocol *pHTTP = NULL;
	BaseProtocol *pTemp = pProtocol;
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
