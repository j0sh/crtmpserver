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

OutNetRTPUDPH264Stream::OutNetRTPUDPH264Stream(BaseProtocol *pProtocol,
		StreamsManager *pStreamsManager, string name)
: BaseOutNetRTPUDPStream(pProtocol, pStreamsManager, name) {
	memset(&_videoData, 0, sizeof (_videoData));
	_videoData.msg_iov = new iovec[2];
	_videoData.msg_iovlen = 2;
	_videoData.msg_namelen = sizeof (sockaddr_in);
	_videoData.msg_iov[0].iov_base = new uint8_t[14];
	((uint8_t *) _videoData.msg_iov[0].iov_base)[0] = 0x80;
	EHTONLP(((uint8_t *) _videoData.msg_iov[0].iov_base) + 8, _videoSsrc);
	_pSPS = NULL;
	_SPSLen = 0;
	_pPPS = NULL;
	_PPSLen = 0;

	memset(&_audioData, 0, sizeof (_audioData));
	_audioData.msg_iov = new iovec[3];
	_audioData.msg_iovlen = 3;
	_audioData.msg_namelen = sizeof (sockaddr_in);
	_audioData.msg_iov[0].iov_len = 14;
	_audioData.msg_iov[0].iov_base = new uint8_t[14];
	((uint8_t *) _audioData.msg_iov[0].iov_base)[0] = 0x80;
	((uint8_t *) _audioData.msg_iov[0].iov_base)[1] = 0xe0;
	EHTONLP(((uint8_t *) _audioData.msg_iov[0].iov_base) + 8, _audioSsrc);
	_audioData.msg_iov[1].iov_len = 0;
	_audioData.msg_iov[1].iov_base = new uint8_t[16];


	_audioPacketsCount = 0;
	_audioDroppedPacketsCount = 0;
	_audioBytesCount = 0;
	_videoPacketsCount = 0;
	_videoDroppedPacketsCount = 0;
	_videoBytesCount = 0;
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

void OutNetRTPUDPH264Stream::GetStats(Variant &info, uint32_t namespaceId) {
	BaseOutNetStream::GetStats(info, namespaceId);
	info["audio"]["bytesCount"] = _audioBytesCount;
	info["audio"]["packetsCount"] = _audioPacketsCount;
	info["audio"]["droppedPacketsCount"] = _audioDroppedPacketsCount;
	info["video"]["bytesCount"] = _videoBytesCount;
	info["video"]["packetsCount"] = _videoPacketsCount;
	info["video"]["droppedPacketsCount"] = _videoDroppedPacketsCount;
}

bool OutNetRTPUDPH264Stream::FeedDataVideo(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp, bool isAudio) {
	_videoBytesCount += dataLength;
	_videoPacketsCount++;
	//1. Test and see if this is an inbound RTMP stream. If so,
	//we have to strip out the RTMP 9 bytes header
	if (_pInStream->GetType() == ST_IN_NET_RTMP) {
		//2. Test and see if we have a brand new packet
		if (processedLength == 0) {
			//3. This must be a payload packet, not codec setup
			if (pData[1] != 1)
				return true;
			//4. since this is a brand new packet, empty previous buffer
			_videoBuffer.IgnoreAll();
		}

		//5. Store the data into the buffer
		_videoBuffer.ReadFromBuffer(pData, dataLength);

		//6. Test and see if this is the last chunk of the RTMP packet
		if (dataLength + processedLength == totalLength) {
			//7. This is the last chunk. Get the pointer and length
			pData = GETIBPOINTER(_videoBuffer);
			uint32_t dataLength = GETAVAILABLEBYTESCOUNT(_videoBuffer);

			//8. We must have at least 9 bytes (RTMP header size)
			if (dataLength < 9) {
				WARN("Bogus packet");
				return true;
			}

			//9. Read the composition timestamp and add it to the
			//absolute timestamp
			uint32_t compositionTimeStamp = (ENTOHLP(pData + 1))&0x00ffffff;
			absoluteTimestamp += compositionTimeStamp;

			//10. Ignore RTMP header and composition offset
			pData += 5;
			dataLength -= 5;

			uint32_t nalSize = 0;
			//uint32_t tsIncrement = 0;

			//11. Start looping over the RTMP payload. Each NAL has a 4 bytes
			//header indicating the length of the following NAL
			while (dataLength >= 4) {
				//12. Read the nal size and compare it to the actual amount
				//of data remaining on the buffer
				nalSize = ENTOHLP(pData);
				if (nalSize > (dataLength - 4)) {
					WARN("Bogus packet");
					return true;
				}

				//13. skip theNAL size field
				pData += 4;
				dataLength -= 4;

				//14. Is this a 0 sized NAL? if so, skip it
				if (nalSize == 0)
					continue;

				//15. Feed the NAL unit using RTP FUA
				if (!FeedDataVideoFUA(pData, nalSize, 0, nalSize,
						absoluteTimestamp)) { //+ (double) tsIncrement / 90000.00)) {
					FATAL("Unable to feed data");
					return false;
				}

				//16. move to the next NAL
				pData += nalSize;
				dataLength -= nalSize;
			}
		}
		return true;
	} else {
		//17. This is NAL stream. Feed it as it is
		return FeedDataVideoFUA(pData, dataLength, processedLength, totalLength,
				absoluteTimestamp);
	}
}

bool OutNetRTPUDPH264Stream::FeedDataAudio(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp, bool isAudio) {
	_audioBytesCount += dataLength;
	_audioPacketsCount++;
	return FeedDataAudioMPEG4Generic(pData, dataLength, processedLength, totalLength,
			absoluteTimestamp);
}

void OutNetRTPUDPH264Stream::SignalAttachedToInStream() {
	StreamCapabilities *pCapabilities = GetCapabilities();

	_SPSLen = pCapabilities->avc._spsLength + 12;
	_pSPS = new uint8_t[_SPSLen];
	_pSPS[0] = 0x80;
	_pSPS[1] = 0xE1;
	EHTONLP(_pSPS + 8, _videoSsrc);
	memcpy(_pSPS + 12, pCapabilities->avc._pSPS,
			pCapabilities->avc._spsLength);

	_PPSLen = pCapabilities->avc._ppsLength + 12;
	_pPPS = new uint8_t[_PPSLen];
	_pPPS[0] = 0x80;
	_pPPS[1] = 0xE1;
	EHTONLP(_pPPS + 8, _videoSsrc);
	memcpy(_pPPS + 12, pCapabilities->avc._pPPS,
			pCapabilities->avc._ppsLength);
}

bool OutNetRTPUDPH264Stream::FeedDataVideoFUA(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp) {
	//1. We are only going to send IDR and SLICE nals
	//	if ((NALU_TYPE(pData[0]) != NALU_TYPE_SLICE)
	//			&& (NALU_TYPE(pData[0]) != NALU_TYPE_IDR)) {
	//		return true;
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
		EHTONSP(((uint8_t *) _videoData.msg_iov[0].iov_base) + 2, _videoCounter);
		_videoCounter++;

		//3. Timestamp
		EHTONLP(((uint8_t *) _videoData.msg_iov[0].iov_base) + 4,
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
				//6. First chunk
				((uint8_t *) _videoData.msg_iov[0].iov_base)[12] = (pData[0]&0xe0) | NALU_TYPE_FUA;
				((uint8_t *) _videoData.msg_iov[0].iov_base)[13] = (pData[0]&0x1f) | 0x80;
				_videoData.msg_iov[1].iov_base = pData + 1;
				_videoData.msg_iov[1].iov_len = chunkSize - 1;
			} else {
				if (processedLength + sentAmount + chunkSize == totalLength) {
					//7. Last chunk
					((uint8_t *) _videoData.msg_iov[0].iov_base)[13] &= 0x1f;
					((uint8_t *) _videoData.msg_iov[0].iov_base)[13] |= 0x40;
				} else {
					//8. Middle chunk
					((uint8_t *) _videoData.msg_iov[0].iov_base)[13] &= 0x1f;
				}
				_videoData.msg_iov[1].iov_base = pData;
				_videoData.msg_iov[1].iov_len = chunkSize;
			}
		}

		_pConnectivity->FeedVideoData(_videoData, absoluteTimestamp);
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
	if (((14 + _audioData.msg_iov[1].iov_len + GETAVAILABLEBYTESCOUNT(_audioBuffer) + 2 + dataLength - 7) > MAX_RTP_PACKET_SIZE)
			|| (_audioData.msg_iov[1].iov_len == 16)) {
		//3. counter
		EHTONSP(((uint8_t *) _audioData.msg_iov[0].iov_base) + 2, _audioCounter);
		_audioCounter++;

		//4. Timestamp
		EHTONLP(((uint8_t *) _audioData.msg_iov[0].iov_base) + 4,
				BaseConnectivity::ToRTPTS(absoluteTimestamp,
				GetCapabilities()->aac._sampleRate));

		//6. put the actual buffer
		_audioData.msg_iov[2].iov_len = GETAVAILABLEBYTESCOUNT(_audioBuffer);
		_audioData.msg_iov[2].iov_base = GETIBPOINTER(_audioBuffer);

		EHTONSP(((uint8_t *) _audioData.msg_iov[0].iov_base) + 12,
				_audioData.msg_iov[1].iov_len * 8);

		_pConnectivity->FeedAudioData(_audioData, absoluteTimestamp);

		_audioBuffer.IgnoreAll();
		_audioData.msg_iov[1].iov_len = 0;
	}

	//3. AU-Header
	uint16_t auHeader = (uint16_t) ((dataLength - 7) << 3);
	auHeader = auHeader | ((uint8_t) (_audioData.msg_iov[1].iov_len / 2));
	EHTONSP(((uint8_t *) _audioData.msg_iov[1].iov_base) + _audioData.msg_iov[1].iov_len, auHeader);
	_audioData.msg_iov[1].iov_len += 2;


	//4. Save the buffer
	_audioBuffer.ReadFromBuffer(pData + 7, dataLength - 7);

	return true;
}

bool OutNetRTPUDPH264Stream::FeedDataAudioMPEG4Generic_one_by_one(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength, double absoluteTimestamp) {

	//1. Take care of chunked content first
	//this will update pData and dataLength if necessary
	if (dataLength != totalLength) {
		//2. This is chunked content. Test if this is the first chunk from the
		//packet
		if (processedLength == 0) {
			//3. This is the first chunk of the packet.
			//Empty the old buffer and store this new chunk
			_audioBuffer.IgnoreAll();
			_audioBuffer.ReadFromBuffer(pData, dataLength);
			return true;
		} else {
			//4. This is not the first chunk. Test to see if this is
			//the last chunk or not
			if (totalLength < processedLength + dataLength) {
				//5. This is not the last chunk of the packet.
				//Test and see if we have any previous data inside the buffer
				//if we don't, that means we didn't catch the beginning
				//of the packet so we discard everything
				if (GETAVAILABLEBYTESCOUNT(_audioBuffer) == 0) {
					return true;
				}

				//6. Store the data
				_audioBuffer.ReadFromBuffer(pData, dataLength);

				//7. Done
				return true;
			} else {
				//8. This is the last chunk of the packet.
				//Test and see if we have any previous data inside the buffer
				//if we don't, that means we didn't catch the beginning
				//of the packet so we discard everything
				if (GETAVAILABLEBYTESCOUNT(_audioBuffer) == 0) {
					return true;
				}

				//9. Store the data
				_audioBuffer.ReadFromBuffer(pData, dataLength);

				//10. Get the buffer and its length
				pData = GETIBPOINTER(_audioBuffer);
				dataLength = GETAVAILABLEBYTESCOUNT(_audioBuffer);

				//11. Do a final test and see if we have all the data
				if (dataLength != totalLength) {
					FATAL("Invalid data length");
					return false;
				}
			}
		}
	}

	uint64_t inStreamType = _pInStream->GetType();

	if ((inStreamType == ST_IN_NET_RTMP) || (inStreamType == ST_IN_NET_RTP)) {
		//2. Do we have enough data to read the RTMP header?
		if (dataLength <= 2) {
			WARN("Bogus AAC packet");
			_audioBuffer.IgnoreAll();
			return true;
		}

		//3. Take care of the RTMP headers if necessary
		if (inStreamType == ST_IN_NET_RTMP) {
			//3. Is this a RTMP codec setup? If so, ignore it
			if (pData[1] != 1) {
				_audioBuffer.IgnoreAll();
				return true;
			}
		}

		//4. Skip the RTMP header
		dataLength -= 2;
		pData += 2;
	}

	//4. Do we have enough data to detect the ADTS header presence?
	if (dataLength <= 2) {
		WARN("Bogus AAC packet");
		_audioBuffer.IgnoreAll();
		return true;
	}

	//4. The packet might start with an ADTS header. Remove it if necessary
	uint32_t adtsHeaderLength = 0;
	if ((ENTOHSP(pData) >> 3) == 0x1fff) {
		adtsHeaderLength = 7;
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

	//5. counter
	EHTONSP(((uint8_t *) _audioData.msg_iov[0].iov_base) + 2, _audioCounter);
	_audioCounter++;

	//6. Timestamp
	EHTONLP(((uint8_t *) _audioData.msg_iov[0].iov_base) + 4,
			(uint32_t) (absoluteTimestamp
			* (double) GetCapabilities()->aac._sampleRate / 1000.000));

	EHTONSP(((uint8_t *) _audioData.msg_iov[0].iov_base) + 12, 16);

	uint16_t auHeader = (uint16_t) ((dataLength - adtsHeaderLength) << 3);
	EHTONSP(((uint8_t *) _audioData.msg_iov[1].iov_base), auHeader);
	_audioData.msg_iov[1].iov_len = 2;

	//7. put the actual buffer
	_audioData.msg_iov[2].iov_len = dataLength - adtsHeaderLength;
	_audioData.msg_iov[2].iov_base = pData + adtsHeaderLength;

	if (!_pConnectivity->FeedAudioData(_audioData, absoluteTimestamp)) {
		FATAL("Unable to feed data");
		_audioBuffer.IgnoreAll();
		return false;
	}

	_audioBuffer.IgnoreAll();
	return true;
}
#endif /* HAS_PROTOCOL_RTP */

