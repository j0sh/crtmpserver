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
#ifndef _TCPCARRIER_H
#define	_TCPCARRIER_H

#include "netio/iocp2/iohandler.h"

class BaseProtocol;

typedef struct _ReadWriteOV : public GenericOV {
    bool isRead;
    uint8_t dummyBuffer;
} ReadWriteOV;

class DLLEXP TCPCarrier
: public IOHandler {
private:
    BaseProtocol *_pProtocol;
    bool _writeDataEnabled;
    ReadWriteOV *_pReadOV;
    ReadWriteOV *_pWriteOV;
    string _farAddress;
    uint16_t _farPort;
    string _nearAddress;
    uint16_t _nearPort; 
public:
    TCPCarrier(int32_t fd, BaseProtocol *pProtocol);
    virtual ~TCPCarrier();
    void ResetProtocol();
    virtual bool OnEvent(GenericOV *pGenericOV);
    virtual bool SignalOutputData();
    ReadWriteOV *GetReadWriteOV(bool isRead);
    virtual operator string();

    string GetFarEndpointAddress();
    uint16_t GetFarEndpointPort();
    string GetNearEndpointAddress();
    uint16_t GetNearEndpointPort();
private:
    bool GetEndpointsInfo();
};


#endif	/* _TCPCARRIER_H */
#endif /* NET_IOCP2 */


