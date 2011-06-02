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
#include "localdefines.h"
#include "echoprotocol.h"
#include "httpdownloadprotocol.h"
using namespace app_samplefactory;

ProtocolFactory::ProtocolFactory() {

}

ProtocolFactory::~ProtocolFactory() {
}

vector<uint64_t> ProtocolFactory::HandledProtocols() {
	vector<uint64_t> result;
	ADD_VECTOR_END(result, PT_ECHO_PROTOCOL);
	ADD_VECTOR_END(result, PT_HTTP_DOWNLOAD_PROTOCOL);
	return result;
}

vector<string> ProtocolFactory::HandledProtocolChains() {
	vector<string> result;
	ADD_VECTOR_END(result, "echoProtocol");
	ADD_VECTOR_END(result, "httpEchoProtocol");
	ADD_VECTOR_END(result, "httpDownload");
	return result;
}

vector<uint64_t> ProtocolFactory::ResolveProtocolChain(string name) {
	vector<uint64_t> result;
	if (name == "echoProtocol") {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_ECHO_PROTOCOL);
	} else if (name == "httpEchoProtocol") {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_INBOUND_HTTP);
		ADD_VECTOR_END(result, PT_ECHO_PROTOCOL);
	} else if (name == "httpDownload") {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_OUTBOUND_HTTP);
		ADD_VECTOR_END(result, PT_HTTP_DOWNLOAD_PROTOCOL);
	} else {
		ASSERT("This protocol stack should not land here");
	}
	return result;
}

BaseProtocol *ProtocolFactory::SpawnProtocol(uint64_t type, Variant &parameters) {
	BaseProtocol *pResult = NULL;
	switch (type) {
		case PT_ECHO_PROTOCOL:
			pResult = new EchoProtocol();
			break;
		case PT_HTTP_DOWNLOAD_PROTOCOL:
			pResult = new HTTPDownloadProtocol();
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

