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


#ifdef SOLARIS

#include "platform/linux/misc.h"
#include "utils/core.h"

static map<int, SignalFnc> _signalHandlers;

void InitNetworking(){
}

string changecase(string &value, bool lowerCase) {
    string result = "";
    //std::locale locE("english");
    std::locale locE;
    for (string::size_type i = 0; i < value.length(); i++) {
        if (lowerCase)
            result += tolower(value[i], locE);
        else
            result += toupper(value[i], locE);
    }
    //FINEST("Value: %s; Result: %s", STR(value), STR(result));
    return result;
}

bool fileExists(string path) {
    struct stat fileInfo;

    if (STAT_FUNCTION(STR(path), &fileInfo) == 0) {
        return true;
    } else {
        return false;
    }
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
    return true;
}

bool ListFolder(string path, vector<string> &result) {
    DIR *pDir = NULL;
    pDir = opendir(STR(path));
    if (pDir == NULL) {
        FATAL("Unable to open folder: %s", STR(path));
        return false;
    }

    struct dirent *pDirent;
    while ((pDirent = readdir(pDir)) != NULL) {
        ADD_VECTOR_END(result, pDirent->d_name);
    }

    closedir(pDir);
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

static int is_leap(unsigned y)
{
    y += 1900;
    return (y % 4) == 0 && ((y % 100) != 0 || (y % 400) == 0);
}

time_t timegm (struct tm *tm)
{
  static const unsigned ndays[2][12] ={
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};
  time_t res = 0;
  int i;

  for (i = 70; i < tm->tm_year; ++i)
    res += is_leap(i) ? 366 : 365;

  for (i = 0; i < tm->tm_mon; ++i)
    res += ndays[is_leap(tm->tm_year)][i];
  res += tm->tm_mday - 1;
  res *= 24;
  res += tm->tm_hour;
  res *= 60;
  res += tm->tm_min;
  res *= 60;
  res += tm->tm_sec;
  return res;
}

double GetFileModificationDate(string path) {
    struct stat s;
    if (stat(STR(path), &s) != 0) {
        FATAL("Unable to stat file %s", STR(path));
        return 0;
    }
    return (double) s.st_mtime;
}

#endif /* SOLARIS */


