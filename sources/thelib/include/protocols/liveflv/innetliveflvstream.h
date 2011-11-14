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


#ifdef HAS_PROTOCOL_LIVEFLV
#ifndef _INNETLIVEFLVSTREAM_H
#define	_INNETLIVEFLVSTREAM_H

#include "streaming/baseinnetstream.h"

class InNetLiveFLVStream
: public BaseInNetStream {
private:
	IOBuffer _videoCodecInit;
	double _lastVideoTime;
	uint64_t _videoBytesCount;
	uint64_t _videoPacketsCount;


	IOBuffer _audioCodecInit;
	double _lastAudioTime;
	uint64_t _audioBytesCount;
	uint64_t _audioPacketsCount;

	Variant _lastStreamMessage;
	StreamCapabilities _streamCapabilities;
public:
	InNetLiveFLVStream(BaseProtocol *pProtocol, StreamsManager *pStreamsManager,
			string name);
	virtual ~InNetLiveFLVStream();

	virtual StreamCapabilities * GetCapabilities();

	virtual bool FeedData(uint8_t *pData, uint32_t dataLength,
			uint32_t processedLength, uint32_t totalLength,
			double absoluteTimestamp, bool isAudio);
	virtual void ReadyForSend();
	virtual bool IsCompatibleWithType(uint64_t type);
	virtual void GetStats(Variant &info, uint32_t namespaceId);
	virtual void SignalOutStreamAttached(BaseOutStream *pOutStream);
	virtual void SignalOutStreamDetached(BaseOutStream *pOutStream);
	virtual bool SignalPlay(double &absoluteTimestamp, double &length);
	virtual bool SignalPause();
	virtual bool SignalResume();
	virtual bool SignalSeek(double &absoluteTimestamp);
	virtual bool SignalStop();
	bool SendStreamMessage(Variant &completeMessage, bool persistent);
	bool SendStreamMessage(string functionName, Variant &parameters,
			bool persistent);
private:
	bool InitializeAudioCapabilities(uint8_t *pData, uint32_t length);
	bool InitializeVideoCapabilities(uint8_t *pData, uint32_t length);
};


#endif	/* _INNETLIVEFLVSTREAM_H */
#endif /* HAS_PROTOCOL_LIVEFLV */

