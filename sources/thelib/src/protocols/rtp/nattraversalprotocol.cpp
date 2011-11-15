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

#ifdef HAS_PROTOCOL_RTP
#include "protocols/rtp/nattraversalprotocol.h"

NATTraversalProtocol::NATTraversalProtocol()
: BaseProtocol(PT_RTP_NAT_TRAVERSAL) {
	_pOutboundAddress = NULL;
}

NATTraversalProtocol::~NATTraversalProtocol() {
}

bool NATTraversalProtocol::Initialize(Variant &parameters) {
	GetCustomParameters() = parameters;
	return true;
}

bool NATTraversalProtocol::AllowFarProtocol(uint64_t type) {
	return type == PT_UDP;
}

bool NATTraversalProtocol::AllowNearProtocol(uint64_t type) {
	return false;
}

bool NATTraversalProtocol::SignalInputData(int32_t recvAmount) {
	NYIR;
}

bool NATTraversalProtocol::SignalInputData(IOBuffer &buffer) {
	NYIR;
}

bool NATTraversalProtocol::SignalInputData(IOBuffer &buffer, sockaddr_in *pPeerAddress) {
	//FINEST("_inputBuffer:\n%s", STR(buffer));
	buffer.IgnoreAll();
	if (_pOutboundAddress == NULL)
		return true;
	if (_pOutboundAddress->sin_addr.s_addr != pPeerAddress->sin_addr.s_addr) {
		WARN("Attempt to divert traffic. DoS attack!?");
		return true;
	}
	string ipAddress = inet_ntoa(_pOutboundAddress->sin_addr);
	if (_pOutboundAddress->sin_port == pPeerAddress->sin_port) {
		INFO("The client has public endpoint: %s:%"PRIu16,
				STR(ipAddress),
				ENTOHS(_pOutboundAddress->sin_port));
	} else {

		INFO("The client is behind firewall: %s:%"PRIu16" -> %s:%"PRIu16,
				STR(ipAddress),
				ENTOHS(_pOutboundAddress->sin_port),
				STR(ipAddress),
				ENTOHS(pPeerAddress->sin_port));
		_pOutboundAddress->sin_port = pPeerAddress->sin_port;
	}
	_pOutboundAddress = NULL;
	return true;
}

void NATTraversalProtocol::SetOutboundAddress(sockaddr_in *pOutboundAddress) {
	_pOutboundAddress = pOutboundAddress;
}

#endif /* HAS_PROTOCOL_RTP */
