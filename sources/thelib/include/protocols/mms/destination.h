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
#ifndef _DESTINATION_H
#define	_DESTINATION_H
extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
}
#include "common.h"

class Destination {
private:
	AVCodecContext *_pContext;
	uint8_t *_pOutbuf;
	int _outbufSize;
	string _file;
	FILE *pFile;
	int16_t *_pInPCM;
	uint32_t _inPCMSize;
	string _codecName;
	int _sampleRate;
	IOBuffer _data;
	IOBuffer _encoded;
public:

	Destination(string file, string codecName, int sampleRate);
	virtual ~Destination();

	bool Init();
	bool EncodePCM(int16_t *pPCM, uint32_t size);
private:
	bool EnqueueForTransfer(uint8_t *pData, uint32_t length);
	static int _writePacket(void* cookie, uint8_t* buffer, int bufferSize);
};

#endif	/* _DESTINATION_H */
#endif /* HAS_PROTOCOL_MMS */
