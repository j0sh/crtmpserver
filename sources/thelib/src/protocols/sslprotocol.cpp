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


#include <openssl/ssl.h>

#include "protocols/sslprotocol.h"
#include "buffering/iobuffer.h"



#define MAX_SSL_READ_BUFFER 8192

map<string, SSL_CTX *> SSLProtocol::_pGlobalContexts;
bool SSLProtocol::_libraryInitialized = false;

SSLProtocol::SSLProtocol()
: BaseProtocol(PT_SSL) {
    _pGlobalSSLContext1 = NULL;
    _pSSL = NULL;
    _pReadBuffer = new uint8_t[MAX_SSL_READ_BUFFER];
    FINEST("SSL protocol created");
}

SSLProtocol::~SSLProtocol() {
    if (_pSSL != NULL) {
        SSL_free(_pSSL);
        _pSSL = NULL;
    }
    delete[] _pReadBuffer;
    FINEST("SSL protocol deleted");
}

bool SSLProtocol::Initialize(Variant &parameters) {
    //1. Comput the hash on key/cert pair and see
    //if we have a global context with that hash
    string hash = "";
    if (parameters["hash"] != V_STRING) {
        if (parameters[CONF_SSL_KEY] != V_STRING
                || parameters[CONF_SSL_CERT] != V_STRING) {
            FATAL("No key/cert provided");
            return false;
        }
        hash = md5((string) parameters[CONF_SSL_KEY]
                + (string) parameters[CONF_SSL_CERT], true);
        parameters["hash"] = hash;
    } else {
        hash = (string) parameters["hash"];
    }
    string key = parameters[CONF_SSL_KEY];
    string cert = parameters[CONF_SSL_CERT];
    _pGlobalSSLContext1 = _pGlobalContexts[hash];

    //2. Initialize the global context based on the specified
    //key/cert pair if we don't have it
    if (_pGlobalSSLContext1 == NULL) {
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

        //4. prepare the global ssl context
        _pGlobalSSLContext1 = SSL_CTX_new(SSLv23_method());
        if (_pGlobalSSLContext1 == NULL) {
            FATAL("Unable to create global SSL context");
            return false;
        }

        //5. setup the certificate
        if (SSL_CTX_use_certificate_file(_pGlobalSSLContext1, STR(cert),
                SSL_FILETYPE_PEM) <= 0) {
            FATAL("Unable to load certificate %s; Error(s) was: %s",
                    STR(cert),
                    STR(GetSSLErrors()));
            SSL_CTX_free(_pGlobalSSLContext1);
            _pGlobalSSLContext1 = NULL;
            return false;
        }

        //6. setup the private key
        if (SSL_CTX_use_PrivateKey_file(_pGlobalSSLContext1, STR(key),
                SSL_FILETYPE_PEM) <= 0) {
            FATAL("Unable to load key %s; Error(s) was: %s",
                    STR(key),
                    STR(GetSSLErrors()));
            SSL_CTX_free(_pGlobalSSLContext1);
            _pGlobalSSLContext1 = NULL;
            return false;
        }

        //7. disable client certificate authentication
        SSL_CTX_set_verify(_pGlobalSSLContext1, SSL_VERIFY_NONE, NULL);

        //8. Store the global context for later usage
        _pGlobalContexts[hash] = _pGlobalSSLContext1;
    }

    //9. create connection SSL context
    _pSSL = SSL_new(_pGlobalSSLContext1);
    if (_pSSL == NULL) {
        FATAL("Unable to create SSL connection context");
        return false;
    }

    SSL_set_bio(_pSSL, BIO_new(BIO_s_mem()), BIO_new(BIO_s_mem()));
    SSL_set_accept_state(_pSSL);

    return true;
}

bool SSLProtocol::AllowFarProtocol(uint64_t type) {
    if (type == PT_TCP)
        return true;
    FATAL("Far protocol %d not accepted", type);
    return false;
}

bool SSLProtocol::AllowNearProtocol(uint64_t type) {
    return true;
}

bool SSLProtocol::EnqueueForOutbound() {
    IOBuffer *pBuffer = _pNearProtocol->GetOutputBuffer();
    if (pBuffer == NULL)
        return true;
    if (SSL_write(_pSSL, GETIBPOINTER(*pBuffer), GETAVAILABLEBYTESCOUNT(*pBuffer))
            != (int32_t) GETAVAILABLEBYTESCOUNT(*pBuffer)) {
        FATAL("Unable to write %d bytes", GETAVAILABLEBYTESCOUNT(*pBuffer));
        return false;
    }
    pBuffer->IgnoreAll();
    return PerformIO();
}

IOBuffer * SSLProtocol::GetOutputBuffer() {
    if (GETAVAILABLEBYTESCOUNT(_outputBuffer) > 0)
        return &_outputBuffer;
    return NULL;
}

IOBuffer * SSLProtocol::GetInputBuffer() {
    return &_inputBuffer;
}

bool SSLProtocol::SignalInputData(int32_t recvAmount) {
    ASSERT("OPERATION NOT SUPPORTED");
    return false;
}

bool SSLProtocol::SignalInputData(IOBuffer &buffer) {
    //2. get the SSL input buffer
    BIO *pInBio = SSL_get_rbio(_pSSL);

    //3. dump all the data from the network inside the ssl input
    BIO_write(pInBio, GETIBPOINTER(buffer),
            GETAVAILABLEBYTESCOUNT(buffer));
    buffer.IgnoreAll();

    //4. Try to do the handshake
    int32_t errorCode = SSL_ERROR_NONE;
    if (!SSL_is_init_finished(_pSSL)) {
        errorCode = SSL_accept(_pSSL);
        if (errorCode < 0) {
            int32_t error = SSL_get_error(_pSSL, errorCode);
            if (error != SSL_ERROR_WANT_READ &&
                    error != SSL_ERROR_WANT_WRITE) {
                FATAL("Unable to accept SSL connection: %d; %s",
                        error, STR(GetSSLErrors()));
                return false;
            }
        }
    }

    //5. if the init is made, read the actual data an put it in the
    //descrypted input buffer
    if (SSL_is_init_finished(_pSSL)) {
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
    }

    //6. If we have pending data inside the decrypted buffer, bubble it up
    //on the protocol stack
    if (GETAVAILABLEBYTESCOUNT(_inputBuffer) > 0) {
        if (_pNearProtocol != NULL) {
            if (!_pNearProtocol->SignalInputData(_inputBuffer)) {
                FATAL("Unable to signal near protocol for new data");
                return false;
            }
        }
    }

    //7. After the data was sent on the upper layers, we might have outstanding
    //data that needs to be sent. Also this outstanding data might come from
    //the ssl handshake itself. So we do I/O
    return PerformIO();
}

string SSLProtocol::DumpBIO(BIO *pBIO) {
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
    formatString += "num_read: %u\n";
    formatString += "num_write: %u";
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
            pBIO->num_read,
            pBIO->num_write);
}

string SSLProtocol::GetSSLErrors() {
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

void SSLProtocol::InitRandGenerator() {
    srand((uint32_t) time(NULL));

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

bool SSLProtocol::PerformIO() {
    //1. Put the data from SSL output buffer inside our
    //protocol output buffer
    if (!_outputBuffer.ReadFromBIO(SSL_get_wbio(_pSSL))) {
        FATAL("Unable to transfer data from outBIO to outputBuffer");
        return false;
    }

    //2. Enqueue the protocol for outbound if we have data that needs
    //to be sent
    if (GETAVAILABLEBYTESCOUNT(_outputBuffer) > 0) {
        if (_pFarProtocol != NULL) {
            return _pFarProtocol->EnqueueForOutbound();
        }
    }

    //3. Done
    return true;
}


