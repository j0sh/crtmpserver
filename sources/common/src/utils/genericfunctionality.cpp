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



#include "utils/core.h"

string alowedCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

string tagToString(uint64_t tag) {
	string result;
	uint64_t copy = htonll(tag);
	result = (char *) & copy;
	return result;
}

uint64_t getTagMask(uint64_t tag) {
	uint64_t result = 0xffffffffffffffffLL;
	for (int8_t i = 56; i >= 0; i -= 8) {
		if (((tag >> i)&0xff) == 0)
			break;
		result = result >> 8;
	}
	return ~result;
}

bool isNumeric(string value) {
	//FINEST("value: `%s`", STR(value));
	return value == format("%d", atoi(STR(value)));
}

string lowercase(string value) {
	return changecase(value, true);
}

string uppercase(string value) {
	return changecase(value, false);
}

void ltrim(string &value) {
	string::size_type i = 0;
	for (i = 0; i < value.length(); i++) {
		if (value[i] != ' ' &&
				value[i] != '\t' &&
				value[i] != '\n' &&
				value[i] != '\r')
			break;
	}
	value = value.substr(i);
}

void rtrim(string &value) {
	int32_t i = 0;
	for (i = (int32_t)value.length() - 1; i >= 0; i--) {
		if (value[i] != ' ' &&
				value[i] != '\t' &&
				value[i] != '\n' &&
				value[i] != '\r')
			break;
	}
	value = value.substr(0, i + 1);
}

void trim(string &value) {
	ltrim(value);
	rtrim(value);
}

void replace(string &target, string search, string replacement) {
	if (search == replacement)
		return;
	if (search == "")
		return;
	string::size_type i = string::npos;
	while ((i = target.find(search)) != string::npos) {
		target.replace(i, search.length(), replacement);
	}
}

void split(string str, string separator, vector<string> &result) {
	result.clear();
	string::size_type position = str.find(separator);
	string::size_type lastPosition = 0;
	uint32_t separatorLength = separator.length();

	while (position != str.npos) {
		ADD_VECTOR_END(result, str.substr(lastPosition, position - lastPosition));
		lastPosition = position + separatorLength;
		position = str.find(separator, lastPosition);
	}
	ADD_VECTOR_END(result, str.substr(lastPosition, string::npos));
}

//bool nameValue(string line, string separator, string &name, string &value) {
//    name = "";
//    value = "";
//    uint64_t index = line.find(separator, 0);
//    if ((index == 0) || (index == string::npos))
//        return false;
//    name = line.substr(0, (uint32_t) index);
//    value = line.substr((uint32_t) index + separator.length(), string::npos);
//    return true;
//}

map<string, string> mapping(string str, string separator1, string separator2, bool trimStrings) {
	map<string, string> result;

	vector<string> pairs;
	split(str, separator1, pairs);

	FOR_VECTOR_ITERATOR(string, pairs, i) {
		if (VECTOR_VAL(i) != "") {
			if (VECTOR_VAL(i).find(separator2) != string::npos) {
				string key = VECTOR_VAL(i).substr(0, VECTOR_VAL(i).find(separator2));
				string value = VECTOR_VAL(i).substr(VECTOR_VAL(i).find(separator2) + 1);
				if (trimStrings) {
					trim(key);
					trim(value);
				}
				result[key] = value;
			} else {
				if (trimStrings) {
					trim(VECTOR_VAL(i));
				}
				result[VECTOR_VAL(i)] = "";
			}
		}
	}
	return result;
}

//string unmapping(map<string, string>& _map, char c1, char c2) {
//    string result;
//
//    FOR_MAP(_map, string, string, i) {
//        result += MAP_KEY(i) + c2 + MAP_VAL(i) + c1;
//    }
//
//    return result;
//}

string vformat(string format, va_list args) {
	char *pBuffer = NULL;
	if (vasprintf(&pBuffer, STR(format), args) == -1) {
		ASSERT("vasprintf failed");
		return "";
	}
	string result = pBuffer;
	free(pBuffer);
	return result;
}

string format(string format, ...) {
	string result = "";
	va_list arguments;
	va_start(arguments, format);
	result = vformat(format, arguments);
	va_end(arguments);
	return result;
}

void splitFileName(string fileName, string &name, string & extension, char separator) {
	size_t dotPosition = fileName.find_last_of(separator);
	if (dotPosition == string::npos) {
		name = fileName;
		extension = "";
		return;
	}
	name = fileName.substr(0, dotPosition);
	extension = fileName.substr(dotPosition + 1);
}

string generateRandomString(uint32_t length) {
	string result = "";
	for (uint32_t i = 0; i < length; i++)
		result += alowedCharacters[rand() % alowedCharacters.length()];
	return result;
}

string GetHostByName(string name) {
	struct hostent *pHostEnt = gethostbyname(STR(name));
	if (pHostEnt == NULL)
		return "";
	if (pHostEnt->h_length <= 0)
		return "";
	string result = format("%u.%u.%u.%u",
			(uint8_t) pHostEnt->h_addr_list[0][0],
			(uint8_t) pHostEnt->h_addr_list[0][1],
			(uint8_t) pHostEnt->h_addr_list[0][2],
			(uint8_t) pHostEnt->h_addr_list[0][3]);
	return result;
}

//THIS IS CODE TAKEN FROM libfetch

struct url * fetchParseURL(const char *URL) {
	char *doc;
	const char *p, *q;
	struct url *u;
	int i;

	/* allocate struct url */
	if ((u = (struct url *) calloc(1, sizeof (*u))) == NULL) {
		return (NULL);
	}

	/* scheme name */
	if ((p = strstr(URL, ":/"))) {
		snprintf(u->scheme, URL_SCHEMELEN + 1,
				"%.*s", (int) (p - URL), URL);
		URL = ++p;
		/*
		 * Only one slash: no host, leave slash as part of document
		 * Two slashes: host follows, strip slashes
		 */
		if (URL[1] == '/')
			URL = (p += 2);
	} else {
		p = URL;
	}
	if (!*URL || *URL == '/' || *URL == '.' ||
			(u->scheme[0] == '\0' &&
			strchr(URL, '/') == NULL && strchr(URL, ':') == NULL))
		goto nohost;

	p = strpbrk(URL, "/@");
	if (p && *p == '@') {
		/* username */
		for (q = URL, i = 0; (*q != ':') && (*q != '@'); q++)
			if (i < URL_USERLEN)
				u->user[i++] = *q;

		/* password */
		if (*q == ':')
			for (q++, i = 0; (*q != ':') && (*q != '@'); q++)
				if (i < URL_PWDLEN)
					u->pwd[i++] = *q;

		p++;
	} else {
		p = URL;
	}

	for (i = 0; *p && (*p != '/') && (*p != ':'); p++)
		if (i < MAXHOSTNAMELEN)
			u->host[i++] = *p;

	/* port */
	if (*p == ':') {
		for (q = ++p; *q && (*q != '/'); q++)
			if (isdigit(*q))
				u->port = u->port * 10 + (*q - '0');
			else {
				/* invalid port */
				goto ouch;
			}
		p = q;
	}

nohost:
	/* document */
	if (!*p)
		p = "/";

	if (strcasecmp(u->scheme, SCHEME_HTTP) == 0 ||
			strcasecmp(u->scheme, SCHEME_HTTPS) == 0) {
		const char hexnums[] = "0123456789abcdef";

		/* percent-escape whitespace. */
		if ((doc = (char *) malloc(strlen(p) * 3 + 1)) == NULL) {
			goto ouch;
		}
		u->doc = doc;
		while (*p != '\0') {
			if (!isspace(*p)) {
				*doc++ = *p++;
			} else {
				*doc++ = '%';
				*doc++ = hexnums[((unsigned int) * p) >> 4];
				*doc++ = hexnums[((unsigned int) * p) & 0xf];
				p++;
			}
		}
		*doc = '\0';
	} else if ((u->doc = strdup(p)) == NULL) {
		goto ouch;
	}

	//    DEBUG("scheme:   [%s]\n"
	//            "user:     [%s]\n"
	//            "password: [%s]\n"
	//            "host:     [%s]\n"
	//            "port:     [%d]\n"
	//            "document: [%s]\n",
	//            u->scheme, u->user, u->pwd,
	//            u->host, u->port, u->doc);

	return (u);

ouch:
	free(u);
	return (NULL);
}

//THIS IS CODE TAKEN FROM libfetch

void fetchFreeURL(struct url * u) {
	free(u->doc);
	free(u);
}

bool ParseURL(string stringUrl, string &host, uint16_t &port, string &user,
		string &pwd, string & doc) {
	host = user = pwd = doc = "";
	port = 0;
	trim(stringUrl);
	if (stringUrl == "")
		return false;

	bool result = true;

	struct url *pUrl = fetchParseURL(STR(stringUrl));
	if (pUrl == NULL) {
		FATAL("Invalid url: `%s`", STR(stringUrl));
		return false;
	}

	string scheme = pUrl->scheme;
	if ((scheme != string("http"))
			&& (scheme != string("https"))
			&& (scheme != string("rtsp"))
			&& (scheme != string("rtmp"))
			&& (scheme != string("rtmpe"))
			&& (scheme != string("rtmpt"))
			&& (scheme != string("rtmps"))
			&& (scheme != string("rtmpte"))) {
		FATAL("The only supported schemes are http or rtsp");
		result = false;
	}

	if (result && (string(pUrl->host) == "")) {
		FATAL("The host can't be empty");
		result = false;
	}

	if (result && ((pUrl->port < 0) || (pUrl->port >= 65536))) {
		FATAL("Invalid port");
		result = false;
	}

	if (result) {
		host = pUrl->host;
		if ((scheme == "http") || (scheme == "rtmpt") || (scheme == "rtmpte"))
			port = (pUrl->port == 0) ? (uint16_t) 80 : (uint16_t) pUrl->port;
		else if ((scheme == "https") || (scheme == "rtmps"))
			port = (pUrl->port == 0) ? (uint16_t) 443 : (uint16_t) pUrl->port;
		else if (scheme == "rtsp")
			port = (pUrl->port == 0) ? (uint16_t) 554 : (uint16_t) pUrl->port;
		else if ((scheme == "rtmp") || (scheme == "rtmpe"))
			port = (pUrl->port == 0) ? (uint16_t) 1935 : (uint16_t) pUrl->port;
		user = pUrl->user;
		pwd = pUrl->pwd;
		doc = pUrl->doc;
	}
	fetchFreeURL(pUrl);

	return result;
}

bool MoveFile(string src, string dst) {
	if (rename(STR(src), STR(dst)) != 0) {
		FATAL("Unable to move file from `%s` to `%s`",
				STR(src), STR(dst));
		return false;
	}
	return true;
}

bool DeleteFile(string path) {
	if (remove(STR(path)) != 0) {
		FATAL("Unable to delete file `%s`", STR(path));
		return false;
	}
	return true;
}


