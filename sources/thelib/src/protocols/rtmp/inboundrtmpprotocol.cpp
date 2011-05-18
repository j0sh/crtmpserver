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
#include "protocols/rtmp/inboundrtmpprotocol.h"
#include "protocols/rtmp/rtmpeprotocol.h"
#include "protocols/rtmp/basertmpappprotocolhandler.h"

InboundRTMPProtocol::InboundRTMPProtocol()
: BaseRTMPProtocol(PT_INBOUND_RTMP) {
	_pKeyIn = NULL;
	_pKeyOut = NULL;
	_pOutputBuffer = NULL;
	_currentFPVersion = 0;
	_validationScheme = 0;
}

InboundRTMPProtocol::~InboundRTMPProtocol() {
	if (_pKeyIn != NULL) {
		delete _pKeyIn;
		_pKeyIn = NULL;
	}

	if (_pKeyOut != NULL) {
		delete _pKeyOut;
		_pKeyOut = NULL;
	}

	if (_pOutputBuffer != NULL) {
		delete[] _pOutputBuffer;
		_pOutputBuffer = NULL;
	}
}

bool InboundRTMPProtocol::PerformHandshake(IOBuffer &buffer) {
	switch (_rtmpState) {
		case RTMP_STATE_NOT_INITIALIZED:
		{
			if (GETAVAILABLEBYTESCOUNT(buffer) < 1537) {
				return true;
			}
			uint8_t handshakeType = GETIBPOINTER(buffer)[0];
			if (!buffer.Ignore(1)) {
				FATAL("Unable to ignore one byte");
				return false;
			}

			_currentFPVersion = ENTOHLP(GETIBPOINTER(buffer) + 4);

			switch (handshakeType) {
				case 3: //plain
				{
					return PerformHandshake(buffer, false);
				}
				case 6: //encrypted
				{
					return PerformHandshake(buffer, true);
				}
				default:
				{
					FATAL("Handshake type not implemented: %hhu", handshakeType);
					return false;
				}
			}
		}
		case RTMP_STATE_SERVER_RESPONSE_SENT:
		{
			if (GETAVAILABLEBYTESCOUNT(buffer) < 1536) {
				return true;
			} else {
				//ignore the client's last handshake part
				if (!buffer.Ignore(1536)) {
					FATAL("Unable to ignore inbound data");
					return false;
				}
				_handshakeCompleted = true;
				_rtmpState = RTMP_STATE_DONE;

				if (_pKeyIn != NULL && _pKeyOut != NULL) {
					//insert the RTMPE protocol in the current protocol stack
					BaseProtocol *pFarProtocol = GetFarProtocol();
					RTMPEProtocol *pRTMPE = new RTMPEProtocol(_pKeyIn, _pKeyOut);
					ResetFarProtocol();
					pFarProtocol->SetNearProtocol(pRTMPE);
					pRTMPE->SetNearProtocol(this);
					FINEST("New protocol chain: %s", STR(*pFarProtocol));

					//decrypt the leftovers
					RC4(_pKeyIn, GETAVAILABLEBYTESCOUNT(buffer),
							GETIBPOINTER(buffer),
							GETIBPOINTER(buffer));
				}

				return true;
			}
		}
		default:
		{
			FATAL("Invalid RTMP state: %hhu", _rtmpState);
			return false;
		}
	}
}

bool InboundRTMPProtocol::ValidateClient(IOBuffer &inputBuffer) {
	if (_currentFPVersion == 0) {
		WARN("This version of player doesn't support validation");
		return true;
	}
	if (ValidateClientScheme(inputBuffer, 0)) {
		_validationScheme = 0;
		return true;
	}
	if (ValidateClientScheme(inputBuffer, 1)) {
		_validationScheme = 1;
		return true;
	}
	FATAL("Unable to validate client");
	return false;
}

bool InboundRTMPProtocol::ValidateClientScheme(IOBuffer &inputBuffer, uint8_t scheme) {
	uint8_t *pBuffer = GETIBPOINTER(inputBuffer);

	uint32_t clientDigestOffset = GetDigestOffset(pBuffer, scheme);

	uint8_t *pTempBuffer = new uint8_t[1536 - 32];
	memcpy(pTempBuffer, pBuffer, clientDigestOffset);
	memcpy(pTempBuffer + clientDigestOffset, pBuffer + clientDigestOffset + 32,
			1536 - clientDigestOffset - 32);

	uint8_t *pTempHash = new uint8_t[512];
	HMACsha256(pTempBuffer, 1536 - 32, genuineFPKey, 30, pTempHash);

	bool result = true;
	for (uint32_t i = 0; i < 32; i++) {
		if (pBuffer[clientDigestOffset + i] != pTempHash[i]) {
			result = false;
			break;
		}
	}

	delete[] pTempBuffer;
	delete[] pTempHash;

	return result;
}

bool InboundRTMPProtocol::PerformHandshake(IOBuffer &buffer, bool encrypted) {
	if (!ValidateClient(buffer)) {
		if (encrypted || _pProtocolHandler->ValidateHandshake()) {
			FATAL("Unable to validate client");
			return false;
		} else {
			WARN("Client not validated");
			_validationScheme = 0;
		}
	}


	//get the buffers
	uint8_t *pInputBuffer = GETIBPOINTER(buffer);
	if (_pOutputBuffer == NULL) {
		_pOutputBuffer = new uint8_t[3072];
	} else {
		delete[] _pOutputBuffer;
		_pOutputBuffer = new uint8_t[3072];
	}

	//timestamp
	EHTONLP(_pOutputBuffer, (uint32_t) time(NULL));

	//version
	EHTONLP(_pOutputBuffer + 4, (uint32_t) 0x00000000);

	//generate random data
	for (uint32_t i = 8; i < 3072; i++) {
		_pOutputBuffer[i] = rand() % 256;
	}
	for (uint32_t i = 0; i < 10; i++) {
		uint32_t index = rand() % (3072 - HTTP_HEADERS_SERVER_US_LEN);
		memcpy(_pOutputBuffer + index, HTTP_HEADERS_SERVER_US, HTTP_HEADERS_SERVER_US_LEN);
	}

	//**** FIRST 1536 bytes from server response ****//
	//compute DH key position
	uint32_t serverDHOffset = GetDHOffset(_pOutputBuffer, _validationScheme);
	uint32_t clientDHOffset = GetDHOffset(pInputBuffer, _validationScheme);

	//generate DH key
	DHWrapper dhWrapper(1024);

	if (!dhWrapper.Initialize()) {
		FATAL("Unable to initialize DH wrapper");
		return false;
	}

	if (!dhWrapper.CreateSharedKey(pInputBuffer + clientDHOffset, 128)) {
		FATAL("Unable to create shared key");
		return false;
	}

	if (!dhWrapper.CopyPublicKey(_pOutputBuffer + serverDHOffset, 128)) {
		FATAL("Couldn't write public key!");
		return false;
	}

	if (encrypted) {
		uint8_t secretKey[128];
		if (!dhWrapper.CopySharedKey(secretKey, sizeof (secretKey))) {
			FATAL("Unable to copy shared key");
			return false;
		}

		_pKeyIn = new RC4_KEY;
		_pKeyOut = new RC4_KEY;
		InitRC4Encryption(
				secretKey,
				(uint8_t*) & pInputBuffer[clientDHOffset],
				(uint8_t*) & _pOutputBuffer[serverDHOffset],
				_pKeyIn,
				_pKeyOut);

		//bring the keys to correct cursor
		uint8_t data[1536];
		RC4(_pKeyIn, 1536, data, data);
		RC4(_pKeyOut, 1536, data, data);
	}

	//generate the digest
	uint32_t serverDigestOffset = GetDigestOffset(_pOutputBuffer, _validationScheme);

	uint8_t *pTempBuffer = new uint8_t[1536 - 32];
	memcpy(pTempBuffer, _pOutputBuffer, serverDigestOffset);
	memcpy(pTempBuffer + serverDigestOffset, _pOutputBuffer + serverDigestOffset + 32,
			1536 - serverDigestOffset - 32);

	uint8_t *pTempHash = new uint8_t[512];
	HMACsha256(pTempBuffer, 1536 - 32, genuineFMSKey, 36, pTempHash);

	//put the digest in place
	memcpy(_pOutputBuffer + serverDigestOffset, pTempHash, 32);

	//cleanup
	delete[] pTempBuffer;
	delete[] pTempHash;


	//**** SECOND 1536 bytes from server response ****//
	//Compute the chalange index from the initial client request
	uint32_t keyChallengeIndex = GetDigestOffset(pInputBuffer, _validationScheme);

	//compute the key
	pTempHash = new uint8_t[512];
	HMACsha256(pInputBuffer + keyChallengeIndex, //pData
			32, //dataLength
			BaseRTMPProtocol::genuineFMSKey, //key
			68, //keyLength
			pTempHash //pResult
			);

	//generate the hash
	uint8_t *pLastHash = new uint8_t[512];
	HMACsha256(_pOutputBuffer + 1536, //pData
			1536 - 32, //dataLength
			pTempHash, //key
			32, //keyLength
			pLastHash //pResult
			);

	//put the hash where it belongs
	memcpy(_pOutputBuffer + 1536 * 2 - 32, pLastHash, 32);


	//cleanup
	delete[] pTempHash;
	delete[] pLastHash;
	//***** DONE BUILDING THE RESPONSE ***//


	//wire the response
	if (encrypted)
		_outputBuffer.ReadFromByte(6);
	else
		_outputBuffer.ReadFromByte(3);
	_outputBuffer.ReadFromBuffer(_pOutputBuffer, 3072);

	//final cleanup
	delete[] _pOutputBuffer;
	_pOutputBuffer = NULL;
	if (!buffer.IgnoreAll()) {
		FATAL("Unable to ignore input buffer");
		return false;
	}

	//signal outbound data
	if (!EnqueueForOutbound()) {
		FATAL("Unable to signal outbound data");
		return false;
	}

	//move to the next stage in the handshake
	_rtmpState = RTMP_STATE_SERVER_RESPONSE_SENT;

	return true;
}

#endif /* HAS_PROTOCOL_RTMP */

