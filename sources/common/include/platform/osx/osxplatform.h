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

#ifdef OSX
#ifndef _OSXPLATFORM_H
#define _OSXPLATFORM_H

#include "platform/baseplatform.h"

#ifndef PRIz
#define PRIz "z"
#endif /* PRIz */

//platform includes
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <AvailabilityMacros.h>
#include <algorithm>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <assert.h>
#include <cctype>
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <glob.h>
#include <iostream>
#include <list>
#include <map>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <sstream>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <sys/event.h>
#include <sys/mman.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <vector>
using namespace std;

//platform defines
#define DLLEXP
#define HAS_MMAP 1
#define COLOR_TYPE string
#define FATAL_COLOR "\033[01;31m"
#define ERROR_COLOR "\033[22;31m"
#define WARNING_COLOR "\033[01;33m"
#define INFO_COLOR "\033[22;36m"
#define DEBUG_COLOR "\033[01;37m"
#define FINE_COLOR "\033[22;37m"
#define FINEST_COLOR "\033[22;37m"
#define NORMAL_COLOR "\033[0m"
#define SET_CONSOLE_TEXT_COLOR(color) cout<<color
#define MSG_NOSIGNAL 0
#define READ_FD read
#define WRITE_FD write
#define LASTSOCKETERROR					errno
#define SOCKERROR_CONNECT_IN_PROGRESS	EINPROGRESS
#define SOCKERROR_SEND_IN_PROGRESS		EAGAIN
#define LIB_HANDLER void *
#define FREE_LIBRARY(libHandler) dlclose((libHandler))
#define LOAD_LIBRARY(file,flags) dlopen((file), (flags))
#define LOAD_LIBRARY_FLAGS RTLD_NOW | RTLD_LOCAL
#define OPEN_LIBRARY_ERROR STR(string(dlerror()))
#define GET_PROC_ADDRESS(libHandler, procName) dlsym((libHandler), (procName))
#define LIBRARY_NAME_PATTERN "lib%s.dylib"
#define PATH_SEPARATOR '/'
#define CLOSE_SOCKET(fd) if((fd)>=0) close((fd))
#define InitNetworking()
#ifdef MAC_OS_X_VERSION_10_6
#define HAS_KQUEUE_TIMERS
#define KQUEUE_TIMER_MULTIPLIER 1000000
#endif
#define SET_UNKNOWN 0
#define SET_READ 1
#define SET_WRITE 2
#define SET_TIMER 3
#define FD_READ_CHUNK 32768
#define FD_WRITE_CHUNK FD_READ_CHUNK
#define RESET_TIMER(timer,sec,usec) timer.tv_sec=sec;timer.tv_usec=usec;
#define SRAND() sranddev();
#define Timestamp struct tm
#define Timestamp_init {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

#define CLOCKS_PER_SECOND CLOCKS_PER_SEC
#define GETCLOCKS(result) \
do { \
    struct timeval ___timer___; \
    gettimeofday(&___timer___,NULL); \
    result=(double)___timer___.tv_sec*(double)CLOCKS_PER_SECOND+(double) ___timer___.tv_usec; \
}while(0);

#define GETNTP(result) \
do { \
	struct timeval tv; \
	gettimeofday(&tv,NULL); \
	result=(((uint64_t)tv.tv_sec + 2208988800U)<<32)|((((uint32_t)tv.tv_usec) << 12) + (((uint32_t)tv.tv_usec) << 8) - ((((uint32_t)tv.tv_usec) * 1825) >> 5)); \
}while (0);

#define GETCUSTOMNTP(result,value) \
do { \
	struct timeval tv; \
	tv.tv_sec=value/CLOCKS_PER_SECOND; \
	tv.tv_usec=value-tv.tv_sec*CLOCKS_PER_SECOND; \
	result=(((uint64_t)tv.tv_sec + 2208988800U)<<32)|((((uint32_t)tv.tv_usec) << 12) + (((uint32_t)tv.tv_usec) << 8) - ((((uint32_t)tv.tv_usec) * 1825) >> 5)); \
}while (0);

class OSXPlatform
: public BasePlatform {
public:
	OSXPlatform();
	virtual ~OSXPlatform();
};

typedef void (*SignalFnc)(void);

typedef struct _select_event {
	uint8_t type;
} select_event;

#define MSGHDR struct msghdr
#define IOVEC iovec
#define MSGHDR_MSG_IOV msg_iov
#define MSGHDR_MSG_IOVLEN msg_iovlen
#define MSGHDR_MSG_NAME msg_name
#define MSGHDR_MSG_NAMELEN msg_namelen
#define IOVEC_IOV_BASE iov_base
#define IOVEC_IOV_LEN iov_len
#define IOVEC_IOV_BASE_TYPE uint8_t
#define SENDMSG(s,msg,flags,sent) sendmsg(s,msg,flags)

string format(string fmt, ...);
string vFormat(string fmt, va_list args);
void replace(string &target, string search, string replacement);
bool fileExists(string path);
string lowerCase(string value);
string upperCase(string value);
string changeCase(string &value, bool lowerCase);
string tagToString(uint64_t tag);
bool setFdNonBlock(int32_t fd);
bool setFdNoSIGPIPE(int32_t fd);
bool setFdKeepAlive(int32_t fd);
bool setFdNoNagle(int32_t fd);
bool setFdReuseAddress(int32_t fd);
bool setFdTTL(int32_t fd, uint8_t ttl);
bool setFdMulticastTTL(int32_t fd, uint8_t ttl);
bool setFdTOS(int32_t fd, uint8_t tos);
bool setFdOptions(int32_t fd);
bool deleteFile(string path);
bool deleteFolder(string path, bool force);
bool createFolder(string path, bool recursive);
string getHostByName(string name);
bool isNumeric(string value);
void split(string str, string separator, vector<string> &result);
uint64_t getTagMask(uint64_t tag);
string generateRandomString(uint32_t length);
void lTrim(string &value);
void rTrim(string &value);
void trim(string &value);
int8_t getCPUCount();
map<string, string> mapping(string str, string separator1, string separator2, bool trimStrings);
void splitFileName(string fileName, string &name, string &extension, char separator = '.');
double getFileModificationDate(string path);
string normalizePath(string base, string file);
bool listFolder(string path, vector<string> &result,
		bool normalizeAllPaths = true, bool includeFolders = false,
		bool recursive = true);
bool moveFile(string src, string dst);
void installQuitSignal(SignalFnc pQuitSignalFnc);
void installConfRereadSignal(SignalFnc pConfRereadSignalFnc);


#endif /* _OSXPLATFORM_H */
#endif /* OSX */
