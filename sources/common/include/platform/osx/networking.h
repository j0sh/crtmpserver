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

#ifndef _NETWORKING_H
#define	_NETWORKING_H

//1. Include necessary files
#include <AvailabilityMacros.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/event.h>
#include <netdb.h>
#include <netinet/tcp.h>

#ifdef MAC_OS_X_VERSION_10_6
#define HAS_KQUEUE_TIMERS
#endif

//2. define the no sigpipe
#define MSG_NOSIGNAL 0

#define SET_UNKNOWN 0
#define SET_READ 1
#define SET_WRITE 2
#define SET_TIMER 3

typedef struct _select_event {
    uint8_t type;
} select_event;

#define RESET_TIMER(timer,sec,usec)
#define FD_READ_CHUNK 32768
#define FD_WRITE_CHUNK FD_READ_CHUNK

#endif	/* _NETWORKING_H */
#endif	/* OSX */


