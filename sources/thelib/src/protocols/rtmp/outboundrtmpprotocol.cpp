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

#include "protocols/rtmp/outboundrtmpprotocol.h"
#include "protocols/protocolfactorymanager.h"
#include "netio/netio.h"
#include "protocols/rtmp/rtmpeprotocol.h"
#include "protocols/rtmp/basertmpappprotocolhandler.h"
#include "application/clientapplicationmanager.h"

OutboundRTMPProtocol::OutboundRTMPProtocol()
: BaseRTMPProtocol(PT_OUTBOUND_RTMP) {
	_pClientPublicKey = NULL;
	_pOutputBuffer = NULL;
	_pClientDigest = NULL;
	_pKeyIn = NULL;
	_pKeyOut = NULL;
	_pDHWrapper = NULL;
}

OutboundRTMPProtocol::~OutboundRTMPProtocol() {
	if (_pKeyIn != NULL) {
		delete _pKeyIn;
		_pKeyIn = NULL;
	}

	if (_pKeyOut != NULL) {
		delete _pKeyOut;
		_pKeyOut = NULL;
	}

	if (_pDHWrapper != NULL) {
		delete _pDHWrapper;
		_pDHWrapper = NULL;
	}

	if (_pClientPublicKey != NULL) {
		delete[] _pClientPublicKey;
		_pClientPublicKey = NULL;
	}
	if (_pOutputBuffer != NULL) {
		delete[] _pOutputBuffer;
		_pOutputBuffer = NULL;
	}
	if (_pClientDigest != NULL) {
		delete[] _pClientDigest;
		_pClientDigest = NULL;
	}
}

bool OutboundRTMPProtocol::PerformHandshake(IOBuffer &buffer) {
	switch (_rtmpState) {
		case RTMP_STATE_NOT_INITIALIZED:
		{
			if ((VariantType) _customParameters[CONF_PROTOCOL] == V_STRING &&
					_customParameters[CONF_PROTOCOL] == CONF_PROTOCOL_OUTBOUND_RTMPE) {
				return PerformHandshakeStage1(true);
			} else {
				return PerformHandshakeStage1(false);
			}
		}
		case RTMP_STATE_CLIENT_REQUEST_SENT:
		{
			if (GETAVAILABLEBYTESCOUNT(buffer) < 3073)
				return true;

			bool encrypted = (VariantType) _customParameters[CONF_PROTOCOL] == V_STRING &&
					_customParameters[CONF_PROTOCOL] == CONF_PROTOCOL_OUTBOUND_RTMPE;

			if (!PerformHandshakeStage2(buffer, encrypted)) {
				FATAL("Unable to handshake");
				return false;
			}

			if (_pFarProtocol != NULL) {
				if (!_pFarProtocol->EnqueueForOutbound()) {
					FATAL("Unable to signal output data");
					return false;
				}
			}

			if (_pKeyIn != NULL && _pKeyOut != NULL) {
				//insert the RTMPE protocol in the current protocol stack
				BaseProtocol *pFarProtocol = GetFarProtocol();
				RTMPEProtocol *pRTMPE = new RTMPEProtocol(_pKeyIn, _pKeyOut,
						GETAVAILABLEBYTESCOUNT(_outputBuffer));
				ResetFarProtocol();
				pFarProtocol->SetNearProtocol(pRTMPE);
				pRTMPE->SetNearProtocol(this);
				FINEST("New protocol chain: %s", STR(*pFarProtocol));
			}

			if (!buffer.Ignore(3073)) {
				FATAL("Unable to ignore 3073 bytes");
				return false;
			}
			_handshakeCompleted = true;
			return true;
		}
		default:
		{
			FATAL("Invalid RTMP state: %d", _rtmpState);
			return false;
		}
	}
}

bool OutboundRTMPProtocol::Connect(string ip, uint16_t port,
		Variant customParameters) {

	vector<uint64_t> chain = ProtocolFactoryManager::ResolveProtocolChain(
			CONF_PROTOCOL_OUTBOUND_RTMP);
	if (chain.size() == 0) {
		FATAL("Unable to obtain protocol chain from settings: %s",
				CONF_PROTOCOL_OUTBOUND_RTMP);
		return false;
	}
	if (!TCPConnector<OutboundRTMPProtocol>::Connect(ip, port, chain,
			customParameters)) {
		FATAL("Unable to connect to %s:%d", STR(ip), port);
		return false;
	}
	return true;
}

bool OutboundRTMPProtocol::SignalProtocolCreated(BaseProtocol *pProtocol,
		Variant customParameters) {
	if (pProtocol == NULL) {
		FATAL("Connection failed:\n%s", STR(customParameters.ToString()));
		return false;
	}

	//1. Get the application designated for the newly created connection
	if (customParameters[CONF_APPLICATION_NAME] != V_STRING) {
		FATAL("connect parameters must have an application name");
		return false;
	}
	BaseClientApplication *pApplication = ClientApplicationManager::FindAppByName(
			customParameters[CONF_APPLICATION_NAME]);
	if (pApplication == NULL) {
		FATAL("Application %s not found", STR(customParameters[CONF_APPLICATION_NAME]));
		return false;
	}

	//2. Set the application
	pProtocol->SetApplication(pApplication);


	//3. Trigger processing, including handshake
	OutboundRTMPProtocol *pOutboundRTMPProtocol = (OutboundRTMPProtocol *) pProtocol;
	pOutboundRTMPProtocol->SetOutboundConnectParameters(customParameters);
	IOBuffer dummy;
	return pOutboundRTMPProtocol->SignalInputData(dummy);
}

bool OutboundRTMPProtocol::PerformHandshakeStage1(bool encrypted) {
	//FINEST("PHS1: 1. Put the protocol type");
	if (encrypted)
		_outputBuffer.ReadFromByte(6);
	else
		_outputBuffer.ReadFromByte(3);

	//FINEST("PHS1: 2. Prepare the buffer");
	if (_pOutputBuffer == NULL) {
		_pOutputBuffer = new uint8_t[1536];
	} else {
		delete[] _pOutputBuffer;
		_pOutputBuffer = new uint8_t[1536];
	}

	//FINEST("PHS1: 3. Randomize the buffer");
	for (uint32_t i = 0; i < 1536; i++) {
		_pOutputBuffer[i] = rand() % 256;
		//_pOutputBuffer[i] = 0;
	}

	//FINEST("PHS1: 4. Put the uptime. In our case, 0.");
	*((uint32_t *) _pOutputBuffer) = 0;

	//FINEST("PHS1: 5. Put the flash version. We impersonate with 9.0.124.2");
	_pOutputBuffer[4] = 9;
	_pOutputBuffer[5] = 0;
	_pOutputBuffer[6] = 124;
	_pOutputBuffer[7] = 2;

	uint32_t clientDHOffset = GetDHOffset(_pOutputBuffer, 0);
	//FINEST("PHS1: 6. Get the DH public key position: %d", clientDHOffset);

	//FINEST("PHS1: 7. Generate the DH public/private key");
	_pDHWrapper = new DHWrapper(1024);
	if (!_pDHWrapper->Initialize()) {
		FATAL("Unable to initialize DH wrapper");
		return false;
	}

	//FINEST("PHS1: 8. Get the public key and store it in the buffer at %d and _pClientPublicKey for later use", clientDHOffset);
	if (!_pDHWrapper->CopyPublicKey(_pOutputBuffer + clientDHOffset, 128)) {
		FATAL("Couldn't write public key!");
		return false;
	}
	_pClientPublicKey = new uint8_t[128];
	memcpy(_pClientPublicKey, _pOutputBuffer + clientDHOffset, 128);


	uint32_t clientDigestOffset = GetDigestOffset(_pOutputBuffer, 0);
	//FINEST("PHS1: 9.Compute the final digest offset: %d", clientDigestOffset);

	//FINEST("PHS1: 10. Generate the digest from pBuffer EXCLUDING the digest portion.");
	uint8_t *pTempBuffer = new uint8_t[1536 - 32];
	memcpy(pTempBuffer, _pOutputBuffer, clientDigestOffset);
	memcpy(pTempBuffer + clientDigestOffset, _pOutputBuffer + clientDigestOffset + 32,
			1536 - clientDigestOffset - 32);

	//FINEST("PHS1: 11. Generate the hash");
	uint8_t *pTempHash = new uint8_t[512];
	HMACsha256(pTempBuffer, 1536 - 32, genuineFPKey, 30, pTempHash);

	//FINEST("PHS1: 12. put the bytes at %d offset. Also save them for later use", clientDigestOffset);
	memcpy(_pOutputBuffer + clientDigestOffset, pTempHash, 32);
	_pClientDigest = new uint8_t[32];
	memcpy(_pClientDigest, pTempHash, 32);


	//FINEST("PHS1: 13. cleanup the temp buffers");
	delete[] pTempBuffer;
	delete[] pTempHash;

	//FINEST("PHS1: 14. Put the buffer on the outputBuffer");
	_outputBuffer.ReadFromBuffer(_pOutputBuffer, 1536);
	//FINEST("InputBuffer:\n%s", STR(_outputBuffer));

	//FINEST("PHS1: 15. delete the buffer");
	delete[] _pOutputBuffer;
	_pOutputBuffer = NULL;

	//FINEST("PHS1: 16. Signal the protocol stack that we have outbound data");
	if (_pFarProtocol != NULL) {
		if (!_pFarProtocol->EnqueueForOutbound()) {
			FATAL("Unable to signal output data");
			return false;
		}
	}

	//FINEST("PHS1: 17. Move to the next stage of the handshake");
	_rtmpState = RTMP_STATE_CLIENT_REQUEST_SENT;

	return true;
}

bool OutboundRTMPProtocol::VerifyServer(IOBuffer & inputBuffer) {
	uint8_t *pBuffer = GETIBPOINTER(inputBuffer) + 1;

	uint32_t serverDigestPos = GetDigestOffset(pBuffer, 0);
	//FINEST("VS: 1. Compute server digest offset: %d", serverDigestPos);

	//FINEST("VS: 2. Prepare the buffer");
	uint8_t *pTempBuffer = new uint8_t[1536 - 32];
	memcpy(pTempBuffer, pBuffer, serverDigestPos);
	memcpy(pTempBuffer + serverDigestPos, pBuffer + serverDigestPos + 32,
			1536 - serverDigestPos - 32);

	//FINEST("VS: 3. Compute the digest");
	uint8_t * pDigest = new uint8_t[512];
	HMACsha256(pTempBuffer, 1536 - 32, genuineFMSKey, 36, pDigest);

	//FINEST("VS: 3. Compare the results");
	int result = memcmp(pDigest, pBuffer + serverDigestPos, 32);

	//FINEST("VS: 4. Cleanup");
	delete[] pTempBuffer;
	delete[] pDigest;

	if (result != 0) {
		FATAL("Server not verified");
		return false;
	}

	//FINEST("VS: 5. Advance the server response to the next chunk");
	pBuffer = pBuffer + 1536;

	//FINEST("VS: 6. Compute the chalange");
	uint8_t * pChallange = new uint8_t[512];
	HMACsha256(_pClientDigest, 32, genuineFMSKey, 68, pChallange);

	//FINEST("VS: 7. Compute the second digest");
	pDigest = new uint8_t[512];
	HMACsha256(pBuffer, 1536 - 32, pChallange, 32, pDigest);

	//FINEST("VS: 8. Compare the results");
	result = memcmp(pDigest, pBuffer + 1536 - 32, 32);

	//FINEST("VS: 9. Cleanup");
	delete[] pChallange;
	delete[] pDigest;

	if (result != 0) {
		FATAL("Server not verified");
		return false;
	}

	//FINEST("VS: 10. Server is verified");

	return true;
}

bool OutboundRTMPProtocol::PerformHandshakeStage2(IOBuffer &inputBuffer,
		bool encrypted) {
	if (encrypted || _pProtocolHandler->ValidateHandshake()) {
		if (!VerifyServer(inputBuffer)) {
			FATAL("Unable to verify server");
			return false;
		}
	}

	uint8_t *pBuffer = GETIBPOINTER(inputBuffer) + 1;

	uint32_t serverDHOffset = GetDHOffset(pBuffer, 0);
	//FINEST("PHS2: 1. get the serverDHOffset: %d", serverDHOffset);

	//FINEST("PHS2: 2. compute the secret key");
	if (_pDHWrapper == NULL) {
		FATAL("dh wrapper not initialized");
		return false;
	}

	if (!_pDHWrapper->CreateSharedKey(pBuffer + serverDHOffset, 128)) {
		FATAL("Unable to create shared key");
		return false;
	}

	uint8_t secretKey[128];
	if (!_pDHWrapper->CopySharedKey(secretKey, sizeof (secretKey))) {
		FATAL("Unable to compute shared");
		return false;
	}

	if (encrypted) {
		_pKeyIn = new RC4_KEY;
		_pKeyOut = new RC4_KEY;

		InitRC4Encryption(
				secretKey,
				(uint8_t*) & pBuffer[serverDHOffset],
				_pClientPublicKey,
				_pKeyIn,
				_pKeyOut);

		//FINEST("PHS2: 3. bring the keys to correct cursor");
		uint8_t data[1536];
		RC4(_pKeyIn, 1536, data, data);
		RC4(_pKeyOut, 1536, data, data);
	}

	delete _pDHWrapper;
	_pDHWrapper = NULL;

	//FINEST("PHS2: 4. Get server digest offset");
	uint32_t serverDigestOffset = GetDigestOffset(pBuffer, 0);

	//FINEST("PHS2: 5. Prepare the response buffer");
	if (_pOutputBuffer == NULL) {
		_pOutputBuffer = new uint8_t[1536];
	} else {
		delete[] _pOutputBuffer;
		_pOutputBuffer = new uint8_t[1536];
	}

	//FINEST("PHS2: 6. Randomize the response");
	for (uint32_t i = 0; i < 1536; i++) {
		_pOutputBuffer[i] = rand() % 256;
	}

	//FINEST("PHS2: 7. Compute the challange");
	uint8_t * pChallangeKey = new uint8_t[512];
	HMACsha256(pBuffer + serverDigestOffset, 32, genuineFPKey, 62, pChallangeKey);

	//FINEST("PHS2: 8. Compute the client digest");
	uint8_t * pDigest = new uint8_t[512];
	HMACsha256(_pOutputBuffer, 1536 - 32, pChallangeKey, 32, pDigest);

	//FINEST("PHS2: 9. Put the digest where it belongs");
	memcpy(_pOutputBuffer + 1536 - 32, pDigest, 32);

	//FINEST("PHS2: 10. Cleanup");
	delete[] pChallangeKey;
	delete[] pDigest;

	//FINEST("PHS2: 11. Put the buffer on the outputBuffer");
	_outputBuffer.ReadFromBuffer(_pOutputBuffer, 1536);

	//FINEST("PHS2: 12. delete the buffer");
	delete[] _pOutputBuffer;
	_pOutputBuffer = NULL;

	//FINEST("PHS2: 13. Go to the next stage in the handshake");
	_rtmpState = RTMP_STATE_DONE;

	return true;
}
#endif /* HAS_PROTOCOL_RTMP */

