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


#ifndef __COMMONTESTSSUITE_H
#define __COMMONTESTSSUITE_H

#include "common.h"
#include "basetestssuite.h"

class CommonTestsSuite
: public BaseTestsSuite {
public:
	CommonTestsSuite();
	virtual ~CommonTestsSuite();

	virtual void Run();
private:
	//genericfunctionality.h
	void test_Endianess();
	void test_isNumeric();
	void test_lowerCase();
	void test_upperCase();
	void test_ltrim();
	void test_rtrim();
	void test_trim();
	void test_replace();
	void test_split();
	void test_mapping();
	void test_format();
	void test_splitFileName();
	void test_generateRandomString();
	void test_GetHostByName();
	void test_md5();
	void test_HMACsha256();
	void test_b64();
	void test_unb64();
	void test_unhex();
	void test_ParseURL();
	void test_setFdOptions();
};

#endif /* __COMMONTESTSSUITE_H */


