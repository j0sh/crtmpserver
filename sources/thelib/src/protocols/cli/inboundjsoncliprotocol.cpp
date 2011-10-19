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
#include "protocols/cli/inboundjsoncliprotocol.h"
#include "protocols/http/inboundhttpprotocol.h"

InboundJSONCLIProtocol::InboundJSONCLIProtocol()
: InboundBaseCLIProtocol(PT_INBOUND_JSONCLI) {
	_useLengthPadding = false;
}

InboundJSONCLIProtocol::~InboundJSONCLIProtocol() {
}

#define MAX_COMMAND_LENGTH 8192

bool InboundJSONCLIProtocol::Initialize(Variant &parameters) {
	InboundBaseCLIProtocol::Initialize(parameters);
	if (parameters["useLengthPadding"] == V_BOOL) {
		_useLengthPadding = (bool)parameters["useLengthPadding"];
	}
	return true;
}

bool InboundJSONCLIProtocol::SignalInputData(IOBuffer &buffer) {
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

bool InboundJSONCLIProtocol::SendMessage(Variant &message) {
	string json;
	if (!message.SerializeToJSON(json)) {
		FATAL("Unable to serialize to JSON");
		return false;
	}
	json += "\r\n\r\n";
	if (_useLengthPadding) {
		uint32_t size = htonl(json.length());
		_outputBuffer.ReadFromBuffer((uint8_t *) & size, 4);
	}
	_outputBuffer.ReadFromString(json);
	return EnqueueForOutbound();
}

bool InboundJSONCLIProtocol::ParseCommand(string &command) {
	//1. Replace the '\\' escape sequence
	replace(command, "\\\\", "_#slash#_");

	//2. Replace the '\ ' escape sequence
	replace(command, "\\ ", "_#space#_");

	//2. Replace the '\=' escape sequence
	replace(command, "\\=", "_#equal#_");

	//2. Replace the '\,' escape sequence
	replace(command, "\\,", "_#coma#_");

	//3. Append "cmd=" in front of the command
	command = "cmd=" + command;
	//INFO("command: `%s`", STR(command));

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
		key = lowerCase(MAP_KEY(i));
		replace(key, "_#space#_", " ");
		replace(key, "_#slash#_", "\\");
		replace(key, "_#equal#_", "=");
		replace(key, "_#coma#_", ",");

		value = MAP_VAL(i);
		replace(value, "_#space#_", " ");
		replace(value, "_#slash#_", "\\");
		replace(value, "_#equal#_", "=");

		list.clear();
		split(value, ",", list);
		if (list.size() != 1) {
			for (uint32_t j = 0; j < list.size(); j++) {
				trim(list[j]);
				if (list[j] == "")
					continue;
				replace(list[j], "_#coma#_", ",");
				message["parameters"][key].PushToArray(list[j]);
			}
		} else {
			replace(value, "_#coma#_", ",");
			message["parameters"][key] = value;
		}
	}
	return ProcessMessage(message);
}

#endif /* HAS_PROTOCOL_CLI */
