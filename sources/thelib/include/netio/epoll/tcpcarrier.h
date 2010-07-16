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

#ifdef NET_EPOLL
#ifndef _TCPCARRIER_H
#define	_TCPCARRIER_H


#include "netio/epoll/iohandler.h"

class BaseProtocol;

class TCPCarrier
: public IOHandler {
private:
	BaseProtocol *_pProtocol;
	bool _writeDataEnabled;
	sockaddr_in _farAddress;
	string _farIp;
	uint16_t _farPort;
	sockaddr_in _nearAddress;
	string _nearIp;
	uint16_t _nearPort;
	int32_t _sendBufferSize;
	int32_t _recvBufferSize;
public:
	TCPCarrier(int32_t fd, BaseProtocol *pProtocol);
	virtual ~TCPCarrier();
	void ResetProtocol();
	virtual bool OnEvent(struct epoll_event &event);
	virtual bool SignalOutputData();
	virtual operator string();

	sockaddr_in &GetFarEndpointAddress();
	string GetFarEndpointAddressIp();
	uint16_t GetFarEndpointPort();
	sockaddr_in &GetNearEndpointAddress();
	string GetNearEndpointAddressIp();
	uint16_t GetNearEndpointPort();
private:
	bool GetEndpointsInfo();
};


#endif	/* _TCPCARRIER_H */
#endif /* NET_EPOLL */


