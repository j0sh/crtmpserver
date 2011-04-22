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

#include "protocols/key/inboundkeyprotocol.h"
#include "protocols/protocolfactory.h"
#include "protocols/http/outboundhttpprotocol.h"
#include "clientcontext.h"
using namespace app_applestreamingclient;

InboundKeyProtocol::InboundKeyProtocol()
: GenericProtocol(PT_INBOUND_KEY) {

}

InboundKeyProtocol::~InboundKeyProtocol() {
}

bool InboundKeyProtocol::AllowFarProtocol(uint64_t type) {
	return type == PT_OUTBOUND_HTTP;
}

bool InboundKeyProtocol::AllowNearProtocol(uint64_t type) {
	return false;
}

bool InboundKeyProtocol::SignalInputData(int32_t recvAmount) {
	NYIR;
}

bool InboundKeyProtocol::SignalInputData(IOBuffer &buffer) {
	//1. Get the context
	ClientContext *pContext = GetContext();
	if (pContext == NULL) {
		FATAL("Unable to get the context");
		return false;
	}

	//2. Get the transport
	OutboundHTTPProtocol *pHttpProtocol = (OutboundHTTPProtocol *) GetFarProtocol();
	assert(pHttpProtocol != NULL);

	//3. See if we have the 200 OK
	if (!pHttpProtocol->Is200OK()) {
		FATAL("The HTTP request failed");
		return false;
	}

	//4. See if we have a complete transfer
	if (!pHttpProtocol->TransferCompleted()) {
		return true;
	}

	//5. Get the string from the input buffer
	string temp = string((char *) GETIBPOINTER(buffer),
			GETAVAILABLEBYTESCOUNT(buffer));
	buffer.IgnoreAll();

	//6. Unbase64 the encrypted key
	string encryptedKey = unb64(temp);
	if (encryptedKey.size() != 16) {
		FATAL("Invalid key length: %"PRIz"u", encryptedKey.size());
		return false;
	}

	//7. Unbase64 the key password
	string keyPassword = unb64(pContext->GetConnectingString().keyPassword);
	if (keyPassword.size() < 16) {
		FATAL("Invalid key length");
		return false;
	}

	//8. Decrypt the key
	AES_KEY decKey;
	AES_set_decrypt_key((unsigned char *) STR(keyPassword), 128, &decKey);
	unsigned char finalKey[16];
	unsigned char iv[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	AES_cbc_encrypt((unsigned char *) STR(encryptedKey), finalKey, 16, &decKey, iv, AES_DECRYPT);

	//9. Store the key inside parameters
	Variant &parameters = GetCustomParameters();
	parameters["payload"]["key"] = string((char *) finalKey, 16);

	//10. Signal the context about the new key
	if (!pContext->SignalAESKeyAvailable(parameters)) {
		FATAL("Unable to signal AES key available");
		return false;
	}

	//11. Job done. Disconnect
	EnqueueForDelete();

	//12. Done
	return true;
}
