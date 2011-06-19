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

#ifdef HAS_PROTOCOL_RAWHTTPSTREAM

#include "protocols/rawhttpstream/inboundrawhttpstreamprotocol.h"
#include "protocols/http/inboundhttpprotocol.h"
#include "streaming/baseinstream.h"
#include "application/baseclientapplication.h"
#include "streaming/streamstypes.h"
#include "streaming/outnetrawstream.h"

InboundRawHTTPStreamProtocol::InboundRawHTTPStreamProtocol()
: BaseProtocol(PT_INBOUND_RAW_HTTP_STREAM) {
	_streamNameAcquired = false;
	_headersSent = false;
	_pOutStream = NULL;
}

InboundRawHTTPStreamProtocol::~InboundRawHTTPStreamProtocol() {
	if (_pOutStream != NULL) {
		delete _pOutStream;
		_pOutStream = NULL;
	}
}

bool InboundRawHTTPStreamProtocol::Initialize(Variant &parameters) {
	if ((parameters["crossDomainFile"] != V_STRING)
			|| ((string) parameters["crossDomainFile"] == "")) {
		FATAL("crossDomainFile not specified");
		return false;
	}
	GetCustomParameters() = parameters;
	_crossDomainFile = (string) parameters["crossDomainFile"];
	return true;
}

IOBuffer * InboundRawHTTPStreamProtocol::GetOutputBuffer() {
	if (GETAVAILABLEBYTESCOUNT(_outputBuffer) != 0)
		return &_outputBuffer;
	return NULL;
}

bool InboundRawHTTPStreamProtocol::AllowFarProtocol(uint64_t type) {
	return (type == PT_TCP) || (type == PT_INBOUND_SSL);
}

bool InboundRawHTTPStreamProtocol::AllowNearProtocol(uint64_t type) {
	ASSERT("This is an endpoint protocol");
	return false;
}

bool InboundRawHTTPStreamProtocol::SignalInputData(int32_t recvAmount) {
	NYIR;
}

bool InboundRawHTTPStreamProtocol::SignalInputData(IOBuffer &buffer) {
	//1. Is the stream name acquired?
	if (_streamNameAcquired) {
		buffer.IgnoreAll();
		return true;
	}

	if (!AcquireStreamName(buffer)) {
		FATAL("Unable to get the stream name");
		return false;
	}

	if (!_streamNameAcquired) {
		return true;
	}

	//7. Search for the stream called streamName and pick the first one
	map<uint32_t, BaseStream *> inStreams =
			GetApplication()->GetStreamsManager()->FindByTypeByName(
			ST_IN_NET_RAW, _streamName, false, true);
	if (inStreams.size() == 0) {
		if (lowerCase(_streamName) == "crossdomain.xml") {
			return SendCrossDomain();
		} else {
			FATAL("Stream %s not found", STR(_streamName));
			return Send404NotFound();
		}
	}
	BaseInStream *pInStream = (BaseInStream *) MAP_VAL(inStreams.begin());

	//8. Create our raw outbound stream
	_pOutStream = new OutNetRawStream(this,
			GetApplication()->GetStreamsManager(), _streamName);

	//9. Link it to the in stream
	if (!pInStream->Link(_pOutStream)) {
		FATAL("Unable to link to the in stream");
		return false;
	}

	//10. All done. Ignore all the traffic
	buffer.IgnoreAll();

	//11. Done
	return true;
}

bool InboundRawHTTPStreamProtocol::PutData(uint8_t *pBuffer, uint32_t length) {
	if (!_headersSent) {
		_outputBuffer.ReadFromString("HTTP/1.1 200 OK\r\n");
		_outputBuffer.ReadFromString(HTTP_HEADERS_SERVER": "HTTP_HEADERS_SERVER_US"\r\n");
		_outputBuffer.ReadFromString(HTTP_HEADERS_X_POWERED_BY": "HTTP_HEADERS_X_POWERED_BY_US"\r\n");
		if (_pOutStream != NULL) {
			StreamCapabilities *pCapabilities = _pOutStream->GetCapabilities();
			if (pCapabilities != NULL) {
				switch (pCapabilities->audioCodecId) {
					case CODEC_AUDIO_MP3:
						_outputBuffer.ReadFromString(HTTP_HEADERS_CONTENT_TYPE": audio/mpeg\r\n");
						break;
					case CODEC_AUDIO_ADTS:
						_outputBuffer.ReadFromString(HTTP_HEADERS_CONTENT_TYPE": audio/x-aac\r\n");
						break;
					default:
						break;
				}
			}
		}
		//_outputBuffer.ReadFromString(HTTP_HEADERS_TRANSFER_ENCODING": "HTTP_HEADERS_TRANSFER_ENCODING_CHUNKED"\r\n\r\n");
		_outputBuffer.ReadFromString(HTTP_HEADERS_CONTENT_LENGTH": 4294967296\r\n\r\n");
		_headersSent = true;
	}
	//_outputBuffer.ReadFromString(format("%x\r\n", length));
	_outputBuffer.ReadFromBuffer(pBuffer, length);
	//_outputBuffer.ReadFromString("\r\n");
	return EnqueueForOutbound();
}

bool InboundRawHTTPStreamProtocol::AcquireStreamName(IOBuffer &buffer) {
	//1. Get the buffer and length
	uint32_t length = GETAVAILABLEBYTESCOUNT(buffer);
	if (length < 16) {
		return true;
	}
	uint8_t *pBuffer = GETIBPOINTER(buffer);


	//2. search for the first \r\n sequence
	bool found = false;
	for (uint32_t i = 0; i < length - 1; i++) {
		//3. Limit the search to 1024 bytes
		if (i >= 1024) {
			FATAL("HTTP first line request too long");
			return false;
		}
		if ((pBuffer[i] == '\r') && (pBuffer[i + 1] == '\n')) {
			pBuffer[i] = 0;
			found = true;
			break;
		}
	}
	if (!found) {
		return true;
	}

	//3. Get the first line
	string rawLine = (char *) pBuffer;

	//4. Split it into components delimited by space characters
	vector<string> parts;
	split(rawLine, " ", parts);

	//5. validate the parts
	if (parts.size() != 3) {
		FATAL("Invalid first line: %s", STR(rawLine));
		return false;
	}
	if (parts[0] != HTTP_METHOD_GET) {
		FATAL("Invalid first line: %s", STR(rawLine));
		return false;
	}
	if (parts[2] != HTTP_VERSION_1_1) {
		FATAL("Invalid first line: %s", STR(rawLine));
		return false;
	}
	if (parts[1].length() < 2) {
		FATAL("Invalid first line: %s", STR(rawLine));
		return false;
	}
	if (parts[1][0] != '/') {
		FATAL("Invalid first line: %s", STR(rawLine));
		return false;
	}

	//6. Pick up the streamName
	_streamName = parts[1].substr(1);
	_streamNameAcquired = true;
	return true;
}

bool InboundRawHTTPStreamProtocol::Send404NotFound() {
	_outputBuffer.ReadFromString("HTTP/1.1 404 Not found\r\n");
	_outputBuffer.ReadFromString(HTTP_HEADERS_SERVER": "HTTP_HEADERS_SERVER_US"\r\n");
	_outputBuffer.ReadFromString(HTTP_HEADERS_X_POWERED_BY": "HTTP_HEADERS_X_POWERED_BY_US"\r\n\r\n");
	if (!EnqueueForOutbound()) {
		FATAL("Unable to enqueue for outbound");
		return false;
	}
	GracefullyEnqueueForDelete();
	return true;
}

bool InboundRawHTTPStreamProtocol::SendCrossDomain() {
	if (!fileExists(_crossDomainFile)) {
		FATAL("cross domain file %s not found", STR(_crossDomainFile));
		return Send404NotFound();
	}
	File cd;
	if (!cd.Initialize(_crossDomainFile, FILE_OPEN_MODE_READ)) {
		FATAL("cross domain file %s could not be read", STR(_crossDomainFile));
		return Send404NotFound();
	}
	_outputBuffer.ReadFromString("HTTP/1.1 200 OK\r\n");
	_outputBuffer.ReadFromString(HTTP_HEADERS_SERVER": "HTTP_HEADERS_SERVER_US"\r\n");
	_outputBuffer.ReadFromString(HTTP_HEADERS_X_POWERED_BY": "HTTP_HEADERS_X_POWERED_BY_US"\r\n");
	_outputBuffer.ReadFromString(HTTP_HEADERS_CONTENT_TYPE": text/xml\r\n");
	_outputBuffer.ReadFromString(format("%s: %"PRIu64"\r\n\r\n", HTTP_HEADERS_CONTENT_LENGTH, cd.Size()));
	_outputBuffer.ReadFromFs(cd, cd.Size());
	//FINEST("_outputBuffer:\n%s", STR(_outputBuffer));
	if (!EnqueueForOutbound()) {
		FATAL("Unable to enqueue for outbound");
		return false;
	}
	GracefullyEnqueueForDelete();
	return true;
}

#endif /* HAS_PROTOCOL_RAWHTTPSTREAM */
