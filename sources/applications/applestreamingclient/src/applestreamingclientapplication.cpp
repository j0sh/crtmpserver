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

#include "applestreamingclientapplication.h"
#include "protocols/rtmp/rtmpappprotocolhandler.h"
#include "protocols/baseprotocol.h"
#include "protocols/protocolfactorymanager.h"
#include "protocols/protocolfactory.h"
#include "protocols/m3u8/m3u8appprotocolhandler.h"
#include "protocols/key/keyappprotocolhandler.h"
#include "protocols/ts/tsappprotocolhandler.h"
#include "protocols/aes/aesappprotocolhandler.h"
#include "protocols/protocolmanager.h"
#include "streaming/basestream.h"
#include "streaming/streamstypes.h"
#include "protocols/baseprotocol.h"
#include "clientcontext.h"
#include "eventsink/baseeventsink.h"
#include "protocols/rtp/rtspappprotocolhandler.h"
#include "protocols/httpbuff/httpbuffappprotocolhandler.h"
#include "protocols/variant/variantappprotocolhandler.h"
using namespace app_applestreamingclient;

AppleStreamingClientApplication::AppleStreamingClientApplication(Variant &configuration)
: BaseClientApplication(configuration) {
#ifdef HAS_PROTOCOL_RTMP
	_pRTMPHandler = NULL;
#endif /* HAS_PROTOCOL_RTMP */
	_pM3U8Handler = NULL;
	_pKeyHandler = NULL;
	_pTSHandler = NULL;
	_pHTTPBuffHandler = NULL;
	_pAESHandler = NULL;
	_pRTSPHandler = NULL;
	_pVariantHandler = NULL;
	_pFactory = NULL;
}

AppleStreamingClientApplication::~AppleStreamingClientApplication() {
	CloseAllContexts();
#ifdef HAS_PROTOCOL_RTMP
	UnRegisterAppProtocolHandler(PT_INBOUND_RTMP);
	UnRegisterAppProtocolHandler(PT_OUTBOUND_RTMP);
	if (_pRTMPHandler != NULL) {
		delete _pRTMPHandler;
		_pRTMPHandler = NULL;
	}
#endif /* HAS_PROTOCOL_RTMP */

	UnRegisterAppProtocolHandler(PT_INBOUND_CHILD_M3U8);
	UnRegisterAppProtocolHandler(PT_INBOUND_MASTER_M3U8);
	if (_pM3U8Handler != NULL) {
		delete _pM3U8Handler;
		_pM3U8Handler = NULL;
	}

	UnRegisterAppProtocolHandler(PT_INBOUND_KEY);
	if (_pKeyHandler != NULL) {
		delete _pKeyHandler;
		_pKeyHandler = NULL;
	}

	UnRegisterAppProtocolHandler(PT_INBOUND_TS);
	if (_pTSHandler != NULL) {
		delete _pTSHandler;
		_pTSHandler = NULL;
	}

	UnRegisterAppProtocolHandler(PT_HTTP_BUFF);
	if (_pHTTPBuffHandler != NULL) {
		delete _pHTTPBuffHandler;
		_pHTTPBuffHandler = NULL;
	}

	UnRegisterAppProtocolHandler(PT_INBOUND_AES);
	if (_pAESHandler != NULL) {
		delete _pAESHandler;
		_pAESHandler = NULL;
	}

	UnRegisterAppProtocolHandler(PT_RTSP);
	if (_pRTSPHandler != NULL) {
		delete _pRTSPHandler;
		_pRTSPHandler = NULL;
	}

	UnRegisterAppProtocolHandler(PT_XML_VAR);
	UnRegisterAppProtocolHandler(PT_BIN_VAR);
	if (_pVariantHandler != NULL) {
		delete _pVariantHandler;
		_pVariantHandler = NULL;
	}

	if (_pFactory != NULL) {
		ProtocolFactoryManager::UnRegisterProtocolFactory(_pFactory);
		delete _pFactory;
		_pFactory = NULL;
	}
}

void AppleStreamingClientApplication::CloseAllContexts() {
	vector<uint32_t> contextIds = ClientContext::GetContextIds();
	for (uint32_t i = 0; i < contextIds.size(); i++) {
		ClientContext::ReleaseContext(contextIds[i]);
	}
}

bool AppleStreamingClientApplication::Initialize() {
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

	_pM3U8Handler = new M3U8AppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_INBOUND_CHILD_M3U8, _pM3U8Handler);
	RegisterAppProtocolHandler(PT_INBOUND_MASTER_M3U8, _pM3U8Handler);

	_pKeyHandler = new KeyAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_INBOUND_KEY, _pKeyHandler);

	_pTSHandler = new TSAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_INBOUND_TS, _pTSHandler);

	_pHTTPBuffHandler = new HTTPBuffAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_HTTP_BUFF, _pHTTPBuffHandler);

	_pAESHandler = new AESAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_INBOUND_AES, _pAESHandler);

	_pRTSPHandler = new RTSPAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_RTSP, _pRTSPHandler);

	_pVariantHandler = new VariantAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_XML_VAR, _pVariantHandler);
	RegisterAppProtocolHandler(PT_BIN_VAR, _pVariantHandler);

	//2. Initialize our protocol factory
	_pFactory = new ProtocolFactory();
	ProtocolFactoryManager::RegisterProtocolFactory(_pFactory);

	return true;
}

#ifdef HAS_MS_TIMER

void AppleStreamingClientApplication::SetFineTimerId(uint32_t fineTimerId) {
	_fineTimerId = fineTimerId;
}

FineTimer *AppleStreamingClientApplication::GetFineTimer() {
	return (FineTimer *) ProtocolManager::GetProtocol(_fineTimerId);
}
#endif /* HAS_MS_TIMER */

#ifdef ANDROID

void AppleStreamingClientApplication::SetJavaCallBackInterface(CallBackInfo ci) {
	_ci = ci;
}

CallBackInfo &AppleStreamingClientApplication::GetJavaCallBackInterface() {
	return _ci;
}
#endif /* ANDROID */

void AppleStreamingClientApplication::SignalStreamRegistered(BaseStream *pStream) {
	if (pStream->GetType() != ST_IN_NET_TS)
		return;

	BaseProtocol *pProtocol = pStream->GetProtocol();
	if (pProtocol == NULL) {
		ASSERT("Protocol is NULL!!!");
	}
	uint32_t contextId = pProtocol->GetCustomParameters()["contextId"];
	ClientContext *pContext = ClientContext::GetContext(contextId, 0, 0);
	if (pContext == NULL) {
		WARN("Context not available anymore");
		pProtocol->EnqueueForDelete();
		return;
	}

	pContext->SignalStreamRegistered(pStream);
}

void AppleStreamingClientApplication::SignalStreamUnRegistered(BaseStream *pStream) {
	if (pStream->GetType() != ST_IN_NET_TS)
		return;

	BaseProtocol *pProtocol = pStream->GetProtocol();
	if (pProtocol == NULL) {
		ASSERT("Protocol is NULL!!!");
	}
	uint32_t contextId = pProtocol->GetCustomParameters()["contextId"];
	ClientContext *pContext = ClientContext::GetContext(contextId, 0, 0);
	if (pContext == NULL) {
		WARN("Context not available anymore");
		pProtocol->EnqueueForDelete();
		return;
	}

	pContext->SignalStreamUnRegistered(pStream);
}
