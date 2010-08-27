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
#include "protocols/rtp/streaming/outnetrtpudph264stream.h"
#include "streaming/nalutypes.h"
#include "streaming/streamstypes.h"
#include "streaming/baseinstream.h"
#include "protocols/rtp/connectivity/outboundconnectivity.h"

#define MAX_RTP_PACKET_SIZE 1350
//#define RTP_BIG_CHUNK
#define RTP_NORMAL

OutNetRTPUDPH264Stream::OutNetRTPUDPH264Stream(BaseProtocol *pProtocol,
		StreamsManager *pStreamsManager, string name)
: BaseOutNetRTPUDPStream(pProtocol, pStreamsManager, name) {
	_videoCounter = rand();
	memset(&_videoData, 0, sizeof (_videoData));
	_videoData.msg_iov = new iovec[2];
	_videoData.msg_iovlen = 2;
	_videoData.msg_namelen = sizeof (sockaddr_in);
	_videoData.msg_iov[0].iov_base = new uint8_t[14];
	((uint8_t *) _videoData.msg_iov[0].iov_base)[0] = 0x80;
	put_htonl(((uint8_t *) _videoData.msg_iov[0].iov_base) + 8, _ssrc);
	_pSPS = NULL;
	_SPSLen = 0;
	_pPPS = NULL;
	_PPSLen = 0;

	_audioCounter = rand();
	memset(&_audioData, 0, sizeof (_audioData));
	_audioData.msg_iov = new iovec[3];
	_audioData.msg_iovlen = 3;
	_audioData.msg_namelen = sizeof (sockaddr_in);
	_audioData.msg_iov[0].iov_len = 14;
	_audioData.msg_iov[0].iov_base = new uint8_t[14];
	((uint8_t *) _audioData.msg_iov[0].iov_base)[0] = 0x80;
	((uint8_t *) _audioData.msg_iov[0].iov_base)[1] = 0xe0;
	put_htonl(((uint8_t *) _audioData.msg_iov[0].iov_base) + 8, _ssrc);
	_audioData.msg_iov[1].iov_len = 0;
	_audioData.msg_iov[1].iov_base = new uint8_t[16];
	_audioPacketsCount = 0;
}

OutNetRTPUDPH264Stream::~OutNetRTPUDPH264Stream() {
	delete[] (uint8_t *) _videoData.msg_iov[0].iov_base;
	delete[] _videoData.msg_iov;
	memset(&_videoData, 0, sizeof (_videoData));
	if (_pSPS != NULL)
		delete[] _pSPS;
	if (_pPPS != NULL)
		delete[] _pPPS;

	delete[] (uint8_t *) _audioData.msg_iov[0].iov_base;
	delete[] (uint8_t *) _audioData.msg_iov[1].iov_base;
	delete[] _audioData.msg_iov;
	memset(&_audioData, 0, sizeof (_audioData));
}

bool OutNetRTPUDPH264Stream::FeedDataVideo(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp, bool isAudio) {

#ifdef RTP_NORMAL
	return FeedDataVideoFUA(pData, dataLength, processedLength, totalLength,
			absoluteTimestamp);
#endif

#ifdef RTP_BIG_CHUNK
#define RTP_BIG_CHUNK_SIZE 5000
	if (processedLength == 0) {
		uint32_t tempProcessed = 0;
		uint32_t tempTotalLength = GETAVAILABLEBYTESCOUNT(_videoBuffer);
		while (GETAVAILABLEBYTESCOUNT(_videoBuffer) > 0) {
			uint32_t size = GETAVAILABLEBYTESCOUNT(_videoBuffer);
			size = size > RTP_BIG_CHUNK_SIZE ? RTP_BIG_CHUNK_SIZE : size;
			if (!FeedDataVideo(GETIBPOINTER(_videoBuffer),
					size,
					tempProcessed, tempTotalLength,
					absoluteTimestamp, isAudio)) {
				return false;
			}
			tempProcessed += size;
			_videoBuffer.Ignore(size);
		}
	}
	_videoBuffer.ReadFromBuffer(pData, dataLength);
#endif

#ifdef RTP_COMPLETE_PACKETS
	if (processedLength == 0) {
		if (GETAVAILABLEBYTESCOUNT(_videoBuffer) > 0) {
			if (!FeedDataVideo(GETIBPOINTER(_videoBuffer),
					GETAVAILABLEBYTESCOUNT(_videoBuffer),
					0, GETAVAILABLEBYTESCOUNT(_videoBuffer),
					absoluteTimestamp, isAudio)) {
				ASSERT("_videoBuffer:\n%s", STR(_videoBuffer));
			}
			_videoBuffer.IgnoreAll();
		}
	}
	_videoBuffer.ReadFromBuffer(pData, dataLength);
#endif

#ifdef RTP_NORMAL_CHECKSUM
	FeedDataVideo(pData, dataLength, processedLength, totalLength,
			absoluteTimestamp, isAudio);
	if (processedLength == 0) {
		if (GETAVAILABLEBYTESCOUNT(_videoBuffer) > 0) {
			_videoBuffer.Ignore(9);
			_orig = md5(string((char *) GETIBPOINTER(_videoBuffer),
					GETAVAILABLEBYTESCOUNT(_videoBuffer)), true);
			if (_copy != "") {
				if (_copy != _orig) {
					ASSERT("oooops!")
				}
				FINEST("_orig: %s; _copy: %s", STR(_orig), STR(_copy));
			}
			_videoBuffer.IgnoreAll();
		}
	}
	_videoBuffer.ReadFromBuffer(pData, dataLength);
#endif

	return true;
}

bool OutNetRTPUDPH264Stream::FeedDataAudio(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp, bool isAudio) {
	//#define FeedDataAudioMPEG4Generic FeedDataAudioMPEG4Generic_one_by_one
#define FeedDataAudioMPEG4Generic FeedDataAudioMPEG4Generic_aggregate
	return FeedDataAudioMPEG4Generic(pData, dataLength, processedLength, totalLength,
			absoluteTimestamp);
}

void OutNetRTPUDPH264Stream::SignalAttachedToInStream() {
	StreamCapabilities *pCapabilities = GetCapabilities();

	_SPSLen = pCapabilities->videoCodecInfo.avc.SPSLength + 12;
	_pSPS = new uint8_t[_SPSLen];
	_pSPS[0] = 0x80;
	_pSPS[1] = 0xE1;
	put_htonl(_pSPS + 8, _ssrc);
	memcpy(_pSPS + 12, pCapabilities->videoCodecInfo.avc.pSPS,
			pCapabilities->videoCodecInfo.avc.SPSLength);

	_PPSLen = pCapabilities->videoCodecInfo.avc.PPSLength + 12;
	_pPPS = new uint8_t[_PPSLen];
	_pPPS[0] = 0x80;
	_pPPS[1] = 0xE1;
	put_htonl(_pPPS + 8, _ssrc);
	memcpy(_pPPS + 12, pCapabilities->videoCodecInfo.avc.pPPS,
			pCapabilities->videoCodecInfo.avc.PPSLength);
}

bool OutNetRTPUDPH264Stream::FeedDataVideoFUA(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp) {
	if (_pInStream->GetType() == ST_IN_NET_RTMP) {
		if (processedLength == 0) {
			if (pData[1] != 1)
				return true;
			pData += 9;
			dataLength -= 9;
		} else {
			processedLength -= 9;
		}
		totalLength -= 9;
	} else {
		if (
				//				(NALU_TYPE(pData[0]) != NALU_TYPE_SPS)
				//				&& (NALU_TYPE(pData[0]) != NALU_TYPE_PPS)
				(NALU_TYPE(pData[0]) != NALU_TYPE_SLICE)
				&& (NALU_TYPE(pData[0]) != NALU_TYPE_IDR)) {
			//FINEST("ignoring absoluteTimestamp: %.2f; %s", absoluteTimestamp, STR(NALUToString(pData[0])));
			return true;
		}
	}

	//	if (processedLength == 0 && NALU_TYPE(pData[0]) == NALU_TYPE_IDR) {
	//		put_htons(_pSPS + 2, _videoCounter);
	//		_videoCounter++;
	//		put_htonl(_pSPS + 4, (uint32_t) (absoluteTimestamp * 90.0));
	//		put_htons(_pPPS + 2, _videoCounter);
	//		_videoCounter++;
	//		put_htonl(_pPPS + 4, (uint32_t) (absoluteTimestamp * 90.0));
	//
	//		_pConnectivity->FeedVideoData(_pSPS, _SPSLen);
	//		_pConnectivity->FeedVideoData(_pPPS, _PPSLen);
	//	}

	uint32_t sentAmount = 0;
	uint32_t chunkSize = 0;
	while (sentAmount != dataLength) {
		chunkSize = dataLength - sentAmount;
		chunkSize = chunkSize < MAX_RTP_PACKET_SIZE ? chunkSize : MAX_RTP_PACKET_SIZE;

		//1. Flags
		if (processedLength + sentAmount + chunkSize == totalLength) {
			((uint8_t *) _videoData.msg_iov[0].iov_base)[1] = 0xe1;
		} else {
			((uint8_t *) _videoData.msg_iov[0].iov_base)[1] = 0x61;
		}

		//2. counter
		put_htons(((uint8_t *) _videoData.msg_iov[0].iov_base) + 2, _videoCounter);
		_videoCounter++;

		//3. Timestamp
		put_htonl(((uint8_t *) _videoData.msg_iov[0].iov_base) + 4,
				BaseConnectivity::ToRTPTS(absoluteTimestamp, 90000));

		if (chunkSize == totalLength) {
			//4. No chunking
			_videoData.msg_iov[0].iov_len = 12;
			_videoData.msg_iov[1].iov_base = pData;
			_videoData.msg_iov[1].iov_len = chunkSize;
		} else {
			//5. Chunking
			_videoData.msg_iov[0].iov_len = 14;

			if (processedLength + sentAmount == 0) {
#ifdef RTP_NORMAL_CHECKSUM
				_videoBufferCopy.ReadFromBuffer(pData, chunkSize);
#endif
				//6. First chunk
				((uint8_t *) _videoData.msg_iov[0].iov_base)[12] = (pData[0]&0xe0) | NALU_TYPE_FUA;
				((uint8_t *) _videoData.msg_iov[0].iov_base)[13] = (pData[0]&0x1f) | 0x80;
				_videoData.msg_iov[1].iov_base = pData + 1;
				_videoData.msg_iov[1].iov_len = chunkSize - 1;
			} else {
				if (processedLength + sentAmount + chunkSize == totalLength) {
#ifdef RTP_NORMAL_CHECKSUM
					_videoBufferCopy.ReadFromBuffer(pData, chunkSize);
					_copy = md5(string((char *) GETIBPOINTER(_videoBufferCopy),
							GETAVAILABLEBYTESCOUNT(_videoBufferCopy)), true);
					_videoBufferCopy.IgnoreAll();
#endif
					//7. Last chunk
					((uint8_t *) _videoData.msg_iov[0].iov_base)[13] &= 0x1f;
					((uint8_t *) _videoData.msg_iov[0].iov_base)[13] |= 0x40;
				} else {
#ifdef RTP_NORMAL_CHECKSUM
					_videoBufferCopy.ReadFromBuffer(pData, chunkSize);
#endif
					//8. Middle chunk
					((uint8_t *) _videoData.msg_iov[0].iov_base)[13] &= 0x1f;
				}
				_videoData.msg_iov[1].iov_base = pData;
				_videoData.msg_iov[1].iov_len = chunkSize;
			}
		}

		_pConnectivity->FeedVideoData(_videoData);
		sentAmount += chunkSize;
		pData += chunkSize;
	}

	return true;
}

bool OutNetRTPUDPH264Stream::FeedDataAudioMPEG4Generic_aggregate(uint8_t *pData,
		uint32_t dataLength, uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp) {
	//1. We only support frame-by-frame approach
	if (dataLength != totalLength) {
		WARN("Chunked mode not yet supported");
		return true;
	}

	//2. Test if we need to send what we have so far
	//	FINEST("_audioData.msg_iov[0].iov_len: %d", _audioData.msg_iov[0].iov_len);
	//	FINEST("_audioData.msg_iov[1].iov_len: %d", _audioData.msg_iov[1].iov_len);
	//	FINEST("GETAVAILABLEBYTESCOUNT(_audioBuffer): %d", GETAVAILABLEBYTESCOUNT(_audioBuffer));
	//	FINEST("dataLength: %d", dataLength);
	//	FINEST("all: %d", 14 + _audioData.msg_iov[1].iov_len + GETAVAILABLEBYTESCOUNT(_audioBuffer) + 2 + dataLength - 7);
	if (((14 + _audioData.msg_iov[1].iov_len + GETAVAILABLEBYTESCOUNT(_audioBuffer) + 2 + dataLength - 7) > MAX_RTP_PACKET_SIZE)
			|| (_audioData.msg_iov[1].iov_len == 16)) {
		//		string message = "";
		//		for (uint32_t i = 0; i < _audioData.msg_iov[1].iov_len; i++) {
		//			message += format("%02x ", ((uint8_t *) _audioData.msg_iov[1].iov_base)[i]);
		//		}
		//		message += "\n";
		//		for (uint32_t i = 0; i < _audioData.msg_iov[1].iov_len; i += 2) {
		//			uint16_t val = ntohs((*((uint16_t *) _audioData.msg_iov[1].iov_base + i / 2)));
		//			message += format("i: %d; val: %04x; len: %d; index: %d\n", i,
		//					val,
		//					(val >> 3),
		//					(val & 0x07));
		//		}
		//		WARN("Flush here\n%s", STR(message));

		//3. counter
		put_htons(((uint8_t *) _audioData.msg_iov[0].iov_base) + 2, _audioCounter);
		_audioCounter++;

		//4. Timestamp
		put_htonl(((uint8_t *) _audioData.msg_iov[0].iov_base) + 4,
				BaseConnectivity::ToRTPTS(absoluteTimestamp,
				GetCapabilities()->audioCodecInfo.aac.sampleRate));

		//6. put the actual buffer
		_audioData.msg_iov[2].iov_len = GETAVAILABLEBYTESCOUNT(_audioBuffer);
		_audioData.msg_iov[2].iov_base = GETIBPOINTER(_audioBuffer);

		put_htons(((uint8_t *) _audioData.msg_iov[0].iov_base) + 12,
				_audioData.msg_iov[1].iov_len * 8);

		_pConnectivity->FeedAudioData(_audioData);

		_audioBuffer.IgnoreAll();
		_audioData.msg_iov[1].iov_len = 0;
	}

	//3. AU-Header
	uint16_t auHeader = (uint16_t) ((dataLength - 7) << 3);
	auHeader = auHeader | ((uint8_t) (_audioData.msg_iov[1].iov_len / 2));
	//FINEST("auHeader: %04x", auHeader);
	put_htons(((uint8_t *) _audioData.msg_iov[1].iov_base) + _audioData.msg_iov[1].iov_len, auHeader);
	_audioData.msg_iov[1].iov_len += 2;


	//4. Save the buffer
	_audioBuffer.ReadFromBuffer(pData + 7, dataLength - 7);

	return true;
}

bool OutNetRTPUDPH264Stream::FeedDataAudioMPEG4Generic_one_by_one(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp) {
	//1. We only support frame-by-frame approach
	if (dataLength != totalLength) {
		WARN("Chunked mode not yet supported");
		return true;
	}
	/*
	0                   1                   2                   3
	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |V=2|P|X|  CC   |M|     PT      |       sequence number         |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                           timestamp                           |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |           synchronization source (SSRC) identifier            |
   +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
   |            contributing source (CSRC) identifiers             |
   |                             ....                              |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- .. -+-+-+-+-+-+-+-+-+-+
   |AU-headers-length|AU-header|AU-header|      |AU-header|padding|
   |                 |   (1)   |   (2)   |      |   (n)   | bits  |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- .. -+-+-+-+-+-+-+-+-+-+
	 */

	//	_audioData.msg_iov[0].iov_base[00] V,P,X,CC
	//	_audioData.msg_iov[0].iov_base[01] M,PT
	//	_audioData.msg_iov[0].iov_base[02] sequence number
	//	_audioData.msg_iov[0].iov_base[03] sequence number
	//	_audioData.msg_iov[0].iov_base[04] timestamp
	//	_audioData.msg_iov[0].iov_base[05] timestamp
	//	_audioData.msg_iov[0].iov_base[06] timestamp
	//	_audioData.msg_iov[0].iov_base[07] timestamp
	//	_audioData.msg_iov[0].iov_base[08] SSRC
	//	_audioData.msg_iov[0].iov_base[09] SSRC
	//	_audioData.msg_iov[0].iov_base[10] SSRC
	//	_audioData.msg_iov[0].iov_base[11] SSRC
	//	_audioData.msg_iov[0].iov_base[12] AU-headers-length
	//	_audioData.msg_iov[0].iov_base[13] AU-headers-length

	//3. counter
	put_htons(((uint8_t *) _audioData.msg_iov[0].iov_base) + 2, _audioCounter);
	_audioCounter++;

	//4. Timestamp
	put_htonl(((uint8_t *) _audioData.msg_iov[0].iov_base) + 4,
			(uint32_t) (absoluteTimestamp
			* (double) GetCapabilities()->audioCodecInfo.aac.sampleRate / 1000.000));

	put_htons(((uint8_t *) _audioData.msg_iov[0].iov_base) + 12, 16);

	uint16_t auHeader = (uint16_t) ((dataLength - 7) << 3);
	put_htons(((uint8_t *) _audioData.msg_iov[1].iov_base), auHeader);
	_audioData.msg_iov[1].iov_len = 2;

	//	FINEST("%02x%02x %02x%02x %02x%02x%02x%02x %02x%02x%02x%02x - %02x%02x %02x%02x %08x %.2f",
	//			((uint8_t *) _audioData.msg_iov[0].iov_base)[0],
	//			((uint8_t *) _audioData.msg_iov[0].iov_base)[1],
	//			((uint8_t *) _audioData.msg_iov[0].iov_base)[2],
	//			((uint8_t *) _audioData.msg_iov[0].iov_base)[3],
	//			((uint8_t *) _audioData.msg_iov[0].iov_base)[4],
	//			((uint8_t *) _audioData.msg_iov[0].iov_base)[5],
	//			((uint8_t *) _audioData.msg_iov[0].iov_base)[6],
	//			((uint8_t *) _audioData.msg_iov[0].iov_base)[7],
	//			((uint8_t *) _audioData.msg_iov[0].iov_base)[8],
	//			((uint8_t *) _audioData.msg_iov[0].iov_base)[9],
	//			((uint8_t *) _audioData.msg_iov[0].iov_base)[10],
	//			((uint8_t *) _audioData.msg_iov[0].iov_base)[11],
	//			((uint8_t *) _audioData.msg_iov[0].iov_base)[12],
	//			((uint8_t *) _audioData.msg_iov[0].iov_base)[13],
	//			((uint8_t *) _audioData.msg_iov[1].iov_base)[0],
	//			((uint8_t *) _audioData.msg_iov[1].iov_base)[1],
	//			dataLength - 7,
	//			absoluteTimestamp);

	//6. put the actual buffer
	_audioData.msg_iov[2].iov_len = dataLength - 7;
	_audioData.msg_iov[2].iov_base = pData + 7;

	return _pConnectivity->FeedAudioData(_audioData);
}
#endif /* HAS_PROTOCOL_RTP */

