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
#include "protocols/mms/mmsprotocol.h"
#include "protocols/mms/transcoder.h"
#include "application/baseclientapplication.h"
#include "streaming/innetrawstream.h"
#include "protocols/mms/innetaacstream.h"
#include "protocols/mms/innetmp3stream.h"

MMSProtocol::MMSProtocol()
: BaseProtocol(PT_OUTBOUND_MMS) {
	_seq = 0;
	_openFileId = 0;
	_pTranscoder = NULL;
	_packetSize = 0;

	_pRawAACStream = NULL;
	_enableRawAAC = true;

	_pAACStream = NULL;
	_enableAAC = true;

	_pRawMP3Stream = NULL;
	_enableRawMP3 = true;

	_pMP3Stream = NULL;
	_enableMP3 = true;
}

MMSProtocol::~MMSProtocol() {
	if (_ic != (iconv_t) - 1)
		iconv_close(_ic);
	if (_pTranscoder != NULL) {
		delete _pTranscoder;
		_pTranscoder = NULL;
	}
	if (_pRawAACStream != NULL) {
		delete _pRawAACStream;
		_pRawAACStream = NULL;
	}
	if (_pAACStream != NULL) {
		delete _pAACStream;
		_pAACStream = NULL;
	}
	if (_pRawMP3Stream != NULL) {
		delete _pRawMP3Stream;
		_pRawMP3Stream = NULL;
	}
	if (_pMP3Stream != NULL) {
		delete _pMP3Stream;
		_pMP3Stream = NULL;
	}
}

bool MMSProtocol::Initialize(Variant &customparams) {
	//1. Store custom parameters
	GetCustomParameters() = customparams;

	//2. Init iconv
	_ic = iconv_open("UTF-16LE", "UTF-8");
	if (_ic == ((iconv_t) - 1)) {
		FATAL("Unable to initialize libiconv");
		return false;
	}

	//3. compute client user agent
	string host = "";
	if (customparams.HasKeyChain(V_STRING, false, 4, "customParameters",
			"externalStreamConfig", "uri", "host"))
		host = (string) customparams["customParameters"]["externalStreamConfig"]["uri"]["host"];

	if (host != "") {
		_player = format("NSPlayer/7.0.0.1956; {babac001-d7f9-3374-f085658ba1e3ba91}; Host: %s",
				STR(host));
	} else {
		_player = "NSPlayer/7.0.0.1956; {babac001-d7f9-3374-f085658ba1e3ba91}";
	}

	//4. read AAC/MP3 availability
	if (customparams.HasKeyChain(V_BOOL, false, 3, "customParameters",
			"externalStreamConfig", "enableRawAAC")) {
		_enableRawAAC = (bool) customparams["customParameters"]["externalStreamConfig"]["enableRawAAC"];
	}
	if (customparams.HasKeyChain(V_BOOL, false, 3, "customParameters",
			"externalStreamConfig", "enableAAC")) {
		_enableAAC = (bool) customparams["customParameters"]["externalStreamConfig"]["enableAAC"];
	}
	if (customparams.HasKeyChain(V_BOOL, false, 3, "customParameters",
			"externalStreamConfig", "enableRawMP3")) {
		_enableRawMP3 = (bool) customparams["customParameters"]["externalStreamConfig"]["enableRawMP3"];
	}
	if (customparams.HasKeyChain(V_BOOL, false, 3, "customParameters",
			"externalStreamConfig", "enableMP3")) {
		_enableMP3 = (bool) customparams["customParameters"]["externalStreamConfig"]["enableMP3"];
	}

	//5. read the localStreamName
	string localStreamName = "";
	if (customparams.HasKeyChain(V_STRING, false, 3, "customParameters",
			"externalStreamConfig", "localStreamName"))
		localStreamName = (string) customparams["customParameters"]["externalStreamConfig"]["localStreamName"];
	if (localStreamName == "") {
		localStreamName = format("mms_stream_%u", GetId());
	}

	//6. compute aac and mp3 streamNames
	if (_enableRawAAC) {
		if (customparams.HasKeyChain(V_STRING, false, 3, "customParameters",
				"externalStreamConfig", "localRawAACStreamName"))
			_rawAACStreamName = (string) customparams["customParameters"]["externalStreamConfig"]["localRawAACStreamName"];
		if (_rawAACStreamName == "") {
			_rawAACStreamName = localStreamName + "_raw_aac";
		}
		if (!GetApplication()->StreamNameAvailable(_rawAACStreamName)) {
			FATAL("Stream name %s already taken", STR(_rawAACStreamName));
			return false;
		}
	}
	if (_enableAAC) {
		if (customparams.HasKeyChain(V_STRING, false, 3, "customParameters",
				"externalStreamConfig", "localAACStreamName"))
			_aacStreamName = (string) customparams["customParameters"]["externalStreamConfig"]["localAACStreamName"];
		if (_aacStreamName == "") {
			_aacStreamName = localStreamName + "_aac";
		}
		if (!GetApplication()->StreamNameAvailable(_aacStreamName)) {
			FATAL("Stream name %s already taken", STR(_aacStreamName));
			return false;
		}
	}
	if (_enableRawMP3) {
		if (customparams.HasKeyChain(V_STRING, false, 3, "customParameters",
				"externalStreamConfig", "localRawMP3StreamName"))
			_rawMP3StreamName = (string) customparams["customParameters"]["externalStreamConfig"]["localRawMP3StreamName"];
		if (_rawMP3StreamName == "") {
			_rawMP3StreamName = localStreamName + "_raw_mp3";
		}
		if (!GetApplication()->StreamNameAvailable(_rawMP3StreamName)) {
			FATAL("Stream name %s already taken", STR(_rawMP3StreamName));
			return false;
		}
	}
	if (_enableMP3) {
		if (customparams.HasKeyChain(V_STRING, false, 3, "customParameters",
				"externalStreamConfig", "localMP3StreamName"))
			_mp3StreamName = (string) customparams["customParameters"]["externalStreamConfig"]["localMP3StreamName"];
		if (_mp3StreamName == "") {
			_mp3StreamName = localStreamName + "_mp3";
		}
		if (!GetApplication()->StreamNameAvailable(_mp3StreamName)) {
			FATAL("Stream name %s already taken", STR(_mp3StreamName));
			return false;
		}
	}

	return true;
}

IOBuffer * MMSProtocol::GetOutputBuffer() {
	if (GETAVAILABLEBYTESCOUNT(_outputBuffer))
		return &_outputBuffer;
	return NULL;
}

bool MMSProtocol::AllowFarProtocol(uint64_t type) {
	return type == PT_TCP;
}

bool MMSProtocol::AllowNearProtocol(uint64_t type) {
	return false;
}

bool MMSProtocol::SignalInputData(int32_t recvAmount) {
	NYIR;
}

bool MMSProtocol::SignalInputData(IOBuffer &buffer) {
	while (true) {
		//1. Do we have enough data?
		if (GETAVAILABLEBYTESCOUNT(buffer) < 8) {
			return true;
		}

		//2. Is this a message or data?
		uint32_t boobFace = *((uint32_t *) (GETIBPOINTER(buffer) + 4));
		if (boobFace == 0xb00bface) {
			//3. This is a message. Do we have enough data?
			if (GETAVAILABLEBYTESCOUNT(buffer) < 12)
				return true;

			//4. Read the length
			uint32_t length = *((uint32_t *) (GETIBPOINTER(buffer) + 8));

			//5. Do we have enough data?
			if (GETAVAILABLEBYTESCOUNT(buffer) < length + 16)
				return true;

			//6. Process the message
			if (!ProcessMessage(buffer)) {
				FATAL("ProcessMessage failed");
				return false;
			}

			//7. Discard buffer
			buffer.Ignore(length + 16);
		} else {
			//8.This is a data packet. Do we have enough data?
			if (GETAVAILABLEBYTESCOUNT(buffer) < 8) {
				return true;
			}

			//9. Read the length
			uint32_t length = *((uint16_t *) (GETIBPOINTER(buffer) + 6));

			//10. Do we have enough data?
			if (GETAVAILABLEBYTESCOUNT(buffer) < length) {
				return true;
			}

			//11. Process the stream data
			if (!ProcessData(buffer)) {
				FATAL("ProcessData failed");
				return false;
			}

			//12. Discard buffer
			buffer.Ignore(length);
		}
	}

	return true;
}

bool MMSProtocol::Start() {
	if (!SendLinkViewerToMacConnect()) {
		FATAL("SendLinkViewerToMacConnect failed");
		return false;
	}
	return true;
}

bool MMSProtocol::ProcessMessage(IOBuffer &buffer) {
	//1. Get the message type
	uint32_t messageType = *((uint32_t *) (GETIBPOINTER(buffer) + 36));
	switch (messageType) {
		case 0x00040001:
		{
			return SendLinkViewerToMacConnectFunnel();
		}
		case 0x00040002:
		{
			return SendLinkViewerToMacOpenFile();
		}
		case 0x00040006:
		{
			_openFileId = *((uint32_t *) (GETIBPOINTER(buffer) + 48));
			return SendLinkViewerToMacReadBlock();
		}
		case 0x00040011:
		case 0x00040005:
		{
			//FINEST("Expecting data now...");
			return true;
		}
		case 0x00040021:
		{
			return SendLinkViewerToMacStartPlaying();
		}
		case 0x0004001b:
		{
			return SendLinkViewerToMacPong();
		}
		default:
		{
			FATAL("Unknown message type %08x", messageType);
			return false;
		}
	}
}

bool MMSProtocol::ProcessData(IOBuffer &buffer) {
	uint8_t dataType = GETIBPOINTER(buffer)[4];
	if (dataType == 2) {
		//header
		//2. Read the length
		uint32_t length = *((uint16_t *) (GETIBPOINTER(buffer) + 6));
		//FINEST("header length: %08x", length);

		//3. Do we have enough data?
		if (GETAVAILABLEBYTESCOUNT(buffer) < length) {
			FINEST("NED");
			return true;
		}

		//4. Save the data
		_asfData.ReadFromBuffer(GETIBPOINTER(buffer) + 8, length - 8);

		uint8_t chunkType = GETIBPOINTER(buffer)[5];
		if (chunkType == 0x0c) {
			if (!ParseASFHeader()) {
				FATAL("Unable to parse ASF header");
				return false;
			}
			return SendLinkViewerToMacStreamSwitch();
		}
		return true;
	} else {
		//2. Read the length
		uint32_t length = *((uint16_t *) (GETIBPOINTER(buffer) + 6));

		//3. Do we have enough data?
		if (GETAVAILABLEBYTESCOUNT(buffer) < length) {
			return true;
		}

		_asfData.ReadFromBuffer(GETIBPOINTER(buffer) + 8, length - 8);
		if (length - 8 > _packetSize) {
			FATAL("Invalid header size");
			return false;
		}
		_asfData.ReadFromRepeat(0, _packetSize - length + 8);

		if (_pTranscoder == NULL) {
			if (_enableRawAAC) {
				_pRawAACStream = new InNetRawStream(this,
						GetApplication()->GetStreamsManager(),
						_rawAACStreamName, CODEC_AUDIO_ADTS);
			}
			if (_enableAAC) {
				_pAACStream = new InNetAACStream(this,
						GetApplication()->GetStreamsManager(),
						_aacStreamName);
			}
			if (_enableRawMP3) {
				_pRawMP3Stream = new InNetRawStream(this,
						GetApplication()->GetStreamsManager(),
						_rawMP3StreamName, CODEC_AUDIO_MP3);
			}
			if (_enableMP3) {
				_pMP3Stream = new InNetMP3Stream(this,
						GetApplication()->GetStreamsManager(),
						_mp3StreamName);
			}
			_pTranscoder = new Transcoder(_asfData);
			if (!_pTranscoder->Init(_enableRawAAC || _enableAAC, _enableRawMP3 || _enableMP3)) {
				FATAL("Unable to initialize transcoder");
				return false;
			}
			_asfData.IgnoreAll();
		}

		if (_pTranscoder == NULL) {
			return true;
		}

		if (GETAVAILABLEBYTESCOUNT(_asfData) == 0) {
			return true;
		}

		if (!_pTranscoder->PushData(
				GETIBPOINTER(_asfData),
				GETAVAILABLEBYTESCOUNT(_asfData),
				_pRawAACStream, _pAACStream, _pRawMP3Stream, _pMP3Stream)) {
			FATAL("Unable to publish data");
			return false;
		}
		//		FINEST("aac: %u; mp3: %u",
		//				GETAVAILABLEBYTESCOUNT(aac),
		//				GETAVAILABLEBYTESCOUNT(mp3));
		//		_asfFile.WriteBuffer(GETIBPOINTER(_asfData), GETAVAILABLEBYTESCOUNT(_asfData));
		//		_aacFile.WriteBuffer(GETIBPOINTER(aac), GETAVAILABLEBYTESCOUNT(aac));
		//		_mp3File.WriteBuffer(GETIBPOINTER(mp3), GETAVAILABLEBYTESCOUNT(mp3));

		_asfData.IgnoreAll();

		return true;
	}
}

bool MMSProtocol::SendLinkViewerToMacConnect() {
	_msgBuffer.ReadFromRepeat(0, 4); //playIncarnation (should be 0xf0f0f0ef (MMS_DISABLE_PACKET_PAIR)?)

	uint8_t revs[] = {
		0x0b, 0x00, 0x04, 0x00, //MacToViewerProtocolRevision
		0x1c, 0x00, 0x03, 0x00 //ViewerToMacProtocolRevision
	};
	_msgBuffer.ReadFromBuffer(revs, sizeof (revs));

	if (!EncodeUTF16(_msgBuffer, _player)) {
		FATAL("EncodeUTF16 failed");
		return false;
	}

	return SendMessage(0x00030001);
}

bool MMSProtocol::SendLinkViewerToMacConnectFunnel() {
	//playIncarnation
	_msgBuffer.ReadFromRepeat(0, 4);

	//maxBlockBytes
	_msgBuffer.ReadFromRepeat(0xff, 4);

	//maxFunnelBytes
	_msgBuffer.ReadFromRepeat(0, 4);

	//maxBitRate
	uint32_t bitRate = 0x00989680;
	_msgBuffer.ReadFromBuffer((uint8_t *) & bitRate, sizeof (bitRate));

	//funnelMode
	uint32_t funnelMode = 0x00000002;
	_msgBuffer.ReadFromBuffer((uint8_t *) & funnelMode, sizeof (funnelMode));

	//funnelName
	string funnelName = "\\\\192.168.0.1\\TCP\\1242";
	if (!EncodeUTF16(_msgBuffer, funnelName)) {
		FATAL("EncodeUTF16 failed");
		return false;
	}

	return SendMessage(0x00030002);
}

bool MMSProtocol::SendLinkViewerToMacOpenFile() {
	_msgBuffer.IgnoreAll();

	//playIncarnation
	uint32_t playIncarnation = 1;
	_msgBuffer.ReadFromBuffer((uint8_t *) & playIncarnation, sizeof (playIncarnation));

	//spare
	_msgBuffer.ReadFromRepeat(0xff, 4);

	//token
	_msgBuffer.ReadFromRepeat(0, 4);

	//cbtoken
	_msgBuffer.ReadFromRepeat(0, 4);

	//fileName
	Variant &cp = GetCustomParameters();
	string doc = "";
	if ((cp == V_MAP)
			&& (cp["customParameters"] == V_MAP)
			&& (cp["customParameters"]["externalStreamConfig"] == V_MAP)
			&& (cp["customParameters"]["externalStreamConfig"]["uri"] == V_MAP)
			&& (cp["customParameters"]["externalStreamConfig"]["uri"]["document"] == V_STRING))
		doc = (string) cp["customParameters"]["externalStreamConfig"]["uri"]["document"];
	if (doc == "") {
		FATAL("Invalid document");
		return false;
	}

	if (!EncodeUTF16(_msgBuffer, doc)) {
		FATAL("EncodeUTF16 failed");
		return false;
	}

	return SendMessage(0x00030005);
}

bool MMSProtocol::SendLinkViewerToMacReadBlock() {

	//openFileId
	_msgBuffer.ReadFromBuffer((uint8_t *) & _openFileId, sizeof (_openFileId));

	//fileBlockId
	_msgBuffer.ReadFromRepeat(0, 4);

	//offset
	_msgBuffer.ReadFromRepeat(0, 4);

	//length
	uint32_t length = 0x00008000;
	_msgBuffer.ReadFromBuffer((uint8_t *) & length, sizeof (length));

	//flags
	_msgBuffer.ReadFromRepeat(0xff, 4);

	//padding
	_msgBuffer.ReadFromRepeat(0, 4);

	//tEarliest
	_msgBuffer.ReadFromRepeat(0, 8);

	//tDeadline
	double tDeadline = 3600;
	_msgBuffer.ReadFromBuffer((uint8_t *) & tDeadline, sizeof (tDeadline));

	//playIncarnation
	uint32_t playIncarnation = 2;
	_msgBuffer.ReadFromBuffer((uint8_t *) & playIncarnation, sizeof (playIncarnation));

	//playSequence
	_msgBuffer.ReadFromRepeat(0, 4);

	return SendMessage(0x00030015);
}

bool MMSProtocol::SendLinkViewerToMacStreamSwitch() {
	//cStreamEntries
	uint32_t cStreamEntries = 1;
	_msgBuffer.ReadFromBuffer((uint8_t *) & cStreamEntries, sizeof (cStreamEntries));

	//wSrcStreamNumber
	uint16_t wSrcStreamNumber = 0xffff;
	_msgBuffer.ReadFromBuffer((uint8_t *) & wSrcStreamNumber, sizeof (wSrcStreamNumber));

	//wDstStreamNumber;
	uint16_t wDstStreamNumber = 1;
	_msgBuffer.ReadFromBuffer((uint8_t *) & wDstStreamNumber, sizeof (wDstStreamNumber));

	//wThinningLevel;;
	uint16_t wThinningLevel = 0;
	_msgBuffer.ReadFromBuffer((uint8_t *) & wThinningLevel, sizeof (wThinningLevel));

	return SendMessage(0x00030033);
}

bool MMSProtocol::SendLinkViewerToMacStartPlaying() {
	//openFileId
	_msgBuffer.ReadFromBuffer((uint8_t *) & _openFileId, sizeof (_openFileId));


	//padding
	_msgBuffer.ReadFromRepeat(0, 4);

	//position
	_msgBuffer.ReadFromRepeat(0, 8);


	//asfOffset
	_msgBuffer.ReadFromRepeat(0xff, 4);

	//locationId
	_msgBuffer.ReadFromRepeat(0xff, 4);

	//frameOffset
	_msgBuffer.ReadFromRepeat(0, 4);

	//playIncarnation
	uint32_t playIncarnation = 1;
	_msgBuffer.ReadFromBuffer((uint8_t *) & playIncarnation, sizeof (playIncarnation));

	return SendMessage(0x00030007);
}

bool MMSProtocol::SendLinkViewerToMacPong() {
	//dwParam1
	_msgBuffer.ReadFromRepeat(0x0, 4);

	//dwParam2
	_msgBuffer.ReadFromRepeat(0x0, 4);

	return SendMessage(0x0003001b);
}

bool MMSProtocol::SendMessage(uint32_t MID) {
	uint32_t msgLength = GETAVAILABLEBYTESCOUNT(_msgBuffer);
	uint32_t len8 = (msgLength + 7) / 8;
	_msgBuffer.ReadFromRepeat(0, len8 * 8 - msgLength);
	msgLength = GETAVAILABLEBYTESCOUNT(_msgBuffer);


	uint8_t fixed[] = {
		0x01, 0x00, 0x00, 0x00, //rep,ver/verm,padding
		0xce, 0xfa, 0x0b, 0xb0, //sessionId 0xb00bface
	};
	_outputBuffer.ReadFromBuffer(fixed, sizeof (fixed));

	//messageLength
	uint32_t messageLength = msgLength + 24;
	_outputBuffer.ReadFromBuffer((uint8_t *) & messageLength, sizeof (messageLength));


	//seal
	_outputBuffer.ReadFromString("MMS ");

	//chunkCount
	uint32_t chunkCount = msgLength / 8 + 3;
	_outputBuffer.ReadFromBuffer((uint8_t *) & chunkCount, sizeof (chunkCount));

	//seq
	_outputBuffer.ReadFromBuffer((uint8_t *) & _seq, sizeof (_seq));
	_seq++;

	//MBZ,timeSent
	_outputBuffer.ReadFromRepeat(0, 10);

	//chunkLen
	uint32_t chunkLen = chunkCount - 2;
	_outputBuffer.ReadFromBuffer((uint8_t *) & chunkLen, sizeof (chunkLen));

	//MID
	_outputBuffer.ReadFromBuffer((uint8_t *) & MID, sizeof (MID));

	//payload
	_outputBuffer.ReadFromBuffer(GETIBPOINTER(_msgBuffer),
			GETAVAILABLEBYTESCOUNT(_msgBuffer));

	_msgBuffer.IgnoreAll();

	//FINEST("_outputBuffer:\n%s", STR(_outputBuffer));

	return EnqueueForOutbound();
}

bool MMSProtocol::EncodeUTF16(IOBuffer &dest, string & src) {
	char *pOrigSrc = new char[src.length() + 1];
	char *pSrc = pOrigSrc;
	memset(pSrc, 0, src.length() + 1);
	memcpy(pSrc, STR(src), src.length());
	size_t srcLen = src.length();

	size_t dstLen = srcLen * 4;
	char *pOrigDst = new char[dstLen + 4];
	char *pDst = pOrigDst;

	if (iconv(_ic, &pSrc, &srcLen, &pDst, &dstLen) == (size_t) - 1) {
		FATAL("iconv failed");
		delete[] pOrigSrc;
		delete[] pOrigDst;
		return false;
	}
	*pDst++ = 0;
	*pDst++ = 0;
	int length = pDst - pOrigDst;
	dest.ReadFromBuffer((uint8_t *) pOrigDst, length);
	delete[] pOrigSrc;
	delete[] pOrigDst;
	return length != 0;
}

bool MMSProtocol::ParseASFHeader() {
	uint64_t length = GETAVAILABLEBYTESCOUNT(_asfData);
	uint8_t *pBuffer = GETIBPOINTER(_asfData);
	uint64_t cursor = 30;
	uint8_t filePropertiesGuid[] = {
		0xa1, 0xdc, 0xab, 0x8c, 0x47, 0xa9, 0xcf, 0x11,
		0x8e, 0xe4, 0x00, 0xc0, 0x0c, 0x20, 0x53, 0x65
	};
	uint64_t atomLength = 0;
	while (true) {
		if (length - cursor < 24) {
			FATAL("Not enough data");
			return false;
		}
		atomLength = *((uint64_t *) (pBuffer + cursor + 16));
		if (memcmp(pBuffer + cursor, filePropertiesGuid, 16) == 0) {
			if (atomLength < 96) {
				FATAL("Invalid ASF header");
				return false;
			}
			_packetSize = *((uint32_t *) (pBuffer + cursor + 92));
			return true;
		}
		cursor += atomLength;
		if (cursor == length)
			break;
		if (cursor > length) {
			FATAL("Invalid ASF header");
			return false;
		}
	}

	return false;
}
#endif /* HAS_PROTOCOL_MMS */
