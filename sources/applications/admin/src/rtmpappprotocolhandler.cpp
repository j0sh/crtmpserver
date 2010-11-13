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
#include "rtmpappprotocolhandler.h"
#include "protocols/rtmp/messagefactories/messagefactories.h"
#include "protocols/rtmp/basertmpprotocol.h"
#include "application/clientapplicationmanager.h"
#include "netio/netio.h"
using namespace app_admin;

RTMPAppProtocolHandler::RTMPAppProtocolHandler(Variant &configuration)
: BaseRTMPAppProtocolHandler(configuration) {

}

RTMPAppProtocolHandler::~RTMPAppProtocolHandler() {
}

bool RTMPAppProtocolHandler::ProcessInvokeConnect(BaseRTMPProtocol *pFrom,
		Variant &request) {
	//1. Get the username and the password
	Variant &username = M_INVOKE_PARAM(request, 1);
	Variant &password = M_INVOKE_PARAM(request, 2);
	if (username != V_STRING || password != V_STRING) {
		FATAL("Invalid connect request:\n%s", STR(request.ToString()));
		return false;
	}

	//2. ***VERY*** basic authentication to get the ball rolling
	if (username != "gigi" || password != "spaima") {
		FATAL("Auth failed");
		return false;
	}

	//3. Auth passes
	return BaseRTMPAppProtocolHandler::ProcessInvokeConnect(pFrom, request);
}

bool RTMPAppProtocolHandler::ProcessInvokeGeneric(BaseRTMPProtocol *pFrom,
		Variant &request) {
	//1. Get the function name
	string functionName = M_INVOKE_FUNCTION(request);
	if (functionName == "ListApplications") {
		return ProcessListApplications(pFrom, request);
	} else if (functionName == "ListServices") {
		return ProcessListServices(pFrom, request);
	} else if (functionName == "StartTests") {
		return ProcessStartTests(pFrom, request);
	} else {
		return BaseRTMPAppProtocolHandler::ProcessInvokeGeneric(pFrom, request);
	}
}

bool RTMPAppProtocolHandler::ProcessListApplications(BaseRTMPProtocol *pFrom,
		Variant &request) {
	//1. Build the list of applications
	Variant applications;
	map<uint32_t, BaseClientApplication *> apps = ClientApplicationManager::GetAllApplications();

	FOR_MAP(apps, uint32_t, BaseClientApplication *, i) {
		applications.PushToArray(MAP_VAL(i)->GetConfiguration());
	}

	//2. Build the response
	Variant parameters;
	parameters.PushToArray(Variant());
	parameters.PushToArray(applications);
	Variant response = GenericMessageFactory::GetInvokeResult(request, parameters);

	//3. Send it back
	return pFrom->SendMessage(response);
}

bool RTMPAppProtocolHandler::ProcessListServices(BaseRTMPProtocol *pFrom,
		Variant &request) {
	//1. Get the list of services
	Variant services;
	map<uint32_t, IOHandler *> &activeHandlers = IOHandlerManager::GetActiveHandlers();

	FOR_MAP(activeHandlers, uint32_t, IOHandler *, i) {

		IOHandler *pIOHandler = MAP_VAL(i);
		switch (pIOHandler->GetType()) {
			case IOHT_ACCEPTOR:
			{
				TCPAcceptor *pAcceptor = (TCPAcceptor *) pIOHandler;
				Variant service;
				service["carrier"] = "TCP";
				service["protocol"] = pAcceptor->GetParameters()[CONF_PROTOCOL];
				service["ip"] = pAcceptor->GetParameters()[CONF_IP];
				service["port"] = pAcceptor->GetParameters()[CONF_PORT];
				service["sslCert"] = pAcceptor->GetParameters()[CONF_SSL_CERT];
				service["sslKey"] = pAcceptor->GetParameters()[CONF_SSL_KEY];
				if (pAcceptor->GetApplication() != NULL) {
					service["application"] = pAcceptor->GetApplication()->GetName();
				} else {
					service["application"] = Variant();
				}
				vector<uint64_t> &protocolChain = pAcceptor->GetProtocolChain();

				FOR_VECTOR(protocolChain, j) {
					service["protocolStack"].PushToArray(tagToString(protocolChain[j]));
				}
				services.PushToArray(service);
				break;
			}
			default:
			{
				continue;
				break;
			}
		}
	}

	//2. Build the response
	Variant parameters;
	parameters.PushToArray(Variant());
	parameters.PushToArray(services);
	Variant response = GenericMessageFactory::GetInvokeResult(request, parameters);

	//3. Send it back
	return pFrom->SendMessage(response);
}

bool RTMPAppProtocolHandler::ProcessStartTests(BaseRTMPProtocol *pFrom,
		Variant &request) {
	Variant complexVar;

	complexVar["V_NULL"] = Variant();
	complexVar["V_UNDEFINED"] = Variant();
	complexVar["V_UNDEFINED"].Reset(true);
	complexVar["V_BOOL1"] = (bool)true;
	complexVar["V_BOOL2"] = (bool)false;
	complexVar["V_INT8"] = (int8_t) - 1;
	complexVar["V_INT16"] = (int16_t) - 2;
	complexVar["V_INT32"] = (int32_t) - 3;
	complexVar["V_INT64"] = (int64_t) - 4;
	complexVar["V_UINT8"] = (uint8_t) 1;
	complexVar["V_UINT16"] = (uint16_t) 2;
	complexVar["V_UINT32"] = (uint32_t) 3;
	complexVar["V_UINT64"] = (uint64_t) 4;
	complexVar["V_DOUBLE"] = 5.6;
	complexVar["V_TIMESTAMP"] = Variant(1979, 10, 31, 15, 16, 17, 0);
	complexVar["V_TIME"] = Variant(15, 16, 17, 0);
	complexVar["V_DATE"] = Variant(1979, 10, 31);
	complexVar["V_STRING"] = "This is a string";
	complexVar["V_MAP"]["key1"] = "key1 content";
	complexVar["V_MAP"]["key2"] = "key2 content";
	complexVar["V_MAP"]["key3"] = "key3 content";
	complexVar["V_MAP_ARRAY"]["key1"] = "array value 1";
	complexVar["V_MAP_ARRAY"]["key2"] = "array value 2";
	complexVar["V_MAP_ARRAY"]["key3"] = "array value 3";
	complexVar["V_MAP_ARRAY"].PushToArray("array value 4");
	complexVar["V_MAP_ARRAY"].PushToArray("array value 5");
	complexVar["V_MAP_ARRAY"].PushToArray("array value 6");
	complexVar["V_MAP_ARRAY"].IsArray(true);
	complexVar["V_BYTEARRAY"] = "Some bytes...";
	complexVar["V_BYTEARRAY"].IsByteArray(true);
	//    complexVar["V_TYPED_MAP"]["member1"] = "string value";
	//    complexVar["V_TYPED_MAP"]["member2"] = 12.4;
	//    complexVar["V_TYPED_MAP"].SetTypeName("TestClass");
	Variant::DeserializeFromXmlFile("/tmp/aaa.xml", complexVar["META"]);

	Variant parameters;
	parameters.PushToArray(Variant());
	parameters.PushToArray(complexVar);
	Variant response = GenericMessageFactory::GetInvokeResult(request, parameters);

	//3. Send it back
	return pFrom->SendMessage(response);
}
#endif /* HAS_PROTOCOL_RTMP */

