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


#ifdef HAS_PROTOCOL_RTMP
#ifndef _RTMPEPROTOCOL_H
#define	_RTMPEPROTOCOL_H

#include "protocols/baseprotocol.h"

class DLLEXP RTMPEProtocol
: public BaseProtocol {
private:
	IOBuffer _outputBuffer;
	IOBuffer _inputBuffer;
	RC4_KEY *_pKeyIn;
	RC4_KEY *_pKeyOut;
	uint32_t _skipBytes;
public:
	RTMPEProtocol(RC4_KEY *pKeyIn, RC4_KEY *pKeyOut, uint32_t skipBytes = 0);
	virtual ~RTMPEProtocol();

	virtual bool AllowFarProtocol(uint64_t type);
	virtual bool AllowNearProtocol(uint64_t type);
	virtual IOBuffer * GetInputBuffer();
	virtual IOBuffer * GetOutputBuffer();
	virtual bool SignalInputData(int32_t recvAmount);
	virtual bool SignalInputData(IOBuffer &buffer);
	virtual bool EnqueueForOutbound();

};


#endif	/* _RTMPEPROTOCOL_H */

#endif /* HAS_PROTOCOL_RTMP */

