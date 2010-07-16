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


#ifdef NET_IOCP2

#include "buffering/iobuffer.h"
#include "netio/iocp2/udpcarrier.h"
#include "netio/iocp2/iohandlermanager.h"
#include "protocols/baseprotocol.h"

UDPCarrier::UDPCarrier(int32_t fd, BaseProtocol *pProtocol)
: IOHandler(fd, fd, IOHT_UDP_CARRIER) {
    NYIA;
}

UDPCarrier::~UDPCarrier() {
    NYIA;
}

void UDPCarrier::ResetProtocol() {
    NYIA;
}

bool UDPCarrier::OnEvent(struct kevent &event) {
    NYIA;
	return false;
}

bool UDPCarrier::SignalOutputData() {
    NYIA;
	return false;
}

UDPCarrier::operator string() {
    NYIA;
	return "";
}

string UDPCarrier::GetFarEndpointAddress() {
    NYIA;
	return "";
}

uint16_t UDPCarrier::GetFarEndpointPort() {
    NYIA;
	return 0;
}

string UDPCarrier::GetNearEndpointAddress() {
    NYIA;
	return "";
}

uint16_t UDPCarrier::GetNearEndpointPort() {
    NYIA;
	return 0;
}

UDPCarrier* UDPCarrier::Create(string bindIp, uint16_t bindPort, BaseProtocol *pProtocol) {
    NYIA;
	return NULL;
}

bool UDPCarrier::Setup(Variant &settings) {
    NYIA;
	return false;
}

#endif /* NET_IOCP2 */

