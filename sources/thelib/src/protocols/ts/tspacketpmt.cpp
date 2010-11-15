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


#ifdef HAS_PROTOCOL_TS
#include "protocols/ts/tspacketpmt.h"
#include "protocols/ts/tsboundscheck.h"

TSPacketPMT::TSPacketPMT() {

}

TSPacketPMT::~TSPacketPMT() {
}

Variant TSPacketPMT::GetVariant() {
	Variant result;
	result["tableId"] = _tableId;

	result["sectionLength"] = _u1.s.sectionLength;
	result["reserved2"] = _u1.s.reserved2;
	result["reserved1"] = _u1.s.reserved1;
	result["sectionSyntaxIndicator"] = _u1.s.sectionSyntaxIndicator;
	result["programNumber"] = _programNumber;
	result["currentNextIndicator"] = _u2.s.currentNextIndicator;
	result["versionNumber"] = _u2.s.versionNumber;
	result["reserved3"] = _u2.s.reserved3;
	result["sectionNumber"] = _sectionNumber;
	result["lastSectionNumber"] = _lastSectionNumber;
	result["pcrPID"] = _pcrPID;
	result["programInfoLength"] = _programInfoLength;

	FOR_MAP(_streams, uint16_t, TSStreamInfo, i) {
		string key = format("stream %d", MAP_KEY(i));
		result[key]["streamType"] = MAP_VAL(i).streamType;
		result[key]["elementaryPID"] = MAP_VAL(i).elementaryPID;
		result[key]["esInfoLength"] = MAP_VAL(i).esInfoLength;
	}

	result["crc"] = _crc;
	return result;
}

TSPacketPMT::operator string() {
	return GetVariant().ToString();
}

uint16_t TSPacketPMT::GetProgramPID() {
	return _programNumber;
}

uint32_t TSPacketPMT::GetCRC() {
	return _crc;
}

map<uint16_t, TSStreamInfo> & TSPacketPMT::GetStreamsInfo() {
	return _streams;
}

bool TSPacketPMT::Read(uint8_t *pBuffer, uint32_t &cursor, bool hasPointerField,
		uint32_t maxCursor) {
	//1. Position where we should
	if (hasPointerField) {
		CHECK_BOUNDS(1);
		CHECK_BOUNDS(pBuffer[cursor] + 1);
		cursor += pBuffer[cursor] + 1;
	}

	//2. Read table id
	CHECK_BOUNDS(1);
	_tableId = pBuffer[cursor++];

	//3. read section length and syntax indicator
	CHECK_BOUNDS(2);
	_u1.raw = ENTOHSP((pBuffer + cursor)); //----MARKED-SHORT----
	cursor += 2;

	//5. Read transport stream id
	CHECK_BOUNDS(2);
	_programNumber = ENTOHSP((pBuffer + cursor)); //----MARKED-SHORT----
	cursor += 2;

	//6. read current next indicator and version
	CHECK_BOUNDS(1);
	_u2.raw = pBuffer[cursor++];

	//7. read section number
	CHECK_BOUNDS(1);
	_sectionNumber = pBuffer[cursor++];

	//8. read last section number
	CHECK_BOUNDS(1);
	_lastSectionNumber = pBuffer[cursor++];
	//9. read PCR PID
	CHECK_BOUNDS(2);
	_pcrPID = ENTOHSP((pBuffer + cursor)); //----MARKED-SHORT----
	_pcrPID &= 0x1fff;
	cursor += 2;

	//10. read the program info length
	CHECK_BOUNDS(2);
	_programInfoLength = ENTOHSP((pBuffer + cursor)); //----MARKED-SHORT----
	_programInfoLength &= 0x0fff;
	cursor += 2;

	//11. Read the descriptors
	//TODO read the elementary stream info
	//Table 2-39 : Program and program element descriptors (page 63)
	uint32_t limit = cursor + _programInfoLength;
	while (cursor != limit) {
		StreamDescriptor descriptor;
		if (!ReadStreamDescriptor(descriptor, pBuffer, cursor, maxCursor)) {
			FATAL("Unable to read descriptor");
			return false;
		}
		CHECK_BOUNDS(0);
		ADD_VECTOR_END(_programInfoDescriptors, descriptor);
	}

	//13. Compute the streams info boundries
	uint8_t streamsInfoLength = (uint8_t) (_u1.s.sectionLength - 9 - _programInfoLength - 4);
	uint8_t streamsInfoCursor = 0;

	//14. Read the streams info
	while (streamsInfoCursor < streamsInfoLength) {
		TSStreamInfo streamInfo = {0};

		//14.1. read the stream type
		CHECK_BOUNDS(1);
		streamInfo.streamType = pBuffer[cursor++];
		streamsInfoCursor++;

		//14.2. read the elementary pid
		CHECK_BOUNDS(2);
		streamInfo.elementaryPID = ENTOHSP((pBuffer + cursor)); //----MARKED-SHORT----
		streamInfo.elementaryPID &= 0x1fff;
		cursor += 2;
		streamsInfoCursor += 2;

		//14.3. Read the elementary stream info length
		CHECK_BOUNDS(2);
		streamInfo.esInfoLength = ENTOHSP((pBuffer + cursor)); //----MARKED-SHORT----
		streamInfo.esInfoLength &= 0x0fff;
		cursor += 2;
		streamsInfoCursor += 2;

		//14.4. read the elementary stream descriptor
		limit = cursor + streamInfo.esInfoLength;
		while (cursor != limit) {
			StreamDescriptor descriptor;
			if (!ReadStreamDescriptor(descriptor, pBuffer, cursor, maxCursor)) {
				FATAL("Unable to read descriptor");
				return false;
			}
			CHECK_BOUNDS(0);
			ADD_VECTOR_END(streamInfo.esDescriptors, descriptor);
		}
		streamsInfoCursor += streamInfo.esInfoLength;

		//14.5. store the stream info
		_streams[streamInfo.elementaryPID] = streamInfo;
	}

	//15. Read the crc
	CHECK_BOUNDS(4);
	_crc = ENTOHLP((pBuffer + cursor)); //----MARKED-LONG---
	cursor += 4;

	//16. Done
	return true;
}

uint32_t TSPacketPMT::PeekCRC(uint8_t *pBuffer, uint32_t cursor,
		bool hasPointerField, uint32_t maxCursor) {
	//1. Ignore the pointer field
	if (hasPointerField) {
		CHECK_BOUNDS(1);
		CHECK_BOUNDS(pBuffer[cursor] + 1);
		cursor += pBuffer[cursor] + 1;
	}

	//2. ignore table id
	CHECK_BOUNDS(1);
	cursor++;

	//3. read section length
	CHECK_BOUNDS(2);
	uint16_t length = ENTOHSP((pBuffer + cursor)); //----MARKED-SHORT----
	length &= 0x0fff;
	cursor += 2;

	//4. Move to the crc position
	CHECK_BOUNDS(length - 4);
	cursor += (length - 4);

	//5. return the crc
	CHECK_BOUNDS(4);
	return ENTOHLP((pBuffer + cursor)); //----MARKED-LONG---
}
#endif	/* HAS_PROTOCOL_TS */

