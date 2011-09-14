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


#ifndef _VARIANT_H
#define	_VARIANT_H

#include "defines.h"
#include "platform/platform.h"

//#define LOG_VARIANT_MEMORY_MANAGEMENT

#ifdef LOG_VARIANT_MEMORY_MANAGEMENT
#define CONSTRUCTOR		printf(" +  %u->%u\n",_constructorCount,_constructorCount+1); _constructorCount++;
#define DESTRUCTOR		printf(" -  %u->%u\n",_constructorCount,_constructorCount-1); _constructorCount--;
#define DYNAMIC_ALLOC(type)	printf("(+) %u->%u (%s)\n",_dynamicAllocationCount,_dynamicAllocationCount+1,type); _dynamicAllocationCount++;
#define DYNAMIC_FREE(type)	printf("(-) %u->%u (%s)\n",_dynamicAllocationCount,_dynamicAllocationCount-1,type); _dynamicAllocationCount--;
#else
#define CONSTRUCTOR
#define DESTRUCTOR
#define DYNAMIC_ALLOC(type)
#define DYNAMIC_FREE(type)
#endif

#define VARIANT_GET(type,name,defaultValue) \
type name() { \
	if((*this)!=V_MAP) \
		return defaultValue; \
	if(this->HasKey(#name)) \
		return (type)((*this)[#name]); \
	return defaultValue; \
};

#define VARIANT_SET(type,name) \
void name(type val) { \
    (*this)[#name]=(type)val; \
};

#define VARIANT_GETSET(type, name, defaultValue) VARIANT_GET(type,name,defaultValue);VARIANT_SET(type,name);

typedef enum _VariantType {
	V_NULL = VAR_ENUM_VALUE_NULL,
	V_UNDEFINED = VAR_ENUM_VALUE_UNDEFINED,
	V_BOOL = VAR_ENUM_VALUE_BOOL,
	V_INT8 = VAR_ENUM_VALUE_INT8,
	V_INT16 = VAR_ENUM_VALUE_INT16,
	V_INT32 = VAR_ENUM_VALUE_INT32,
	V_INT64 = VAR_ENUM_VALUE_INT64,
	V_UINT8 = VAR_ENUM_VALUE_UINT8,
	V_UINT16 = VAR_ENUM_VALUE_UINT16,
	V_UINT32 = VAR_ENUM_VALUE_UINT32,
	V_UINT64 = VAR_ENUM_VALUE_UINT64,
	V_DOUBLE = VAR_ENUM_VALUE_DOUBLE,
	_V_NUMERIC = VAR_ENUM_VALUE_NUMERIC,
	V_TIMESTAMP = VAR_ENUM_VALUE_TIMESTAMP,
	V_DATE = VAR_ENUM_VALUE_DATE,
	V_TIME = VAR_ENUM_VALUE_TIME,
	V_STRING = VAR_ENUM_VALUE_STRING,
	V_TYPED_MAP = VAR_ENUM_VALUE_TYPED_MAP,
	V_MAP = VAR_ENUM_VALUE_MAP,
	V_BYTEARRAY = VAR_ENUM_VALUE_BYTEARRAY
} VariantType;

struct VariantMap;
class TiXmlElement;

class DLLEXP Variant {
private:
	VariantType _type;

	union {
		bool b;
		int8_t i8;
		int16_t i16;
		int32_t i32;
		int64_t i64;
		uint8_t ui8;
		uint16_t ui16;
		uint32_t ui32;
		uint64_t ui64;
		double d;
		Timestamp *t;
		string *s;
		VariantMap *m;
	} _value;
#ifdef LOG_VARIANT_MEMORY_MANAGEMENT
	static int _constructorCount;
	static int _dynamicAllocationCount;
#endif
public:
	Variant();
	Variant(const Variant &val);

	Variant(const bool &val);
	Variant(const int8_t &val);
	Variant(const int16_t &val);
	Variant(const int32_t &val);
	Variant(const int64_t &val);
	Variant(const uint8_t &val);
	Variant(const uint16_t &val);
	Variant(const uint32_t &val);
	Variant(const uint64_t &val);
	Variant(const double &val);

	Variant(const Timestamp &time);
	Variant(const uint16_t year, const uint8_t month, const uint8_t day);
	Variant(const uint8_t hour, const uint8_t min, const uint8_t sec, const uint16_t m);
	Variant(const uint16_t year, const uint8_t month, const uint8_t day,
			const uint8_t hour, const uint8_t min, const uint8_t sec, const uint16_t m);

	Variant(const char *pValue);
	Variant(const string &value);

	virtual ~Variant();

	void Reset(bool isUndefined = false);
	string ToString(string name = "", uint32_t indent = 0);

	Variant & operator=(const Variant &val);
	Variant & operator=(const bool &val);
	Variant & operator=(const int8_t &val);
	Variant & operator=(const int16_t &val);
	Variant & operator=(const int32_t &val);
	Variant & operator=(const int64_t &val);
	Variant & operator=(const uint8_t &val);
	Variant & operator=(const uint16_t &val);
	Variant & operator=(const uint32_t &val);
	Variant & operator=(const uint64_t &val);
	Variant & operator=(const double &val);

	Variant & operator=(const Timestamp &val);

	Variant & operator=(const char *pVal);
	Variant & operator=(const string &val);

	operator VariantType();
	operator bool();
	operator int8_t();
	operator int16_t();
	operator int32_t();
	operator int64_t();
	operator uint8_t();
	operator uint16_t();
	operator uint32_t();
	operator uint64_t();
	operator double();
	operator Timestamp();
	operator string();

	Variant & operator[](const string &key);
	Variant & operator[](const char *key);
	Variant & operator[](const double &key);
	Variant & operator[](const uint32_t &key);
	Variant & operator[](Variant &key);
	Variant & GetValue(string key, bool caseSensitive);

	bool operator==(Variant variant);
	bool operator!=(Variant variant);
	bool operator==(VariantType type);
	bool operator!=(VariantType type);

	string GetTypeName();
	void SetTypeName(string name);
	bool HasKey(const string &key, bool caseSensitive = true);
	bool HasKeyChain(VariantType end, bool caseSensitive, uint32_t depth, ...);
	void RemoveKey(const string &key);
	void RemoveAt(const uint32_t index);
	void RemoveAllKeys();
	uint32_t MapSize();
	uint32_t MapDenseSize();
	void PushToArray(Variant value);

	map<string, Variant>::iterator begin();
	map<string, Variant>::iterator end();

	bool IsTimestamp(VariantType &type);
	bool IsNumeric();
	bool IsArray();
	void IsArray(bool isArray);
	bool IsByteArray();
	void IsByteArray(bool isByteArray);
	bool ConvertToTimestamp();
	void Compact();

	static bool DeserializeFromBin(uint8_t *pBuffer, uint32_t bufferLength,
			Variant &variant);
	static bool DeserializeFromBin(string &data, Variant &variant);
	bool SerializeToBin(string &result);

	static bool DeserializeFromXml(const uint8_t *pBuffer, uint32_t bufferLength,
			Variant &result);
	static bool DeserializeFromXml(string data, Variant &result);
	bool SerializeToXml(string &result, bool prettyPrint = true);

	static bool DeserializeFromBinFile(string path, Variant &variant);
	bool SerializeToBinFile(string fileName);

	static bool DeserializeFromXmlFile(string path, Variant &variant);
	bool SerializeToXmlFile(string fileName);

	static bool DeserializeFromJSON(string &raw, Variant &result, uint32_t &start);
	bool SerializeToJSON(string &result);

	static bool DeserializeFromCmdLineArgs(uint32_t count, char **pArguments,
			Variant &result);
private:
	TiXmlElement *SerializeToXmlElement(string &name);
	static bool DeserializeFromBin(uint8_t *pBuffer, uint32_t bufferSize,
			Variant &variant, uint32_t &cursor);
	static bool DeserializeFromXml(TiXmlElement *pNode, Variant &variant);
	void InternalCopy(const Variant &val);
	void NormalizeTs();
	static void EscapeJSON(string &value);
	static void UnEscapeJSON(string &value);
	static bool ReadJSONWhiteSpace(string &raw, uint32_t &start);
	static bool ReadJSONDelimiter(string &raw, uint32_t &start, char &c);
	static bool ReadJSONString(string &raw, Variant &result, uint32_t &start);
	static bool ReadJSONNumber(string &raw, Variant &result, uint32_t &start);
	static bool ReadJSONObject(string &raw, Variant &result, uint32_t &start);
	static bool ReadJSONArray(string &raw, Variant &result, uint32_t &start);
	static bool ReadJSONBool(string &raw, Variant &result, uint32_t &start, string wanted);
	static bool ReadJSONNull(string &raw, Variant &result, uint32_t &start);
};


#endif	/* _VARIANT_H */


