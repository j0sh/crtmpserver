/*
 * Copyright (c) 2009, Gavriloaie Eugen-Andrei (shiretu@gmail.com)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *              *Redistributions of source code must retain the above copyright notice,
 *               this list of conditions and the following disclaimer.
 *              *Redistributions in binary form must reproduce the above copyright
 *               notice, this list of conditions and the following disclaimer in the
 *               documentation and/or other materials provided with the distribution.
 *              *Neither the name of the DEVSS nor the names of its
 *               contributors may be used to endorse or promote products derived from
 *               this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#include "#APPNAME_LC#application.h"
#include "rtmpappprotocolhandler.h"
#include "protocols/baseprotocol.h"

#APPNAME#Application::#APPNAME#Application(Variant &configuration)
: BaseClientApplication(configuration) {
#ifdef HAS_PROTOCOL_RTMP
    _pRTMPHandler=NULL;
#endif /* HAS_PROTOCOL_RTMP */
}

#APPNAME#Application::~#APPNAME#Application() {
#ifdef HAS_PROTOCOL_RTMP
    UnRegisterAppProtocolHandler(PT_INBOUND_RTMP);
    UnRegisterAppProtocolHandler(PT_OUTBOUND_RTMP);
    if(_pRTMPHandler!=NULL) {
	delete _pRTMPHandler;
	_pRTMPHandler=NULL;
    }
#endif /* HAS_PROTOCOL_RTMP */
}

bool #APPNAME#Application::Initialize() {
    //TODO: Add your app init code here
    //Things like parsing custom sections inside _configuration for example,
    //initialize the protocol handler(s)
    
    //1. Initialize the protocol handler(s)
#ifdef HAS_PROTOCOL_RTMP    
    _pRTMPHandler = new RTMPAppProtocolHandler(_configuration);
    RegisterAppProtocolHandler(PT_INBOUND_RTMP, _pRTMPHandler);
    RegisterAppProtocolHandler(PT_OUTBOUND_RTMP, _pRTMPHandler);
#endif /* HAS_PROTOCOL_RTMP */

    //2. Use your custom values inside your app config node
    //I'll just print the config for now... Watch the logs
    FINEST("%s app config node:\n%s",
            STR(GetName()), STR(_configuration.ToString()));
    return true;
}

