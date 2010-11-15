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
#include "protocols/ts/tspacketpat.h"
#include "protocols/ts/tsboundscheck.h"

TSPacketPAT::TSPacketPAT() {
	_tableId = 0;
	_u1.raw = 0;
	_transportStreamId = 0;
	_u2.raw = 0;
	_sectionNumber = 0;
	_lastSectionNumber = 0;
	_entriesCount = 0;
	_crc = 0;
}

TSPacketPAT::~TSPacketPAT() {
}

TSPacketPAT::operator string() {
	string result = "";
	result += format("tableId:                %d\n", _tableId);
	result += format("sectionLength:          %d\n", _u1.s.sectionLength);
	result += format("reserved2:              %d\n", _u1.s.reserved2);
	result += format("reserved1:              %d\n", _u1.s.reserved1);
	result += format("sectionSyntaxIndicator: %d\n", _u1.s.sectionSyntaxIndicator);
	result += format("transportStreamId:      %d\n", _transportStreamId);
	result += format("currentNextIndicator:   %d\n", _u2.s.currentNextIndicator);
	result += format("versionNumber:          %d\n", _u2.s.versionNumber);
	result += format("reserved3:              %d\n", _u2.s.reserved3);
	result += format("sectionNumber:          %d\n", _sectionNumber);
	result += format("lastSectionNumber:      %d\n", _lastSectionNumber);
	result += format("entriesCount:           %d\n", _entriesCount);
	result += format("NIT count:              %d\n", _networkPids.size());
	if (_networkPids.size() > 0) {

		FOR_MAP(_networkPids, uint16_t, uint16_t, i) {
			result += format("\tNIT %d: %d\n", MAP_KEY(i), MAP_VAL(i));
		}
	}
	result += format("PMT count:              %d\n", _programPids.size());
	if (_programPids.size() > 0) {

		FOR_MAP(_programPids, uint16_t, uint16_t, i) {
			result += format("\tPMT %d: %d\n", MAP_KEY(i), MAP_VAL(i));
		}
	}
	result += format("crc:                    %08x", _crc);


	return result;
}

bool TSPacketPAT::Read(uint8_t *pBuffer, uint32_t &cursor, bool hasPointerField,
		uint32_t maxCursor) {
	//    FINEST("%02x %02x %02x %02x %02x",
	//            pBuffer[cursor],
	//            pBuffer[cursor + 1],
	//            pBuffer[cursor + 2],
	//            pBuffer[cursor + 3],
	//            pBuffer[cursor + 4]);
	//1. Position on the right place
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

	//4. Compute entries count
	_entriesCount = (_u1.s.sectionLength - 9) / 4;

	//5. Read transport stream id
	CHECK_BOUNDS(2);
	_transportStreamId = ENTOHSP((pBuffer + cursor)); //----MARKED-SHORT----
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

	//9. read the table
	for (uint32_t i = 0; i < _entriesCount; i++) {
		//9.1. read the program number
		CHECK_BOUNDS(2);
		uint16_t programNumber = ENTOHSP((pBuffer + cursor)); //----MARKED-SHORT----
		cursor += 2;

		//9.2. read the network or program map pid
		CHECK_BOUNDS(2);
		uint16_t temp = ENTOHSP((pBuffer + cursor)); //----MARKED-SHORT----
		cursor += 2;
		temp &= 0x1fff;

		if (programNumber == 0)
			_networkPids[programNumber] = temp;
		else
			_programPids[programNumber] = temp;

		//FINEST("PN: %d; ID: %d", programNumber, temp);
	}

	//10. read the CRC
	CHECK_BOUNDS(4);
	_crc = ENTOHLP((pBuffer + cursor)); //----MARKED-LONG---
	cursor += 4;

	//12. done
	return true;
}

map<uint16_t, uint16_t> &TSPacketPAT::GetPMTs() {

	return _programPids;
}

map<uint16_t, uint16_t> &TSPacketPAT::GetNITs() {

	return _networkPids;
}

uint32_t TSPacketPAT::GetCRC() {

	return _crc;
}

uint32_t TSPacketPAT::PeekCRC(uint8_t *pBuffer, uint32_t cursor,
		bool hasPointerField, uint32_t maxCursor) {
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

