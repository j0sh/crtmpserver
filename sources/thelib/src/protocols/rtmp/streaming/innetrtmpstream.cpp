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
#include "protocols/rtmp/streaming/innetrtmpstream.h"
#include "streaming/baseoutstream.h"
#include "protocols/rtmp/basertmpprotocol.h"
#include "protocols/rtmp/messagefactories/streammessagefactory.h"
#include "protocols/rtmp/streaming/baseoutnetrtmpstream.h"
#include "protocols/rtmp/streaming/outfilertmpflvstream.h"
#include "streaming/streamstypes.h"

InNetRTMPStream::InNetRTMPStream(BaseProtocol *pProtocol,
		StreamsManager *pStreamsManager, string name,
		uint32_t rtmpStreamId, uint32_t chunkSize, uint32_t channelId)
: BaseInNetStream(pProtocol, pStreamsManager, ST_IN_NET_RTMP, name) {
	_rtmpStreamId = rtmpStreamId;
	_chunkSize = chunkSize;
	_channelId = channelId;
	_clientId = format("%d_%d_%"PRIz"u", _pProtocol->GetId(), _rtmpStreamId, (size_t)this);
	_lastVideoTime = 0;
	_lastAudioTime = 0;
	_pOutFileRTMPFLVStream = NULL;

	_audioPacketsCount = 0;
	_audioDroppedPacketsCount = 0;
	_audioBytesCount = 0;
	_audioDroppedBytesCount = 0;
	_videoPacketsCount = 0;
	_videoDroppedPacketsCount = 0;
	_videoBytesCount = 0;
	_videoDroppedBytesCount = 0;
}

InNetRTMPStream::~InNetRTMPStream() {
	if (_pOutFileRTMPFLVStream != NULL) {
		delete _pOutFileRTMPFLVStream;
		_pOutFileRTMPFLVStream = NULL;
	}
}

StreamCapabilities * InNetRTMPStream::GetCapabilities() {
	return &_streamCapabilities;
}

void InNetRTMPStream::ReadyForSend() {
	ASSERT("Operation not supported");
}

bool InNetRTMPStream::IsCompatibleWithType(uint64_t type) {
	return TAG_KIND_OF(type, ST_OUT_NET_RTMP_4_RTMP)
			|| TAG_KIND_OF(type, ST_OUT_FILE_RTMP)
			|| TAG_KIND_OF(type, ST_OUT_NET_RTP)
			|| TAG_KIND_OF(type, ST_OUT_FILE_HLS);
}

void InNetRTMPStream::GetStats(Variant &info, uint32_t namespaceId) {
	BaseInNetStream::GetStats(info, namespaceId);
	info["audio"]["packetsCount"] = _audioPacketsCount;
	info["audio"]["droppedPacketsCount"] = (uint64_t) 0;
	info["audio"]["bytesCount"] = _audioBytesCount;
	info["audio"]["droppedBytesCount"] = (uint64_t) 0;
	info["video"]["packetsCount"] = _videoPacketsCount;
	info["video"]["droppedPacketsCount"] = (uint64_t) 0;
	info["video"]["bytesCount"] = _videoBytesCount;
	info["video"]["droppedBytesCount"] = (uint64_t) 0;
}

uint32_t InNetRTMPStream::GetRTMPStreamId() {
	return _rtmpStreamId;
}

uint32_t InNetRTMPStream::GetChunkSize() {
	return _chunkSize;
}

void InNetRTMPStream::SetChunkSize(uint32_t chunkSize) {
	_chunkSize = chunkSize;
	LinkedListNode<BaseOutStream *> *pTemp = _pOutStreams;
	while (pTemp != NULL) {
		if (!pTemp->info->IsEnqueueForDelete()) {
			if (TAG_KIND_OF(pTemp->info->GetType(), ST_OUT_NET_RTMP)) {
				((BaseRTMPProtocol *) pTemp->info->GetProtocol())->TrySetOutboundChunkSize(chunkSize);
			}
		}
		pTemp = pTemp->pPrev;
	}
}

bool InNetRTMPStream::SendStreamMessage(Variant &completeMessage, bool persistent) {
	//2. Loop on the subscribed streams and send the message
	LinkedListNode<BaseOutStream *> *pTemp = _pOutStreams;
	while ((pTemp != NULL) && (!IsEnqueueForDelete())) {
		if (pTemp->info->IsEnqueueForDelete()) {
			FINEST("IsEnqueueForDelete is true. Move ahead....");
			pTemp = pTemp->pPrev;
			continue;
		}
		if (TAG_KIND_OF(pTemp->info->GetType(), ST_OUT_NET_RTMP)) {
			if (!((BaseOutNetRTMPStream *) pTemp->info)->SendStreamMessage(completeMessage)) {
				FATAL("Unable to send notify on stream. The connection will go down");
				pTemp->info->EnqueueForDelete();
			}
		}
		pTemp = pTemp->pPrev;
	}

	//3. Test to see if we are still alive. One of the target streams might
	//be on the same RTMP connection as this stream is and our connection
	//here might be enque for delete
	if (IsEnqueueForDelete())
		return false;

	//4. Save the message for future use if necessary
	if (persistent)
		_lastStreamMessage = completeMessage;

	if ((uint32_t) VH_MT(completeMessage) == RM_HEADER_MESSAGETYPE_NOTIFY) {
		Variant &params = M_NOTIFY_PARAMS(completeMessage);
		if ((params == V_MAP) && (params.MapSize() >= 2)) {
			Variant &notify = MAP_VAL(params.begin());
			if ((notify == V_STRING) && (lowerCase((string) notify) == "onmetadata")) {
				Variant &metadata = MAP_VAL(++params.begin());
				if (metadata == V_MAP) {
					if (metadata.HasKeyChain(_V_NUMERIC, false, 1, "bandwidth")) {
						_streamCapabilities.bandwidthHint = (uint32_t) metadata["bandwidth"];
					} else {
						if (metadata.HasKeyChain(_V_NUMERIC, false, 1, "audiodatarate")) {
							_streamCapabilities.bandwidthHint =
									(uint32_t) metadata["audiodatarate"];
						}
						if (metadata.HasKeyChain(_V_NUMERIC, false, 1, "videodatarate")) {
							_streamCapabilities.bandwidthHint +=
									(uint32_t) metadata["videodatarate"];
						}
					}
				}
			}
		}
	}
	//5. Done
	return true;
}

bool InNetRTMPStream::SendStreamMessage(string functionName, Variant &parameters,
		bool persistent) {

	//1. Prepare the message
	Variant message = StreamMessageFactory::GetFlexStreamSend(0, 0, 0, true,
			functionName, parameters);

	return SendStreamMessage(message, persistent);
}

bool InNetRTMPStream::SendOnStatusStreamPublished() {
	Variant response = StreamMessageFactory::GetInvokeOnStatusStreamPublished(
			_channelId,
			_rtmpStreamId,
			0, false,
			0,
			RM_INVOKE_PARAMS_ONSTATUS_LEVEL_STATUS,
			RM_INVOKE_PARAMS_ONSTATUS_CODE_NETSTREAMPUBLISHSTART,
			format("Stream `%s` is now published", STR(GetName())),
			GetName(),
			_clientId);

	if (!GetRTMPProtocol()->SendMessage(response)) {
		FATAL("Unable to send message");
		return false;
	}
	return true;
}

bool InNetRTMPStream::Record(BaseOutFileStream *pOutStream) {

	_pOutFileRTMPFLVStream = pOutStream;
	return _pOutFileRTMPFLVStream->Link(this);
}

void InNetRTMPStream::SignalOutStreamAttached(BaseOutStream *pOutStream) {
	if (GETAVAILABLEBYTESCOUNT(_videoCodecInit) != 0) {
		if (!pOutStream->FeedData(GETIBPOINTER(_videoCodecInit),
				GETAVAILABLEBYTESCOUNT(_videoCodecInit), 0,
				GETAVAILABLEBYTESCOUNT(_videoCodecInit),
				_lastAudioTime, false)) {
			FINEST("Unable to feed OS: %u", pOutStream->GetUniqueId());
			pOutStream->EnqueueForDelete();
		}
	}

	if (GETAVAILABLEBYTESCOUNT(_audioCodecInit) != 0) {
		if (!pOutStream->FeedData(GETIBPOINTER(_audioCodecInit),
				GETAVAILABLEBYTESCOUNT(_audioCodecInit), 0,
				GETAVAILABLEBYTESCOUNT(_audioCodecInit),
				_lastAudioTime, true)) {
			FINEST("Unable to feed OS: %u", pOutStream->GetUniqueId());
			pOutStream->EnqueueForDelete();
		}
	}

	if (_lastStreamMessage != V_NULL) {
		if (TAG_KIND_OF(pOutStream->GetType(), ST_OUT_NET_RTMP)) {
			if (!((BaseOutNetRTMPStream *) pOutStream)->SendStreamMessage(
					_lastStreamMessage)) {
				FATAL("Unable to send notify on stream. The connection will go down");
				pOutStream->EnqueueForDelete();
			}
		}
	}
}

void InNetRTMPStream::SignalOutStreamDetached(BaseOutStream *pOutStream) {
	FINEST("outbound stream %u detached from inbound stream %u",
			pOutStream->GetUniqueId(), GetUniqueId());
}

bool InNetRTMPStream::SignalPlay(double &absoluteTimestamp, double &length) {
	return true;
}

bool InNetRTMPStream::SignalPause() {
	return true;
}

bool InNetRTMPStream::SignalResume() {
	return true;
}

bool InNetRTMPStream::SignalSeek(double &absoluteTimestamp) {
	return true;
}

bool InNetRTMPStream::SignalStop() {
	return true;
}

bool InNetRTMPStream::FeedData(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp, bool isAudio) {
	if (isAudio) {
		_audioPacketsCount++;
		_audioBytesCount += dataLength;
		if ((processedLength == 0) //beginning of a packet
				&& ((pData[0] >> 4) == 10) //AAC content
				&& (pData[1] == 0) // AAC sequence header
				) {
			if (!InitializeAudioCapabilities(pData, dataLength)) {
				FATAL("Unable to initialize audio capabilities");
				return false;
			}
		}
		_lastAudioTime = absoluteTimestamp;
	} else {
		_videoPacketsCount++;
		_videoBytesCount += dataLength;
		if ((processedLength == 0) //beginning of a packet
				&& (pData[0] == 0x17) //0x10 - key frame, 0x07 h264 content
				&& (pData[1] == 0) //AVC sequence header
				) {
			if (!InitializeVideoCapabilities(pData, dataLength)) {
				FATAL("Unable to initialize audio capabilities");
				return false;
			}
		}
		_lastVideoTime = absoluteTimestamp;
	}

	LinkedListNode<BaseOutStream *> *pTemp = _pOutStreams;
	while (pTemp != NULL) {
		if (!pTemp->info->IsEnqueueForDelete()) {
			if (!pTemp->info->FeedData(pData, dataLength, processedLength, totalLength,
					absoluteTimestamp, isAudio)) {
				FINEST("Unable to feed OS: %p", pTemp->info);
				pTemp->info->EnqueueForDelete();
				if (GetProtocol() == pTemp->info->GetProtocol()) {
					return false;
				}
			}
		}
		pTemp = pTemp->pPrev;
	}
	return true;
}

BaseRTMPProtocol *InNetRTMPStream::GetRTMPProtocol() {
	return (BaseRTMPProtocol *) _pProtocol;
}

bool InNetRTMPStream::InitializeAudioCapabilities(uint8_t *pData, uint32_t length) {
	if (length < 4) {
		FATAL("Invalid length");
		return false;
	}
	_audioCodecInit.IgnoreAll();
	_audioCodecInit.ReadFromBuffer(pData, length);
	if (!_streamCapabilities.InitAudioAAC(pData + 2, length - 2)) {
		FATAL("InitAudioAAC failed");
		return false;
	}

	//	FINEST("Cached the AAC audio codec initialization: %"PRIu32,
	//			GETAVAILABLEBYTESCOUNT(_audioCodecInit));

	return true;
}

bool InNetRTMPStream::InitializeVideoCapabilities(uint8_t *pData, uint32_t length) {
	if (length == 0)
		return false;

	_videoCodecInit.IgnoreAll();
	_videoCodecInit.ReadFromBuffer(pData, length);
	uint8_t *pSPS = pData + 13;
	uint32_t spsLength = ENTOHSP(pData + 11);
	uint8_t *pPPS = pData + 13 + spsLength + 3;
	uint32_t ppsLength = ENTOHSP(pData
			+ 13 + spsLength + 1);
	if (!_streamCapabilities.InitVideoH264(pSPS, spsLength, pPPS, ppsLength)) {
		FATAL("InitVideoH264 failed");
		return false;
	}

	//	FINEST("Cached the h264 video codec initialization: %u",
	//			GETAVAILABLEBYTESCOUNT(_videoCodecInit));

	return true;
}
#endif /* HAS_PROTOCOL_RTMP */

