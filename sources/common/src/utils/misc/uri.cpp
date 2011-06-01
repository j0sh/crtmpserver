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

//#define LOG_URI_SPLIT(...) FINEST(__VA_ARGS__)
#define LOG_URI_SPLIT(...)

static map<string, uint16_t> ___schemeToPort;

bool parseURI(string stringUri, URI &uri) {
	LOG_URI_SPLIT("stringUri: %s", STR(stringUri));
	uri.Reset();
	trim(stringUri);
	if (stringUri == "")
		return false;

	if (stringUri.size() > 1024)
		return false;

	uri.fullUri = stringUri;
	LOG_URI_SPLIT("uri.fullUri: %s", STR(uri.fullUri));


	// scheme://user:pwd@host:port/document/parts/here

	vector<string> components;
	split(stringUri, "/", components);
	for (uint32_t i = 0; i < components.size(); i++) {
		LOG_URI_SPLIT("%u: %s", i, STR(components[i]));
	}

	//0 - scheme:
	if (components[0] == "")
		return false;

	if (components[0][components[0].size() - 1] != ':')
		return false;

	uri.scheme = lowerCase(components[0].substr(0, components[0].size() - 1));
	LOG_URI_SPLIT("uri.scheme: %s", STR(uri.scheme));


	//1 - nothing
	if (components[1] != "")
		return false;

	//2 - user:pwd@host:port
	vector<string> hostComponents;
	if (components[2].find("@") != string::npos) {
		split(components[2], "@", hostComponents);
		if (hostComponents.size() != 2)
			return false;
		if ((hostComponents[0] == "")
				|| (hostComponents[1] == "")) {
			return false;
		}
		components[2] = hostComponents[1];
		vector<string> userNamePasswordComponents;
		split(hostComponents[0], ":", userNamePasswordComponents);
		if (userNamePasswordComponents.size() != 2)
			return false;
		if ((userNamePasswordComponents[0] == "")
				|| (userNamePasswordComponents[1] == "")) {
			return false;
		}
		uri.userName = userNamePasswordComponents[0];
		LOG_URI_SPLIT("uri.userName: %s", STR(uri.userName));
		uri.password = userNamePasswordComponents[1];
		LOG_URI_SPLIT("uri.password: %s", STR(uri.password));
	}

	split(components[2], ":", hostComponents);
	if (hostComponents.size() == 1) {
		if (hostComponents[0] == "")
			return false;
		uri.host = hostComponents[0];
		LOG_URI_SPLIT("uri.host: %s", STR(uri.host));
	} else if (hostComponents.size() == 2) {
		if ((hostComponents[0] == "")
				|| (hostComponents[0] == ""))
			return false;
		uri.host = hostComponents[0];
		LOG_URI_SPLIT("uri.host: %s", STR(uri.host));
		int32_t port = atoi(STR(hostComponents[1]));
		if ((port <= 0) || (port > 65535))
			return false;
		uri.port = (uint16_t) port;
	} else {
		return false;
	}

	if (uri.port == 0) {
		if (___schemeToPort.size() == 0) {
			___schemeToPort["http"] = 80;
			___schemeToPort["rtmpt"] = 80;
			___schemeToPort["rtmpte"] = 80;
			___schemeToPort["https"] = 443;
			___schemeToPort["rtmps"] = 443;
			___schemeToPort["rtsp"] = 554;
			___schemeToPort["rtmp"] = 1935;
			___schemeToPort["rtmpe"] = 1935;
			___schemeToPort["mms"] = 1755;
		}
		if (MAP_HAS1(___schemeToPort, uri.scheme))
			uri.port = ___schemeToPort[uri.scheme];
		else
			return false;
	}
	LOG_URI_SPLIT("uri.port: %u", uri.port);

	for (uint32_t i = 3; i < components.size(); i++) {
		uri.fullDocumentPath += "/" + components[i];
	}
	LOG_URI_SPLIT("uri.fullDocumentPath: %s", STR(uri.fullDocumentPath));

	uri.documentPath = "/";
	for (uint32_t i = 3; i < components.size() - 1; i++) {
		uri.documentPath += components[i];
		if (i != components.size() - 2)
			uri.documentPath += "/";
	}
	LOG_URI_SPLIT("uri.documentPath: %s", STR(uri.documentPath));

	if ((components.size() - 1) >= 3) {
		uri.document = components[components.size() - 1];
		vector<string> documentComponents;
		split(uri.document, "?", documentComponents);
		if (documentComponents.size() == 2) {
			uri.document = documentComponents[0];
			map<string, string> params;
			params = mapping(documentComponents[1], "&", "=", true);

			FOR_MAP(params, string, string, i) {
				uri.parameters[MAP_KEY(i)] = MAP_VAL(i);
				LOG_URI_SPLIT("uri.parameters[\"%s\"]: %s",
						STR(MAP_KEY(i)),
						STR(MAP_VAL(i)));
			}
		}
	}
	LOG_URI_SPLIT("uri.document: %s", STR(uri.document));

	return true;
}

void URI::Reset() {
	fullUri = scheme = host = userName = password = fullDocumentPath = documentPath = document = "";
	port = 0;
	parameters.clear();
}

Variant URI::ToVariant() {
	Variant result;
	result["fullUri"] = fullUri;
	result["scheme"] = scheme;
	result["host"] = host;
	result["ip"] = ip;
	result["port"] = (uint16_t) port;
	result["userName"] = userName;
	result["password"] = password;
	result["fullDocumentPath"] = fullDocumentPath;
	result["documentPath"] = documentPath;
	result["document"] = document;

	FOR_MAP(parameters, string, string, i) {
		result["parameters"][MAP_KEY(i)] = MAP_VAL(i);
	}

	return result;
}

bool URI::FromVariant(Variant & variant, URI &uri) {
	uri.Reset();

	if (variant != V_MAP)
		return false;

	if ((!(variant.HasKey("fullUri")))
			|| (!(variant.HasKey("scheme")))
			|| (!(variant.HasKey("host")))
			|| (!(variant.HasKey("ip")))
			|| (!(variant.HasKey("port")))
			|| (!(variant.HasKey("userName")))
			|| (!(variant.HasKey("password")))
			|| (!(variant.HasKey("fullDocumentPath")))
			|| (!(variant.HasKey("documentPath")))
			|| (!(variant.HasKey("document")))
			) {
		return false;
	}

	if ((variant["fullUri"] != V_STRING)
			|| (variant["scheme"] != V_STRING)
			|| (variant["host"] != V_STRING)
			|| (variant["ip"] != V_STRING)
			|| (variant["port"] != _V_NUMERIC)
			|| (variant["userName"] != V_STRING)
			|| (variant["password"] != V_STRING)
			|| (variant["fullDocumentPath"] != V_STRING)
			|| (variant["documentPath"] != V_STRING)
			|| (variant["document"] != V_STRING)) {
		return false;
	}

	uri.fullUri = (string) variant["fullUri"];
	uri.scheme = (string) variant["scheme"];
	uri.host = (string) variant["host"];
	uri.ip = (string) variant["ip"];
	uri.port = (uint16_t) variant["port"];
	uri.userName = (string) variant["userName"];
	uri.password = (string) variant["password"];
	uri.fullDocumentPath = (string) variant["fullDocumentPath"];
	uri.documentPath = (string) variant["documentPath"];
	uri.document = (string) variant["document"];

	return true;
}

string URI::ToString() {
	return fullUri;
}

bool URI::FromString(string stringUri, bool resolveHost, URI &uri) {
	if (!parseURI(stringUri, uri)) {
		uri.Reset();
		return false;
	}

	if (resolveHost) {
		uri.ip = getHostByName(uri.host);
		if (uri.ip == "") {
			FATAL("Unable to resolve host: %s", STR(uri.host));
			uri.Reset();
			return false;
		}
	}

	return true;
}
