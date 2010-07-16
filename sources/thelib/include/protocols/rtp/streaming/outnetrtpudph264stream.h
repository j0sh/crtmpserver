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


#ifndef _OUTNETRTPUDPH264STREAM_H
#define	_OUTNETRTPUDPH264STREAM_H

#include "protocols/rtp/streaming/baseoutnetrtpudpstream.h"

class OutNetRTPUDPH264Stream
: public BaseOutNetRTPUDPStream {
private:
	IOBuffer _videoBuffer;
#ifdef RTP_NORMAL_CHECKSUM
	IOBuffer _videoBufferCopy;
	string _orig;
	string _copy;
#endif
	uint16_t _videoCounter;
	msghdr _videoData;
	uint8_t *_pSPS;
	uint32_t _SPSLen;
	uint8_t *_pPPS;
	uint32_t _PPSLen;

	uint16_t _audioCounter;
	uint8_t _audioPacketsCount;
	IOBuffer _audioBuffer;
	msghdr _audioData;
public:
	OutNetRTPUDPH264Stream(BaseProtocol *pProtocol,
			StreamsManager *pStreamsManager, string name);
	virtual ~OutNetRTPUDPH264Stream();

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
	bool FeedDataAudioMPEG4Generic(uint8_t *pData, uint32_t dataLength,
			uint32_t processedLength, uint32_t totalLength,
			double absoluteTimestamp);
};


#endif	/* _OUTNETRTPUDPH264STREAM_H */

