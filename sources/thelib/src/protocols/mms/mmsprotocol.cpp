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
#include "protocols/mms/source.h"
#include "protocols/mms/destination.h"

MMSProtocol::MMSProtocol()
: BaseProtocol(PT_OUTBOUND_MMS) {
	_file.Initialize("/tmp/test.asf", FILE_OPEN_MODE_TRUNCATE);
	_seq = 0;
	_openFileId = 0;
	_pSource = NULL;
	_pDestination = NULL;
}

MMSProtocol::~MMSProtocol() {
	if (_ic != (iconv_t) - 1)
		iconv_close(_ic);
	if (_pSource != NULL) {
		delete _pSource;
		_pSource = NULL;
	}
	if (_pDestination != NULL) {
		delete _pDestination;
		_pDestination = NULL;
	}
}

bool MMSProtocol::Initialize(Variant &parameters) {
	GetCustomParameters() = parameters;
	_ic = iconv_open("UTF-16LE", "UTF-8");
	if (_ic == ((iconv_t) - 1)) {
		FATAL("Unable to initialize libiconv");
		return false;
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
			FINEST("Expecting data now...");
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
		FINEST("header length: %08x", length);

		//3. Do we have enough data?
		if (GETAVAILABLEBYTESCOUNT(buffer) < length) {
			FINEST("NED");
			return true;
		}

		//4. Save the data
		_asfHeader.ReadFromBuffer(GETIBPOINTER(buffer) + 8,
				GETAVAILABLEBYTESCOUNT(buffer) - 8);

		uint8_t chunkType = GETIBPOINTER(buffer)[5];
		if (chunkType == 0x0c) {
			FINEST("_asfHeader: %d", GETAVAILABLEBYTESCOUNT(_asfHeader));
			_pSource = new Source(_asfHeader);
			if (!_pSource->Init()) {
				FATAL("Unable to initialize source");
				return false;
			}

			_pDestination = new Destination("", "libfaac", _pSource->GetRate());
			if (!_pDestination->Init()) {
				FATAL("Unable to initialize source");
				return false;
			}

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
		FINEST("data length: %08x %u", length, length);

		//3. Do we have enough data?
		if (GETAVAILABLEBYTESCOUNT(buffer) < length) {
			return true;
		}

		if (_pSource != NULL) {
			if (!_pSource->PushData(GETIBPOINTER(buffer) + 8, length - 8)) {
				FATAL("Unable to publish data");
				return false;
			}
			FINEST("length-8: %u", length - 8);
			uint8_t gigi[3] = {0};
			if (!_pSource->PushData(gigi, 3)) {
				FATAL("Unable to publish data");
				return false;
			}
		}

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

	string host = "";
	Variant &cp = GetCustomParameters();
	if ((cp == V_MAP)
			&& (cp["customParameters"] == V_MAP)
			&& (cp["customParameters"]["externalStreamConfig"] == V_MAP)
			&& (cp["customParameters"]["externalStreamConfig"]["uri"] == V_MAP)
			&& (cp["customParameters"]["externalStreamConfig"]["uri"]["host"] == V_STRING))
		host = (string) cp["customParameters"]["externalStreamConfig"]["uri"]["host"];
	string player;
	if (host != "")
		player = format("NSPlayer/7.0.0.1956; {babac001-d7f9-3374-f085658ba1e3ba91}; Host: %s",
			STR(host));
	else
		player = "NSPlayer/7.0.0.1956; {babac001-d7f9-3374-f085658ba1e3ba91}";

	if (!EncodeUTF16(_msgBuffer, player)) {
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
	NYI;
	return true;
}
#endif /* HAS_PROTOCOL_MMS */
