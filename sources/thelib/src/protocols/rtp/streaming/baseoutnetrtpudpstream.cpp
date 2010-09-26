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
#include "protocols/rtp/streaming/baseoutnetrtpudpstream.h"
#include "streaming/streamstypes.h"
#include "protocols/protocolmanager.h"
#include "protocols/baseprotocol.h"
#include "protocols/rtp/connectivity/outboundconnectivity.h"

BaseOutNetRTPUDPStream::BaseOutNetRTPUDPStream(BaseProtocol *pProtocol,
		StreamsManager *pStreamsManager, string name)
: BaseOutNetStream(NULL /*FIXME pProtocol*/, pStreamsManager, ST_OUT_NET_RTP, name) {
	_ssrc = 12;
	_pConnectivity = NULL;
	_videoCounter = rand();
	_audioCounter = rand();
}

BaseOutNetRTPUDPStream::~BaseOutNetRTPUDPStream() {
}

OutboundConnectivity *BaseOutNetRTPUDPStream::GetConnectivity() {
	return _pConnectivity;
}

void BaseOutNetRTPUDPStream::SetConnectivity(OutboundConnectivity *pConnectivity) {
	_pConnectivity = pConnectivity;
}

uint32_t BaseOutNetRTPUDPStream::SSRC() {
	return _ssrc;
}

uint16_t BaseOutNetRTPUDPStream::VideoCounter() {
	return _videoCounter;
}

uint16_t BaseOutNetRTPUDPStream::AudioCounter() {
	return _audioCounter;
}

bool BaseOutNetRTPUDPStream::SignalPlay(double &absoluteTimestamp, double &length) {
	NYIR;
}

bool BaseOutNetRTPUDPStream::SignalPause() {
	NYIR;
}

bool BaseOutNetRTPUDPStream::SignalResume() {
	NYIR;
}

bool BaseOutNetRTPUDPStream::SignalSeek(double &absoluteTimestamp) {
	NYIR;
}

bool BaseOutNetRTPUDPStream::SignalStop() {
	NYIR;
}

bool BaseOutNetRTPUDPStream::IsCompatibleWithType(uint64_t type) {
	return type == ST_IN_NET_RTMP
			|| type == ST_IN_NET_TS;
}

void BaseOutNetRTPUDPStream::SignalDetachedFromInStream() {
	_pConnectivity->SignalDetachedFromInStream();
}

void BaseOutNetRTPUDPStream::SignalStreamCompleted() {
	NYIA;
}

bool BaseOutNetRTPUDPStream::FeedData(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp, bool isAudio) {
	if (isAudio)
		return FeedDataAudio(pData, dataLength, processedLength, totalLength,
			absoluteTimestamp, isAudio);
	else
		return FeedDataVideo(pData, dataLength, processedLength, totalLength,
			absoluteTimestamp, isAudio);
}

#endif /* HAS_PROTOCOL_RTP */
