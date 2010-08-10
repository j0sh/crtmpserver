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
	test_lowercase();
	test_uppercase();
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
	test_b64();
	test_unb64();
	test_ParseURL();
}

void CommonTestsSuite::test_Endianess() {
	uint16_t ui16 = 0x0102;
	uint32_t ui32 = 0x01020304;
	uint64_t ui64 = 0x0102030405060708LL;

	//host to network
	uint8_t *pBuffer = NULL;
	ui16 = htons(ui16);
	pBuffer = (uint8_t *) & ui16;
	TS_ASSERT(pBuffer[0] == 0x01);
	TS_ASSERT(pBuffer[1] == 0x02);

	pBuffer = NULL;
	ui32 = htonl(ui32);
	pBuffer = (uint8_t *) & ui32;
	TS_ASSERT(pBuffer[0] == 0x01);
	TS_ASSERT(pBuffer[1] == 0x02);
	TS_ASSERT(pBuffer[2] == 0x03);
	TS_ASSERT(pBuffer[3] == 0x04);

	pBuffer = NULL;
	ui32 = 0x01020304;
	ui32 = htona(ui32);
	pBuffer = (uint8_t *) & ui32;
	TS_ASSERT(pBuffer[0] == 0x02);
	TS_ASSERT(pBuffer[1] == 0x03);
	TS_ASSERT(pBuffer[2] == 0x04);
	TS_ASSERT(pBuffer[3] == 0x01);

	pBuffer = NULL;
	ui64 = htonll(ui64);
	pBuffer = (uint8_t *) & ui64;
	TS_ASSERT(pBuffer[0] == 0x01);
	TS_ASSERT(pBuffer[1] == 0x02);
	TS_ASSERT(pBuffer[2] == 0x03);
	TS_ASSERT(pBuffer[3] == 0x04);
	TS_ASSERT(pBuffer[4] == 0x05);
	TS_ASSERT(pBuffer[5] == 0x06);
	TS_ASSERT(pBuffer[6] == 0x07);
	TS_ASSERT(pBuffer[7] == 0x08);

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

	ui16 = ntohsp(buffer);
	TS_ASSERT(ui16 == 0x0001);
	ui16 = ntohsp(buffer + 1);
	TS_ASSERT(ui16 == 0x0102);
	ui16 = ntohsp(buffer + 2);
	TS_ASSERT(ui16 == 0x0203);
	ui16 = ntohsp(buffer + 3);
	TS_ASSERT(ui16 == 0x0304);
	ui16 = ntohsp(buffer + 4);
	TS_ASSERT(ui16 == 0x0405);
	ui16 = ntohsp(buffer + 5);
	TS_ASSERT(ui16 == 0x0506);
	ui16 = ntohsp(buffer + 6);
	TS_ASSERT(ui16 == 0x0607);
	ui16 = ntohsp(buffer + 7);
	TS_ASSERT(ui16 == 0x0708);
	ui16 = ntohsp(buffer + 8);
	TS_ASSERT(ui16 == 0x0809);
	ui16 = ntohsp(buffer + 9);
	TS_ASSERT(ui16 == 0x090a);
	ui16 = ntohsp(buffer + 10);
	TS_ASSERT(ui16 == 0x0a0b);
	ui16 = ntohsp(buffer + 11);
	TS_ASSERT(ui16 == 0x0b0c);
	ui16 = ntohsp(buffer + 12);
	TS_ASSERT(ui16 == 0x0c0d);
	ui16 = ntohsp(buffer + 13);
	TS_ASSERT(ui16 == 0x0d0e);
	ui16 = ntohsp(buffer + 14);
	TS_ASSERT(ui16 == 0x0e0f);

	ui32 = ntohlp(buffer);
	TS_ASSERT(ui32 == 0x00010203);
	ui32 = ntohlp(buffer + 1);
	TS_ASSERT(ui32 == 0x01020304);
	ui32 = ntohlp(buffer + 2);
	TS_ASSERT(ui32 == 0x02030405);
	ui32 = ntohlp(buffer + 3);
	TS_ASSERT(ui32 == 0x03040506);
	ui32 = ntohlp(buffer + 4);
	TS_ASSERT(ui32 == 0x04050607);
	ui32 = ntohlp(buffer + 5);
	TS_ASSERT(ui32 == 0x05060708);
	ui32 = ntohlp(buffer + 6);
	TS_ASSERT(ui32 == 0x06070809);
	ui32 = ntohlp(buffer + 7);
	TS_ASSERT(ui32 == 0x0708090a);
	ui32 = ntohlp(buffer + 8);
	TS_ASSERT(ui32 == 0x08090a0b);
	ui32 = ntohlp(buffer + 9);
	TS_ASSERT(ui32 == 0x090a0b0c);
	ui32 = ntohlp(buffer + 10);
	TS_ASSERT(ui32 == 0x0a0b0c0d);
	ui32 = ntohlp(buffer + 11);
	TS_ASSERT(ui32 == 0x0b0c0d0e);
	ui32 = ntohlp(buffer + 12);
	TS_ASSERT(ui32 == 0x0c0d0e0f);

	ui32 = ntohap(buffer);
	TS_ASSERT(ui32 == 0x03000102);
	ui32 = ntohap(buffer + 1);
	TS_ASSERT(ui32 == 0x04010203);
	ui32 = ntohap(buffer + 2);
	TS_ASSERT(ui32 == 0x05020304);
	ui32 = ntohap(buffer + 3);
	TS_ASSERT(ui32 == 0x06030405);
	ui32 = ntohap(buffer + 4);
	TS_ASSERT(ui32 == 0x07040506);
	ui32 = ntohap(buffer + 5);
	TS_ASSERT(ui32 == 0x08050607);
	ui32 = ntohap(buffer + 6);
	TS_ASSERT(ui32 == 0x09060708);
	ui32 = ntohap(buffer + 7);
	TS_ASSERT(ui32 == 0x0a070809);
	ui32 = ntohap(buffer + 8);
	TS_ASSERT(ui32 == 0x0b08090a);
	ui32 = ntohap(buffer + 9);
	TS_ASSERT(ui32 == 0x0c090a0b);
	ui32 = ntohap(buffer + 10);
	TS_ASSERT(ui32 == 0x0d0a0b0c);
	ui32 = ntohap(buffer + 11);
	TS_ASSERT(ui32 == 0x0e0b0c0d);
	ui32 = ntohap(buffer + 12);
	TS_ASSERT(ui32 == 0x0f0c0d0e);

	ui64 = ntohllp(buffer);
	TS_ASSERT(ui64 == 0x0001020304050607LL);
	ui64 = ntohllp(buffer + 1);
	TS_ASSERT(ui64 == 0x0102030405060708LL);
	ui64 = ntohllp(buffer + 2);
	TS_ASSERT(ui64 == 0x0203040506070809LL);
	ui64 = ntohllp(buffer + 3);
	TS_ASSERT(ui64 == 0x030405060708090aLL);
	ui64 = ntohllp(buffer + 4);
	TS_ASSERT(ui64 == 0x0405060708090a0bLL);
	ui64 = ntohllp(buffer + 5);
	TS_ASSERT(ui64 == 0x05060708090a0b0cLL);
	ui64 = ntohllp(buffer + 6);
	TS_ASSERT(ui64 == 0x060708090a0b0c0dLL);
	ui64 = ntohllp(buffer + 7);
	TS_ASSERT(ui64 == 0x0708090a0b0c0d0eLL);
	ui64 = ntohllp(buffer + 8);
	TS_ASSERT(ui64 == 0x08090a0b0c0d0e0fLL);

	//network to host
#ifdef LITTLE_ENDIAN_BYTE_ALIGNED
	TS_ASSERT(ntoha(0x01040302) == 0x01020304);
	TS_ASSERT(ntohll(0x0807060504030201LL) == 0x0102030405060708LL);
#endif /* LITTLE_ENDIAN_BYTE_ALIGNED */

#ifdef LITTLE_ENDIAN_SHORT_ALIGNED
	TS_ASSERT(ntoha(0x01040302) == 0x01020304);
	TS_ASSERT(ntohll(0x0807060504030201LL) == 0x0102030405060708LL);
#endif /* LITTLE_ENDIAN_SHORT_ALIGNED */

#ifdef BIG_ENDIAN_BYTE_ALIGNED
	TS_ASSERT(ntoha(0x02030401) == 0x01020304);
	TS_ASSERT(ntohll(0x0102030405060708LL) == 0x0102030405060708LL);
#endif /* BIG_ENDIAN_BYTE_ALIGNED */

#ifdef BIG_ENDIAN_SHORT_ALIGNED
#error BIG_ENDIAN_SHORT_ALIGNED set of tests not yet implemented!!! Please take care of this first!!!
#endif /* BIG_ENDIAN_SHORT_ALIGNED */

	//double mirror
	TS_ASSERT(ntohs(htons(0x0102)) == 0x0102);
	TS_ASSERT(htons(ntohs(0x0102)) == 0x0102);

	TS_ASSERT(ntohl(htonl(0x01020304)) == 0x01020304);
	TS_ASSERT(htonl(ntohl(0x01020304)) == 0x01020304);

	TS_ASSERT(ntoha(htona(0x01020304)) == 0x01020304);
	TS_ASSERT(htona(ntoha(0x01020304)) == 0x01020304);

	TS_ASSERT(ntohll(htonll(0x0102030405060708LL)) == 0x0102030405060708LL);
	TS_ASSERT(htonll(ntohll(0x0102030405060708LL)) == 0x0102030405060708LL);

	// Buffer Put routines
	for (int i = 0; i < 16; i++) {
		put_htons(buffer + i, 0x0102);
		TS_ASSERT(ntohsp(buffer + i) == 0x0102);

		put_htonl(buffer + i, 0x01020304);
		TS_ASSERT(ntohlp(buffer + i) == 0x01020304);

		put_htonll(buffer + i, 0x0102030405060708LL);
		TS_ASSERT(ntohllp(buffer + i) == 0x0102030405060708LL);
	}
}

void CommonTestsSuite::test_isNumeric() {
	//bool isNumeric(string value);
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
	//TS_ASSERT(isNumeric("1234"));
}

void CommonTestsSuite::test_lowercase() {
	TS_ASSERT(lowercase("TeXt") == "text");
	TS_ASSERT(lowercase("TEXT") == "text");
	TS_ASSERT(lowercase("text") == "text");
	TS_ASSERT(lowercase("") == "");
	TS_ASSERT(lowercase(" ") == " ");
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
	TS_ASSERT(lowercase(testString) == lowerString);
}

void CommonTestsSuite::test_uppercase() {
	TS_ASSERT(uppercase("TeXt") == "TEXT");
	TS_ASSERT(uppercase("TEXT") == "TEXT");
	TS_ASSERT(uppercase("text") == "TEXT");
	TS_ASSERT(uppercase("") == "");
	TS_ASSERT(uppercase(" ") == " ");
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
	TS_ASSERT(uppercase(testString) == upperString);
}

void CommonTestsSuite::test_ltrim() {
	string str;

	str = "";
	ltrim(str);
	TS_ASSERT(str == "");

	str = " ";
	ltrim(str);
	TS_ASSERT(str == "");

	str = " b";
	ltrim(str);
	TS_ASSERT(str == "b");

	str = " bb";
	ltrim(str);
	TS_ASSERT(str == "bb");

	str = " bb ";
	ltrim(str);
	TS_ASSERT(str == "bb ");

	str = " bb b";
	ltrim(str);
	TS_ASSERT(str == "bb b");

	str = " bb b ";
	ltrim(str);
	TS_ASSERT(str == "bb b ");
}

void CommonTestsSuite::test_rtrim() {
	string str;

	str = "";
	rtrim(str);
	TS_ASSERT(str == "");

	str = " ";
	rtrim(str);
	TS_ASSERT(str == "");

	str = " b";
	rtrim(str);
	TS_ASSERT(str == " b");

	str = " bb";
	rtrim(str);
	TS_ASSERT(str == " bb");

	str = " bb ";
	rtrim(str);
	TS_ASSERT(str == " bb");

	str = " bb b";
	rtrim(str);
	TS_ASSERT(str == " bb b");

	str = " bb b ";
	rtrim(str);
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

//void CommonTests::test_nameValue() {
//    string key, value;
//    string line;
//
//    line = "key=value";
//
//    nameValue(line, "=", key, value);
//    TS_ASSERT(key == "key");
//    TS_ASSERT(value == "value");
//
//    nameValue(line, "K", key, value);
//    TS_ASSERT(key == "");
//    TS_ASSERT(value == "");
//
//    line = "key=";
//    nameValue(line, "=", key, value);
//    TS_ASSERT(key == "key");
//    TS_ASSERT(value == "");
//
//    line = "=value";
//    nameValue(line, "=", key, value);
//    TS_ASSERT(key == "");
//    TS_ASSERT(value == "value");
//
//    line = "=";
//    nameValue(line, "=", key, value);
//    TS_ASSERT(key == "");
//    TS_ASSERT(value == "");
//}

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

void CommonTestsSuite::test_ParseURL() {
	string host;
	uint16_t port;
	string user;
	string passwd;
	string doc;

	host = user = passwd = doc = "dummy";
	port = 9999;
	TS_ASSERT(ParseURL("", host, port, user, passwd, doc) == false);
	TS_ASSERT(host == "");
	TS_ASSERT(port == 0);
	TS_ASSERT(user == "");
	TS_ASSERT(passwd == "");
	TS_ASSERT(doc == "");

	host = user = passwd = doc = "dummy";
	port = 9999;
	TS_ASSERT(ParseURL("sjdhfkjsdhfjksdh", host, port, user, passwd, doc) == false);
	TS_ASSERT(host == "");
	TS_ASSERT(port == 0);
	TS_ASSERT(user == "");
	TS_ASSERT(passwd == "");
	TS_ASSERT(doc == "");

	host = user = passwd = doc = "dummy";
	port = 9999;
	TS_ASSERT(ParseURL("http:/gigi/stiintza", host, port, user, passwd, doc) == false);
	TS_ASSERT(host == "");
	TS_ASSERT(port == 0);
	TS_ASSERT(user == "");
	TS_ASSERT(passwd == "");
	TS_ASSERT(doc == "");

	host = user = passwd = doc = "dummy";
	port = 9999;
	TS_ASSERT(ParseURL("htt://gigi/stiintza", host, port, user, passwd, doc) == false);
	TS_ASSERT(host == "");
	TS_ASSERT(port == 0);
	TS_ASSERT(user == "");
	TS_ASSERT(passwd == "");
	TS_ASSERT(doc == "");

	host = user = passwd = doc = "dummy";
	port = 9999;
	TS_ASSERT(ParseURL("http://:gigi/stiintza", host, port, user, passwd, doc) == false);
	TS_ASSERT(host == "");
	TS_ASSERT(port == 0);
	TS_ASSERT(user == "");
	TS_ASSERT(passwd == "");
	TS_ASSERT(doc == "");

	host = user = passwd = doc = "dummy";
	port = 9999;
	TS_ASSERT(ParseURL("http://gigi:gfgf/stiintza", host, port, user, passwd, doc) == false);
	TS_ASSERT(host == "");
	TS_ASSERT(port == 0);
	TS_ASSERT(user == "");
	TS_ASSERT(passwd == "");
	TS_ASSERT(doc == "");

	host = user = passwd = doc = "dummy";
	port = 9999;
	TS_ASSERT(ParseURL("http://gigi:123456/stiintza", host, port, user, passwd, doc) == false);
	TS_ASSERT(host == "");
	TS_ASSERT(port == 0);
	TS_ASSERT(user == "");
	TS_ASSERT(passwd == "");
	TS_ASSERT(doc == "");


	host = user = passwd = doc = "dummy";
	port = 9999;
	TS_ASSERT(ParseURL("http://:@gigi/stiintza", host, port, user, passwd, doc) == true);
	TS_ASSERT(host == "gigi");
	TS_ASSERT(port == 80);
	TS_ASSERT(user == "");
	TS_ASSERT(passwd == "");
	TS_ASSERT(doc == "/stiintza");

	host = user = passwd = doc = "dummy";
	port = 9999;
	TS_ASSERT(ParseURL("http://aaa:ddd@gigi/stiintza", host, port, user, passwd, doc) == true);
	TS_ASSERT(host == "gigi");
	TS_ASSERT(port == 80);
	TS_ASSERT(user == "aaa");
	TS_ASSERT(passwd == "ddd");
	TS_ASSERT(doc == "/stiintza");

	host = user = passwd = doc = "dummy";
	port = 9999;
	TS_ASSERT(ParseURL("http://gigi/stiintza", host, port, user, passwd, doc) == true);
	TS_ASSERT(host == "gigi");
	TS_ASSERT(port == 80);
	TS_ASSERT(user == "");
	TS_ASSERT(passwd == "");
	TS_ASSERT(doc == "/stiintza");

	host = user = passwd = doc = "dummy";
	port = 9999;
	TS_ASSERT(ParseURL("http://@gigi/stiintza", host, port, user, passwd, doc) == true);
	TS_ASSERT(host == "gigi");
	TS_ASSERT(port == 80);
	TS_ASSERT(user == "");
	TS_ASSERT(passwd == "");
	TS_ASSERT(doc == "/stiintza");

	host = user = passwd = doc = "dummy";
	port = 9999;
	TS_ASSERT(ParseURL("http://:@gigi/stiintza", host, port, user, passwd, doc) == true);
	TS_ASSERT(host == "gigi");
	TS_ASSERT(port == 80);
	TS_ASSERT(user == "");
	TS_ASSERT(passwd == "");
	TS_ASSERT(doc == "/stiintza");

	host = user = passwd = doc = "dummy";
	port = 9999;
	TS_ASSERT(ParseURL("http://gigi:/stiintza", host, port, user, passwd, doc) == true);
	TS_ASSERT(host == "gigi");
	TS_ASSERT(port == 80);
	TS_ASSERT(user == "");
	TS_ASSERT(passwd == "");
	TS_ASSERT(doc == "/stiintza");

	host = user = passwd = doc = "dummy";
	port = 9999;
	TS_ASSERT(ParseURL("http://gigi:1234/stiintza", host, port, user, passwd, doc) == true);
	TS_ASSERT(host == "gigi");
	TS_ASSERT(port == 1234);
	TS_ASSERT(user == "");
	TS_ASSERT(passwd == "");
	TS_ASSERT(doc == "/stiintza");
}

