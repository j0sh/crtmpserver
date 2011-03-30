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

#include "protocols/ssl/outboundsslprotocol.h"

OutboundSSLProtocol::OutboundSSLProtocol()
: BaseSSLProtocol(PT_OUTBOUND_SSL) {

}

OutboundSSLProtocol::~OutboundSSLProtocol() {
}

bool OutboundSSLProtocol::InitGlobalContext(Variant &parameters) {
	//1. get the hash which is always the same for client connetions
	string hash = "clientConnection";
	_pGlobalSSLContext = _pGlobalContexts[hash];
	if (_pGlobalSSLContext == NULL) {
		//2. prepare the global ssl context
		_pGlobalSSLContext = SSL_CTX_new(SSLv23_method());
		if (_pGlobalSSLContext == NULL) {
			FATAL("Unable to create global SSL context");
			return false;
		}

		//3. Store the global context for later usage
		_pGlobalContexts[hash] = _pGlobalSSLContext;
	}

	return true;
}

bool OutboundSSLProtocol::DoHandshake() {
	if (_sslHandshakeCompleted)
		return true;
	int32_t errorCode = SSL_ERROR_NONE;
	errorCode = SSL_connect(_pSSL);
	if (errorCode < 0) {
		int32_t error = SSL_get_error(_pSSL, errorCode);
		if (error != SSL_ERROR_WANT_READ &&
				error != SSL_ERROR_WANT_WRITE) {
			FATAL("Unable to connect SSL: %d; %s", error, STR(GetSSLErrors()));
			return false;
		}
	}

	_sslHandshakeCompleted = SSL_is_init_finished(_pSSL);

	if (!PerformIO()) {
		FATAL("Unable to perform I/O");
		return false;
	}

	if (_sslHandshakeCompleted)
		return EnqueueForOutbound();

	return true;
}
