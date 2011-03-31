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

#ifdef LINUX

#include "platform/linux/linuxplatform.h"
#include "common.h"

string alowedCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
static map<int, SignalFnc> _signalHandlers;

LinuxPlatform::LinuxPlatform() {

}

LinuxPlatform::~LinuxPlatform() {
}

string format(string fmt, ...) {
	string result = "";
	va_list arguments;
	va_start(arguments, fmt);
	result = vformat(fmt, arguments);
	va_end(arguments);
	return result;
}

string vformat(string fmt, va_list args) {
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

string lowercase(string value) {
	return changecase(value, true);
}

string uppercase(string value) {
	return changecase(value, false);
}

string changecase(string &value, bool lowerCase) {
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

bool SetFdNonBlock(int32_t fd) {
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

bool SetFdNoSIGPIPE(int32_t fd) {
	//This is not needed because we use MSG_NOSIGNAL when using
	//send/write functions
	return true;
}

bool SetFdKeepAlive(int32_t fd) {
	int32_t one = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE,
			(const char*) & one, sizeof (one)) != 0) {
		FATAL("Unable to set SO_NOSIGPIPE");
		return false;
	}
	return true;
}

bool SetFdNoNagle(int32_t fd) {
	int32_t one = 1;
	if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *) & one, sizeof (one)) != 0) {
		FATAL("Unable to disable Nagle");
		return false;
	}
	return true;
}

bool SetFdReuseAddress(int32_t fd) {
	int32_t one = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *) & one, sizeof (one)) != 0) {
		FATAL("Unable to reuse address");
		return false;
	}
#ifdef SO_REUSEPORT
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, (char *) & one, sizeof (one)) != 0) {
		FATAL("Unable to reuse port");
		return false;
	}
#endif /* SO_REUSEPORT */
	return true;
}

bool SetFdOptions(int32_t fd) {
	if (!SetFdNonBlock(fd)) {
		FATAL("Unable to set non block");
		return false;
	}

	if (!SetFdNoSIGPIPE(fd)) {
		FATAL("Unable to set no SIGPIPE");
		return false;
	}

	if (!SetFdKeepAlive(fd)) {
		FATAL("Unable to set keep alive");
		return false;
	}

	if (!SetFdNoNagle(fd)) {
		FATAL("Unable to disable Nagle algorithm");
		return false;
	}

	if (!SetFdReuseAddress(fd)) {
		FATAL("Unable to enable reuse address");
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

string GetHostByName(string name) {
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
	ltrim(value);
	rtrim(value);
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

double GetFileModificationDate(string path) {
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

bool ListFolder(string root, string path, vector<string> &result) {
	if (path == "")
		return true;
	if (path[path.size() - 1] == '/')
		path = path.substr(0, path.size() - 1);
	DIR *pDir = NULL;
	pDir = opendir(STR(path));
	if (pDir == NULL) {
		int err = errno;
		FATAL("Unable to open folder: %s %d %s", STR(path), err, strerror(err));
		return false;
	}

	struct dirent *pDirent;
	struct stat entryStat;
	memset(&entryStat, 0, sizeof (entryStat));
	while ((pDirent = readdir(pDir)) != NULL) {
		string entry = pDirent->d_name;
		if (entry == "." || entry == "..") {
			continue;
		}
		entry = path + "/" + entry;
		if (stat(STR(entry), &entryStat) != 0) {
			FINEST("%d %s", errno, strerror(errno));
			continue;
		}
		ADD_VECTOR_END(result, entry.substr(root.size(), -1));
		if (entryStat.st_mode & S_IFDIR) {
			ListFolder(root, entry, result);
		}
	}

	closedir(pDir);
	return true;
}

bool MoveFile(string src, string dst) {
	if (rename(STR(src), STR(dst)) != 0) {
		FATAL("Unable to move file from `%s` to `%s`",
				STR(src), STR(dst));
		return false;
	}
	return true;
}

void SignalHandler(int sig) {
	if (!MAP_HAS1(_signalHandlers, sig))
		return;
	_signalHandlers[sig]();
}

void InstallSignal(int sig, SignalFnc pSignalFnc) {
	_signalHandlers[sig] = pSignalFnc;
	struct sigaction action;
	action.sa_handler = SignalHandler;
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

void InstallQuitSignal(SignalFnc pQuitSignalFnc) {
	InstallSignal(SIGINT, pQuitSignalFnc);
}

void InstallConfRereadSignal(SignalFnc pConfRereadSignalFnc) {
	InstallSignal(SIGHUP, pConfRereadSignalFnc);
}

#endif /* LINUX */

