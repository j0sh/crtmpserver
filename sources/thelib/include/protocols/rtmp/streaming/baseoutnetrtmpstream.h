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
#ifndef _BASEOUTNETRTMPSTREAM_H
#define	_BASEOUTNETRTMPSTREAM_H

#include "streaming/baseoutnetstream.h"
#include "protocols/rtmp/header.h"
#include "protocols/rtmp/channel.h"

class BaseRTMPProtocol;

class DLLEXP BaseOutNetRTMPStream
: public BaseOutNetStream {
private:
	uint32_t _rtmpStreamId;
	uint32_t _chunkSize;
	BaseRTMPProtocol *_pRTMPProtocol;

	double _deltaVideoTime;
	double _deltaAudioTime;
	double *_pDeltaVideoTime;
	double *_pDeltaAudioTime;
	double _seekTime;

	uint32_t _isFirstVideoFrame;
	Header _videoHeader;
	IOBuffer _videoBucket;

	uint32_t _isFirstAudioFrame;
	Header _audioHeader;
	IOBuffer _audioBucket;

	Channel *_pChannelAudio;
	Channel *_pChannelVideo;
	Channel *_pChannelCommands;
	uint32_t _feederChunkSize;
	bool _canDropFrames;
	bool _audioCurrentFrameDropped;
	bool _videoCurrentFrameDropped;
	uint32_t _maxBufferSize;
	uint64_t _attachedStreamType;
	Variant _completeMetadata;
	string _clientId;
	bool _paused;

	bool _sendOnStatusPlayMessages;

	uint64_t _audioPacketsCount;
	uint64_t _audioDroppedPacketsCount;
	uint64_t _audioBytesCount;
	uint64_t _audioDroppedBytesCount;
	uint64_t _videoPacketsCount;
	uint64_t _videoDroppedPacketsCount;
	uint64_t _videoBytesCount;
	uint64_t _videoDroppedBytesCount;
protected:
	BaseOutNetRTMPStream(BaseProtocol *pProtocol, StreamsManager *pStreamsManager,
			uint64_t type, string name, uint32_t rtmpStreamId, uint32_t chunkSize);
public:
	static BaseOutNetRTMPStream *GetInstance(BaseProtocol *pProtocol,
			StreamsManager *pStreamsManager,
			string name, uint32_t rtmpStreamId,
			uint32_t chunkSize,
			uint64_t inStreamType);
	virtual ~BaseOutNetRTMPStream();

	uint32_t GetRTMPStreamId();
	uint32_t GetCommandsChannelId();
	void SetChunkSize(uint32_t chunkSize);
	uint32_t GetChunkSize();
	void SetFeederChunkSize(uint32_t feederChunkSize);
	bool CanDropFrames();
	void CanDropFrames(bool canDropFrames);
	void SetSendOnStatusPlayMessages(bool value);
	virtual void GetStats(Variant &info, uint32_t namespaceId = 0);

	virtual bool FeedData(uint8_t *pData, uint32_t dataLength,
			uint32_t processedLength, uint32_t totalLength,
			double absoluteTimestamp, bool isAudio);

	virtual bool SendStreamMessage(Variant &message);
	virtual void SignalAttachedToInStream();
	virtual void SignalDetachedFromInStream();

	virtual bool SignalPlay(double &absoluteTimestamp, double &length);
	virtual bool SignalPause();
	virtual bool SignalResume();
	virtual bool SignalSeek(double &absoluteTimestamp);
	virtual bool SignalStop();

	virtual void SignalStreamCompleted();
private:
	bool ChunkAndSend(uint8_t *pData, uint32_t length, IOBuffer &bucket,
			Header &header, Channel &channel);
	bool AllowExecution(uint32_t totalProcessed, uint32_t dataLength, bool isAudio);
	void InternalReset();
	void FixTimeBase();
};


#endif	/* _BASEOUTNETRTMPSTREAM_H */

#endif /* HAS_PROTOCOL_RTMP */

