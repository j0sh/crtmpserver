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


#ifndef _UDPPROTOCOL_H
#define	_UDPPROTOCOL_H

#include "protocols/baseprotocol.h"

class IOHandler;

class UDPProtocol
: public BaseProtocol {
private:
	IOHandler *_pCarrier;
	IOBuffer _inputBuffer;
	uint64_t _decodedBytesCount;
public:
	UDPProtocol();
	virtual ~UDPProtocol();
	virtual bool Initialize(Variant &parameters);
	virtual IOHandler *GetIOHandler();
	virtual void SetIOHandler(IOHandler *pIOHandler);
	virtual bool AllowFarProtocol(uint64_t type);
	virtual bool AllowNearProtocol(uint64_t type);
	virtual IOBuffer * GetInputBuffer();
	virtual bool SignalInputData(int32_t recvAmount);
	virtual bool SignalInputData(int32_t recvAmount, sockaddr_in *pPeerAddress);
	virtual bool SignalInputData(IOBuffer &buffer);
	virtual bool EnqueueForOutbound();
	virtual uint64_t GetDecodedBytesCount();
};


#endif	/* _UDPPROTOCOL_H */


