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

#ifdef HAS_PROTOCOL_MMS
#include "protocols/mms/source.h"
#include "protocols/rtmp/header_le_ba.h"

Source::Source(IOBuffer &asfHeaders) {
	_inputBuffer.ReadFromBuffer(GETIBPOINTER(asfHeaders),
			GETAVAILABLEBYTESCOUNT(asfHeaders));
	File f;
	f.Initialize("/tmp/bad.asf", FILE_OPEN_MODE_TRUNCATE);
	f.WriteBuffer(GETIBPOINTER(asfHeaders),
			GETAVAILABLEBYTESCOUNT(asfHeaders));
	f.Close();
	//asfHeaders.IgnoreAll();
	_pRawData = NULL;
	_pPCMData = NULL;
	_pcmSize = 0;
	_pIO = NULL;
	_pFmt = NULL;
	_pFormatCtx = NULL;
	_audioStream = -1;
	_pCodecCtx = NULL;
	_pCodec = NULL;
	av_init_packet(&_packet);
}

Source::~Source() {
	if (_pRawData != NULL) {
		delete[] _pRawData;
		_pRawData = NULL;
	}
	if (_pPCMData != NULL) {
		delete[] _pPCMData;
		_pPCMData = NULL;
	}
	if (_pIO != NULL) {
		av_free(_pIO);
		_pIO = NULL;
	}
	if (_pFormatCtx != NULL) {
		av_close_input_stream(_pFormatCtx);
		_pFormatCtx = NULL;
	}
	av_destruct_packet(&_packet);
}

bool Source::Init() {
	_pRawData = new uint8_t[MMS_DATA_SIZE];
	_pPCMData = new int16_t[AVCODEC_MAX_AUDIO_FRAME_SIZE * 2];
	av_register_all();
	avcodec_init();
	avcodec_register_all();
	_pIO = avio_alloc_context(_pRawData, MMS_DATA_SIZE, 0, this, _readPacket, NULL, NULL);
	_pFmt = av_find_input_format("asf");
	if (_pFmt == NULL) {
		return false;
	}
	if (av_open_input_stream(&_pFormatCtx, _pIO, "", _pFmt, NULL) != 0) {
		FATAL("Unable to open input stream");
		return false;
	}
	if (av_find_stream_info(_pFormatCtx) < 0) {
		FATAL("Couldn't find stream information");
		return false;
	}
	_audioStream = -1;
	for (uint32_t i = 0; i < _pFormatCtx->nb_streams; i++)
		if (_pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
			_audioStream = i;
			break;
		}
	if (_audioStream == -1) {
		FATAL("Didn't find a audio stream");
		return false;
	}
	_pCodecCtx = _pFormatCtx->streams[_audioStream]->codec;

	_pCodec = avcodec_find_decoder(_pCodecCtx->codec_id);
	if (_pCodec == NULL) {
		FATAL("Codec not found");
		return -1;
	}
	if (_pCodec->capabilities & CODEC_CAP_TRUNCATED)
		_pCodecCtx->flags |= CODEC_FLAG_TRUNCATED;

	if (avcodec_open(_pCodecCtx, _pCodec) < 0) {
		FATAL("Could not open codec");
		return false;
	}



	return true;
}

uint32_t Source::GetPCMSize() {
	return _pcmSize;
}

int16_t *Source::GetPCMData() {
	return _pPCMData;
}

int Source::GetRate() {
	return _pCodecCtx->sample_rate;
}

bool Source::ReadPCM() {
	FINEST("_packet.stream_index: %d", _packet.stream_index);
	while (true) {
		if (_packet.data != NULL)
			av_free_packet(&_packet);
		if (av_read_packet(_pFormatCtx, &_packet) < 0) {
			FATAL("av_read_packet failed");
			return false;
		}
		if (_packet.stream_index != _audioStream)
			continue;
		else
			break;
	}
	int outBuffSize = AVCODEC_MAX_AUDIO_FRAME_SIZE;
	if (avcodec_decode_audio3(_pCodecCtx, _pPCMData, &outBuffSize, &_packet) < 0) {
		FATAL("avcodec_decode_audio3 failed");
		return false;
	}
	_pcmSize = outBuffSize / _pCodecCtx->frame_size / _pCodecCtx->channels;
	FINEST("_pcmSize: %u", _pcmSize);
	return true;
}

bool Source::PushData(uint8_t *pData, uint32_t length) {
	//ASSERT("%s", STR(IOBuffer::DumpBuffer(pData, length)));
	File f;
	f.Initialize("/tmp/bad.asf", FILE_OPEN_MODE_APPEND);
	f.WriteBuffer(pData, length);
	f.Close();
	_inputBuffer.ReadFromBuffer(pData, length);
	return ReadPCM();
	//return true;
}

int Source::_readPacket(void* pOpaque, uint8_t* buffer, int bufferSize) {
	Source *pSource = (Source *) pOpaque;
	if (GETAVAILABLEBYTESCOUNT(pSource->_asfHeaders) > 0) {
		int32_t chunk = GETAVAILABLEBYTESCOUNT(pSource->_asfHeaders);
		chunk = chunk < bufferSize ? chunk : bufferSize;
		memcpy(buffer, GETIBPOINTER(pSource->_asfHeaders), chunk);
		pSource->_asfHeaders.Ignore(chunk);
		FINEST("chunk1: %d", chunk);
		return chunk;
	} else if (GETAVAILABLEBYTESCOUNT(pSource->_inputBuffer) != 0) {
		int32_t chunk = GETAVAILABLEBYTESCOUNT(pSource->_inputBuffer);
		chunk = chunk < bufferSize ? chunk : bufferSize;
		memcpy(buffer, GETIBPOINTER(pSource->_inputBuffer), chunk);
		pSource->_inputBuffer.Ignore(chunk);
		FINEST("chunk2: %d", chunk);
		return chunk;
	} else {
		return 0;
	}
}
#endif /* HAS_PROTOCOL_MMS */
