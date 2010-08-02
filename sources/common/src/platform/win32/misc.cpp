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


#ifdef WIN32

#include "utils/core.h"
#include "platform/win32/misc.h"
#include <sys/stat.h>

static map<uint32_t, SignalFnc> _signalHandlers;

int gettimeofday (struct timeval *tv, void* tz){
	FILETIME ft;
	GetSystemTimeAsFileTime (&ft);
	uint64_t value=((uint64_t)ft.dwHighDateTime<<32)|ft.dwLowDateTime;
	tv->tv_usec = (long) ((value / 10LL) % 1000000LL);
	tv->tv_sec = (long) ((value - 116444736000000000LL) / 10000000LL);
	return (0);
}

string changecase(string &value, bool lowerCase) {
    int32_t len = value.length();
    string newvalue(value);
    for (string::size_type i = 0, l = newvalue.length(); i < l; ++i)
        newvalue[i] = lowerCase ? tolower(newvalue[i]) : toupper(newvalue[i]);
    return newvalue;
}

int vasprintf(char **strp, const char *fmt, va_list ap, int size) {
    *strp = (char *) malloc(size);
    int result = 0;
    if ((result = vsnprintf(*strp, size, fmt, ap)) == -1) {
        free(*strp);
        return vasprintf(strp, fmt, ap, size + size / 2);
    } else {
        return result;
    }
}

bool fileExists(string path) {
    char *lpStr2;
    lpStr2 = (char *) path.c_str();

    if (PathFileExists(lpStr2))
        return true;
    else
        return false;
}

int inet_aton(const char *pStr, struct in_addr *pRes) {
    pRes->S_un.S_addr = inet_addr(pStr);
    return true;
}

HMODULE UnicodeLoadLibrary(string fileName) {
    return LoadLibrary(STR(fileName));
}

bool ListFolder(string path, vector<string> &result) {
    WIN32_FIND_DATA ffd;
    //LARGE_INTEGER filesize;
    TCHAR szDir[MAX_PATH];
    size_t length_of_arg;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError = 0;

    // Check that the input path plus 3 is not longer than MAX_PATH.
    // Three characters are for the "\*" plus NULL appended below.

    StringCchLength(path.c_str(), MAX_PATH, &length_of_arg);

    if (length_of_arg > (MAX_PATH - 3)) {
        WARN("Directory path is too long: %s.", path.c_str());
        return false;
    }

    FINEST("Target directory: %s", path.c_str());

    // Prepare string for use with FindFile functions.  First, copy the
    // string to a buffer, then append '\*' to the directory name.

    StringCchCopy(szDir, MAX_PATH, path.c_str());
    StringCchCat(szDir, MAX_PATH, TEXT("\\*.*"));

    // Find the first file in the directory.

    hFind = FindFirstFile(szDir, &ffd);

    if (INVALID_HANDLE_VALUE == hFind) {
        WARN("Invalid handle for FindFirstFile");
        return false;
    }

    do {
        if (lstrcmp(ffd.cFileName, TEXT(".")) == 0 || lstrcmp(ffd.cFileName, TEXT("..")) == 0)
            continue;

        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            WARN("Subdirectorylisting not implemented");
        } else {
            //filesize.LowPart = ffd.nFileSizeLow;
            //filesize.HighPart = ffd.nFileSizeHigh;
            result.push_back(ffd.cFileName);
        }
    } while (FindNextFile(hFind, &ffd) != 0);

    dwError = GetLastError();
    if (dwError != ERROR_NO_MORE_FILES) {
        WARN("Unable to find first file");
        return false;
    }
    FindClose(hFind);
    return true;
}

bool SetFdNonBlock(int32_t fd) {
    // int32_t arg;
    u_long iMode = 1; // 0 for blocking, anything else for nonblocking

    if (ioctlsocket(fd, FIONBIO, &iMode) < 0) {
        int32_t err = errno;
        FATAL("Unable to set fd flags: %d,%s", err, strerror(err));
        return false;
    }
    return true;
}

bool SetFdNoSIGPIPE(int32_t fd) {
   NYI;
   return true;
}

int32_t getpid() {
    return (int32_t) GetCurrentProcessId();
}

string normalizePath(string base, string file){
	char dummy1[MAX_PATH ];
    char dummy2[MAX_PATH ];
	if(GetFullPathName(STR(base),MAX_PATH,dummy1,NULL)==0)
		return "";
	if(GetFullPathName(STR(base + file),MAX_PATH,dummy2,NULL)==0)
		return "";
	
	base = dummy1;
	file = dummy2;

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

bool HandlerRoutine(uint32_t dwCtrlType){
	if(MAP_HAS1(_signalHandlers,dwCtrlType)){
		_signalHandlers[dwCtrlType]();
		return true;
	}
	return false;
}

void InstallConfRereadSignal(SignalFnc pConfRereadSignalFnc) {
    _signalHandlers[CTRL_BREAK_EVENT]=pConfRereadSignalFnc;
    SetConsoleCtrlHandler((PHANDLER_ROUTINE) HandlerRoutine, TRUE);
} 

void InstallQuitSignal(SignalFnc pQuitSignalFnc) {
    _signalHandlers[CTRL_C_EVENT]=pQuitSignalFnc;
    SetConsoleCtrlHandler((PHANDLER_ROUTINE) HandlerRoutine, TRUE);
}

double GetFileModificationDate(string path){
	struct _stat64 s;
    if (_stat64(STR(path), &s) != 0) {
        FATAL("Unable to stat file %s", STR(path));
        return 0;
    }
	return (double) s.st_mtime;
}

void InitNetworking(){
	WSADATA wsa={0};
	WSAStartup(0,&wsa);
	WSAStartup(wsa.wHighVersion,&wsa);
}

#endif /* WIN32 */


