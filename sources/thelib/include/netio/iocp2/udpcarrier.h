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
#ifndef _UDPCARRIER_H
#define	_UDPCARRIER_H

#include "netio/iocp2/iohandler.h"

class BaseProtocol;

class DLLEXP UDPCarrier
: public IOHandler {
private:
public:
    UDPCarrier(int32_t fd, BaseProtocol *pProtocol);
    virtual ~UDPCarrier();

    void ResetProtocol();
    virtual bool OnEvent(struct kevent &event);
    virtual bool SignalOutputData();
    virtual operator string();

    string GetFarEndpointAddress();
    uint16_t GetFarEndpointPort();
    string GetNearEndpointAddress();
    uint16_t GetNearEndpointPort();

    static UDPCarrier* Create(string bindIp, uint16_t bindPort, BaseProtocol *pProtocol);
private:
    bool Setup(Variant &settings);
};


#endif	/* _UDPCARRIER_H */
#endif /* NET_IOCP2 */

