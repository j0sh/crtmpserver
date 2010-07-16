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





#include "controller.h"
#include "netio/netio.h"
#include "protocolfactory.h"
#include "dbaccessprotocol.h"
#include "protocols/protocolfactorymanager.h"
#include "application/clientapplicationmanager.h"

Controller::Controller() {

}

Controller::~Controller() {
}

bool Controller::EnqueueDBRequest(string uri, Variant &payload, string applicationName) {

	vector<uint64_t> protocolStackTypes =
			ProtocolFactoryManager::ResolveProtocolChain("outboundHTTPDBAccess");
	if (protocolStackTypes.size() == 0) {
		FATAL("Unable to resolve protocol stack protocolStackTypes");
		return false;
	}

	string host;
	string ip;
	uint16_t port;
	string document;
	string dummy;

	if (!ParseURL(uri, host, port, dummy, dummy, document)) {
		FATAL("Invalid uri: %s", STR(uri));
		return false;
	}

	ip = GetHostByName(host);
	if (ip == "") {
		return false;
	}

	if (document == "") {
		document = "/";
	}

	Variant parameters;
	parameters["document"] = document;
	parameters["host"] = host;
	parameters["applicationName"] = applicationName;
	parameters["payload"] = payload;

	FINEST("parameters:\n%s", STR(parameters.ToString()));

	if (!TCPConnector<Controller>::Connect(ip, port, protocolStackTypes, parameters)) {
		FATAL("Unable to open connection to origin");
		return false;
	}

	return true;
}

bool Controller::SignalProtocolCreated(BaseProtocol *pProtocol, Variant &parameters) {
	BaseProtocol *pEndpoint = pProtocol->GetNearEndpoint();
	if (pEndpoint->GetType() != PT_DBACCESS) {
		FATAL("This is not waht we've expected");
		return false;
	}

	DBAccessProtocol *pODBA = (DBAccessProtocol *) pEndpoint;

	pODBA->SetApplication(ClientApplicationManager::FindAppByName(parameters["applicationName"]));

	return pODBA->DoRequest(parameters);
}

