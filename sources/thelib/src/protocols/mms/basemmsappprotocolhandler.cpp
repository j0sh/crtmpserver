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

#ifdef HAS_PROTOCOL_MMS
#include "protocols/mms/basemmsappprotocolhandler.h"
#include "protocols/protocolfactorymanager.h"
#include "netio/netio.h"
#include "application/baseclientapplication.h"
#include "application/clientapplicationmanager.h"
#include "protocols/mms/mmsprotocol.h"

BaseMMSAppProtocolHandler::BaseMMSAppProtocolHandler(Variant &configuration)
: BaseAppProtocolHandler(configuration) {

}

BaseMMSAppProtocolHandler::~BaseMMSAppProtocolHandler() {
}

void BaseMMSAppProtocolHandler::RegisterProtocol(BaseProtocol *pProtocol) {
	if (!((MMSProtocol *) pProtocol)->Start()) {
		FATAL("Unable to start MMS protocol");
		pProtocol->EnqueueForDelete();
	}
}

void BaseMMSAppProtocolHandler::UnRegisterProtocol(BaseProtocol *pProtocol) {

}

bool BaseMMSAppProtocolHandler::PullExternalStream(URI uri, Variant streamConfig) {
	//1. Get the chain
	vector<uint64_t> chain = ProtocolFactoryManager::ResolveProtocolChain(
			CONF_PROTOCOL_OUTBOUND_MMS);
	if (chain.size() == 0) {
		FATAL("Unable to resolve protocol chain");
		return false;
	}

	//2. Save the external stream parameters
	Variant customParameters;
	customParameters["customParameters"]["externalStreamConfig"] = streamConfig;
	customParameters["appId"] = GetApplication()->GetId();

	//3. Connect
	if (!TCPConnector<BaseMMSAppProtocolHandler>::Connect(uri.ip, uri.port,
			chain, customParameters)) {
		FATAL("Unable to connect to %s:%hu",
				STR(customParameters["uri"]["ip"]),
				(uint16_t) customParameters["uri"]["port"]);
		return false;
	}

	//4. Done
	return true;
}

bool BaseMMSAppProtocolHandler::SignalProtocolCreated(BaseProtocol *pProtocol,
		Variant &parameters) {
	//1. Sanitize
	if (parameters["appId"] != V_UINT32) {
		FATAL("Invalid custom parameters:\n%s", STR(parameters.ToString()));
		return false;
	}

	//2. Get the application
	BaseClientApplication *pApplication = ClientApplicationManager::FindAppById(
			parameters["appId"]);

	if (pProtocol == NULL) {
		FATAL("Connection failed:\n%s", STR(parameters.ToString()));
		return pApplication->OutboundConnectionFailed(parameters);
	}

	//3. Set it up inside the protocol
	pProtocol->GetNearEndpoint()->SetApplication(pApplication);

	//4. Done
	return true;
}

#endif /* HAS_PROTOCOL_MMS */
