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
#ifndef _RTSPPROTOCOL_H
#define	_RTSPPROTOCOL_H

#include "protocols/baseprotocol.h"
#include "protocols/rtp/sdp.h"
#include "streaming/baseoutnetrtpudpstream.h"

class BaseRTSPAppProtocolHandler;
class BaseInNetStream;
class BaseOutNetRTPUDPStream;
class InboundRTPProtocol;
class InNetRTPStream;
class OutboundConnectivity;
class InboundConnectivity;

class DLLEXP RTSPProtocol
: public BaseProtocol {
protected:
	uint32_t _state;
	bool _rtpData;
	uint32_t _rtpDataLength;
	uint32_t _rtpDataChanel;
	Variant _inboundHeaders;
	string _inboundContent;
	uint32_t _contentLength;
	SDP _inboundSDP;
	BaseRTSPAppProtocolHandler *_pProtocolHandler;
	IOBuffer _outputBuffer;

	Variant _responseHeaders;
	string _responseContent;

	Variant _requestHeaders;
	string _requestContent;
	uint32_t _requestSequence;

	OutboundConnectivity *_pOutboundConnectivity;
	InboundConnectivity *_pInboundConnectivity;
public:
	RTSPProtocol();
	virtual ~RTSPProtocol();

	virtual IOBuffer * GetOutputBuffer();
	virtual bool Initialize(Variant &parameters);
	virtual void SetApplication(BaseClientApplication *pApplication);
	virtual bool AllowFarProtocol(uint64_t type);
	virtual bool AllowNearProtocol(uint64_t type);
	virtual bool SignalInputData(int32_t recvAmount);
	virtual bool SignalInputData(IOBuffer &buffer);
	virtual void GetStats(Variant &info);

	SDP &GetInboundSDP();

	void ClearRequestMessage();
	void PushRequestFirstLine(string method, string url, string version);
	void PushRequestHeader(string name, string value);
	void PushRequestContent(string outboundContent, bool append);
	bool SendRequestMessage();
	uint32_t LastRequestSequence();
	Variant &GetRequestHeaders();
	string &GetRequestContent();

	void ClearResponseMessage();
	void PushResponseFirstLine(string version, uint32_t code, string reason);
	void PushResponseHeader(string name, string value);
	void PushResponseContent(string outboundContent, bool append);
	bool SendResponseMessage();

	OutboundConnectivity * GetOutboundConnectivity(BaseInNetStream *pInNetStream);
	void CloseOutboundConnectivity();

	InboundConnectivity *GetInboundConnectivity(Variant &videoTrack,
			Variant &audioTrack, string sdpStreamName);
	void CloseInboundConnectivity();

	string GetTransportHeaderLine(bool isAudio);

	bool SendRaw(uint8_t *pBuffer, uint32_t length);

private:
	bool SendMessage(Variant &headers, string &content);
	bool ParseHeaders(IOBuffer &buffer);
	bool ParseInterleavedHeaders(IOBuffer &buffer);
	bool ParseNormalHeaders(IOBuffer &buffer);
	bool ParseFirstLine(string &line);
	bool HandleRTSPMessage(IOBuffer &buffer);
};

#endif	/* _RTSPPROTOCOL_H */
#endif /* HAS_PROTOCOL_RTP */
