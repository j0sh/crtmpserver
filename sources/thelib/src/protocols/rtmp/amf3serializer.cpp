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
#include "protocols/rtmp/amf3serializer.h"
#include "protocols/rtmp/amftypes.h"
#include "protocols/baseprotocol.h"

#define READ_AMF3_TYPE(wanted) \
if (readType) { \
    AMF_CHECK_BOUNDARIES(buffer, 1); \
    if (GETIBPOINTER(buffer)[0] != wanted) { \
        FATAL("AMF type not valid: want: %hhu; got: %hhu", \
                (uint8_t)wanted, GETIBPOINTER(buffer)[0]); \
        return false; \
    } \
    \
    if (!buffer.Ignore(1)) { \
        FATAL("Unable to ignore 1 bytes"); \
        return false; \
    } \
}

#define WRITE_AMF3_TYPE(type) \
if (writeType) { \
    buffer.ReadFromRepeat(type,1); \
}

AMF3Serializer::AMF3Serializer() {

}

AMF3Serializer::~AMF3Serializer() {
}

bool AMF3Serializer::Read(IOBuffer &buffer, Variant & variant) {
	AMF_CHECK_BOUNDARIES(buffer, 1);
	switch (GETIBPOINTER(buffer)[0]) {
		case AMF3_UNDEFINED:
		{
			return ReadUndefined(buffer, variant);
		}
		case AMF3_NULL:
		{
			return ReadNull(buffer, variant);
		}
		case AMF3_FALSE:
		{
			return ReadFalse(buffer, variant);
		}
		case AMF3_TRUE:
		{
			return ReadTrue(buffer, variant);
		}
		case AMF3_INTEGER:
		{
			return ReadInteger(buffer, variant);
		}
		case AMF3_DOUBLE:
		{
			return ReadDouble(buffer, variant);
		}
		case AMF3_STRING:
		{
			return ReadString(buffer, variant);
		}
		case AMF3_XMLDOC:
		{
			return ReadXMLDoc(buffer, variant);
		}
		case AMF3_DATE:
		{
			return ReadDate(buffer, variant);
		}
		case AMF3_ARRAY:
		{
			return ReadArray(buffer, variant);
		}
		case AMF3_OBJECT:
		{
			return ReadObject(buffer, variant);
		}
		case AMF3_XML:
		{
			return ReadXML(buffer, variant);
		}
		case AMF3_BYTEARRAY:
		{
			return ReadByteArray(buffer, variant);
		}
		default:
		{
			FATAL("Unable to deserialize type %hhu; Buffer is:\n%s",
					GETIBPOINTER(buffer)[0], STR(buffer));
			return false;
		}
	}
}

bool AMF3Serializer::Write(IOBuffer &buffer, Variant &variant) {
	switch ((VariantType) variant) {
		case V_NULL:
		{
			return WriteNull(buffer);
		}
		case V_UNDEFINED:
		{
			return WriteUndefined(buffer);
		}
		case V_BYTEARRAY:
		{
			return WriteByteArray(buffer, variant);
		}
		case V_BOOL:
		{
			if ((bool)variant)
				return WriteTrue(buffer);
			return WriteFalse(buffer);
		}
		case V_MAP:
		{
			if (variant.IsArray())
				return WriteArray(buffer, variant);
			else
				return WriteObject(buffer, variant);
		}
		case V_STRING:
		{
			return WriteString(buffer, variant);
		}
		case V_INT8:
		case V_INT16:
		case V_INT32:
		case V_UINT8:
		case V_UINT16:
		case V_UINT32:
		case V_UINT64:
		case V_INT64:
		case V_DOUBLE:
		{
			return WriteDouble(buffer, variant);
		}
		case V_DATE:
		case V_TIME:
		case V_TIMESTAMP:
		{
			return WriteDate(buffer, variant);
		}
		default:
		{
			FATAL("Unable to serialize type %hhu; variant is:\n%s",
					(VariantType) variant, STR(variant.ToString()));
			return false;
		}
	}
}

bool AMF3Serializer::ReadUndefined(IOBuffer &buffer, Variant &variant, bool readType) {
	READ_AMF3_TYPE(AMF3_UNDEFINED);

	variant.Reset();
	return true;
}

bool AMF3Serializer::WriteUndefined(IOBuffer &buffer) {
	buffer.ReadFromRepeat(AMF3_UNDEFINED, 1);
	return true;
}

bool AMF3Serializer::ReadNull(IOBuffer &buffer, Variant &variant, bool readType) {
	READ_AMF3_TYPE(AMF3_NULL);

	variant.Reset();
	return true;
}

bool AMF3Serializer::WriteNull(IOBuffer &buffer) {
	buffer.ReadFromRepeat(AMF3_NULL, 1);
	return true;
}

bool AMF3Serializer::ReadFalse(IOBuffer &buffer, Variant &variant, bool readType) {
	READ_AMF3_TYPE(AMF3_FALSE);

	variant = (bool)false;
	return true;
}

bool AMF3Serializer::WriteFalse(IOBuffer &buffer) {
	buffer.ReadFromRepeat(AMF3_FALSE, 1);
	return true;
}

bool AMF3Serializer::ReadTrue(IOBuffer &buffer, Variant &variant, bool readType) {
	READ_AMF3_TYPE(AMF3_TRUE);

	variant = (bool)true;
	return true;
}

bool AMF3Serializer::WriteTrue(IOBuffer &buffer) {
	buffer.ReadFromRepeat(AMF3_TRUE, 1);
	return true;
}

bool AMF3Serializer::ReadInteger(IOBuffer &buffer, Variant &variant, bool readType) {
	READ_AMF3_TYPE(AMF3_INTEGER);

	uint32_t result;
	if (!ReadU29(buffer, result)) {
		FATAL("Unable to read integer");
		return false;
	}

	variant = (uint32_t) result;

	return true;
}

bool AMF3Serializer::WriteInteger(IOBuffer &buffer, uint32_t value, bool writeType) {
	WRITE_AMF3_TYPE(AMF3_INTEGER);

	return WriteU29(buffer, value);
}

bool AMF3Serializer::ReadDouble(IOBuffer &buffer, Variant &variant, bool readType) {
	READ_AMF3_TYPE(AMF3_DOUBLE);

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

bool AMF3Serializer::WriteDouble(IOBuffer &buffer, double value, bool writeType) {
	WRITE_AMF3_TYPE(AMF3_DOUBLE);

	uint64_t u64Val = 0;
	EHTOND(value, u64Val);

	return buffer.ReadFromBuffer((uint8_t *) & u64Val, 8);
}

bool AMF3Serializer::ReadString(IOBuffer &buffer, Variant &variant, bool readType) {
	READ_AMF3_TYPE(AMF3_STRING);

	uint32_t temp;
	if (!ReadU29(buffer, temp)) {
		FATAL("Unable to read reference");
		return false;
	}

	if ((temp & 0x01) == 1) {
		//new string
		uint32_t length = temp >> 1;
		if (length != 0) {
			AMF_CHECK_BOUNDARIES(buffer, length);
			string result = string((char *) GETIBPOINTER(buffer), length);
			if (!buffer.Ignore(length)) {
				FATAL("Unable to ignore %u bytes", length);
				return false;
			}
			variant = result;
			ADD_VECTOR_END(_strings, result);
		} else {
			variant = "";
		}
	} else {
		//cached string
		variant = _strings[temp >> 1];
	}

	return true;
}

bool AMF3Serializer::WriteString(IOBuffer &buffer, string value, bool writeType) {
	WRITE_AMF3_TYPE(AMF3_STRING);

	if (!WriteU29(buffer, (value.size() << 1) | 0x01)) {
		FATAL("Unable to read reference");
		return false;
	}

	buffer.ReadFromString(value);

	return true;
}

bool AMF3Serializer::ReadXMLDoc(IOBuffer &buffer, Variant &variant, bool readType) {
	READ_AMF3_TYPE(AMF3_XMLDOC);
	NYIR;
}

bool AMF3Serializer::WriteXMLDoc(IOBuffer &buffer, Variant &variant, bool writeType) {
	NYIR;
}

bool AMF3Serializer::ReadDate(IOBuffer &buffer, Variant &variant, bool readType) {
	READ_AMF3_TYPE(AMF3_DATE);

	uint32_t temp;
	if (!ReadU29(buffer, temp)) {
		FATAL("Unable to read reference");
		return false;
	}

	if ((temp & 0x01) == 1) {
		//new date
		AMF_CHECK_BOUNDARIES(buffer, 8);
		uint64_t u64Val = ENTOHLLP(GETIBPOINTER(buffer));
		if (!buffer.Ignore(8)) {
			FATAL("Unable to ignore 8 bytes");
			return false;
		}
		time_t tVal = (time_t) ((*((double *) & u64Val)) / 1000.00);
		Timestamp tsVal = *(gmtime(&tVal));
		variant = tsVal;
		ADD_VECTOR_END(_objects, variant);
	} else {
		//cached string
		variant = _objects[temp >> 1];
	}

	return true;
}

bool AMF3Serializer::WriteDate(IOBuffer &buffer, Timestamp value, bool writeType) {
	WRITE_AMF3_TYPE(AMF3_DATE);
	if (!WriteU29(buffer, 1)) {
		FATAL("Unable to write U29");
		return false;
	}

	double seconds = (double) timegm(&value)*1000.00;
	if (!WriteDouble(buffer, seconds, false)) {
		FATAL("Unable to write double");
		return false;
	}

	return true;
}

bool AMF3Serializer::ReadArray(IOBuffer &buffer, Variant &variant, bool readType) {
	READ_AMF3_TYPE(AMF3_ARRAY);

	//1. Read the flags
	uint32_t temp;
	if (!ReadU29(buffer, temp)) {
		FATAL("Unable to read reference");
		return false;
	}

	if ((temp & 0x01) == 0) {
		WARN("Array reference: %u", temp >> 1);
		variant = _objects[temp >> 1];
		return true;
	}

	//2. Read the associative portion
	while (true) {
		Variant key;
		if (!ReadString(buffer, key, false)) {
			FATAL("Unable to read the key");
			return false;
		}
		if (key == "")
			break;
		if (!Read(buffer, variant[(string) key])) {
			FATAL("Unable to read the value");
			return false;
		}
	}

	//3. read the dense portion
	uint32_t denseSize = temp >> 1;
	for (uint32_t i = 0; i < denseSize; i++) {
		if (!Read(buffer, variant[(uint32_t) i])) {
			FATAL("Unable to read value");
			return false;
		}
	}

	//4. Mark the object as an array
	variant.IsArray(true);

	//5. Store it in the reference table
	ADD_VECTOR_END(_objects, variant);

	//6. Done
	return true;
}

bool AMF3Serializer::WriteArray(IOBuffer &buffer, Variant &variant, bool writeType) {
	WRITE_AMF3_TYPE(AMF3_ARRAY);

	Variant v = variant;

	uint32_t denseSize = v.MapDenseSize();

	for (uint32_t i = 0; i < denseSize; i++) {
		v.RemoveAt(i);
	}

	if (!WriteU29(buffer, (denseSize << 1) | 0x01)) {
		FATAL("Unable to write dense size");
		return false;
	}

	Variant key = "";

	FOR_MAP(v, string, Variant, i) {
		key = MAP_KEY(i);
		if (!WriteString(buffer, key, false)) {
			FATAL("Unable to write key");
			return false;
		}
		if (!Write(buffer, MAP_VAL(i))) {
			FATAL("Unable to write value");
			return false;
		}
	}

	key = "";
	if (!WriteString(buffer, key, false)) {
		FATAL("Unable to write key");
		return false;
	}

	for (uint32_t i = 0; i < denseSize; i++) {
		if (!Write(buffer, variant[i])) {
			FATAL("Unable to write value");
			return false;
		}
	}

	return true;
}

bool AMF3Serializer::ReadObject(IOBuffer &buffer, Variant &variant, bool readType) {
	READ_AMF3_TYPE(AMF3_OBJECT);

	uint32_t temp;
	if (!ReadU29(buffer, temp)) {
		FATAL("Unable to read reference");
		return false;
	}

	bool objectReference = ((temp & 0x01) == 0);
	uint32_t objectReferenceIndex = temp >> 1;

	bool traitsReference = ((temp & 0x02) == 0);
	uint32_t traitsReferenceIndex = temp >> 2;

	bool traitsExtended = ((temp & 0x07) == 0x07);

	bool isDynamic = ((temp & 0x08) != 0);
	uint32_t traitsCount = temp >> 4;


	if (objectReference) {
		variant = _objects[objectReferenceIndex];
		return true;
	}

	if (traitsExtended) {
		Variant className;
		if (!ReadString(buffer, className, false)) {
			FATAL("Unable to read the class name");
			return false;
		}

		if (className == "flex.messaging.io.ArrayCollection") {
			variant.SetTypeName((string) className);
			if (!Read(buffer, variant)) {
				FATAL("Unable to read object");
				return false;
			}
			ADD_VECTOR_END(_objects, variant);
			return true;
		} else {
			FATAL("Class name %s not supported yet", STR(className));
			return false;
		}
	}

	uint32_t objectIndex = _objects.size();
	Variant tempVariant = Variant();
	ADD_VECTOR_END(_objects, tempVariant);

	Variant traits;

	if (traitsReference) {
		traits = _traits[traitsReferenceIndex];
		FINEST("Traits:\n%s", STR(traits.ToString()));
	} else {
		uint32_t traitsIndex = _traits.size();

		tempVariant = Variant();
		ADD_VECTOR_END(_traits, tempVariant);
		traits[AMF3_TRAITS_DYNAMIC] = (bool)isDynamic;

		Variant className;
		if (!ReadString(buffer, className, false)) {
			FATAL("Unable to read the class name");
			return false;
		}
		traits[AMF3_TRAITS_CLASSNAME] = className;

		for (uint32_t i = 0; i < traitsCount; i++) {
			Variant traitName;
			if (!ReadString(buffer, traitName, false)) {
				FATAL("Unable to read trait name");
				return false;
			}
			traits[AMF3_TRAITS][i] = traitName;
		}

		_traits[traitsIndex] = traits;
	}

	if (traits[AMF3_TRAITS_CLASSNAME] != "") {
		variant.SetTypeName(traits[AMF3_TRAITS_CLASSNAME]);
		ASSERT("%s", STR(traits.ToString()));
	}
	for (uint32_t i = 0; i < traits[AMF3_TRAITS].MapSize(); i++) {
		string traitName = traits[AMF3_TRAITS][i];
		FINEST("Read value for trait %s", STR(traitName));
		if (!Read(buffer, variant[AMF3_TRAITS][traitName])) {
			FATAL("Unable to read trait value");
			return false;
		}
	}

	bool readDynamicPoperties = false;

	if (traitsReference) {
		readDynamicPoperties = (bool)traits[AMF3_TRAITS_DYNAMIC];
	} else {
		readDynamicPoperties = isDynamic;
	}

	if (readDynamicPoperties) {
		while (true) {
			Variant key;
			if (!ReadString(buffer, key, false)) {
				FATAL("Unable to read the key");
				return false;
			}
			if (key == "") {
				break;
			}

			if (!Read(buffer, variant[(string) key])) {
				FATAL("Unable to read the value");
				return false;
			}
		}
	}

	_objects[objectIndex] = variant;
	return true;
}

bool AMF3Serializer::WriteObject(IOBuffer &buffer, Variant &variant, bool writeType) {
	WRITE_AMF3_TYPE(AMF3_OBJECT);
	if (!WriteU29(buffer, 0x0b)) {
		FATAL("Unable to save the traits count");
		return false;
	}

	Variant className = "";
	if (!WriteString(buffer, className, false)) {
		FATAL("Unable to read the class name");
		return false;
	}

	FOR_MAP(variant, string, Variant, i) {
		if (!WriteString(buffer, MAP_KEY(i), false)) {
			FATAL("Unable to write key");
			return false;
		}
		if (!Write(buffer, MAP_VAL(i))) {
			FATAL("Unable to write value");
			return false;
		}
	}

	if (!WriteString(buffer, "", false)) {
		FATAL("Unable to write key");
		return false;
	}

	return true;
}

bool AMF3Serializer::ReadXML(IOBuffer &buffer, Variant &variant, bool readType) {

	READ_AMF3_TYPE(AMF3_XML);
	NYIR;
}

bool AMF3Serializer::WriteXML(IOBuffer &buffer, Variant &variant, bool writeType) {

	NYIR;
}

bool AMF3Serializer::ReadByteArray(IOBuffer &buffer, Variant &variant, bool readType) {
	READ_AMF3_TYPE(AMF3_BYTEARRAY);

	uint32_t temp;
	if (!ReadU29(buffer, temp)) {
		FATAL("Unable to read reference");
		return false;
	}

	if ((temp & 0x01) == 1) {
		//new byte array
		uint32_t length = temp >> 1;
		if (length != 0) {
			AMF_CHECK_BOUNDARIES(buffer, length);
			string result = string((char *) GETIBPOINTER(buffer), length);
			if (!buffer.Ignore(length)) {
				FATAL("Unable to ignore %u bytes", length);
				return false;
			}
			variant = result;
			variant.IsByteArray(true);
			ADD_VECTOR_END(_byteArrays, result);
		} else {
			variant = "";
			variant.IsByteArray(true);
		}
	} else {
		//cached string

		variant = _byteArrays[temp >> 1];
	}

	return true;
}

bool AMF3Serializer::WriteByteArray(IOBuffer &buffer, Variant &variant, bool writeType) {
	WRITE_AMF3_TYPE(AMF3_BYTEARRAY);

	string temp = (string) variant;

	if (!WriteU29(buffer, (temp.length() << 1) | 0x01)) {
		FATAL("Unable to write U29");

		return false;
	}

	return buffer.ReadFromString(temp);
}

bool AMF3Serializer::ReadU29(IOBuffer &buffer, uint32_t & value) {
	value = 0;
	for (uint32_t i = 0; i < 4; i++) {
		AMF_CHECK_BOUNDARIES(buffer, 1);
		uint8_t byte = GETIBPOINTER(buffer)[0];
		if (!buffer.Ignore(1)) {
			FATAL("Unable to ignore 1 byte");
			return false;
		}
		if (i != 3) {
			value = (value << 7) | (byte & 0x7f);
		} else {
			value = (value << 8) | byte;
		}
		if ((byte & 0x80) == 0)
			break;
	}

	return true;
}

bool AMF3Serializer::WriteU29(IOBuffer &buffer, uint32_t value) {
	uint32_t temp = EHTONL(value);
	uint8_t *pBuffer = (uint8_t*) & temp;
	if ((0x00000000 <= value) && (value <= 0x0000007f)) {
		buffer.ReadFromRepeat(pBuffer[3], 1);
		return true;
	} else if ((0x00000080 <= value) && (value <= 0x00003fff)) {
		buffer.ReadFromRepeat(((pBuffer[2] << 1) | (pBuffer[3] >> 7)) | 0x80, 1);
		buffer.ReadFromRepeat(pBuffer[3]&0x7f, 1);
		return true;
	} else if ((0x00004000 <= value) && (value <= 0x001fffff)) {
		buffer.ReadFromRepeat(((pBuffer[1] << 2) | (pBuffer[2] >> 6)) | 0x80, 1);
		buffer.ReadFromRepeat(((pBuffer[2] << 1) | (pBuffer[3] >> 7)) | 0x80, 1);
		buffer.ReadFromRepeat(pBuffer[3]&0x7f, 1);
		return true;
	} else if ((0x0020000 <= value) && (value <= 0x01fffffff)) {
		buffer.ReadFromRepeat(((pBuffer[0] << 2) | (pBuffer[1] >> 6)) | 0x80, 1);
		buffer.ReadFromRepeat(((pBuffer[1] << 1) | (pBuffer[2] >> 7)) | 0x80, 1);
		buffer.ReadFromRepeat(pBuffer[2] | 0x80, 1);
		buffer.ReadFromRepeat(pBuffer[3], 1);
		return true;
	}

	return false;
}
#endif /* HAS_PROTOCOL_RTMP */

