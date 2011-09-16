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


#ifdef HAS_PROTOCOL_TS
#ifndef _INNETTSSTREAM_H
#define	_INNETTSSTREAM_H

#include "streaming/baseinnetstream.h"
#include "streaming/streamcapabilities.h"

#define COMPUTE_DTS_TIME

struct _PIDDescriptor;

class DLLEXP InNetTSStream
: public BaseInNetStream {
private:
	//audio section
	_PIDDescriptor *_pAudioPidDescriptor;
	int8_t _currentAudioSequenceNumber;
	uint64_t _lastRawPtsAudio;
	uint32_t _audioRollOverCount;
	double _ptsTimeAudio;
#ifdef COMPUTE_DTS_TIME
	double _dtsTimeAudio;
#endif
	double _deltaTimeAudio;
	IOBuffer _audioBuffer;
	double _lastGotAudioTimestamp;
	double _lastSentAudioTimestamp;
	uint64_t _audioPacketsCount;
	uint64_t _statsAudioPacketsCount;
	uint64_t _audioBytesCount;
	uint64_t _audioDroppedPacketsCount;
	uint64_t _audioDroppedBytesCount;


	//video section
	_PIDDescriptor *_pVideoPidDescriptor;
	int8_t _currentVideoSequenceNumber;
	uint64_t _lastRawPtsVideo;
	uint32_t _videoRollOverCount;
	double _ptsTimeVideo;
#ifdef COMPUTE_DTS_TIME
	double _dtsTimeVideo;
#endif
	double _deltaTimeVideo;
	uint64_t _videoPacketsCount;
	uint64_t _videoBytesCount;
	uint64_t _videoDroppedPacketsCount;
	uint64_t _videoDroppedBytesCount;
	IOBuffer _currentNal;

	double _feedTime;

	uint32_t _cursor;
	StreamCapabilities _streamCapabilities;
	bool _firstNAL;

	IOBuffer _SPS;
	IOBuffer _PPS;
public:
	InNetTSStream(BaseProtocol *pProtocol, StreamsManager *pStreamsManager,
			string name, uint32_t bandwidthHint);
	virtual ~InNetTSStream();
	virtual StreamCapabilities * GetCapabilities();

	void SetAudioVideoPidDescriptors(_PIDDescriptor *pAudioPidDescriptor,
			_PIDDescriptor *pVideoPidDescriptor);

	double GetFeedTime();

	bool FeedData(uint8_t *pData, uint32_t length, bool packetStart,
			bool isAudio, int8_t sequenceNumber);
	virtual bool FeedData(uint8_t *pData, uint32_t dataLength,
			uint32_t processedLength, uint32_t totalLength,
			double absoluteTimestamp, bool isAudio);
	virtual void ReadyForSend();
	virtual bool IsCompatibleWithType(uint64_t type);
	virtual void SignalOutStreamAttached(BaseOutStream *pOutStream);
	virtual void SignalOutStreamDetached(BaseOutStream *pOutStream);
	virtual bool SignalPlay(double &absoluteTimestamp, double &length);
	virtual bool SignalPause();
	virtual bool SignalResume();
	virtual bool SignalSeek(double &absoluteTimestamp);
	virtual bool SignalStop();
	virtual void GetStats(Variant &info, uint32_t namespaceId = 0);
private:
	bool HandleAudioData(uint8_t *pRawBuffer, uint32_t rawBufferLength,
			double timestamp, bool packetStart);
	bool HandleVideoData(uint8_t *pRawBuffer, uint32_t rawBufferLength,
			double timestamp, bool packetStart);
	bool ProcessNal(double timestamp);
	void InitializeVideoCapabilities(uint8_t *pData, uint32_t length);
	void InitializeAudioCapabilities(uint8_t *pData, uint32_t length);
};


#endif	/* _INNETTSSTREAM_H */
#endif	/* HAS_PROTOCOL_TS */

