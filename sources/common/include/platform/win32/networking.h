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

#ifndef _NETWORKING_H
#define	_NETWORKING_H

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
#define CLOSE_SOCKET(fd) closesocket(fd)
#define LASTSOCKETERROR WSAGetLastError()
#define SOCKERROR_CONNECT_IN_PROGRESS	WSAEWOULDBLOCK
#define SOCKERROR_SEND_IN_PROGRESS		WSAEWOULDBLOCK

 struct iovec {
	 void	*iov_base;	/* Base address. */
	 size_t	iov_len;	/* Length. */
 };

struct msghdr {
	void			*msg_name;		/* optional address */
	int				msg_namelen;	/* size of address */
	struct iovec	*msg_iov;		/* scatter/gather array */
	int				msg_iovlen;		/* # elements in msg_iov */
	void			*msg_control;	/* ancillary data, see below */
	int				msg_controllen;	/* ancillary data buffer len */
	int				msg_flags;		/* flags on received message */
};

#define FD_COPY(f, t)   (void)(*(t) = *(f))
DLLEXP int sendmsg(int s, const struct msghdr *msg, int flags);

#endif	/* _NETWORKING_H */
#endif	/* WIN32 */


