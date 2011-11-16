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
#ifndef _OUTNETRTPUDPH264STREAM_H
#define	_OUTNETRTPUDPH264STREAM_H

#include "protocols/rtp/streaming/baseoutnetrtpudpstream.h"

class DLLEXP OutNetRTPUDPH264Stream
: public BaseOutNetRTPUDPStream {
private:
	bool _forceTcp;
	IOBuffer _videoBuffer;
	MSGHDR _videoData;
	uint8_t *_pSPS;
	uint32_t _SPSLen;
	uint8_t *_pPPS;
	uint32_t _PPSLen;

	IOBuffer _audioBuffer;
	MSGHDR _audioData;

	uint64_t _audioPacketsCount;
	uint64_t _audioDroppedPacketsCount;
	uint64_t _audioBytesCount;
	uint64_t _videoPacketsCount;
	uint64_t _videoDroppedPacketsCount;
	uint64_t _videoBytesCount;
	uint32_t _maxRTPPacketSize;
public:
	OutNetRTPUDPH264Stream(BaseProtocol *pProtocol,
			StreamsManager *pStreamsManager, string name, bool forceTcp);
	virtual ~OutNetRTPUDPH264Stream();

	virtual void GetStats(Variant &info, uint32_t namespaceId = 0);

	virtual bool FeedDataVideo(uint8_t *pData, uint32_t dataLength,
			uint32_t processedLength, uint32_t totalLength,
			double absoluteTimestamp, bool isAudio);
	virtual bool FeedDataAudio(uint8_t *pData, uint32_t dataLength,
			uint32_t processedLength, uint32_t totalLength,
			double absoluteTimestamp, bool isAudio);
private:
	virtual void SignalAttachedToInStream();
	bool FeedDataVideoFUA(uint8_t *pData, uint32_t dataLength,
			uint32_t processedLength, uint32_t totalLength,
			double absoluteTimestamp);
	bool FeedDataAudioMPEG4Generic_aggregate(uint8_t *pData, uint32_t dataLength,
			uint32_t processedLength, uint32_t totalLength,
			double absoluteTimestamp);
	bool FeedDataAudioMPEG4Generic_one_by_one(uint8_t *pData, uint32_t dataLength,
			uint32_t processedLength, uint32_t totalLength,
			double absoluteTimestamp);
};


#endif	/* _OUTNETRTPUDPH264STREAM_H */
#endif /* HAS_PROTOCOL_RTP */

