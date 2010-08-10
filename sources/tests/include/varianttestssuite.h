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



#ifndef __VARIANTTESTSSUITE_H
#define __VARIANTTESTSSUITE_H

#include "common.h"
#include "basetestssuite.h"

class VariantTestsSuite
: public BaseTestsSuite {
private:
	Variant _noParamsVar;
	Variant _boolVar1;
	Variant _boolVar2;
	Variant _int8Var;
	Variant _int16Var;
	Variant _int32Var;
	Variant _int64Var;
	Variant _uint8Var;
	Variant _uint16Var;
	Variant _uint32Var;
	Variant _uint64Var;
	Variant _doubleVar;
	Variant _dateVar;
	Variant _timeVar;
	Variant _timestampVar1;
	Variant _timestampVar2;
	Variant _pcharVar;
	Variant _stringVar;
	Variant _mapVar;
	Variant _namedMapVar;
	Variant _referenceVariant1;
	Variant _referenceVariant2;
	Variant _referenceVariant3;
	Variant _referenceVariant4;
	Variant _referenceVariant5;
	Variant _referenceVariant6;
	Variant _referenceVariant7;
	Variant _referenceVariant8;
	Variant _referenceVariant9;
	Variant _referenceVariant10;
	Variant _referenceVariant11;
	Variant _referenceVariant12;
	Variant _referenceVariant13;
	Variant _referenceVariant14;
	Variant _referenceVariant15;
	Variant _referenceVariant16;
	Variant _referenceVariant17;
	Variant _referenceVariant18;
	Variant _referenceVariant19;
	Variant _referenceVariant20;
public:
	VariantTestsSuite();
	virtual ~VariantTestsSuite();
	virtual void Run();
private:
	void test_Constructors();
	void test_Reset();
	void test_ToString();
	void test_OperatorAssign();
	void test_OperatorCast();
	void test_Compact();
};

#endif /* __VARIANTTESTSSUITE_H */

