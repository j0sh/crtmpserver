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


#ifdef HAS_PROTOCOL_RTMP
#ifndef _AMFTYPES_H
#define	_AMFTYPES_H

/*AMF0_P_Number = 0x00,
	AMF0_P_Boolean = 0x01,
	AMF0_P_String = 0x02,
	AMF0_P_Object = 0x03,
	AMF0_P_MovieClip = 0x04,
	AMF0_P_Null = 0x05,
	AMF0_P_Undefined = 0x06,
	AMF0_P_Reference = 0x07,
	AMF0_P_MixedArray = 0x08,
	AMF0_P_EndOfObject = 0x09,
	AMF0_P_Array = 0x0a,
	AMF0_P_Date = 0x0b,
	AMF0_P_LongString = 0x0c,
	AMF0_P_Unsupported = 0x0d,
	AMF0_P_Recordset = 0x0e,
	AMF0_P_XML = 0x0f,
	AMF0_P_TypedObject = 0x10,
	AMF0_P_AMF3_data = 0x11,
	AMF0_P_NS_Int16 = 0xf0,
	AMF0_P_NS_Int32 = 0xf1,
	AMF0_P_uninitialized = 0xff*/

#define AMF0_NUMBER         0x00
#define AMF0_BOOLEAN        0x01
#define AMF0_SHORT_STRING   0x02
#define AMF0_OBJECT         0x03
#define AMF0_NULL           0x05
#define AMF0_UNDEFINED      0x06
#define AMF0_MIXED_ARRAY    0x08
#define AMF0_ARRAY          0x0a
#define AMF0_TIMESTAMP      0x0b
#define AMF0_LONG_STRING    0x0c
#define AMF0_TYPED_OBJECT	0x10
#define AMF0_AMF3_OBJECT    0x11

#define AMF3_UNDEFINED      0x00
#define AMF3_NULL           0x01
#define AMF3_FALSE          0x02
#define AMF3_TRUE           0x03
#define AMF3_INTEGER        0x04
#define AMF3_DOUBLE         0x05
#define AMF3_STRING         0x06
#define AMF3_XMLDOC         0x07
#define AMF3_DATE           0x08
#define AMF3_ARRAY          0x09
#define AMF3_OBJECT         0x0a
#define AMF3_XML            0x0b
#define AMF3_BYTEARRAY      0x0c

#define AMF_CHECK_BOUNDARIES(x,y) \
do { \
    if(GETAVAILABLEBYTESCOUNT(x)<(y)) { \
        FATAL("Not enough data. Wanted: %u; Got: %u", \
            (uint32_t)(y),GETAVAILABLEBYTESCOUNT(x)); \
        return false; \
    } \
}while(0)

#endif	/* _AMFTYPES_H */

#endif /* HAS_PROTOCOL_RTMP */

