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

#ifndef _NETWORKING_H
#define	_NETWORKING_H

//1. Include necessary files
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define SO_NOSIGPIPE 0

#define SET_UNKNOWN 0
#define SET_READ 1
#define SET_WRITE 2
#define SET_TIMER 3

typedef struct _select_event {
    uint8_t type;
} select_event;

#define FD_COPY(src,dst) memcpy(dst,src,sizeof(fd_set));
#define RESET_TIMER(timer,sec,usec) timer.tv_sec=sec;timer.tv_usec=usec;

#define FD_READ_CHUNK 32768
#define FD_WRITE_CHUNK FD_READ_CHUNK
#define CLOSE_SOCKET(fd) close(fd)
#define LASTSOCKETERROR errno
#define SOCKERROR_CONNECT_IN_PROGRESS	EINPROGRESS
#define SOCKERROR_SEND_IN_PROGRESS		EAGAIN

#endif	/* _NETWORKING_H */
#endif	/* ANDROID */


