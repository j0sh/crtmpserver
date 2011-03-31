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


#ifndef _BASETESTSSUITE_H
#define	_BASETESTSSUITE_H

#include <assert.h>


#define __TESTS_VALIDATE_FROMAT_SPECIFIERS(...) \
do { \
   char ___tempLocation[1024]; \
   snprintf(___tempLocation,1023,__VA_ARGS__); \
}while (0)

#define TS_PRINT(...) do{__TESTS_VALIDATE_FROMAT_SPECIFIERS(__VA_ARGS__);printf(__VA_ARGS__);}while(0)

#define TS_ASSERT_NO_INCREMENT(x) \
do { \
    if(!(x)) { \
        TS_PRINT("Test failed. %s:%u\n",__FILE__,__LINE__); \
        exit(-1); \
    } \
} while(0)

#define TS_ASSERT(x) \
do { \
    if(!(x)) { \
        TS_PRINT("Test %s failed. %s:%u\n",#x,__FILE__,__LINE__); \
        exit(-1); \
    } else { \
        _testsCount++; \
    } \
} while(0)

#define EXECUTE_SUITE(x) \
do { \
        TS_PRINT("Executing %s... ",#x); \
        int before = BaseTestsSuite::_testsCount; \
        x().Run(); \
        int testsCount = BaseTestsSuite::_testsCount - before; \
        TS_PRINT("%u tests completed successfuly\n", testsCount); \
} while(0)

class BaseTestsSuite {
public:
	static int _testsCount;
public:
	BaseTestsSuite();
	virtual ~BaseTestsSuite();

	virtual void Run() = 0;
};


#endif	/* _BASETESTSSUITE_H */

