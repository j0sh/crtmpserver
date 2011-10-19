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


#include "utils/misc/uri.h"
#include "utils/logging/logging.h"

static map<string, uint16_t> _schemeToPort;
//Variant URI::_dummy;

#ifdef DEBUG_URI
#define LOG_URI(...) FINEST(__VA_ARGS__)
#else
#define LOG_URI(...)
#endif /* DEBUG_URI */

bool parseURI(string stringUri, URI &uri) {
	/*
	 * schema://[username[:password]@]host[:port][/[path[?parameters]]]
	 */
	LOG_URI("------------------------");
	LOG_URI("stringUri: `%s`", STR(stringUri));
	string fullUri;
	string fullUriWithAuth = stringUri;
	string scheme;
	string authentication;
	string username;
	string password;
	string hostPort;
	string host;
	string portString;
	uint16_t port = 0;
	bool portSpecified = false;
	string fullDocumentPathWithParameters;
	string fullDocumentPath;
	string fullParameters;
	string documentPath;
	string document;
	string documentWithFullParameters;
	Variant parameters;

	string::size_type cursor = 0;
	string::size_type pos = 0;

	//1. Reset
	uri.Reset();

	//2. trim
	trim(stringUri);
	if (stringUri == "") {
		FATAL("Empty uri");
		return false;
	}

	//2. Get the scheme and the default port
	pos = stringUri.find("://", cursor);
	if (pos == string::npos) {
		FATAL("Unable to determine scheme");
		return false;
	}
	scheme = lowerCase(stringUri.substr(cursor, pos - cursor));
	cursor = pos + 3;
	if (_schemeToPort.size() == 0) {
		_schemeToPort["http"] = 80;
		_schemeToPort["rtmpt"] = 80;
		_schemeToPort["rtmpte"] = 80;
		_schemeToPort["https"] = 443;
		_schemeToPort["rtmps"] = 443;
		_schemeToPort["rtsp"] = 554;
		_schemeToPort["rtmp"] = 1935;
		_schemeToPort["rtmpe"] = 1935;
		_schemeToPort["mms"] = 1755;
	}
	if (MAP_HAS1(_schemeToPort, scheme)) {
		port = _schemeToPort[scheme];
	}
	//	else {
	//		FATAL("Scheme `%s` not supported", STR(scheme));
	//		return false;
	//	}
	LOG_URI("scheme: %s; default port: %"PRIu16, STR(scheme), port);

	//3. get the authentication portion. the search starts from
	//where the scheme detection left and up to the first / character
	string::size_type limit = stringUri.find("/", cursor);
	bool hasAuthentication = false;
	pos = stringUri.find("@", cursor);
	if (pos != string::npos) {
		if (limit != string::npos) {
			hasAuthentication = pos<limit;
		}
		hasAuthentication = true;
	}
	if (hasAuthentication) {
		authentication = stringUri.substr(cursor, pos - cursor);
		fullUri = stringUri.substr(0, cursor);
		fullUri += stringUri.substr(pos + 1);
		cursor = pos + 1;
	} else {
		fullUri = fullUriWithAuth;
	}
	if (authentication != "") {
		pos = authentication.find(":");
		if (pos != string::npos) {
			username = authentication.substr(0, pos);
			password = authentication.substr(pos + 1);
		} else {
			username = authentication;
			password = "";
		}
	}
	LOG_URI("fullUri: `%s`; fullUriWithAuth: `%s`", STR(fullUri), STR(fullUriWithAuth));
	LOG_URI("username: `%s`; password: `%s`", STR(username), STR(password));

	//4. Get the host:port
	pos = stringUri.find("/", cursor);
	if (pos == string::npos) {
		hostPort = stringUri.substr(cursor);
		cursor = stringUri.size() - 1;
		fullDocumentPathWithParameters = "/";
	} else {
		hostPort = stringUri.substr(cursor, pos - cursor);
		cursor = pos + 1;
		fullDocumentPathWithParameters = "/" + stringUri.substr(cursor);
	}
	trim(hostPort);
	if (hostPort == "") {
		FATAL("Invalid host:port specified");
		return false;
	}
	pos = hostPort.find(":");
	if (pos == string::npos) {
		host = hostPort;
		portSpecified = false;
	} else {
		host = hostPort.substr(0, pos);
		trim(host);
		portString = hostPort.substr(pos + 1);
		portSpecified = true;
		port = (uint16_t) atoi(STR(portString));
		if (format("%"PRIu16, port) != portString) {
			FATAL("Invalid port number specified: `%s`", STR(portString));
			return false;
		}
	}
	LOG_URI("host: %s; port: %"PRIu16"; portSpecified: %d", STR(host), port, portSpecified);

	//5. fullDocumentPathWithParameters
	fullDocumentPath = "/";
	fullParameters = "";
	documentPath = "/";
	document = "";
	documentWithFullParameters = "";
	parameters.Reset();
	parameters.IsArray(false);
	if (fullDocumentPathWithParameters != "/") {
		pos = fullDocumentPathWithParameters.find("?");
		if (pos == string::npos) {
			fullDocumentPath = fullDocumentPathWithParameters;
			fullParameters = "";
		} else {
			fullDocumentPath = fullDocumentPathWithParameters.substr(0, pos);
			fullParameters = fullDocumentPathWithParameters.substr(pos + 1);
		}

		trim(fullParameters);
		if (fullParameters != "") {
			vector<string> elements;
			split(fullParameters, "&", elements);
			for (uint32_t i = 0; i < elements.size(); i++) {
				string kvp = elements[i];
				if (kvp == "")
					continue;
				string k = "";
				string v = "";
				pos = kvp.find("=");
				if (pos == string::npos) {
					k = kvp;
					v = "";
				} else {
					k = kvp.substr(0, pos);
					v = kvp.substr(pos + 1);
				}
				if (k == "")
					continue;
				parameters[k] = v;
			}
		}

		for (string::size_type i = fullDocumentPath.size() - 1; i >= 0; i--) {
			if (fullDocumentPath[i] == '/')
				break;
			document = fullDocumentPath[i] + document;
		}
		documentPath = fullDocumentPath.substr(0, fullDocumentPath.size() - document.size());
		documentWithFullParameters = document;
		if (fullParameters != "")
			documentWithFullParameters += "?" + fullParameters;
	}
	LOG_URI("fullDocumentPathWithParameters: `%s`", STR(fullDocumentPathWithParameters));
	LOG_URI("fullDocumentPath: `%s`", STR(fullDocumentPath));
	LOG_URI("fullParameters: `%s`", STR(fullParameters));
	LOG_URI("documentPath: `%s`", STR(documentPath));
	LOG_URI("document: `%s`", STR(document));
	LOG_URI("documentWithFullParameters: `%s`", STR(documentWithFullParameters));
	LOG_URI("parameters:");
#ifdef DEBUG_URI

	FOR_MAP(parameters, string, Variant, i) {
		LOG_URI("\t`%s`: `%s`", STR(MAP_KEY(i)), STR(MAP_VAL(i)));
	}
#endif /* DEBUG_URI */

	if (port == 0) {
		FATAL("Invalid URI. No port specified and the scheme `%s` is unknown", STR(scheme));
		return false;
	}
	uri.originalUri(stringUri);
	uri.fullUri(fullUri);
	uri.fullUriWithAuth(fullUriWithAuth);
	uri.scheme(scheme);
	uri.userName(username);
	uri.password(password);
	uri.host(host);
	uri.port(port);
	uri.portSpecified(portSpecified);
	uri.fullDocumentPathWithParameters(fullDocumentPathWithParameters);
	uri.fullDocumentPath(fullDocumentPath);
	uri.fullParameters(fullParameters);
	uri.documentPath(documentPath);
	uri.document(document);
	uri.documentWithFullParameters(documentWithFullParameters);
	uri.parameters(parameters);

	return true;
}

bool URI::FromVariant(Variant & variant, URI &uri) {
	uri.Reset();

	if (variant != V_MAP) {
		FATAL("Variant is not a map");
		return false;
	}

	if ((!(variant.HasKeyChain(V_STRING, true, 1, "originalUri")))
			|| (!(variant.HasKeyChain(V_STRING, true, 1, "fullUri")))
			|| (!(variant.HasKeyChain(V_STRING, true, 1, "fullUriWithAuth")))
			|| (!(variant.HasKeyChain(V_STRING, true, 1, "scheme")))
			|| (!(variant.HasKeyChain(V_STRING, true, 1, "userName")))
			|| (!(variant.HasKeyChain(V_STRING, true, 1, "password")))
			|| (!(variant.HasKeyChain(V_STRING, true, 1, "host")))
			|| (!(variant.HasKeyChain(V_STRING, true, 1, "ip")))
			|| (!(variant.HasKeyChain(_V_NUMERIC, true, 1, "port")))
			|| (!(variant.HasKeyChain(V_BOOL, true, 1, "portSpecified")))
			|| (!(variant.HasKeyChain(V_STRING, true, 1, "fullDocumentPathWithParameters")))
			|| (!(variant.HasKeyChain(V_STRING, true, 1, "fullDocumentPath")))
			|| (!(variant.HasKeyChain(V_STRING, true, 1, "fullParameters")))
			|| (!(variant.HasKeyChain(V_STRING, true, 1, "documentPath")))
			|| (!(variant.HasKeyChain(V_STRING, true, 1, "document")))
			|| (!(variant.HasKeyChain(V_STRING, true, 1, "documentWithFullParameters")))
			|| (!(variant.HasKeyChain(V_MAP, true, 1, "parameters")))
			) {
		FATAL("One or more type mismatch");
		return false;
	}

	Variant *pURI = (Variant *) & uri;
	*pURI = variant;

	return true;
}

bool URI::FromString(string stringUri, bool resolveHost, URI &uri) {
	if (!parseURI(stringUri, uri)) {
		uri.Reset();
		return false;
	}

	if (resolveHost) {
		string ip = getHostByName(uri.host());
		if (ip == "") {
			FATAL("Unable to resolve host: %s", STR(uri.host()));
			uri.Reset();
			return false;
		}
		uri.ip(ip);
	} else {
		uri.ip("");
	}

	return true;
}
