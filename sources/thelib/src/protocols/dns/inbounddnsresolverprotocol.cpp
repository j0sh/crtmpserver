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

#ifdef HAS_PROTOCOL_DNS
#include "protocols/dns/inbounddnsresolverprotocol.h"
#include "protocols/http/basehttpprotocol.h"

InboundDNSResolverProtocol::InboundDNSResolverProtocol()
: BaseProtocol(PT_INBOUND_DNS) {

}

InboundDNSResolverProtocol::~InboundDNSResolverProtocol() {
}

bool InboundDNSResolverProtocol::Initialize(Variant &parameters) {
	return true;
}

bool InboundDNSResolverProtocol::AllowFarProtocol(uint64_t type) {
	if (type == PT_TCP)
		return true;
	return false;
}

bool InboundDNSResolverProtocol::AllowNearProtocol(uint64_t type) {
	FATAL("This protocol doesn't allow any near protocols");
	return false;
}

IOBuffer * InboundDNSResolverProtocol::GetOutputBuffer() {
	if (GETAVAILABLEBYTESCOUNT(_outputBuffer) > 0)
		return &_outputBuffer;
	return NULL;
}

bool InboundDNSResolverProtocol::SignalInputData(int32_t recvAmount) {
	ASSERT("OPERATION NOT SUPPORTED");
	return false;
}

bool InboundDNSResolverProtocol::SignalInputData(IOBuffer &buffer) {
	while (true) {
		if (GETAVAILABLEBYTESCOUNT(buffer) < 4) {
			return true;
		}
		uint32_t size = *((uint32_t *) GETIBPOINTER(buffer));
		if (GETAVAILABLEBYTESCOUNT(buffer) < 4 + size) {
			FINEST("Not enough data");
			return true;
		}
		string rawData((char *) (GETIBPOINTER(buffer) + 4), size);
		Variant request;
		bool result = Variant::DeserializeFromBin(rawData, request);
		if (!result) {
			FATAL("Unable to deserialize variant:\n%s", STR(buffer));
			return false;
		}
		if (!buffer.Ignore(4 + size)) {
			FATAL("Unable to ignore %u bytes", 4 + size);
			return false;
		}
		if (!HandleRequest(request)) {
			FATAL("Unable to handle request:\n%s", STR(request.ToString()));
			return false;
		}
	}
}

bool InboundDNSResolverProtocol::HandleRequest(Variant &request) {
	if ((VariantType) request[DNS_RESOLVER_HOST] != V_STRING) {
		FATAL("Invalid request:\n%s", STR(request.ToString()));
		return false;
	}
	string ip = getHostByName(request[DNS_RESOLVER_HOST]);
	Variant response;
	response[DNS_RESOLVER_REQUEST] = request;
	response[DNS_RESOLVER_IP] = ip;
	return SendResponse(response);
}

bool InboundDNSResolverProtocol::SendResponse(Variant &response) {
	string rawRequest = "";
	if (!response.SerializeToBin(rawRequest)) {
		FATAL("Unable to serialize variant to bin");
		return false;
	}
	uint32_t size = rawRequest.length();
	_outputBuffer.ReadFromBuffer((uint8_t *) & size, 4);
	_outputBuffer.ReadFromString(rawRequest);
	return EnqueueForOutbound();
}
#endif /* HAS_PROTOCOL_DNS */
