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

class BaseInStream;

#define MMS_DATA_SIZE 1024*8

class Transcoder {
private:

	struct Input {
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
		IOBuffer _inputBuffer;
		Input();
		virtual ~Input();
		bool Init();
		bool ReadPCM();
		static int _readPacket(void* cookie, uint8_t* buffer, int bufferSize);
	} _input;

	struct Output {
		AVCodecContext *_pCodecContext;
		uint8_t *_pOutbuf;
		int _outbufSize;
		int16_t *_pInPCM;
		uint32_t _inPCMSize;
		string _codecName;
		IOBuffer _data;
		IOBuffer _encoded;
		double _absoluteTimestamp;
		uint64_t _totalSamplesEncoded;
		Output();
		virtual ~Output();
		bool Init(string codecName, int _sampleRate, int channelsCount);
		bool EncodePCM(int16_t *pPCM, uint32_t size,
				BaseInStream * pStream1,
				BaseInStream * pStream2);
	};
	Output *_pAAC;
	Output *_pMP3;
public:
	Transcoder(IOBuffer &asfHeaders);
	virtual ~Transcoder();
	bool Init(bool hasAAC, bool hasMP3);
	bool PushData(uint8_t *pData, uint32_t length,
			BaseInStream *pAACStream1,
			BaseInStream *pAACStream2,
			BaseInStream *pMP3Stream1,
			BaseInStream *pMP3Stream2);
};

#endif	/* _SOURCE_H */
#endif /* HAS_PROTOCOL_MMS */
