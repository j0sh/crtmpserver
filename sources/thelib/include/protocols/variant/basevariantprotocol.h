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


#ifdef HAS_PROTOCOL_VAR
#ifndef _BASEVARIANTPROTOCOL_H
#define	_BASEVARIANTPROTOCOL_H

#include "protocols/baseprotocol.h"

class BaseClientApplication;
class BaseVariantAppProtocolHandler;

class DLLEXP BaseVariantProtocol
: public BaseProtocol {
private:
	IOBuffer _outputBuffer;
	BaseVariantAppProtocolHandler *_pProtocolHandler;
protected:
	Variant _lastSent;
	Variant _lastReceived;
public:
	BaseVariantProtocol(uint64_t type);
	virtual ~BaseVariantProtocol();

	virtual bool Initialize(Variant &parameters);
	virtual void SetApplication(BaseClientApplication *pApplication);
	virtual IOBuffer * GetOutputBuffer();
	virtual bool AllowFarProtocol(uint64_t type);
	virtual bool AllowNearProtocol(uint64_t type);
	virtual bool SignalInputData(int32_t recvAmount);
	virtual bool SignalInputData(IOBuffer &buffer);
	bool Send(Variant &variant);
protected:
	virtual bool Serialize(string &rawData, Variant &variant) = 0;
	virtual bool Deserialize(uint8_t *pBuffer, uint32_t bufferLength,
			Variant &result) = 0;
};


#endif	/* _BASEVARIANTPROTOCOL_H */
#endif	/* HAS_PROTOCOL_VAR */

