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
using namespace app_applestreamingclient;

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
	if (!GenericProtocol::Initialize(parameters)) {
		FATAL("Unable to initialize AES protocol");
		return false;
	}

	memset(_pIV, 0, 16);
	EHTONLLP(_pIV, (uint64_t) parameters["payload"]["iv"]);
	memcpy(_pKey, STR(parameters["payload"]["key"]), 16);

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
		WARN("chunkSize hardcoded to 188 bytes");
		uint32_t chunkSize = 188;
		padding = _totalDecrypted - (((uint32_t) (_totalDecrypted / chunkSize)) * chunkSize);
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
