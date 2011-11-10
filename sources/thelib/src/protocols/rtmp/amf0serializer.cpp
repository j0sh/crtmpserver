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
#include "protocols/rtmp/amf0serializer.h"
#include "protocols/rtmp/amf3serializer.h"
#include "protocols/rtmp/amftypes.h"

vector<string> AMF0Serializer::_keysOrder;
uint8_t AMF0Serializer::_endOfObject[] = {0, 0, 9};

AMF0Serializer::AMF0Serializer() {
	if (_keysOrder.size() <= 0) {
		ADD_VECTOR_END(_keysOrder, "app");
		ADD_VECTOR_END(_keysOrder, "flashVer");
		ADD_VECTOR_END(_keysOrder, "fmsVer");
		ADD_VECTOR_END(_keysOrder, "swfUrl");
		ADD_VECTOR_END(_keysOrder, "tcUrl");
		ADD_VECTOR_END(_keysOrder, "fpad");
		ADD_VECTOR_END(_keysOrder, "capabilities");
		ADD_VECTOR_END(_keysOrder, "audioCodecs");
		ADD_VECTOR_END(_keysOrder, "videoCodecs");
		ADD_VECTOR_END(_keysOrder, "videoFunction");
		ADD_VECTOR_END(_keysOrder, "pageUrl");

		ADD_VECTOR_END(_keysOrder, "level");
		ADD_VECTOR_END(_keysOrder, "code");
		ADD_VECTOR_END(_keysOrder, "description");
		ADD_VECTOR_END(_keysOrder, "details");
		ADD_VECTOR_END(_keysOrder, "clientid");
		ADD_VECTOR_END(_keysOrder, "duration");
		ADD_VECTOR_END(_keysOrder, "width");
		ADD_VECTOR_END(_keysOrder, "height");
		ADD_VECTOR_END(_keysOrder, "videorate");
		ADD_VECTOR_END(_keysOrder, "framerate");
		ADD_VECTOR_END(_keysOrder, "videocodecid");
		ADD_VECTOR_END(_keysOrder, "audiorate");
		ADD_VECTOR_END(_keysOrder, "audiorate");
		ADD_VECTOR_END(_keysOrder, "audiodelay");
		ADD_VECTOR_END(_keysOrder, "audiocodecid");
		ADD_VECTOR_END(_keysOrder, "canSeekToEnd");
		ADD_VECTOR_END(_keysOrder, "creationdate");
	}
}

AMF0Serializer::~AMF0Serializer() {

}

bool AMF0Serializer::ReadShortString(IOBuffer &buffer, Variant &variant,
		bool readType) {
	if (readType) {
		AMF_CHECK_BOUNDARIES(buffer, 1);
		if (GETIBPOINTER(buffer)[0] != AMF0_SHORT_STRING) {
			FATAL("AMF type not valid: want: %"PRIu8"; got: %"PRIu8,
					AMF0_SHORT_STRING, GETIBPOINTER(buffer)[0]);
			return false;
		}

		if (!buffer.Ignore(1)) {
			FATAL("Unable to ignore 1 bytes");
			return false;
		}
	}

	AMF_CHECK_BOUNDARIES(buffer, 2);
	uint16_t length = ENTOHSP(GETIBPOINTER(buffer)); //----MARKED-SHORT----
	if (!buffer.Ignore(2)) {
		FATAL("Unable to ignore 2 bytes");
		return false;
	}
	AMF_CHECK_BOUNDARIES(buffer, length);
	variant = string((char *) (GETIBPOINTER(buffer)), length);
	if (!buffer.Ignore(length)) {
		FATAL("Unable to ignore %"PRIu16" bytes", length);
		return false;
	}
	return true;
}

bool AMF0Serializer::WriteShortString(IOBuffer &buffer, string &value, bool writeType) {
	if (writeType)
		buffer.ReadFromRepeat(AMF0_SHORT_STRING, 1);

	uint16_t length = EHTONS((uint16_t) value.length()); //----MARKED-SHORT----

	buffer.ReadFromBuffer((uint8_t *) & length, 2);
	buffer.ReadFromString(value);

	return true;
}

bool AMF0Serializer::ReadLongString(IOBuffer &buffer, Variant &variant, bool readType) {
	if (readType) {
		AMF_CHECK_BOUNDARIES(buffer, 1);
		if (GETIBPOINTER(buffer)[0] != AMF0_LONG_STRING) {
			FATAL("AMF type not valid: want: %"PRIu8"; got: %"PRIu8,
					AMF0_LONG_STRING, GETIBPOINTER(buffer)[0]);
			return false;
		}

		if (!buffer.Ignore(1)) {
			FATAL("Unable to ignore 1 bytes");
			return false;
		}
	}

	AMF_CHECK_BOUNDARIES(buffer, 4);
	uint32_t length = ENTOHLP(GETIBPOINTER(buffer)); //----MARKED-LONG---
	if (!buffer.Ignore(4)) {
		FATAL("Unable to ignore 4 bytes");
		return false;
	}

	AMF_CHECK_BOUNDARIES(buffer, length);
	variant = string((char *) (GETIBPOINTER(buffer)), length);
	if (!buffer.Ignore(length)) {
		FATAL("Unable to ignore %"PRIu32" bytes", length);
		return false;
	}
	return true;
}

bool AMF0Serializer::WriteLongString(IOBuffer &buffer, string &value, bool writeType) {
	if (writeType)
		buffer.ReadFromRepeat(AMF0_LONG_STRING, 1);

	uint32_t length = EHTONL((uint32_t) value.length());

	buffer.ReadFromBuffer((uint8_t *) & length, 4);
	buffer.ReadFromString(value);

	return true;
}

bool AMF0Serializer::ReadDouble(IOBuffer &buffer, Variant &variant,
		bool readType) {
	if (readType) {
		AMF_CHECK_BOUNDARIES(buffer, 1);
		if (GETIBPOINTER(buffer)[0] != AMF0_NUMBER) {
			FATAL("AMF type not valid: want: %"PRIu8"; got: %"PRIu8,
					AMF0_NUMBER, GETIBPOINTER(buffer)[0]);
			return false;
		}

		if (!buffer.Ignore(1)) {
			FATAL("Unable to ignore 1 bytes");
			return false;
		}
	}

	AMF_CHECK_BOUNDARIES(buffer, 8);
	double temp = 0;
	ENTOHDP(GETIBPOINTER(buffer), temp);
	variant = (double) temp;

	if (!buffer.Ignore(8)) {
		FATAL("Unable to ignore 8 bytes");
		return false;
	}
	return true;
}

bool AMF0Serializer::WriteDouble(IOBuffer &buffer, double value, bool writeType) {
	if (writeType)
		buffer.ReadFromRepeat(AMF0_NUMBER, 1);

	uint64_t temp = 0;
	EHTOND(value, temp);
	buffer.ReadFromBuffer((uint8_t *) & temp, 8);

	return true;
}

bool AMF0Serializer::ReadObject(IOBuffer &buffer, Variant &variant,
		bool readType) {
	if (readType) {
		AMF_CHECK_BOUNDARIES(buffer, 1);
		if (GETIBPOINTER(buffer)[0] != AMF0_OBJECT) {
			FATAL("AMF type not valid: want: %"PRIu8"; got: %"PRIu8,
					AMF0_OBJECT, GETIBPOINTER(buffer)[0]);
			return false;
		}

		if (!buffer.Ignore(1)) {
			FATAL("Unable to ignore 1 bytes");
			return false;
		}
	}

	AMF_CHECK_BOUNDARIES(buffer, 3);
	while (!((GETIBPOINTER(buffer)[0] == 0) && (GETIBPOINTER(buffer)[1] == 0) && (GETIBPOINTER(buffer)[2] == 9))) {
		Variant key;
		Variant value;
		if (!ReadShortString(buffer, key, false)) {
			FATAL("Unable to read key");
			return false;
		}
		if (!Read(buffer, value)) {
			FATAL("Unable to read value");
			return false;
		}
		variant[key] = value;
	}

	AMF_CHECK_BOUNDARIES(buffer, 3);
	if (!buffer.Ignore(3)) {
		FATAL("Unable to ignore 3 bytes");
		return false;
	}

	variant.IsArray(false);

	return true;
}

bool AMF0Serializer::WriteObject(IOBuffer &buffer, Variant &variant,
		bool writeType) {

	if (writeType)
		buffer.ReadFromRepeat(AMF0_OBJECT, 1);

	Variant temp = variant;

	FOR_VECTOR_ITERATOR(string, _keysOrder, i) {
		if (temp.HasKey(VECTOR_VAL(i))) {
			if (!WriteShortString(buffer, VECTOR_VAL(i), false)) {
				FATAL("Unable to serialize key");
				return false;
			}
			if (!Write(buffer, temp[VECTOR_VAL(i)])) {
				FATAL("Unable to serialize value");
				return false;
			}
			temp.RemoveKey(VECTOR_VAL(i));
		}
	}

	FOR_MAP(temp, string, Variant, i) {
		string key = MAP_KEY(i);
		if (key.find(VAR_INDEX_VALUE) == 0) {
			key = key.substr(VAR_INDEX_VALUE_LEN);
		}
		if (!WriteShortString(buffer, key, false)) {
			FATAL("Unable to serialize key");
			return false;
		}
		if (!Write(buffer, MAP_VAL(i))) {
			FATAL("Unable to serialize value");
			return false;
		}
	}

	buffer.ReadFromBuffer(_endOfObject, 3);

	return true;
}

bool AMF0Serializer::ReadTypedObject(IOBuffer &buffer, Variant &variant, bool readType) {
	NYIR;
}

bool AMF0Serializer::WriteTypedObject(IOBuffer &buffer, Variant &variant, bool writeType) {
	if (writeType)
		buffer.ReadFromRepeat(AMF0_TYPED_OBJECT, 1);
	string typeName = variant.GetTypeName();
	if (!WriteShortString(buffer, typeName, false)) {
		FATAL("Unable to write the type name");
		return false;
	}
	return WriteObject(buffer, variant, false);
}

bool AMF0Serializer::ReadMixedArray(IOBuffer &buffer, Variant &variant,
		bool readType) {
	if (readType) {
		AMF_CHECK_BOUNDARIES(buffer, 1);
		if (GETIBPOINTER(buffer)[0] != AMF0_MIXED_ARRAY) {
			FATAL("AMF type not valid: want: %"PRIu8"; got: %"PRIu8,
					AMF0_MIXED_ARRAY, GETIBPOINTER(buffer)[0]);
			return false;
		}

		if (!buffer.Ignore(1)) {
			FATAL("Unable to ignore 1 bytes");
			return false;
		}
	}

	AMF_CHECK_BOUNDARIES(buffer, 4);
	uint32_t length = ENTOHLP(GETIBPOINTER(buffer)); //----MARKED-LONG---
	if (!buffer.Ignore(4)) {
		FATAL("Unable to ignore 4 bytes");
		return false;
	}

	for (uint32_t i = 0; i < length; i++) {
		Variant key;
		Variant value;
		if (!ReadShortString(buffer, key, false)) {
			FATAL("Unable to read key");
			return false;
		}
		if (!Read(buffer, value)) {
			FATAL("Unable to read value");
			return false;
		}

		if (isNumeric(key))
			variant[(uint32_t) atoi(STR(key))] = value;
		else
			variant[key] = value;
	}

	AMF_CHECK_BOUNDARIES(buffer, 3);
	while (!((GETIBPOINTER(buffer)[0] == 0) && (GETIBPOINTER(buffer)[1] == 0) && (GETIBPOINTER(buffer)[2] == 9))) {
		Variant key;
		Variant value;
		if (!ReadShortString(buffer, key, false)) {
			FATAL("Unable to read key");
			return false;
		}
		if (!Read(buffer, value)) {
			FATAL("Unable to read value");
			return false;
		}
		variant[key] = value;
	}

	AMF_CHECK_BOUNDARIES(buffer, 3);
	if (!buffer.Ignore(3)) {
		FATAL("Unable to ignore 3 bytes");
		return false;
	}

	variant.IsArray(true);


	return true;
}

bool AMF0Serializer::WriteMixedArray(IOBuffer &buffer, Variant &variant,
		bool writeType) {
	if (writeType)
		buffer.ReadFromRepeat(AMF0_MIXED_ARRAY, 1);

	Variant temp = variant;

	if (!WriteUInt32(buffer, temp.MapSize(), false)) {
		FATAL("Unable to serialize keys count");
		return false;
	}

	FOR_VECTOR_ITERATOR(string, _keysOrder, i) {
		if (temp.HasKey(VECTOR_VAL(i))) {
			if (!WriteShortString(buffer, VECTOR_VAL(i), false)) {
				FATAL("Unable to serialize key");
				return false;
			}
			if (!Write(buffer, temp[VECTOR_VAL(i)])) {
				FATAL("Unable to serialize value");
				return false;
			}
			temp.RemoveKey(VECTOR_VAL(i));
		}
	}

	FOR_MAP(temp, string, Variant, i) {
		string key = MAP_KEY(i);
		if (key.find(VAR_INDEX_VALUE) == 0) {
			key = key.substr(VAR_INDEX_VALUE_LEN);
		}
		if (!WriteShortString(buffer, key, false)) {
			FATAL("Unable to serialize key");
			return false;
		}
		if (!Write(buffer, MAP_VAL(i))) {
			FATAL("Unable to serialize value");
			return false;
		}
	}

	buffer.ReadFromBuffer(_endOfObject, 3);

	return true;
}

bool AMF0Serializer::ReadArray(IOBuffer &buffer, Variant &variant, bool readType) {
	if (readType) {
		AMF_CHECK_BOUNDARIES(buffer, 1);
		if (GETIBPOINTER(buffer)[0] != AMF0_ARRAY) {
			FATAL("AMF type not valid: want: %"PRIu8"; got: %"PRIu8,
					AMF0_ARRAY, GETIBPOINTER(buffer)[0]);
			return false;
		}

		if (!buffer.Ignore(1)) {
			FATAL("Unable to ignore 1 bytes");
			return false;
		}
	}

	AMF_CHECK_BOUNDARIES(buffer, 4);
	uint32_t length = ENTOHLP(GETIBPOINTER(buffer)); //----MARKED-LONG---
	if (!buffer.Ignore(4)) {
		FATAL("Unable to ignore 4 bytes");
		return false;
	}

	for (uint32_t i = 0; i < length; i++) {
		Variant value;
		if (!Read(buffer, value)) {
			FATAL("Unable to read value");
			return false;
		}
		variant[i] = value;
	}

	variant.IsArray(true);


	return true;
}

bool AMF0Serializer::WriteArray(IOBuffer &buffer, Variant &variant, bool writeType) {
	NYIR;
}

bool AMF0Serializer::ReadAMF3Object(IOBuffer &buffer, Variant &variant,
		bool readType) {
	if (readType) {
		AMF_CHECK_BOUNDARIES(buffer, 1);
		if (GETIBPOINTER(buffer)[0] != AMF0_AMF3_OBJECT) {
			FATAL("AMF type not valid: want: %"PRIu8"; got: %"PRIu8,
					AMF0_AMF3_OBJECT, GETIBPOINTER(buffer)[0]);
			return false;
		}

		if (!buffer.Ignore(1)) {
			FATAL("Unable to ignore 1 bytes");
			return false;
		}
	}

	AMF3Serializer amf3;

	if (!amf3.Read(buffer, variant))
		return false;

	return true;
}

bool AMF0Serializer::WriteAMF3Object(IOBuffer &buffer, Variant &variant,
		bool writeType) {
	if (writeType)
		buffer.ReadFromRepeat(AMF0_AMF3_OBJECT, 1);

	AMF3Serializer amf3;
	if (!amf3.Write(buffer, variant))
		return false;

	return true;
}

bool AMF0Serializer::ReadBoolean(IOBuffer &buffer, Variant &variant,
		bool readType) {
	if (readType) {
		AMF_CHECK_BOUNDARIES(buffer, 1);
		if (GETIBPOINTER(buffer)[0] != AMF0_BOOLEAN) {
			FATAL("AMF type not valid: want: %"PRIu8"; got: %"PRIu8,
					AMF0_BOOLEAN, GETIBPOINTER(buffer)[0]);
			return false;
		}

		if (!buffer.Ignore(1)) {
			FATAL("Unable to ignore 1 bytes");
			return false;
		}
	}

	AMF_CHECK_BOUNDARIES(buffer, 1);
	variant = (bool)(GETIBPOINTER(buffer)[0] == 1);

	if (!buffer.Ignore(1)) {
		FATAL("Unable to ignore 1 bytes");
		return false;
	}

	return true;
}

bool AMF0Serializer::WriteBoolean(IOBuffer &buffer, bool value, bool writeType) {
	if (writeType)
		buffer.ReadFromRepeat(AMF0_BOOLEAN, 1);

	if (value)
		buffer.ReadFromByte((uint8_t) 1);
	else
		buffer.ReadFromByte((uint8_t) 0);

	return true;
}

bool AMF0Serializer::ReadTimestamp(IOBuffer &buffer, Variant &variant, bool readType) {
	if (readType) {
		AMF_CHECK_BOUNDARIES(buffer, 1);
		if (GETIBPOINTER(buffer)[0] != AMF0_TIMESTAMP) {
			FATAL("AMF type not valid: want: %"PRIu8"; got: %"PRIu8,
					AMF0_TIMESTAMP, GETIBPOINTER(buffer)[0]);
			return false;
		}

		if (!buffer.Ignore(1)) {
			FATAL("Unable to ignore 1 bytes");
			return false;
		}
	}

	Variant doubleVariant;
	if (!ReadDouble(buffer, doubleVariant, false)) {
		FATAL("Unable to read the value");
		return false;
	}

	time_t timeVal = (time_t) (((double) doubleVariant) / 1000);
	Timestamp ts = *(gmtime(&timeVal));

	variant = Variant(ts);

	AMF_CHECK_BOUNDARIES(buffer, 2);
	if (!buffer.Ignore(2)) {
		FATAL("Unable to ignore 2 bytes");
		return false;
	}

	return true;
}

bool AMF0Serializer::WriteTimestamp(IOBuffer &buffer, Timestamp value, bool writeType) {
	if (writeType)
		buffer.ReadFromRepeat(AMF0_TIMESTAMP, 1);
	double seconds = (double) timegm(&value)*1000.00;
	if (!WriteDouble(buffer, seconds, false)) {
		FATAL("Unable to write double");
		return false;
	}
	buffer.ReadFromRepeat(0, 2);
	return true;
}

bool AMF0Serializer::ReadNull(IOBuffer &buffer, Variant &variant) {
	AMF_CHECK_BOUNDARIES(buffer, 1);
	if (GETIBPOINTER(buffer)[0] != AMF0_NULL) {
		FATAL("AMF type not valid: want: %"PRIu8"; got: %"PRIu8,
				AMF0_NULL, GETIBPOINTER(buffer)[0]);
		return false;
	}

	if (!buffer.Ignore(1)) {
		FATAL("Unable to ignore 1 bytes");
		return false;
	}

	variant.Reset();

	return true;
}

bool AMF0Serializer::WriteNull(IOBuffer &buffer) {
	buffer.ReadFromRepeat(AMF0_NULL, 1);
	return true;
}

bool AMF0Serializer::ReadUndefined(IOBuffer &buffer, Variant &variant) {
	AMF_CHECK_BOUNDARIES(buffer, 1);
	if (GETIBPOINTER(buffer)[0] != AMF0_UNDEFINED) {
		FATAL("AMF type not valid: want: %"PRIu8"; got: %"PRIu8,
				AMF0_UNDEFINED, GETIBPOINTER(buffer)[0]);
		return false;
	}

	if (!buffer.Ignore(1)) {
		FATAL("Unable to ignore 1 bytes");
		return false;
	}

	variant.Reset();

	return true;
}

bool AMF0Serializer::WriteUndefined(IOBuffer &buffer) {
	buffer.ReadFromRepeat(AMF0_UNDEFINED, 1);
	return true;
}

bool AMF0Serializer::ReadUInt8(IOBuffer &buffer, Variant &variant, bool readType) {
	if (readType) {
		NYIR;
	}
	AMF_CHECK_BOUNDARIES(buffer, 1);
	uint8_t temp = GETIBPOINTER(buffer)[0];
	variant = Variant((uint8_t) temp);
	return buffer.Ignore(1);
}

bool AMF0Serializer::WriteUInt8(IOBuffer &buffer, uint8_t value, bool writeType) {
	if (writeType) {
		NYIR;
	}
	buffer.ReadFromByte(value);
	return true;
}

bool AMF0Serializer::ReadInt16(IOBuffer &buffer, Variant &variant, bool readType) {
	if (readType) {
		NYIR;
	}
	AMF_CHECK_BOUNDARIES(buffer, 2);
	int16_t temp = (GETIBPOINTER(buffer)[0] << 8) | GETIBPOINTER(buffer)[1];
	variant = Variant((int16_t) temp);
	return buffer.Ignore(2);
}

bool AMF0Serializer::WriteInt16(IOBuffer &buffer, int16_t value, bool writeType) {
	if (writeType) {
		NYIR;
	}
	buffer.ReadFromByte((uint8_t) (value >> 8));
	buffer.ReadFromByte((uint8_t) (value & 0x00ff));
	return true;
}

bool AMF0Serializer::ReadUInt32(IOBuffer &buffer, Variant &variant, bool readType) {
	if (readType) {
		NYIR;
	}
	AMF_CHECK_BOUNDARIES(buffer, 4);
	variant = (uint32_t) ENTOHLP(GETIBPOINTER(buffer)); //----MARKED-LONG---
	return buffer.Ignore(4);
}

bool AMF0Serializer::WriteUInt32(IOBuffer &buffer, uint32_t value, bool writeType) {
	if (writeType) {
		NYIR;
	}
	uint32_t val = EHTONL(value); //----MARKED-LONG---
	buffer.ReadFromBuffer((uint8_t *) & val, 4);
	return true;
}

bool AMF0Serializer::ReadInt32(IOBuffer &buffer, Variant &variant, bool readType) {
	if (!ReadUInt32(buffer, variant, readType))
		return false;
	uint32_t temp = (uint32_t) variant;
	variant = (int32_t) (*((int32_t *) & temp));
	return true;
}

bool AMF0Serializer::WriteInt32(IOBuffer &buffer, int32_t value, bool writeType) {
	return WriteUInt32(buffer, *((uint32_t *) & value), writeType);
}

bool AMF0Serializer::Read(IOBuffer &buffer, Variant &variant) {
	AMF_CHECK_BOUNDARIES(buffer, 1);
	switch (GETIBPOINTER(buffer)[0]) {
		case AMF0_NUMBER:
		{
			return ReadDouble(buffer, variant);
		}
		case AMF0_SHORT_STRING:
		{
			return ReadShortString(buffer, variant);
		}
		case AMF0_LONG_STRING:
		{
			return ReadLongString(buffer, variant);
		}
		case AMF0_OBJECT:
		{
			return ReadObject(buffer, variant);
		}
		case AMF0_BOOLEAN:
		{
			return ReadBoolean(buffer, variant);
		}
		case AMF0_NULL:
		{
			return ReadNull(buffer, variant);
		}
		case AMF0_UNDEFINED:
		{
			return ReadUndefined(buffer, variant);
		}
		case AMF0_MIXED_ARRAY:
		{
			return ReadMixedArray(buffer, variant);
		}
		case AMF0_ARRAY:
		{
			return ReadArray(buffer, variant);
		}
		case AMF0_AMF3_OBJECT:
		{
			return ReadAMF3Object(buffer, variant);
		}
		case AMF0_TIMESTAMP:
		{
			return ReadTimestamp(buffer, variant);
		}
		default:
		{
			FATAL("Unable to de-serialize type %"PRIu8"; Buffer: %s",
					GETIBPOINTER(buffer)[0], STR(buffer));
			return false;
		}
	}
}

bool AMF0Serializer::Write(IOBuffer &buffer, Variant &variant) {
	switch ((VariantType) variant) {
		case V_NULL:
		{
			return WriteNull(buffer);
		}
		case V_UNDEFINED:
		{
			return WriteUndefined(buffer);
		}
		case V_MAP:
		{
			if (IsAMF3(variant)) {
				FINEST("AMF3:\n%s", STR(variant.ToString()));
				return WriteAMF3Object(buffer, variant);
			} else {
				if (variant.IsArray()) {
					return WriteMixedArray(buffer, variant);
				}
				return WriteObject(buffer, variant);
			}
		}
		case V_TYPED_MAP:
		{
			if (IsAMF3(variant)) {
				FINEST("AMF3:\n%s", STR(variant.ToString()));
				return WriteAMF3Object(buffer, variant);
			} else {
				return WriteTypedObject(buffer, variant);
			}
		}
		case V_BYTEARRAY:
		{
			return WriteAMF3Object(buffer, variant);
		}
		case V_STRING:
		{
			string temp = variant;
			if (temp.length() >= 65536)
				return WriteLongString(buffer, temp);
			else
				return WriteShortString(buffer, temp);
		}
		case V_INT8:
		case V_UINT8:
		case V_INT16:
		case V_UINT16:
		case V_INT32:
		case V_UINT32:
		case V_INT64:
		case V_UINT64:
		case V_DOUBLE:
		{
			double temp = (double) variant;
			return WriteDouble(buffer, temp);
		}
		case V_BOOL:
		{
			bool temp = (bool)variant;
			return WriteBoolean(buffer, temp);
		}
		case V_DATE:
		case V_TIME:
		case V_TIMESTAMP:
		{
			Timestamp temp = (Timestamp) variant;
			return WriteTimestamp(buffer, temp);
		}
		default:
		{
			FATAL("Invalid variant type: %s", STR(variant.ToString()));
			return false;
		}
	}
}

bool AMF0Serializer::IsAMF3(Variant &variant) {
	return false;
}
#endif /* HAS_PROTOCOL_RTMP */

