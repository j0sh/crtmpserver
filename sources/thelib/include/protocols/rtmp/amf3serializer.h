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
#ifndef _AMF3SERIALIZER_H
#define	_AMF3SERIALIZER_H

#include "common.h"

class DLLEXP AMF3Serializer {
private:
	vector<Variant> _objects;
	vector<Variant> _traits;
	vector<string> _strings;
	vector<string> _byteArrays;
public:
	AMF3Serializer();
	virtual ~AMF3Serializer();

	bool Read(IOBuffer &buffer, Variant &variant);
	bool Write(IOBuffer &buffer, Variant &variant);


	bool ReadUndefined(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteUndefined(IOBuffer &buffer);

	bool ReadNull(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteNull(IOBuffer &buffer);

	bool ReadFalse(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteFalse(IOBuffer &buffer);

	bool ReadTrue(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteTrue(IOBuffer &buffer);

	bool ReadInteger(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteInteger(IOBuffer &buffer, uint32_t value, bool writeType = true);

	bool ReadDouble(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteDouble(IOBuffer &buffer, double value, bool writeType = true);

	bool ReadString(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteString(IOBuffer &buffer, string value, bool writeType = true);

	bool ReadXMLDoc(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteXMLDoc(IOBuffer &buffer, Variant &variant, bool writeType = true);

	bool ReadDate(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteDate(IOBuffer &buffer, Timestamp value, bool writeType = true);

	bool ReadArray(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteArray(IOBuffer &buffer, Variant &variant, bool writeType = true);

	bool ReadObject(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteObject(IOBuffer &buffer, Variant &variant, bool writeType = true);

	bool ReadXML(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteXML(IOBuffer &buffer, Variant &variant, bool writeType = true);

	bool ReadByteArray(IOBuffer &buffer, Variant &variant, bool readType = true);
	bool WriteByteArray(IOBuffer &buffer, Variant &variant, bool writeType = true);

	static bool ReadU29(IOBuffer &buffer, uint32_t &value);
	static bool WriteU29(IOBuffer &buffer, uint32_t value);
};


#endif	/* _AMF3SERIALIZER_H */

#endif /* HAS_PROTOCOL_RTMP */

