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
#ifndef _BASEOUTNETRTPUDPSTREAM_H
#define	_BASEOUTNETRTPUDPSTREAM_H

#include "streaming/baseoutnetstream.h"

class OutboundConnectivity;

class DLLEXP BaseOutNetRTPUDPStream
: public BaseOutNetStream {
protected:
	uint32_t _audioSsrc;
	uint32_t _videoSsrc;
	OutboundConnectivity *_pConnectivity;
	uint16_t _videoCounter;
	uint16_t _audioCounter;
	bool _hasAudio;
	bool _hasVideo;
public:
	BaseOutNetRTPUDPStream(BaseProtocol *pProtocol,
			StreamsManager *pStreamsManager, string name);
	virtual ~BaseOutNetRTPUDPStream();

	OutboundConnectivity *GetConnectivity();
	void SetConnectivity(OutboundConnectivity *pConnectivity);
	void HasAudioVideo(bool hasAudio, bool hasVideo);

	uint32_t AudioSSRC();
	uint32_t VideoSSRC();
	uint16_t VideoCounter();
	uint16_t AudioCounter();

	virtual bool SignalPlay(double &absoluteTimestamp, double &length);
	virtual bool SignalPause();
	virtual bool SignalResume();
	virtual bool SignalSeek(double &absoluteTimestamp);
	virtual bool SignalStop();

	virtual bool IsCompatibleWithType(uint64_t type);
	virtual void SignalDetachedFromInStream();
	virtual void SignalStreamCompleted();

	virtual bool FeedData(uint8_t *pData, uint32_t dataLength,
			uint32_t processedLength, uint32_t totalLength,
			double absoluteTimestamp, bool isAudio);
protected:
	virtual bool FeedDataVideo(uint8_t *pData, uint32_t dataLength,
			uint32_t processedLength, uint32_t totalLength,
			double absoluteTimestamp, bool isAudio) = 0;
	virtual bool FeedDataAudio(uint8_t *pData, uint32_t dataLength,
			uint32_t processedLength, uint32_t totalLength,
			double absoluteTimestamp, bool isAudio) = 0;

};

#endif	/* _BASEOUTNETRTPUDPSTREAM_H */
#endif /* HAS_PROTOCOL_RTP */

