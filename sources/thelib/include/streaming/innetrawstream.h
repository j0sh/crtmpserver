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

#ifndef _INNETRAWSTREAM_H
#define	_INNETRAWSTREAM_H

#include "streaming/baseinnetstream.h"
#include "streaming/streamcapabilities.h"

class InNetRawStream
: public BaseInNetStream {
private:
	uint64_t _bytesCount;
	uint64_t _packetsCount;
	StreamCapabilities _capabilities;
	File _witness;
public:
	InNetRawStream(BaseProtocol *pProtocol, StreamsManager *pStreamsManager,
			string name, uint64_t codecType);
	virtual ~InNetRawStream();

	virtual bool IsCompatibleWithType(uint64_t type);
	virtual StreamCapabilities * GetCapabilities();
	virtual void GetStats(Variant &info, uint32_t namespaceId = 0);
	virtual void ReadyForSend();
	virtual void SignalOutStreamAttached(BaseOutStream *pOutStream);
	virtual void SignalOutStreamDetached(BaseOutStream *pOutStream);

	virtual bool SignalPlay(double &absoluteTimestamp, double &length);
	virtual bool SignalPause();
	virtual bool SignalResume();
	virtual bool SignalSeek(double &absoluteTimestamp);
	virtual bool SignalStop();
	virtual bool FeedData(uint8_t *pData, uint32_t dataLength,
			uint32_t processedLength, uint32_t totalLength,
			double absoluteTimestamp, bool isAudio);
};


#endif	/* _INNETRAWSTREAM_H */

