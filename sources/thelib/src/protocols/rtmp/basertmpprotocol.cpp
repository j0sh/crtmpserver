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
#include "application/clientapplicationmanager.h"
#include "netio/netio.h"
#include "protocols/protocolmanager.h"
#include "protocols/rtmp/basertmpprotocol.h"
#include "protocols/rtmp/basertmpappprotocolhandler.h"
#include "protocols/rtmp/messagefactories/messagefactories.h"
#include "protocols/rtmp/streaming/baseoutnetrtmpstream.h"
#include "streaming/streamstypes.h"
#include "protocols/rtmp/streaming/innetrtmpstream.h"
#include "protocols/rtmp/streaming/infilertmpstream.h"
#include "protocols/rtmp/streaming/rtmpstream.h"
#include "streaming/streamstypes.h"

#define MAX_RTMP_OUTPUT_BUFFER 1024*256

uint8_t BaseRTMPProtocol::genuineFMSKey[] = {
	0x47, 0x65, 0x6e, 0x75, 0x69, 0x6e, 0x65, 0x20,
	0x41, 0x64, 0x6f, 0x62, 0x65, 0x20, 0x46, 0x6c,
	0x61, 0x73, 0x68, 0x20, 0x4d, 0x65, 0x64, 0x69,
	0x61, 0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72,
	0x20, 0x30, 0x30, 0x31, // Genuine Adobe Flash Media Server 001
	0xf0, 0xee, 0xc2, 0x4a, 0x80, 0x68, 0xbe, 0xe8,
	0x2e, 0x00, 0xd0, 0xd1, 0x02, 0x9e, 0x7e, 0x57,
	0x6e, 0xec, 0x5d, 0x2d, 0x29, 0x80, 0x6f, 0xab,
	0x93, 0xb8, 0xe6, 0x36, 0xcf, 0xeb, 0x31, 0xae
}; // 68

uint8_t BaseRTMPProtocol::genuineFPKey[] = {
	0x47, 0x65, 0x6E, 0x75, 0x69, 0x6E, 0x65, 0x20,
	0x41, 0x64, 0x6F, 0x62, 0x65, 0x20, 0x46, 0x6C,
	0x61, 0x73, 0x68, 0x20, 0x50, 0x6C, 0x61, 0x79,
	0x65, 0x72, 0x20, 0x30, 0x30, 0x31, // Genuine Adobe Flash Player 001
	0xF0, 0xEE, 0xC2, 0x4A, 0x80, 0x68, 0xBE, 0xE8,
	0x2E, 0x00, 0xD0, 0xD1, 0x02, 0x9E, 0x7E, 0x57,
	0x6E, 0xEC, 0x5D, 0x2D, 0x29, 0x80, 0x6F, 0xAB,
	0x93, 0xB8, 0xE6, 0x36, 0xCF, 0xEB, 0x31, 0xAE
}; // 62

BaseRTMPProtocol::BaseRTMPProtocol(uint64_t protocolType)
: BaseProtocol(protocolType) {
	_handshakeCompleted = false;
	_rtmpState = RTMP_STATE_NOT_INITIALIZED;
	//TODO: Make use of winacksize which is in fact the value setted up for
	//the nex bytes sent report
	_winAckSize = RECEIVED_BYTES_COUNT_REPORT_CHUNK;
	_nextReceivedBytesCountReport = _winAckSize;
	for (uint32_t i = 0; i < MAX_CHANNELS_COUNT; i++) {
		memset(&_channels[i], 0, sizeof (Channel));
		_channels[i].id = i;
		_channels[i].lastOutStreamId = 0xffffffff;
	}
	_selectedChannel = -1;
	_inboundChunkSize = 128;
	_outboundChunkSize = 128;

	for (uint32_t i = 0; i < MAX_STREAMS_COUNT; i++) {
		_streams[i] = NULL;
	}

	for (uint32_t i = MIN_AV_CHANNLES; i < MAX_AV_CHANNLES; i++)
		ADD_VECTOR_END(_channelsPool, i);

	_pSignaledRTMPOutNetStream = NULL;
	_rxInvokes = 0;
	_txInvokes = 0;
}

BaseRTMPProtocol::~BaseRTMPProtocol() {
	for (uint32_t i = 0; i < MAX_STREAMS_COUNT; i++) {
		if (_streams[i] != NULL) {
			delete _streams[i];
			_streams[i] = NULL;
		}
	}
	LinkedListNode<BaseOutNetRTMPStream *> *pTemp = _pSignaledRTMPOutNetStream;
	while (pTemp != NULL) {
		pTemp = RemoveLinkedList<BaseOutNetRTMPStream *> (pTemp);
	}
	_pSignaledRTMPOutNetStream = NULL;
	while (_inFileStreams.size() > 0) {
		InFileRTMPStream *pStream = MAP_VAL(_inFileStreams.begin());
		delete pStream;
		_inFileStreams.erase(pStream);
	}
}

bool BaseRTMPProtocol::Initialize(Variant &parameters) {
	GetCustomParameters() = parameters;
	return true;
}

bool BaseRTMPProtocol::AllowFarProtocol(uint64_t type) {
	if (type == PT_TCP
			|| type == PT_RTMPE
			|| type == PT_INBOUND_SSL
			|| type == PT_INBOUND_HTTP_FOR_RTMP)
		return true;
	FATAL("Far protocol %d not accepted", type);
	return false;
}

bool BaseRTMPProtocol::AllowNearProtocol(uint64_t type) {
	FATAL("This protocol doesn't allow any near protocols");
	return false;
}

IOBuffer * BaseRTMPProtocol::GetOutputBuffer() {
	if (GETAVAILABLEBYTESCOUNT(_outputBuffer) > 0)
		return &_outputBuffer;
	return NULL;
}

bool BaseRTMPProtocol::SignalInputData(int32_t recvAmount) {
	ASSERT("OPERATION NOT SUPPORTED");
	return false;
}

bool BaseRTMPProtocol::SignalInputData(IOBuffer &buffer) {
	if (_enqueueForDelete)
		return true;

	bool result = false;
	if (_handshakeCompleted) {
		result = ProcessBytes(buffer);
		//FINEST("Bytes were processed in %d clocks", end - start);
		uint32_t decodedBytes = GetDecodedBytesCount();
		if (result && decodedBytes >= _nextReceivedBytesCountReport) {
			Variant _bytesReadMessage = GenericMessageFactory::GetAck(decodedBytes);
			_nextReceivedBytesCountReport += _winAckSize;
			//FINEST("BR\n%s", STR(_bytesReadMessage.ToString()));
			if (!SendMessage(_bytesReadMessage)) {
				FATAL("Unable to send\n%s", STR(_bytesReadMessage.ToString()));
				return false;
			}
		}
	} else {
		result = PerformHandshake(buffer);
		if (!result) {
			FATAL("Unable to perform handshake");
			return false;
		}
		if (_handshakeCompleted) {
			result = SignalInputData(buffer);
			if (result && (GetType() == PT_OUTBOUND_RTMP)) {
				result = _pProtocolHandler->OutboundConnectionEstablished(
						(OutboundRTMPProtocol *) this);
			}
		}
	}
	return result;
}

bool BaseRTMPProtocol::TimePeriodElapsed() {
	ASSERT("Operation not supported");
	return false;
}

void BaseRTMPProtocol::ReadyForSend() {
	LinkedListNode<BaseOutNetRTMPStream *> *pTemp = _pSignaledRTMPOutNetStream;
	while (pTemp != NULL) {
		pTemp->info->ReadyForSend();
		pTemp = pTemp->pPrev;
	}
}

void BaseRTMPProtocol::SetApplication(BaseClientApplication *pApplication) {
	BaseProtocol::SetApplication(pApplication);
	if (pApplication != NULL) {
		_pProtocolHandler = (BaseRTMPAppProtocolHandler *)
				pApplication->GetProtocolHandler(this);
	} else {
		_pProtocolHandler = NULL;
	}
}

void BaseRTMPProtocol::GetStats(Variant &info) {
	BaseProtocol::GetStats(info);
	info["rxInvokes"] = _rxInvokes;
	info["txInvokes"] = _txInvokes;
	for (uint32_t i = 0; i < MAX_STREAMS_COUNT; i++) {
		if (_streams[i] != NULL) {
			Variant si;
			_streams[i]->GetStats(si);
			info["streams"].PushToArray(si);
		}
	}

	FOR_MAP(_inFileStreams, InFileRTMPStream *, InFileRTMPStream *, i) {
		Variant si;
		MAP_VAL(i)->GetStats(si);
		info["streams"].PushToArray(si);
	}
}

bool BaseRTMPProtocol::SendMessage(Variant & message) {
	//    //1. Test the ability to further send data
	//    if (GETAVAILABLEBYTESCOUNT(_outputBuffer) >= MAX_RTMP_OUTPUT_BUFFER) {
	//        FATAL("Too many data left unsent [%d of maximum %d]. Abort!",
	//                GETAVAILABLEBYTESCOUNT(_outputBuffer), MAX_RTMP_OUTPUT_BUFFER);
	//        return false;
	//    }
	//    FINEST("message:\n%s", STR(message.ToString()));

	//2. Send the message
	if (!_rtmpProtocolSerializer.Serialize(_channels[(uint32_t) VH_CI(message)],
			message, _outputBuffer, _outboundChunkSize)) {
		FATAL("Unable to serialize RTMP message");
		return false;
	}

	_txInvokes++;

	//3. Mark the connection as ready for outbound transfer
	return EnqueueForOutbound();
}

bool BaseRTMPProtocol::SendRawData(Header &header, Channel &channel, uint8_t *pData,
		uint32_t length) {
	if (!header.Write(channel, _outputBuffer)) {
		FATAL("Unable to serialize message header");
		return false;
	}

	_outputBuffer.ReadFromBuffer(pData, length);

	return EnqueueForOutbound();
}

bool BaseRTMPProtocol::SendRawData(uint8_t *pData, uint32_t length) {
	_outputBuffer.ReadFromBuffer(pData, length);
	return EnqueueForOutbound();
}

void BaseRTMPProtocol::SetWinAckSize(uint32_t winAckSize) {
	_nextReceivedBytesCountReport -= _winAckSize;
	_winAckSize = winAckSize;
	_nextReceivedBytesCountReport += _winAckSize;
}

uint32_t BaseRTMPProtocol::GetOutboundChunkSize() {
	return _outboundChunkSize;
}

bool BaseRTMPProtocol::SetInboundChunkSize(uint32_t chunkSize) {
	WARN("Chunk size changed for RTMP connection %p: %d->%d", this,
			_inboundChunkSize, chunkSize);
	_inboundChunkSize = chunkSize;
	for (uint32_t i = 0; i < MAX_STREAMS_COUNT; i++) {
		if (_streams[i] != NULL) {
			if (TAG_KIND_OF(_streams[i]->GetType(), ST_IN_NET_RTMP))
				((InNetRTMPStream *) _streams[i])->SetChunkSize(_inboundChunkSize);
		}
	}
	return true;
}

void BaseRTMPProtocol::TrySetOutboundChunkSize(uint32_t chunkSize) {
	//    WARN("Try to set outbound Chunk size for RTMP connection %p: %d->%d", this,
	//            _outboundChunkSize, chunkSize);
	if (_outboundChunkSize >= chunkSize) {
		//        WARN("The chunk size will remain the same.");
		return;
	}
	_outboundChunkSize = chunkSize;
	Variant chunkSizeMessage = GenericMessageFactory::GetChunkSize(_outboundChunkSize);
	SendMessage(chunkSizeMessage);
	for (uint32_t i = 0; i < MAX_STREAMS_COUNT; i++) {
		if (_streams[i] != NULL) {
			if (TAG_KIND_OF(_streams[i]->GetType(), ST_OUT_NET_RTMP))
				((BaseOutNetRTMPStream *) _streams[i])->SetChunkSize(_outboundChunkSize);
		}
	}
}

bool BaseRTMPProtocol::CloseStream(uint32_t streamId, bool createNeutralStream) {
	//FINEST("-----bool BaseRTMPProtocol::CloseStream: %d", streamId);
	//1. Validate request
	if (streamId == 0 || streamId >= MAX_STREAMS_COUNT) {
		FATAL("Invalid stream id: %d", streamId);
		return false;
	}

	if (_streams[streamId] == NULL) {
		FATAL("Try to close a NULL stream");
		return false;
	}

	if (_streams[streamId]->GetType() == ST_NEUTRAL_RTMP) {
		return true;
	}


	if (TAG_KIND_OF(_streams[streamId]->GetType(), ST_OUT_NET_RTMP)) {
		//2. Remove it from signaled streams
		LinkedListNode<BaseOutNetRTMPStream *> *pTemp = _pSignaledRTMPOutNetStream;
		while (pTemp != NULL) {
			if (pTemp->info->GetRTMPStreamId() == streamId) {
				_pSignaledRTMPOutNetStream =
						RemoveLinkedList<BaseOutNetRTMPStream *>(pTemp);
				break;
			}
			pTemp = pTemp->pPrev;
		}

		//3. If this is an outbound network stream and his publisher
		//is a file, close that as well
		BaseOutNetRTMPStream *pBaseOutNetRTMPStream = (BaseOutNetRTMPStream *) _streams[streamId];
		if (pBaseOutNetRTMPStream->GetInStream() != NULL) {
			if (TAG_KIND_OF(pBaseOutNetRTMPStream->GetInStream()->GetType(), ST_IN_FILE_RTMP))
				RemoveIFS((InFileRTMPStream *) pBaseOutNetRTMPStream->GetInStream());
		}
	}

	//4. Delete the stream and replace it with a neutral one
	delete _streams[streamId];
	_streams[streamId] = NULL;
	if (createNeutralStream) {
		_streams[streamId] = new RTMPStream(this,
				GetApplication()->GetStreamsManager(), streamId);
	}
	//FINEST("Stream %d closed", streamId);

	return true;
}

RTMPStream * BaseRTMPProtocol::CreateNeutralStream(uint32_t & streamId) {
	//FINEST("-----bool BaseRTMPProtocol::CreateNeutralStream: %d", streamId);
	if (streamId == 0) {
		//Automatic allocation
		for (uint32_t i = 1; i < MAX_STREAMS_COUNT; i++) {
			if (_streams[i] == NULL) {
				streamId = i;
				break;
			}
		}

		if (streamId == 0) {
			return NULL;
		}
	} else {
		if (streamId == 0 || streamId >= MAX_STREAMS_COUNT) {
			FATAL("Invalid stream id: %d", streamId);
			return NULL;
		}
		if (_streams[streamId] != NULL) {
			FATAL("Try to create a neutral stream on a non NULL placeholder");
			return NULL;
		}
	}

	RTMPStream *pStream = new RTMPStream(this,
			GetApplication()->GetStreamsManager(), streamId);
	_streams[streamId] = pStream;

	return pStream;
}

InNetRTMPStream * BaseRTMPProtocol::CreateINS(uint32_t channelId,
		uint32_t streamId, string streamName) {
	//FINEST("-----bool BaseRTMPProtocol::CreateNIS: %d", streamId);
	if (streamId == 0 || streamId >= MAX_STREAMS_COUNT) {
		FATAL("Invalid stream id: %d", streamId);
		return NULL;
	}

	if (_streams[streamId] == NULL) {
		FATAL("Try to publish a stream on a NULL placeholder");
		return NULL;
	}

	if (_streams[streamId]->GetType() != ST_NEUTRAL_RTMP) {
		FATAL("Try to publish a stream over a non neutral stream");
		return NULL;
	}

	delete _streams[streamId];
	_streams[streamId] = NULL;

	InNetRTMPStream *pStream = new InNetRTMPStream(this,
			GetApplication()->GetStreamsManager(), streamName, streamId,
			_inboundChunkSize, channelId);

	_streams[streamId] = pStream;

	return pStream;
}

BaseOutNetRTMPStream * BaseRTMPProtocol::CreateONS(uint32_t streamId,
		string streamName, uint64_t inStreamType) {
	//FINEST("-----bool BaseRTMPProtocol::CreateNOS: %d", streamId);
	if (streamId == 0 || streamId >= MAX_STREAMS_COUNT) {
		FATAL("Invalid stream id: %d", streamId);
		return NULL;
	}

	if (_streams[streamId] == NULL) {
		FATAL("Try to play a stream on a NULL placeholder");
		return NULL;
	}

	if (_streams[streamId]->GetType() != ST_NEUTRAL_RTMP) {
		FATAL("Try to play a stream over a non neutral stream: id: %d; type: %d",
				streamId, _streams[streamId]->GetType());
		return NULL;
	}

	delete _streams[streamId];
	_streams[streamId] = NULL;

	BaseOutNetRTMPStream *pBaseOutNetRTMPStream = BaseOutNetRTMPStream::GetInstance(
			this, GetApplication()->GetStreamsManager(), streamName, streamId,
			_outboundChunkSize, inStreamType);

	if (pBaseOutNetRTMPStream == NULL) {
		FATAL("Unable to create stream");
		return NULL;
	}

	_streams[streamId] = pBaseOutNetRTMPStream;

	return pBaseOutNetRTMPStream;
}

void BaseRTMPProtocol::SignalONS(BaseOutNetRTMPStream *pONS) {
	//    FINEST("-----Stream %d from protocol %d was marked as signal-able",
	//            GetId(), pNOS->GetId());
	LinkedListNode<BaseOutNetRTMPStream *> *pTemp = _pSignaledRTMPOutNetStream;
	while (pTemp != NULL) {
		if (pTemp->info == pONS) {
			//WARN("Already marked for signaling");
			return;
		}
		pTemp = pTemp->pPrev;
	}
	_pSignaledRTMPOutNetStream = AddLinkedList<BaseOutNetRTMPStream *>(
			_pSignaledRTMPOutNetStream, pONS, true);
}

InFileRTMPStream * BaseRTMPProtocol::CreateIFS(Variant &metadata) {
	//FINEST("-----bool BaseRTMPProtocol::CreateFIS:\n%s", STR(metadata.ToString()));
	InFileRTMPStream *pRTMPInFileStream = InFileRTMPStream::GetInstance(
			this, GetApplication()->GetStreamsManager(), metadata);
	if (pRTMPInFileStream == NULL) {
		FATAL("Unable to get file stream. Metadata:\n%s", STR(metadata.ToString()));
		return NULL;
	}
	if (!pRTMPInFileStream->Initialize(
			(int32_t) metadata[CONF_APPLICATION_CLIENTSIDEBUFFER])) {
		FATAL("Unable to initialize file inbound stream");
		delete pRTMPInFileStream;
		return NULL;
	}
	_inFileStreams[pRTMPInFileStream] = pRTMPInFileStream;
	return pRTMPInFileStream;
}

void BaseRTMPProtocol::RemoveIFS(InFileRTMPStream *pIFS) {
	//FINEST("-----Remove FIS: %p", pIFS);
	_inFileStreams.erase(pIFS);
	delete pIFS;
}

Channel *BaseRTMPProtocol::ReserveChannel() {
	if (_channelsPool.size() == 0)
		return 0;

	uint32_t result = 0;
	result = _channelsPool[0];
	_channelsPool.erase(_channelsPool.begin());

	return &_channels[result];
}

void BaseRTMPProtocol::ReleaseChannel(Channel *pChannel) {
	if (pChannel == NULL)
		return;
	if (pChannel->id <= 63)
		ADD_VECTOR_BEGIN(_channelsPool, pChannel->id);
	else
		ADD_VECTOR_END(_channelsPool, pChannel->id);
}

bool BaseRTMPProtocol::EnqueueForTimeEvent(uint32_t seconds) {
	ASSERT("Operation not supported. Please use a timer protocol");
	return false;
}

uint32_t BaseRTMPProtocol::GetDHOffset(uint8_t *pBuffer, uint8_t schemeNumber) {
	switch (schemeNumber) {
		case 0:
		{
			return GetDHOffset0(pBuffer);
		}
		case 1:
		{
			return GetDHOffset1(pBuffer);
		}
		default:
		{
			WARN("Invalid scheme number: %d. Defaulting to 0", schemeNumber);
			return GetDHOffset0(pBuffer);
		}
	}
}

uint32_t BaseRTMPProtocol::GetDigestOffset(uint8_t *pBuffer, uint8_t schemeNumber) {
	switch (schemeNumber) {
		case 0:
		{
			return GetDigestOffset0(pBuffer);
		}
		case 1:
		{
			return GetDigestOffset1(pBuffer);
		}
		default:
		{
			WARN("Invalid scheme number: %d. Defaulting to 0", schemeNumber);
			return GetDigestOffset0(pBuffer);
		}
	}
}

uint32_t BaseRTMPProtocol::GetDHOffset0(uint8_t *pBuffer) {
	uint32_t offset = pBuffer[1532] + pBuffer[1533] + pBuffer[1534] + pBuffer[1535];
	offset = offset % 632;
	offset = offset + 772;
	if (offset + 128 >= 1536) {
		ASSERT("Invalid DH offset");
	}
	return offset;
}

uint32_t BaseRTMPProtocol::GetDHOffset1(uint8_t *pBuffer) {
	uint32_t offset = pBuffer[768] + pBuffer[769] + pBuffer[770] + pBuffer[771];
	offset = offset % 632;
	offset = offset + 8;
	if (offset + 128 >= 1536) {
		ASSERT("Invalid DH offset");
	}
	return offset;
}

uint32_t BaseRTMPProtocol::GetDigestOffset0(uint8_t *pBuffer) {
	uint32_t offset = pBuffer[8] + pBuffer[9] + pBuffer[10] + pBuffer[11];
	offset = offset % 728;
	offset = offset + 12;
	if (offset + 32 >= 1536) {
		ASSERT("Invalid digest offset");
	}
	return offset;
}

uint32_t BaseRTMPProtocol::GetDigestOffset1(uint8_t *pBuffer) {
	uint32_t offset = pBuffer[772] + pBuffer[773] + pBuffer[774] + pBuffer[775];
	offset = offset % 728;
	offset = offset + 776;
	if (offset + 32 >= 1536) {
		ASSERT("Invalid digest offset");
	}
	return offset;
}

bool BaseRTMPProtocol::ProcessBytes(IOBuffer &buffer) {
	while (true) {
		uint32_t availableBytesCount = GETAVAILABLEBYTESCOUNT(buffer);
		if (_selectedChannel < 0) {
			//FINEST("availableBytesCount: %d",availableBytesCount);
			if (availableBytesCount < 1) {
				//FINEST("Not enough data");
				return true;
			} else {
				switch (GETIBPOINTER(buffer)[0]&0x3f) {
					case 0:
					{
						if (availableBytesCount < 2) {
							FINEST("Not enough data");
							return true;
						}
						_selectedChannel = 64 + GETIBPOINTER(buffer)[1];
						_channels[_selectedChannel].lastInHeaderType = GETIBPOINTER(buffer)[0] >> 6;
						buffer.Ignore(2);
						availableBytesCount -= 2;
						break;
					}
					case 1:
					{
						if (availableBytesCount < 3) {
							FINEST("Not enough data");
							return true;
						}
						_selectedChannel = 64 + GETIBPOINTER(buffer)[1] + GETIBPOINTER(buffer)[2];
						_channels[_selectedChannel].lastInHeaderType = GETIBPOINTER(buffer)[0] >> 6;
						buffer.Ignore(3);
						availableBytesCount -= 3;
						break;
					};
					default:
					{
						_selectedChannel = GETIBPOINTER(buffer)[0]&0x3f;
						_channels[_selectedChannel].lastInHeaderType = GETIBPOINTER(buffer)[0] >> 6;
						buffer.Ignore(1);
						availableBytesCount -= 1;
						break;
					}
				}
			}
		}

		Channel &channel = _channels[_selectedChannel];
		Header &header = channel.lastInHeader;

		if (channel.state == CS_HEADER) {
			if (!header.Read(_selectedChannel, channel.lastInHeaderType,
					buffer, availableBytesCount)) {
				FATAL("Unable to read header");
				return false;
			} else {
				if (header.readCompleted) {
					//FINEST("Header:%s", STR(header));
					channel.state = CS_PAYLOAD;
					switch (channel.lastInHeaderType) {
						case HT_FULL:
							channel.lastInAbsTs = H_TS(header);
							break;
						case HT_SAME_STREAM:
						case HT_SAME_LENGTH_AND_STREAM:
							channel.lastInAbsTs += H_TS(header);
							break;
						case HT_CONTINUATION:
							if (channel.lastInProcBytes == 0) {
								//FINEST("Time adjusted on channel %d", channel.id);
								channel.lastInAbsTs += H_TS(header);
							}
							break;
					}
				} else {
					//FINEST("Not enough data");
					return true;
				}
			}
		}

		if (channel.state == CS_PAYLOAD) {
			uint32_t tempSize = H_ML(header) - channel.lastInProcBytes;
			//FINEST("Expected: %d", tempSize);
			tempSize = (tempSize >= _inboundChunkSize) ? _inboundChunkSize : tempSize;
			//FINEST("Chunked: %d", tempSize);
			uint32_t availableBytes = GETAVAILABLEBYTESCOUNT(buffer);
			switch (H_MT(header)) {
				case RM_HEADER_MESSAGETYPE_VIDEODATA:
				{
					//FINEST("V: tempSize: %d; availableBytes: %d; totalRequired: %d",
					//        tempSize, availableBytes, header.messageLength);
					if (tempSize <= availableBytes) {
						//FINEST("V: Enough data");
						channel.state = CS_HEADER;
						_selectedChannel = -1;

						//TODO: THIS is anti-performance fix. It works
						//but it needs to be replaced with other means of detecting
						//the type of stream. Calling a function for each packet
						//is not a nice thing to do
						if (H_SI(header) >= MAX_STREAMS_COUNT) {
							FATAL("Incorrect stream index");
							return false;
						}
						if ((_streams[H_SI(header)] != NULL)
								&& (_streams[H_SI(header)]->GetType() == ST_IN_NET_RTMP)) {
							if (!((InNetRTMPStream *) _streams[H_SI(header)])->FeedData(
									GETIBPOINTER(buffer), //pData,
									tempSize, //dataLength,
									channel.lastInProcBytes, //processedLength,
									H_ML(header), //totalLength,
									channel.lastInAbsTs, //absoluteTimestamp,
									false //isAudio
									)) {
								FATAL("Unable to feed video");
								return false;
							}
						}

						channel.lastInProcBytes += tempSize;
						//FINEST("V: processedBytes: %d", channel.processedBytes);
						if (H_ML(header) == channel.lastInProcBytes) {
							//FINEST("V: %d Message completed: %d",
							//  GetId(),
							//  header.messageLength);
							channel.lastInProcBytes = 0;
						}
						if (!buffer.Ignore(tempSize)) {
							FATAL("V: Unable to ignore %d bytes", tempSize);
							return false;
						}
						break;
					} else {
						//FINEST("V: Not enough data. Available: %d; Wanted: %d",
						//  availableBytes, tempSize);
						return true;
					}
				}
				case RM_HEADER_MESSAGETYPE_AUDIODATA:
				{
					//FINEST("A: tempSize: %d; availableBytes: %d; totalRequired: %d",
					//        tempSize, availableBytes, header.messageLength);
					if (tempSize <= availableBytes) {
						//FINEST("A: Enough data");
						channel.state = CS_HEADER;
						_selectedChannel = -1;

						//TODO: see the todo in the video case
						if (H_SI(header) >= MAX_STREAMS_COUNT) {
							FATAL("Incorrect stream index");
							return false;
						}
						if ((_streams[H_SI(header)] != NULL)
								&& (_streams[H_SI(header)]->GetType() == ST_IN_NET_RTMP)) {
							if (!((InNetRTMPStream *) _streams[H_SI(header)])->FeedData(
									GETIBPOINTER(buffer), //pData,
									tempSize, //dataLength,
									channel.lastInProcBytes, //processedLength,
									H_ML(header), //totalLength,
									channel.lastInAbsTs, //absoluteTimestamp,
									true //isAudio
									)) {
								FATAL("Unable to feed audio");
								return false;
							}
						}


						channel.lastInProcBytes += tempSize;
						//FINEST("A: processedBytes: %d", channel.processedBytes);
						if (H_ML(header) == channel.lastInProcBytes) {
							//FINEST("A: %d Message completed: %d",
							//  GetId(),
							//  header.messageLength);
							channel.lastInProcBytes = 0;
						}
						if (!buffer.Ignore(tempSize)) {
							FATAL("A: Unable to ignore %d bytes", tempSize);
							return false;
						}
						break;
					} else {
						//FINEST("A: Not enough data. Available: %d; Wanted: %d",
						//  availableBytes, tempSize);
						return true;
					}
				}
				default:
				{
					if (tempSize <= availableBytes) {
						channel.state = CS_HEADER;
						_selectedChannel = -1;
						channel.inputData.ReadFromInputBuffer(buffer, tempSize);
						channel.lastInProcBytes += tempSize;
						if (!buffer.Ignore(tempSize)) {
							FATAL("Unable to ignore %d bytes", tempSize);
							return false;
						}
						if (H_ML(header) == channel.lastInProcBytes) {
							channel.lastInProcBytes = 0;
							if (_pProtocolHandler == NULL) {
								FATAL("RTMP connection no longer associated with an application");
								return false;
							}
							//FINEST("Buffer:\n%s", STR(channel.buffer));
							if (!_pProtocolHandler->InboundMessageAvailable(this, header, channel.inputData)) {
								FATAL("Unable to send rtmp message to application");
								return false;
							}
							_rxInvokes++;

							if (GETAVAILABLEBYTESCOUNT(channel.inputData) != 0) {
								FATAL("Invalid message!!! We have leftovers: %d bytes",
										GETAVAILABLEBYTESCOUNT(channel.inputData));
								return false;
							}
						} else {
							//FINEST("Channel buffer length: %d", channel.buffer.GetAvailableBytesCount());
						}
						break;
					} else {
						//                        FINEST("I: Not enough data. Available: %d; Wanted: %d",
						//                                availableBytes, tempSize);
						return true;
					}
				}
			}
		}
	}
}
#endif /* HAS_PROTOCOL_RTMP */

