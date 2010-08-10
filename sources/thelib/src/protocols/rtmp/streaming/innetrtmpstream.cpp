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
	_clientId = format("%d_%d_%d", _pProtocol->GetId(), _rtmpStreamId, this);
	_lastVideoTime = 0;
	_lastAudioTime = 0;
	_pOutFileRTMPFLVStream = NULL;
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
			|| TAG_KIND_OF(type, ST_OUT_NET_RTP);
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

bool InNetRTMPStream::RecordFLV(Variant &meta, bool append) {
	//1. Compute the file name
	string fileName = meta[META_SERVER_MEDIA_DIR];
	fileName += (string) meta[META_SERVER_FILE_NAME];
	FINEST("fileName: %s", STR(fileName));

	//2. Delete the old file
	if (append) {
		WARN("append not supported yet. File will be overwritten");
	}
	DeleteFile(fileName);

	//3. Create the out file
	_pOutFileRTMPFLVStream = new OutFileRTMPFLVStream(_pProtocol,
			_pStreamsManager, fileName);

	//4. Link it
	return _pOutFileRTMPFLVStream->Link(this);
}

bool InNetRTMPStream::RecordMP4(Variant &meta) {
	NYIR;
}

void InNetRTMPStream::SignalOutStreamAttached(BaseOutStream *pOutStream) {
	if (GETAVAILABLEBYTESCOUNT(_videoCodecInit) != 0) {
		if (!pOutStream->FeedData(GETIBPOINTER(_videoCodecInit),
				GETAVAILABLEBYTESCOUNT(_videoCodecInit), 0,
				GETAVAILABLEBYTESCOUNT(_videoCodecInit),
				_lastAudioTime, false)) {
			FINEST("Unable to feed OS: %d", pOutStream->GetUniqueId());
			pOutStream->EnqueueForDelete();
		}
	}

	if (GETAVAILABLEBYTESCOUNT(_audioCodecInit) != 0) {
		if (!pOutStream->FeedData(GETIBPOINTER(_audioCodecInit),
				GETAVAILABLEBYTESCOUNT(_audioCodecInit), 0,
				GETAVAILABLEBYTESCOUNT(_audioCodecInit),
				_lastAudioTime, true)) {
			FINEST("Unable to feed OS: %d", pOutStream->GetUniqueId());
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
		if (_streamCapabilities.audioCodecId == CODEC_AUDIO_UNKNOWN) {
			if ((processedLength == 0) && //beginning of a packet
					(pData[0] >> 4) == 10 && //AAC content
					(pData[1] == 0)) {// AAC sequence header
				_audioCodecInit.IgnoreAll();
				_audioCodecInit.ReadFromBuffer(pData, dataLength);
				FINEST("Cached the audio codec initialization: %d",
						GETAVAILABLEBYTESCOUNT(_audioCodecInit));
			}
		}
		_lastAudioTime = absoluteTimestamp;
	} else {
		if (_streamCapabilities.videoCodecId == CODEC_VIDEO_UNKNOWN) {
			if (processedLength == 0) {
				InitializeVideoCapabilities(pData, dataLength);
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

void InNetRTMPStream::InitializeVideoCapabilities(uint8_t *pData, uint32_t length) {
	switch (pData[0]&0x0f) {
		case 1:
		{
			_streamCapabilities.videoCodecId = CODEC_VIDEO_JPEG;
			break;
		}
		case 2:
		{
			_streamCapabilities.videoCodecId = CODEC_VIDEO_SORENSON_H263;
			break;
		}
		case 3:
		{
			_streamCapabilities.videoCodecId = CODEC_VIDEO_SCREEN_VIDEO;
			break;
		}
		case 4:
		{
			_streamCapabilities.videoCodecId = CODEC_VIDEO_VP6;
			break;
		}
		case 5:
		{
			_streamCapabilities.videoCodecId = CODEC_VIDEO_VP6_ALPHA;
			break;
		}
		case 7:
		{
			if ((pData[0] >> 4) != 1)
				break;

			_videoCodecInit.IgnoreAll();
			_videoCodecInit.ReadFromBuffer(pData, length);
			//            FINEST("_videoCodecInit:\n%s", STR(_videoCodecInit));
			//            FINEST("Cached the video codec initialization: %d",
			//                    GETAVAILABLEBYTESCOUNT(_videoCodecInit));

			_streamCapabilities.videoCodecId = CODEC_VIDEO_AVC;

			_streamCapabilities.videoCodecInfo.avc.SPSLength = ntohsp(pData + 11);
			_streamCapabilities.videoCodecInfo.avc.pSPS = new uint8_t[_streamCapabilities.videoCodecInfo.avc.SPSLength];
			memcpy(_streamCapabilities.videoCodecInfo.avc.pSPS, pData + 13,
					_streamCapabilities.videoCodecInfo.avc.SPSLength);

			_streamCapabilities.videoCodecInfo.avc.PPSLength = ntohsp(pData
					+ 13 + _streamCapabilities.videoCodecInfo.avc.SPSLength + 1);
			_streamCapabilities.videoCodecInfo.avc.pPPS = new uint8_t[_streamCapabilities.videoCodecInfo.avc.PPSLength];
			memcpy(_streamCapabilities.videoCodecInfo.avc.pPPS,
					pData + 13 + _streamCapabilities.videoCodecInfo.avc.SPSLength + 3,
					_streamCapabilities.videoCodecInfo.avc.PPSLength);

			break;
		}
		default:
		{
			WARN("Unsupported codec: %d", pData[0]&0x0f);
		}
	}
}
#endif /* HAS_PROTOCOL_RTMP */

