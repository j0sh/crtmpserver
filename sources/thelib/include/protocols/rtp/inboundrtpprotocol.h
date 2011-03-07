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
#ifndef _INBOUNDRTPPROTOCOL_H
#define	_INBOUNDRTPPROTOCOL_H

#include "protocols/baseprotocol.h"
#include "protocols/rtp/rtpheader.h"

class InNetRTPStream;
class InboundConnectivity;

//#define RTP_DETECT_ROLLOVER

class DLLEXP InboundRTPProtocol
: public BaseProtocol {
private:
	RTPHeader _rtpHeader;
	uint8_t _spsPpsPeriod;
	InNetRTPStream *_pInStream;
	InboundConnectivity *_pConnectivity;
	uint16_t _lastSeq;
	uint16_t _seqRollOver;
	bool _isAudio;
	uint32_t _packetsCount;
#ifdef RTP_DETECT_ROLLOVER
	uint64_t _lastTimestamp;
	uint64_t _timestampRollover;
#endif
public:
	InboundRTPProtocol();
	virtual ~InboundRTPProtocol();

	virtual bool Initialize(Variant &parameters);
	virtual bool AllowFarProtocol(uint64_t type);
	virtual bool AllowNearProtocol(uint64_t type);
	virtual bool SignalInputData(int32_t recvAmount);
	virtual bool SignalInputData(IOBuffer &buffer);
	virtual bool SignalInputData(IOBuffer &buffer, sockaddr_in *pPeerAddress);

	uint32_t GetSSRC();
	uint32_t GetExtendedSeq();

	void SetStream(InNetRTPStream *pInStream, bool isAudio);
	void SetInbboundConnectivity(InboundConnectivity *pConnectivity);
};


#endif	/* _INBOUNDRTPPROTOCOL_H */
#endif /* HAS_PROTOCOL_RTP */
