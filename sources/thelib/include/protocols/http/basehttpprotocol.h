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
#ifndef _BASEHTTPPROTOCOL_H
#define	_BASEHTTPPROTOCOL_H

#include "protocols/baseprotocol.h"

class IOBuffer;
class IOBuffer;

class DLLEXP BaseHTTPProtocol
: public BaseProtocol {
protected:
	uint32_t _state;
	Variant _headers;
	bool _chunkedContent;
	bool _lastChunk;
	uint32_t _contentLength;
	uint32_t _sessionDecodedBytesCount;
	uint64_t _decodedBytesCount;
	bool _disconnectAfterTransfer;
	bool _autoFlush;
	IOBuffer _outputBuffer;
	IOBuffer _inputBuffer;
	bool _hasAuth;
	Variant _outboundHeaders;
	bool _continueAfterParseHeaders;
public:
	BaseHTTPProtocol(uint64_t protocolType);
	virtual ~BaseHTTPProtocol();

	virtual IOBuffer * GetOutputBuffer();
	virtual IOBuffer * GetInputBuffer();
	virtual bool AllowFarProtocol(uint64_t type);
	virtual bool AllowNearProtocol(uint64_t type);
	virtual uint64_t GetDecodedBytesCount();
	virtual bool Initialize(Variant &parameters);
	virtual bool EnqueueForOutbound();
	virtual bool SignalInputData(int32_t recvAmount);
	virtual bool SignalInputData(IOBuffer &buffer);

	bool GetDisconnectAfterTransfer();
	void SetDisconnectAfterTransfer(bool disconnectAfterTransfer);
	bool IsChunkedContent();
	bool IsLastChunk();
	uint32_t GetContentLength();
	uint32_t GetSessionDecodedBytesCount();
	bool TransferCompleted();
	Variant GetHeaders();
	bool Flush();

	void SetOutboundHeader(string name, string value);
protected:
	virtual string GetOutputFirstLine() = 0;
	virtual bool ParseFirstLine(string &line, Variant &headers) = 0;
	virtual bool Authenticate() = 0;
private:
	string DumpState();
	bool ParseHeaders(IOBuffer &buffer);
	bool HandleChunkedContent(IOBuffer &buffer);
	bool HandleFixedLengthContent(IOBuffer &buffer);

};


#endif	/* _BASEHTTPPROTOCOL_H */
#endif /* HAS_PROTOCOL_HTTP */

