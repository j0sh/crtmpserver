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
#include "streaming/outnetrawstream.h"
#include "streaming/streamstypes.h"
#include "protocols/protocoltypes.h"
#include "protocols/rawhttpstream/inboundrawhttpstreamprotocol.h"

OutNetRawStream::OutNetRawStream(BaseProtocol *pProtocol,
		StreamsManager *pStreamsManager, string name)
: BaseOutNetStream(pProtocol, pStreamsManager, ST_OUT_NET_RAW, name) {
	if (pProtocol == NULL) {
		ASSERT("OutNetRawStream requires a protocol to host it");
	}
	if (_pProtocol->GetType() != PT_INBOUND_RAW_HTTP_STREAM) {
		ASSERT("OutNetRawStream can be hosted only inside %s protocol",
				STR(tagToString(PT_INBOUND_RAW_HTTP_STREAM)));
	}
	_bytesCount = 0;
	_packetsCount = 0;
}

OutNetRawStream::~OutNetRawStream() {
}

void OutNetRawStream::GetStats(Variant &info, uint32_t namespaceId) {
	BaseOutNetStream::GetStats(info, namespaceId);
	info["video"]["bytesCount"] = _bytesCount;
	info["video"]["packetsCount"] = _packetsCount;
	info["video"]["droppedPacketsCount"] = 0;
	info["audio"]["bytesCount"] = 0;
	info["audio"]["packetsCount"] = 0;
	info["audio"]["droppedPacketsCount"] = 0;
}

void OutNetRawStream::SignalAttachedToInStream() {

}

void OutNetRawStream::SignalDetachedFromInStream() {
	NYI;
}

void OutNetRawStream::SignalStreamCompleted() {
	NYI;
}

bool OutNetRawStream::SignalPlay(double &absoluteTimestamp, double &length) {
	NYIR;
}

bool OutNetRawStream::SignalPause() {
	NYIR;
}

bool OutNetRawStream::SignalResume() {
	NYIR;
}

bool OutNetRawStream::SignalSeek(double &absoluteTimestamp) {
	NYIR;
}

bool OutNetRawStream::SignalStop() {
	NYIR;
}

bool OutNetRawStream::FeedData(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp, bool isAudio) {
	_bytesCount += dataLength;
	_packetsCount++;
	if (_pProtocol != NULL) {
		return ((InboundRawHTTPStreamProtocol *) _pProtocol)->PutData(pData, dataLength);
	}
	return false;
}

bool OutNetRawStream::IsCompatibleWithType(uint64_t type) {
	//This stream is compatible with everything
	return true;
}
#endif /* HAS_PROTOCOL_RAWHTTPSTREAM */
