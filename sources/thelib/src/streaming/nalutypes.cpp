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



#include "streaming/nalutypes.h"

string NALUToString(uint8_t naluType) {
	switch (NALU_TYPE(naluType)) {
		case NALU_TYPE_SLICE: return "SLICE";
		case NALU_TYPE_DPA: return "DPA";
		case NALU_TYPE_DPB: return "DPB";
		case NALU_TYPE_DPC: return "DPC";
		case NALU_TYPE_IDR: return "IDR";
		case NALU_TYPE_SEI: return "SEI";
		case NALU_TYPE_SPS: return "SPS";
		case NALU_TYPE_PPS: return "PPS";
		case NALU_TYPE_PD: return "PD";
		case NALU_TYPE_EOSEQ: return "EOSEQ";
		case NALU_TYPE_EOSTREAM: return "EOSTREAM";
		case NALU_TYPE_FILL: return "FILL";
		case NALU_TYPE_RESERVED13: return "RESERVED13";
		case NALU_TYPE_RESERVED14: return "RESERVED14";
		case NALU_TYPE_RESERVED15: return "RESERVED15";
		case NALU_TYPE_RESERVED16: return "RESERVED16";
		case NALU_TYPE_RESERVED17: return "RESERVED17";
		case NALU_TYPE_RESERVED18: return "RESERVED18";
		case NALU_TYPE_RESERVED19: return "RESERVED19";
		case NALU_TYPE_RESERVED20: return "RESERVED20";
		case NALU_TYPE_RESERVED21: return "RESERVED21";
		case NALU_TYPE_RESERVED22: return "RESERVED22";
		case NALU_TYPE_RESERVED23: return "RESERVED23";
		case NALU_TYPE_STAPA: return "STAPA";
		case NALU_TYPE_STAPB: return "STAPB";
		case NALU_TYPE_MTAP16: return "MTAP16";
		case NALU_TYPE_MTAP24: return "MTAP24";
		case NALU_TYPE_FUA: return "FUA";
		case NALU_TYPE_FUB: return "FUB";
		case NALU_TYPE_RESERVED30: return "RESERVED30";
		case NALU_TYPE_RESERVED31: return "RESERVED31";
		case NALU_TYPE_UNDEFINED:
		default:
			return "UNDEFINED";
	}
}

