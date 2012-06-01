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
#ifndef _OUTNETRTMP4TSSTREAM_H
#define	_OUTNETRTMP4TSSTREAM_H

#include "protocols/rtmp/streaming/baseoutnetrtmpstream.h"

class DLLEXP OutNetRTMP4TSStream
: public BaseOutNetRTMPStream {
private:
	bool _audioCodecSent;
	bool _audioIsG711;
	bool _videoCodecSent;
	IOBuffer _videoBuffer;
	bool _inboundStreamIsRTP;
	double _lastVideoTimestamp;
	bool _isKeyFrame;
public:
	OutNetRTMP4TSStream(BaseRTMPProtocol *pProtocol, StreamsManager *pStreamsManager,
			string name, uint32_t rtmpStreamId, uint32_t chunkSize);
	virtual ~OutNetRTMP4TSStream();

	virtual bool IsCompatibleWithType(uint64_t type);

	virtual bool FeedData(uint8_t *pData, uint32_t dataLength,
			uint32_t processedLength, uint32_t totalLength,
			double absoluteTimestamp, bool isAudio);
private:
	bool FeedAudioData(uint8_t *pData, uint32_t dataLength, double absoluteTimestamp);
	bool FeedVideoData(uint8_t *pData, uint32_t dataLength, double absoluteTimestamp);
	bool SendVideoCodec(double absoluteTimestamp);
	bool SendAudioCodec(double absoluteTimestamp);
};


#endif	/* _OUTNETRTMP4TSSTREAM_H */

#endif /* HAS_PROTOCOL_RTMP */

