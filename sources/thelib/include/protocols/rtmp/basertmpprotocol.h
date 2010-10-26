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
#define MAX_CHANNELS_COUNT (64+256+256)
#define MAX_STREAMS_COUNT 256

#define MIN_AV_CHANNLES 20
#define MAX_AV_CHANNLES 60

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

#define AS_AUTHENTICATING 0
#define AS_AUTHENTICATED 1
#define AS_NO_NEED_FOR_AUTHENTICATION 2
#define AS_FAILED 3

class DLLEXP BaseRTMPProtocol
: public BaseProtocol {
protected:
	static uint8_t genuineFMSKey[];
	static uint8_t genuineFPKey[];
	bool _handshakeCompleted;
	RTMPState _rtmpState;
	IOBuffer _outputBuffer;
	uint32_t _nextReceivedBytesCountReport;
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
	uint8_t _authStage;
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

	bool SendMessage(Variant &message);
	bool SendRawData(Header &header, Channel &channel, uint8_t *pData, uint32_t length);
	bool SendRawData(uint8_t *pData, uint32_t length);

	void SetWinAckSize(uint32_t winAckSize);
	uint8_t GetAuthStage();
	void SetAuthStage(uint8_t authStage);

	uint32_t GetOutboundChunkSize();
	bool SetInboundChunkSize(uint32_t chunkSize);
	void TrySetOutboundChunkSize(uint32_t chunkSize);

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
	void HMACsha256(const void *pData, uint32_t dataLength, const void *pKey,
			uint32_t keyLength, void *pResult);
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

