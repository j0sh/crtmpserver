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
#include "varianttestssuite.h"
#include "thelibtestssuite.h"

int main(int argc, char **argv) {
	TS_PRINT("Begin tests...\n");
	EXECUTE_SUITE(CommonTestsSuite);
	EXECUTE_SUITE(VariantTestsSuite);
	EXECUTE_SUITE(TheLibTestsSuite);
	TS_PRINT("A total of %u tests completed successfuly\n", BaseTestsSuite::_testsCount);
	return 0;
}

