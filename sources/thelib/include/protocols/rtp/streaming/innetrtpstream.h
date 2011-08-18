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
	double _lastAudioTs;

	bool _hasVideo;
	IOBuffer _currentNalu;
	uint16_t _videoSequence;
	uint64_t _videoPacketsCount;
	uint64_t _videoDroppedPacketsCount;
	uint64_t _videoBytesCount;
	double _videoNTP;
	double _videoRTP;
	double _lastVideoTs;
public:
	InNetRTPStream(BaseProtocol *pProtocol, StreamsManager *pStreamsManager,
			string name, string SPS, string PPS, string AAC, uint32_t bandwidthHint);
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
	virtual void GetStats(Variant &info);

	void ReportSR(uint64_t ntpMicroseconds, uint32_t rtpTimestamp, bool isAudio);
private:
	void FeedVideoCodecSetup(BaseOutStream *pOutStream);
	void FeedAudioCodecSetup(BaseOutStream *pOutStream);
};

#endif	/* _INNETRTPSTREAM_H */
#endif /* HAS_PROTOCOL_RTP */
