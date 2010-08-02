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


#ifdef ANDROID

#ifndef _MISC_H
#define	_MISC_H

//1. Include necessary files
#include <sys/mman.h>
#include <dlfcn.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/time.h>
#include <assert.h>
#include <signal.h>
#include <sys/param.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "platform/android/stl.h"

#define DLLEXP

#define HAS_MMAP

//2. Glue defines
#define STAT_FUNCTION stat
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

#define PATH_SEPARATOR '/'
#define LIB_HANDLER void *
#define LOAD_LIBRARY(file,flags) dlopen((file), (flags))
#define LOAD_LIBRARY_FLAGS RTLD_NOW | RTLD_LOCAL
#define OPEN_LIBRARY_ERROR STR(string(dlerror()))
#define GET_PROC_ADDRESS(libHandler, procName) dlsym((libHandler), (procName))
#define FREE_LIBRARY(libHandler) dlclose((libHandler))
#define LIBRARY_NAME_PATTERN "lib%s.so"
#define safe_strerror strerror
#define CLOCKS_PER_SECOND 1000000L
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
#define KQUEUE_WAIT_TIME_TYPE struct timespec *
/*#define INIT_ONE_SECOND_TIME(x) \
{ \
	x=new struct timespec; \
	x->tv_sec=KQUEUE_WAIT_TIME; \
	x->tv_nsec=0; \
}*/

#define MAP_NOCACHE 0
#define MAP_NOEXTEND 0

#define DECLARE_EXTERROR
#define READ_FD read
#define WRITE_FD write

//3. Functions
string changecase(string &value, bool lowerCase);
bool fileExists(string path);
string normalizePath(string base, string file);
bool SetFdNonBlock(int32_t fd);
bool SetFdNoSIGPIPE(int32_t fd);
bool ListFolder(std::string path, std::vector<std::string> &result);
time_t timegm(struct tm *tm);

#endif	/* _MISC_H */
#endif	/* ANDROID */


