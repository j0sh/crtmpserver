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


#ifndef _NALUTYPES_H
#define	_NALUTYPES_H

#include "common.h"

//iso14496-10.pdf Page 60/280
//Table 7-1 – NAL unit type codes
#define NALU_TYPE_UNDEFINED     0
#define NALU_TYPE_SLICE         1
#define NALU_TYPE_DPA           2
#define NALU_TYPE_DPB           3
#define NALU_TYPE_DPC           4
#define NALU_TYPE_IDR           5
#define NALU_TYPE_SEI           6
#define NALU_TYPE_SPS           7
#define NALU_TYPE_PPS           8
#define NALU_TYPE_PD            9
#define NALU_TYPE_EOSEQ         10
#define NALU_TYPE_EOSTREAM      11
#define NALU_TYPE_FILL          12
#define NALU_TYPE_RESERVED13    13
#define NALU_TYPE_RESERVED14    14
#define NALU_TYPE_RESERVED15    15
#define NALU_TYPE_RESERVED16    16
#define NALU_TYPE_RESERVED17    17
#define NALU_TYPE_RESERVED18    18
#define NALU_TYPE_RESERVED19    19
#define NALU_TYPE_RESERVED20    20
#define NALU_TYPE_RESERVED21    21
#define NALU_TYPE_RESERVED22    22
#define NALU_TYPE_RESERVED23    23

//RFC3984 Page 11/82
//Table 1.  Summary of NAL unit types and their payload structures
#define NALU_TYPE_STAPA         24
#define NALU_TYPE_STAPB         25
#define NALU_TYPE_MTAP16        26
#define NALU_TYPE_MTAP24        27
#define NALU_TYPE_FUA           28
#define NALU_TYPE_FUB           29
#define NALU_TYPE_RESERVED30    30
#define NALU_TYPE_RESERVED31    31

#define NALU_F(x) ((bool)((x)>>7))
#define NALU_NRI(x) ((uint8_t)(((x)>>5)&0x03))
#define NALU_TYPE(x) ((uint8_t)((x)&0x1f))

DLLEXP string NALUToString(uint8_t naluType);

#endif	/* _NALUTYPES_H */


