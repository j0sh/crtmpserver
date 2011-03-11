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

#ifdef HAS_PROTOCOL_DNS
#ifndef _OUTBOUNDDNSRESOLVERPROTOCOL_H
#define	_OUTBOUNDDNSRESOLVERPROTOCOL_H

#include "protocols/baseprotocol.h"

class DLLEXP OutboundDNSResolverProtocol
: public BaseProtocol {
private:
	IOBuffer _outputBuffer;
	static uint32_t _requestIdGenerator;
	static uint32_t _dnsProtocolResolverId;
	static map<uint32_t, Variant> _requests;
public:
	OutboundDNSResolverProtocol();
	virtual ~OutboundDNSResolverProtocol();

	static bool Connect(string ip, uint16_t port, Variant customParameters);
	static bool SignalProtocolCreated(BaseProtocol *pProtocol, Variant customParameters);

	static bool Resolve(string host, uint32_t consumerProtocolId, Variant customData);

	virtual bool Initialize(Variant &parameters);
	virtual bool AllowFarProtocol(uint64_t type);
	virtual bool AllowNearProtocol(uint64_t type);
	virtual IOBuffer * GetOutputBuffer();
	virtual bool SignalInputData(int32_t recvAmount);
	virtual bool SignalInputData(IOBuffer &buffer);
private:
	bool SendRequest(Variant &request);
	bool HandleResponse(Variant &response);
};


#endif	/* _OUTBOUNDDNSRESOLVERPROTOCOL_H */
#endif /* HAS_PROTOCOL_DNS */

