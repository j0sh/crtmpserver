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

#include "protocols/ssl/basesslprotocol.h"

#define MAX_SSL_READ_BUFFER 65536

map<string, SSL_CTX *> BaseSSLProtocol::_pGlobalContexts;
bool BaseSSLProtocol::_libraryInitialized = false;

BaseSSLProtocol::BaseSSLProtocol(uint64_t type)
: BaseProtocol(type) {
	_pGlobalSSLContext = NULL;
	_pSSL = NULL;
	_sslHandshakeCompleted = false;
	_pReadBuffer = new uint8_t[MAX_SSL_READ_BUFFER];
}

BaseSSLProtocol::~BaseSSLProtocol() {
	if (_pSSL != NULL) {
		SSL_free(_pSSL);
		_pSSL = NULL;
	}
	delete[] _pReadBuffer;
}

bool BaseSSLProtocol::Initialize(Variant &parameters) {
	//1. Initialize the SSL library
	if (!_libraryInitialized) {
		//3. This is the first time we use the library. So we have to
		//initialize it first
		SSL_library_init();

		//init readable error messages
		SSL_load_error_strings();
		ERR_load_SSL_strings();
		ERR_load_CRYPTO_strings();
		ERR_load_crypto_strings();
		OpenSSL_add_all_algorithms();
		OpenSSL_add_all_ciphers();
		OpenSSL_add_all_digests();

		//initialize the random numbers generator
		InitRandGenerator();
		_libraryInitialized = false;
	}

	//2. Initialize the global context
	if (!InitGlobalContext(parameters)) {
		FATAL("Unable to initialize global context");
		return false;
	}

	//3. create connection SSL context
	_pSSL = SSL_new(_pGlobalSSLContext);
	if (_pSSL == NULL) {
		FATAL("Unable to create SSL connection context");
		return false;
	}

	//4. setup the I/O buffers
	SSL_set_bio(_pSSL, BIO_new(BIO_s_mem()), BIO_new(BIO_s_mem()));

	return DoHandshake();
}

bool BaseSSLProtocol::AllowFarProtocol(uint64_t type) {
	if (type == PT_TCP)
		return true;
	return false;
}

bool BaseSSLProtocol::AllowNearProtocol(uint64_t type) {
	return true;
}

bool BaseSSLProtocol::EnqueueForOutbound() {
	//1. Is the SSL handshake completed?
	if (!_sslHandshakeCompleted) {
		return DoHandshake();
	}

	//2. Do we have some outstanding data?
	IOBuffer *pBuffer = _pNearProtocol->GetOutputBuffer();
	if (pBuffer == NULL)
		return true;

	//3. Encrypt the outstanding data
	if (SSL_write(_pSSL, GETIBPOINTER(*pBuffer), GETAVAILABLEBYTESCOUNT(*pBuffer))
			!= (int32_t) GETAVAILABLEBYTESCOUNT(*pBuffer)) {
		FATAL("Unable to write %u bytes", GETAVAILABLEBYTESCOUNT(*pBuffer));
		return false;
	}
	pBuffer->IgnoreAll();

	//4. Do the actual I/O
	return PerformIO();
}

IOBuffer * BaseSSLProtocol::GetOutputBuffer() {
	if (GETAVAILABLEBYTESCOUNT(_outputBuffer) > 0)
		return &_outputBuffer;
	return NULL;
}

IOBuffer * BaseSSLProtocol::GetInputBuffer() {
	return &_inputBuffer;
}

bool BaseSSLProtocol::SignalInputData(int32_t recvAmount) {
	ASSERT("OPERATION NOT SUPPORTED");
	return false;
}

bool BaseSSLProtocol::SignalInputData(IOBuffer &buffer) {
	//1. get the SSL input buffer
	BIO *pInBio = SSL_get_rbio(_pSSL);

	//2. dump all the data from the network inside the ssl input
	BIO_write(pInBio, GETIBPOINTER(buffer),
			GETAVAILABLEBYTESCOUNT(buffer));
	buffer.IgnoreAll();

	//3. Do we have to do some handshake?
	if (!_sslHandshakeCompleted) {
		if (!DoHandshake()) {
			FATAL("Unable to do the SSL handshake");
			return false;
		}
		if (!_sslHandshakeCompleted) {
			return true;
		}
	}

	//4. Read the actual data an put it in the descrypted input buffer
	int32_t read = 0;
	while ((read = SSL_read(_pSSL, _pReadBuffer, MAX_SSL_READ_BUFFER)) > 0) {
		_inputBuffer.ReadFromBuffer(_pReadBuffer, (uint32_t) read);
	}
	if (read < 0) {
		int32_t error = SSL_get_error(_pSSL, read);
		if (error != SSL_ERROR_WANT_READ &&
				error != SSL_ERROR_WANT_WRITE) {
			FATAL("Unable to read data: %d", error);
			return false;
		}
	}

	//6. If we have pending data inside the decrypted buffer, bubble it up on the protocol stack
	if (GETAVAILABLEBYTESCOUNT(_inputBuffer) > 0) {
		if (_pNearProtocol != NULL) {
			if (!_pNearProtocol->SignalInputData(_inputBuffer)) {
				FATAL("Unable to signal near protocol for new data");
				return false;
			}
		}
	}

	//7. After the data was sent on the upper layers, we might have outstanding
	//data that needs to be sent.
	return PerformIO();
}

bool BaseSSLProtocol::PerformIO() {
	//1. Put the data from SSL output buffer inside our protocol output buffer
	if (!_outputBuffer.ReadFromBIO(SSL_get_wbio(_pSSL))) {
		FATAL("Unable to transfer data from outBIO to outputBuffer");
		return false;
	}

	//2. Enqueue the protocol for outbound if we have data that needs to be sent
	if (GETAVAILABLEBYTESCOUNT(_outputBuffer) > 0) {
		if (_pFarProtocol != NULL) {
			return _pFarProtocol->EnqueueForOutbound();
		}
	}

	//3. Done
	return true;
}

string BaseSSLProtocol::GetSSLErrors() {
	string result = "";
	uint32_t errorCode;
	char *pTempBuffer = new char[4096];
	while ((errorCode = ERR_get_error()) != 0) {
		memset(pTempBuffer, 0, 4096);
		ERR_error_string_n(errorCode, pTempBuffer, 4095);
		result += "\n";
		result += pTempBuffer;
	}
	delete[] pTempBuffer;
	return result;
}

string BaseSSLProtocol::DumpBIO(BIO *pBIO) {
	string formatString;
	formatString = "method: %p\n";
	formatString += "callback: %p\n";
	formatString += "cb_arg: %p\n";
	formatString += "init: %d\n";
	formatString += "shutdown: %d\n";
	formatString += "flags: %d\n";
	formatString += "retry_reason: %d\n";
	formatString += "num: %d\n";
	formatString += "ptr: %p\n";
	formatString += "next_bio: %p\n";
	formatString += "prev_bio: %p\n";
	formatString += "references: %d\n";
	formatString += "num_read: %"PRId64"\n";
	formatString += "num_write: %"PRId64;
	return format(formatString,
			pBIO->method,
			pBIO->callback,
			pBIO->cb_arg,
			pBIO->init,
			pBIO->shutdown,
			pBIO->flags,
			pBIO->retry_reason,
			pBIO->num,
			pBIO->ptr,
			pBIO->next_bio,
			pBIO->prev_bio,
			pBIO->references,
			(int64_t) pBIO->num_read,
			(int64_t) pBIO->num_write);
}

void BaseSSLProtocol::InitRandGenerator() {
	uint32_t length = 16;
	uint32_t *pBuffer = new uint32_t[length];

	while (RAND_status() == 0) {
		for (uint32_t i = 0; i < length; i++) {
			pBuffer[i] = rand();
		}

		RAND_seed(pBuffer, length * 4);
	}

	delete[] pBuffer;
}


