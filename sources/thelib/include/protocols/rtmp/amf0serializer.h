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
#ifndef _AMF0SERIALIZER_H
#define	_AMF0SERIALIZER_H

#include "common.h"

class DLLEXP AMF0Serializer {
private:
	static vector<string> _keysOrder;
	static uint8_t _endOfObject[];
public:
	AMF0Serializer();
	virtual ~AMF0Serializer();

	bool ReadShortString(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteShortString(IOBuffer &buffer, string &value, bool writeType = true);

	bool ReadLongString(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteLongString(IOBuffer &buffer, string &value, bool writeType = true);

	bool ReadDouble(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteDouble(IOBuffer &buffer, double value, bool writeType = true);

	bool ReadObject(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteObject(IOBuffer &buffer, Variant &variant, bool writeType = true);

	bool ReadTypedObject(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteTypedObject(IOBuffer &buffer, Variant &variant, bool writeType = true);

	bool ReadMixedArray(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteMixedArray(IOBuffer &buffer, Variant &variant, bool writeType = true);

	bool ReadArray(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteArray(IOBuffer &buffer, Variant &variant, bool writeType = true);

	bool ReadAMF3Object(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteAMF3Object(IOBuffer &buffer, Variant &variant, bool writeType = true);

	bool ReadBoolean(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteBoolean(IOBuffer &buffer, bool value, bool writeType = true);

	bool ReadTimestamp(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteTimestamp(IOBuffer &buffer, Timestamp value, bool writeType = true);

	bool ReadNull(IOBuffer &buffer, Variant &variant);
	bool WriteNull(IOBuffer &buffer);

	bool ReadUndefined(IOBuffer &buffer, Variant &variant);
	bool WriteUndefined(IOBuffer &buffer);

	bool ReadUInt8(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteUInt8(IOBuffer &buffer, uint8_t value, bool writeType = true);

	bool ReadInt16(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteInt16(IOBuffer &buffer, int16_t value, bool writeType = true);

	bool ReadUInt32(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteUInt32(IOBuffer &buffer, uint32_t value, bool writeType = true);

	bool ReadInt32(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteInt32(IOBuffer &buffer, int32_t value, bool writeType = true);

	bool Read(IOBuffer &buffer, Variant &variant);
	bool Write(IOBuffer &buffer, Variant &variant);
private:
	bool IsAMF3(Variant &variant);
};


#endif	/* _AMF0SERIALIZER_H */

#endif /* HAS_PROTOCOL_RTMP */

