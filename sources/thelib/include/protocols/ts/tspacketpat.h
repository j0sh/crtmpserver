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
#ifndef _TSPACKETPAT_H
#define	_TSPACKETPAT_H

#include "common.h"

//iso13818-1.pdf page 61/174
//2.4.4.3	Program association Table

class TSPacketPAT {
private:
	uint8_t _tableId;

	union {

		struct {
			uint32_t sectionLength : 12;
			uint32_t reserved2 : 2;
			uint32_t reserved1 : 1;
			uint32_t sectionSyntaxIndicator : 1;
		} s;
		uint16_t raw;
	} _u1;
	uint16_t _transportStreamId;

	union {

		struct {
			uint32_t currentNextIndicator : 1;
			uint32_t versionNumber : 5;
			uint32_t reserved3 : 2;
		} s;
		uint8_t raw;
	} _u2;
	uint8_t _sectionNumber;
	uint8_t _lastSectionNumber;

	uint32_t _patStart;
	uint32_t _patLength;
	uint32_t _entriesCount;

	map<uint16_t, uint16_t> _networkPids;
	map<uint16_t, uint16_t> _programPids;

	uint32_t _crc;

public:
	TSPacketPAT();
	virtual ~TSPacketPAT();

	operator string();

	bool Read(uint8_t *pBuffer, uint32_t &cursor, bool hasPointerField, uint32_t maxCursor);

	map<uint16_t, uint16_t> &GetPMTs();
	map<uint16_t, uint16_t> &GetNITs();
	uint32_t GetCRC();

	static uint32_t PeekCRC(uint8_t *pBuffer, uint32_t cursor,
			bool hasPointerField, uint32_t maxCursor);
};

#endif	/* _TSPACKETPAT_H */
#endif	/* HAS_PROTOCOL_TS */

