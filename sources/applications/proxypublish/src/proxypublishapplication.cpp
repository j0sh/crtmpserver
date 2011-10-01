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

#include "proxypublishapplication.h"
#include "rtmpappprotocolhandler.h"
#include "protocols/baseprotocol.h"
#include "liveflvappprotocolhandler.h"
#include "streaming/streamstypes.h"
#include "streaming/basestream.h"
#include "streaming/baseinstream.h"
#include "protocols/rtmp/outboundrtmpprotocol.h"
#include "rtpappprotocolhandler.h"
#include "rtspappprotocolhandler.h"
#include "protocols/protocolfactorymanager.h"
#include "application/clientapplicationmanager.h"
#include "netio/netio.h"
#include "jobstimerappprotocolhandler.h"
#include "jobstimerprotocol.h"
#include "protocols/protocolmanager.h"
using namespace app_proxypublish;

ProxyPublishApplication::ProxyPublishApplication(Variant &configuration)
: BaseClientApplication(configuration) {
#ifdef HAS_PROTOCOL_RTMP
	_pRTMPHandler = NULL;
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_LIVEFLV
	_pLiveFLVHandler = NULL;
#endif /* HAS_PROTOCOL_LIVEFLV */
#ifdef HAS_PROTOCOL_RTP
	_pRTPHandler = NULL;
	_pRTSPHandler = NULL;
#endif /* HAS_PROTOCOL_RTP */
	_pJobsHandler = NULL;
	_jobsTimerProtocolId = 0;
}

ProxyPublishApplication::~ProxyPublishApplication() {
#ifdef HAS_PROTOCOL_RTMP
	UnRegisterAppProtocolHandler(PT_INBOUND_RTMP);
	UnRegisterAppProtocolHandler(PT_OUTBOUND_RTMP);
	if (_pRTMPHandler != NULL) {
		delete _pRTMPHandler;
		_pRTMPHandler = NULL;
	}
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_LIVEFLV
	UnRegisterAppProtocolHandler(PT_INBOUND_LIVE_FLV);
	if (_pLiveFLVHandler != NULL) {
		delete _pLiveFLVHandler;
		_pLiveFLVHandler = NULL;
	}
#endif /* HAS_PROTOCOL_LIVEFLV */
#ifdef HAS_PROTOCOL_RTP
	UnRegisterAppProtocolHandler(PT_INBOUND_RTP);
	if (_pRTPHandler != NULL) {
		delete _pRTPHandler;
		_pRTPHandler = NULL;
	}

	UnRegisterAppProtocolHandler(PT_RTSP);
	if (_pRTSPHandler != NULL) {
		delete _pRTSPHandler;
		_pRTSPHandler = NULL;
	}
#endif /* HAS_PROTOCOL_RTP */

	BaseProtocol *pProtocol = ProtocolManager::GetProtocol(_jobsTimerProtocolId);
	if (pProtocol != NULL) {
		pProtocol->EnqueueForDelete();
	}

	UnRegisterAppProtocolHandler(PT_TIMER);
	if (_pJobsHandler != NULL) {
		delete _pJobsHandler;
		_pJobsHandler = NULL;
	}
}

bool ProxyPublishApplication::Initialize() {
	if (!BaseClientApplication::Initialize()) {
		FATAL("Unable to initialize application");
		return false;
	}
	//1. read the target servers section, validate it and store it for later usage
	if (_configuration["abortOnConnectError"] != V_BOOL) {
		FATAL("Invalid abortOnConnectError");
		return false;
	}
	if (_configuration["targetServers"] != V_MAP) {
		FATAL("Invalid targetServers");
		return false;
	}

	FOR_MAP(_configuration["targetServers"], string, Variant, i) {
		Variant &target = MAP_VAL(i);
		if (target != V_MAP) {
			FATAL("Invalid target: %s", STR(target.ToString()));
			return false;
		}
		if (target["targetUri"] != V_STRING) {
			FATAL("Invalid target: %s", STR(target.ToString()));
			return false;
		}
		if (target.HasKey("localStreamName")) {
			if (target["localStreamName"] != V_STRING) {
				FATAL("Invalid target: %s", STR(target.ToString()));
				return false;
			}
		}
		if (target.HasKey("targetStreamName")) {
			if (target["targetStreamName"] != V_STRING) {
				FATAL("Invalid target: %s", STR(target.ToString()));
				return false;
			}
		}
		if (target.HasKey("emulateUserAgent")) {
			if (target["emulateUserAgent"] != V_STRING) {
				FATAL("Invalid target: %s", STR(target.ToString()));
				return false;
			}
		} else {
			target["emulateUserAgent"] = HTTP_HEADERS_SERVER_US;
		}

		URI uri;
		if (!URI::FromString(target["targetUri"], true, uri)) {
			FATAL("Invalid uri: %s", STR(target["targetUri"]));
			return false;
		}
		if (uri.scheme().find("rtmp") != 0) {
			FATAL("Supported target scheme is rtmp for now....");
			return false;
		}
	}
	_targetServers = _configuration["targetServers"];
	_abortOnConnectError = (bool)_configuration["abortOnConnectError"];


	//1. Initialize the protocol handler(s)
#ifdef HAS_PROTOCOL_RTMP
	_pRTMPHandler = new RTMPAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_INBOUND_RTMP, _pRTMPHandler);
	RegisterAppProtocolHandler(PT_OUTBOUND_RTMP, _pRTMPHandler);
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_LIVEFLV
	_pLiveFLVHandler = new LiveFLVAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_INBOUND_LIVE_FLV, _pLiveFLVHandler);
#endif /* HAS_PROTOCOL_LIVEFLV */

#ifdef HAS_PROTOCOL_RTP
	_pRTPHandler = new RTPAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_INBOUND_RTP, _pRTPHandler);
	RegisterAppProtocolHandler(PT_RTCP, _pRTPHandler);

	_pRTSPHandler = new RTSPAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_RTSP, _pRTSPHandler);
#endif /* HAS_PROTOCOL_RTP */

	_pJobsHandler = new JobsTimerAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_TIMER, _pJobsHandler);

	//2. Initialize the jobs timer
	BaseTimerProtocol *pProtocol = new JobsTimerProtocol();
	_jobsTimerProtocolId = pProtocol->GetId();
	pProtocol->SetApplication(this);
	pProtocol->EnqueueForTimeEvent(1);

	return PullExternalStreams();
}

void ProxyPublishApplication::UnRegisterProtocol(BaseProtocol *pProtocol) {
	//1. Get the parameters assigned to this connection
	Variant &parameters = pProtocol->GetCustomParameters();

	//FINEST("parameters:\n%s", STR(parameters.ToString()));
	//2. depending on the pull/push config, we retry only if keepAlive is true and the
	//source stream is still there
	if ((parameters.HasKeyChain(V_BOOL, true, 3, "customParameters", "localStreamConfig", "keepAlive"))
			&& ((bool)parameters["customParameters"]["localStreamConfig"]["keepAlive"])
			&& (parameters.HasKeyChain(_V_NUMERIC, true, 3, "customParameters", "localStreamConfig", "localUniqueStreamId"))) {
		//3. This is a push.  First, fix the uri
		string uri = parameters["customParameters"]["localStreamConfig"]["targetUri"]["fullUriWithAuth"];
		parameters["customParameters"]["localStreamConfig"]["targetUri"] = uri;
		EnqueuePush(parameters["customParameters"]["localStreamConfig"]);
	} else if ((parameters.HasKeyChain(V_BOOL, true, 3, "customParameters", "externalStreamConfig", "keepAlive"))
			&& ((bool)parameters["customParameters"]["externalStreamConfig"]["keepAlive"])) {
		//4. This is a pull with keep alive. Just o it again. First, fix the uri
		//which is currently "resolved"
		string uri = parameters["customParameters"]["externalStreamConfig"]["uri"]["fullUriWithAuth"];
		parameters["customParameters"]["externalStreamConfig"]["uri"] = uri;
		EnqueuePull(parameters["customParameters"]["externalStreamConfig"]);
	}

	//5. Finally, call the base class
	BaseClientApplication::UnRegisterProtocol(pProtocol);
}

void ProxyPublishApplication::SignalStreamRegistered(BaseStream *pStream) {
	//1. Call the base class
	BaseClientApplication::SignalStreamRegistered(pStream);
	if ((!TAG_KIND_OF(pStream->GetType(), ST_IN_NET_LIVEFLV))
			&& (!TAG_KIND_OF(pStream->GetType(), ST_IN_NET_RTMP))
			&& (!TAG_KIND_OF(pStream->GetType(), ST_IN_NET_RTP))) {
		FINEST("Stay put. We don't forward this kind of streams: %s",
				STR(tagToString(pStream->GetType())));
		return;
	}

	//2. Start the forwarding process
	if (!InitiateForwardingStream((BaseInStream *) pStream)) {
		FATAL("Unable to initiate the forwarding process");
		pStream->EnqueueForDelete();
	}
}

void ProxyPublishApplication::EnqueuePush(Variant &parameters) {
	//1. get the timer protocol
	JobsTimerProtocol *pProtocol = (JobsTimerProtocol *) ProtocolManager::GetProtocol(
			_jobsTimerProtocolId);
	if (pProtocol == NULL) {
		FATAL("Jobs protocol died. Aborting ...");
		return;
	}

	//2. Enqueue the operation
	pProtocol->EnqueuePush(parameters);
}

void ProxyPublishApplication::EnqueuePull(Variant &parameters) {
	//1. get the timer protocol
	JobsTimerProtocol *pProtocol = (JobsTimerProtocol *) ProtocolManager::GetProtocol(
			_jobsTimerProtocolId);
	if (pProtocol == NULL) {
		FATAL("Jobs protocol died. Aborting ...");
		return;
	}

	//2. Enqueue the operation
	pProtocol->EnqueuePull(parameters);
}

bool ProxyPublishApplication::InitiateForwardingStream(BaseInStream *pStream) {

	FOR_MAP(_targetServers, string, Variant, i) {
		Variant &target = MAP_VAL(i);
		if (!InitiateForwardingStream(pStream, target)) {
			WARN("Unable to forward stream %u of type %s with name `%s` owned by application `%s` to server %s",
					pStream->GetUniqueId(),
					STR(tagToString(pStream->GetType())),
					STR(pStream->GetName()),
					STR(GetName()),
					STR(target["targetUri"]["fullUri"]));
			if (_abortOnConnectError) {
				FATAL("Abort");
				return false;
			}
		}
	}
	return true;
}

bool ProxyPublishApplication::InitiateForwardingStream(BaseInStream *pStream, Variant &target) {
#ifndef HAS_PROTOCOL_RTMP
	FATAL("RTMP protocol not supported");
	return false;
#else
	//1. Filter the stream
	if (target.HasKey("localStreamName")) {
		if (((string) target["localStreamName"]) != pStream->GetName()) {
			if (pStream->GetName().find(((string) target["localStreamName"]) + "?") != 0) {
				FINEST("Stream name not matching. Wanted: %s(?); Got: %s",
						STR(target["localStreamName"]),
						STR(pStream->GetName()));
				return true;
			}
		}
	}

	//2. Compute the target stream name
	Variant parameters = target;
	if (!parameters.HasKey("targetStreamName"))
		parameters["targetStreamName"] = pStream->GetName();

	//3. Some nice info
	INFO("Initiate forward stream %u of type %s with name `%s` owned by application `%s` to server %s with name `%s`",
			pStream->GetUniqueId(),
			STR(tagToString(pStream->GetType())),
			STR(pStream->GetName()),
			STR(GetName()),
			STR((string) target["targetUri"]),
			STR(parameters["targetStreamName"]));

	//4. Enqueue the push
	EnqueuePush(parameters);
	return true;
#endif /* HAS_PROTOCOL_RTMP */
}
