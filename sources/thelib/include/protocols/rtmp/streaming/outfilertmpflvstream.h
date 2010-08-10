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
#ifndef _OUTFILERTMPFLVSTREAM_H
#define	_OUTFILERTMPFLVSTREAM_H

#include "streaming/baseoutfilestream.h"

class DLLEXP OutFileRTMPFLVStream
: public BaseOutFileStream {
private:
	File _file;
	double _timeBase;
	IOBuffer _audioBuffer;
	IOBuffer _videoBuffer;
	uint32_t _prevTagSize;
public:
	OutFileRTMPFLVStream(BaseProtocol *pProtocol, StreamsManager *pStreamsManager,
			string name);
	virtual ~OutFileRTMPFLVStream();

	virtual bool SignalPlay(double &absoluteTimestamp, double &length);
	virtual bool SignalPause();
	virtual bool SignalResume();
	virtual bool SignalSeek(double &absoluteTimestamp);
	virtual bool SignalStop();
	virtual bool FeedData(uint8_t *pData, uint32_t dataLength,
			uint32_t processedLength, uint32_t totalLength,
			double absoluteTimestamp, bool isAudio);
	virtual bool IsCompatibleWithType(uint64_t type);
	virtual void SignalAttachedToInStream();
	virtual void SignalDetachedFromInStream();
	virtual void SignalStreamCompleted();
};


#endif	/* _OUTFILERTMPFLVSTREAM_H */

#endif /* HAS_PROTOCOL_RTMP */

