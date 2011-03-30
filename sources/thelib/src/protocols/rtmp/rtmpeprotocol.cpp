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


#ifdef HAS_PROTOCOL_RTMP
#include "protocols/rtmp/rtmpeprotocol.h"

RTMPEProtocol::RTMPEProtocol(RC4_KEY *pKeyIn, RC4_KEY *pKeyOut, uint32_t skipBytes)
: BaseProtocol(PT_RTMPE) {
	_pKeyIn = pKeyIn;
	_pKeyOut = pKeyOut;
	_skipBytes = skipBytes;
}

RTMPEProtocol::~RTMPEProtocol() {
}

bool RTMPEProtocol::AllowFarProtocol(uint64_t type) {
	return (type == PT_INBOUND_HTTP_FOR_RTMP)
			|| (type == PT_TCP);
}

bool RTMPEProtocol::AllowNearProtocol(uint64_t type) {
	return (type == PT_INBOUND_RTMP)
			|| (type == PT_OUTBOUND_RTMP);
}

IOBuffer * RTMPEProtocol::GetInputBuffer() {
	return &_inputBuffer;
}

IOBuffer * RTMPEProtocol::GetOutputBuffer() {
	if (GETAVAILABLEBYTESCOUNT(_outputBuffer) > 0)
		return &_outputBuffer;
	return NULL;
}

bool RTMPEProtocol::SignalInputData(int32_t recvAmount) {
	ASSERT("OPERATION NOT SUPPORTED");
	return false;
}

bool RTMPEProtocol::SignalInputData(IOBuffer &buffer) {
	RC4(_pKeyIn, GETAVAILABLEBYTESCOUNT(buffer),
			GETIBPOINTER(buffer),
			GETIBPOINTER(buffer));

	_inputBuffer.ReadFromBuffer(GETIBPOINTER(buffer),
			GETAVAILABLEBYTESCOUNT(buffer));
	buffer.IgnoreAll();

	if (_pNearProtocol != NULL)
		return _pNearProtocol->SignalInputData(_inputBuffer);
	return true;
}

bool RTMPEProtocol::EnqueueForOutbound() {

	IOBuffer *pOutputBuffer = _pNearProtocol->GetOutputBuffer();

	if (pOutputBuffer == NULL)
		return true;

	RC4(_pKeyOut, GETAVAILABLEBYTESCOUNT(*pOutputBuffer) - _skipBytes,
			GETIBPOINTER(*pOutputBuffer) + _skipBytes,
			GETIBPOINTER(*pOutputBuffer) + _skipBytes);
	_skipBytes = 0;

	_outputBuffer.ReadFromInputBuffer(pOutputBuffer, 0, GETAVAILABLEBYTESCOUNT(*pOutputBuffer));
	pOutputBuffer->Ignore(GETAVAILABLEBYTESCOUNT(*pOutputBuffer));

	if (_pFarProtocol != NULL)
		return _pFarProtocol->EnqueueForOutbound();

	return true;
}

#endif /* HAS_PROTOCOL_RTMP */

