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

#ifndef _MISC_H
#define	_MISC_H

#define FD_SETSIZE 1024
#include <WinSock2.h> // Added by alaknas@gmail.com (should be in networking.h ..)
#include <mswsock.h>
#include <Ws2tcpip.h> // Ditto
#include "Shlwapi.h"
#include <strsafe.h>
#include <windows.h>
#include <assert.h>
#include <io.h>
#include <time.h>
#include "platform/win32/stl.h"
#include "platform/win32/types.h"

#define DLLEXP __declspec(dllexport)
//#define HAS_MMAP

#define __func__ __FUNCTION__

#define COLOR_TYPE WORD
#define FATAL_COLOR FOREGROUND_INTENSITY | FOREGROUND_RED
#define ERROR_COLOR FOREGROUND_RED
#define WARNING_COLOR 6
#define INFO_COLOR FOREGROUND_GREEN | FOREGROUND_BLUE
#define DEBUG_COLOR 7
#define FINE_COLOR 8
#define FINEST_COLOR 8
#define SET_CONSOLE_TEXT_COLOR(color) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color)

#define MAXHOSTNAMELEN 256
#define snprintf _snprintf
#define PATH_SEPARATOR '\\'
#define LIBRARY_NAME_PATTERN "%s.dll"
#define LIB_HANDLER HMODULE
#define LOAD_LIBRARY(file,flags) UnicodeLoadLibrary((file))
#define OPEN_LIBRARY_ERROR "OPEN_LIBRARY_ERROR NOT IMPLEMENTED YET"
#define GET_PROC_ADDRESS(libHandler, procName) GetProcAddress((libHandler), (procName))
#define FREE_LIBRARY(libHandler) FreeLibrary((libHandler))
#define READ_FD _read
#define WRITE_FD _write


#define DECLARE_EXTERROR int lastError = WSAGetLastError();
#define PUTTOFD_SENDAGAIN (lastError == ERROR_IO_PENDING || lastError == WSAEWOULDBLOCK || err == EAGAIN)

#define gmtime_r(_p_time_t, _p_struct_tm) *(_p_struct_tm) = *gmtime(_p_time_t);

DLLEXP string changecase(string &value, bool lowerCase);
DLLEXP int vasprintf(char **strp, const char *fmt, va_list ap, int size = 256);
DLLEXP bool fileExists(string path);
DLLEXP bool SetFdNonBlock(int32_t fd);
DLLEXP bool SetFdNoSIGPIPE(int32_t fd);
DLLEXP int inet_aton(const char *pStr, struct in_addr *pRes);
DLLEXP HMODULE UnicodeLoadLibrary(string fileName);
DLLEXP bool ListFolder(string path, vector<string> &result);
DLLEXP int32_t getpid();
DLLEXP string normalizePath(string base, string file);
DLLEXP time_t timegm(struct tm *tm);
DLLEXP int strcasecmp(const char *s1, const char *s2);
DLLEXP int strncasecmp(const char *s1, const char *s2, size_t n);
DLLEXP char *strptime (const char *buf, const char *format, struct tm *timeptr);


#define GETCLOCKS(result) result=GetTickCount()
#define CLOCKS_PER_SECOND 1000


#endif	/* _MISC_H */
#endif	/* WIN32 */


