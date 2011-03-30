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
#include <openssl/aes.h>

#include "protocols/rtmfp/basertmfpprotocol.h"
#include "protocols/protocoltypes.h"
#include "netio/netio.h"
#include "protocols/http/basehttpprotocol.h"
#include "protocols/rtmp/outboundrtmpprotocol.h"

uint8_t BaseRTMFPProtocol::_serverCert[64] = {
	0xF4, 0xE4, 0x53, 0x6A, 0x66, 0x76, 0xEA, 0xEA,
	0x7D, 0xBA, 0x43, 0x5B, 0xB8, 0x82, 0x72, 0x54,
	0x4C, 0x46, 0xDF, 0x91, 0x54, 0x2A, 0xB8, 0x1D,
	0xE0, 0x32, 0xB1, 0x8F, 0x10, 0xD2, 0x5F, 0x1E,
	0x3A, 0x1E, 0x2C, 0x42, 0xAE, 0x68, 0x64, 0x52,
	0xD5, 0x56, 0x87, 0xFE, 0x7B, 0xC4, 0xEA, 0xED,
	0x14, 0x06, 0x38, 0x9D, 0x27, 0x6F, 0xE6, 0xCE,
	0x9C, 0x38, 0x18, 0xB3, 0xEF, 0x12, 0x06, 0x2A
};

BaseRTMFPProtocol::BaseRTMFPProtocol(uint64_t type)
: BaseProtocol(type) {

}

BaseRTMFPProtocol::~BaseRTMFPProtocol() {
}

bool BaseRTMFPProtocol::Initialize(Variant &parameters) {
	GetCustomParameters() = parameters;
	return true;
}

bool BaseRTMFPProtocol::AllowFarProtocol(uint64_t type) {
	return type == PT_UDP;
}

bool BaseRTMFPProtocol::AllowNearProtocol(uint64_t type) {
	return false;
}

bool BaseRTMFPProtocol::SignalInputData(int32_t recvAmount) {
	ASSERT("Operation not supported");
}

bool BaseRTMFPProtocol::SignalInputData(IOBuffer &buffer, sockaddr_in *pPeerAddress) {
	//1. get the buffer and the length
	uint8_t *pBuffer = GETIBPOINTER(buffer);
	uint32_t length = GETAVAILABLEBYTESCOUNT(buffer);

	//2. is the length correct?
	if ((length < 12)
			|| (((length - 4) % 16) != 0)) {
		FATAL("Incorrect length: %u", length);
		return true;
	}

	//3. Compute the session id
	uint32_t sessionId = ENTOHLP(pBuffer)^ENTOHLP(pBuffer + 4)^ENTOHLP(pBuffer + 8);

	//4. Get the session
	RTMFPSession *pSession = NULL;

	if (sessionId == 0) {
		if (MAP_HAS2(_initSessions, pPeerAddress->sin_addr.s_addr, pPeerAddress->sin_port)) {
			WARN("Session in hndshake mode: %s:%d",
					inet_ntoa(pPeerAddress->sin_addr),
					ENTOHS(pPeerAddress->sin_port));
			pSession = _initSessions[pPeerAddress->sin_addr.s_addr][pPeerAddress->sin_port];
		} else {
			WARN("Brand new session: %s:%d",
					inet_ntoa(pPeerAddress->sin_addr),
					ENTOHS(pPeerAddress->sin_port));
			pSession = new RTMFPSession;
			_initSessions[pPeerAddress->sin_addr.s_addr][pPeerAddress->sin_port] = pSession;
		}
	} else {
		if (MAP_HAS1(_sessions, sessionId)) {
			WARN("Active session: %08x - %s:%d",
					sessionId,
					inet_ntoa(pPeerAddress->sin_addr),
					ENTOHS(pPeerAddress->sin_port));
			pSession = _sessions[sessionId];
		} else {
			WARN("Bogus session: %08x - %s:%d",
					sessionId,
					inet_ntoa(pPeerAddress->sin_addr),
					ENTOHS(pPeerAddress->sin_port));
		}
	}

	//5. Process session
	if (pSession != NULL) {
		memcpy(&pSession->client.address, pPeerAddress, sizeof (sockaddr_in));
		ProcessSession(pSession, buffer);
	}

	buffer.IgnoreAll();
	return true;
}

bool BaseRTMFPProtocol::SignalInputData(IOBuffer &buffer) {
	ASSERT("Operation not supported");
}

unsigned short get_timestamp() {
	uint32_t ts;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	ts = ((tv.tv_sec % 1000) * 1000 + (tv.tv_usec / 1000)) / 4;
	ts = ts % 65536;
	return ts & 0xffff;
}

uint16_t BaseRTMFPProtocol::ComputeChkSum(uint8_t *pBuffer, uint32_t length) {
	register uint32_t remaining = length;
	register uint8_t *pCursor = pBuffer;
	register int32_t sum = 0;
	uint16_t result = 0;

	while (remaining > 1) {
		sum += *(pCursor++) << 8;
		sum += *(pCursor++);
		remaining -= 2;
	}

	if (remaining == 1) {
		sum += *pCursor;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	result = ~sum;
	return (result);
}

void BaseRTMFPProtocol::Decrypt(RTMFPSession *pSession, uint8_t *pBuffer, uint32_t length) {
	memset(pSession->dec.iv, 0, sizeof (pSession->dec.iv));
	AES_cbc_encrypt(pBuffer, pBuffer, length, &pSession->dec.key, pSession->dec.iv, AES_DECRYPT);
}

void BaseRTMFPProtocol::Encrypt(RTMFPSession *pSession, uint8_t *pBuffer, uint32_t length) {
	memset(pSession->enc.iv, 0, sizeof (pSession->enc.iv));
	AES_cbc_encrypt(pBuffer, pBuffer, length, &pSession->enc.key, pSession->enc.iv, AES_ENCRYPT);
}

void BaseRTMFPProtocol::SendToClient(RTMFPSession *pSession,
		uint8_t messageType, uint8_t chunkType, uint16_t chunkLength) {
	//1. Set the message type
	*_outputBuffer.pMessageType = messageType;

	//2. Set the timestamp
	EHTONSP(_outputBuffer.pTimestamp, get_timestamp());

	//3. Set the chunk type
	*_outputBuffer.pChunkType = chunkType;

	//4. Set the chunk length
	EHTONSP(_outputBuffer.pChunkLength, chunkLength);

	//5. Compute the length of the padding
	uint16_t padLen = (16 - ((chunkLength + 8) % 16)) % 16;

	//6. Apply the padding
	memset(_outputBuffer.pChunk + chunkLength, 0xff, padLen);

	//3. Do the CRC
	EHTONSP(_outputBuffer.pCRC, ComputeChkSum(_outputBuffer.pMessageType, chunkLength + 6 + padLen));

	//4. Encrypt it
	WARN("OUTPUT: \n%s", STR(IOBuffer::DumpBuffer(_outputBuffer.pCRC, chunkLength + 8 + padLen)));
	Encrypt(pSession, _outputBuffer.pCRC, chunkLength + 8 + padLen);

	//5. Compute the xor
	EHTONLP(_outputBuffer.pXor_1,
			ENTOHLP(_outputBuffer.pXor_2)^ENTOHLP(_outputBuffer.pXor_3)^pSession->id);

	//8. Before actual we do the actual transfer, move the current session where it belongs (if necessary)
	if (chunkType == 0x78) {
		MAP_ERASE2(_initSessions, pSession->client.address.sin_addr.s_addr, pSession->client.address.sin_port);
		pSession->id = pSession->newId;
		INFO("New session: %08x", pSession->id);
		_sessions[pSession->id] = pSession;
	}

	//6. Send it
	sendto(((UDPCarrier *) GetIOHandler())->GetOutboundFd(),
			_outputBuffer.outputBuffer,
			chunkLength + 8 + padLen + 4,
			0,
			(sockaddr *) & pSession->client.address,
			sizeof (sockaddr));
}

void BaseRTMFPProtocol::ProcessSession(RTMFPSession *pSession, IOBuffer &buffer) {
	uint8_t *pBuffer = GETIBPOINTER(buffer);
	uint32_t length = GETAVAILABLEBYTESCOUNT(buffer);

	//1. Is valid packet?
	if ((ENTOHLP(pBuffer)^ENTOHLP(pBuffer + 4)^ENTOHLP(pBuffer + 8)^pSession->id) != 0) {
		FATAL("Invalid session crc: %08x", pSession->id);
		return;
	}

	//2. Decrypt the packet
	Decrypt(pSession, pBuffer + 4, length - 4);
	FINEST("INPUT:\n%s", STR(buffer));

	//3. Compute CRC
	uint16_t crc = ComputeChkSum(pBuffer + 6, length - 6);

	//4. Is this a valid CRC?
	if (ENTOHSP(pBuffer + 4) != crc) {
		FATAL("Invalid CRC");
		return;
	}

	//5. Read message type
	uint8_t type = pBuffer[6];
	bool E = (type & 0x80) != 0;
	bool S = (type & 0x40) != 0;
	bool r1 = (type & 0x20) != 0;
	bool r2 = (type & 0x10) != 0;
	bool I = (type & 0x08) != 0;
	bool R = (type & 0x04) != 0;
	bool t = (type & 0x02) != 0;
	bool T = (type & 0x01) != 0;
	FINEST("BITS: %d %d %d %d %d %d %d %d",
			E, S, r1, r2, I, R, t, T);
	FINEST("E Timestamp Echo Valid: %d", E);
	FINEST("S Startup: %d", S);
	FINEST("R1 Reserved1: %d", r1);
	FINEST("R2 Reserved2: %d", r2);
	FINEST("I Initiator Mark: %d", I);
	FINEST("R Responder Mark: %d", R);
	FINEST("t Time Critical Reverse Notification: %d", t);
	FINEST("T Time Critical Forward Notification: %d", T);
	FINEST("type: %02x E: %d; S: %d; b1: %d; b2: %d; I: %d; R: %d; t: %d; T: %d",
			type, E, S, r1, r2, I, R, t, T);

	//6. Read the timestamp
	uint16_t timestamp = ENTOHSP(pBuffer + 7);

	//7. Read the direction and messageId
	uint8_t chunkType = pBuffer[9];

	//8. Read message length
	uint16_t chunkLength = ENTOHSP(pBuffer + 10);
	if (GETAVAILABLEBYTESCOUNT(buffer) < chunkLength) {
		FATAL("Invalid message lenth");
		return;
	}

	//9. Ignore the first 12 bytes now. RTMFP transport layer ended
	buffer.Ignore(12);

	//10. Process it further
	ProcessSessionMessage(pSession, buffer, type, timestamp, chunkType, chunkLength);
}

void BaseRTMFPProtocol::ProcessSessionMessage(
		RTMFPSession *pSession,
		IOBuffer &buffer,
		uint8_t type,
		uint16_t timestamp,
		uint8_t chunkType,
		uint16_t chunkLength) {
	FINEST("type: %02x; timestamp: %04x; chunkType: %02x; chunkLength: %04x",
			type, timestamp, chunkType, chunkLength);
	switch (type) {
		case 0x0B:
			ProcessSessionMessage_0b(pSession, buffer, timestamp, chunkType, chunkLength);
			break;
		default:
			FATAL("Invalid message type: %02x", type);
			break;
	}
}

void BaseRTMFPProtocol::ProcessSessionMessage_0b(
		RTMFPSession *pSession,
		IOBuffer &buffer,
		uint16_t timestamp,
		uint8_t chunkType,
		uint16_t chunkLength) {
	switch (chunkType) {
		case 0x30:
			ProcessSessionMessage_0b_30(pSession, buffer, timestamp, chunkLength);
			break;
		case 0x38:
			ProcessSessionMessage_0b_38(pSession, buffer, timestamp, chunkLength);
			break;
		default:
			FATAL("Invalid chunk type: %02x", chunkType);
			break;
	}
}

#define __READ_U29(value) \
if (!_amf3Serializer.ReadU29(buffer, value)) { \
	FATAL("Unable to read U29 value: "#value); \
	return; \
} \
FINEST(#value": %u (0x%x)", value, value);

#define __CHECK_SIZE(size) \
if(GETAVAILABLEBYTESCOUNT(buffer)<(size)) {\
	FATAL("Invalid buffer size. Wanted: %u; got: %u",(size),GETAVAILABLEBYTESCOUNT(buffer)); \
	return; \
} \

#define __READ_BYTES_ARRAY(value,size) \
__CHECK_SIZE(size); \
memcpy((value),GETIBPOINTER(buffer),(size)); \
buffer.Ignore((size)); \
{ \
	string __tempString__; \
	for(uint32_t i=0;i<(size);i++) { \
		__tempString__+=format("%02x",(value)[i]); \
	} \
	FINEST(#value"[%d]: %s",(size), STR(__tempString__)); \
}

#define __READ_STRING(value,size) \
__CHECK_SIZE(size); \
value=string((char *)GETIBPOINTER(buffer),(size)); \
buffer.Ignore((size)); \
FINEST(#value"[%d]: `%s`",(size),STR(value));

void BaseRTMFPProtocol::ProcessSessionMessage_0b_30(
		RTMFPSession *pSession,
		IOBuffer &buffer,
		uint16_t timestamp,
		uint16_t chunkLength) {
	//1. read the first length
	uint32_t length1 = 0;
	__READ_U29(length1);
	if (GETAVAILABLEBYTESCOUNT(buffer) < length1 + 16) {
		FATAL("Invalid message lenth");
		return;
	}

	//2. read the second length
	uint32_t length2 = 0;
	__READ_U29(length2);
	if (GETAVAILABLEBYTESCOUNT(buffer) < length2 + 16) {
		FATAL("Invalid message lenth");
		return;
	}

	//3. Read the unknown byte
	uint8_t unknown1;
	__READ_BYTES_ARRAY(&unknown1, 1);

	//4. read the URI
	__READ_STRING(pSession->connectUri, length2 - 1);


	//5. Read the client tag
	__READ_BYTES_ARRAY(pSession->client.tag, 16);

	//6. Now, we start the real message. First, length of the client tag
	uint16_t cursor = 0;
	_outputBuffer.pChunk[cursor] = 0x10;
	cursor++;

	//7. Client tag
	memcpy(_outputBuffer.pChunk + cursor, pSession->client.tag, 16);
	cursor += 16;

	//8. Server cookie length
	_outputBuffer.pChunk[cursor] = 0x40;
	cursor++;

	//9. Server cookie
	RAND_bytes(pSession->server.cookie, 64);
	memcpy(_outputBuffer.pChunk + cursor, pSession->server.cookie, 64);
	cursor += 64;

	//10. Unknown 2 bytes which looks like they are a length (01) and
	//the payload (0A)
	_outputBuffer.pChunk[cursor] = 0x01;
	_outputBuffer.pChunk[cursor + 1] = 0x0a;
	cursor += 2;

	//11. length of the server certificate (which is 64+1) and the type.
	//That one byte is 0x0E which I think is some sort of type
	_outputBuffer.pChunk[cursor] = 0x41;
	_outputBuffer.pChunk[cursor + 1] = 0x0e;
	cursor += 2;

	//12. Server's certificate
	memcpy(_outputBuffer.pChunk + cursor, _serverCert, 64);
	cursor += 64;

	//13. A succession of 3 length/payload bytes which I have no idea about
	uint8_t unknownBytes[] = {0x02, 0x15, 0x02, 0x02, 0x15, 0x05, 0x02, 0x15, 0x0E};
	memcpy(_outputBuffer.pChunk + cursor, unknownBytes, sizeof (unknownBytes));
	cursor += sizeof (unknownBytes);

	//14. Done. Wrap it up and send it to the client
	SendToClient(pSession, 0x0b, 0x70, cursor);
}

void BaseRTMFPProtocol::ProcessSessionMessage_0b_38(
		RTMFPSession *pSession,
		IOBuffer &buffer,
		uint16_t timestamp,
		uint16_t chunkLength) {
	//1. Read the client crc
	uint8_t clientCrc[4];
	__READ_BYTES_ARRAY(clientCrc, 4);
	pSession->id = ENTOHLP(clientCrc);

	//2. Read server cookie length
	uint32_t clientCookieLength = 0;
	__READ_U29(clientCookieLength);
	if (clientCookieLength != 64) {
		FATAL("Invalid server cookie length");
		return;
	}

	//3. Read the server cookie
	uint8_t serverCookieEcho[64];
	__READ_BYTES_ARRAY(serverCookieEcho, clientCookieLength);
	if (memcmp(pSession->server.cookie, serverCookieEcho, clientCookieLength) != 0) {
		FATAL("Invalid server cookie");
		return;
	}

	//4. read the first length
	uint32_t length1 = 0;
	__READ_U29(length1);

	//5. read the second length
	uint32_t length2 = 0;
	__READ_U29(length2);
	if ((length2 - 2) != 128) {
		FATAL("Invalid length2");
		return;
	}

	//6. Read 2 unknown bytes
	uint8_t unknown2[2];
	__READ_BYTES_ARRAY(unknown2, 2);

	//7. Read the client public key
	__READ_BYTES_ARRAY(pSession->client.publicKey, length2 - 2);

	//8. read the third length
	uint32_t length3 = 0;
	__READ_U29(length3);
	if (length3 != 0x4c) {
		FATAL("Invalid length");
		return;
	}

	//12. Read client cookie
	__READ_BYTES_ARRAY(pSession->client.cookie, length3);

	//13. Create the shared key
	if (!pSession->dhwComputed) {
		if (!pSession->dhw.Initialize()) {
			FATAL("Unable to initialize DH wrapper");
			return;
		}
		pSession->dhw.CopyPublicKey(pSession->server.publicKey, 128);
		if (!pSession->dhw.CreateSharedKey(pSession->client.publicKey, 128)) {
			FATAL("Unable to create shared key");
			return;
		}
		pSession->dhwComputed = true;
	}

	//14. compute farId

	EVP_Digest(pSession->client.publicKey, 128, pSession->client.id, NULL, EVP_sha256(), NULL);
	string temp = "";
	for (uint32_t i = 0; i < 32; i++) {
		temp += format("%02x", pSession->client.id[i]);
	}
	FINEST("farId: %s", STR(temp));

	//15. compute nearId
	EVP_Digest(pSession->server.publicKey, 128, pSession->server.id, NULL, EVP_sha256(), NULL);
	temp = "";
	for (uint32_t i = 0; i < 32; i++) {
		temp += format("%02x", pSession->server.id[i]);
	}
	FINEST("nearId: %s", STR(temp));

	//16. Start building the response. Generate a random session ID
	uint16_t cursor = 0;
	pSession->newId = rand();
	EHTONLP(_outputBuffer.pChunk + cursor, pSession->newId);
	cursor += 4;

	//17. Put the u29 length which is actually 0x8b bytes
	_outputBuffer.pChunk[cursor] = 0x81;
	_outputBuffer.pChunk[cursor + 1] = 0x0b;
	cursor += 2;

	//18. Put some unknown bytes. They are looking like lengt-value type
	uint8_t unknown5[] = {0x03, 0x1a, 0x00, 0x00, 0x02, 0x1e, 0x00};
	memcpy(_outputBuffer.pChunk + cursor, unknown5, sizeof (unknown5));
	cursor += sizeof (unknown5);

	//19. Put another u29 length (which is actually 0x82) followed by 2 unknown bytes
	//Basically, is the server public key, padded by those 2 bytes, padded by the length
	uint8_t unknown6[] = {0x81, 0x02, 0x0d, 0x02};
	memcpy(_outputBuffer.pChunk + cursor, unknown6, sizeof (unknown6));
	cursor += sizeof (unknown6);

	//20. Put the server's public key
	memcpy(_outputBuffer.pChunk + cursor, pSession->server.publicKey, 128);
	cursor += 128;

	//21. Put the last unknown byte
	_outputBuffer.pChunk[cursor] = 0x58;
	cursor++;

	//22. Send it
	SendToClient(pSession, 0x0b, 0x78, cursor);

	//23. Compute keys
	pSession->ComputeKeys();
}

#endif /* HAS_PROTOCOL_RTMFP */
