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


#ifdef NET_IOCP2

#include "utils/core.h"
#include "netio/iocp2/stdiocarrier.h"

StdioCarrier *StdioCarrier::_pInstance = NULL;

StdioCarrier::StdioCarrier(BaseProtocol *pProtocol)
: IOHandler(_fileno(stdin), _fileno(stdout), IOHT_STDIO) {
	ASSERT("StdioCarrier not yet supported on win32");
}

StdioCarrier *StdioCarrier::GetInstance(BaseProtocol *pProtocol){
	ASSERT("StdioCarrier not yet supported on win32");
	return NULL;
}

StdioCarrier::~StdioCarrier(){
	ASSERT("StdioCarrier not yet supported on win32");
}

void StdioCarrier::ResetProtocol(){
	ASSERT("StdioCarrier not yet supported on win32");
}

bool StdioCarrier::OnEvent(struct kevent &event){
	ASSERT("StdioCarrier not yet supported on win32");
	return false;
}

bool StdioCarrier::SignalOutputData(){
	ASSERT("StdioCarrier not yet supported on win32");
	return false;
}

StdioCarrier::operator string(){
	ASSERT("StdioCarrier not yet supported on win32");
	return "";
}

#endif /* NET_IOCP2 */

