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


#ifndef _CRYPTO_H
#define	_CRYPTO_H

#include "new/platform/platform.h"

class DLLEXP DHWrapper {
private:
	int32_t _bitsCount;
	DH *_pDH;
	uint8_t *_pSharedKey;
	int32_t _sharedKeyLength;
	BIGNUM *_peerPublickey;
public:
	DHWrapper(int32_t bitsCount);
	virtual ~DHWrapper();

	bool Initialize();
	bool CopyPublicKey(uint8_t *pDst, int32_t dstLength);
	bool CopyPrivateKey(uint8_t *pDst, int32_t dstLength);
	bool CreateSharedKey(uint8_t *pPeerPublicKey, int32_t length);
	bool CopySharedKey(uint8_t *pDst, int32_t dstLength);
private:
	void Cleanup();
	bool CopyKey(BIGNUM *pNum, uint8_t *pDst, int32_t dstLength);
};

DLLEXP void InitRC4Encryption(uint8_t *secretKey, uint8_t *pubKeyIn, uint8_t *pubKeyOut,
		RC4_KEY *rc4keyIn, RC4_KEY *rc4keyOut);
DLLEXP string md5(string source, bool textResult);
DLLEXP string b64(string source);
DLLEXP string b64(uint8_t *pBuffer, uint32_t length);
DLLEXP string unb64(string source);
DLLEXP string unb64(uint8_t *pBuffer, uint32_t length);

#endif /* _CRYPTO_H */

