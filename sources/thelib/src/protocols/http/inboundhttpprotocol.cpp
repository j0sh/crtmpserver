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



#ifdef HAS_PROTOCOL_HTTP
#include "protocols/http/inboundhttpprotocol.h"

InboundHTTPProtocol::InboundHTTPProtocol()
: BaseHTTPProtocol(PT_INBOUND_HTTP) {

}

InboundHTTPProtocol::~InboundHTTPProtocol() {
}

string InboundHTTPProtocol::GetOutputFirstLine() {
	return "HTTP/1.1 200 OK";
}

Variant InboundHTTPProtocol::GetOutputHTTPHeaders() {
	return Variant();
}

bool InboundHTTPProtocol::ParseFirstLine(string &line, Variant &headers) {
	//FINEST("first line: %s", STR(line));

	vector<string> parts;
	split(line, " ", parts);
	if (parts.size() != 3) {
		FATAL("Incorrect first line: %s", STR(line));
		return false;
	}

	if (parts[2] != HTTP_VERSION_1_1) {
		FATAL("Http version not supported: %s", STR(parts[2]));
		return false;
	}

	if ((parts[0] != HTTP_METHOD_GET) && (parts[0] != HTTP_METHOD_POST)) {
		FATAL("Http method not supported: %s", STR(parts[0]));
		return false;
	}

	headers[HTTP_METHOD] = parts[0];
	headers[HTTP_URL] = parts[1];
	headers[HTTP_VERSION] = parts[2];

	return true;
}

#endif /* HAS_PROTOCOL_HTTP */

