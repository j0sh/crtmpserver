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


#include "protocolfactory.h"
#include "protocols/baseprotocol.h"
#include "dbaccessprotocol.h"

ProtocolFactory::ProtocolFactory() {

}

ProtocolFactory::~ProtocolFactory() {
}

vector<uint64_t> ProtocolFactory::HandledProtocols() {
	vector<uint64_t> result;
	ADD_VECTOR_END(result, PT_DBACCESS);
	return result;
}

vector<string> ProtocolFactory::HandledProtocolChains() {
	vector<string> result;
	ADD_VECTOR_END(result, "outboundHTTPDBAccess");
	ADD_VECTOR_END(result, "inboundHTTPDBAccess");
	return result;
}

vector<uint64_t> ProtocolFactory::ResolveProtocolChain(string name) {
	vector<uint64_t> result;
	if (name == "outboundHTTPDBAccess") {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_OUTBOUND_HTTP);
		ADD_VECTOR_END(result, PT_DBACCESS);
	} else if (name == "inboundHTTPDBAccess") {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_INBOUND_HTTP);
		ADD_VECTOR_END(result, PT_DBACCESS);
	} else {
		ASSERT("This protocol stack should not land here");
	}
	return result;
}

BaseProtocol *ProtocolFactory::SpawnProtocol(uint64_t type, Variant &parameters) {
	BaseProtocol *pResult = NULL;
	switch (type) {
		case PT_DBACCESS:
			pResult = new DBAccessProtocol();
			break;
		default:
			FATAL("Spawning protocol %s not yet implemented",
					STR(tagToString(type)));
			break;
	}

	if (pResult != NULL) {
		if (!pResult->Initialize(parameters)) {
			FATAL("Unable to initialize protocol %s",
					STR(tagToString(type)));
			delete pResult;
			pResult = NULL;
		}
	}

	return pResult;
}

