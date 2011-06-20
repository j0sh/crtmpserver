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
#ifndef _MMSPROTOCOL_H
#define	_MMSPROTOCOL_H

#include "protocols/baseprotocol.h"
#include <iconv.h>

class Transcoder;
class Destination;
class InNetRawStream;
class InNetAACStream;
class InNetMP3Stream;

class MMSProtocol
: public BaseProtocol {
private:
	string _player;
	IOBuffer _outputBuffer;
	IOBuffer _msgBuffer;
	IOBuffer _asfData;
	iconv_t _ic;
	uint16_t _seq;
	uint32_t _openFileId;
	Transcoder *_pTranscoder;
	uint32_t _packetSize;

	InNetRawStream *_pRawAACStream;
	bool _enableRawAAC;
	string _rawAACStreamName;

	InNetAACStream *_pAACStream;
	bool _enableAAC;
	string _aacStreamName;

	InNetRawStream *_pRawMP3Stream;
	bool _enableRawMP3;
	string _rawMP3StreamName;

	InNetMP3Stream *_pMP3Stream;
	bool _enableMP3;
	string _mp3StreamName;
public:
	MMSProtocol();
	virtual ~MMSProtocol();

	virtual bool Initialize(Variant &parameters);
	virtual IOBuffer * GetOutputBuffer();
	virtual bool AllowFarProtocol(uint64_t type);
	virtual bool AllowNearProtocol(uint64_t type);
	virtual bool SignalInputData(int32_t recvAmount);
	virtual bool SignalInputData(IOBuffer &buffer);

	bool Start();
private:
	bool ProcessMessage(IOBuffer &buffer);
	bool ProcessData(IOBuffer &buffer);
	bool SendLinkViewerToMacConnect();
	bool SendLinkViewerToMacConnectFunnel();
	bool SendLinkViewerToMacOpenFile();
	bool SendLinkViewerToMacReadBlock();
	bool SendLinkViewerToMacStreamSwitch();
	bool SendLinkViewerToMacStartPlaying();
	bool SendLinkViewerToMacPong();
	bool SendMessage(uint32_t MID);
	bool EncodeUTF16(IOBuffer &dest, string &src);
	bool ParseASFHeader();
};

#endif	/* _MMSPROTOCOL_H */
#endif /* HAS_PROTOCOL_MMS */
