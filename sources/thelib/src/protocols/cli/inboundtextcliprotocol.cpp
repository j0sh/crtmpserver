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
#include "protocols/cli/inboundtextcliprotocol.h"

InboundTextCLIProtocol::InboundTextCLIProtocol()
: InboundBaseCLIProtocol(PT_INBOUND_CLITXT) {

}

InboundTextCLIProtocol::~InboundTextCLIProtocol() {
}

#define MAX_COMMAND_LENGTH 1024

bool InboundTextCLIProtocol::Initialize(Variant &parameters) {
	InboundBaseCLIProtocol::Initialize(parameters);
	_outputBuffer.ReadFromString("\r\nWelcome to "HTTP_HEADERS_X_POWERED_BY_US"\r\n\r\nType help for a list of commands\r\n\r\nrtmpd>");
	return EnqueueForOutbound();
}

bool InboundTextCLIProtocol::SignalInputData(IOBuffer &buffer) {
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
				if (!ParseCommand(command)) {
					FATAL("Unable to parse command\n`%s`", STR(command));
					return false;
				}
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

bool InboundTextCLIProtocol::SendMessage(Variant &message) {
	string output = "";
	output = "\r\nSTATUS: " + (string) message["status"] + "\r\n\r\n";
	output += (string) message["description"] + "\r\n";
	if (message["data"] == V_MAP) {
		output += "\r\n";

		string item = "";
		Variant count;
		count["count"] = message["data"].MapSize();
		if (!count.SerializeToJSON(item)) {
			FATAL("Unable to serialize to JSON");
			return false;
		}
		output += item + "\r\n\r\n";

		FOR_MAP(message["data"], string, Variant, i) {
			item = "";
			if (!MAP_VAL(i).SerializeToJSON(item)) {
				FATAL("Unable to serialize to JSON");
				return false;
			}
			output += item + "\r\n";
		}
	}
	
	output += "\r\nrtmpd>";
	_outputBuffer.ReadFromString(output);
	return EnqueueForOutbound();
}

bool InboundTextCLIProtocol::ParseCommand(string &command) {
	//1. Replace the '\\' escape sequence
	replace(command, "\\\\", "_#slash#_");

	//2. Replace the '\ ' escape sequence
	replace(command, "\\ ", "_#space#_");

	//3. Append "cmd=" in front of the command
	command = "cmd=" + command;

	//4. create the map
	map<string, string> rawMap = mapping(command, " ", "=", true);

	//5. Create the variant
	Variant message;
	message["command"] = rawMap["cmd"];
	rawMap.erase("cmd");

	string key;
	string value;
	vector<string> list;

	FOR_MAP(rawMap, string, string, i) {
		key = lowercase(MAP_KEY(i));
		replace(key, "_#space#_", " ");
		replace(key, "_#slash#_", "\\");

		value = MAP_VAL(i);
		replace(value, "_#space#_", " ");
		replace(value, "_#slash#_", "\\");

		list.clear();
		split(value, ",", list);
		if (list.size() != 1) {

		} else {
			message["parameters"][key] = value;
		}
	}

	return ProcessMessage(message);
}

#endif /* HAS_PROTOCOL_CLI */
