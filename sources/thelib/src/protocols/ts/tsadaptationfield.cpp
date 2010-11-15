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
#include "protocols/ts/tsadaptationfield.h"
#include "protocols/ts/tspacketheader.h"
#include "protocols/ts/tsboundscheck.h"

TSAdaptationField::TSAdaptationField() {
	_flags.raw = 0;
	_pcr = 0;
	_pcrExt = 0;
	_pcrFinal = 0;
	_opcr = 0;
	_opcrExt = 0;
	_opcrFinal = 0;
	_spliceCountdown = 0;
	_transportPrivateDataLength = 0;
	_pTransportPrivateData = NULL;
	_adaptationFieldExtensionLength = 0;
}

TSAdaptationField::~TSAdaptationField() {
	if (_pTransportPrivateData != NULL)
		delete[] _pTransportPrivateData;
	_pTransportPrivateData = NULL;
}

TSAdaptationField::operator string() {
	string result = "";
	result += format("adaptationFieldExtension: %d\n", _flags.s.adaptationFieldExtension);
	result += format("transportPrivateData: %d\n", _flags.s.transportPrivateData);
	result += format("splicingPoint: %d\n", _flags.s.splicingPoint);
	result += format("OPCR: %d\n", _flags.s.OPCR);
	result += format("PCR: %d\n", _flags.s.PCR);
	result += format("elementaryStreamPriorityIndicator: %d\n", _flags.s.elementaryStreamPriorityIndicator);
	result += format("randomAccessIndicator: %d\n", _flags.s.randomAccessIndicator);
	result += format("discontinuityIndicator: %d", _flags.s.discontinuityIndicator);
	return result;
}

bool TSAdaptationField::Read(uint8_t *pBuffer, uint32_t &cursor, uint32_t maxCursor) {
	uint32_t adaptationFieldStart = cursor;

	//2. Read the adaptation field length
	CHECK_BOUNDS(1);
	uint32_t adaptationFieldLength = pBuffer[cursor++] + 1;

	//4. Read the adaptation field flags
	CHECK_BOUNDS(1);
	_flags.raw = pBuffer[cursor++];

	//5. read PCR
	if (_flags.s.PCR == 1) {
		if (!ReadComplexTime(pBuffer, cursor, _pcr, _pcrExt, maxCursor)) {
			FATAL("Unable to read PCR");
		}
		CHECK_BOUNDS(0);
		//TODO: take into consideration "Table 2-54 – System clock descriptor"
		//when computing syste_clock
		_pcrFinal = (_pcr * 300.00 + _pcrExt) / 27000.00;
		//        FINEST("%x PCR: %f; PCR_EXT: %f; PCR_FINAL: %f",
		//                (uint32_t) file.tellg(), _pcr, _pcrExt, _pcrFinal);
		//FINEST("---- TIMESTAMP(%d) (ms): %f", _pTSPacket->GetPID(), _pcrFinal);
	}

	//6. read OPCR
	if (_flags.s.OPCR == 1) {
		if (!ReadComplexTime(pBuffer, cursor, _opcr, _opcrExt, maxCursor)) {
			FATAL("Unable to read OPCR");
		}
		CHECK_BOUNDS(0);
		_opcrFinal = (_opcr * 300 + _opcrExt) / 27000.00;
		//        FINEST("%x OPCR: %f; OPCR_EXT: %f; OPCR_FINAL: %f",
		//                (uint32_t) file.tellg(), _opcr, _opcrExt, _opcrFinal);
		//FINEST("---- ORIGINAL TIMESTAMP(%d) (ms): %f", _pTSPacket->_header.data.pid, _opcrFinal);
	}

	//7. splicingPoint
	if (_flags.s.splicingPoint == 1) {
		CHECK_BOUNDS(1);
		_spliceCountdown = pBuffer[cursor++];
	}

	//8. transportPrivateData
	if (_flags.s.transportPrivateData == 1) {
		//        CHECK_BOUNDS(1);
		//        _transportPrivateDataLength = pBuffer[cursor++];
		//
		//        _pTransportPrivateData = new uint8_t[_transportPrivateDataLength];
		//        CHECK_BOUNDS(_transportPrivateDataLength);
		//        memcpy(_pTransportPrivateData, pBuffer + cursor, _transportPrivateDataLength);
		//        cursor += _transportPrivateDataLength;
		WARN("transportPrivateData not yet implemented");
	}

	//9. Skip the next bytes until the adaptation field end
	//Will implement them later
	cursor = (uint8_t) (adaptationFieldStart + adaptationFieldLength);
	CHECK_BOUNDS(0);

	return true;
}

double TSAdaptationField::GetTime() {
	if (_pcrFinal != 0)
		return _pcrFinal;
	return _opcrFinal;
}

bool TSAdaptationField::ReadComplexTime(uint8_t *pBuffer, uint32_t &cursor,
		double &p1, double &p2, uint32_t maxCursor) {
	CHECK_BOUNDS(4);
	uint32_t temp32 = ENTOHLP((pBuffer + cursor)); //----MARKED-LONG---
	cursor += 4;

	CHECK_BOUNDS(2);
	uint16_t temp16 = ENTOHSP((pBuffer + cursor)); //----MARKED-SHORT----
	cursor += 2;

	p1 = (double) temp32 * 2.0 + (temp16 >> 15);
	p2 = (double) (temp16 & 0x1ff);

	return true;
}
#endif	/* HAS_PROTOCOL_TS */

