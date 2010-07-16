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




#include "dbaccessprotocol.h"
#include "protocolfactory.h"
#include "buffering/iobuffer.h"
#include "protocols/http/outboundhttpprotocol.h"
#include "controller.h"

DBAccessProtocol::DBAccessProtocol()
: BaseProtocol(PT_DBACCESS) {

}

DBAccessProtocol::~DBAccessProtocol() {
}

bool DBAccessProtocol::AllowFarProtocol(uint64_t type) {
    return (type == PT_TCP) || (type == PT_OUTBOUND_HTTP) || (type == PT_INBOUND_HTTP);
}

bool DBAccessProtocol::AllowNearProtocol(uint64_t type) {
    ASSERT("This protocol is a near endpoint protocol");
    return false;
}

bool DBAccessProtocol::SignalInputData(int32_t recvAmount) {
    ASSERT("OPERATION NOT SUPPORTED");
    return false;
}

bool DBAccessProtocol::SignalInputData(IOBuffer &buffer) {
    if (GETAVAILABLEBYTESCOUNT(buffer) == 0)
        return true;
    string str((char *) GETIBPOINTER(buffer), GETAVAILABLEBYTESCOUNT(buffer));
    FINEST("str: %s", STR(str));
    buffer.IgnoreAll();
    //FINEST("buffer:\n%s", STR(buffer));
    return true;
}

IOBuffer * DBAccessProtocol::GetOutputBuffer() {
    if (GETAVAILABLEBYTESCOUNT(_outputBuffer) > 0)
        return &_outputBuffer;
    return NULL;
}

bool DBAccessProtocol::DoRequest(Variant &parameters) {
    //1. Get the far protocol and inspect its type
    //we need that becase we land on 3 kinds of protocols (see AllowFarProtocol)
    BaseProtocol *pFarProtocol = GetFarProtocol();
    if (pFarProtocol == NULL) {
        FATAL("No far protocol!?");
        return false;
    }

    switch (pFarProtocol->GetType()) {
        case PT_TCP:
        {
            //2. This directly TCP. So, we just write it on the pipe
            //and be done with it. Also, parameters contains our request
            //directly (no wrappers around it)
            _outputBuffer.ReadFromString(STR(parameters.ToString()));
            return EnqueueForOutbound();
        }
        case PT_OUTBOUND_HTTP:
        {
#ifdef HAS_PROTOCOL_HTTP
            //3. This is a HTTP request that we have to do. So, first things firs
            //Get the http protocol
            OutboundHTTPProtocol *pHTTP = (OutboundHTTPProtocol *) pFarProtocol;

            //4. We wish to disconnect after the transfer is complete
            pHTTP->SetDisconnectAfterTransfer(true);

            //5. This is a GET request
            pHTTP->Method(HTTP_METHOD_GET);

            //6. Our document and the host
            pHTTP->Document(parameters["document"]);
            pHTTP->Host(parameters["host"]);

            //7. And the actual payload
            _outputBuffer.ReadFromString(STR(parameters["payload"].ToString()));

            //8. Done :)
            return EnqueueForOutbound();
#else
            FATAL("HTTP protocol not supported");
            return false;
#endif /* HAS_PROTOCOL_HTTP */
        }
        case PT_INBOUND_HTTP:
        {
#ifdef HAS_PROTOCOL_HTTP
            NYIR;
#else
            FATAL("HTTP protocol not supported");
            return false;
#endif /* HAS_PROTOCOL_HTTP */
        }
        default:
        {
            ASSERT("Wow!!! Hold your horses... This protocol type is %s and we only accept %s, %s and %s",
                    STR(tagToString(pFarProtocol->GetType())),
                    STR(tagToString(PT_TCP)),
                    STR(tagToString(PT_INBOUND_HTTP)),
                    STR(tagToString(PT_OUTBOUND_HTTP)));
            return false;
        }
    }
}

