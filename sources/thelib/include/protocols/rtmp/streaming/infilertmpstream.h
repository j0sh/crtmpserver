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
#ifndef _INFILERTMPSTREAM_H
#define	_INFILERTMPSTREAM_H

#include "streaming/baseinfilestream.h"
#include "protocols/rtmp/header.h"


class BaseRTMPProtocol;
class StreamsManager;

class DLLEXP InFileRTMPStream
: public BaseInFileStream {
protected:
	Variant _completeMetadata;
	uint32_t _chunkSize;
public:
	InFileRTMPStream(BaseProtocol *pProtocol, StreamsManager *pStreamsManager,
			uint64_t type, string name);
	virtual ~InFileRTMPStream();

	virtual bool FeedData(uint8_t *pData, uint32_t dataLength,
			uint32_t processedLength, uint32_t totalLength,
			double absoluteTimestamp, bool isAudio);

	virtual bool IsCompatibleWithType(uint64_t type);

	uint32_t GetChunkSize();

	static InFileRTMPStream *GetInstance(BaseRTMPProtocol *pRTMPProtocol,
			StreamsManager *pStreamsManager, Variant &metadata);

	void SetCompleteMetadata(Variant &completeMetadata);
	Variant GetCompleteMetadata();
	virtual void SignalOutStreamAttached(BaseOutStream *pOutStream);
	virtual void SignalOutStreamDetached(BaseOutStream *pOutStream);

};


#endif	/* _INFILERTMPSTREAM_H */

#endif /* HAS_PROTOCOL_RTMP */

