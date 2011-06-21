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


#ifdef HAS_PROTOCOL_HTTP
#include "protocols/http/basehttpprotocol.h"
#include "protocols/tcpprotocol.h"

#define HTTP_STATE_HEADERS 0
#define HTTP_STATE_PAYLOAD 1
#define HTTP_MAX_LINE_LENGTH 256
#define HTTP_MAX_HEADERS_COUNT 64
#define HTTP_MAX_HEADERS_SIZE 2048
#define HTTP_MAX_CHUNK_SIZE 1024*128

BaseHTTPProtocol::BaseHTTPProtocol(uint64_t protocolType)
: BaseProtocol(protocolType) {
	_state = HTTP_STATE_HEADERS;
	_chunkedContent = false;
	_lastChunk = false;
	_contentLength = 0;
	_disconnectAfterTransfer = false;
	_sessionDecodedBytesCount = 0;
	_decodedBytesCount = 0;
	_hasAuth = false;
	_outboundHeaders.IsArray(false);
	_continueAfterParseHeaders = true;
}

BaseHTTPProtocol::~BaseHTTPProtocol() {

}

IOBuffer * BaseHTTPProtocol::GetOutputBuffer() {
	if (GETAVAILABLEBYTESCOUNT(_outputBuffer) > 0) {
		return &_outputBuffer;
	}
	return NULL;
}

IOBuffer * BaseHTTPProtocol::GetInputBuffer() {
	return &_inputBuffer;
}

bool BaseHTTPProtocol::AllowFarProtocol(uint64_t type) {
	return type == PT_TCP
			|| type == PT_INBOUND_SSL
			|| type == PT_OUTBOUND_SSL;
}

bool BaseHTTPProtocol::AllowNearProtocol(uint64_t type) {
	//we can tunnel any kind of data using HTTP
	return true;
}

uint64_t BaseHTTPProtocol::GetDecodedBytesCount() {
	return _decodedBytesCount;
}

bool BaseHTTPProtocol::Initialize(Variant &parameters) {
	return true;
}

bool BaseHTTPProtocol::EnqueueForOutbound() {
	//1. Get the output buffer
	if (_pNearProtocol == NULL) {
		FATAL("No near protocol");
		return false;
	}
	IOBuffer *pBuffer = _pNearProtocol->GetOutputBuffer();
	uint32_t bufferLength = 0;
	if (pBuffer != NULL) {
		bufferLength = GETAVAILABLEBYTESCOUNT(*pBuffer);
	}

	//3. add or replace X-Powered-By attribute
	_outboundHeaders[HTTP_HEADERS_X_POWERED_BY] = HTTP_HEADERS_X_POWERED_BY_US;

	//4. add or replace the Server attribute
	if (GetType() == PT_INBOUND_HTTP) {
		_outboundHeaders[HTTP_HEADERS_SERVER] = HTTP_HEADERS_SERVER_US;
	}

	//5. Get rid of the Content-Length attribute and add it only if necessary
	_outboundHeaders.RemoveKey(HTTP_HEADERS_CONTENT_LENGTH);
	if (bufferLength > 0) {
		_outboundHeaders[HTTP_HEADERS_CONTENT_LENGTH] = format("%u", bufferLength);
	}

	//6. Get rid of Transfer-Encoding attribute
	_outboundHeaders.RemoveKey(HTTP_HEADERS_TRANSFER_ENCODING);

	//7. Write the first line of the request/response
	_outputBuffer.ReadFromString(GetOutputFirstLine() + "\r\n");

	//8. Write the headers and the final '\r\n'

	FOR_MAP(_outboundHeaders, string, Variant, i) {
		if (MAP_VAL(i) != V_STRING) {
			FATAL("Invalid HTTP headers:\n%s", STR(_outboundHeaders.ToString()));
			return false;
		}
		_outputBuffer.ReadFromString(format("%s: %s\r\n", STR(MAP_KEY(i)), STR(MAP_VAL(i))));
	}
	_outboundHeaders.Reset();
	_outboundHeaders.IsArray(false);
	_outputBuffer.ReadFromString("\r\n");

	//9. Write the actual content if necessary
	if (bufferLength > 0) {
		_outputBuffer.ReadFromBuffer(GETIBPOINTER(*pBuffer), GETAVAILABLEBYTESCOUNT(*pBuffer));

		//10. Empty the upper output buffer
		pBuffer->IgnoreAll();
	}

	//11. Let the request flow further
	return BaseProtocol::EnqueueForOutbound();
}

bool BaseHTTPProtocol::SignalInputData(int32_t recvAmount) {
	ASSERT("OPERATION NOT SUPPORTED");
	return false;
}

bool BaseHTTPProtocol::SignalInputData(IOBuffer &buffer) {
	//1. Get the first line and the headers if necessary
	if (_state == HTTP_STATE_HEADERS) {
		if (!ParseHeaders(buffer)) {
			FATAL("Unable to read response headers");
			return false;
		}
	}

	if (!_continueAfterParseHeaders)
		return true;

	//2. Are we still in the "get headers state"? If so, wait for more data
	if (_state != HTTP_STATE_PAYLOAD) {
		return true;
	}

	//3. Turning point in processing
	if (_chunkedContent) {
		//4. We deal with chunked content
		if (!HandleChunkedContent(buffer)) {
			FATAL("Unable to handle chunked content");
			return false;
		}
	} else {
		//5. We deal with length-specified type of content
		if (!HandleFixedLengthContent(buffer)) {
			FATAL("Unable to handle fixed length content");
			return false;
		}
	}

	//6. Are we in the get headers state? if so, we might have a new request
	//on the pipe.
	if (_state == HTTP_STATE_HEADERS) {
		//7. So, get to work again...
		return SignalInputData(buffer);
	} else {
		//8 We are done :)
		return true;
	}
}

bool BaseHTTPProtocol::GetDisconnectAfterTransfer() {
	return _disconnectAfterTransfer;
}

void BaseHTTPProtocol::SetDisconnectAfterTransfer(bool disconnectAfterTransfer) {
	_disconnectAfterTransfer = disconnectAfterTransfer;
}

bool BaseHTTPProtocol::IsChunkedContent() {
	return _chunkedContent;
}

bool BaseHTTPProtocol::IsLastChunk() {
	return _lastChunk;
}

uint32_t BaseHTTPProtocol::GetContentLength() {
	return _contentLength;
}

uint32_t BaseHTTPProtocol::GetSessionDecodedBytesCount() {
	return _sessionDecodedBytesCount;
}

bool BaseHTTPProtocol::TransferCompleted() {
	if (_state != HTTP_STATE_PAYLOAD)
		return false;
	if (_chunkedContent) {
		return _lastChunk;
	} else {
		assert(_sessionDecodedBytesCount <= _contentLength);
		return _sessionDecodedBytesCount == _contentLength;
	}
}

Variant BaseHTTPProtocol::GetHeaders() {
	return _headers;
}

void BaseHTTPProtocol::SetOutboundHeader(string name, string value) {
	_outboundHeaders[name] = value;
}

string BaseHTTPProtocol::DumpState() {
	string result = "";

	result += _state == HTTP_STATE_HEADERS ? "HTTP_STATE_HEADERS\n" : "HTTP_STATE_PAYLOAD\n";
	result += format("_chunkedContent: %hhu\n", _chunkedContent);
	result += format("_lastChunk: %hhu\n", _lastChunk);
	result += format("_contentLength: %u\n", _contentLength);
	result += format("_sessionDecodedBytesCount: %u\n", _sessionDecodedBytesCount);
	result += format("_decodedBytesCount: %u\n", _decodedBytesCount);
	result += format("_disconnectAfterTransfer: %hhu\n", _disconnectAfterTransfer);
	result += format("TransferCompleted(): %hhu\n", TransferCompleted());
	result += format("_headers:\n%s\n", STR(_headers.ToString()));
	result += format("_outputBuffer:\n%s\n", STR(_outputBuffer));
	result += format("_inputBuffer:\n%s", STR(_inputBuffer));

	return result;
}

bool BaseHTTPProtocol::ParseHeaders(IOBuffer& buffer) {
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
		if (i >= HTTP_MAX_HEADERS_SIZE) {
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
		FATAL("Incorrect HTTP request");
		return false;
	}

	//4. Get the fisrt line and parse it. This is either a status code
	//for a previous request made by us, or the request that we just received
	if (!ParseFirstLine(lines[0], _headers[HTTP_FIRST_LINE])) {
		FATAL("Unable to parse the first line");
		return false;
	}

	//5. Consider the rest of the lines as key: value pairs and store them
	//0. Reset the headers
	_headers[HTTP_HEADERS].IsArray(false);
	for (uint32_t i = 1; i < lines.size(); i++) {
		string line = lines[i];
		string::size_type splitterPos = line.find(": ");

		if ((splitterPos == string::npos)
				|| (splitterPos == 0)
				|| (splitterPos == line.size() - 2)) {
			FATAL("Invalid header line");
			return false;
		}
		_headers[HTTP_HEADERS][line.substr(0, splitterPos)] = line.substr(splitterPos + 2, string::npos);
	}

	//6. default a transfer type to Content-Length: 0 if necessary
	if ((!_headers[HTTP_HEADERS].HasKey(HTTP_HEADERS_CONTENT_LENGTH, false)) &&
			(!_headers[HTTP_HEADERS].HasKey(HTTP_HEADERS_TRANSFER_ENCODING, false))) {
		_headers[HTTP_HEADERS][HTTP_HEADERS_CONTENT_LENGTH] = "0";
	}

	//7. read the transfer type and set this request or response flags
	if (_headers[HTTP_HEADERS].HasKey(HTTP_HEADERS_CONTENT_LENGTH, false)) {
		string contentLengthString = _headers[HTTP_HEADERS].GetValue(
				HTTP_HEADERS_CONTENT_LENGTH, false);
		replace(contentLengthString, " ", "");
		if (!isNumeric(contentLengthString)) {
			FATAL("Invalid HTTP headers:\n%s", STR(_headers.ToString()));
			return false;
		}
		_contentLength = atoi(STR(contentLengthString));
		_chunkedContent = false;
		_lastChunk = false;
	} else if (_headers[HTTP_HEADERS].HasKey(HTTP_HEADERS_TRANSFER_ENCODING, false)) {
		if (lowerCase(_headers[HTTP_HEADERS].GetValue(HTTP_HEADERS_TRANSFER_ENCODING, false)) !=
				lowerCase(HTTP_HEADERS_TRANSFER_ENCODING_CHUNKED)) {
			FATAL("The only supported %s is %s",
					HTTP_HEADERS_TRANSFER_ENCODING,
					HTTP_HEADERS_TRANSFER_ENCODING_CHUNKED);
			return false;
		}
		_chunkedContent = true;
		_lastChunk = false;
		_contentLength = 0;
	}

	//7. Advance the state and ignore the headers part from the buffer
	_state = HTTP_STATE_PAYLOAD;
	buffer.Ignore(headersSize + 4);

	return Authenticate();
}

bool BaseHTTPProtocol::HandleChunkedContent(IOBuffer &buffer) {
	//2. We cycle until we don't have any complete chunks anymore
	//or we hit the 0 bytes chunks (end of chunked content)
	uint8_t *pBuffer = NULL;
	string chunkSizeString = "";
	uint32_t chunkSize = 0;
	while (true) {
		//1. Get the raw pointer. We need it almost everywhere
		pBuffer = GETIBPOINTER(buffer);

		//3. Read the string which represents the number of bytes in the chunk
		chunkSizeString = "0x";
		for (uint32_t i = 0; i < GETAVAILABLEBYTESCOUNT(buffer) - 1; i++) {
			//5. Are we at the end of chunk size string?
			if (pBuffer[i] == 0x0d
					&& pBuffer[i + 1] == 0x0a) {
				break;
			}

			//4. If the chunk size string has more than 10 bytes, we drop it like
			//is hot! Also test each char to be a hex number.
			//This is a bogus request/response
			if (i >= 10 || (!(((pBuffer[i] >= '0') && (pBuffer[i] <= '9'))
					|| ((pBuffer[i] >= 'a') && (pBuffer[i] <= 'f'))
					|| ((pBuffer[i] >= 'A') && (pBuffer[i] <= 'F'))))) {
				FATAL("Unable to read chunk size length");
				return false;
			}

			//6. Save the current character
			chunkSizeString += (char) pBuffer[i];
		}
		//7. Test the newly extracted chunk size
		if (chunkSizeString == "0x") {
			FATAL("Unable to read chunk size");
			return false;
		}

		//8. Get its actual value and test it as well
		chunkSize = strtol(STR(chunkSizeString), NULL, 16);
		if (chunkSize > HTTP_MAX_CHUNK_SIZE) {
			FATAL("Chunk size too large. Maximum allowed is %u and we got %u",
					(uint32_t) HTTP_MAX_CHUNK_SIZE, chunkSize);
			return false;
		}

		//9. Now, we know the chunk size... do we have enough data?
		if (GETAVAILABLEBYTESCOUNT(buffer) <
				chunkSizeString.size() //length of the chunk size string
				- 2 //substract the 0x particle
				+ 2 //the \r\n that follows the chunk size string
				+ chunkSize //chunk size itself
				+ 2 //the \r\n that follows the data chunk
				) {
			return true;
		}

		//10. Update the session decoded bytes count and decoded bytes count
		_sessionDecodedBytesCount += chunkSize;
		_decodedBytesCount += chunkSize;

		if (chunkSize != 0) {
			//11. Make the copy
			_contentLength += chunkSize;
			_inputBuffer.ReadFromBuffer(GETIBPOINTER(buffer) + chunkSizeString.size() - 2 + 2, chunkSize);
		} else {
			//12. This was the last chunk (0 bytes size)
			_lastChunk = true;
		}

		//12. Call the near protocol
		if (!_pNearProtocol->SignalInputData(_inputBuffer)) {
			FATAL("Unable to call the next protocol in stack");
			return false;
		}

		//13. Ignore the bytes from the input buffer
		buffer.Ignore(chunkSizeString.size() - 2 + 2 + chunkSize + 2);

		//14. reset the state if necessary
		if (TransferCompleted()) {
			_headers.Reset();
			_chunkedContent = false;
			_lastChunk = false;
			_contentLength = 0;
			_state = HTTP_STATE_HEADERS;
			_sessionDecodedBytesCount = 0;
			return true;
		}
	}
}

bool BaseHTTPProtocol::HandleFixedLengthContent(IOBuffer &buffer) {
	//1. Compute the chunk size that we areg going to read
	//which is how many bytes we have available, but no more than _contentLength
	uint32_t chunkSize = GETAVAILABLEBYTESCOUNT(buffer);
	assert(_sessionDecodedBytesCount <= _contentLength);
	uint32_t remaining = _contentLength - _sessionDecodedBytesCount;
	chunkSize = chunkSize > remaining ? remaining : chunkSize;

	//2. Update the session decoded bytes count and decoded bytes count
	_sessionDecodedBytesCount += chunkSize;
	_decodedBytesCount += chunkSize;

	//3. Make the copy and ignore the chunk size
	_inputBuffer.ReadFromBuffer(GETIBPOINTER(buffer), chunkSize);
	buffer.Ignore(chunkSize);

	//3. Call the near protocol
	if (!_pNearProtocol->SignalInputData(_inputBuffer)) {
		FATAL("Unable to call the next protocol in stack");
		return false;
	}

	//4. reset the state if necessary
	if (TransferCompleted()) {
		_headers.Reset();
		_contentLength = 0;
		_chunkedContent = false;
		_lastChunk = false;
		_state = HTTP_STATE_HEADERS;
		_sessionDecodedBytesCount = 0;
	}

	//5. we are done
	return true;
}
#endif /* HAS_PROTOCOL_HTTP */

