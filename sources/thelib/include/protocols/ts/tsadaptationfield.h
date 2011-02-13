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
#ifndef _TSADAPTATIONFIELD_H
#define	_TSADAPTATIONFIELD_H

#include "common.h"

//class DLLEXP TSAdaptationField {
//private:
//	//Table 2-6 : Transport Stream adaptation field
//
//	union {
//
//		struct {
//			uint32_t adaptationFieldExtension : 1;
//			uint32_t transportPrivateData : 1;
//			uint32_t splicingPoint : 1;
//			uint32_t OPCR : 1;
//			uint32_t PCR : 1;
//			uint32_t elementaryStreamPriorityIndicator : 1;
//			uint32_t randomAccessIndicator : 1;
//			uint32_t discontinuityIndicator : 1;
//		} s;
//		uint8_t raw;
//	} _flags;
//	double _pcr;
//	double _pcrExt;
//	double _pcrFinal;
//	double _opcr;
//	double _opcrExt;
//	double _opcrFinal;
//	uint8_t _spliceCountdown;
//	uint8_t _transportPrivateDataLength;
//	uint8_t *_pTransportPrivateData;
//	uint8_t _adaptationFieldExtensionLength;
//public:
//	TSAdaptationField();
//	virtual ~TSAdaptationField();
//
//	operator string();
//
//	bool Read(uint8_t *pBuffer, uint32_t &cursor, uint32_t maxCursor);
//	double GetTime();
//private:
//	bool ReadComplexTime(uint8_t *pBuffer, uint32_t &cursor, double &p1,
//			double &p2, uint32_t maxCursor);
//};

#endif	/* _TSADAPTATIONFIELD_H */
#endif	/* HAS_PROTOCOL_TS */

