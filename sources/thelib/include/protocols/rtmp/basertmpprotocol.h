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
#ifndef _BASERTMPPROTOCOL_H
#define	_BASERTMPPROTOCOL_H

#include "protocols/baseprotocol.h"
#include "protocols/rtmp/channel.h"
#include "protocols/rtmp/rtmpprotocolserializer.h"
#include "streaming/rtmpstream.h"

#define RECEIVED_BYTES_COUNT_REPORT_CHUNK 131072
#define MAX_CHANNELS_COUNT (64+255)
#define MAX_STREAMS_COUNT 256

#define MIN_AV_CHANNLES 20
#define MAX_AV_CHANNLES MAX_CHANNELS_COUNT

typedef enum _RTMPState {
	RTMP_STATE_NOT_INITIALIZED,
	RTMP_STATE_CLIENT_REQUEST_RECEIVED,
	RTMP_STATE_CLIENT_REQUEST_SENT,
	RTMP_STATE_SERVER_RESPONSE_SENT,
	RTMP_STATE_DONE
} RTMPState;

class BaseStream;
class BaseOutStream;
class BaseOutNetRTMPStream;
class InFileRTMPStream;
class InNetRTMPStream;
class BaseRTMPAppProtocolHandler;
#ifdef ENFORCE_RTMP_OUTPUT_CHECKS
class MonitorRTMPProtocol;
#endif  /* ENFORCE_RTMP_OUTPUT_CHECKS */

class DLLEXP BaseRTMPProtocol
: public BaseProtocol {
protected:
	static uint8_t genuineFMSKey[];
	static uint8_t genuineFPKey[];
	bool _handshakeCompleted;
	RTMPState _rtmpState;
	IOBuffer _outputBuffer;
#ifdef ENFORCE_RTMP_OUTPUT_CHECKS
	IOBuffer _intermediateBuffer;
	MonitorRTMPProtocol *_pMonitor;
#endif /* ENFORCE_RTMP_OUTPUT_CHECKS */
	uint64_t _nextReceivedBytesCountReport;
	uint32_t _winAckSize;
	Channel _channels[MAX_CHANNELS_COUNT];
	int32_t _selectedChannel;
	uint32_t _inboundChunkSize;
	uint32_t _outboundChunkSize;
	BaseRTMPAppProtocolHandler *_pProtocolHandler;
	RTMPProtocolSerializer _rtmpProtocolSerializer;
	BaseStream *_streams[MAX_STREAMS_COUNT];
	vector<uint32_t> _channelsPool;
	LinkedListNode<BaseOutNetRTMPStream *> *_pSignaledRTMPOutNetStream;
	map<InFileRTMPStream *, InFileRTMPStream *> _inFileStreams;
	uint64_t _rxInvokes;
	uint64_t _txInvokes;
public:
	BaseRTMPProtocol(uint64_t protocolType);
	virtual ~BaseRTMPProtocol();

	virtual bool Initialize(Variant &parameters);
	virtual bool AllowFarProtocol(uint64_t type);
	virtual bool AllowNearProtocol(uint64_t type);
	virtual IOBuffer * GetOutputBuffer();
	virtual bool SignalInputData(int32_t recvAmount);
	virtual bool SignalInputData(IOBuffer &buffer);
	virtual bool TimePeriodElapsed();
	virtual void ReadyForSend();
	virtual void SetApplication(BaseClientApplication *pApplication);

	virtual void GetStats(Variant &info, uint32_t namespaceId = 0);

	bool ResetChannel(uint32_t channelId);

	bool SendMessage(Variant &message);
	bool SendRawData(Header &header, Channel &channel, uint8_t *pData, uint32_t length);
	bool SendRawData(uint8_t *pData, uint32_t length);

	void SetWinAckSize(uint32_t winAckSize);

	uint32_t GetOutboundChunkSize();
	bool SetInboundChunkSize(uint32_t chunkSize);
	void TrySetOutboundChunkSize(uint32_t chunkSize);

	BaseStream * GetRTMPStream(uint32_t rtmpStreamId);
	bool CloseStream(uint32_t streamId, bool createNeutralStream);
	RTMPStream * CreateNeutralStream(uint32_t &streamId);
	InNetRTMPStream * CreateINS(uint32_t channelId, uint32_t streamId, string streamName);
	BaseOutNetRTMPStream * CreateONS(uint32_t streamId, string streamName,
			uint64_t inStreamType);
	void SignalONS(BaseOutNetRTMPStream *pONS);
	InFileRTMPStream * CreateIFS(Variant &metadata);
	void RemoveIFS(InFileRTMPStream *pIFS);

	Channel *ReserveChannel();
	void ReleaseChannel(Channel *pChannel);
	virtual bool EnqueueForTimeEvent(uint32_t seconds);
protected:
	virtual bool PerformHandshake(IOBuffer &buffer) = 0;
	uint32_t GetDHOffset(uint8_t *pBuffer, uint8_t schemeNumber);
	uint32_t GetDigestOffset(uint8_t *pBuffer, uint8_t schemeNumber);
private:
	uint32_t GetDHOffset0(uint8_t *pBuffer);
	uint32_t GetDHOffset1(uint8_t *pBuffer);
	uint32_t GetDigestOffset0(uint8_t *pBuffer);
	uint32_t GetDigestOffset1(uint8_t *pBuffer);
	bool ProcessBytes(IOBuffer &buffer);

};


#endif	/* _BASERTMPPROTOCOL_H */

#endif /* HAS_PROTOCOL_RTMP */

