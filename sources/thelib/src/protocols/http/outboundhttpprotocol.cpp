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
#include "protocols/http/outboundhttpprotocol.h"

OutboundHTTPProtocol::OutboundHTTPProtocol()
: BaseHTTPProtocol(PT_OUTBOUND_HTTP) {
}

OutboundHTTPProtocol::~OutboundHTTPProtocol() {
}

string OutboundHTTPProtocol::Method() {
	return _method;
}

void OutboundHTTPProtocol::Method(string method) {
	_method = method;
}

string OutboundHTTPProtocol::Document() {
	return _document;
}

void OutboundHTTPProtocol::Document(string document) {
	_document = document;
}

string OutboundHTTPProtocol::Host() {
	return _host;
}

void OutboundHTTPProtocol::Host(string host) {
	_host = host;
}

bool OutboundHTTPProtocol::EnqueueForOutbound() {
	SetOutboundHeader(HTTP_HEADERS_HOST, _host);
	return BaseHTTPProtocol::EnqueueForOutbound();
}

bool OutboundHTTPProtocol::Is200OK() {
	return _headers[HTTP_FIRST_LINE][HTTP_STATUS_CODE] == HTTP_STATUS_CODE_200;
}

string OutboundHTTPProtocol::GetOutputFirstLine() {
	return format("%s %s HTTP/1.1", STR(_method), STR(_document));
}

bool OutboundHTTPProtocol::ParseFirstLine(string &line, Variant &firstLineHeader) {
	vector<string> parts;
	split(line, " ", parts);
	if (parts.size() < 3) {
		FATAL("Incorrect first line: %s", STR(line));
		return false;
	}

	if ((parts[0] != HTTP_VERSION_1_1)
			&& (parts[0] != HTTP_VERSION_1_0)) {
		FATAL("Http version not supported: %s", STR(parts[0]));
		return false;
	}

	if (!isNumeric(parts[1])) {
		FATAL("Invalid HTTP code: %s", STR(parts[1]));
		return false;
	}

	string reason = "";
	for (uint32_t i = 2; i < parts.size(); i++) {
		reason += parts[i];
		if (i != parts.size() - 1)
			reason += " ";
	}

	firstLineHeader[HTTP_VERSION] = parts[0];
	firstLineHeader[HTTP_STATUS_CODE] = parts[1];
	firstLineHeader[HTTP_STATUS_CODE_REASON] = reason;

	return true;
}

bool OutboundHTTPProtocol::Authenticate() {
	return true;
}
#endif /* HAS_PROTOCOL_HTTP */

