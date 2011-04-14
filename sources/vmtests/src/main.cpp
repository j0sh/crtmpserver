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


#include "common.h"
#include "basevm.h"
#include "testvmlua.h"
#include "testvmv8.h"

#define VM_TEST_DESCRIPTION "description"
#define VM_TEST_TYPE "type"
#define VM_TEST_TYPE_ECHO_FUNCTION_CALL "echoFunctionCall"
#define VM_TEST_TYPE_TEST_FUNCTION_EXISTENCE "hasFunction"
#define VM_TEST_FUNCTION_NAME "functionName"
#define VM_TEST_FUNCTION_PARAMETERS "functionParameters"
#define VM_TEST_WANTED_RESULT "wantedResult"

Variant PrepareTestsSuite();
void PreProcessData(Variant &variant, bool has64bit, bool hasUndefined);
bool RunTestEchoFunctionCall(BaseVM *pVM, Variant &test);
bool RunTestFunctionExistence(BaseVM *pVM, Variant &test);
bool RunTest(BaseVM *pVM, Variant &test);
bool TestVM(BaseVM *pVM, vector<string> &scriptFiles, Variant &testSuite);
#ifdef HAS_LUA
bool TestLuaVM(Variant &testSuite);
#endif /* HAS_LUA */
#ifdef HAS_V8
bool TestV8VM(Variant &testSuite);
#endif /* HAS_V8 */

int main(int argc, char **argv) {
	//1. Init the logger
	Logger::Init();
	ConsoleLogLocation consoleLogLocation(true);
	consoleLogLocation.SetLevel(_FINEST_);
	Logger::AddLogLocation(&consoleLogLocation);

	FINEST("Prepare test suite");
	Variant testSuite = PrepareTestsSuite();

	bool result = true;

#ifdef HAS_LUA
	FINEST("Run lua");
	if (!TestLuaVM(testSuite)) {
		FATAL("Lua VM tests failed");
		result &= false;
	}
#endif /* HAS_LUA */

#ifdef HAS_V8
	FINEST("Run v8");
	if (!TestV8VM(testSuite)) {
		FATAL("V8 VM tests failed");
		result &= false;
	}
#endif /* HAS_V8 */

	if (result) {
		FINEST("Tests completed successfully");
	} else {
		FINEST("Tests completed with errors");
	}

	Logger::Free(false);

	return result ? 0 : -1;
}

Variant PrepareTestsSuite() {
	Variant result;

	Variant functionExistence;
	functionExistence[VM_TEST_TYPE] = VM_TEST_TYPE_TEST_FUNCTION_EXISTENCE;
	functionExistence[VM_TEST_DESCRIPTION] = "Testing function existence";
	functionExistence[VM_TEST_FUNCTION_NAME] = VM_TEST_TYPE_ECHO_FUNCTION_CALL;
	functionExistence[VM_TEST_WANTED_RESULT] = (bool)true;
	result.PushToArray(functionExistence);

	functionExistence[VM_TEST_FUNCTION_NAME] = "___missingFunction____";
	functionExistence[VM_TEST_WANTED_RESULT] = (bool)false;
	result.PushToArray(functionExistence);

	//1. Call function
	Variant functionCall;
	functionCall[VM_TEST_DESCRIPTION] = "Testing function call";
	functionCall[VM_TEST_TYPE] = VM_TEST_TYPE_ECHO_FUNCTION_CALL;
	functionCall[VM_TEST_FUNCTION_NAME] = VM_TEST_TYPE_ECHO_FUNCTION_CALL;
	Variant complexParameter;

	complexParameter["00_bool_true"] = (bool)true;
	complexParameter["01_bool_false"] = (bool)false;

	complexParameter["02_uint8_t_max"] = (uint8_t) UINT8_MAX;
	complexParameter["03_int8_t_max"] = (int8_t) INT8_MAX;
	complexParameter["04_int8_t_min"] = (int8_t) INT8_MIN;

	complexParameter["05_uint16_t_max"] = (uint16_t) UINT16_MAX;
	complexParameter["06_int16_t_max"] = (int16_t) INT16_MAX;
	complexParameter["07_int16_t_min"] = (int16_t) INT16_MIN;

	complexParameter["08_uint32_t_max"] = (uint32_t) UINT32_MAX;
	complexParameter["09_int32_t_max"] = (int32_t) INT32_MAX;
	complexParameter["10_int32_t_min"] = (int32_t) INT32_MIN;

	complexParameter["11_uint64_t_max"] = (uint64_t) UINT64_MAX;
	complexParameter["12_int64_t_max"] = (int64_t) INT64_MAX;
	complexParameter["13_int64_t_min"] = (int64_t) INT64_MIN;

	complexParameter["14_double"] = 123.5;

	complexParameter["15_string"] = "some string...";

	complexParameter["16_undefined"] = Variant();
	complexParameter["16_undefined"].Reset(true);

	complexParameter["17_null"] = Variant();

	//	complexParameter["18_date"] = Variant(1979, 10, 31);
	//
	//	complexParameter["19_time"] = Variant(13, 14, 15, 0);
	//
	//	complexParameter["20_timestamp"] = Variant(1979, 10, 31, 13, 14, 15, 0);

	complexParameter["21_arraySample"].PushToArray("one");
	complexParameter["21_arraySample"].PushToArray((uint32_t) 1);
	complexParameter["21_arraySample"].PushToArray((uint32_t) 127);
	complexParameter["21_arraySample"].PushToArray((uint32_t) 128);
	complexParameter["21_arraySample"].PushToArray((uint32_t) 32767);
	complexParameter["21_arraySample"].PushToArray((uint32_t) 32768);
	complexParameter["21_arraySample"].PushToArray((uint32_t) 0x7fffffffUL);
	complexParameter["21_arraySample"].PushToArray((uint32_t) 0x80000000UL);
	complexParameter["21_arraySample"].PushToArray(3.2);

	complexParameter["22_sampleInnerMap"] = Variant(complexParameter);

	functionCall[VM_TEST_FUNCTION_PARAMETERS].PushToArray(complexParameter);
	result.PushToArray(functionCall);

	result.Compact();
	return result;
}

void PreProcessData(Variant &variant, bool has64bit, bool hasUndefined) {
	if (variant != V_MAP && variant != V_TYPED_MAP)
		return;
	vector<string> keys;

	FOR_MAP(variant, string, Variant, i) {
		if (MAP_VAL(i) == V_MAP || MAP_VAL(i) == V_TYPED_MAP) {
			PreProcessData(MAP_VAL(i), has64bit, hasUndefined);
			continue;
		}

		if ((!has64bit) && (MAP_VAL(i) == V_UINT64 || MAP_VAL(i) == V_INT64)) {
			ADD_VECTOR_END(keys, MAP_KEY(i));
			continue;
		}

		if ((!hasUndefined) && (MAP_VAL(i) == V_UNDEFINED)) {
			ADD_VECTOR_END(keys, MAP_KEY(i));
			continue;
		}
	}

	FOR_VECTOR(keys, i) {
		variant.RemoveKey(keys[i]);
	}
}

bool RunTestEchoFunctionCall(BaseVM *pVM, Variant &test) {
	Variant result;

	//1. Pre-process the input data
	Variant input = test;
	PreProcessData(input, pVM->Supports64bit(),
			pVM->SupportsUndefined());


	//2. Do the call
	if (!pVM->CallWithParams(VM_TEST_TYPE_ECHO_FUNCTION_CALL, input[VM_TEST_FUNCTION_PARAMETERS], result)) {
		FATAL("Calling %s failed", STR(test[VM_TEST_FUNCTION_NAME]));
		return false;
	}

	//3. Prepare the wanted/got data
	Variant wantedVariant;
	wantedVariant.PushToArray(input[VM_TEST_FUNCTION_PARAMETERS][(uint32_t) 0]);
	wantedVariant[(uint32_t) 0]["____THIS_IS_ADDED____"] = "____THIS_IS_ADDED____";
	string wanted = "";
	if (!wantedVariant.SerializeToBin(wanted)) {
		FATAL("Unable to serialize variant");
		return false;
	}
	string got = "";
	if (!result.SerializeToBin(got)) {
		FATAL("Unable to serialize variant");
		return false;
	}

	//4. Compare the results
	if (wanted != got) {
		IOBuffer wantedBuffer;
		IOBuffer gotBuffer;
		wantedBuffer.ReadFromString(wanted);
		gotBuffer.ReadFromString(got);
		for (uint32_t i = 0; i < GETAVAILABLEBYTESCOUNT(wantedBuffer) && i < GETAVAILABLEBYTESCOUNT(gotBuffer); i++) {
			if (GETIBPOINTER(wantedBuffer)[i] != GETIBPOINTER(gotBuffer)[i]) {
				FATAL("w: %hhx; g: %hhx; i: %u",
						GETIBPOINTER(wantedBuffer)[i],
						GETIBPOINTER(gotBuffer)[i],
						i);
			}
		}
		FINEST("w:\n%s\ng:\n%s", STR(wantedBuffer), STR(gotBuffer));
		FATAL("wanted:\n%s", STR(wantedVariant.ToString()));
		FATAL("got:\n%s", STR(result.ToString()));

		return false;
	}

	//4. We are done
	return true;
}

bool RunTestFunctionExistence(BaseVM *pVM, Variant &test) {
	string functionName = test[VM_TEST_FUNCTION_NAME];
	bool hasFunction = pVM->HasFunction(functionName);
	return hasFunction == ((bool)test[VM_TEST_WANTED_RESULT]);
}

bool RunTest(BaseVM *pVM, Variant &test) {
	FINEST("%s", STR(test[VM_TEST_DESCRIPTION]));
	if (test[VM_TEST_TYPE] == VM_TEST_TYPE_ECHO_FUNCTION_CALL) {
		return RunTestEchoFunctionCall(pVM, test);
	} else if (test[VM_TEST_TYPE] == VM_TEST_TYPE_TEST_FUNCTION_EXISTENCE) {
		return RunTestFunctionExistence(pVM, test);
	} else {
		FATAL("Invalid test:\n%s", STR(test.ToString()));
		return false;
	}
}

bool TestVM(BaseVM *pVM, vector<string> &scriptFiles, Variant &testSuite) {
	//1. Initialize
	if (!pVM->Initialize()) {
		FATAL("Unable to initialize VM");

		return false;
	}

	//2. Load scripts files

	FOR_VECTOR(scriptFiles, i) {
		if (!pVM->LoadScriptFile(scriptFiles[i], scriptFiles[i])) {
			FATAL("Unable to load script file: %s", STR(scriptFiles[i]));
			return false;
		}
	}

	//3. run the test suite
	for (uint32_t i = 0; i < testSuite.MapSize(); i++) {
		if (!RunTest(pVM, testSuite[(uint32_t) i])) {
			FATAL("VM failed on this test:\n%s",
					STR(testSuite[(uint32_t) i].ToString()));
			return false;
		}
	}

	//4. clear the vm
	if (!pVM->Shutdown()) {
		FATAL("Unable to shutdown the VM");
		return false;
	}

	//5. We are done
	return true;
}
#ifdef HAS_LUA

bool TestLuaVM(Variant &testSuite) {
	//1. Create the VM
	TestVMLua VM;

	//2. Prepare the scripts locations
	vector<string> scriptFiles;
	ADD_VECTOR_END(scriptFiles, "./vmtests.lua");
	ADD_VECTOR_END(scriptFiles, "./utils.lua");

	//3. Test the machine
	if (!TestVM(&VM, scriptFiles, testSuite)) {
		FATAL("TestLuaVM failed");
		return false;
	}

	//5. We are done here
	return true;
}
#endif /* HAS_LUA */

#ifdef HAS_V8

bool TestV8VM(Variant &testSuite) {
	//1. Create the VM
	TestVMV8 VM;

	//2. Prepare the scripts locations
	vector<string> scriptFiles;
	ADD_VECTOR_END(scriptFiles, "./vmtests.js");

	//3. Test the machine
	if (!TestVM(&VM, scriptFiles, testSuite)) {
		FATAL("TestV8VM failed");
		return false;
	}

	//5. We are done here
	return true;
}
#endif /* HAS_V8 */
