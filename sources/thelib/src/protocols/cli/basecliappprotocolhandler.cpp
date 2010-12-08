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

#ifdef HAS_PROTOCOL_CLI
#include "protocols/cli/basecliappprotocolhandler.h"
#include "protocols/baseprotocol.h"
#include "protocols/cli/inboundbasecliprotocol.h"
#include "netio/netio.h"
#include "application/clientapplicationmanager.h"
#include "protocols/protocolmanager.h"

BaseCLIAppProtocolHandler::BaseCLIAppProtocolHandler(Variant &configuration)
: BaseAppProtocolHandler(configuration) {

}

BaseCLIAppProtocolHandler::~BaseCLIAppProtocolHandler() {
}

void BaseCLIAppProtocolHandler::RegisterProtocol(BaseProtocol *pProtocol) {

}

void BaseCLIAppProtocolHandler::UnRegisterProtocol(BaseProtocol *pProtocol) {

}

bool BaseCLIAppProtocolHandler::ProcessMessage(BaseProtocol *pFrom,
		Variant &message) {
	string command = lowercase(message["command"]);
	if (command == "help") {
		return ProcessMessageHelp(pFrom, message["parameters"]);
	} else if (command == "quit") {
		return ProcessMessageQuit(pFrom, message["parameters"]);
	} else if (command == "shutdown") {
		return ProcessMessageShutdown(pFrom, message["parameters"]);
	} else if (command == "listapps") {
		return ProcessMessageListApps(pFrom, message["parameters"]);
	} else if (command == "listservices") {
		return ProcessMessageListServices(pFrom, message["parameters"]);
	} else if (command == "listconnections") {
		return ProcessMessageListConnections(pFrom, message["parameters"]);
	} else if (command == "shutdownapp") {
		return ProcessMessageShutdownApp(pFrom, message["parameters"]);
	} else if (command == "shutdownservice") {
		return ProcessMessageShutdownService(pFrom, message["parameters"]);
	} else {
		return SendFail(pFrom,
				format("command %s not known. Type help for list of commands",
				STR(command)));
	}
}

bool BaseCLIAppProtocolHandler::ProcessMessageHelp(BaseProtocol *pFrom,
		Variant &parameters) {
	Variant data;

	data.PushToArray("help                             prints this help");
	data.PushToArray("quit                             quit CLI");
	data.PushToArray("shutdown                         terminate the server process");
	data.PushToArray("listApps                         returns a list of loaded applications");
	data.PushToArray("listServices                     returns a list of available services");
	data.PushToArray("listConnections [type=<proto>]   returns the list of active connection");
	data.PushToArray("shutdownapp id=<id>              terminates an application");
	data.PushToArray("shutdownservice id=<id>          terminates a service");

	return SendSuccess(pFrom, "Available commands", data);
}

bool BaseCLIAppProtocolHandler::ProcessMessageQuit(BaseProtocol *pFrom,
		Variant &parameters) {
	Variant dummy;
	if (!SendSuccess(pFrom, "Bye!", dummy)) {
		FATAL("Unable to send message");
		return false;
	}
	pFrom->GracefullyEnqueueForDelete();
	return true;
}

bool BaseCLIAppProtocolHandler::ProcessMessageShutdown(BaseProtocol *pFrom,
		Variant &parameters) {
	IOHandlerManager::SignalShutdown();
	return true;
}

bool BaseCLIAppProtocolHandler::ProcessMessageListApps(BaseProtocol *pFrom,
		Variant &parameters) {
	Variant data;

	map<uint32_t, BaseClientApplication *> apps = ClientApplicationManager::GetAllApplications();

	FOR_MAP(apps, uint32_t, BaseClientApplication *, i) {
		Variant item;
		item["id"] = (uint32_t) MAP_KEY(i);
		item["name"] = MAP_VAL(i)->GetName();
		data.PushToArray(item);
	}

	return SendSuccess(pFrom, "Loaded applications", data);
}

bool BaseCLIAppProtocolHandler::ProcessMessageListServices(BaseProtocol *pFrom,
		Variant &parameters) {
	map<uint32_t, IOHandler *> handlers = IOHandlerManager::GetActiveHandlers();

	Variant data;

	FOR_MAP(handlers, uint32_t, IOHandler *, i) {
		Variant item;

		IOHandler *pHandler = MAP_VAL(i);
		if ((pHandler->GetType() != IOHT_ACCEPTOR)
				&& (pHandler->GetType() != IOHT_UDP_CARRIER))
			continue;
		switch (pHandler->GetType()) {
			case IOHT_ACCEPTOR:
			{
				TCPAcceptor *pAcceptor = (TCPAcceptor *) pHandler;
				Variant params = pAcceptor->GetParameters();
				item["id"] = pAcceptor->GetId();
				item["ip"] = params[CONF_IP];
				item["port"] = params[CONF_PORT];
				item["proto"] = params[CONF_PROTOCOL];
				item["appOwner"] = pAcceptor->GetApplication() != NULL ?
						pAcceptor->GetApplication()->GetName() : "#none#";
				if (params[CONF_SSL_CERT] == V_STRING) {
					item["sslCert"] = params[CONF_SSL_CERT];
				}
				if (params[CONF_SSL_KEY] == V_STRING) {
					item["sslKey"] = params[CONF_SSL_KEY];
				}
				data.PushToArray(item);
				break;
			}
			default:
			{
				break;
			}
		}
	}
	return SendSuccess(pFrom, "Available services", data);
}

bool BaseCLIAppProtocolHandler::ProcessMessageListConnections(BaseProtocol *pFrom,
		Variant &parameters) {

	uint64_t protoType = 0;

	if (parameters == V_MAP) {
		//1. Get type
		if (parameters.HasKey("type")) {
			if (parameters["type"] != V_STRING) {
				return SendFail(pFrom, "Syntax error. Type help for details");
			}
			string typeString = parameters["type"];
			if ((typeString.length() == 0)
					|| (typeString.length() > 8)) {
				return SendFail(pFrom, "Syntax error. Type help for details");
			}
			for (uint32_t i = 0; i < typeString.length(); i++) {
				protoType = protoType | (((uint64_t) typeString[i]) << ((7 - i)*8));
			}
		}
	}

	//2. Get all protocols
	map<uint32_t, BaseProtocol *> allProtocols = ProtocolManager::GetActiveProtocols();

	//3. Filter them
	map<uint32_t, BaseProtocol *> protocols;

	FOR_MAP(allProtocols, uint32_t, BaseProtocol *, i) {
		if (protoType != 0) {
			if (MAP_VAL(i)->GetType() == protoType) {
				protocols[MAP_VAL(i)->GetId()] = MAP_VAL(i);
			}
		} else {
			protocols[MAP_VAL(i)->GetNearEndpoint()->GetId()] = MAP_VAL(i)->GetNearEndpoint();
		}
	}

	Variant data;

	//4. Get the stats for each connection

	FOR_MAP(protocols, uint32_t, BaseProtocol *, i) {
		Variant item;
		MAP_VAL(i)->GetStackStats(item);
		if (item != V_NULL)
			data.PushToArray(item);
	}

	return SendSuccess(pFrom, "Connections", data);
}

bool BaseCLIAppProtocolHandler::ProcessMessageShutdownApp(BaseProtocol *pFrom,
		Variant &parameters) {
	if (parameters["id"] != V_STRING) {
		return SendFail(pFrom, "Syntax error. Type help for details");
	}

	uint32_t id = atoi(STR(parameters["id"]));
	BaseClientApplication *pApp = ClientApplicationManager::FindAppById(id);
	if (pApp == NULL) {
		return SendFail(pFrom, format("Application %s not found", STR(parameters["id"])));
	}

	BaseClientApplication::Shutdown(pApp);

	Variant dummy;
	return SendSuccess(pFrom, format("Application %s removed", STR(parameters["id"])), dummy);
}

bool BaseCLIAppProtocolHandler::ProcessMessageShutdownService(BaseProtocol *pFrom,
		Variant &parameters) {
	if (parameters["id"] != V_STRING) {
		return SendFail(pFrom, "Syntax error. Type help for details");
	}

	map<uint32_t, IOHandler *> services = IOHandlerManager::GetActiveHandlers();

	uint32_t id = atoi(STR(parameters["id"]));

	if (!MAP_HAS1(services, id)) {
		return SendFail(pFrom, format("Service %s not found", STR(parameters["id"])));
	}
	if (services[id]->GetType() != IOHT_ACCEPTOR) {
		return SendFail(pFrom, format("Service %s not found", STR(parameters["id"])));
	}

	IOHandlerManager::EnqueueForDelete(services[id]);

	Variant dummy;
	return SendSuccess(pFrom, format("Service %s removed", STR(parameters["id"])), dummy);
}

bool BaseCLIAppProtocolHandler::Send(BaseProtocol *pTo, string status, string description, Variant &data) {
	//1. Prepare the final message
	Variant message;
	message["status"] = status;
	message["description"] = description;
	message["data"] = data;

	//2. Send it
	switch (pTo->GetType()) {
		case PT_INBOUND_CLITXT:
			return ((InboundBaseCLIProtocol *) pTo)->SendMessage(message);
		default:
			WARN("Protocol %s not supported yet", STR(tagToString(pTo->GetType())));
			return false;
	}
}

bool BaseCLIAppProtocolHandler::SendFail(BaseProtocol *pTo, string description) {
	Variant dummy;
	return Send(pTo, "FAIL", description, dummy);
}

bool BaseCLIAppProtocolHandler::SendSuccess(BaseProtocol *pTo, string description, Variant &data) {
	return Send(pTo, "SUCCESS", description, data);
}

#endif /* HAS_PROTOCOL_CLI */
