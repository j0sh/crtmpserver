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

#include "common.h"
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

	_TSStreamInfo() {
		streamType = 0;
		elementaryPID = 0;
		esInfoLength = 0;
	}

	string toString(int32_t indent) {
		string result = format("%sstreamType: %hhx; elementaryPID: %hu; esInfoLength: %hu; descriptors count: %"PRIz"u\n",
				STR(string(indent, '\t')),
				streamType, elementaryPID, esInfoLength, esDescriptors.size());
		for (uint32_t i = 0; i < esDescriptors.size(); i++) {
			result += format("%s%s", STR(string(indent + 1, '\t')), STR(esDescriptors[i]));
			if (i != esDescriptors.size() - 1)
				result += "\n";
		}
		return result;
	}
} TSStreamInfo;

//iso13818-1.pdf page 64/174
//Table 2-28 ‚Äö√Ñ√¨ Transport Stream program map section

class TSPacketPMT {
private:
	//fields
	uint8_t _tableId;
	bool _sectionSyntaxIndicator;
	bool _reserved1;
	uint8_t _reserved2;
	uint16_t _sectionLength;
	uint16_t _programNumber;
	uint8_t _reserved3;
	uint8_t _versionNumber;
	bool _currentNextIndicator;
	uint8_t _sectionNumber;
	uint8_t _lastSectionNumber;
	uint8_t _reserved4;
	uint16_t _pcrPid;
	uint8_t _reserved5;
	uint16_t _programInfoLength;
	uint32_t _crc;

	//internal variables
	vector<StreamDescriptor> _programInfoDescriptors;
	map<uint16_t, TSStreamInfo> _streams;
public:
	TSPacketPMT();
	virtual ~TSPacketPMT();

	operator string();

	uint16_t GetProgramNumber();
	uint32_t GetCRC();
	map<uint16_t, TSStreamInfo> & GetStreamsInfo();

	bool Read(uint8_t *pBuffer, uint32_t &cursor, uint32_t maxCursor);
	static uint32_t PeekCRC(uint8_t *pBuffer, uint32_t cursor, uint32_t maxCursor);
	uint32_t GetBandwidth();
};


#endif	/* _TSPACKETPMT_H */
#endif	/* HAS_PROTOCOL_TS */

