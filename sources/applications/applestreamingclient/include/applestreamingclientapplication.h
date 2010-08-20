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


#ifndef _APPLESTREAMINGCLIENTAPPLICATION_H
#define _APPLESTREAMINGCLIENTAPPLICATION_H

#include "application/baseclientapplication.h"
#include "jnihelpers.h"

#ifdef HAS_PROTOCOL_RTMP
class RTMPAppProtocolHandler;
#endif /* HAS_PROTOCOL_RTMP */
class M3U8AppProtocolHandler;
class KeyAppProtocolHandler;
class TSAppProtocolHandler;
class HTTPBuffAppProtocolHandler;
class AESAppProtocolHandler;
class RTSPAppProtocolHandler;
class VariantAppProtocolHandler;
class ProtocolFactory;

class AppleStreamingClientApplication
: public BaseClientApplication {
private:
#ifdef HAS_PROTOCOL_RTMP
	RTMPAppProtocolHandler *_pRTMPHandler;
#endif /* HAS_PROTOCOL_RTMP */
	M3U8AppProtocolHandler *_pM3U8Handler;
	KeyAppProtocolHandler *_pKeyHandler;
	TSAppProtocolHandler *_pTSHandler;
	HTTPBuffAppProtocolHandler *_pHTTPBuff;
	AESAppProtocolHandler *_pAESHandler;
	RTSPAppProtocolHandler *_pRTSPHandler;
	VariantAppProtocolHandler *_pVariantHandler;
	ProtocolFactory *_pFactory;
#ifdef ANDROID
	CallBackInfo _ci;
#endif /* ANDROID */
public:
	AppleStreamingClientApplication(Variant &configuration);
	virtual ~AppleStreamingClientApplication();

	void CloseAllContexts();

	virtual bool Initialize();

#ifdef ANDROID
	void SetJavaCallBackInterface(CallBackInfo ci);
	CallBackInfo &GetJavaCallBackInterface();
#endif /* ANDROID */

	virtual void SignalStreamRegistered(BaseStream *pStream);
	virtual void SignalStreamUnRegistered(BaseStream *pStream);
};


#endif	/* _APPLESTREAMINGCLIENTAPPLICATION_H */

