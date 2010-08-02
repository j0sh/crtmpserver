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
#include "platform/win32/networking.h"

int sendmsg(int s, const struct msghdr *msg, int flags){
	int result=0;
	int sentChunk=0;
	for(int i=0;i<msg->msg_iovlen;i++){
		sentChunk=sendto(s,
			(char *)msg->msg_iov[i].iov_base,
			msg->msg_iov[i].iov_len,
			flags,
			(sockaddr *)msg->msg_name,
			msg->msg_namelen);
		if(sentChunk==SOCKET_ERROR)
			return SOCKET_ERROR;
		result+=sentChunk;
		if(sentChunk!=msg->msg_iov[i].iov_len)
			return result;
	}
	return result;
}

#endif /* WIN32 */
