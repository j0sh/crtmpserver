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
#ifndef _SOURCE_H
#define	_SOURCE_H

#include "common.h"
extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
}

#define MMS_DATA_SIZE 1024*8

class Source {
private:
	uint8_t *_pRawData;
	int16_t *_pPCMData;
	uint32_t _pcmSize;
	AVIOContext *_pIO;
	AVInputFormat *_pFmt;
	AVFormatContext *_pFormatCtx;
	int _audioStream;
	AVCodecContext *_pCodecCtx;
	AVCodec *_pCodec;
	AVPacket _packet;
	IOBuffer _asfHeaders;
	IOBuffer _inputBuffer;
public:
	Source(IOBuffer &asfHeaders);
	virtual ~Source();

	bool Init();
	uint32_t GetPCMSize();
	int16_t *GetPCMData();
	int GetRate();
	bool ReadPCM();
	bool PushData(uint8_t *pData, uint32_t length);
private:
	static int _readPacket(void* cookie, uint8_t* buffer, int bufferSize);
	static int _writePacket(void *opaque, uint8_t *buf, int buf_size);
};

#endif	/* _SOURCE_H */
#endif /* HAS_PROTOCOL_MMS */
