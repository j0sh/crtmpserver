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
#ifndef _TSPACKETHEADER_H
#define	_TSPACKETHEADER_H

#include "common.h"

class TSPacketHeader {
public:

	union _TSPacketHeader {
		//iso13818-1.pdf page 36/174
		//2.4.3.2	Transport Stream packet layer

		//iso13818-1.pdf page 143/174
		//Figure F.1 – Transport Stream syntax diagram

		//pid field has some reserved values. See theirs values here:
		//iso13818-1.pdf page 37/174
		//Table 2-3 – PID table

		struct {
			uint32_t syncByte : 8;
			uint32_t pid : 13;
			uint32_t transportPriority : 1;
			uint32_t payloadUnitStartIndicator : 1;
			uint32_t transportErrorIndicator : 1;
			uint32_t continuityCounter : 4;
			uint32_t hasPayload : 1;
			uint32_t hasAdaptationField : 1;
			uint32_t transportScramblingControl : 2;
		} data;
		uint8_t raw[4];
	} _header;
public:
	TSPacketHeader();
	virtual ~TSPacketHeader();

	bool Read(IOBuffer &buffer);

	Variant GetVariant();
	operator string();

	//    inline uint16_t GetPID() {
	//        return _header.data.pid;
	//    }
	bool IsPayloadStart();
	bool HasAdaptationField();
};


#endif	/* _TSPACKETHEADER_H */
#endif	/* HAS_PROTOCOL_TS */

