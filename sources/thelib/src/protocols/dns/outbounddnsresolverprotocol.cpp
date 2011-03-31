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
#include "protocols/dns/outbounddnsresolverprotocol.h"
#include "protocols/protocolfactorymanager.h"
#include "netio/netio.h"
#include "protocols/protocolmanager.h"


uint32_t OutboundDNSResolverProtocol::_requestIdGenerator = 0;
uint32_t OutboundDNSResolverProtocol::_dnsProtocolResolverId = 0;
map<uint32_t, Variant> OutboundDNSResolverProtocol::_requests;

OutboundDNSResolverProtocol::OutboundDNSResolverProtocol()
: BaseProtocol(PT_OUTBOUND_DNS) {

}

OutboundDNSResolverProtocol::~OutboundDNSResolverProtocol() {
	_dnsProtocolResolverId = 0;
}

bool OutboundDNSResolverProtocol::Connect(string ip, uint16_t port,
		Variant customParameters) {
	vector<uint64_t> chain = ProtocolFactoryManager::ResolveProtocolChain(
			CONF_PROTOCOL_OUTBOUND_DNS);
	if (chain.size() == 0) {
		FATAL("Unable to obtain protocol chain from settings: %s",
				CONF_PROTOCOL_OUTBOUND_DNS);
		return false;
	}
	if (!TCPConnector<OutboundDNSResolverProtocol>::Connect(ip, port, chain,
			customParameters)) {
		FATAL("Unable to connect to %s:%hu", STR(ip), port);
		return false;
	}
	return true;
}

bool OutboundDNSResolverProtocol::SignalProtocolCreated(BaseProtocol *pProtocol,
		Variant customParameters) {
	if (pProtocol == NULL) {
		FATAL("Connection failed:\n%s", STR(customParameters.ToString()));
		return false;
	}

	if (_dnsProtocolResolverId != 0) {
		FATAL("There is already a connected protocol resolver");
		return false;
	}
	_dnsProtocolResolverId = pProtocol->GetId();

	FOR_MAP(_requests, uint32_t, Variant, i) {
		if (!((OutboundDNSResolverProtocol *) pProtocol)->SendRequest(MAP_VAL(i))) {
			FATAL("Unable to send requests");
			return false;
		}
	}
	return true;
}

bool OutboundDNSResolverProtocol::Resolve(string host,
		uint32_t consumerProtocolId, Variant customData) {
	Variant request;

	//1. Build the request
	request[DNS_RESOLVER_CONSUMER_PROTOCOL_ID] = consumerProtocolId;
	request[DNS_RESOLVER_REQUEST_ID] = _requestIdGenerator++;
	request[DNS_RESOLVER_HOST] = host;
	request[DNS_RESOLVER_CUSTOM_DATA] = customData;

	//2. Get our hands on the protocol
	BaseProtocol *pProtocol = NULL;
	if (_dnsProtocolResolverId != 0)
		pProtocol = ProtocolManager::GetProtocol(_dnsProtocolResolverId);

	//3. See if we have a valid protocol
	if (pProtocol != NULL) {
		//3.1. We have a protocol. See if it is the correct type
		if (pProtocol->GetType() != PT_OUTBOUND_DNS) {
			//3.1.1 the protocol is not the correct type. Reset the protocol
			//info and queue the request
			_dnsProtocolResolverId = 0;
			pProtocol = NULL;
			_requests[request[DNS_RESOLVER_REQUEST_ID]] = request;
		} else {
			//3.1.2 All is ok. Send the request
			if (!((OutboundDNSResolverProtocol *) pProtocol)->SendRequest(request)) {
				FATAL("Unable to send requests");
				return false;
			}
		}
	} else {
		//3.2. The dns resolver is not yet available. Queue the request
		_requests[request[DNS_RESOLVER_REQUEST_ID]] = request;
	}

	return true;
}

bool OutboundDNSResolverProtocol::Initialize(Variant &parameters) {
	return true;
}

bool OutboundDNSResolverProtocol::AllowFarProtocol(uint64_t type) {
	if (type == PT_TCP)
		return true;
	return false;
}

bool OutboundDNSResolverProtocol::AllowNearProtocol(uint64_t type) {
	FATAL("This protocol doesn't allow any near protocols");
	return false;
}

IOBuffer * OutboundDNSResolverProtocol::GetOutputBuffer() {
	if (GETAVAILABLEBYTESCOUNT(_outputBuffer) > 0)
		return &_outputBuffer;
	return NULL;
}

bool OutboundDNSResolverProtocol::SignalInputData(int32_t recvAmount) {
	ASSERT("OPERATION NOT SUPPORTED");
	return false;
}

bool OutboundDNSResolverProtocol::SignalInputData(IOBuffer &buffer) {
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
		Variant response;
		bool result = Variant::DeserializeFromBin(rawData, response);
		if (!result) {
			FATAL("Unable to deserialize variant:\n%s", STR(buffer));
			return false;
		}
		if (!buffer.Ignore(4 + size)) {
			FATAL("Unable to ignore %u bytes", 4 + size);
			return false;
		}
		if (!HandleResponse(response)) {
			FATAL("Unable to handle response:\n%s", STR(response.ToString()));
			return false;
		}
	}
}

bool OutboundDNSResolverProtocol::SendRequest(Variant &request) {
	string rawRequest = "";
	if (!request.SerializeToBin(rawRequest)) {
		FATAL("Unable to serialize variant to bin");
		return false;
	}
	uint32_t size = rawRequest.length();
	_outputBuffer.ReadFromBuffer((uint8_t *) & size, 4);
	_outputBuffer.ReadFromString(rawRequest);
	return EnqueueForOutbound();
}

bool OutboundDNSResolverProtocol::HandleResponse(Variant &response) {
	//1. Validate the response
	if ((VariantType) response[DNS_RESOLVER_REQUEST][DNS_RESOLVER_REQUEST_ID] != V_UINT32) {
		FATAL("Invalid response:\n%s", STR(response.ToString()));
		return false;
	}

	if ((VariantType) response[DNS_RESOLVER_REQUEST][DNS_RESOLVER_CONSUMER_PROTOCOL_ID] != V_UINT32) {
		FATAL("Invalid response:\n%s", STR(response.ToString()));
		return false;
	}

	uint32_t protocolId = response[DNS_RESOLVER_REQUEST][DNS_RESOLVER_CONSUMER_PROTOCOL_ID];
	BaseProtocol *pProtocol = ProtocolManager::GetProtocol(protocolId, false);
	if (pProtocol != NULL) {
		Variant event;
		event[PROTOCOL_EVENT_TYPE] = PROTOCOL_EVENT_TYPE_DNS_RESPONSE;
		event[PROTOCOL_EVENT_PAYLOAD] = response;
		pProtocol->SignalInterProtocolEvent(event);
	}

	return true;
}
#endif /* HAS_PROTOCOL_DNS */
