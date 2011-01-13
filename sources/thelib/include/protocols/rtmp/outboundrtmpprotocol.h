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
#ifndef _OUTBOUNDRTMPPROTOCOL_H
#define	_OUTBOUNDRTMPPROTOCOL_H

#include "protocols/rtmp/basertmpprotocol.h"

class DHWrapper;

class DLLEXP OutboundRTMPProtocol
: public BaseRTMPProtocol {
private:
	uint8_t *_pClientPublicKey;
	uint8_t *_pOutputBuffer;
	uint8_t *_pClientDigest;
	RC4_KEY* _pKeyIn;
	RC4_KEY* _pKeyOut;
	DHWrapper *_pDHWrapper;
	uint8_t _usedScheme;
	IOBuffer _outputBuffer222;
public:
	OutboundRTMPProtocol();
	virtual ~OutboundRTMPProtocol();
protected:
	virtual bool PerformHandshake(IOBuffer &buffer);
public:
	static bool Connect(string ip, uint16_t port, Variant customParameters);
	static bool SignalProtocolCreated(BaseProtocol *pProtocol, Variant customParameters);
private:
	bool PerformHandshakeStage1(bool encrypted);
	bool VerifyServer(IOBuffer &inputBuffer);
	bool PerformHandshakeStage2(IOBuffer &inputBuffer, bool encrypted);
};

#endif	/* _OUTBOUNDRTMPPROTOCOL_H */

#endif /* HAS_PROTOCOL_RTMP */

