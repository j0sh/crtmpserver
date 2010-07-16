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

#ifndef _RTCPPROTOCOL_H
#define	_RTCPPROTOCOL_H

#include "protocols/baseprotocol.h"

class InboundConnectivity;

class RTCPProtocol
: public BaseProtocol {
private:
	InboundConnectivity *_pConnectivity;
	uint32_t _lsr;
	uint8_t _buff[32];
	sockaddr_in _dummy;
public:
	RTCPProtocol();
	virtual ~RTCPProtocol();

	virtual bool Initialize(Variant &parameters);
	virtual bool AllowFarProtocol(uint64_t type);
	virtual bool AllowNearProtocol(uint64_t type);
	virtual bool SignalInputData(int32_t recvAmount);
	virtual bool SignalInputData(IOBuffer &buffer, sockaddr_in *pPeerAddress);
	virtual bool SignalInputData(IOBuffer &buffer);

	bool SendRR(sockaddr_in &address);

	void SetInbboundConnectivity(InboundConnectivity *pConnectivity);
};


#endif	/* _RTCPPROTOCOL_H */

