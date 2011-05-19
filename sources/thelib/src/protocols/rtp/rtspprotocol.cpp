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
#include "protocols/rtp/rtspprotocol.h"
#include "application/baseclientapplication.h"
#include "protocols/rtp/basertspappprotocolhandler.h"
#include "protocols/protocolmanager.h"
#include "streaming/streamstypes.h"
#include "streaming/baseinnetstream.h"
#include "protocols/rtp/streaming/outnetrtpudph264stream.h"
#include "protocols/rtp/streaming/innetrtpstream.h"
#include "protocols/protocolfactorymanager.h"
#include "protocols/rtp/inboundrtpprotocol.h"
#include "netio/netio.h"
#include "protocols/rtp/connectivity/outboundconnectivity.h"
#include "protocols/rtp/connectivity/inboundconnectivity.h"

#define RTSP_STATE_HEADERS 0
#define RTSP_STATE_PAYLOAD 1
#define RTSP_MAX_LINE_LENGTH 256
#define RTSP_MAX_HEADERS_COUNT 64
#define RTSP_MAX_HEADERS_SIZE 2048
#define RTSP_MAX_CHUNK_SIZE 1024*128

RTSPProtocol::RTSPProtocol()
: BaseProtocol(PT_RTSP) {
	_state = RTSP_STATE_HEADERS;
	_rtpData = false;
	_rtpDataLength = 0;
	_rtpDataChanel = 0;
	_contentLength = 0;
	_requestSequence = 0;
	_pOutboundConnectivity = NULL;
	_pInboundConnectivity = NULL;
	_basicAuthentication = "";
}

RTSPProtocol::~RTSPProtocol() {
	CloseOutboundConnectivity();
	CloseInboundConnectivity();
}

IOBuffer * RTSPProtocol::GetOutputBuffer() {
	if (GETAVAILABLEBYTESCOUNT(_outputBuffer))
		return &_outputBuffer;
	return NULL;
}

bool RTSPProtocol::Initialize(Variant &parameters) {
	GetCustomParameters() = parameters;
	return true;
}

void RTSPProtocol::SetApplication(BaseClientApplication *pApplication) {
	BaseProtocol::SetApplication(pApplication);
	if (pApplication != NULL) {
		_pProtocolHandler = (BaseRTSPAppProtocolHandler *)
				pApplication->GetProtocolHandler(GetType());
		if (_pProtocolHandler == NULL) {
			FATAL("Protocol handler not found");
			EnqueueForDelete();
		}
	} else {
		_pProtocolHandler = NULL;
	}
}

bool RTSPProtocol::AllowFarProtocol(uint64_t type) {
	return type == PT_TCP;
}

bool RTSPProtocol::AllowNearProtocol(uint64_t type) {
	return type == PT_INBOUND_RTP;
}

bool RTSPProtocol::SignalInputData(int32_t recvAmount) {
	NYIR;
}

void RTSPProtocol::GetStats(Variant &info) {
	BaseProtocol::GetStats(info);
	info["streams"].IsArray(true);
	Variant si;
	if (GetApplication() != NULL) {
		StreamsManager *pStreamsManager = GetApplication()->GetStreamsManager();
		map<uint32_t, BaseStream*> streams = pStreamsManager->FindByProtocolId(GetId());

		FOR_MAP(streams, uint32_t, BaseStream *, i) {
			si.Reset();
			MAP_VAL(i)->GetStats(si);
			info["streams"].PushToArray(si);
		}
	}
}

void RTSPProtocol::SetBasicAuthentication(string userName, string password) {
	_basicAuthentication = format("Basic %s", STR(b64(format("%s:%s", STR(userName), STR(password)))));
}

SDP &RTSPProtocol::GetInboundSDP() {
	return _inboundSDP;
}

bool RTSPProtocol::SignalInputData(IOBuffer &buffer) {
	while (GETAVAILABLEBYTESCOUNT(buffer) > 0) {
		switch (_state) {
			case RTSP_STATE_HEADERS:
			{
				if (!ParseHeaders(buffer)) {
					FATAL("Unable to read headers");
					return false;
				}
				if (_state != RTSP_STATE_PAYLOAD) {
					return true;
				}
			}
			case RTSP_STATE_PAYLOAD:
			{
				if (_rtpData) {
					if (_pInboundConnectivity == NULL) {
						FATAL("No inbound connectivity available");
						return false;
					}
					if (!_pInboundConnectivity->FeedData(
							_rtpDataChanel, GETIBPOINTER(buffer), _rtpDataLength)) {
						FATAL("Unable to handle raw RTP packet");
						return false;
					}
					buffer.Ignore(_rtpDataLength);
					_state = RTSP_STATE_HEADERS;
				} else {
					if (!HandleRTSPMessage(buffer)) {
						FATAL("Unable to handle content");
						return false;
					}
				}
				break;
			}
			default:
			{
				ASSERT("Invalid RTSP state");
				return false;
			}
		}
	}

	return true;
}

void RTSPProtocol::ClearRequestMessage() {
	_requestHeaders.Reset();
	_requestContent = "";
}

void RTSPProtocol::PushRequestFirstLine(string method, string url,
		string version) {
	_requestHeaders[RTSP_FIRST_LINE][RTSP_METHOD] = method;
	_requestHeaders[RTSP_FIRST_LINE][RTSP_URL] = url;
	_requestHeaders[RTSP_FIRST_LINE][RTSP_VERSION] = version;
}

void RTSPProtocol::PushRequestHeader(string name, string value) {
	_requestHeaders[RTSP_HEADERS][name] = value;
}

void RTSPProtocol::PushRequestContent(string outboundContent, bool append) {
	if (append)
		_requestContent += "\r\n" + outboundContent;
	else
		_requestContent = outboundContent;
}

bool RTSPProtocol::SendRequestMessage() {
	//1. Put the first line
	_outputBuffer.ReadFromString(format("%s %s %s\r\n",
			STR(_requestHeaders[RTSP_FIRST_LINE][RTSP_METHOD]),
			STR(_requestHeaders[RTSP_FIRST_LINE][RTSP_URL]),
			STR(_requestHeaders[RTSP_FIRST_LINE][RTSP_VERSION])));

	//2. Put our request sequence in place
	_requestHeaders[RTSP_HEADERS][RTSP_HEADERS_CSEQ] = format("%u", ++_requestSequence);

	//3. Put authentication if necessary
	if (_basicAuthentication != "") {
		_requestHeaders[RTSP_HEADERS][RTSP_HEADERS_AUTHORIZATION] = _basicAuthentication;
	}

	//3. send the mesage
	return SendMessage(_requestHeaders, _requestContent);
}

uint32_t RTSPProtocol::LastRequestSequence() {
	return _requestSequence;
}

Variant &RTSPProtocol::GetRequestHeaders() {
	return _requestHeaders;
}

string &RTSPProtocol::GetRequestContent() {
	return _requestContent;
}

void RTSPProtocol::ClearResponseMessage() {
	_responseHeaders.Reset();
	_responseContent = "";
}

void RTSPProtocol::PushResponseFirstLine(string version, uint32_t code,
		string reason) {
	_responseHeaders[RTSP_FIRST_LINE][RTSP_VERSION] = version;
	_responseHeaders[RTSP_FIRST_LINE][RTSP_STATUS_CODE] = code;
	_responseHeaders[RTSP_FIRST_LINE][RTSP_STATUS_CODE_REASON] = reason;
}

void RTSPProtocol::PushResponseHeader(string name, string value) {
	_responseHeaders[RTSP_HEADERS][name] = value;
}

void RTSPProtocol::PushResponseContent(string outboundContent, bool append) {
	if (append)
		_responseContent += "\r\n" + outboundContent;
	else
		_responseContent = outboundContent;
}

bool RTSPProtocol::SendResponseMessage() {
	//1. Put the first line
	_outputBuffer.ReadFromString(format("%s %u %s\r\n",
			STR(_responseHeaders[RTSP_FIRST_LINE][RTSP_VERSION]),
			(uint32_t) _responseHeaders[RTSP_FIRST_LINE][RTSP_STATUS_CODE],
			STR(_responseHeaders[RTSP_FIRST_LINE][RTSP_STATUS_CODE_REASON])));

	//2. send the mesage
	return SendMessage(_responseHeaders, _responseContent);
}

OutboundConnectivity * RTSPProtocol::GetOutboundConnectivity(
		BaseInNetStream *pInNetStream) {
	if (_pOutboundConnectivity == NULL) {
		vector<BaseOutStream *> outStreams = pInNetStream->GetOutStreams();

		BaseOutNetRTPUDPStream *pOutStream = NULL;

		FOR_VECTOR(outStreams, i) {
			if (outStreams[i]->GetType() == ST_OUT_NET_RTP) {
				pOutStream = (BaseOutNetRTPUDPStream *) outStreams[i];
				break;
			}
		}

		if (pOutStream == NULL) {
			FINEST("Create stream");
			pOutStream = new OutNetRTPUDPH264Stream(this,
					GetApplication()->GetStreamsManager(), pInNetStream->GetName());
			if (!pInNetStream->Link(pOutStream)) {
				FATAL("Unable to link streams");
				delete pOutStream;
				return false;
			}

			_pOutboundConnectivity = new OutboundConnectivity();
			if (!_pOutboundConnectivity->Initialize()) {
				FATAL("Unable to initialize outbound connectivity");
				delete pOutStream;
				delete _pOutboundConnectivity;
				_pOutboundConnectivity = NULL;
				return false;
			}
			pOutStream->SetConnectivity(_pOutboundConnectivity);
			_pOutboundConnectivity->SetOutStream(pOutStream);
		} else {
			_pOutboundConnectivity = pOutStream->GetConnectivity();
		}
	}

	return _pOutboundConnectivity;
}

void RTSPProtocol::CloseOutboundConnectivity() {
	if (_pOutboundConnectivity != NULL) {
		_pOutboundConnectivity->UnRegisterClient(GetId());
		if (!_pOutboundConnectivity->HasClients()) {
			delete _pOutboundConnectivity;
		}
		_pOutboundConnectivity = NULL;
	}
}

InboundConnectivity *RTSPProtocol::GetInboundConnectivity(Variant &videoTrack,
		Variant &audioTrack, string sdpStreamName) {
	CloseInboundConnectivity();
	string streamName;
	if (GetCustomParameters().HasKey("localStreamName")) {
		streamName = (string) GetCustomParameters()["localStreamName"];
	} else {
		streamName = sdpStreamName;
	}
	_pInboundConnectivity = new InboundConnectivity(this);
	if (!_pInboundConnectivity->Initialize(videoTrack, audioTrack,
			streamName, (bool)GetCustomParameters()["forceTcp"])) {
		FATAL("Unable to initialize inbound connectivity");
		CloseInboundConnectivity();
		return false;
	}
	return _pInboundConnectivity;
}

void RTSPProtocol::CloseInboundConnectivity() {
	if (_pInboundConnectivity != NULL) {
		delete _pInboundConnectivity;
		_pInboundConnectivity = NULL;
	}
}

string RTSPProtocol::GetTransportHeaderLine(bool isAudio) {
	if (_pInboundConnectivity == NULL)
		return "";
	return _pInboundConnectivity->GetTransportHeaderLine(isAudio);
}

bool RTSPProtocol::SendRaw(uint8_t *pBuffer, uint32_t length) {
	_outputBuffer.ReadFromBuffer(pBuffer, length);
	return EnqueueForOutbound();
}

bool RTSPProtocol::SendMessage(Variant &headers, string &content) {
	//1. Add info about us
	headers[RTSP_HEADERS][RTSP_HEADERS_SERVER] = RTSP_HEADERS_SERVER_US;
	headers[RTSP_HEADERS][RTSP_HEADERS_X_POWERED_BY] = RTSP_HEADERS_X_POWERED_BY_US;

	//2. Add the content length if required
	if (content.size() > 0) {
		headers[RTSP_HEADERS][RTSP_HEADERS_CONTENT_LENGTH] = format("%"PRIz"u", content.size());
	}

	//3. Write the headers

	FOR_MAP(headers[RTSP_HEADERS], string, Variant, i) {
		_outputBuffer.ReadFromString(MAP_KEY(i) + ": " + (string) MAP_VAL(i) + "\r\n");
	}
	_outputBuffer.ReadFromString("\r\n");

	//4. Write the content
	_outputBuffer.ReadFromString(content);

	//5. Enqueue for outbound
	return EnqueueForOutbound();
}

bool RTSPProtocol::ParseHeaders(IOBuffer& buffer) {
	if (GETAVAILABLEBYTESCOUNT(buffer) < 1) {
		FINEST("Not enough data");
		return true;
	}

	if (GETIBPOINTER(buffer)[0] == '$') {
		return ParseInterleavedHeaders(buffer);
	} else {
		return ParseNormalHeaders(buffer);
	}
}

bool RTSPProtocol::ParseInterleavedHeaders(IOBuffer &buffer) {
	//1. Marl this as a interleaved content
	_rtpData = true;

	//2. Do we have at least 4 bytes ($ sign, channel byte an 2-bytes length)?
	uint32_t bufferLength = GETAVAILABLEBYTESCOUNT(buffer);
	if (bufferLength < 4) {
		return true;
	}

	//3. Get the buffer
	uint8_t *pBuffer = GETIBPOINTER(buffer);

	//4. Get the channel id
	_rtpDataChanel = pBuffer[1];

	//5. Get the packet length
	_rtpDataLength = ENTOHSP(pBuffer + 2);
	if (_rtpDataLength > 8192) {
		FATAL("RTP data length too big");
		return false;
	}

	//6. Do we have enough data?
	if (_rtpDataLength + 4 > bufferLength) {
		return true;
	}

	//7. Advance the state and and ignore the RTP header
	buffer.Ignore(4);
	_state = RTSP_STATE_PAYLOAD;

	//8. Done

	return true;
}

bool RTSPProtocol::ParseNormalHeaders(IOBuffer &buffer) {
	_inboundHeaders.Reset();
	_inboundContent = "";
	//1. We have to have at least 4 bytes (double \r\n)
	if (GETAVAILABLEBYTESCOUNT(buffer) < 4) {
		return true;
	}

	//2. Detect the headers boundaries
	uint32_t headersSize = 0;
	bool markerFound = false;
	uint8_t *pBuffer = GETIBPOINTER(buffer);
	for (uint32_t i = 0; i <= GETAVAILABLEBYTESCOUNT(buffer) - 4; i++) {
		if ((pBuffer[i] == 0x0d)
				&& (pBuffer[i + 1] == 0x0a)
				&& (pBuffer[i + 2] == 0x0d)
				&& (pBuffer[i + 3] == 0x0a)) {
			markerFound = true;
			headersSize = i;
			break;
		}
		if (i >= RTSP_MAX_HEADERS_SIZE) {
			FATAL("Headers section too long");
			return false;
		}
	}

	//3. Are the boundaries correct?
	//Do we have enough data to parse the headers?
	if (headersSize == 0) {
		if (markerFound)
			return false;
		else
			return true;
	}

	//4. Get the raw headers and plit it into lines
	string rawHeaders = string((char *) GETIBPOINTER(buffer), headersSize);
	vector<string> lines;
	split(rawHeaders, "\r\n", lines);
	if (lines.size() == 0) {
		FATAL("Incorrect RTSP request");
		return false;
	}

	//4. Get the fisrt line and parse it. This is either a status code
	//for a previous request made by us, or the request that we just received
	if (!ParseFirstLine(lines[0])) {
		FATAL("Unable to parse the first line");
		return false;
	}

	//5. Consider the rest of the lines as key: value pairs and store them
	//0. Reset the headers
	_inboundHeaders[RTSP_HEADERS].IsArray(false);
	for (uint32_t i = 1; i < lines.size(); i++) {
		string line = lines[i];
		string::size_type splitterPos = line.find(": ");

		if ((splitterPos == string::npos)
				|| (splitterPos == 0)
				|| (splitterPos == line.size() - 2)) {
			WARN("Invalid header line: %s", STR(line));
			continue;
		}
		_inboundHeaders[RTSP_HEADERS][line.substr(0, splitterPos)] = line.substr(splitterPos + 2, string::npos);
	}

	//6. default a transfer type to Content-Length: 0 if necessary
	if (!_inboundHeaders[RTSP_HEADERS].HasKey(RTSP_HEADERS_CONTENT_LENGTH, false)) {
		_inboundHeaders[RTSP_HEADERS][RTSP_HEADERS_CONTENT_LENGTH] = "0";
	}

	//7. read the transfer type and set this request or response flags
	string contentLengthString = _inboundHeaders[RTSP_HEADERS].GetValue(
			RTSP_HEADERS_CONTENT_LENGTH, false);
	replace(contentLengthString, " ", "");
	if (!isNumeric(contentLengthString)) {
		FATAL("Invalid RTSP headers:\n%s", STR(_inboundHeaders.ToString()));
		return false;
	}
	_contentLength = atoi(STR(contentLengthString));

	//7. Advance the state and ignore the headers part from the buffer
	_state = RTSP_STATE_PAYLOAD;
	buffer.Ignore(headersSize + 4);

	_rtpData = false;

	return true;
}

bool RTSPProtocol::ParseFirstLine(string &line) {
	vector<string> parts;
	split(line, " ", parts);
	if (parts.size() < 3) {
		FATAL("Incorrect first line: %s", STR(line));
		return false;
	}

	if (parts[0] == RTSP_VERSION_1_0) {
		if (!isNumeric(parts[1])) {
			FATAL("Invalid RTSP code: %s", STR(parts[1]));
			return false;
		}

		string reason = "";
		for (uint32_t i = 2; i < parts.size(); i++) {
			reason += parts[i];
			if (i != parts.size() - 1)
				reason += " ";
		}

		_inboundHeaders[RTSP_FIRST_LINE][RTSP_VERSION] = parts[0];
		_inboundHeaders[RTSP_FIRST_LINE][RTSP_STATUS_CODE] = (uint32_t) atoi(STR(parts[1]));
		_inboundHeaders[RTSP_FIRST_LINE][RTSP_STATUS_CODE_REASON] = reason;
		_inboundHeaders["isRequest"] = false;


		return true;

	} else if ((parts[0] == RTSP_METHOD_DESCRIBE)
			|| (parts[0] == RTSP_METHOD_OPTIONS)
			|| (parts[0] == RTSP_METHOD_PAUSE)
			|| (parts[0] == RTSP_METHOD_PLAY)
			|| (parts[0] == RTSP_METHOD_SETUP)
			|| (parts[0] == RTSP_METHOD_TEARDOWN)
			|| (parts[0] == RTSP_METHOD_RECORD)
			|| (parts[0] == RTSP_METHOD_ANNOUNCE)) {
		if (parts[2] != RTSP_VERSION_1_0) {
			FATAL("RTSP version not supported: %s", STR(parts[2]));
			return false;
		}

		_inboundHeaders[RTSP_FIRST_LINE][RTSP_METHOD] = parts[0];
		_inboundHeaders[RTSP_FIRST_LINE][RTSP_URL] = parts[1];
		_inboundHeaders[RTSP_FIRST_LINE][RTSP_VERSION] = parts[2];
		_inboundHeaders["isRequest"] = true;

		return true;
	} else {
		FATAL("Incorrect first line: %s", STR(line));

		return false;
	}
}

bool RTSPProtocol::HandleRTSPMessage(IOBuffer &buffer) {
	//1. Get the content
	if (_contentLength > 0) {
		if (_contentLength > 1024 * 1024) {
			FATAL("Bogus content length: %u", _contentLength);
			return false;
		}
		uint32_t chunkLength = _contentLength - _inboundContent.size();
		chunkLength = GETAVAILABLEBYTESCOUNT(buffer) < chunkLength ?
				GETAVAILABLEBYTESCOUNT(buffer) : chunkLength;
		_inboundContent += string((char *) GETIBPOINTER(buffer), chunkLength);
		buffer.Ignore(chunkLength);
		if (_inboundContent.size() < _contentLength) {
			FINEST("Not enough data. Wanted: %u; got: %"PRIz"u", _contentLength, _inboundContent.size());
			return true;
		}
	}

	bool result;

	//2. Call the protocol handler
	if ((bool)_inboundHeaders["isRequest"]) {
		result = _pProtocolHandler->HandleRTSPRequest(this, _inboundHeaders, _inboundContent);
	} else {
		result = _pProtocolHandler->HandleRTSPResponse(this, _inboundHeaders, _inboundContent);
	}

	_state = RTSP_STATE_HEADERS;
	return result;
}

#endif /* HAS_PROTOCOL_RTP */
