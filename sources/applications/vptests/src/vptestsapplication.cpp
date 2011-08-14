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

#include "protocols/baseprotocol.h"
#include "vptestsapplication.h"
#include "rtmpappprotocolhandler.h"
#include "variantappprotocolhandler.h"
using namespace app_vptests;

VPTestsApplication::VPTestsApplication(Variant &configuration)
: BaseClientApplication(configuration) {
#ifdef HAS_PROTOCOL_RTMP
	_pRTMPHandler = NULL;
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_VAR
	_pVariantHandler = NULL;
#endif /* HAS_PROTOCOL_RTMP */
}

VPTestsApplication::~VPTestsApplication() {
#ifdef HAS_PROTOCOL_RTMP
	UnRegisterAppProtocolHandler(PT_INBOUND_RTMP);
	UnRegisterAppProtocolHandler(PT_OUTBOUND_RTMP);
	if (_pRTMPHandler != NULL) {
		delete _pRTMPHandler;
		_pRTMPHandler = NULL;
	}
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_VAR
	UnRegisterAppProtocolHandler(PT_BIN_VAR);
	UnRegisterAppProtocolHandler(PT_XML_VAR);
	if (_pVariantHandler != NULL) {
		delete _pVariantHandler;
		_pVariantHandler = NULL;
	}
#endif /* HAS_PROTOCOL_VAR */
}

bool VPTestsApplication::Initialize() {
	if (!BaseClientApplication::Initialize()) {
		FATAL("Unable to initialize application");
		return false;
	}
	//TODO: Add your app init code here
	//Things like parsing custom sections inside _configuration for example,
	//initialize the protocol handler(s)

	//1. Initialize the protocol handler(s)
#ifdef HAS_PROTOCOL_RTMP
	_pRTMPHandler = new RTMPAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_INBOUND_RTMP, _pRTMPHandler);
	RegisterAppProtocolHandler(PT_OUTBOUND_RTMP, _pRTMPHandler);
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_VAR
	_pVariantHandler = new VariantAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_BIN_VAR, _pVariantHandler);
	RegisterAppProtocolHandler(PT_XML_VAR, _pVariantHandler);
#endif /* HAS_PROTOCOL_VAR */

	return true;
}


