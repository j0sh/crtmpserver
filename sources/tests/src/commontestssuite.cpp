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


#include "commontestssuite.h"

CommonTestsSuite::CommonTestsSuite()
: BaseTestsSuite() {

}

CommonTestsSuite::~CommonTestsSuite() {

}

void CommonTestsSuite::Run() {
	test_Endianess();
	test_isNumeric();
	test_lowerCase();
	test_upperCase();
	test_ltrim();
	test_rtrim();
	test_trim();
	test_replace();
	test_split();
	test_mapping();
	test_format();
	test_splitFileName();
	test_generateRandomString();
	test_GetHostByName();
	test_md5();
	test_HMACsha256();
	test_b64();
	test_unb64();
	test_unhex();
	test_ParseURL();
	test_setFdOptions();
}

void CommonTestsSuite::test_Endianess() {
	uint16_t ui16 = 0x0102;
	uint32_t ui32 = 0x01020304;
	uint64_t ui64 = 0x0102030405060708LL;
	double d = 123.456;

	//host to network
	uint8_t *pBuffer = NULL;
	ui16 = EHTONS(ui16);
	pBuffer = (uint8_t *) & ui16;
	TS_ASSERT(pBuffer[0] == 0x01);
	TS_ASSERT(pBuffer[1] == 0x02);

	pBuffer = NULL;
	ui32 = EHTONL(ui32);
	pBuffer = (uint8_t *) & ui32;
	TS_ASSERT(pBuffer[0] == 0x01);
	TS_ASSERT(pBuffer[1] == 0x02);
	TS_ASSERT(pBuffer[2] == 0x03);
	TS_ASSERT(pBuffer[3] == 0x04);

	pBuffer = NULL;
	ui32 = 0x01020304;
	ui32 = EHTONA(ui32);
	pBuffer = (uint8_t *) & ui32;
	TS_ASSERT(pBuffer[0] == 0x02);
	TS_ASSERT(pBuffer[1] == 0x03);
	TS_ASSERT(pBuffer[2] == 0x04);
	TS_ASSERT(pBuffer[3] == 0x01);

	pBuffer = NULL;
	ui64 = EHTONLL(ui64);
	pBuffer = (uint8_t *) & ui64;
	TS_ASSERT(pBuffer[0] == 0x01);
	TS_ASSERT(pBuffer[1] == 0x02);
	TS_ASSERT(pBuffer[2] == 0x03);
	TS_ASSERT(pBuffer[3] == 0x04);
	TS_ASSERT(pBuffer[4] == 0x05);
	TS_ASSERT(pBuffer[5] == 0x06);
	TS_ASSERT(pBuffer[6] == 0x07);
	TS_ASSERT(pBuffer[7] == 0x08);

	pBuffer = NULL;
	EHTOND(d, ui64);
	pBuffer = (uint8_t *) & ui64;
	TS_ASSERT(pBuffer[0] == 0x40);
	TS_ASSERT(pBuffer[1] == 0x5e);
	TS_ASSERT(pBuffer[2] == 0xdd);
	TS_ASSERT(pBuffer[3] == 0x2f);
	TS_ASSERT(pBuffer[4] == 0x1a);
	TS_ASSERT(pBuffer[5] == 0x9f);
	TS_ASSERT(pBuffer[6] == 0xbe);
	TS_ASSERT(pBuffer[7] == 0x77);

	//network to host pointer
	char buffer[] = {
		0x00, 0x01, 0x02, 0x03,
		0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0a, 0x0b,
		0x0c, 0x0d, 0x0e, 0x0f,
		0x00, 0x01, 0x02, 0x03,
		0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0a, 0x0b,
		0x0c, 0x0d, 0x0e, 0x0f,
		0x00, 0x01, 0x02, 0x03,
		0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0a, 0x0b,
		0x0c, 0x0d, 0x0e, 0x0f,
		0x00, 0x01, 0x02, 0x03,
		0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0a, 0x0b,
		0x0c, 0x0d, 0x0e, 0x0f,
		0x00, 0x01, 0x02, 0x03,
		0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0a, 0x0b,
		0x0c, 0x0d, 0x0e, 0x0f,
		0x00, 0x01, 0x02, 0x03,
		0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0a, 0x0b,
		0x0c, 0x0d, 0x0e, 0x0f,
		0x00, 0x01, 0x02, 0x03,
		0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0a, 0x0b,
		0x0c, 0x0d, 0x0e, 0x0f,
		0x00, 0x01, 0x02, 0x03,
		0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0a, 0x0b,
		0x0c, 0x0d, 0x0e, 0x0f
	};

	ui16 = ENTOHSP(buffer);
	TS_ASSERT(ui16 == 0x0001);
	ui16 = ENTOHSP(buffer + 1);
	TS_ASSERT(ui16 == 0x0102);
	ui16 = ENTOHSP(buffer + 2);
	TS_ASSERT(ui16 == 0x0203);
	ui16 = ENTOHSP(buffer + 3);
	TS_ASSERT(ui16 == 0x0304);
	ui16 = ENTOHSP(buffer + 4);
	TS_ASSERT(ui16 == 0x0405);
	ui16 = ENTOHSP(buffer + 5);
	TS_ASSERT(ui16 == 0x0506);
	ui16 = ENTOHSP(buffer + 6);
	TS_ASSERT(ui16 == 0x0607);
	ui16 = ENTOHSP(buffer + 7);
	TS_ASSERT(ui16 == 0x0708);
	ui16 = ENTOHSP(buffer + 8);
	TS_ASSERT(ui16 == 0x0809);
	ui16 = ENTOHSP(buffer + 9);
	TS_ASSERT(ui16 == 0x090a);
	ui16 = ENTOHSP(buffer + 10);
	TS_ASSERT(ui16 == 0x0a0b);
	ui16 = ENTOHSP(buffer + 11);
	TS_ASSERT(ui16 == 0x0b0c);
	ui16 = ENTOHSP(buffer + 12);
	TS_ASSERT(ui16 == 0x0c0d);
	ui16 = ENTOHSP(buffer + 13);
	TS_ASSERT(ui16 == 0x0d0e);
	ui16 = ENTOHSP(buffer + 14);
	TS_ASSERT(ui16 == 0x0e0f);

	ui32 = ENTOHLP(buffer);
	TS_ASSERT(ui32 == 0x00010203);
	ui32 = ENTOHLP(buffer + 1);
	TS_ASSERT(ui32 == 0x01020304);
	ui32 = ENTOHLP(buffer + 2);
	TS_ASSERT(ui32 == 0x02030405);
	ui32 = ENTOHLP(buffer + 3);
	TS_ASSERT(ui32 == 0x03040506);
	ui32 = ENTOHLP(buffer + 4);
	TS_ASSERT(ui32 == 0x04050607);
	ui32 = ENTOHLP(buffer + 5);
	TS_ASSERT(ui32 == 0x05060708);
	ui32 = ENTOHLP(buffer + 6);
	TS_ASSERT(ui32 == 0x06070809);
	ui32 = ENTOHLP(buffer + 7);
	TS_ASSERT(ui32 == 0x0708090a);
	ui32 = ENTOHLP(buffer + 8);
	TS_ASSERT(ui32 == 0x08090a0b);
	ui32 = ENTOHLP(buffer + 9);
	TS_ASSERT(ui32 == 0x090a0b0c);
	ui32 = ENTOHLP(buffer + 10);
	TS_ASSERT(ui32 == 0x0a0b0c0d);
	ui32 = ENTOHLP(buffer + 11);
	TS_ASSERT(ui32 == 0x0b0c0d0e);
	ui32 = ENTOHLP(buffer + 12);
	TS_ASSERT(ui32 == 0x0c0d0e0f);

	ui32 = ENTOHAP(buffer);
	TS_ASSERT(ui32 == 0x03000102);
	ui32 = ENTOHAP(buffer + 1);
	TS_ASSERT(ui32 == 0x04010203);
	ui32 = ENTOHAP(buffer + 2);
	TS_ASSERT(ui32 == 0x05020304);
	ui32 = ENTOHAP(buffer + 3);
	TS_ASSERT(ui32 == 0x06030405);
	ui32 = ENTOHAP(buffer + 4);
	TS_ASSERT(ui32 == 0x07040506);
	ui32 = ENTOHAP(buffer + 5);
	TS_ASSERT(ui32 == 0x08050607);
	ui32 = ENTOHAP(buffer + 6);
	TS_ASSERT(ui32 == 0x09060708);
	ui32 = ENTOHAP(buffer + 7);
	TS_ASSERT(ui32 == 0x0a070809);
	ui32 = ENTOHAP(buffer + 8);
	TS_ASSERT(ui32 == 0x0b08090a);
	ui32 = ENTOHAP(buffer + 9);
	TS_ASSERT(ui32 == 0x0c090a0b);
	ui32 = ENTOHAP(buffer + 10);
	TS_ASSERT(ui32 == 0x0d0a0b0c);
	ui32 = ENTOHAP(buffer + 11);
	TS_ASSERT(ui32 == 0x0e0b0c0d);
	ui32 = ENTOHAP(buffer + 12);
	TS_ASSERT(ui32 == 0x0f0c0d0e);

	ui64 = ENTOHLLP(buffer);
	TS_ASSERT(ui64 == 0x0001020304050607LL);
	ui64 = ENTOHLLP(buffer + 1);
	TS_ASSERT(ui64 == 0x0102030405060708LL);
	ui64 = ENTOHLLP(buffer + 2);
	TS_ASSERT(ui64 == 0x0203040506070809LL);
	ui64 = ENTOHLLP(buffer + 3);
	TS_ASSERT(ui64 == 0x030405060708090aLL);
	ui64 = ENTOHLLP(buffer + 4);
	TS_ASSERT(ui64 == 0x0405060708090a0bLL);
	ui64 = ENTOHLLP(buffer + 5);
	TS_ASSERT(ui64 == 0x05060708090a0b0cLL);
	ui64 = ENTOHLLP(buffer + 6);
	TS_ASSERT(ui64 == 0x060708090a0b0c0dLL);
	ui64 = ENTOHLLP(buffer + 7);
	TS_ASSERT(ui64 == 0x0708090a0b0c0d0eLL);
	ui64 = ENTOHLLP(buffer + 8);
	TS_ASSERT(ui64 == 0x08090a0b0c0d0e0fLL);

	char *pTempBuffer = new char[64 + 8];
	unsigned char rawDouble[] = {0x40, 0x5E, 0xDD, 0x2F, 0x1A, 0x9F, 0xBE, 0x77};
	double tempDoubleVal = 0;
	for (int i = 0; i <= 64; i++) {
		memset(pTempBuffer, 0, i);
		memcpy(pTempBuffer + i, rawDouble, 8);
		memset(pTempBuffer + i + 8, 0, 64 + 8 - i - 8);
		ENTOHDP((pTempBuffer + i), tempDoubleVal);
		TS_ASSERT(d == tempDoubleVal);
	}
	delete[] pTempBuffer;

	//network to host
#ifdef LITTLE_ENDIAN_BYTE_ALIGNED
	TS_ASSERT(ENTOHA(0x01040302) == 0x01020304);
	TS_ASSERT(ENTOHLL(0x0807060504030201LL) == 0x0102030405060708LL);
	ENTOHD(0x77BE9F1A2FDD5E40LL, tempDoubleVal);
	TS_ASSERT(d == tempDoubleVal);
#endif /* LITTLE_ENDIAN_BYTE_ALIGNED */

#ifdef LITTLE_ENDIAN_SHORT_ALIGNED
	TS_ASSERT(ENTOHA(0x01040302) == 0x01020304);
	TS_ASSERT(ENTOHLL(0x0807060504030201LL) == 0x0102030405060708LL);
	ENTOHD(0x77BE9F1A2FDD5E40LL, tempDoubleVal);
	TS_ASSERT(d == tempDoubleVal);
#endif /* LITTLE_ENDIAN_SHORT_ALIGNED */

#ifdef BIG_ENDIAN_BYTE_ALIGNED
	TS_ASSERT(ENTOHA(0x02030401) == 0x01020304);
	TS_ASSERT(ENTOHLL(0x0102030405060708LL) == 0x0102030405060708LL);
	ENTOHD(0x405EDD2F1A9FBE77LL, tempDoubleVal);
	TS_ASSERT(d == tempDoubleVal);
#endif /* BIG_ENDIAN_BYTE_ALIGNED */

#ifdef BIG_ENDIAN_SHORT_ALIGNED
#error BIG_ENDIAN_SHORT_ALIGNED set of tests not yet implemented!!! Please take care of this first!!!
#endif /* BIG_ENDIAN_SHORT_ALIGNED */

	//double mirror
	TS_ASSERT(ENTOHS(EHTONS(0x0102)) == 0x0102);
	TS_ASSERT(EHTONS(ENTOHS(0x0102)) == 0x0102);

	TS_ASSERT(ENTOHL(EHTONL(0x01020304)) == 0x01020304);
	TS_ASSERT(EHTONL(ENTOHL(0x01020304)) == 0x01020304);

	TS_ASSERT(ENTOHLL(EHTONLL(0x0102030405060708LL)) == 0x0102030405060708LL);
	TS_ASSERT(EHTONLL(ENTOHLL(0x0102030405060708LL)) == 0x0102030405060708LL);

	//EHTOND/ENTOHD are different. Requires 2 parameters. So, no double mirror

	TS_ASSERT(ENTOHA(EHTONA(0x01020304)) == 0x01020304);
	TS_ASSERT(EHTONA(ENTOHA(0x01020304)) == 0x01020304);

	// Buffer Put routines
	for (int i = 0; i < 16; i++) {
		EHTONSP(buffer + i, 0x0102);
		TS_ASSERT(ENTOHSP(buffer + i) == 0x0102);

		EHTONLP(buffer + i, 0x01020304);
		TS_ASSERT(ENTOHLP(buffer + i) == 0x01020304);

		EHTONLLP(buffer + i, 0x0102030405060708LL);
		TS_ASSERT(ENTOHLLP(buffer + i) == 0x0102030405060708LL);

		EHTONDP(d, (buffer + i));
		ENTOHDP(buffer + i, tempDoubleVal);
		TS_ASSERT(d == tempDoubleVal);
	}
}

void CommonTestsSuite::test_isNumeric() {
	TS_ASSERT(!isNumeric(""));
	TS_ASSERT(!isNumeric(" "));
	TS_ASSERT(!isNumeric("sdfsdfsd"));
	TS_ASSERT(!isNumeric("0x1234"));
	TS_ASSERT(!isNumeric("4294967294"));
	TS_ASSERT(!isNumeric("4294967295"));
	TS_ASSERT(!isNumeric("4294967296"));
	TS_ASSERT(!isNumeric("2147483648"));
	TS_ASSERT(!isNumeric("-4294967294"));
	TS_ASSERT(!isNumeric("-4294967295"));
	TS_ASSERT(!isNumeric("-4294967296"));
	TS_ASSERT(isNumeric("2147483647"));
	TS_ASSERT(isNumeric("2147483646"));
	TS_ASSERT(isNumeric("1234"));
	TS_ASSERT(isNumeric("0"));
	TS_ASSERT(isNumeric("-1234"));
	TS_ASSERT(isNumeric("-2147483647"));
	TS_ASSERT(isNumeric("-2147483648"));
}

void CommonTestsSuite::test_lowerCase() {
	TS_ASSERT(lowerCase("TeXt") == "text");
	TS_ASSERT(lowerCase("TEXT") == "text");
	TS_ASSERT(lowerCase("text") == "text");
	TS_ASSERT(lowerCase("") == "");
	TS_ASSERT(lowerCase(" ") == " ");
	string testString;
	for (int c = -128; c <= 255; c++)
		testString += (char) c;
	string lowerString;
	for (int c = -128; c <= 255; c++) {
		if (c >= 65 && c <= 90)
			lowerString += (char) (c + 32);
		else
			lowerString += (char) c;
	}
	TS_ASSERT(testString != lowerString);
	TS_ASSERT(lowerCase(testString) == lowerString);
}

void CommonTestsSuite::test_upperCase() {
	TS_ASSERT(upperCase("TeXt") == "TEXT");
	TS_ASSERT(upperCase("TEXT") == "TEXT");
	TS_ASSERT(upperCase("text") == "TEXT");
	TS_ASSERT(upperCase("") == "");
	TS_ASSERT(upperCase(" ") == " ");
	string testString;
	for (int c = -128; c <= 255; c++)
		testString += (char) c;
	string upperString;
	for (int c = -128; c <= 255; c++) {
		if (c >= 97 && c <= 122)
			upperString += (char) (c - 32);
		else
			upperString += (char) c;
	}
	TS_ASSERT(testString != upperString);
	TS_ASSERT(upperCase(testString) == upperString);
}

void CommonTestsSuite::test_ltrim() {
	string str;

	str = "";
	lTrim(str);
	TS_ASSERT(str == "");

	str = " ";
	lTrim(str);
	TS_ASSERT(str == "");

	str = " b";
	lTrim(str);
	TS_ASSERT(str == "b");

	str = " bb";
	lTrim(str);
	TS_ASSERT(str == "bb");

	str = " bb ";
	lTrim(str);
	TS_ASSERT(str == "bb ");

	str = " bb b";
	lTrim(str);
	TS_ASSERT(str == "bb b");

	str = " bb b ";
	lTrim(str);
	TS_ASSERT(str == "bb b ");
}

void CommonTestsSuite::test_rtrim() {
	string str;

	str = "";
	rTrim(str);
	TS_ASSERT(str == "");

	str = " ";
	rTrim(str);
	TS_ASSERT(str == "");

	str = " b";
	rTrim(str);
	TS_ASSERT(str == " b");

	str = " bb";
	rTrim(str);
	TS_ASSERT(str == " bb");

	str = " bb ";
	rTrim(str);
	TS_ASSERT(str == " bb");

	str = " bb b";
	rTrim(str);
	TS_ASSERT(str == " bb b");

	str = " bb b ";
	rTrim(str);
	TS_ASSERT(str == " bb b");
}

void CommonTestsSuite::test_trim() {
	string str;

	str = "";
	trim(str);
	TS_ASSERT(str == "");

	str = " ";
	trim(str);
	TS_ASSERT(str == "");

	str = " b";
	trim(str);
	TS_ASSERT(str == "b");

	str = " bb";
	trim(str);
	TS_ASSERT(str == "bb");

	str = " bb ";
	trim(str);
	TS_ASSERT(str == "bb");

	str = " bb b";
	trim(str);
	TS_ASSERT(str == "bb b");

	str = " bb b ";
	trim(str);
	TS_ASSERT(str == "bb b");
}

void CommonTestsSuite::test_replace() {
	string str;

	str = "";
	replace(str, "", "");
	TS_ASSERT(str == "");

	str = "";
	replace(str, "test", "");
	TS_ASSERT(str == "");

	str = "";
	replace(str, "test", "TEST");
	TS_ASSERT(str == "");

	str = "";
	replace(str, "", "TEST");
	TS_ASSERT(str == "");

	str = "test string ";
	replace(str, "test", "");
	TS_ASSERT(str == " string ");

	str = "test string ";
	replace(str, "test", "TEST");
	TS_ASSERT(str == "TEST string ");

	str = "test string ";
	replace(str, "", "TEST");
	TS_ASSERT(str == "test string ");

	str = "aaa";
	replace(str, "a", "aa");
	TS_ASSERT(str == "aaaaaa");

	str = "aaaaa";
	replace(str, "aa", "a");
	TS_ASSERT(str == "aaa");

	str = "aaa";
	replace(str, "aa", "a");
	TS_ASSERT(str == "aa");
}

void CommonTestsSuite::test_split() {
	string input = "some::string:with:::a lot of : ::";
	vector<string> parts;
	split(input, ":", parts);
	TS_ASSERT(parts.size() == 10);
	TS_ASSERT(parts[0] == "some");
	TS_ASSERT(parts[1] == "");
	TS_ASSERT(parts[2] == "string");
	TS_ASSERT(parts[3] == "with");
	TS_ASSERT(parts[4] == "");
	TS_ASSERT(parts[5] == "");
	TS_ASSERT(parts[6] == "a lot of ");
	TS_ASSERT(parts[7] == " ");
	TS_ASSERT(parts[8] == "");
	TS_ASSERT(parts[9] == "");

	input = "some string split with s and g text...sg...";
	split(input, "sg", parts);
	TS_ASSERT(parts.size() == 2);
	TS_ASSERT(parts[0] == "some string split with s and g text...");
	TS_ASSERT(parts[1] == "...");

	input = "some string split with s and g text...sg";
	split(input, "sg", parts);
	TS_ASSERT(parts.size() == 2);
	TS_ASSERT(parts[0] == "some string split with s and g text...");
	TS_ASSERT(parts[1] == "");

	input = "some string split with s and g text...";
	split(input, "sg", parts);
	TS_ASSERT(parts.size() == 1);
	TS_ASSERT(parts[0] == "some string split with s and g text...");

	input = "::";
	split(input, ":", parts);
	TS_ASSERT(parts.size() == 3);
	TS_ASSERT(parts[0] == "");
	TS_ASSERT(parts[1] == "");
	TS_ASSERT(parts[2] == "");

	input = ":";
	split(input, ":", parts);
	TS_ASSERT(parts.size() == 2);
	TS_ASSERT(parts[0] == "");
	TS_ASSERT(parts[1] == "");

	input = "aaaa";
	split(input, ":", parts);
	TS_ASSERT(parts.size() == 1);
	TS_ASSERT(parts[0] == "aaaa");

	input = "";
	split(input, ":", parts);
	TS_ASSERT(parts.size() == 1);
	TS_ASSERT(parts[0] == "");
}

void CommonTestsSuite::test_mapping() {
	string line = "name1=value1;name2=value2;=value3;name4=;";
	map<string, string> m = mapping(line, ";", "=", false);
	TS_ASSERT(m.size() == 4);
	TS_ASSERT(m["name1"] == "value1");
	TS_ASSERT(m["name2"] == "value2");
	TS_ASSERT(m[""] == "value3");
	TS_ASSERT(m["name4"] == "");
	TS_ASSERT(m.size() == 4);

	line = "name1=value1;name2=value2;=value3;name4=";
	m = mapping(line, ";", "=", false);
	TS_ASSERT(m.size() == 4);
	TS_ASSERT(m["name1"] == "value1");
	TS_ASSERT(m["name2"] == "value2");
	TS_ASSERT(m[""] == "value3");
	TS_ASSERT(m["name4"] == "");
	TS_ASSERT(m.size() == 4);

	line = "name1=value1;name2=value2;=value3;name4=;;;";
	m = mapping(line, ";", "=", false);
	TS_ASSERT(m.size() == 4);
	TS_ASSERT(m["name1"] == "value1");
	TS_ASSERT(m["name2"] == "value2");
	TS_ASSERT(m[""] == "value3");
	TS_ASSERT(m["name4"] == "");
	TS_ASSERT(m.size() == 4);

	line = "name1=value1;name2=value2;=value3;name4=;;;name1=value11;";
	m = mapping(line, ";", "=", false);
	TS_ASSERT(m.size() == 4);
	TS_ASSERT(m["name1"] == "value11");
	TS_ASSERT(m["name2"] == "value2");
	TS_ASSERT(m[""] == "value3");
	TS_ASSERT(m["name4"] == "");
	TS_ASSERT(m.size() == 4);

	line = ";=;";
	m = mapping(line, ";", "=", false);
	TS_ASSERT(m.size() == 1);
	TS_ASSERT(m[""] == "");
	TS_ASSERT(m.size() == 1);

	line = "=;=";
	m = mapping(line, ";", "=", false);
	TS_ASSERT(m.size() == 1);
	TS_ASSERT(m[""] == "");
	TS_ASSERT(m.size() == 1);

	line = "aa = b b ; = ";
	m = mapping(line, ";", "=", true);
	TS_ASSERT(m.size() == 2);
	TS_ASSERT(m["aa"] == "b b");
	TS_ASSERT(m[""] == "");
	TS_ASSERT(m.size() == 2);
}

void CommonTestsSuite::test_format() {
	TS_ASSERT(format("%s%d%.2f", "test", -1, 123.45) == "test-1123.45");
}

void CommonTestsSuite::test_splitFileName() {
	string name;
	string extension;

	name = extension = "dummy";
	splitFileName("", name, extension);
	TS_ASSERT(name == "");
	TS_ASSERT(extension == "");

	name = extension = "dummy";
	splitFileName("/path/", name, extension);
	TS_ASSERT(name == "/path/");
	TS_ASSERT(extension == "");

	name = extension = "dummy";
	splitFileName("/path/file", name, extension);
	TS_ASSERT(name == "/path/file");
	TS_ASSERT(extension == "");

	name = extension = "dummy";
	splitFileName("/path/file.", name, extension);
	TS_ASSERT(name == "/path/file");
	TS_ASSERT(extension == "");

	name = extension = "dummy";
	splitFileName("/path/file.ext", name, extension);
	TS_ASSERT(name == "/path/file");
	TS_ASSERT(extension == "ext");

	name = extension = "dummy";
	splitFileName("/path/.ext", name, extension);
	TS_ASSERT(name == "/path/");
	TS_ASSERT(extension == "ext");

	name = extension = "dummy";
	splitFileName("/path/.", name, extension);
	TS_ASSERT(name == "/path/");
	TS_ASSERT(extension == "");

	name = extension = "dummy";
	splitFileName(".", name, extension);
	TS_ASSERT(name == "");
	TS_ASSERT(extension == "");
}

void CommonTestsSuite::test_generateRandomString() {
	map<string, string> strings;
	for (uint32_t i = 0; i < 16384; i++) {
		string randString = generateRandomString(16);
		//we don't want to increment tests count
		//That is why we use TS_ASSERT_NO_INCREMENT
		TS_ASSERT_NO_INCREMENT(randString != "");
		if (strings.find(randString) != strings.end()) {
			TS_ASSERT(false);
		}
		strings[randString];
	}
	//Ok, treat all this 16384 tests as one test by doing a dummy test
	TS_ASSERT(true);
}

void CommonTestsSuite::test_GetHostByName() {
	//TS_ASSERT(GetHostByName("___this_should_not_exist___") == "");
	//TS_ASSERT(GetHostByName("localhost") == "127.0.0.1");
	//TS_ASSERT(GetHostByName("127.0.0.1") == "127.0.0.1");
	//TS_ASSERT(GetHostByName("rtmpd.com") == "89.149.7.54");
}

void CommonTestsSuite::test_md5() {
	TS_ASSERT(md5("MD5 test string", true) == "da42a8cd6c584e577a85dd796ae75eef");
}

void CommonTestsSuite::test_HMACsha256() {
	string plain = "This is a test";
	string key = "test_key";
	uint8_t hash[32] = {0};
	uint8_t wanted[] = {
		0xA7, 0x97, 0x3B, 0x48, 0xB2, 0x92, 0x5A, 0x81,
		0xFD, 0x93, 0x41, 0x95, 0x48, 0x51, 0x91, 0x88,
		0xE4, 0xC5, 0xFB, 0xAE, 0x63, 0xD1, 0xF6, 0xF3,
		0x1F, 0x16, 0xF7, 0xC0, 0x9A, 0xD3, 0xC4, 0x07
	};
	HMACsha256(STR(plain), plain.length(), STR(key), key.length(), hash);
	for (uint32_t i = 0; i < 32; i++) {
		TS_ASSERT(hash[i] == wanted[i]);
	}
}

void CommonTestsSuite::test_b64() {
	TS_ASSERT(b64("this is a test1") == "dGhpcyBpcyBhIHRlc3Qx");
	TS_ASSERT(b64("this is a test") == "dGhpcyBpcyBhIHRlc3Q=");
	TS_ASSERT(b64("this is a tes") == "dGhpcyBpcyBhIHRlcw==");
	TS_ASSERT(b64("") == "");
}

void CommonTestsSuite::test_unb64() {
	TS_ASSERT(unb64("dGhpcyBpcyBhIHRlc3Qx") == "this is a test1");
	TS_ASSERT(unb64("dGhpcyBpcyBhIHRlc3Q=") == "this is a test");
	TS_ASSERT(unb64("dGhpcyBpcyBhIHRlcw==") == "this is a tes");
	TS_ASSERT(unb64("") == "");
}

void CommonTestsSuite::test_unhex() {
	string hexString;
	string bin;

	hexString = "";
	bin = unhex(hexString);
	TS_ASSERT(bin == "");

	hexString = "1";
	bin = unhex(hexString);
	TS_ASSERT(bin == "");

	hexString = "1non_permitted_characters";
	bin = unhex(hexString);
	TS_ASSERT(bin == "");

	hexString = "1non_permitted_characters1";
	bin = unhex(hexString);
	TS_ASSERT(bin == "");

	for (uint32_t i = 0; i <= 255; i++) {
		hexString = format("%02x", i);
		bin = unhex(hexString);
		TS_ASSERT(bin.length() == 1);
		TS_ASSERT((uint8_t) bin[0] == i);
	}

	for (uint32_t i = 1; i <= 255; i++) {
		hexString = "";
		for (uint32_t j = 0; j < i; j++) {
			hexString += format("%02x", j);
		}
		bin = unhex(hexString);
		if (bin.length() != i) {
			TS_ASSERT(bin.length() == i);
		}
		for (uint32_t j = 0; j < i; j++) {
			if ((uint8_t) bin[j] != j) {
				TS_ASSERT((uint8_t) bin[j] == j);
			}
		}
	}
}

void CommonTestsSuite::test_ParseURL() {
	//	URI uri;
	//
	//	uri.Reset();
	//	uri.port = 9999;
	//	TS_ASSERT(URI::FromString("", false, uri) == false);
	//	TS_ASSERT(uri.host == "");
	//	TS_ASSERT(uri.port == 0);
	//	TS_ASSERT(uri.userName == "");
	//	TS_ASSERT(uri.password == "");
	//	TS_ASSERT(uri.fullDocumentPath == "");
	//
	//	uri.Reset();
	//	uri.port = 9999;
	//	TS_ASSERT(URI::FromString("sjdhfkjsdhfjksdh", false, uri) == false);
	//	TS_ASSERT(uri.host == "");
	//	TS_ASSERT(uri.port == 0);
	//	TS_ASSERT(uri.userName == "");
	//	TS_ASSERT(uri.password == "");
	//	TS_ASSERT(uri.fullDocumentPath == "");
	//
	//	uri.Reset();
	//	uri.port = 9999;
	//	TS_ASSERT(URI::FromString("http:/gigi/stiintza", false, uri) == false);
	//	TS_ASSERT(uri.host == "");
	//	TS_ASSERT(uri.port == 0);
	//	TS_ASSERT(uri.userName == "");
	//	TS_ASSERT(uri.password == "");
	//	TS_ASSERT(uri.fullDocumentPath == "");
	//
	//	uri.Reset();
	//	uri.port = 9999;
	//	TS_ASSERT(URI::FromString("htt://gigi/stiintza", false, uri) == false);
	//	TS_ASSERT(uri.host == "");
	//	TS_ASSERT(uri.port == 0);
	//	TS_ASSERT(uri.userName == "");
	//	TS_ASSERT(uri.password == "");
	//	TS_ASSERT(uri.fullDocumentPath == "");
	//
	//	uri.Reset();
	//	uri.port = 9999;
	//	TS_ASSERT(URI::FromString("http://:gigi/stiintza", false, uri) == false);
	//	TS_ASSERT(uri.host == "");
	//	TS_ASSERT(uri.port == 0);
	//	TS_ASSERT(uri.userName == "");
	//	TS_ASSERT(uri.password == "");
	//	TS_ASSERT(uri.fullDocumentPath == "");
	//
	//	uri.Reset();
	//	uri.port = 9999;
	//	TS_ASSERT(URI::FromString("http://gigi:gfgf/stiintza", false, uri) == false);
	//	TS_ASSERT(uri.host == "");
	//	TS_ASSERT(uri.port == 0);
	//	TS_ASSERT(uri.userName == "");
	//	TS_ASSERT(uri.password == "");
	//	TS_ASSERT(uri.fullDocumentPath == "");
	//
	//	uri.Reset();
	//	uri.port = 9999;
	//	TS_ASSERT(URI::FromString("http://gigi:123456/stiintza", false, uri) == false);
	//	TS_ASSERT(uri.host == "");
	//	TS_ASSERT(uri.port == 0);
	//	TS_ASSERT(uri.userName == "");
	//	TS_ASSERT(uri.password == "");
	//	TS_ASSERT(uri.fullDocumentPath == "");
	//
	//
	//	uri.Reset();
	//	uri.port = 9999;
	//	TS_ASSERT(URI::FromString("http://:@gigi/stiintza", false, uri) == false);
	//	TS_ASSERT(uri.host == "");
	//	TS_ASSERT(uri.port == 0);
	//	TS_ASSERT(uri.userName == "");
	//	TS_ASSERT(uri.password == "");
	//	TS_ASSERT(uri.fullDocumentPath == "");
	//
	//	uri.Reset();
	//	uri.port = 9999;
	//	TS_ASSERT(URI::FromString("http://aaa:ddd@gigi/stiintza", false, uri) == true);
	//	TS_ASSERT(uri.host == "gigi");
	//	TS_ASSERT(uri.port == 80);
	//	TS_ASSERT(uri.userName == "aaa");
	//	TS_ASSERT(uri.password == "ddd");
	//	TS_ASSERT(uri.fullDocumentPath == "/stiintza");
	//
	//	uri.Reset();
	//	uri.port = 9999;
	//	TS_ASSERT(URI::FromString("http://gigi/stiintza", false, uri) == true);
	//	TS_ASSERT(uri.host == "gigi");
	//	TS_ASSERT(uri.port == 80);
	//	TS_ASSERT(uri.userName == "");
	//	TS_ASSERT(uri.password == "");
	//	TS_ASSERT(uri.fullDocumentPath == "/stiintza");
	//
	//	uri.Reset();
	//	uri.port = 9999;
	//	TS_ASSERT(URI::FromString("http://@gigi/stiintza", false, uri) == false);
	//	TS_ASSERT(uri.host == "");
	//	TS_ASSERT(uri.port == 0);
	//	TS_ASSERT(uri.userName == "");
	//	TS_ASSERT(uri.password == "");
	//	TS_ASSERT(uri.fullDocumentPath == "");
	//
	//	uri.Reset();
	//	uri.port = 9999;
	//	TS_ASSERT(URI::FromString("http://:@gigi/stiintza", false, uri) == false);
	//	TS_ASSERT(uri.host == "");
	//	TS_ASSERT(uri.port == 0);
	//	TS_ASSERT(uri.userName == "");
	//	TS_ASSERT(uri.password == "");
	//	TS_ASSERT(uri.fullDocumentPath == "");
	//
	//	uri.Reset();
	//	uri.port = 9999;
	//	TS_ASSERT(URI::FromString("http://gigi:/stiintza", false, uri) == false);
	//	TS_ASSERT(uri.host == "");
	//	TS_ASSERT(uri.port == 0);
	//	TS_ASSERT(uri.userName == "");
	//	TS_ASSERT(uri.password == "");
	//	TS_ASSERT(uri.fullDocumentPath == "");
	//
	//	uri.Reset();
	//	uri.port = 9999;
	//	TS_ASSERT(URI::FromString("http://gigi:1234/stiintza", false, uri) == true);
	//	TS_ASSERT(uri.host == "gigi");
	//	TS_ASSERT(uri.port == 1234);
	//	TS_ASSERT(uri.userName == "");
	//	TS_ASSERT(uri.password == "");
	//	TS_ASSERT(uri.fullDocumentPath == "/stiintza");
}

void CommonTestsSuite::test_setFdOptions() {
	InitNetworking();
	int32_t fd = socket(AF_INET, SOCK_STREAM, 0);
	TS_ASSERT(fd > 0);
	TS_ASSERT(setFdNoSIGPIPE(fd));
	TS_ASSERT(setFdNonBlock(fd));
	TS_ASSERT(setFdNoNagle(fd));
	TS_ASSERT(setFdKeepAlive(fd));
	TS_ASSERT(setFdReuseAddress(fd));
	CLOSE_SOCKET(fd);
	fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	TS_ASSERT(fd > 0);
	TS_ASSERT(setFdOptions(fd));
	CLOSE_SOCKET(fd);
}
