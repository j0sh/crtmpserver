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
#ifndef _INNETRTMPSTREAM_H
#define	_INNETRTMPSTREAM_H

#include "streaming/baseinnetstream.h"
#include "streaming/streamcapabilities.h"

class BaseRTMPProtocol;
class BaseOutStream;
class BaseOutFileStream;

class DLLEXP InNetRTMPStream
: public BaseInNetStream {
private:
	uint32_t _rtmpStreamId;
	uint32_t _chunkSize;
	uint32_t _channelId;
	string _clientId;
	IOBuffer _videoCodecInit;
	double _lastVideoTime;
	IOBuffer _audioCodecInit;
	double _lastAudioTime;
	Variant _lastStreamMessage;
	BaseOutStream *_pOutFileRTMPFLVStream;
	StreamCapabilities _streamCapabilities;

	uint64_t _audioPacketsCount;
	uint64_t _audioDroppedPacketsCount;
	uint64_t _audioBytesCount;
	uint64_t _audioDroppedBytesCount;
	uint64_t _videoPacketsCount;
	uint64_t _videoDroppedPacketsCount;
	uint64_t _videoBytesCount;
	uint64_t _videoDroppedBytesCount;
public:
	InNetRTMPStream(BaseProtocol *pProtocol, StreamsManager *pStreamsManager,
			string name, uint32_t rtmpStreamId, uint32_t chunkSize,
			uint32_t channelId);
	virtual ~InNetRTMPStream();
	virtual StreamCapabilities * GetCapabilities();

	virtual void ReadyForSend();
	virtual bool IsCompatibleWithType(uint64_t type);
	virtual void GetStats(Variant &info, uint32_t namespaceId = 0);

	uint32_t GetRTMPStreamId();
	uint32_t GetChunkSize();
	void SetChunkSize(uint32_t chunkSize);

	bool SendStreamMessage(Variant &message, bool persistent = true);
	virtual bool SendStreamMessage(string functionName, Variant &parameters,
			bool persistent = true);
	bool SendOnStatusStreamPublished();
	bool Record(BaseOutFileStream* pOutFileStream);

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
private:
	BaseRTMPProtocol *GetRTMPProtocol();
	bool InitializeAudioCapabilities(uint8_t *pData, uint32_t length);
	bool InitializeVideoCapabilities(uint8_t *pData, uint32_t length);
};


#endif	/* _INNETRTMPSTREAM_H */

#endif /* HAS_PROTOCOL_RTMP */

