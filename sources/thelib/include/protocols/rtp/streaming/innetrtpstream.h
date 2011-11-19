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


#ifdef HAS_PROTOCOL_RTP
#ifndef _INNETRTPSTREAM_H
#define	_INNETRTPSTREAM_H

#include "streaming/baseinnetstream.h"
#include "protocols/rtp/rtpheader.h"

#define RTCP_PRESENCE_UNKNOWN 0
#define RTCP_PRESENCE_AVAILABLE 1
#define RTCP_PRESENCE_ABSENT 2

class DLLEXP InNetRTPStream
: public BaseInNetStream {
private:
	StreamCapabilities _capabilities;

	bool _hasAudio;
	uint16_t _audioSequence;
	uint64_t _audioPacketsCount;
	uint64_t _audioDroppedPacketsCount;
	uint64_t _audioBytesCount;
	double _audioNTP;
	double _audioRTP;
	double _audioLastTs;
	uint32_t _audioLastRTP;
	uint32_t _audioRTPRollCount;
	double _audioFirstTimestamp;
	uint32_t _lastAudioRTCPRTP;
	uint32_t _audioRTCPRTPRollCount;

	bool _hasVideo;
	IOBuffer _currentNalu;
	uint16_t _videoSequence;
	uint64_t _videoPacketsCount;
	uint64_t _videoDroppedPacketsCount;
	uint64_t _videoBytesCount;
	double _videoNTP;
	double _videoRTP;
	double _videoLastTs;
	uint32_t _videoLastRTP;
	uint32_t _videoRTPRollCount;
	double _videoFirstTimestamp;
	uint32_t _lastVideoRTCPRTP;
	uint32_t _videoRTCPRTPRollCount;

	uint8_t _rtcpPresence;
	uint8_t _rtcpDetectionInterval;
	time_t _rtcpDetectionStart;

	bool _avCodecsSent;
public:
	InNetRTPStream(BaseProtocol *pProtocol, StreamsManager *pStreamsManager,
			string name, string SPS, string PPS, string AAC,
			uint32_t bandwidthHint, uint8_t rtcpDetectionInterval);
	virtual ~InNetRTPStream();

	virtual StreamCapabilities * GetCapabilities();
	virtual void ReadyForSend();
	virtual bool IsCompatibleWithType(uint64_t type);
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
	virtual bool FeedVideoData(uint8_t *pData, uint32_t dataLength,
			RTPHeader &rtpHeader);
	virtual bool FeedAudioData(uint8_t *pData, uint32_t dataLength,
			RTPHeader &rtpHeader);
	virtual void GetStats(Variant &info, uint32_t namespaceId = 0);

	void ReportSR(uint64_t ntpMicroseconds, uint32_t rtpTimestamp, bool isAudio);
private:
	void FeedVideoCodecSetup(BaseOutStream *pOutStream);
	void FeedAudioCodecSetup(BaseOutStream *pOutStream);
	uint64_t ComputeRTP(uint32_t &currentRtp, uint32_t &lastRtp,
			uint32_t &rtpRollCount);
};

#endif	/* _INNETRTPSTREAM_H */
#endif /* HAS_PROTOCOL_RTP */
