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
#include "protocols/mms/destination.h"

Destination::Destination(string file, string codecName, int sampleRate) {
	_file = file;
	_outbufSize = 1024 * 16;
	_inPCMSize = 0;
	_pInPCM = NULL;
	_codecName = codecName;
	_sampleRate = sampleRate;
	FINEST("_sampleRate: %d", _sampleRate);
}

Destination::~Destination() {

}

bool Destination::Init() {
	//AVCodec *codec = avcodec_find_encoder(_codecId);
	AVCodec *codec = avcodec_find_encoder_by_name(_codecName.c_str());
	if (!codec) {
		FATAL("codec not found");
		return false;
	}

	_pContext = avcodec_alloc_context();

	//_pContext->bit_rate = 64000;
	_pContext->sample_rate = _sampleRate;
	_pContext->channels = 2;
	_pContext->sample_fmt = AV_SAMPLE_FMT_S16;

	if (avcodec_open(_pContext, codec) < 0) {
		FATAL("could not open codec");
		return false;
	}

	_pOutbuf = new uint8_t[_outbufSize];
	_pInPCM = new int16_t[_pContext->frame_size * _pContext->channels];
	_inPCMSize = 0;

	//FINEST("Done init");
	pFile = fopen(_file.c_str(), "w+");

	return true;
}

bool Destination::EncodePCM(int16_t *pPCM, uint32_t size) {
	_data.ReadFromBuffer((uint8_t *) pPCM, size * 2);
	uint32_t chunkSize = (uint32_t) _pContext->frame_size * (uint32_t) _pContext->channels;
	while (GETAVAILABLEBYTESCOUNT(_data) / 2 >= chunkSize) {
		int out_size = avcodec_encode_audio(_pContext, _pOutbuf, _outbufSize,
				(int16_t *) GETIBPOINTER(_data));
		_data.Ignore(chunkSize * 2);
		if (out_size < 0) {
			FATAL("avcodec_encode_audio failed");
			return false;
		}
		if (!EnqueueForTransfer(_pOutbuf, (uint32_t) out_size)) {
			FATAL("Unable to enqueue data for transfer");
			return false;
		}
	}
	return _data.MoveData();
}

bool Destination::EnqueueForTransfer(uint8_t *pData, uint32_t length) {
	_encoded.ReadFromBuffer(pData, length);
	return true;
}

int Destination::_writePacket(void* cookie, uint8_t* buffer, int bufferSize) {
	return 0;
}
#endif /* HAS_PROTOCOL_MMS */
