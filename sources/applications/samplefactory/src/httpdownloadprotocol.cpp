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

#include "httpdownloadprotocol.h"
#include "localdefines.h"
#include "protocols/protocolfactorymanager.h"
#include "protocols/http/outboundhttpprotocol.h"
#include "netio/netio.h"

using namespace app_samplefactory;

HTTPDownloadProtocol::HTTPDownloadProtocol()
: BaseProtocol(PT_HTTP_DOWNLOAD_PROTOCOL) {

}

HTTPDownloadProtocol::~HTTPDownloadProtocol() {
}

bool HTTPDownloadProtocol::Initialize(Variant &parameters) {
	//save the parameters
	GetCustomParameters() = parameters;
	return true;
}

IOBuffer * HTTPDownloadProtocol::GetOutputBuffer() {
	//This function should return the buffer
	//ready to be transmitted over the wire or
	//NULL if no data needs to be transmitted
	//Also, there is NO guarantee that after
	//a I/O operation ALL the data is consumed
	//The I/O layer will try to send as many data as possible from this buffer
	//and adjust it afterwards. Rule of thumb: after writing into this buffer,
	//you should forget about it and NEVER try to modify data inside it. You
	//are only permitted to append data to it
	if (GETAVAILABLEBYTESCOUNT(_outputBuffer) != 0)
		return &_outputBuffer;
	return NULL;
}

bool HTTPDownloadProtocol::AllowFarProtocol(uint64_t type) {
	//we only allow outbound HTTP for the far protocol
	return type == PT_OUTBOUND_HTTP;
}

bool HTTPDownloadProtocol::AllowNearProtocol(uint64_t type) {
	//this is an endpoint protocol so we don't accept
	//anything on top of it
	return false;
}

bool HTTPDownloadProtocol::SignalInputData(int32_t recvAmount) {
	//this protocol doesn't have a carrier attached directly so
	//we will never operate on file descriptors
	ASSERT("Operation not supported");
	return false;
}

bool HTTPDownloadProtocol::SignalInputData(IOBuffer &buffer) {
	//1. Get the HTTP protocol
	OutboundHTTPProtocol *pHTTP = (OutboundHTTPProtocol *) GetFarProtocol();

	//2. Is the transfer completed?
	if (!pHTTP->TransferCompleted()) {
		FINEST("HTTP transfer not completed yet");
		return true;
	}

	//3. Just dump the data (as string) and finish
	string data = string((const char *) GETIBPOINTER(buffer), GETAVAILABLEBYTESCOUNT(buffer));
	FINEST("data:\n%s", STR(data));

	//4. consume the buffer
	buffer.IgnoreAll();

	//5. We are done
	return true;
}

bool HTTPDownloadProtocol::DoSimpleGETRequestWithSomePayload(string stringUri, string payload) {
	//1. Split the URL into components
	URI uri;
	if (!URI::FromString(stringUri, true, uri)) {
		FATAL("Invalid uri: %s", STR(stringUri));
		return false;
	}

	//2. resolve the protocol chain types
	vector<uint64_t> chain = ProtocolFactoryManager::ResolveProtocolChain("httpDownload");

	//3. Prepare the custom parameters (the payload)
	Variant parameters;
	parameters["uri"] = uri;
	parameters["payload"] = payload;

	//4. Start the HTTP request
	if (!TCPConnector<HTTPDownloadProtocol>::Connect(uri.ip(), uri.port(), chain,
			parameters)) {
		FATAL("Unable to open connection");
		return false;
	}

	//5. Done :)
	return true;
}

bool HTTPDownloadProtocol::SignalProtocolCreated(BaseProtocol *pProtocol, Variant &parameters) {
	//1. Did we connected?
	if (pProtocol == NULL) {
		FATAL("Connection failed");
		return false;
	}

	//2. Send the HTTP request
	return ((HTTPDownloadProtocol *) pProtocol)->SendRequest(parameters);
}

bool HTTPDownloadProtocol::SendRequest(Variant &parameters) {
	//1. Ok, we connected. Do the actual HTTP request now. First, get the HTTP protocol
	OutboundHTTPProtocol *pHTTP = (OutboundHTTPProtocol *) GetFarProtocol();

	//2. We wish to disconnect after the transfer is complete
	pHTTP->SetDisconnectAfterTransfer(true);

	//3. This is going to be a GET request
	pHTTP->Method(HTTP_METHOD_GET);

	//4. This 2 are mandatory
	pHTTP->Document(parameters["uri"]["fullDocumentPath"]);
	pHTTP->Host(parameters["uri"]["host"]);

	//5. Put the data
	_outputBuffer.ReadFromString((string) parameters["payload"]);

	//6. Done
	return EnqueueForOutbound();
}
