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
#ifndef _INBOUNDRTMPPROTOCOL_H
#define	_INBOUNDRTMPPROTOCOL_H

#include "protocols/rtmp/basertmpprotocol.h"

class DLLEXP InboundRTMPProtocol
: public BaseRTMPProtocol {
private:
	RC4_KEY*_pKeyIn;
	RC4_KEY*_pKeyOut;
	uint8_t *_pOutputBuffer;
	uint32_t _currentFPVersion;
	uint8_t _handshakeScheme;
public:
	InboundRTMPProtocol();
	virtual ~InboundRTMPProtocol();
protected:
	virtual bool PerformHandshake(IOBuffer &buffer);
private:
	bool ValidateClient(IOBuffer &inputBuffer);
	bool ValidateClientScheme(IOBuffer &inputBuffer, uint8_t scheme);
	bool PerformHandshake(IOBuffer &buffer, bool encrypted);
	bool PerformSimpleHandshake(IOBuffer &buffer);
	bool PerformComplexHandshake(IOBuffer &buffer, bool encrypted);
};

#endif	/* _INBOUNDRTMPPROTOCOL_H */
#endif /* HAS_PROTOCOL_RTMP */

