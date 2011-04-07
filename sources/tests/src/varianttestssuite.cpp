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


#include "varianttestssuite.h"
#include "common.h"

#define di64_1 ((int64_t) (0x8000000000000000LL))
#define di32_1 ((int32_t) (0x80000000L))
#define di16_1 ((int16_t) (0x8000))
#define di8_1  ((int8_t)  (0x80))
#define di8_2  ((int8_t)  (0x00))
#define di8_3  ((int8_t)  (0x7F))
#define dui8   ((uint8_t) (0xFF))
#define di16_2 ((int16_t) (0x7FFF))
#define dui16  ((uint16_t)(0xFFFF))
#define di32_2 ((int32_t) (0x7FFFFFFFUL))
#define dui32  ((uint32_t)(0xFFFFFFFFUL))
#define di64_2 ((int64_t) (0x7FFFFFFFFFFFFFFFULL))
#define dui64  ((uint64_t)(0xFFFFFFFFFFFFFFFFULL))

VariantTestsSuite::VariantTestsSuite()
: BaseTestsSuite() {
	_boolVar1 = Variant(true);
	_boolVar2 = Variant(false);
	_int8Var = Variant((int8_t) - 1);
	_int16Var = Variant((int16_t) - 2);
	_int32Var = Variant((int32_t) - 3);
	_int64Var = Variant((int64_t) - 4);
	_uint8Var = Variant((uint8_t) 129);
	_uint16Var = Variant((uint16_t) 32769);
	_uint32Var = Variant((uint32_t) 2147483649UL);
	_uint64Var = Variant((uint64_t) (9223372036854780001ULL));
	_doubleVar = Variant((double) 123.456);
	_dateVar = Variant(1979, 10, 31);
	_timeVar = Variant(12, 13, 14, 0);
	Timestamp ts;
	memset(&ts, 0, sizeof (Timestamp));
	_timestampVar1 = Variant(ts);
	_timestampVar2 = Variant(1979, 10, 31, 12, 13, 14, 0);
	_pcharVar = Variant((char *) "test");
	_stringVar = Variant(string("test"));
	_mapVar.Reset();
	_mapVar["test"] = "test";
	_namedMapVar.Reset();
	_namedMapVar["test"] = "test";
	_namedMapVar.SetTypeName("test");
	_referenceVariant1 = Variant(_noParamsVar);
	_referenceVariant2 = Variant(_boolVar1);
	_referenceVariant3 = Variant(_boolVar2);
	_referenceVariant4 = Variant(_int8Var);
	_referenceVariant5 = Variant(_int16Var);
	_referenceVariant6 = Variant(_int32Var);
	_referenceVariant7 = Variant(_int64Var);
	_referenceVariant8 = Variant(_uint8Var);
	_referenceVariant9 = Variant(_uint16Var);
	_referenceVariant10 = Variant(_uint32Var);
	_referenceVariant11 = Variant(_uint64Var);
	_referenceVariant12 = Variant(_doubleVar);
	_referenceVariant13 = Variant(_dateVar);
	_referenceVariant14 = Variant(_timeVar);
	_referenceVariant15 = Variant(_timestampVar1);
	_referenceVariant16 = Variant(_timestampVar2);
	_referenceVariant17 = Variant(_pcharVar);
	_referenceVariant18 = Variant(_stringVar);
	_referenceVariant19 = Variant(_mapVar);
	_referenceVariant20 = Variant(_namedMapVar);
}

VariantTestsSuite::~VariantTestsSuite() {

}

void VariantTestsSuite::Run() {
	test_Constructors();
	test_Reset();
	test_ToString();
	test_OperatorAssign();
	test_OperatorCast();
	test_Compact();
}

void VariantTestsSuite::test_Constructors() {
	TS_ASSERT(_noParamsVar == V_NULL);
	TS_ASSERT(_boolVar1 == V_BOOL);
	TS_ASSERT(_boolVar2 == V_BOOL);
	TS_ASSERT(_int8Var == V_INT8);
	TS_ASSERT(_int16Var == V_INT16);
	TS_ASSERT(_int32Var == V_INT32);
	TS_ASSERT(_int64Var == V_INT64);
	TS_ASSERT(_uint8Var == V_UINT8);
	TS_ASSERT(_uint16Var == V_UINT16);
	TS_ASSERT(_uint32Var == V_UINT32);
	TS_ASSERT(_uint64Var == V_UINT64);
	TS_ASSERT(_doubleVar == V_DOUBLE);
	TS_ASSERT(_dateVar == V_DATE);
	TS_ASSERT(_timeVar == V_TIME);
	TS_ASSERT(_timestampVar1 == V_TIMESTAMP);
	TS_ASSERT(_timestampVar2 == V_TIMESTAMP);
	TS_ASSERT(_pcharVar == V_STRING);
	TS_ASSERT(_stringVar == V_STRING);
	TS_ASSERT(_mapVar == V_MAP);
	TS_ASSERT(_namedMapVar == V_TYPED_MAP);
	TS_ASSERT(_referenceVariant1 == V_NULL);
	TS_ASSERT(_referenceVariant2 == V_BOOL);
	TS_ASSERT(_referenceVariant3 == V_BOOL);
	TS_ASSERT(_referenceVariant4 == V_INT8);
	TS_ASSERT(_referenceVariant5 == V_INT16);
	TS_ASSERT(_referenceVariant6 == V_INT32);
	TS_ASSERT(_referenceVariant7 == V_INT64);
	TS_ASSERT(_referenceVariant8 == V_UINT8);
	TS_ASSERT(_referenceVariant9 == V_UINT16);
	TS_ASSERT(_referenceVariant10 == V_UINT32);
	TS_ASSERT(_referenceVariant11 == V_UINT64);
	TS_ASSERT(_referenceVariant12 == V_DOUBLE);
	TS_ASSERT(_referenceVariant13 == V_DATE);
	TS_ASSERT(_referenceVariant14 == V_TIME);
	TS_ASSERT(_referenceVariant15 == V_TIMESTAMP);
	TS_ASSERT(_referenceVariant16 == V_TIMESTAMP);
	TS_ASSERT(_referenceVariant17 == V_STRING);
	TS_ASSERT(_referenceVariant18 == V_STRING);
	TS_ASSERT(_referenceVariant19 == V_MAP);
	TS_ASSERT(_referenceVariant20 == V_TYPED_MAP);
}

void VariantTestsSuite::test_Reset() {
#define test_Reset_def(value) \
{ \
    Variant v; \
    v=value; \
    v.Reset(); \
    TS_ASSERT(v == V_NULL); \
    v=value; \
    v.Reset(true); \
    TS_ASSERT(v == V_UNDEFINED); \
}
	test_Reset_def(_boolVar1);
	test_Reset_def(_boolVar2);
	test_Reset_def(_int8Var);
	test_Reset_def(_int16Var);
	test_Reset_def(_int32Var);
	test_Reset_def(_int64Var);
	test_Reset_def(_uint8Var);
	test_Reset_def(_uint16Var);
	test_Reset_def(_uint32Var);
	test_Reset_def(_uint64Var);
	test_Reset_def(_doubleVar);
	test_Reset_def(_dateVar);
	test_Reset_def(_timeVar);
	test_Reset_def(_timestampVar1);
	test_Reset_def(_timestampVar2);
	test_Reset_def(_pcharVar);
	test_Reset_def(_stringVar);
	test_Reset_def(_mapVar);
	test_Reset_def(_namedMapVar);
	test_Reset_def(_referenceVariant1);
	test_Reset_def(_referenceVariant2);
	test_Reset_def(_referenceVariant3);
	test_Reset_def(_referenceVariant4);
	test_Reset_def(_referenceVariant5);
	test_Reset_def(_referenceVariant6);
	test_Reset_def(_referenceVariant7);
	test_Reset_def(_referenceVariant8);
	test_Reset_def(_referenceVariant9);
	test_Reset_def(_referenceVariant10);
	test_Reset_def(_referenceVariant11);
	test_Reset_def(_referenceVariant12);
	test_Reset_def(_referenceVariant13);
	test_Reset_def(_referenceVariant14);
	test_Reset_def(_referenceVariant15);
	test_Reset_def(_referenceVariant16);
	test_Reset_def(_referenceVariant17);
	test_Reset_def(_referenceVariant18);
	test_Reset_def(_referenceVariant19);
	test_Reset_def(_referenceVariant20);
}

void VariantTestsSuite::test_ToString() {
	TS_ASSERT(_noParamsVar.ToString() == "<NULL name=\"\"></NULL>");
	TS_ASSERT(_boolVar1.ToString() == "<BOOL name=\"\">true</BOOL>");
	TS_ASSERT(_boolVar2.ToString() == "<BOOL name=\"\">false</BOOL>");
	TS_ASSERT(_int8Var.ToString() == "<INT8 name=\"\">-1</INT8>");
	TS_ASSERT(_int16Var.ToString() == "<INT16 name=\"\">-2</INT16>");
	TS_ASSERT(_int32Var.ToString() == "<INT32 name=\"\">-3</INT32>");
	TS_ASSERT(_int64Var.ToString() == "<INT64 name=\"\">-4</INT64>");
	TS_ASSERT(_uint8Var.ToString() == "<UINT8 name=\"\">129</UINT8>");
	TS_ASSERT(_uint16Var.ToString() == "<UINT16 name=\"\">32769</UINT16>");
	TS_ASSERT(_uint32Var.ToString() == "<UINT32 name=\"\">2147483649</UINT32>");
	TS_ASSERT(_uint64Var.ToString() == "<UINT64 name=\"\">9223372036854780001</UINT64>");
	TS_ASSERT(_doubleVar.ToString() == "<DOUBLE name=\"\">123.456</DOUBLE>");
	TS_ASSERT(_dateVar.ToString() == "<DATE name=\"\">1979-10-31</DATE>");
	TS_ASSERT(_timeVar.ToString() == "<TIME name=\"\">12:13:14.000</TIME>");
	TS_ASSERT(_timestampVar1.ToString() == "<TIMESTAMP name=\"\">1970-01-01T00:00:00.000</TIMESTAMP>");
	TS_ASSERT(_timestampVar2.ToString() == "<TIMESTAMP name=\"\">1979-10-31T12:13:14.000</TIMESTAMP>");
	TS_ASSERT(_pcharVar.ToString() == "<STR name=\"\">test</STR>");
	TS_ASSERT(_stringVar.ToString() == "<STR name=\"\">test</STR>");
	TS_ASSERT(_mapVar.ToString() == "<MAP name=\"\" isArray=\"false\">\n    <STR name=\"test\">test</STR>\n</MAP>");
	TS_ASSERT(_namedMapVar.ToString() == "<TYPED_MAP name=\"\" typename=\"test\" isArray=\"false\">\n    <STR name=\"test\">test</STR>\n</TYPED_MAP>");
	TS_ASSERT(_referenceVariant1.ToString() == "<NULL name=\"\"></NULL>");
	TS_ASSERT(_referenceVariant2.ToString() == "<BOOL name=\"\">true</BOOL>");
	TS_ASSERT(_referenceVariant3.ToString() == "<BOOL name=\"\">false</BOOL>");
	TS_ASSERT(_referenceVariant4.ToString() == "<INT8 name=\"\">-1</INT8>");
	TS_ASSERT(_referenceVariant5.ToString() == "<INT16 name=\"\">-2</INT16>");
	TS_ASSERT(_referenceVariant6.ToString() == "<INT32 name=\"\">-3</INT32>");
	TS_ASSERT(_referenceVariant7.ToString() == "<INT64 name=\"\">-4</INT64>");
	TS_ASSERT(_referenceVariant8.ToString() == "<UINT8 name=\"\">129</UINT8>");
	TS_ASSERT(_referenceVariant9.ToString() == "<UINT16 name=\"\">32769</UINT16>");
	TS_ASSERT(_referenceVariant10.ToString() == "<UINT32 name=\"\">2147483649</UINT32>");
	TS_ASSERT(_referenceVariant11.ToString() == "<UINT64 name=\"\">9223372036854780001</UINT64>");
	TS_ASSERT(_referenceVariant12.ToString() == "<DOUBLE name=\"\">123.456</DOUBLE>");
	TS_ASSERT(_referenceVariant13.ToString() == "<DATE name=\"\">1979-10-31</DATE>");
	TS_ASSERT(_referenceVariant14.ToString() == "<TIME name=\"\">12:13:14.000</TIME>");
	TS_ASSERT(_referenceVariant15.ToString() == "<TIMESTAMP name=\"\">1970-01-01T00:00:00.000</TIMESTAMP>");
	TS_ASSERT(_referenceVariant16.ToString() == "<TIMESTAMP name=\"\">1979-10-31T12:13:14.000</TIMESTAMP>");
	TS_ASSERT(_referenceVariant17.ToString() == "<STR name=\"\">test</STR>");
	TS_ASSERT(_referenceVariant18.ToString() == "<STR name=\"\">test</STR>");
	TS_ASSERT(_referenceVariant19.ToString() == "<MAP name=\"\" isArray=\"false\">\n    <STR name=\"test\">test</STR>\n</MAP>");
	TS_ASSERT(_referenceVariant20.ToString() == "<TYPED_MAP name=\"\" typename=\"test\" isArray=\"false\">\n    <STR name=\"test\">test</STR>\n</TYPED_MAP>");

	TS_ASSERT(_noParamsVar.ToString("sampleName", 1) == "    <NULL name=\"sampleName\"></NULL>");
	TS_ASSERT(_boolVar1.ToString("sampleName", 1) == "    <BOOL name=\"sampleName\">true</BOOL>");
	TS_ASSERT(_boolVar2.ToString("sampleName", 1) == "    <BOOL name=\"sampleName\">false</BOOL>");
	TS_ASSERT(_int8Var.ToString("sampleName", 1) == "    <INT8 name=\"sampleName\">-1</INT8>");
	TS_ASSERT(_int16Var.ToString("sampleName", 1) == "    <INT16 name=\"sampleName\">-2</INT16>");
	TS_ASSERT(_int32Var.ToString("sampleName", 1) == "    <INT32 name=\"sampleName\">-3</INT32>");
	TS_ASSERT(_int64Var.ToString("sampleName", 1) == "    <INT64 name=\"sampleName\">-4</INT64>");
	TS_ASSERT(_uint8Var.ToString("sampleName", 1) == "    <UINT8 name=\"sampleName\">129</UINT8>");
	TS_ASSERT(_uint16Var.ToString("sampleName", 1) == "    <UINT16 name=\"sampleName\">32769</UINT16>");
	TS_ASSERT(_uint32Var.ToString("sampleName", 1) == "    <UINT32 name=\"sampleName\">2147483649</UINT32>");
	TS_ASSERT(_uint64Var.ToString("sampleName", 1) == "    <UINT64 name=\"sampleName\">9223372036854780001</UINT64>");
	TS_ASSERT(_doubleVar.ToString("sampleName", 1) == "    <DOUBLE name=\"sampleName\">123.456</DOUBLE>");
	TS_ASSERT(_dateVar.ToString("sampleName", 1) == "    <DATE name=\"sampleName\">1979-10-31</DATE>");
	TS_ASSERT(_timeVar.ToString("sampleName", 1) == "    <TIME name=\"sampleName\">12:13:14.000</TIME>");
	TS_ASSERT(_timestampVar1.ToString("sampleName", 1) == "    <TIMESTAMP name=\"sampleName\">1970-01-01T00:00:00.000</TIMESTAMP>");
	TS_ASSERT(_timestampVar2.ToString("sampleName", 1) == "    <TIMESTAMP name=\"sampleName\">1979-10-31T12:13:14.000</TIMESTAMP>");
	TS_ASSERT(_pcharVar.ToString("sampleName", 1) == "    <STR name=\"sampleName\">test</STR>");
	TS_ASSERT(_stringVar.ToString("sampleName", 1) == "    <STR name=\"sampleName\">test</STR>");
	TS_ASSERT(_mapVar.ToString("sampleName", 1) == "    <MAP name=\"sampleName\" isArray=\"false\">\n        <STR name=\"test\">test</STR>\n    </MAP>");
	TS_ASSERT(_namedMapVar.ToString("sampleName", 1) == "    <TYPED_MAP name=\"sampleName\" typename=\"test\" isArray=\"false\">\n        <STR name=\"test\">test</STR>\n    </TYPED_MAP>");
	TS_ASSERT(_referenceVariant1.ToString("sampleName", 1) == "    <NULL name=\"sampleName\"></NULL>");
	TS_ASSERT(_referenceVariant2.ToString("sampleName", 1) == "    <BOOL name=\"sampleName\">true</BOOL>");
	TS_ASSERT(_referenceVariant3.ToString("sampleName", 1) == "    <BOOL name=\"sampleName\">false</BOOL>");
	TS_ASSERT(_referenceVariant4.ToString("sampleName", 1) == "    <INT8 name=\"sampleName\">-1</INT8>");
	TS_ASSERT(_referenceVariant5.ToString("sampleName", 1) == "    <INT16 name=\"sampleName\">-2</INT16>");
	TS_ASSERT(_referenceVariant6.ToString("sampleName", 1) == "    <INT32 name=\"sampleName\">-3</INT32>");
	TS_ASSERT(_referenceVariant7.ToString("sampleName", 1) == "    <INT64 name=\"sampleName\">-4</INT64>");
	TS_ASSERT(_referenceVariant8.ToString("sampleName", 1) == "    <UINT8 name=\"sampleName\">129</UINT8>");
	TS_ASSERT(_referenceVariant9.ToString("sampleName", 1) == "    <UINT16 name=\"sampleName\">32769</UINT16>");
	TS_ASSERT(_referenceVariant10.ToString("sampleName", 1) == "    <UINT32 name=\"sampleName\">2147483649</UINT32>");
	TS_ASSERT(_referenceVariant11.ToString("sampleName", 1) == "    <UINT64 name=\"sampleName\">9223372036854780001</UINT64>");
	TS_ASSERT(_referenceVariant12.ToString("sampleName", 1) == "    <DOUBLE name=\"sampleName\">123.456</DOUBLE>");
	TS_ASSERT(_referenceVariant13.ToString("sampleName", 1) == "    <DATE name=\"sampleName\">1979-10-31</DATE>");
	TS_ASSERT(_referenceVariant14.ToString("sampleName", 1) == "    <TIME name=\"sampleName\">12:13:14.000</TIME>");
	TS_ASSERT(_referenceVariant15.ToString("sampleName", 1) == "    <TIMESTAMP name=\"sampleName\">1970-01-01T00:00:00.000</TIMESTAMP>");
	TS_ASSERT(_referenceVariant16.ToString("sampleName", 1) == "    <TIMESTAMP name=\"sampleName\">1979-10-31T12:13:14.000</TIMESTAMP>");
	TS_ASSERT(_referenceVariant17.ToString("sampleName", 1) == "    <STR name=\"sampleName\">test</STR>");
	TS_ASSERT(_referenceVariant18.ToString("sampleName", 1) == "    <STR name=\"sampleName\">test</STR>");
	TS_ASSERT(_referenceVariant19.ToString("sampleName", 1) == "    <MAP name=\"sampleName\" isArray=\"false\">\n        <STR name=\"test\">test</STR>\n    </MAP>");
	TS_ASSERT(_referenceVariant20.ToString("sampleName", 1) == "    <TYPED_MAP name=\"sampleName\" typename=\"test\" isArray=\"false\">\n        <STR name=\"test\">test</STR>\n    </TYPED_MAP>");

	TS_ASSERT(_noParamsVar.ToString("", 1) == "    <NULL name=\"\"></NULL>");
	TS_ASSERT(_boolVar1.ToString("", 1) == "    <BOOL name=\"\">true</BOOL>");
	TS_ASSERT(_boolVar2.ToString("", 1) == "    <BOOL name=\"\">false</BOOL>");
	TS_ASSERT(_int8Var.ToString("", 1) == "    <INT8 name=\"\">-1</INT8>");
	TS_ASSERT(_int16Var.ToString("", 1) == "    <INT16 name=\"\">-2</INT16>");
	TS_ASSERT(_int32Var.ToString("", 1) == "    <INT32 name=\"\">-3</INT32>");
	TS_ASSERT(_int64Var.ToString("", 1) == "    <INT64 name=\"\">-4</INT64>");
	TS_ASSERT(_uint8Var.ToString("", 1) == "    <UINT8 name=\"\">129</UINT8>");
	TS_ASSERT(_uint16Var.ToString("", 1) == "    <UINT16 name=\"\">32769</UINT16>");
	TS_ASSERT(_uint32Var.ToString("", 1) == "    <UINT32 name=\"\">2147483649</UINT32>");
	TS_ASSERT(_uint64Var.ToString("", 1) == "    <UINT64 name=\"\">9223372036854780001</UINT64>");
	TS_ASSERT(_doubleVar.ToString("", 1) == "    <DOUBLE name=\"\">123.456</DOUBLE>");
	TS_ASSERT(_dateVar.ToString("", 1) == "    <DATE name=\"\">1979-10-31</DATE>");
	TS_ASSERT(_timeVar.ToString("", 1) == "    <TIME name=\"\">12:13:14.000</TIME>");
	TS_ASSERT(_timestampVar1.ToString("", 1) == "    <TIMESTAMP name=\"\">1970-01-01T00:00:00.000</TIMESTAMP>");
	TS_ASSERT(_timestampVar2.ToString("", 1) == "    <TIMESTAMP name=\"\">1979-10-31T12:13:14.000</TIMESTAMP>");
	TS_ASSERT(_pcharVar.ToString("", 1) == "    <STR name=\"\">test</STR>");
	TS_ASSERT(_stringVar.ToString("", 1) == "    <STR name=\"\">test</STR>");
	TS_ASSERT(_mapVar.ToString("", 1) == "    <MAP name=\"\" isArray=\"false\">\n        <STR name=\"test\">test</STR>\n    </MAP>");
	TS_ASSERT(_namedMapVar.ToString("", 1) == "    <TYPED_MAP name=\"\" typename=\"test\" isArray=\"false\">\n        <STR name=\"test\">test</STR>\n    </TYPED_MAP>");
	TS_ASSERT(_referenceVariant1.ToString("", 1) == "    <NULL name=\"\"></NULL>");
	TS_ASSERT(_referenceVariant2.ToString("", 1) == "    <BOOL name=\"\">true</BOOL>");
	TS_ASSERT(_referenceVariant3.ToString("", 1) == "    <BOOL name=\"\">false</BOOL>");
	TS_ASSERT(_referenceVariant4.ToString("", 1) == "    <INT8 name=\"\">-1</INT8>");
	TS_ASSERT(_referenceVariant5.ToString("", 1) == "    <INT16 name=\"\">-2</INT16>");
	TS_ASSERT(_referenceVariant6.ToString("", 1) == "    <INT32 name=\"\">-3</INT32>");
	TS_ASSERT(_referenceVariant7.ToString("", 1) == "    <INT64 name=\"\">-4</INT64>");
	TS_ASSERT(_referenceVariant8.ToString("", 1) == "    <UINT8 name=\"\">129</UINT8>");
	TS_ASSERT(_referenceVariant9.ToString("", 1) == "    <UINT16 name=\"\">32769</UINT16>");
	TS_ASSERT(_referenceVariant10.ToString("", 1) == "    <UINT32 name=\"\">2147483649</UINT32>");
	TS_ASSERT(_referenceVariant11.ToString("", 1) == "    <UINT64 name=\"\">9223372036854780001</UINT64>");
	TS_ASSERT(_referenceVariant12.ToString("", 1) == "    <DOUBLE name=\"\">123.456</DOUBLE>");
	TS_ASSERT(_referenceVariant13.ToString("", 1) == "    <DATE name=\"\">1979-10-31</DATE>");
	TS_ASSERT(_referenceVariant14.ToString("", 1) == "    <TIME name=\"\">12:13:14.000</TIME>");
	TS_ASSERT(_referenceVariant15.ToString("", 1) == "    <TIMESTAMP name=\"\">1970-01-01T00:00:00.000</TIMESTAMP>");
	TS_ASSERT(_referenceVariant16.ToString("", 1) == "    <TIMESTAMP name=\"\">1979-10-31T12:13:14.000</TIMESTAMP>");
	TS_ASSERT(_referenceVariant17.ToString("", 1) == "    <STR name=\"\">test</STR>");
	TS_ASSERT(_referenceVariant18.ToString("", 1) == "    <STR name=\"\">test</STR>");
	TS_ASSERT(_referenceVariant19.ToString("", 1) == "    <MAP name=\"\" isArray=\"false\">\n        <STR name=\"test\">test</STR>\n    </MAP>");
	TS_ASSERT(_referenceVariant20.ToString("", 1) == "    <TYPED_MAP name=\"\" typename=\"test\" isArray=\"false\">\n        <STR name=\"test\">test</STR>\n    </TYPED_MAP>");

	TS_ASSERT(_noParamsVar.ToString("sampleName", 0) == "<NULL name=\"sampleName\"></NULL>");
	TS_ASSERT(_boolVar1.ToString("sampleName", 0) == "<BOOL name=\"sampleName\">true</BOOL>");
	TS_ASSERT(_boolVar2.ToString("sampleName", 0) == "<BOOL name=\"sampleName\">false</BOOL>");
	TS_ASSERT(_int8Var.ToString("sampleName", 0) == "<INT8 name=\"sampleName\">-1</INT8>");
	TS_ASSERT(_int16Var.ToString("sampleName", 0) == "<INT16 name=\"sampleName\">-2</INT16>");
	TS_ASSERT(_int32Var.ToString("sampleName", 0) == "<INT32 name=\"sampleName\">-3</INT32>");
	TS_ASSERT(_int64Var.ToString("sampleName", 0) == "<INT64 name=\"sampleName\">-4</INT64>");
	TS_ASSERT(_uint8Var.ToString("sampleName", 0) == "<UINT8 name=\"sampleName\">129</UINT8>");
	TS_ASSERT(_uint16Var.ToString("sampleName", 0) == "<UINT16 name=\"sampleName\">32769</UINT16>");
	TS_ASSERT(_uint32Var.ToString("sampleName", 0) == "<UINT32 name=\"sampleName\">2147483649</UINT32>");
	TS_ASSERT(_uint64Var.ToString("sampleName", 0) == "<UINT64 name=\"sampleName\">9223372036854780001</UINT64>");
	TS_ASSERT(_doubleVar.ToString("sampleName", 0) == "<DOUBLE name=\"sampleName\">123.456</DOUBLE>");
	TS_ASSERT(_dateVar.ToString("sampleName", 0) == "<DATE name=\"sampleName\">1979-10-31</DATE>");
	TS_ASSERT(_timeVar.ToString("sampleName", 0) == "<TIME name=\"sampleName\">12:13:14.000</TIME>");
	TS_ASSERT(_timestampVar1.ToString("sampleName", 0) == "<TIMESTAMP name=\"sampleName\">1970-01-01T00:00:00.000</TIMESTAMP>");
	TS_ASSERT(_timestampVar2.ToString("sampleName", 0) == "<TIMESTAMP name=\"sampleName\">1979-10-31T12:13:14.000</TIMESTAMP>");
	TS_ASSERT(_pcharVar.ToString("sampleName", 0) == "<STR name=\"sampleName\">test</STR>");
	TS_ASSERT(_stringVar.ToString("sampleName", 0) == "<STR name=\"sampleName\">test</STR>");
	TS_ASSERT(_mapVar.ToString("sampleName", 0) == "<MAP name=\"sampleName\" isArray=\"false\">\n    <STR name=\"test\">test</STR>\n</MAP>");
	TS_ASSERT(_namedMapVar.ToString("sampleName", 0) == "<TYPED_MAP name=\"sampleName\" typename=\"test\" isArray=\"false\">\n    <STR name=\"test\">test</STR>\n</TYPED_MAP>");
	TS_ASSERT(_referenceVariant1.ToString("sampleName", 0) == "<NULL name=\"sampleName\"></NULL>");
	TS_ASSERT(_referenceVariant2.ToString("sampleName", 0) == "<BOOL name=\"sampleName\">true</BOOL>");
	TS_ASSERT(_referenceVariant3.ToString("sampleName", 0) == "<BOOL name=\"sampleName\">false</BOOL>");
	TS_ASSERT(_referenceVariant4.ToString("sampleName", 0) == "<INT8 name=\"sampleName\">-1</INT8>");
	TS_ASSERT(_referenceVariant5.ToString("sampleName", 0) == "<INT16 name=\"sampleName\">-2</INT16>");
	TS_ASSERT(_referenceVariant6.ToString("sampleName", 0) == "<INT32 name=\"sampleName\">-3</INT32>");
	TS_ASSERT(_referenceVariant7.ToString("sampleName", 0) == "<INT64 name=\"sampleName\">-4</INT64>");
	TS_ASSERT(_referenceVariant8.ToString("sampleName", 0) == "<UINT8 name=\"sampleName\">129</UINT8>");
	TS_ASSERT(_referenceVariant9.ToString("sampleName", 0) == "<UINT16 name=\"sampleName\">32769</UINT16>");
	TS_ASSERT(_referenceVariant10.ToString("sampleName", 0) == "<UINT32 name=\"sampleName\">2147483649</UINT32>");
	TS_ASSERT(_referenceVariant11.ToString("sampleName", 0) == "<UINT64 name=\"sampleName\">9223372036854780001</UINT64>");
	TS_ASSERT(_referenceVariant12.ToString("sampleName", 0) == "<DOUBLE name=\"sampleName\">123.456</DOUBLE>");
	TS_ASSERT(_referenceVariant13.ToString("sampleName", 0) == "<DATE name=\"sampleName\">1979-10-31</DATE>");
	TS_ASSERT(_referenceVariant14.ToString("sampleName", 0) == "<TIME name=\"sampleName\">12:13:14.000</TIME>");
	TS_ASSERT(_referenceVariant15.ToString("sampleName", 0) == "<TIMESTAMP name=\"sampleName\">1970-01-01T00:00:00.000</TIMESTAMP>");
	TS_ASSERT(_referenceVariant16.ToString("sampleName", 0) == "<TIMESTAMP name=\"sampleName\">1979-10-31T12:13:14.000</TIMESTAMP>");
	TS_ASSERT(_referenceVariant17.ToString("sampleName", 0) == "<STR name=\"sampleName\">test</STR>");
	TS_ASSERT(_referenceVariant18.ToString("sampleName", 0) == "<STR name=\"sampleName\">test</STR>");
	TS_ASSERT(_referenceVariant19.ToString("sampleName", 0) == "<MAP name=\"sampleName\" isArray=\"false\">\n    <STR name=\"test\">test</STR>\n</MAP>");
	TS_ASSERT(_referenceVariant20.ToString("sampleName", 0) == "<TYPED_MAP name=\"sampleName\" typename=\"test\" isArray=\"false\">\n    <STR name=\"test\">test</STR>\n</TYPED_MAP>");
}

void VariantTestsSuite::test_OperatorAssign() {
#define test_OperatorAssign_int_def(ctype,vtype,value) \
    { \
        Variant variant = value; \
        TS_ASSERT(variant == vtype); \
        TS_ASSERT((ctype) variant == value); \
    }

#define test_OperatorAssign_var_def(value) \
    { \
        Variant variant = value; \
        TS_ASSERT((VariantType)variant == (VariantType)value); \
        TS_ASSERT(variant == value); \
    }


	test_OperatorAssign_int_def(bool, V_BOOL, (bool)true);
	test_OperatorAssign_int_def(bool, V_BOOL, (bool)false);
	test_OperatorAssign_int_def(int64_t, V_INT64, di64_1);
	test_OperatorAssign_int_def(int32_t, V_INT32, di32_1);
	test_OperatorAssign_int_def(int16_t, V_INT16, di16_1);
	test_OperatorAssign_int_def(int8_t, V_INT8, di8_1);
	test_OperatorAssign_int_def(int8_t, V_INT8, di8_2);
	test_OperatorAssign_int_def(int8_t, V_INT8, di8_3);
	test_OperatorAssign_int_def(uint8_t, V_UINT8, dui8);
	test_OperatorAssign_int_def(int16_t, V_INT16, di16_2);
	test_OperatorAssign_int_def(uint16_t, V_UINT16, dui16);
	test_OperatorAssign_int_def(int32_t, V_INT32, di32_2);
	test_OperatorAssign_int_def(uint32_t, V_UINT32, dui32);
	test_OperatorAssign_int_def(int64_t, V_INT64, di64_2);
	test_OperatorAssign_int_def(uint64_t, V_UINT64, dui64);
	test_OperatorAssign_int_def(double, V_DOUBLE, 123.456);


	test_OperatorAssign_var_def(_noParamsVar);
	test_OperatorAssign_var_def(_boolVar1);
	test_OperatorAssign_var_def(_boolVar2);
	test_OperatorAssign_var_def(_int8Var);
	test_OperatorAssign_var_def(_int16Var);
	test_OperatorAssign_var_def(_int32Var);
	test_OperatorAssign_var_def(_int64Var);
	test_OperatorAssign_var_def(_uint8Var);
	test_OperatorAssign_var_def(_uint16Var);
	test_OperatorAssign_var_def(_uint32Var);
	test_OperatorAssign_var_def(_uint64Var);
	test_OperatorAssign_var_def(_doubleVar);
	test_OperatorAssign_var_def(_dateVar);
	test_OperatorAssign_var_def(_timeVar);
	test_OperatorAssign_var_def(_timestampVar1);
	test_OperatorAssign_var_def(_timestampVar2);
	test_OperatorAssign_var_def(_pcharVar);
	test_OperatorAssign_var_def(_stringVar);
	test_OperatorAssign_var_def(_mapVar);
	test_OperatorAssign_var_def(_namedMapVar);
	test_OperatorAssign_var_def(_referenceVariant1);
	test_OperatorAssign_var_def(_referenceVariant2);
	test_OperatorAssign_var_def(_referenceVariant3);
	test_OperatorAssign_var_def(_referenceVariant4);
	test_OperatorAssign_var_def(_referenceVariant5);
	test_OperatorAssign_var_def(_referenceVariant6);
	test_OperatorAssign_var_def(_referenceVariant7);
	test_OperatorAssign_var_def(_referenceVariant8);
	test_OperatorAssign_var_def(_referenceVariant9);
	test_OperatorAssign_var_def(_referenceVariant10);
	test_OperatorAssign_var_def(_referenceVariant11);
	test_OperatorAssign_var_def(_referenceVariant12);
	test_OperatorAssign_var_def(_referenceVariant13);
	test_OperatorAssign_var_def(_referenceVariant14);
	test_OperatorAssign_var_def(_referenceVariant15);
	test_OperatorAssign_var_def(_referenceVariant16);
	test_OperatorAssign_var_def(_referenceVariant17);
	test_OperatorAssign_var_def(_referenceVariant18);
	test_OperatorAssign_var_def(_referenceVariant19);
	test_OperatorAssign_var_def(_referenceVariant20);

	{
		Variant variant = "This is a test";
		TS_ASSERT(variant == V_STRING);
		TS_ASSERT((string) variant == "This is a test");
	}

	{
		Variant variant = string("This is a test");
		TS_ASSERT(variant == V_STRING);
		TS_ASSERT((string) variant == "This is a test");
	}

	{
		Variant variant = (Timestamp) _timestampVar1;
		TS_ASSERT(variant == V_TIMESTAMP);
		string temp1 = "";
		TS_ASSERT(variant.SerializeToBin(temp1));
		string temp2 = "";
		TS_ASSERT(_timestampVar1.SerializeToBin(temp2));
		TS_ASSERT(temp1 == temp2);
	}
}

void VariantTestsSuite::test_OperatorCast() {
#define test_OperatorCast_line_def(subject,op,value) TS_ASSERT((op)subject==value);
	test_OperatorCast_line_def(_noParamsVar, VariantType, V_NULL);
	test_OperatorCast_line_def(_boolVar1, VariantType, V_BOOL);
	test_OperatorCast_line_def(_boolVar2, VariantType, V_BOOL);
	test_OperatorCast_line_def(_int8Var, VariantType, V_INT8);
	test_OperatorCast_line_def(_int16Var, VariantType, V_INT16);
	test_OperatorCast_line_def(_int32Var, VariantType, V_INT32);
	test_OperatorCast_line_def(_int64Var, VariantType, V_INT64);
	test_OperatorCast_line_def(_uint8Var, VariantType, V_UINT8);
	test_OperatorCast_line_def(_uint16Var, VariantType, V_UINT16);
	test_OperatorCast_line_def(_uint32Var, VariantType, V_UINT32);
	test_OperatorCast_line_def(_uint64Var, VariantType, V_UINT64);
	test_OperatorCast_line_def(_doubleVar, VariantType, V_DOUBLE);
	test_OperatorCast_line_def(_dateVar, VariantType, V_DATE);
	test_OperatorCast_line_def(_timeVar, VariantType, V_TIME);
	test_OperatorCast_line_def(_timestampVar1, VariantType, V_TIMESTAMP);
	test_OperatorCast_line_def(_timestampVar2, VariantType, V_TIMESTAMP);
	test_OperatorCast_line_def(_pcharVar, VariantType, V_STRING);
	test_OperatorCast_line_def(_stringVar, VariantType, V_STRING);
	test_OperatorCast_line_def(_mapVar, VariantType, V_MAP);
	test_OperatorCast_line_def(_namedMapVar, VariantType, V_TYPED_MAP);


	test_OperatorCast_line_def(_noParamsVar, bool, false);
	test_OperatorCast_line_def(_boolVar1, bool, true);
	test_OperatorCast_line_def(_boolVar2, bool, false);
	test_OperatorCast_line_def(_int8Var, bool, true);
	test_OperatorCast_line_def(_int16Var, bool, true);
	test_OperatorCast_line_def(_int32Var, bool, true);
	test_OperatorCast_line_def(_int64Var, bool, true);
	test_OperatorCast_line_def(_uint8Var, bool, true);
	test_OperatorCast_line_def(_uint16Var, bool, true);
	test_OperatorCast_line_def(_uint32Var, bool, true);
	test_OperatorCast_line_def(_uint64Var, bool, true);
	test_OperatorCast_line_def(_doubleVar, bool, true);

	test_OperatorCast_line_def(_noParamsVar, int8_t, 0);
	test_OperatorCast_line_def(_boolVar1, int8_t, 1);
	test_OperatorCast_line_def(_boolVar2, int8_t, 0);
	test_OperatorCast_line_def(_int8Var, int8_t, -1);
	test_OperatorCast_line_def(_int16Var, int8_t, -2);
	test_OperatorCast_line_def(_int32Var, int8_t, -3);
	test_OperatorCast_line_def(_int64Var, int8_t, -4);
	test_OperatorCast_line_def(_uint8Var, int8_t, -127);
	test_OperatorCast_line_def(_uint16Var, int8_t, 1);
	test_OperatorCast_line_def(_uint32Var, int8_t, 1);
	test_OperatorCast_line_def(_uint64Var, int8_t, 97);
	test_OperatorCast_line_def(_doubleVar, int8_t, 123);

	test_OperatorCast_line_def(_noParamsVar, int16_t, 0);
	test_OperatorCast_line_def(_boolVar1, int16_t, 1);
	test_OperatorCast_line_def(_boolVar2, int16_t, 0);
	test_OperatorCast_line_def(_int8Var, int16_t, -1);
	test_OperatorCast_line_def(_int16Var, int16_t, -2);
	test_OperatorCast_line_def(_int32Var, int16_t, -3);
	test_OperatorCast_line_def(_int64Var, int16_t, -4);
	test_OperatorCast_line_def(_uint8Var, int16_t, 129);
	test_OperatorCast_line_def(_uint16Var, int16_t, -32767);
	test_OperatorCast_line_def(_uint32Var, int16_t, 1);
	test_OperatorCast_line_def(_uint64Var, int16_t, 4193);
	test_OperatorCast_line_def(_doubleVar, int16_t, 123);

	test_OperatorCast_line_def(_noParamsVar, int32_t, 0);
	test_OperatorCast_line_def(_boolVar1, int32_t, 1);
	test_OperatorCast_line_def(_boolVar2, int32_t, 0);
	test_OperatorCast_line_def(_int8Var, int32_t, -1);
	test_OperatorCast_line_def(_int16Var, int32_t, -2);
	test_OperatorCast_line_def(_int32Var, int32_t, -3);
	test_OperatorCast_line_def(_int64Var, int32_t, -4);
	test_OperatorCast_line_def(_uint8Var, int32_t, 129);
	test_OperatorCast_line_def(_uint16Var, int32_t, 32769);
	test_OperatorCast_line_def(_uint32Var, int32_t, -2147483647);
	test_OperatorCast_line_def(_uint64Var, int32_t, 4193);
	test_OperatorCast_line_def(_doubleVar, int32_t, 123);

	test_OperatorCast_line_def(_noParamsVar, int64_t, 0);
	test_OperatorCast_line_def(_boolVar1, int64_t, 1);
	test_OperatorCast_line_def(_boolVar2, int64_t, 0);
	test_OperatorCast_line_def(_int8Var, int64_t, -1);
	test_OperatorCast_line_def(_int16Var, int64_t, -2);
	test_OperatorCast_line_def(_int32Var, int64_t, -3);
	test_OperatorCast_line_def(_int64Var, int64_t, -4);
	test_OperatorCast_line_def(_uint8Var, int64_t, 129);
	test_OperatorCast_line_def(_uint16Var, int64_t, 32769);
	test_OperatorCast_line_def(_uint32Var, int64_t, 2147483649LL);
	test_OperatorCast_line_def(_uint64Var, int64_t, -9223372036854771615LL);
	test_OperatorCast_line_def(_doubleVar, int64_t, 123);


	test_OperatorCast_line_def(_noParamsVar, uint8_t, 0);
	test_OperatorCast_line_def(_boolVar1, uint8_t, 1);
	test_OperatorCast_line_def(_boolVar2, uint8_t, 0);
	test_OperatorCast_line_def(_int8Var, uint8_t, 255);
	test_OperatorCast_line_def(_int16Var, uint8_t, 254);
	test_OperatorCast_line_def(_int32Var, uint8_t, 253);
	test_OperatorCast_line_def(_int64Var, uint8_t, 252);
	test_OperatorCast_line_def(_uint8Var, uint8_t, 129);
	test_OperatorCast_line_def(_uint16Var, uint8_t, 1);
	test_OperatorCast_line_def(_uint32Var, uint8_t, 1);
	test_OperatorCast_line_def(_uint64Var, uint8_t, 97);
	test_OperatorCast_line_def(_doubleVar, uint8_t, 123);

	test_OperatorCast_line_def(_noParamsVar, uint16_t, 0);
	test_OperatorCast_line_def(_boolVar1, uint16_t, 1);
	test_OperatorCast_line_def(_boolVar2, uint16_t, 0);
	test_OperatorCast_line_def(_int8Var, uint16_t, 65535);
	test_OperatorCast_line_def(_int16Var, uint16_t, 65534);
	test_OperatorCast_line_def(_int32Var, uint16_t, 65533);
	test_OperatorCast_line_def(_int64Var, uint16_t, 65532);
	test_OperatorCast_line_def(_uint8Var, uint16_t, 129);
	test_OperatorCast_line_def(_uint16Var, uint16_t, 32769);
	test_OperatorCast_line_def(_uint32Var, uint16_t, 1);
	test_OperatorCast_line_def(_uint64Var, uint16_t, 4193);
	test_OperatorCast_line_def(_doubleVar, uint16_t, 123);

	test_OperatorCast_line_def(_noParamsVar, uint32_t, 0);
	test_OperatorCast_line_def(_boolVar1, uint32_t, 1);
	test_OperatorCast_line_def(_boolVar2, uint32_t, 0);
	test_OperatorCast_line_def(_int8Var, uint32_t, 4294967295UL);
	test_OperatorCast_line_def(_int16Var, uint32_t, 4294967294UL);
	test_OperatorCast_line_def(_int32Var, uint32_t, 4294967293UL);
	test_OperatorCast_line_def(_int64Var, uint32_t, 4294967292UL);
	test_OperatorCast_line_def(_uint8Var, uint32_t, 129);
	test_OperatorCast_line_def(_uint16Var, uint32_t, 32769);
	test_OperatorCast_line_def(_uint32Var, uint32_t, 2147483649UL);
	test_OperatorCast_line_def(_uint64Var, uint32_t, 4193);
	test_OperatorCast_line_def(_doubleVar, uint32_t, 123);

	test_OperatorCast_line_def(_noParamsVar, uint64_t, 0);
	test_OperatorCast_line_def(_boolVar1, uint64_t, 1);
	test_OperatorCast_line_def(_boolVar2, uint64_t, 0);
	test_OperatorCast_line_def(_int8Var, uint64_t, 18446744073709551615ULL);
	test_OperatorCast_line_def(_int16Var, uint64_t, 18446744073709551614ULL);
	test_OperatorCast_line_def(_int32Var, uint64_t, 18446744073709551613ULL);
	test_OperatorCast_line_def(_int64Var, uint64_t, 18446744073709551612ULL);
	test_OperatorCast_line_def(_uint8Var, uint64_t, 129);
	test_OperatorCast_line_def(_uint16Var, uint64_t, 32769);
	test_OperatorCast_line_def(_uint32Var, uint64_t, 2147483649UL);
	test_OperatorCast_line_def(_uint64Var, uint64_t, 9223372036854780001ULL);
	test_OperatorCast_line_def(_doubleVar, uint64_t, 123);

	test_OperatorCast_line_def(_noParamsVar, double, 0.0);
	test_OperatorCast_line_def(_boolVar1, double, 1.0);
	test_OperatorCast_line_def(_boolVar2, double, 0.0);
	test_OperatorCast_line_def(_int8Var, double, -1.0);
	test_OperatorCast_line_def(_int16Var, double, -2.0);
	test_OperatorCast_line_def(_int32Var, double, -3.0);
	test_OperatorCast_line_def(_int64Var, double, -4.0);
	test_OperatorCast_line_def(_uint8Var, double, 129);
	test_OperatorCast_line_def(_uint16Var, double, 32769);
	test_OperatorCast_line_def(_uint32Var, double, 2147483649.0);
	test_OperatorCast_line_def(_uint64Var, double, 9223372036854780001.0);
	test_OperatorCast_line_def(_doubleVar, double, 123.456);


	test_OperatorCast_line_def(_boolVar1, string, "true");
	test_OperatorCast_line_def(_boolVar2, string, "false");
	test_OperatorCast_line_def(_int8Var, string, "-1");
	test_OperatorCast_line_def(_int16Var, string, "-2");
	test_OperatorCast_line_def(_int32Var, string, "-3");
	test_OperatorCast_line_def(_int64Var, string, "-4");
	test_OperatorCast_line_def(_uint8Var, string, "129");
	test_OperatorCast_line_def(_uint16Var, string, "32769");
	test_OperatorCast_line_def(_uint32Var, string, "2147483649");
	test_OperatorCast_line_def(_uint64Var, string, "9223372036854780001");
	test_OperatorCast_line_def(_doubleVar, string, "123.456");
	test_OperatorCast_line_def(_stringVar, string, "test");
	test_OperatorCast_line_def(_dateVar, string, "1979-10-31");
	test_OperatorCast_line_def(_timeVar, string, "12:13:14.000");
	test_OperatorCast_line_def(_timestampVar1, string, "1970-01-01T00:00:00.000");
	test_OperatorCast_line_def(_timestampVar2, string, "1979-10-31T12:13:14.000");

	Timestamp test = (Timestamp) _dateVar;
	TS_ASSERT(test.tm_year == 79);
	TS_ASSERT(test.tm_mon == 9);
	TS_ASSERT(test.tm_mday == 31);
	TS_ASSERT(test.tm_wday == 3);
	TS_ASSERT(test.tm_yday == 303);
	TS_ASSERT(test.tm_hour == 0);
	TS_ASSERT(test.tm_min == 0);
	TS_ASSERT(test.tm_sec == 0);
	TS_ASSERT(test.tm_isdst == 0);
#ifndef SOLARIS
#ifndef WIN32
	TS_ASSERT(test.tm_gmtoff == 0);
#endif
#endif

	test = (Timestamp) _timeVar;
	TS_ASSERT(test.tm_year == 70);
	TS_ASSERT(test.tm_mon == 0);
	TS_ASSERT(test.tm_mday == 1);
	TS_ASSERT(test.tm_wday == 4);
	TS_ASSERT(test.tm_yday == 0);
	TS_ASSERT(test.tm_hour == 12);
	TS_ASSERT(test.tm_min == 13);
	TS_ASSERT(test.tm_sec == 14);
	TS_ASSERT(test.tm_isdst == 0);
#ifndef SOLARIS
#ifndef WIN32
	TS_ASSERT(test.tm_gmtoff == 0);
#endif
#endif

	test = (Timestamp) _timestampVar1;
	TS_ASSERT(test.tm_year == 70);
	TS_ASSERT(test.tm_mon == 0);
	TS_ASSERT(test.tm_mday == 1);
	TS_ASSERT(test.tm_wday == 4);
	TS_ASSERT(test.tm_yday == 0);
	TS_ASSERT(test.tm_hour == 0);
	TS_ASSERT(test.tm_min == 0);
	TS_ASSERT(test.tm_sec == 0);
	TS_ASSERT(test.tm_isdst == 0);
#ifndef SOLARIS
#ifndef WIN32
	TS_ASSERT(test.tm_gmtoff == 0);
#endif
#endif

	test = (Timestamp) _timestampVar2;
	TS_ASSERT(test.tm_year == 79);
	TS_ASSERT(test.tm_mon == 9);
	TS_ASSERT(test.tm_mday == 31);
	TS_ASSERT(test.tm_wday == 3);
	TS_ASSERT(test.tm_yday == 303);
	TS_ASSERT(test.tm_hour == 12);
	TS_ASSERT(test.tm_min == 13);
	TS_ASSERT(test.tm_sec == 14);
	TS_ASSERT(test.tm_isdst == 0);
#ifndef SOLARIS
#ifndef WIN32
	TS_ASSERT(test.tm_gmtoff == 0);
#endif
#endif
}

void VariantTestsSuite::test_Compact() {
#define compact(v,r) { \
        Variant v_copy=v; \
        Variant r_copy=r; \
        v_copy.Compact(); \
        string r_string=""; \
        TS_ASSERT(r_copy.SerializeToBin(r_string)); \
        string v_string = ""; \
        TS_ASSERT(v_copy.SerializeToBin(v_string)); \
        TS_ASSERT(r_string == v_string); \
    }

	Variant reference;
	reference["n01"] = di64_1;
	reference["n02"] = di32_1;
	reference["n03"] = di16_1;
	reference["n04"] = di8_1;
	reference["n05"] = di8_2;
	reference["n06"] = di8_3;
	reference["n07"] = dui8;
	reference["n08"] = di16_2;
	reference["n09"] = dui16;
	reference["n10"] = di32_2;
	reference["n11"] = dui32;
	reference["n12"] = di64_2;
	reference["n13"] = dui64;


	Variant v_double;
	v_double["n01"] = di64_1;
	v_double["n02"] = (double) di32_1;
	v_double["n03"] = (double) di16_1;
	v_double["n04"] = (double) di8_1;
	v_double["n05"] = (double) di8_2;
	v_double["n06"] = (double) di8_3;
	v_double["n07"] = (double) dui8;
	v_double["n08"] = (double) di16_2;
	v_double["n09"] = (double) dui16;
	v_double["n10"] = (double) di32_2;
	v_double["n11"] = (double) dui32;
	v_double["n12"] = di64_2;
	v_double["n13"] = dui64;
	compact(v_double, reference);


	Variant v_i64;
	v_i64["n01"] = (int64_t) di64_1;
	v_i64["n02"] = (int64_t) di32_1;
	v_i64["n03"] = (int64_t) di16_1;
	v_i64["n04"] = (int64_t) di8_1;
	v_i64["n05"] = (int64_t) di8_2;
	v_i64["n06"] = (int64_t) di8_3;
	v_i64["n07"] = (int64_t) dui8;
	v_i64["n08"] = (int64_t) di16_2;
	v_i64["n09"] = (int64_t) dui16;
	v_i64["n10"] = (int64_t) di32_2;
	v_i64["n11"] = (int64_t) dui32;
	v_i64["n12"] = (int64_t) di64_2;
	v_i64["n13"] = dui64;
	compact(v_i64, reference);

	Variant v_i32;
	v_i32["n01"] = di64_1;
	v_i32["n02"] = (int32_t) di32_1;
	v_i32["n03"] = (int32_t) di16_1;
	v_i32["n04"] = (int32_t) di8_1;
	v_i32["n05"] = (int32_t) di8_2;
	v_i32["n06"] = (int32_t) di8_3;
	v_i32["n07"] = (int32_t) dui8;
	v_i32["n08"] = (int32_t) di16_2;
	v_i32["n09"] = (int32_t) dui16;
	v_i32["n10"] = (int32_t) di32_2;
	v_i32["n11"] = dui32;
	v_i32["n12"] = di64_2;
	v_i32["n13"] = dui64;
	compact(v_i32, reference);

	Variant v_i16;
	v_i16["n01"] = di64_1;
	v_i16["n02"] = di32_1;
	v_i16["n03"] = (int16_t) di16_1;
	v_i16["n04"] = (int16_t) di8_1;
	v_i16["n05"] = (int16_t) di8_2;
	v_i16["n06"] = (int16_t) di8_3;
	v_i16["n07"] = (int16_t) dui8;
	v_i16["n08"] = (int16_t) di16_2;
	v_i16["n09"] = dui16;
	v_i16["n10"] = di32_2;
	v_i16["n11"] = dui32;
	v_i16["n12"] = di64_2;
	v_i16["n13"] = dui64;
	compact(v_i16, reference);

	Variant v_i8;
	v_i8["n01"] = di64_1;
	v_i8["n02"] = di32_1;
	v_i8["n03"] = di16_1;
	v_i8["n04"] = di8_1;
	v_i8["n05"] = di8_2;
	v_i8["n06"] = di8_3;
	v_i8["n07"] = dui8;
	v_i8["n08"] = di16_2;
	v_i8["n09"] = dui16;
	v_i8["n10"] = di32_2;
	v_i8["n11"] = dui32;
	v_i8["n12"] = di64_2;
	v_i8["n13"] = dui64;
	compact(v_i8, reference);

	Variant v_ui64;
	v_ui64["n01"] = di64_1;
	v_ui64["n02"] = di32_1;
	v_ui64["n03"] = di16_1;
	v_ui64["n04"] = di8_1;
	v_ui64["n05"] = (uint64_t) di8_2;
	v_ui64["n06"] = (uint64_t) di8_3;
	v_ui64["n07"] = (uint64_t) dui8;
	v_ui64["n08"] = (uint64_t) di16_2;
	v_ui64["n09"] = (uint64_t) dui16;
	v_ui64["n10"] = (uint64_t) di32_2;
	v_ui64["n11"] = (uint64_t) dui32;
	v_ui64["n12"] = (uint64_t) di64_2;
	v_ui64["n13"] = (uint64_t) dui64;
	compact(v_ui64, reference);

	Variant v_ui32;
	v_ui32["n01"] = di64_1;
	v_ui32["n02"] = di32_1;
	v_ui32["n03"] = di16_1;
	v_ui32["n04"] = di8_1;
	v_ui32["n05"] = (uint32_t) di8_2;
	v_ui32["n06"] = (uint32_t) di8_3;
	v_ui32["n07"] = (uint32_t) dui8;
	v_ui32["n08"] = (uint32_t) di16_2;
	v_ui32["n09"] = (uint32_t) dui16;
	v_ui32["n10"] = (uint32_t) di32_2;
	v_ui32["n11"] = (uint32_t) dui32;
	v_ui32["n12"] = di64_2;
	v_ui32["n13"] = dui64;
	compact(v_ui32, reference);

	Variant v_ui16;
	v_ui16["n01"] = di64_1;
	v_ui16["n02"] = di32_1;
	v_ui16["n03"] = di16_1;
	v_ui16["n04"] = di8_1;
	v_ui16["n05"] = (uint16_t) di8_2;
	v_ui16["n06"] = (uint16_t) di8_3;
	v_ui16["n07"] = (uint16_t) dui8;
	v_ui16["n08"] = (uint16_t) di16_2;
	v_ui16["n09"] = (uint16_t) dui16;
	v_ui16["n10"] = di32_2;
	v_ui16["n11"] = dui32;
	v_ui16["n12"] = di64_2;
	v_ui16["n13"] = dui64;
	compact(v_ui16, reference);

	Variant v_ui8;
	v_ui8["n01"] = di64_1;
	v_ui8["n02"] = di32_1;
	v_ui8["n03"] = di16_1;
	v_ui8["n04"] = di8_1;
	v_ui8["n05"] = (uint8_t) di8_2;
	v_ui8["n06"] = (uint8_t) di8_3;
	v_ui8["n07"] = (uint8_t) dui8;
	v_ui8["n08"] = di16_2;
	v_ui8["n09"] = dui16;
	v_ui8["n10"] = di32_2;
	v_ui8["n11"] = dui32;
	v_ui8["n12"] = di64_2;
	v_ui8["n13"] = dui64;
	compact(v_ui8, reference);
}


