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
#include "protocols/rtmp/header_le_ba.h"

TSPacketPMT::TSPacketPMT() {

}

TSPacketPMT::~TSPacketPMT() {
}

TSPacketPMT::operator string() {
	string result = "";
	result += format("tableId:                %hhu\n", _tableId);
	result += format("sectionSyntaxIndicator: %hhu\n", _sectionSyntaxIndicator);
	result += format("reserved1:              %hhu\n", _reserved1);
	result += format("reserved2:              %hhu\n", _reserved2);
	result += format("sectionLength:          %hu\n", _sectionLength);
	result += format("programNumber:          %hu\n", _programNumber);
	result += format("reserved3:              %hhu\n", _reserved3);
	result += format("versionNumber:          %hhu\n", _versionNumber);
	result += format("currentNextIndicator:   %hhu\n", _currentNextIndicator);
	result += format("sectionNumber:          %hhu\n", _sectionNumber);
	result += format("lastSectionNumber:      %hhu\n", _lastSectionNumber);
	result += format("reserved4:              %hhu\n", _reserved4);
	result += format("pcrPid:                 %hu\n", _pcrPid);
	result += format("reserved5:              %hhu\n", _reserved5);
	result += format("programInfoLength:      %hu\n", _programInfoLength);
	result += format("crc:                    %x\n", _crc);
	result += format("descriptors count:      %"PRIz"u\n", _programInfoDescriptors.size());
	for (uint32_t i = 0; i < _programInfoDescriptors.size(); i++) {
		result += format("\t%s", STR(_programInfoDescriptors[i]));
		if (i != _programInfoDescriptors.size() - 1)
			result += "\n";
	}
	result += format("streams count:          %"PRIz"u\n", _streams.size());

	FOR_MAP(_streams, uint16_t, TSStreamInfo, i) {
		result += format("\t%hu: %s\n", MAP_KEY(i), STR(MAP_VAL(i).toString(1)));
	}
	return result;
}

uint16_t TSPacketPMT::GetProgramNumber() {
	return _programNumber;
}

uint32_t TSPacketPMT::GetCRC() {
	return _crc;
}

map<uint16_t, TSStreamInfo> & TSPacketPMT::GetStreamsInfo() {
	return _streams;
}

bool TSPacketPMT::Read(uint8_t *pBuffer, uint32_t &cursor, uint32_t maxCursor) {
	//1. Read table id
	CHECK_BOUNDS(1);
	_tableId = pBuffer[cursor++];
	if (_tableId != 2) {
		FATAL("Invalid table id");
		return false;
	}

	//2. read section length and syntax indicator
	CHECK_BOUNDS(2);
	_sectionSyntaxIndicator = (pBuffer[cursor]&0x80) != 0;
	_reserved1 = (pBuffer[cursor]&0x40) != 0;
	_reserved2 = (pBuffer[cursor] >> 4)&0x03;
	_sectionLength = ENTOHSP((pBuffer + cursor))&0x0fff;
	cursor += 2;

	//5. Read transport stream id
	CHECK_BOUNDS(2);
	_programNumber = ENTOHSP((pBuffer + cursor));
	cursor += 2;

	//6. read current next indicator and version
	CHECK_BOUNDS(1);
	_reserved3 = pBuffer[cursor] >> 6;
	_versionNumber = (pBuffer[cursor] >> 1)&0x1f;
	_currentNextIndicator = (pBuffer[cursor]&0x01) != 0;
	cursor++;

	//7. read section number
	CHECK_BOUNDS(1);
	_sectionNumber = pBuffer[cursor++];

	//8. read last section number
	CHECK_BOUNDS(1);
	_lastSectionNumber = pBuffer[cursor++];

	//9. read PCR PID
	CHECK_BOUNDS(2);
	_pcrPid = ENTOHSP((pBuffer + cursor))&0x1fff; //----MARKED-SHORT----
	cursor += 2;

	//10. read the program info length
	CHECK_BOUNDS(2);
	_programInfoLength = ENTOHSP((pBuffer + cursor))&0x0fff; //----MARKED-SHORT----
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
	uint8_t streamsInfoLength = (uint8_t) (_sectionLength - 9 - _programInfoLength - 4);
	uint8_t streamsInfoCursor = 0;

	//14. Read the streams info
	while (streamsInfoCursor < streamsInfoLength) {
		TSStreamInfo streamInfo;

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

uint32_t TSPacketPMT::PeekCRC(uint8_t *pBuffer, uint32_t cursor, uint32_t maxCursor) {
	//2. ignore table id
	CHECK_BOUNDS(1);
	cursor++;

	//3. read section length
	CHECK_BOUNDS(2);
	uint16_t length = ENTOHSP((pBuffer + cursor))&0x0fff; //----MARKED-SHORT----
	cursor += 2;

	//4. Move to the crc position
	CHECK_BOUNDS(length - 4);
	cursor += (length - 4);

	//5. return the crc
	CHECK_BOUNDS(4);
	return ENTOHLP((pBuffer + cursor)); //----MARKED-LONG---
}

uint32_t TSPacketPMT::GetBandwidth() {
	for (uint32_t i = 0; i < _programInfoDescriptors.size(); i++) {
		if (_programInfoDescriptors[i].type == 14) {
			return _programInfoDescriptors[i].payload.maximum_bitrate_descriptor.maximum_bitrate;
		}
	}

	uint32_t result = 0;

	FOR_MAP(_streams, uint16_t, TSStreamInfo, i) {
		TSStreamInfo &si = MAP_VAL(i);
		for (uint32_t j = 0; j < si.esDescriptors.size(); j++) {
			if (si.esDescriptors[j].type == 14) {
				result += si.esDescriptors[j].payload.maximum_bitrate_descriptor.maximum_bitrate;
				break;
			}
		}
	}

	return result;
}
#endif	/* HAS_PROTOCOL_TS */

