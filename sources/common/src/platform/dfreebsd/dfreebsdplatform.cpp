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

#ifdef DFREEBSD
#include "common.h"

string alowedCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
static map<int, SignalFnc> _signalHandlers;

DFreeBSDPlatform::DFreeBSDPlatform() {

}

DFreeBSDPlatform::~DFreeBSDPlatform() {
}

string format(string fmt, ...) {
	string result = "";
	va_list arguments;
	va_start(arguments, fmt);
	result = vFormat(fmt, arguments);
	va_end(arguments);
	return result;
}

string vFormat(string fmt, va_list args) {
	char *pBuffer = NULL;
	if (vasprintf(&pBuffer, STR(fmt), args) == -1) {
		assert(false);
		return "";
	}
	string result = pBuffer;
	free(pBuffer);
	return result;
}

void replace(string &target, string search, string replacement) {
	if (search == replacement)
		return;
	if (search == "")
		return;
	string::size_type i = string::npos;
	string::size_type lastPos = 0;
	while ((i = target.find(search, lastPos)) != string::npos) {
		target.replace(i, search.length(), replacement);
		lastPos = i + replacement.length();
	}
}

bool fileExists(string path) {
	struct stat fileInfo;
	if (stat(STR(path), &fileInfo) == 0) {
		return true;
	} else {
		return false;
	}
}

string lowerCase(string value) {
	return changeCase(value, true);
}

string upperCase(string value) {
	return changeCase(value, false);
}

string changeCase(string &value, bool lowerCase) {
	string result = "";
	for (string::size_type i = 0; i < value.length(); i++) {
		if (lowerCase)
			result += tolower(value[i]);
		else
			result += toupper(value[i]);
	}
	return result;
}

string tagToString(uint64_t tag) {
	string result;
	for (uint32_t i = 0; i < 8; i++) {
		uint8_t v = (tag >> ((7 - i)*8)&0xff);
		if (v == 0)
			break;
		result += (char) v;
	}
	return result;
}

bool setFdNonBlock(int32_t fd) {
	int32_t arg;
	if ((arg = fcntl(fd, F_GETFL, NULL)) < 0) {
		int32_t err = errno;
		FATAL("Unable to get fd flags: %d,%s", err, strerror(err));
		return false;
	}
	arg |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, arg) < 0) {
		int32_t err = errno;
		FATAL("Unable to set fd flags: %d,%s", err, strerror(err));
		return false;
	}

	return true;
}

bool setFdNoSIGPIPE(int32_t fd) {
	//This is not needed because we use MSG_NOSIGNAL when using
	//send/write functions
	return true;
}

bool setFdKeepAlive(int32_t fd) {
	int32_t one = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE,
			(const char*) & one, sizeof (one)) != 0) {
		FATAL("Unable to set SO_NOSIGPIPE");
		return false;
	}
	return true;
}

bool setFdNoNagle(int32_t fd) {
	int32_t one = 1;
	if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *) & one, sizeof (one)) != 0) {
		return false;
	}
	return true;
}

bool setFdReuseAddress(int32_t fd) {
	int32_t one = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *) & one, sizeof (one)) != 0) {
		FATAL("Unable to reuse address");
		return false;
	}
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, (char *) & one, sizeof (one)) != 0) {
		FATAL("Unable to reuse port");
		return false;
	}
	return true;
}

bool setFdTTL(int32_t fd, uint8_t ttl) {
	int temp = ttl;
	if (setsockopt(fd, IPPROTO_IP, IP_TTL, &temp, sizeof (temp)) != 0) {
		int err = errno;
		WARN("Unable to set IP_TTL: %"PRIu8"; error was %"PRId32" %s", ttl, err, strerror(err));
	}
	return true;
}

bool setFdMulticastTTL(int32_t fd, uint8_t ttl) {
	int temp = ttl;
	if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL, &temp, sizeof (temp)) != 0) {
		int err = errno;
		WARN("Unable to set IP_MULTICAST_TTL: %"PRIu8"; error was %"PRId32" %s", ttl, err, strerror(err));
	}
	return true;
}

bool setFdTOS(int32_t fd, uint8_t tos) {
	int temp = tos;
	if (setsockopt(fd, IPPROTO_IP, IP_TOS, &temp, sizeof (temp)) != 0) {
		int err = errno;
		WARN("Unable to set IP_TOS: %"PRIu8"; error was %"PRId32" %s", tos, err, strerror(err));
	}
	return true;
}

bool setFdOptions(int32_t fd) {
	if (!setFdNonBlock(fd)) {
		FATAL("Unable to set non block");
		return false;
	}

	if (!setFdNoSIGPIPE(fd)) {
		FATAL("Unable to set no SIGPIPE");
		return false;
	}

	if (!setFdKeepAlive(fd)) {
		FATAL("Unable to set keep alive");
		return false;
	}

	if (!setFdNoNagle(fd)) {
		WARN("Unable to disable Nagle algorithm");
	}

	if (!setFdReuseAddress(fd)) {
		FATAL("Unable to enable reuse address");
		return false;
	}

	return true;
}

bool deleteFile(string path) {
	if (remove(STR(path)) != 0) {
		FATAL("Unable to delete file `%s`", STR(path));
		return false;
	}
	return true;
}

bool deleteFolder(string path, bool force) {
	if (!force) {
		return deleteFile(path);
	} else {
		string command = format("rm -rf %s", STR(path));
		if (system(STR(command)) != 0) {
			FATAL("Unable to delete folder %s", STR(path));
			return false;
		}
		return true;
	}
}

string getHostByName(string name) {
	struct hostent *pHostEnt = gethostbyname(STR(name));
	if (pHostEnt == NULL)
		return "";
	if (pHostEnt->h_length <= 0)
		return "";
	string result = format("%hhu.%hhu.%hhu.%hhu",
			(uint8_t) pHostEnt->h_addr_list[0][0],
			(uint8_t) pHostEnt->h_addr_list[0][1],
			(uint8_t) pHostEnt->h_addr_list[0][2],
			(uint8_t) pHostEnt->h_addr_list[0][3]);
	return result;
}

bool isNumeric(string value) {
	//FINEST("value: `%s`", STR(value));
	return value == format("%d", atoi(STR(value)));
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

uint64_t getTagMask(uint64_t tag) {
	uint64_t result = 0xffffffffffffffffLL;
	for (int8_t i = 56; i >= 0; i -= 8) {
		if (((tag >> i)&0xff) == 0)
			break;
		result = result >> 8;
	}
	return ~result;
}

string generateRandomString(uint32_t length) {
	string result = "";
	for (uint32_t i = 0; i < length; i++)
		result += alowedCharacters[rand() % alowedCharacters.length()];
	return result;
}

void lTrim(string &value) {
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

void rTrim(string &value) {
	int32_t i = 0;
	for (i = (int32_t) value.length() - 1; i >= 0; i--) {
		if (value[i] != ' ' &&
				value[i] != '\t' &&
				value[i] != '\n' &&
				value[i] != '\r')
			break;
	}
	value = value.substr(0, i + 1);
}

void trim(string &value) {
	lTrim(value);
	rTrim(value);
}

int8_t getCPUCount() {
	NYI;
	return 0;
}

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

double getFileModificationDate(string path) {
	struct stat s;
	if (stat(STR(path), &s) != 0) {
		FATAL("Unable to stat file %s", STR(path));
		return 0;
	}
	return (double) s.st_mtime;
}

string normalizePath(string base, string file) {
	char dummy1[PATH_MAX];
	char dummy2[PATH_MAX];
	char *pBase = realpath(STR(base), dummy1);
	char *pFile = realpath(STR(base + file), dummy2);

	if (pBase != NULL) {
		base = pBase;
	} else {
		base = "";
	}

	if (pFile != NULL) {
		file = pFile;
	} else {
		file = "";
	}

	if (file == "" || base == "") {
		return "";
	}

	if (file.find(base) != 0) {
		return "";
	} else {
		if (!fileExists(file)) {
			return "";
		} else {
			return file;
		}
	}
}

bool listFolder(string path, vector<string> &result, bool normalizeAllPaths,
		bool includeFolders, bool recursive) {
	if (path == "")
		path = ".";
	if (path[path.size() - 1] != PATH_SEPARATOR)
		path += PATH_SEPARATOR;

	DIR *pDir = NULL;
	pDir = opendir(STR(path));
	if (pDir == NULL) {
		int err = errno;
		FATAL("Unable to open folder: %s %d %s", STR(path), err, strerror(err));
		return false;
	}

	struct dirent *pDirent = NULL;
	while ((pDirent = readdir(pDir)) != NULL) {
		string entry = pDirent->d_name;
		if ((entry == ".")
				|| (entry == "..")) {
			continue;
		}
		if (normalizeAllPaths) {
			entry = normalizePath(path, entry);
		} else {
			entry = path + entry;
		}
		if (entry == "")
			continue;

		if (pDirent->d_type == DT_DIR) {
			if (includeFolders) {
				ADD_VECTOR_END(result, entry);
			}
			if (recursive) {
				if (!listFolder(entry, result, normalizeAllPaths, includeFolders, recursive)) {
					FATAL("Unable to list folder");
					closedir(pDir);
					return false;
				}
			}
		} else {
			ADD_VECTOR_END(result, entry);
		}
	}

	closedir(pDir);
	return true;
}

bool moveFile(string src, string dst) {
	if (rename(STR(src), STR(dst)) != 0) {
		FATAL("Unable to move file from `%s` to `%s`",
				STR(src), STR(dst));
		return false;
	}
	return true;
}

void signalHandler(int sig) {
	if (!MAP_HAS1(_signalHandlers, sig))
		return;
	_signalHandlers[sig]();
}

void installSignal(int sig, SignalFnc pSignalFnc) {
	_signalHandlers[sig] = pSignalFnc;
	struct sigaction action;
	action.sa_handler = signalHandler;
	action.sa_flags = 0;
	if (sigemptyset(&action.sa_mask) != 0) {
		ASSERT("Unable to install the quit signal");
		return;
	}
	if (sigaction(sig, &action, NULL) != 0) {
		ASSERT("Unable to install the quit signal");
		return;
	}
}

void installQuitSignal(SignalFnc pQuitSignalFnc) {
	installSignal(SIGINT, pQuitSignalFnc);
}

void installConfRereadSignal(SignalFnc pConfRereadSignalFnc) {
	installSignal(SIGHUP, pConfRereadSignalFnc);
}


#endif /* FREEBSD */

