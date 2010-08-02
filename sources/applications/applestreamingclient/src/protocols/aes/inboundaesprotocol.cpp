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




#include "protocols/aes/inboundaesprotocol.h"
#include "protocols/protocolfactory.h"
#include "protocols/httpbuff/httpbufferprotocol.h"
#include "protocols/ts/inboundtsprotocol.h"
#include "clientcontext.h"

InboundAESProtocol::InboundAESProtocol()
: GenericProtocol(PT_INBOUND_AES) {
	_lastChunk = false;
	_pIV = new uint8_t[16];
	memset(_pIV, 0, 16);
	_pKey = new uint8_t[16];
	memset(_pKey, 0, 16);
	memset(&_decContex, 0, sizeof (EVP_CIPHER_CTX));
	_totalDecrypted = 0;
}

InboundAESProtocol::~InboundAESProtocol() {
	EVP_CIPHER_CTX_cleanup(&_decContex);
	memset(&_decContex, 0, sizeof (EVP_CIPHER_CTX));
	delete[] _pIV;
	delete[] _pKey;
}

IOBuffer * InboundAESProtocol::GetInputBuffer() {
	return &_inputBuffer;
}

bool InboundAESProtocol::Initialize(Variant &parameters) {
	//FINEST("parameters:\n%s", STR(parameters.ToString()));
	if (!GenericProtocol::Initialize(parameters)) {
		FATAL("Unable to initialize AES protocol");
		return false;
	}

	memset(_pIV, 0, 16);
	put_htonll(_pIV + 8, (uint64_t) parameters["payload"]["iv"]);
	memcpy(_pKey, STR(parameters["payload"]["key"]), 16);

	//	FINEST(" IV: %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
	//			_pIV[0], _pIV[1], _pIV[2], _pIV[3], _pIV[4], _pIV[5], _pIV[6], _pIV[7],
	//			_pIV[8], _pIV[9], _pIV[10], _pIV[11], _pIV[12], _pIV[13], _pIV[14], _pIV[15]);
	//
	//	FINEST("KEY: %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
	//			_pKey[0], _pKey[1], _pKey[2], _pKey[3], _pKey[4], _pKey[5], _pKey[6], _pKey[7],
	//			_pKey[8], _pKey[9], _pKey[10], _pKey[11], _pKey[12], _pKey[13], _pKey[14], _pKey[15]);

	_lastChunk = false;
	_inputBuffer.IgnoreAll();
	_tempBuffer.IgnoreAll();

	EVP_CIPHER_CTX_cleanup(&_decContex);
	memset(&_decContex, 0, sizeof (EVP_CIPHER_CTX));
	EVP_CIPHER_CTX_init(&_decContex);
	EVP_DecryptInit_ex(&_decContex, EVP_aes_128_cbc(), NULL, _pKey, _pIV);
	EVP_CIPHER_CTX_set_padding(&_decContex, 0);

	return true;
}

bool InboundAESProtocol::AllowFarProtocol(uint64_t type) {
	return type == PT_HTTP_BUFF;
}

bool InboundAESProtocol::AllowNearProtocol(uint64_t type) {
	return type == PT_INBOUND_TS;
}

bool InboundAESProtocol::SignalInputData(int32_t recvAmount) {
	NYIR;
}

bool InboundAESProtocol::SignalInputData(IOBuffer &buffer) {
	//2. Get the size
	int32_t size = GETAVAILABLEBYTESCOUNT(buffer);

	//3. Get the multiple of block size
	int32_t safeSize = (size / 16)*16;

	//4. Get the outputBufferSize
	int32_t bufferSize = safeSize + 16;

	//3. Do we have data?
	if (size == 0)
		return true;

	//4. Make room for the descrypted data
	_tempBuffer.IgnoreAll();
	_tempBuffer.EnsureSize(bufferSize);

	//5. Decrypt
	uint8_t *pTempData = GETIBPOINTER(_tempBuffer);
	int decryptedSize = 0;
	int decryptedFinalSize = 0;
	uint32_t padding = 0;

	EVP_DecryptUpdate(&_decContex, pTempData, &decryptedSize, GETIBPOINTER(buffer), safeSize);
	_totalDecrypted += decryptedSize;

	//6. Decrypt leftovers
	bool transferCompleted = false;
	if (((HTTPBufferProtocol *) GetFarProtocol())->TransferCompleted()) {
		transferCompleted = true;
		EVP_DecryptFinal_ex(&_decContex,
				pTempData + decryptedSize,
				&decryptedFinalSize);
		_totalDecrypted += decryptedFinalSize;
		//		uint32_t chunkSize = ((InboundTSProtocol *) GetNearProtocol())->GetChunkSize();
		//		if (chunkSize == 0) {
		//			FATAL("Invalid TS chunk size");
		//			return false;
		//		}
		WARN("chunkSize hardcoded to 188 bytes");
		uint32_t chunkSize = 188;
		padding = _totalDecrypted - (((uint32_t) (_totalDecrypted / chunkSize)) * chunkSize);
		//		ASSERT("_totalDecrypted: %d; padding: %d; got: %d; wanted: %d",
		//				_totalDecrypted, padding,
		//				_totalDecrypted - padding,
		//				((OutboundHTTPProtocol *) GetFarProtocol())->GetContentLength());
		if (size != decryptedSize + decryptedFinalSize) {
			FINEST("size: %d; safeSize: %d; bufferSize: %d; decryptedSize: %d; decryptedFinalSize: %d",
					size,
					safeSize,
					bufferSize,
					decryptedSize,
					decryptedFinalSize);
			FATAL("Malformed AES content. It should always be 16 bytes aligned");
			return false;
		}
	}

	//7. Ignore decrypted data
	buffer.Ignore(decryptedSize + decryptedFinalSize);

	//8. Prepare the buffer for the next protocol in the stack
	_inputBuffer.ReadFromBuffer(pTempData, decryptedSize + decryptedFinalSize - padding);

	//	//9. Continue processing with the next protocol in the stack
	//	if (!_pNearProtocol->SignalInputData(_inputBuffer)) {
	//		FATAL("Unable to signal upper protocols");
	//		return false;
	//	}

	//9. Get the context
	ClientContext *pContext = GetContext();
	if (pContext == NULL) {
		FATAL("Unable to get context");
		return false;
	}

	//10. Feed the data
	if (!pContext->SignalAVDataAvailable(_inputBuffer)) {
		FATAL("Unable to signal ts A/V data available");
		return false;
	}

	//11. Done
	return true;
}
