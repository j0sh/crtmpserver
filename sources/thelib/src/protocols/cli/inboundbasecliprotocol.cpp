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

#include "netio/kqueue/iohandlermanager.h"


#ifdef HAS_PROTOCOL_CLI
#include "protocols/cli/inboundbasecliprotocol.h"
#include "application/clientapplicationmanager.h"
#include "netio/netio.h"

InboundBaseCLIProtocol::InboundBaseCLIProtocol(uint64_t type)
: BaseProtocol(type) {

}

InboundBaseCLIProtocol::~InboundBaseCLIProtocol() {
}

bool InboundBaseCLIProtocol::Initialize(Variant &parameters) {
	GetCustomParameters() = parameters;
	SendSuccess("Hello from rtmpd (http://www.rtmpd.com)");
	return true;
}

bool InboundBaseCLIProtocol::AllowFarProtocol(uint64_t type) {
	return (type == PT_TCP) || (type == PT_INBOUND_HTTP);
}

bool InboundBaseCLIProtocol::AllowNearProtocol(uint64_t type) {
	ASSERT("Operation not supported");
}

IOBuffer * InboundBaseCLIProtocol::GetOutputBuffer() {
	if (GETAVAILABLEBYTESCOUNT(_outputBuffer) != 0)
		return &_outputBuffer;
	return NULL;
}

bool InboundBaseCLIProtocol::SignalInputData(int32_t recvAmount) {
	ASSERT("Operation not supported");
}

#define MAX_COMMAND_LENGTH 1024

bool InboundBaseCLIProtocol::SignalInputData(IOBuffer &buffer) {
	//1. Get the buffer and the length
	uint8_t *pBuffer = GETIBPOINTER(buffer);
	uint32_t length = GETAVAILABLEBYTESCOUNT(buffer);
	if (length == 0)
		return true;

	//2. Walk through the buffer and execute the commands
	string command = "";
	for (uint32_t i = 0; i < length; i++) {
		if ((pBuffer[i] == 0x0d) || (pBuffer[i] == 0x0a)) {
			if (command != "") {
				ExecuteCommand(command);
			}
			command = "";
			buffer.Ignore(i);
			pBuffer = GETIBPOINTER(buffer);
			length = GETAVAILABLEBYTESCOUNT(buffer);
			i = 0;
			continue;
		}
		command += (char) pBuffer[i];
		if (command.length() >= MAX_COMMAND_LENGTH) {
			FATAL("Command too long");
			return false;
		}
	}

	//3. Done
	return true;
}

void InboundBaseCLIProtocol::Send(string status, string message) {
	_outputBuffer.ReadFromString("STATUS: " + status + "\r\n\r\n" + message + "\r\nrtmpd> ");
	EnqueueForOutbound();
}

void InboundBaseCLIProtocol::SendFail(string message) {
	Send("FAIL", message);
}

void InboundBaseCLIProtocol::SendSuccess(string message) {
	Send("SUCCESS", message);
}

void InboundBaseCLIProtocol::ExecuteCommand(string &command) {
	//1. Replace the '\\' escape sequence
	replace(command, "\\\\", "_#slash#_");

	//2. Replace the '\ ' escape sequence
	replace(command, "\\ ", "_#space#_");

	//3. Append "cmd=" in front of the command
	command = "cmd=" + command;

	//4. create the map
	map<string, string> rawMap = mapping(command, " ", "=", true);

	//5. Create the variant
	Variant cmd;
	cmd["command"] = rawMap["cmd"];
	rawMap.erase("cmd");

	string key;

	FOR_MAP(rawMap, string, string, i) {
		key = MAP_KEY(i);
		replace(key, "_#space#_", " ");
		replace(key, "_#slash#_", "\\");
		replace(MAP_VAL(i), "_#space#_", " ");
		replace(MAP_VAL(i), "_#slash#_", "\\");
		cmd["parameters"][key] = MAP_VAL(i);
	}

	ExecuteCommand(cmd);
}

void InboundBaseCLIProtocol::ExecuteCommand(Variant &command) {
	string cmdName = lowercase(command["command"]);
	if (cmdName == "help") {
		ExecuteCommandHelp(command);
	} else if (cmdName == "shutdown") {
		ExecuteCommandShutdown(command);
	} else if (cmdName == "listapps") {
		ExecuteCommandListApps(command);
	} else if (cmdName == "listservices") {
		ExecuteCommandListServices(command);
	} else if (cmdName == "shutdownapp") {
		ExecuteCommandShutdownApp(command);
	} else {
		SendFail(format("command %s not known. Type help for list of commands", STR(cmdName)));
	}
}

void InboundBaseCLIProtocol::ExecuteCommandHelp(Variant &command) {
	string output = "Available commands:\r\n\r\n";
	output += "help                      prints this help\r\n";
	output += "shutdown                  terminate the server process\r\n";
	output += "listApps                  returns a list of loaded applications\r\n";
	output += "listServices              returns a list of available services\r\n";
	output += "shutdownapp appId=<id>    terminates an application\r\n";
	SendSuccess(output);
}

void InboundBaseCLIProtocol::ExecuteCommandShutdown(Variant &command) {
	IOHandlerManager::SignalShutdown();
}

void InboundBaseCLIProtocol::ExecuteCommandListApps(Variant &command) {
	string output = "Loaded applications:\r\n\r\n";
	map<uint32_t, BaseClientApplication *> apps = ClientApplicationManager::GetAllApplications();

	FOR_MAP(apps, uint32_t, BaseClientApplication *, i) {
		output += format("id: % 2d: name: %s\r\n", MAP_KEY(i), STR(MAP_VAL(i)->GetName()));
	}
	SendSuccess(output);
}

void InboundBaseCLIProtocol::ExecuteCommandListServices(Variant &command) {
	string output = "Available services:\r\n\r\n";

	map<uint32_t, IOHandler *> handlers = IOHandlerManager::GetActiveHandlers();

	FOR_MAP(handlers, uint32_t, IOHandler *, i) {
		IOHandler *pHandler = MAP_VAL(i);
		if ((pHandler->GetType() != IOHT_ACCEPTOR)
				&& (pHandler->GetType() != IOHT_UDP_CARRIER))
			continue;
		switch (pHandler->GetType()) {
			case IOHT_ACCEPTOR:
			{
				TCPAcceptor *pAcceptor = (TCPAcceptor *) pHandler;
				Variant params = pAcceptor->GetParameters();
				string ip = params[CONF_IP];
				uint16_t port = params[CONF_PORT];
				string protocol = params[CONF_PROTOCOL];
				string sslCert = "";
				if (params[CONF_SSL_CERT] == V_STRING) {
					sslCert = (string) params[CONF_SSL_CERT];
				}
				string sslKey = "";
				if (params[CONF_SSL_KEY] == V_STRING) {
					sslKey = (string) params[CONF_SSL_KEY];
				}

				output += format("id: % 3d; ip: % 15s; port: % 5d; protocol: %s; appOwner: %s",
						pAcceptor->GetId(),
						STR(ip),
						port,
						STR(protocol),
						pAcceptor->GetApplication() != NULL ?
						STR(pAcceptor->GetApplication()->GetName()) : "#none#");
				if (sslCert != "") {
					output += "; sslCert: " + sslCert;
				}
				if (sslKey != "") {
					output += "; sslKey: " + sslKey;
				}
				output += "\r\n";
				break;
			}
			case IOHT_UDP_CARRIER:
			{
				break;
			}
			default:
			{
				break;
			}
		}
	}

	SendSuccess(output);
}

void InboundBaseCLIProtocol::ExecuteCommandShutdownApp(Variant &command) {
	if (command["parameters"]["appid"] != V_STRING) {
		SendFail("Incorrect appid");
		return;
	}
	uint32_t appId = atoi(STR(command["parameters"]["appid"]));
	BaseClientApplication *pApp = ClientApplicationManager::FindAppById(appId);
	if (pApp == NULL) {
		SendFail(format("appId %d not found", appId));
		return;
	}
	BaseClientApplication::Shutdown(pApp);
	SendSuccess("Application removed");
}

#endif /* HAS_PROTOCOL_CLI */
