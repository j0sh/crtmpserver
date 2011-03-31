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



#include "thelibtestssuite.h"
#ifdef HAS_PROTOCOL_RTMP
#include "protocols/rtmp/header.h"
#endif /* HAS_PROTOCOL_RTMP */

TheLibTestsSuite::TheLibTestsSuite()
: BaseTestsSuite() {

}

TheLibTestsSuite::~TheLibTestsSuite() {

}

void TheLibTestsSuite::Run() {
#ifdef HAS_PROTOCOL_RTMP
	test_Header();
#endif /* HAS_PROTOCOL_RTMP */
}

#ifdef HAS_PROTOCOL_RTMP

void TheLibTestsSuite::test_Header() {
	/*
	 * Formats
	 * 1. Full
	 * 2. same stream
	 * 3. same stream, type and length
	 * 4. same stream, type, length and timestamp
	 */

	/*
	 * Channel types
	 * 1. small
	 * 2. medium
	 * 3. big
	 */

	/*
	 * timestamp types
	 * 1. short
	 * 2. extended
	 */
	IOBuffer buffer;
	string wanted = "";
	string got = "";
	uint8_t full_small_short[] = {0x02, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b};
	uint8_t ss_small_short[] = {0x42, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
	uint8_t sstl_small_short[] = {0x82, 0x01, 0x02, 0x03};
	uint8_t sstlt_small_short[] = {0xc2};
	uint8_t full_medium_short[] = {0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b};
	uint8_t ss_medium_short[] = {0x40, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
	uint8_t sstl_medium_short[] = {0x80, 0x00, 0x01, 0x02, 0x03};
	uint8_t sstlt_medium_short[] = {0xc0, 0x00};
	uint8_t full_big_short[] = {0x01, 0x01, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b};
	uint8_t ss_big_short[] = {0x41, 0x01, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
	uint8_t sstl_big_short[] = {0x81, 0x01, 0x00, 0x01, 0x02, 0x03};
	uint8_t sstlt_big_short[] = {0xc1, 0x01, 0x00};


	uint8_t full_small_extended[] = {0x02, 0xff, 0xff, 0xff, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x51, 0x52, 0x53, 0x54};
	uint8_t ss_small_extended[] = {0x42, 0xff, 0xff, 0xff, 0x04, 0x05, 0x06, 0x07, 0x51, 0x52, 0x53, 0x54};
	uint8_t sstl_small_extended[] = {0x82, 0xff, 0xff, 0xff, 0x51, 0x52, 0x53, 0x54};
	uint8_t full_medium_extended[] = {0x00, 0x00, 0xff, 0xff, 0xff, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x51, 0x52, 0x53, 0x54};
	uint8_t ss_medium_extended[] = {0x40, 0x00, 0xff, 0xff, 0xff, 0x04, 0x05, 0x06, 0x07, 0x51, 0x52, 0x53, 0x54};
	uint8_t sstl_medium_extended[] = {0x80, 0x00, 0xff, 0xff, 0xff, 0x51, 0x52, 0x53, 0x54};
	uint8_t full_big_extended[] = {0x01, 0x01, 0x00, 0xff, 0xff, 0xff, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x51, 0x52, 0x53, 0x54};
	uint8_t ss_big_extended[] = {0x41, 0x01, 0x00, 0xff, 0xff, 0xff, 0x04, 0x05, 0x06, 0x07, 0x51, 0x52, 0x53, 0x54};
	uint8_t sstl_big_extended[] = {0x81, 0x01, 0x00, 0xff, 0xff, 0xff, 0x51, 0x52, 0x53, 0x54};

	Header header;
	memset(&header, 0, sizeof (header));

#define HEADER_READ_TEST(h,ciSize, ci, buff, type, ia, ts) \
do { \
    buffer.IgnoreAll(); \
    buffer.ReadFromBuffer((buff), sizeof ((buff))); \
    buffer.Ignore(ciSize); \
    TS_ASSERT(h.Read(ci, type, buffer, GETAVAILABLEBYTESCOUNT(buffer))); \
    TS_ASSERT(H_HT(h) == type); \
    TS_ASSERT(H_CI(h) == ci); \
    /*TS_PRINT("H_TS(h): %08x\n",H_TS(h)); */\
    TS_ASSERT(H_TS(h) == ts); \
    TS_ASSERT(H_ML(h) == 0x040506); \
    TS_ASSERT(H_MT(h) == 0x07); \
    TS_ASSERT(H_SI(h) == 0x0b0a0908); \
    TS_ASSERT(H_IA(h) == ia); \
	TS_ASSERT(GETAVAILABLEBYTESCOUNT(buffer)==0); \
} while (0)

#define HEADER_WRITE_TEST(h,ciSize, ci, buff, type, ia, ts) \
do { \
    HEADER_READ_TEST(h,ciSize, ci, buff, type, ia, ts); \
    Header mirrorHeader=h; \
    buffer.IgnoreAll(); \
    TS_ASSERT(h.Write(buffer)); \
    wanted = string((char *) (buff), sizeof ((buff))); \
    got = string((char *) GETIBPOINTER(buffer), GETAVAILABLEBYTESCOUNT(buffer)); \
    /*TS_PRINT("\nwanted: "); \
    for (uint32_t i = 0; i < wanted.size(); i++) { \
        TS_PRINT("%02x ", (uint8_t) wanted[i]); \
    } \
    TS_PRINT("\n   got: "); \
    for (uint32_t i = 0; i < got.size(); i++) { \
        TS_PRINT("%02x ", (uint8_t) got[i]); \
    } */\
    TS_ASSERT(wanted == got); \
    got = string((char *) &h, sizeof ((h))); \
    wanted = string((char *) &mirrorHeader, sizeof (mirrorHeader)); \
    /*TS_PRINT("\n   got: "); \
    for (uint32_t i = 0; i < got.size(); i++) { \
        TS_PRINT("%02x ", (uint8_t) got[i]); \
    } \
    TS_PRINT("\nwanted: "); \
    for (uint32_t i = 0; i < wanted.size(); i++) { \
        TS_PRINT("%02x ", (uint8_t) wanted[i]); \
    } \
    TS_PRINT("\n"); */\
    TS_ASSERT(wanted == got); \
} while (0)

	HEADER_READ_TEST(header, 1, 2, full_small_short, HT_FULL, true, 0x010203);
	HEADER_READ_TEST(header, 1, 2, ss_small_short, HT_SAME_STREAM, false, 0x010203);
	HEADER_READ_TEST(header, 1, 2, sstl_small_short, HT_SAME_LENGTH_AND_STREAM, false, 0x010203);
	HEADER_READ_TEST(header, 1, 2, sstlt_small_short, HT_CONTINUATION, false, 0x010203);

	HEADER_READ_TEST(header, 2, 64, full_medium_short, HT_FULL, true, 0x010203);
	HEADER_READ_TEST(header, 2, 64, ss_medium_short, HT_SAME_STREAM, false, 0x010203);
	HEADER_READ_TEST(header, 2, 64, sstl_medium_short, HT_SAME_LENGTH_AND_STREAM, false, 0x010203);
	HEADER_READ_TEST(header, 2, 64, sstlt_medium_short, HT_CONTINUATION, false, 0x010203);

	HEADER_READ_TEST(header, 3, 320, full_big_short, HT_FULL, true, 0x010203);
	HEADER_READ_TEST(header, 3, 320, ss_big_short, HT_SAME_STREAM, false, 0x010203);
	HEADER_READ_TEST(header, 3, 320, sstl_big_short, HT_SAME_LENGTH_AND_STREAM, false, 0x010203);
	HEADER_READ_TEST(header, 3, 320, sstlt_big_short, HT_CONTINUATION, false, 0x010203);

	HEADER_READ_TEST(header, 1, 2, full_small_extended, HT_FULL, true, 0x51525354);
	HEADER_READ_TEST(header, 1, 2, ss_small_extended, HT_SAME_STREAM, false, 0x51525354);
	HEADER_READ_TEST(header, 1, 2, sstl_small_extended, HT_SAME_LENGTH_AND_STREAM, false, 0x51525354);

	HEADER_READ_TEST(header, 2, 64, full_medium_extended, HT_FULL, true, 0x51525354);
	HEADER_READ_TEST(header, 2, 64, ss_medium_extended, HT_SAME_STREAM, false, 0x51525354);
	HEADER_READ_TEST(header, 2, 64, sstl_medium_extended, HT_SAME_LENGTH_AND_STREAM, false, 0x51525354);

	HEADER_READ_TEST(header, 3, 320, full_big_extended, HT_FULL, true, 0x51525354);
	HEADER_READ_TEST(header, 3, 320, ss_big_extended, HT_SAME_STREAM, false, 0x51525354);
	HEADER_READ_TEST(header, 3, 320, sstl_big_extended, HT_SAME_LENGTH_AND_STREAM, false, 0x51525354);



	HEADER_WRITE_TEST(header, 1, 2, full_small_short, HT_FULL, true, 0x010203);
	HEADER_WRITE_TEST(header, 1, 2, ss_small_short, HT_SAME_STREAM, false, 0x010203);
	HEADER_WRITE_TEST(header, 1, 2, sstl_small_short, HT_SAME_LENGTH_AND_STREAM, false, 0x010203);
	HEADER_WRITE_TEST(header, 1, 2, sstlt_small_short, HT_CONTINUATION, false, 0x010203);

	HEADER_WRITE_TEST(header, 2, 64, full_medium_short, HT_FULL, true, 0x010203);
	HEADER_WRITE_TEST(header, 2, 64, ss_medium_short, HT_SAME_STREAM, false, 0x010203);
	HEADER_WRITE_TEST(header, 2, 64, sstl_medium_short, HT_SAME_LENGTH_AND_STREAM, false, 0x010203);
	HEADER_WRITE_TEST(header, 2, 64, sstlt_medium_short, HT_CONTINUATION, false, 0x010203);

	HEADER_WRITE_TEST(header, 3, 320, full_big_short, HT_FULL, true, 0x010203);
	HEADER_WRITE_TEST(header, 3, 320, ss_big_short, HT_SAME_STREAM, false, 0x010203);
	HEADER_WRITE_TEST(header, 3, 320, sstl_big_short, HT_SAME_LENGTH_AND_STREAM, false, 0x010203);
	HEADER_WRITE_TEST(header, 3, 320, sstlt_big_short, HT_CONTINUATION, false, 0x010203);

	HEADER_WRITE_TEST(header, 1, 2, full_small_extended, HT_FULL, true, 0x51525354);
	HEADER_WRITE_TEST(header, 1, 2, ss_small_extended, HT_SAME_STREAM, false, 0x51525354);
	HEADER_WRITE_TEST(header, 1, 2, sstl_small_extended, HT_SAME_LENGTH_AND_STREAM, false, 0x51525354);

	HEADER_WRITE_TEST(header, 2, 64, full_medium_extended, HT_FULL, true, 0x51525354);
	HEADER_WRITE_TEST(header, 2, 64, ss_medium_extended, HT_SAME_STREAM, false, 0x51525354);
	HEADER_WRITE_TEST(header, 2, 64, sstl_medium_extended, HT_SAME_LENGTH_AND_STREAM, false, 0x51525354);

	HEADER_WRITE_TEST(header, 3, 320, full_big_extended, HT_FULL, true, 0x51525354);
	HEADER_WRITE_TEST(header, 3, 320, ss_big_extended, HT_SAME_STREAM, false, 0x51525354);
	HEADER_WRITE_TEST(header, 3, 320, sstl_big_extended, HT_SAME_LENGTH_AND_STREAM, false, 0x51525354);
}
#endif /* HAS_PROTOCOL_RTMP */
