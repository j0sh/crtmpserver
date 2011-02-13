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
//Table 2-25 – Program association section

class TSPacketPAT {
private:
	//fields
	uint8_t _tableId;
	bool _sectionSyntaxIndicator;
	bool _reserved1;
	uint8_t _reserved2;
	uint16_t _sectionLength;
	uint16_t _transportStreamId;
	uint8_t _reserved3;
	uint8_t _versionNumber;
	bool _currentNextIndicator;
	uint8_t _sectionNumber;
	uint8_t _lastSectionNumber;
	uint32_t _crc;

	//internal variables
	uint32_t _patStart;
	uint32_t _patLength;
	uint32_t _entriesCount;
	map<uint16_t, uint16_t> _networkPids;
	map<uint16_t, uint16_t> _programPids;
public:
	TSPacketPAT();
	virtual ~TSPacketPAT();

	operator string();

	bool Read(uint8_t *pBuffer, uint32_t &cursor, uint32_t maxCursor);

	map<uint16_t, uint16_t> &GetPMTs();
	map<uint16_t, uint16_t> &GetNITs();
	uint32_t GetCRC();

	static uint32_t PeekCRC(uint8_t *pBuffer, uint32_t cursor, uint32_t maxCursor);
};

#endif	/* _TSPACKETPAT_H */
#endif	/* HAS_PROTOCOL_TS */

