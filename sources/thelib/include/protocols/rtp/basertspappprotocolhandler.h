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


#ifdef HAS_PROTOCOL_RTP
#ifndef _BASERTSPAPPPROTOCOLHANDLER_H
#define	_BASERTSPAPPPROTOCOLHANDLER_H

#include "application/baseappprotocolhandler.h"
#include "streaming/streamcapabilities.h"

class RTSPProtocol;
class BaseInNetStream;
class OutboundConnectivity;

class DLLEXP BaseRTSPAppProtocolHandler
: public BaseAppProtocolHandler {
protected:
	Variant _realms;
	string _usersFile;
	double _lastUsersFileUpdate;
public:
	BaseRTSPAppProtocolHandler(Variant &configuration);
	virtual ~BaseRTSPAppProtocolHandler();

	virtual bool ParseAuthenticationNode(Variant &node, Variant &result);

	virtual void RegisterProtocol(BaseProtocol *pProtocol);
	virtual void UnRegisterProtocol(BaseProtocol *pProtocol);

	virtual bool PullExternalStream(URI uri, Variant streamConfig);
	virtual bool PushLocalStream(Variant streamConfig);
	static bool SignalProtocolCreated(BaseProtocol *pProtocol,
			Variant &parameters);

	virtual bool HandleRTSPRequest(RTSPProtocol *pFrom, Variant &requestHeaders,
			string &requestContent);
	virtual bool HandleRTSPResponse(RTSPProtocol *pFrom, Variant &responseHeaders,
			string &responseContent);
protected:
	//handle requests routines
	virtual bool HandleRTSPRequestOptions(RTSPProtocol *pFrom,
			Variant &requestHeaders, string &requestContent);
	virtual bool HandleRTSPRequestDescribe(RTSPProtocol *pFrom,
			Variant &requestHeaders, string &requestContent);
	virtual bool HandleRTSPRequestSetup(RTSPProtocol *pFrom,
			Variant &requestHeaders, string &requestContent);
	virtual bool HandleRTSPRequestSetupOutbound(RTSPProtocol *pFrom,
			Variant &requestHeaders, string &requestContent);
	virtual bool HandleRTSPRequestSetupInbound(RTSPProtocol *pFrom,
			Variant &requestHeaders, string &requestContent);
	virtual bool HandleRTSPRequestPlay(RTSPProtocol *pFrom,
			Variant &requestHeaders, string &requestContent);
	virtual bool HandleRTSPRequestTearDown(RTSPProtocol *pFrom,
			Variant &requestHeaders, string &requestContent);
	virtual bool HandleRTSPRequestAnnounce(RTSPProtocol *pFrom,
			Variant &requestHeaders, string &requestContent);
	virtual bool HandleRTSPRequestRecord(RTSPProtocol *pFrom,
			Variant &requestHeaders, string &requestContent);
	virtual bool HandleRTSPRequestPause(RTSPProtocol *pFrom,
			Variant &requestHeaders, string &requestContent);

	//handle response routines
	virtual bool HandleRTSPResponse(RTSPProtocol *pFrom, Variant &requestHeaders,
			string &requestContent, Variant &responseHeaders,
			string &responseContent);
	virtual bool HandleRTSPResponse200(RTSPProtocol *pFrom, Variant &requestHeaders,
			string &requestContent, Variant &responseHeaders,
			string &responseContent);
	virtual bool HandleRTSPResponse401(RTSPProtocol *pFrom, Variant &requestHeaders,
			string &requestContent, Variant &responseHeaders,
			string &responseContent);
	virtual bool HandleRTSPResponse404(RTSPProtocol *pFrom, Variant &requestHeaders,
			string &requestContent, Variant &responseHeaders,
			string &responseContent);
	virtual bool HandleRTSPResponse200Options(RTSPProtocol *pFrom, Variant &requestHeaders,
			string &requestContent, Variant &responseHeaders,
			string &responseContent);
	virtual bool HandleRTSPResponse200Describe(RTSPProtocol *pFrom, Variant &requestHeaders,
			string &requestContent, Variant &responseHeaders,
			string &responseContent);
	virtual bool HandleRTSPResponse200Setup(RTSPProtocol *pFrom, Variant &requestHeaders,
			string &requestContent, Variant &responseHeaders,
			string &responseContent);
	virtual bool HandleRTSPResponse200Play(RTSPProtocol *pFrom, Variant &requestHeaders,
			string &requestContent, Variant &responseHeaders,
			string &responseContent);
	virtual bool HandleRTSPResponse200Announce(RTSPProtocol *pFrom, Variant &requestHeaders,
			string &requestContent, Variant &responseHeaders,
			string &responseContent);
	virtual bool HandleRTSPResponse200Record(RTSPProtocol *pFrom, Variant &requestHeaders,
			string &requestContent, Variant &responseHeaders,
			string &responseContent);
	virtual bool HandleRTSPResponse404Play(RTSPProtocol *pFrom, Variant &requestHeaders,
			string &requestContent, Variant &responseHeaders,
			string &responseContent);
	virtual bool HandleRTSPResponse404Describe(RTSPProtocol *pFrom, Variant &requestHeaders,
			string &requestContent, Variant &responseHeaders,
			string &responseContent);

	//operations
	virtual bool TriggerPlayOrAnnounce(RTSPProtocol *pFrom);
protected:
	virtual bool NeedAuthentication(RTSPProtocol *pFrom,
			Variant &requestHeaders, string &requestContent);
	virtual string GetAuthenticationRealm(RTSPProtocol *pFrom,
			Variant &requestHeaders, string &requestContent);
private:
	OutboundConnectivity *GetOutboundConnectivity(RTSPProtocol *pFrom, bool forceTcp);
	BaseInNetStream *GetInboundStream(string streamName);
	StreamCapabilities *GetInboundStreamCapabilities(string streamName);
	string GetAudioTrack(RTSPProtocol *pFrom,
			StreamCapabilities *pCapabilities);
	string GetVideoTrack(RTSPProtocol *pFrom,
			StreamCapabilities *pCapabilities);
	bool SendSetupTrackMessages(RTSPProtocol *pFrom);
	bool ParseUsersFile();
	bool SendAuthenticationChallenge(RTSPProtocol *pFrom, Variant &realm);
	string ComputeSDP(RTSPProtocol *pFrom, string localStreamName,
			string targetStreamName, string host);
};


#endif	/* _BASERTSPAPPPROTOCOLHANDLER_H */
#endif /* HAS_PROTOCOL_RTP */
