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
#include "protocols/mms/transcoder.h"
#include "protocols/rtmp/header_le_ba.h"
#include "streaming/baseinstream.h"

Transcoder::Input::Input() {
	_pRawData = NULL;
	_pPCMData = NULL;
	_pcmSize = 0;
	_pIO = NULL;
	_pFmt = NULL;
	_pFormatCtx = NULL;
	_audioStream = -1;
	_pCodecCtx = NULL;
	_pCodec = NULL;
	memset(&_packet, 0, sizeof (_packet));
}

Transcoder::Input::~Input() {
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
	if (_packet.data != NULL) {
		av_free_packet(&_packet);
	}
}

bool Transcoder::Input::Init() {
	_pRawData = new uint8_t[MMS_DATA_SIZE];
	_pPCMData = new int16_t[AVCODEC_MAX_AUDIO_FRAME_SIZE * 2];
	av_register_all();
	avcodec_init();
	avcodec_register_all();
	av_log_set_level(AV_LOG_QUIET);
	_pIO = avio_alloc_context(_pRawData, MMS_DATA_SIZE, 0, this, _readPacket, NULL, NULL);
	_pFmt = av_find_input_format("asf");
	if (_pFmt == NULL) {
		return false;
	}
	_pFormatCtx=avformat_alloc_context();
	_pFormatCtx->pb=_pIO;
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

bool Transcoder::Input::ReadPCM() {
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
	//FINEST("_pcmSize: %u", _pcmSize);

	return true;
}

int Transcoder::Input::_readPacket(void* pOpaque, uint8_t* buffer, int bufferSize) {
	Input *pInput = (Input *) pOpaque;
	if (GETAVAILABLEBYTESCOUNT(pInput->_inputBuffer) != 0) {
		int32_t chunk = GETAVAILABLEBYTESCOUNT(pInput->_inputBuffer);
		chunk = chunk < bufferSize ? chunk : bufferSize;
		memcpy(buffer, GETIBPOINTER(pInput->_inputBuffer), chunk);
		pInput->_inputBuffer.Ignore(chunk);
		return chunk;
	} else {
		return 0;
	}
}

Transcoder::Output::Output() {
	_pCodecContext = NULL;
	_pOutbuf = NULL;
	_outbufSize = 1024 * 16;
	_pInPCM = NULL;
	_inPCMSize = 0;
	_absoluteTimestamp = 0;
	_totalSamplesEncoded = 0;
}

Transcoder::Output::~Output() {
	if (_pCodecContext != NULL) {
		avcodec_close(_pCodecContext);
		_pCodecContext = NULL;
	}
	if (_pOutbuf != NULL) {
		delete[] _pOutbuf;
	}
	if (_pInPCM != NULL) {
		delete[] _pInPCM;
	}
}

bool Transcoder::Output::Init(string codecName, int sampleRate, int channelsCount) {
	AVCodec *_pCodec = avcodec_find_encoder_by_name(STR(codecName));
	if (!_pCodec) {
		FATAL("codec for format %s not found", STR(codecName));
		return false;
	}

	_pCodecContext = avcodec_alloc_context();

	//_pContext->bit_rate = 64000;
	_pCodecContext->sample_rate = sampleRate;
	_pCodecContext->channels = channelsCount;
	_pCodecContext->sample_fmt = AV_SAMPLE_FMT_S16;

	if (avcodec_open(_pCodecContext, _pCodec) < 0) {
		FATAL("could not open codec");
		return false;
	}

	_pOutbuf = new uint8_t[_outbufSize];
	_pInPCM = new int16_t[_pCodecContext->frame_size * _pCodecContext->channels];
	_inPCMSize = 0;

	return true;
}

bool Transcoder::Output::EncodePCM(int16_t *pPCM, uint32_t size,
		BaseInStream *pStream1,
		BaseInStream *pStream2) {
	_data.ReadFromBuffer((uint8_t *) pPCM, size * 2);
	uint32_t chunkSize = (uint32_t) _pCodecContext->frame_size * (uint32_t) _pCodecContext->channels;
	while (GETAVAILABLEBYTESCOUNT(_data) / 2 >= chunkSize) {
		int out_size = avcodec_encode_audio(_pCodecContext, _pOutbuf, _outbufSize,
				(int16_t *) GETIBPOINTER(_data));
		_data.Ignore(chunkSize * 2);
		if (out_size < 0) {
			FATAL("avcodec_encode_audio failed");
			return false;
		}
		if (pStream1 != NULL) {
			if (!pStream1->FeedData(
					_pOutbuf,
					(uint32_t) out_size,
					0,
					(uint32_t) out_size,
					_absoluteTimestamp, true
					)) {
				FATAL("Unable to feed in stream");
				return false;
			}
		}
		if (pStream2 != NULL) {
			if (!pStream2->FeedData(
					_pOutbuf,
					(uint32_t) out_size,
					0,
					(uint32_t) out_size,
					_absoluteTimestamp, true
					)) {
				FATAL("Unable to feed in stream");
				return false;
			}
		}
		_totalSamplesEncoded += _pCodecContext->frame_size;
		_absoluteTimestamp = ((double) _totalSamplesEncoded
				/ (double) _pCodecContext->sample_rate)*1000.0;
	}
	return _data.MoveData();
}

Transcoder::Transcoder(IOBuffer &initBuffer) {
	_input._inputBuffer.ReadFromBuffer(GETIBPOINTER(initBuffer),
			GETAVAILABLEBYTESCOUNT(initBuffer));
	_pMP3 = NULL;
	_pAAC = NULL;
}

Transcoder::~Transcoder() {
	if (_pAAC != NULL) {
		delete _pAAC;
		_pAAC = NULL;
	}

	if (_pMP3 != NULL) {
		delete _pMP3;
		_pMP3 = NULL;
	}
}

bool Transcoder::Init(bool hasAAC, bool hasMP3) {
	if (!_input.Init()) {
		FATAL("Unable to init input");
		return false;
	}
	if (hasAAC) {
		_pAAC = new Output();
		if (!_pAAC->Init("libfaac",
				_input._pCodecCtx->sample_rate,
				_input._pCodecCtx->channels)) {
			FATAL("Unable to init AAC encoder");
			delete _pAAC;
			_pAAC = NULL;
			return false;
		}
	}
	if (hasMP3) {
		_pMP3 = new Output();
		if (!_pMP3->Init("libmp3lame",
				_input._pCodecCtx->sample_rate,
				_input._pCodecCtx->channels)) {
			FATAL("Unable to init MP3 encoder");
			delete _pMP3;
			_pMP3 = NULL;
			return false;
		}
	}
	return true;
}

time_t _lastT = 0;

bool Transcoder::PushData(uint8_t *pData, uint32_t length,
		BaseInStream *pAACStream1,
		BaseInStream *pAACStream2,
		BaseInStream *pMP3Stream1,
		BaseInStream *pMP3Stream2) {
	if (_lastT == 0)
		_lastT = time(NULL);
	//
	_input._inputBuffer.ReadFromBuffer(pData, length);
	if (!_input.ReadPCM()) {
		FATAL("ASF decoding failed");
		return false;
	}

	if (_pAAC != NULL) {
		if (!_pAAC->EncodePCM(_input._pPCMData, _input._pcmSize, pAACStream1,
				pAACStream2)) {
			FATAL("AAC encoder failed");
			return false;
		}
		//		FINEST("AAC: _absoluteTimestamp: %.6f (%ld) (%.2f)",
		//				_pAAC->_absoluteTimestamp,
		//				time(NULL) - _lastT,
		//				(double) (time(NULL) - _lastT)*1000.0 - _pAAC->_absoluteTimestamp);
	}

	if (_pMP3 != NULL) {
		if (!_pMP3->EncodePCM(_input._pPCMData, _input._pcmSize, pMP3Stream1,
				pMP3Stream2)) {
			FATAL("MP3 encoder failed");
			return false;
		}
		//		FINEST("MP3: _absoluteTimestamp: %.6f (%ld) (%.2f)",
		//				_pMP3->_absoluteTimestamp,
		//				time(NULL) - _lastT,
		//				(double) (time(NULL) - _lastT)*1000.0 - _pMP3->_absoluteTimestamp);
	}

	return true;
}
#endif /* HAS_PROTOCOL_MMS */
