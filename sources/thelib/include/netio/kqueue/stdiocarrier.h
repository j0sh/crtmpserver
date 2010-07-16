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


#ifdef NET_KQUEUE
#ifndef _STDIOCARRIER_H
#define	_STDIOCARRIER_H

#include "netio/kqueue/iohandler.h"

class BaseProtocol;

class StdioCarrier
: public IOHandler {
private:
    static StdioCarrier *_pInstance;
    BaseProtocol *_pProtocol;
    bool _writeDataEnabled;
private:
    StdioCarrier(BaseProtocol *pProtocol);
public:
    static StdioCarrier *GetInstance(BaseProtocol *pProtocol);
    virtual ~StdioCarrier();
    void ResetProtocol();
    virtual bool OnEvent(struct kevent &event);
    virtual bool SignalOutputData();
    virtual operator string();
};

#endif	/* _STDIOCARRIER_H */
#endif /* NET_KQUEUE */

