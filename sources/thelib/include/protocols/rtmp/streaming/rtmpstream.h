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
#ifndef _RTMPSTREAM_H
#define	_RTMPSTREAM_H

#include "streaming/basestream.h"

class DLLEXP RTMPStream
: public BaseStream {
private:
	uint32_t _rtmpStreamId;
public:
	RTMPStream(BaseProtocol *pProtocol, StreamsManager *pStreamsManager,
			uint32_t rtmpStreamId);
	virtual ~RTMPStream();
	virtual StreamCapabilities * GetCapabilities();


	virtual bool Play(double absoluteTimestamp, double length);
	virtual bool Pause();
	virtual bool Resume();
	virtual bool Seek(double absoluteTimestamp);
	virtual bool Stop();
	virtual bool SignalPlay(double &absoluteTimestamp, double &length);
	virtual bool SignalPause();
	virtual bool SignalResume();
	virtual bool SignalSeek(double &absoluteTimestamp);
	virtual bool SignalStop();

	virtual bool FeedData(uint8_t *pData, uint32_t dataLength,
			uint32_t processedLength, uint32_t totalLength,
			double absoluteTimestamp, bool isAudio);
	virtual void ReadyForSend();
	virtual bool IsCompatibleWithType(uint64_t type);
};


#endif	/* _RTMPSTREAM_H */

#endif /* HAS_PROTOCOL_RTMP */

