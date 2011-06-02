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

#include "echoprotocol.h"
#include "localdefines.h"
#include "protocols/http/inboundhttpprotocol.h"
#include "httpdownloadprotocol.h"
using namespace app_samplefactory;

EchoProtocol::EchoProtocol()
: BaseProtocol(PT_ECHO_PROTOCOL) {

}

EchoProtocol::~EchoProtocol() {
}

bool EchoProtocol::Initialize(Variant &parameters) {
	//save the parameters
	GetCustomParameters() = parameters;
	return true;
}

IOBuffer * EchoProtocol::GetOutputBuffer() {
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

bool EchoProtocol::AllowFarProtocol(uint64_t type) {
	//we only allow tcp and inbound HTTP for the
	//far protocol
	return (type == PT_TCP) || (type == PT_INBOUND_HTTP);
}

bool EchoProtocol::AllowNearProtocol(uint64_t type) {
	//this is an endpoint protocol so we don't accept
	//anything on top of it
	return false;
}

bool EchoProtocol::SignalInputData(int32_t recvAmount) {
	//this protocol doesn't have a carrier attached directly so
	//we will never operate on file descriptors
	ASSERT("Operation not supported");
	return false;
}

bool EchoProtocol::SignalInputData(IOBuffer &buffer) {
	//PREAMBLE
	//First, check the transport type. If it is a 
	//http transport, wait for it to finish the request
	//before doing stuff with it. This is not mandatory.
	//We can start consume the buffer right away, but since
	//we do a echo protocol, let's keep the things simple
	//If the transport is direct TCP, we will echo back only after
	//getting a new line character.
	//I will try to keep the things extremely simple in this function
	//sacrificing "user input sanitize" for simplicity. Anyway, I think
	//we all know how an echo protocol should behave...

	//1. Check and see if the protocol is HTTP or not
	if (GetFarProtocol()->GetType() == PT_INBOUND_HTTP) {
		//2. This has HTTP protocol as carrier. Get it and
		//wait for it to complete
		InboundHTTPProtocol *pHTTP = (InboundHTTPProtocol *) GetFarProtocol();
		if (!pHTTP->TransferCompleted()) {
			FINEST("HTTP transfer not completed yet");
			return true;
		}

		//3. Ok, it is complete. Get the data and put it inside the output buffer
		//Actually, we are going to add the string "ECHO " first, and after that
		//the actual data. Just for fun...
		_outputBuffer.ReadFromString("ECHO ");
		_outputBuffer.ReadFromBuffer(
				GETIBPOINTER(buffer),
				GETAVAILABLEBYTESCOUNT(buffer));

		//3.1. Let's also dump the complete HTTP request. You might want to pick
		//up a thing or 2 from it. Just for fun...
		FINEST("HTTP request:\n%s", STR(pHTTP->GetHeaders().ToString()));

		//4. Ignore the input buffer now.
		buffer.IgnoreAll();

		//5. Add some fancy mime type and some custom HTTP headers... Just for fun...
		pHTTP->SetOutboundHeader(HTTP_HEADERS_CONTENT_TYPE, "text/plain");
		pHTTP->SetOutboundHeader("My-fancy-http-header", "aloha from C++ RTMP Server");

		//6. We are done. Enqueue the stack for outbound I/O
		return EnqueueForOutbound();
	} else {
		//7. Get the data inside a string
		string data = string((const char *) GETIBPOINTER(buffer), GETAVAILABLEBYTESCOUNT(buffer));

		//8. extremely minimal and dangerous test to see if we have a new-line,
		//but for the sake of simplicity, I'm just going to keep it like that
		if (data.length() == 0 || data[data.length() - 1] != '\n') {
			FINEST("Not enough data. So far I have %s. Wait for more...", STR(data));
			return true;
		}

		//9. Ok, it is complete. Get the data and put it inside the output buffer
		//Actually, we are going to add the string "ECHO " first, and after that
		//the actual data. Just for fun...
		_outputBuffer.ReadFromString("ECHO ");
		_outputBuffer.ReadFromBuffer(
				GETIBPOINTER(buffer),
				GETAVAILABLEBYTESCOUNT(buffer));

		//10. Ignore the input buffer now.
		buffer.IgnoreAll();
		
		//11. for demonstration purposes, whenever I get the string "testHttpRequest"
		//I'm just going to do a request to "http://www.rtmpd.com" and print out
		//the page on console. This should illustrate how to use the outbound HTTP protocol
		HTTPDownloadProtocol::DoSimpleGETRequestWithSomePayload("http://www.rtmpd.com/resources","Some data.... Hello World!");
		

		//11. We are done. Enqueue the stack for outbound I/O
		return EnqueueForOutbound();
	}
}
