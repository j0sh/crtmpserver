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

BaseMMSAppProtocolHandler::BaseMMSAppProtocolHandler(Variant &configuration)
: BaseAppProtocolHandler(configuration) {

}

BaseMMSAppProtocolHandler::~BaseMMSAppProtocolHandler() {
}

void BaseMMSAppProtocolHandler::RegisterProtocol(BaseProtocol *pProtocol) {
	NYI;
}

void BaseMMSAppProtocolHandler::UnRegisterProtocol(BaseProtocol *pProtocol) {
	NYI;
}

bool BaseMMSAppProtocolHandler::PullExternalStream(URI uri, Variant streamConfig) {
//	vector<uint64_t> chain = ProtocolFactoryManager::ResolveProtocolChain(
//			CONF_PROTOCOL_OUTBOUND_MMS);
//	if (chain.size() == 0) {
//		FATAL("Unable to resolve protocol chain");
//		return false;
//	}
//
//	//2. Save the app id inside the custom parameters and mark this connection
//	//as client connection
//	Variant customParameters = streamConfig;
//	customParameters["customParameters"]["externalStreamConfig"] = streamConfig;
//	customParameters["isClient"] = (bool)true;
//	customParameters["appId"] = GetApplication()->GetId();
//	customParameters["uri"] = uri.ToVariant();
//
//	//3. Connect
//	if (!TCPConnector<BaseRTSPAppProtocolHandler>::Connect(
//			uri.ip,
//			uri.port,
//			chain, customParameters)) {
//		FATAL("Unable to connect to %s:%hu",
//				STR(customParameters["uri"]["ip"]),
//				(uint16_t) customParameters["uri"]["port"]);
//		return false;
//	}
//
//	//4. Done
//	return true;
	return false;
}

#endif /* HAS_PROTOCOL_MMS */
