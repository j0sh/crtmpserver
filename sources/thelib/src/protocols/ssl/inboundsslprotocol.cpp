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


#include "protocols/ssl/inboundsslprotocol.h"

InboundSSLProtocol::InboundSSLProtocol()
: BaseSSLProtocol(PT_INBOUND_SSL) {

}

InboundSSLProtocol::~InboundSSLProtocol() {
}

bool InboundSSLProtocol::InitGlobalContext(Variant &parameters) {
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
	_pGlobalSSLContext = _pGlobalContexts[hash];

	//2. Initialize the global context based on the specified
	//key/cert pair if we don't have it
	if (_pGlobalSSLContext == NULL) {
		//3. prepare the global ssl context
		_pGlobalSSLContext = SSL_CTX_new(SSLv23_method());
		if (_pGlobalSSLContext == NULL) {
			FATAL("Unable to create global SSL context");
			return false;
		}

		//4. setup the certificate
		if (SSL_CTX_use_certificate_file(_pGlobalSSLContext, STR(cert),
				SSL_FILETYPE_PEM) <= 0) {
			FATAL("Unable to load certificate %s; Error(s) was: %s",
					STR(cert),
					STR(GetSSLErrors()));
			SSL_CTX_free(_pGlobalSSLContext);
			_pGlobalSSLContext = NULL;
			return false;
		}

		//5. setup the private key
		if (SSL_CTX_use_PrivateKey_file(_pGlobalSSLContext, STR(key),
				SSL_FILETYPE_PEM) <= 0) {
			FATAL("Unable to load key %s; Error(s) was: %s",
					STR(key),
					STR(GetSSLErrors()));
			SSL_CTX_free(_pGlobalSSLContext);
			_pGlobalSSLContext = NULL;
			return false;
		}

		//6. disable client certificate authentication
		SSL_CTX_set_verify(_pGlobalSSLContext, SSL_VERIFY_NONE, NULL);

		//7. Store the global context for later usage
		_pGlobalContexts[hash] = _pGlobalSSLContext;
		INFO("SSL server context initialized");
	}

	return true;
}

bool InboundSSLProtocol::DoHandshake() {
	if (_sslHandshakeCompleted)
		return true;

	int32_t errorCode = SSL_ERROR_NONE;

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

	if (!PerformIO()) {
		FATAL("Unable to perform I/O");
		return false;
	}

	_sslHandshakeCompleted = SSL_is_init_finished(_pSSL);

	return true;
}

