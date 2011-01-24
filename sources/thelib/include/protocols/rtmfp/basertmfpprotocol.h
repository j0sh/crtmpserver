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

#ifdef HAS_PROTOCOL_RTMFP
#ifndef _BASERTMFPPROTOCOL_H
#define	_BASERTMFPPROTOCOL_H

#include "protocols/baseprotocol.h"
#include "protocols/rtmp/amf3serializer.h"
#include "protocols/rtmp/amf0serializer.h"

struct EncDec {
	uint8_t iv[128];
	AES_KEY key;

	EncDec() {
		memset(iv, 0, sizeof (iv));
		memset(&key, 0, sizeof (key));
	}

	void Setup(bool isEnc, const uint8_t *pKey, uint8_t bitsCount) {
		if (isEnc) {
			AES_set_encrypt_key(pKey, bitsCount, &key);
		} else {
			AES_set_decrypt_key(pKey, bitsCount, &key);
		}
	}
};

struct RTMFPPeer {
	uint8_t tag[16];
	uint8_t cookie[0x4c];
	uint8_t publicKey[128];
	uint8_t id[32];
	sockaddr_in address;
	//	EncDec enc;
	//	EncDec dec;

	RTMFPPeer() {
		memset(tag, 0, sizeof (tag));
		memset(cookie, 0, sizeof (cookie));
		//		enc.Setup(true, (uint8_t *) "Adobe Systems 02", 128);
		//		dec.Setup(false, (uint8_t *) "Adobe Systems 02", 128);
	}

	//	void SetEncDecKeys(uint8_t *pEnc, uint8_t * pDec) {
	//		enc.Setup(true, (uint8_t *) pEnc, 128);
	//		dec.Setup(false, (uint8_t *) pDec, 128);
	//	}
};

struct RTMFPSession {
	uint32_t id;
	uint32_t newId;
	string connectUri;
	RTMFPPeer client;
	RTMFPPeer server;
	DHWrapper dhw;
	bool dhwComputed;
	EncDec enc;
	EncDec dec;

	RTMFPSession() : dhw(1024) {
		id = 0;
		dhwComputed = false;
		enc.Setup(true, (uint8_t *) "Adobe Systems 02", 128);
		dec.Setup(false, (uint8_t *) "Adobe Systems 02", 128);
	}

	void ComputeKeys() {
		unsigned char buf[0x8b] = "\x03\x1a\x00\x00\x02\x1e\x00\x81\x02\x0d\x02";
		unsigned char md1[0x20];
		unsigned char md2[0x20];
		unsigned char sec1[0x20];
		unsigned char sec2[0x20];
		unsigned char ss[128];

		dhw.CopyPublicKey(buf + 11, 128);
		dhw.CopySharedKey(ss, 128);
		HMAC(EVP_sha256(), buf, 0x8b, client.cookie, 0x4c, md1, NULL);
		HMAC(EVP_sha256(), client.cookie, 0x4c, buf, 0x8b, md2, NULL);
		HMAC(EVP_sha256(), ss, 0x80, md1, 0x20, sec1, NULL);
		HMAC(EVP_sha256(), ss, 0x80, md2, 0x20, sec2, NULL);
		enc.Setup(true, (uint8_t *) sec2, 128);
		dec.Setup(false, (uint8_t *) sec1, 128);
		//		server.SetEncDecKeys(sec1, sec2);
		//		client.SetEncDecKeys(sec2, sec1);
	}
};

struct OutputBuffer {
	uint8_t outputBuffer[65536];
	uint8_t *pXor_1;
	uint8_t *pXor_2;
	uint8_t *pXor_3;
	uint8_t *pCRC;
	uint8_t *pMessageType;
	uint8_t *pTimestamp;
	uint8_t *pChunkType;
	uint8_t *pChunkLength;
	uint8_t *pChunk;

	OutputBuffer() {

		memset(outputBuffer, 0, sizeof (outputBuffer));
		pXor_1 = outputBuffer;
		pXor_2 = pXor_1 + 4;
		pXor_3 = pXor_2 + 4;
		pCRC = pXor_1 + 4;
		pMessageType = pCRC + 2;
		pTimestamp = pMessageType + 1;
		pChunkType = pTimestamp + 2;
		pChunkLength = pChunkType + 1;
		pChunk = pChunkLength + 2;
	}
};

class BaseRTMFPProtocol :
public BaseProtocol {
private:
	map<uint32_t, map <uint16_t, RTMFPSession *> > _initSessions;
	map<uint32_t, RTMFPSession *> _sessions;
	AMF3Serializer _amf3Serializer;
	AMF0Serializer _amf0Serializer;
	OutputBuffer _outputBuffer;
	static uint8_t _serverCert[64];
public:
	BaseRTMFPProtocol();
	virtual ~BaseRTMFPProtocol();

	virtual bool Initialize(Variant &parameters);
	virtual bool AllowFarProtocol(uint64_t type);
	virtual bool AllowNearProtocol(uint64_t type);
	virtual bool SignalInputData(int32_t recvAmount);
	virtual bool SignalInputData(IOBuffer &buffer, sockaddr_in *pPeerAddress);
	virtual bool SignalInputData(IOBuffer &buffer);
private:
	static inline uint16_t ComputeChkSum(uint8_t *pBuffer, uint32_t length);
	static inline void Decrypt(RTMFPSession *pSession, uint8_t *pBuffer, uint32_t length);
	static inline void Encrypt(RTMFPSession *pSession, uint8_t *pBuffer, uint32_t length);
	void SendToClient(RTMFPSession *pSession, uint8_t messageType, uint8_t chunkType, uint16_t chunkLength);
	void ProcessSession(RTMFPSession *pSession, IOBuffer &buffer);
	void ProcessSessionMessage(
			RTMFPSession *pSession,
			IOBuffer &buffer,
			uint8_t type,
			uint16_t timestamp,
			uint8_t chunkType,
			uint16_t chunkLength);
	void ProcessSessionMessage_0b(
			RTMFPSession *pSession,
			IOBuffer &buffer,
			uint16_t timestamp,
			uint8_t chunkType,
			uint16_t chunkLength);
	void ProcessSessionMessage_0b_30(
			RTMFPSession *pSession,
			IOBuffer &buffer,
			uint16_t timestamp,
			uint16_t chunkLength);
	void ProcessSessionMessage_0b_38(
			RTMFPSession *pSession,
			IOBuffer &buffer,
			uint16_t timestamp,
			uint16_t chunkLength);
};

#endif	/* _BASERTMFPPROTOCOL_H */
#endif /* HAS_PROTOCOL_RTMFP */
