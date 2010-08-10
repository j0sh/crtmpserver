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
#ifndef _TSPACKETPMT_H
#define	_TSPACKETPMT_H

#include "new/common.h"
#include "protocols/ts/streamdescriptors.h"


//Table 2-29 : Stream type assignments
#define TS_STREAMTYPE_VIDEO_MPEG1     0x01
#define TS_STREAMTYPE_VIDEO_MPEG2     0x02
#define TS_STREAMTYPE_VIDEO_MPEG4     0x10
#define TS_STREAMTYPE_VIDEO_H264      0x1b
#define TS_STREAMTYPE_VIDEO_VC1       0xea
#define TS_STREAMTYPE_VIDEO_DIRAC     0xd1

#define TS_STREAMTYPE_AUDIO_MPEG1     0x03
#define TS_STREAMTYPE_AUDIO_MPEG2     0x04
#define TS_STREAMTYPE_AUDIO_AAC       0x0f
#define TS_STREAMTYPE_AUDIO_AC3       0x81
#define TS_STREAMTYPE_AUDIO_DTS       0x8a
#define TS_STREAMTYPE_UNDEFINED       0x8a

//2.4.4.8

typedef struct _TSStreamInfo {
	uint8_t streamType; //Table 2-29 : Stream type assignments
	uint16_t elementaryPID;
	uint16_t esInfoLength;
	vector<StreamDescriptor> esDescriptors;
} TSStreamInfo;

class TSPacketPMT {
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

	uint16_t _programNumber;

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
	uint16_t _pcrPID;
	uint16_t _programInfoLength;
	vector<StreamDescriptor> _programInfoDescriptors;
	map<uint16_t, TSStreamInfo> _streams;
	uint32_t _crc;
public:
	TSPacketPMT();
	virtual ~TSPacketPMT();

	Variant GetVariant();
	operator string();

	uint16_t GetProgramPID();
	uint32_t GetCRC();
	map<uint16_t, TSStreamInfo> & GetStreamsInfo();

	bool Read(uint8_t *pBuffer, uint32_t &cursor, bool hasPointerField, uint32_t maxCursor);
	static uint32_t PeekCRC(uint8_t *pBuffer, uint32_t cursor, bool hasPointerField, uint32_t maxCursor);
};


#endif	/* _TSPACKETPMT_H */
#endif	/* HAS_PROTOCOL_TS */

