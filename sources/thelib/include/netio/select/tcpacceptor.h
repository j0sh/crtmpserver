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

#ifdef NET_SELECT
#ifndef _TCPACCEPTOR_H
#define	_TCPACCEPTOR_H


#include "netio/select/iohandler.h"
#include "protocols/baseprotocol.h"

class BaseClientApplication;

class DLLEXP TCPAcceptor
: public IOHandler {
private:
	sockaddr_in _address;
	vector<uint64_t> _protocolChain;
	BaseClientApplication *_pApplication;
	Variant _parameters;
	bool _enabled;
	uint32_t _acceptedCount;
	uint32_t _droppedCount;
	string _ipAddress;
	uint16_t _port;
public:
	TCPAcceptor(string ipAddress, uint16_t port, Variant parameters,
			vector<uint64_t>/*&*/ protocolChain);
	virtual ~TCPAcceptor();
	bool Bind();
	void SetApplication(BaseClientApplication *pApplication);
	bool StartAccept();
	virtual bool SignalOutputData();
	virtual bool OnEvent(select_event &event);
	virtual bool OnConnectionAvailable(select_event &event);
	bool Accept();
	Variant & GetParameters();
	BaseClientApplication *GetApplication();
	vector<uint64_t> &GetProtocolChain();
	virtual operator string();
	virtual void GetStats(Variant &info, uint32_t namespaceId = 0);
	bool Enable();
	void Enable(bool enabled);
private:
	bool IsAlive();
};


#endif	/* _TCPACCEPTOR_H */
#endif /* NET_SELECT */


