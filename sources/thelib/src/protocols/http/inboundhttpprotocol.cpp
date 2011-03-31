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
#include "protocols/rtmp/inboundhttp4rtmp.h"

InboundHTTPProtocol::InboundHTTPProtocol()
: BaseHTTPProtocol(PT_INBOUND_HTTP) {
	_statusCode = 200;
}

InboundHTTPProtocol::~InboundHTTPProtocol() {
}

void InboundHTTPProtocol::SetStatusCode(uint16_t statusCode) {
	_statusCode = statusCode;
}

bool InboundHTTPProtocol::Initialize(Variant& parameters) {
	if (parameters["hasAuth"] == V_BOOL) {
		_hasAuth = (bool)parameters["hasAuth"];
		return BaseHTTPProtocol::Initialize(parameters);
	}
	if ((parameters.HasKey("auth"))
			&& (parameters["auth"] == V_MAP)
			&& (parameters["auth"].HasKey("realms"))
			&& (parameters["auth"]["realms"] == V_MAP)
			&& (parameters["auth"]["realms"].MapSize() != 0)
			&& (parameters["auth"].HasKey("uris"))
			&& (parameters["auth"]["uris"] == V_MAP)
			&& (parameters["auth"]["uris"].MapSize() != 0)
			) {

		Variant optimizedRealms;
		optimizedRealms.IsArray(false);

		FOR_MAP(parameters["auth"]["realms"], string, Variant, i) {
			Variant realm = MAP_VAL(i);
			if ((!realm.HasKey("method"))
					|| (realm["method"] != V_STRING)
					|| (realm["method"] != "digest")
					) {
				FATAL("Invalid realm:\n%s", STR(realm.ToString()));
				return false;
			}
			if ((!realm.HasKey("name"))
					|| (realm["name"] != V_STRING)
					|| (optimizedRealms.HasKey(realm["name"]))
					) {
				FATAL("Duplicate realm:\n%s", STR(realm.ToString()));
				return false;
			}
			if ((!realm.HasKey("credentials"))
					|| (realm["credentials"] != V_MAP)
					|| (realm["credentials"].MapSize() == 0)
					) {
				FATAL("Credentials list is empty:\n%s", STR(realm.ToString()));
				return false;
			}

			FOR_MAP(realm["credentials"], string, Variant, j) {
				if (MAP_VAL(j) != V_STRING) {
					FATAL("Invalid credentials in realm:\n%s", STR(realm.ToString()));
					return false;
				}
			}
			optimizedRealms[realm["name"]] = realm;
		}
		parameters["auth"]["realms"] = optimizedRealms;

		Variant optimizedUris;
		optimizedUris.IsArray(false);

		FOR_MAP(parameters["auth"]["uris"], string, Variant, i) {
			Variant uri = MAP_VAL(i);
			if ((!uri.HasKey("uri"))
					|| (uri["uri"] != V_STRING)
					|| (optimizedUris.HasKey(uri["uri"]))
					) {
				FATAL("Invalid or duplicate uri value:\n%s", STR(uri.ToString()));
				return false;
			}
			if ((!uri.HasKey("realm"))
					|| (uri["realm"] != V_STRING)
					|| (!optimizedRealms.HasKey(uri["realm"]))
					) {
				FATAL("Invalid realm or realm not found:\n%s", STR(uri.ToString()));
				return false;
			}
			optimizedUris[uri["uri"]] = uri["realm"];
		}
		parameters["auth"]["uris"] = optimizedUris;
		_hasAuth = true;
	}
	parameters["hasAuth"] = (bool)_hasAuth;
	return BaseHTTPProtocol::Initialize(parameters);
}

string InboundHTTPProtocol::GetOutputFirstLine() {
	switch (_statusCode) {
		case 200:
			return "HTTP/1.1 200 OK";
		case 401:
			return "HTTP/1.1 401 Unauthorized";
		default:
			return format("HTTP/1.1 %hu Unknwon", _statusCode);
	}
}

bool InboundHTTPProtocol::ParseFirstLine(string &line, Variant &headers) {

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

bool InboundHTTPProtocol::Authenticate() {
	_continueAfterParseHeaders = true;
	if (!_hasAuth)
		return true;

	//1. Get the auth node
	BaseProtocol *pProtocol = GetNearEndpoint();
	if (pProtocol == NULL) {
		FATAL("No near endpoint");
		return false;
	}
	Variant &auth = pProtocol->GetCustomParameters()["auth"];

	//2. Is realm/uri combination into authentication mode?
	if (!auth["uris"].HasKey(_headers[HTTP_FIRST_LINE][HTTP_URL])) {
		return true;
	}

	//3. Do we have Authorization header in the request?
	if (!_headers[HTTP_HEADERS].HasKey(HTTP_HEADERS_AUTORIZATION, false)) {
		return SendAuthRequired(auth);
	}
	string authorization = _headers[HTTP_HEADERS][HTTP_HEADERS_AUTORIZATION];
	if (authorization.length() < 7) {
		FATAL("Invalid authorization header");
		return SendAuthRequired(auth);
	}
	authorization = authorization.substr(7);
	map<string, string> authMap = mapping(authorization, ",", "=", true);

	FOR_MAP(authMap, string, string, i) {
		replace(MAP_VAL(i), "\"", "");
	}

	string pass = "";
	if (!auth["realms"].HasKey(authMap["realm"])) {
		FATAL("Invalid authorization header");
		return SendAuthRequired(auth);
	}
	if (!auth["realms"][authMap["realm"]]["credentials"].HasKey(authMap["username"])) {
		FATAL("Invalid authorization header");
		return SendAuthRequired(auth);
	}
	pass = (string) auth["realms"][(string) authMap["realm"]]["credentials"][(string) authMap["username"]];

	string wanted = md5(
			md5(authMap["username"] + ":" + authMap["realm"] + ":" + pass, true)
			+ ":" + authMap["nonce"] + ":" + authMap["nc"] + ":" + authMap["cnonce"] + ":" + authMap["qop"] + ":"
			+ md5((string) _headers[HTTP_FIRST_LINE][HTTP_METHOD] + ":" + authMap["uri"], true), true);
	if (wanted != authMap["response"]) {
		FATAL("Invalid authorization header");
		return SendAuthRequired(auth);
	}

	return true;
}

bool InboundHTTPProtocol::SendAuthRequired(Variant &auth) {
	SetStatusCode(401);
	string wwwAuth = format("Digest realm=\"%s\",qop=\"auth\",nonce=\"%s\",opaque=\"%s\"",
			STR(auth["uris"][_headers[HTTP_FIRST_LINE][HTTP_URL]]),
			STR(md5(generateRandomString(8), true)),
			STR(md5(generateRandomString(8), true)));
	SetOutboundHeader(HTTP_HEADERS_WWWAUTHENTICATE, wwwAuth);
	_continueAfterParseHeaders = false;
	EnqueueForOutbound();
	GracefullyEnqueueForDelete();
	return true;
}

#endif /* HAS_PROTOCOL_HTTP */

