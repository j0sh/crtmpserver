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

#include "protocols/aes/aesappprotocolhandler.h"
#include "protocols/baseprotocol.h"
#include "protocols/protocolmanager.h"
#include "protocols/genericprotocol.h"
using namespace app_applestreamingclient;

AESAppProtocolHandler::AESAppProtocolHandler(Variant &configuration)
: BaseAppProtocolHandler(configuration) {

}

AESAppProtocolHandler::~AESAppProtocolHandler() {
}

void AESAppProtocolHandler::RegisterProtocol(BaseProtocol *pProtocol) {
	//1. Get the TS protocol ID from the parameters
	uint32_t tsId = pProtocol->GetCustomParameters()["payload"]["tsId"];

	//2. Get the TS protocol
	BaseProtocol *pTSProtocol = ProtocolManager::GetProtocol(tsId);
	if (pTSProtocol == NULL) {
		FATAL("Unable to get TS protocol by id: %u", tsId);
		pProtocol->EnqueueForDelete();
		return;
	}

	//3. Link them
	pProtocol->SetNearProtocol(pTSProtocol);
	pTSProtocol->SetFarProtocol(pProtocol);

	//4. make sure that upper protocols survive AES protocol death
	pProtocol->DeleteNearProtocol(false);

	//5. Do the HTTP request
	if (!((GenericProtocol *) pProtocol)->DoHTTPRequest()) {
		FATAL("Unable to do HTTP request");
		pProtocol->EnqueueForDelete();
		return;
	}
}

void AESAppProtocolHandler::UnRegisterProtocol(BaseProtocol *pProtocol) {
	NYI;
}
