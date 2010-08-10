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


#include "new/common.h"
#include "testvmlua.h"

#define VM_TEST_DESCRIPTION "description"
#define VM_TEST_TYPE "type"
#define VM_TEST_TYPE_ECHO_FUNCTION_CALL "echoFunctionCall"
#define VM_TEST_FUNCTION_NAME "functionName"
#define VM_TEST_FUNCTION_PARAMETERS "functionParameters"

Variant PrepareTestsSuite();
void PreProcessData(Variant &variant, bool has64bit, bool hasUndefined);
bool RunTestEchoFunctionCall(BaseVM *pVM, Variant &test);
bool RunTest(BaseVM *pVM, Variant &test);
bool TestVM(BaseVM *pVM, vector<string> &scriptFiles, Variant &testSuite);
bool TestLuaVM(Variant &testSuite);

int main(int argc, char **argv) {
	//1. Init the logger
	Logger::Init();
	ConsoleLogLocation consoleLogLocation(true);
	consoleLogLocation.SetLevel(_FINEST_);
	Logger::AddLogLocation(&consoleLogLocation);

	FINEST("Prepare test suite");
	Variant testSuite = PrepareTestsSuite();

	bool result = true;

	FINEST("Run lua");
	if (!TestLuaVM(testSuite)) {
		FATAL("Lua VM tests failed");
		result &= false;
	}

	Logger::Free(false);

	return result ? 0 : -1;
}

Variant PrepareTestsSuite() {
	Variant result;

	//1. Call function
	Variant functionCall;
	functionCall[VM_TEST_DESCRIPTION] = "Testing function call";
	functionCall[VM_TEST_TYPE] = VM_TEST_TYPE_ECHO_FUNCTION_CALL;
	Variant params;

	params["00_bool_true"] = (bool)true;
	params["01_bool_false"] = (bool)false;

	params["02_uint8_t_max"] = (uint8_t) UINT8_MAX;
	params["03_int8_t_max"] = (int8_t) INT8_MAX;
	params["04_int8_t_min"] = (int8_t) INT8_MIN;

	params["05_uint16_t_max"] = (uint16_t) UINT16_MAX;
	params["06_int16_t_max"] = (int16_t) INT16_MAX;
	params["07_int16_t_min"] = (int16_t) INT16_MIN;

	params["08_uint32_t_max"] = (uint32_t) UINT32_MAX;
	params["09_int32_t_max"] = (int32_t) INT32_MAX;
	params["10_int32_t_min"] = (int32_t) INT32_MIN;

	params["11_uint64_t_max"] = (uint64_t) UINT64_MAX;
	params["12_int64_t_max"] = (int64_t) INT64_MAX;
	params["13_int64_t_min"] = (int64_t) INT64_MIN;

	params["14_double"] = 123.5;

	params["15_string"] = "some string...";

	params["16_undefined"] = Variant();
	params["16_undefined"].Reset(true);

	params["17_null"] = Variant();

	params["18_date"] = Variant(1979, 10, 31);

	params["19_time"] = Variant(13, 14, 15, 0);

	params["20_timestamp"] = Variant(1979, 10, 31, 13, 14, 15, 0);

	params["21_arraySample"].PushToArray("one");
	params["21_arraySample"].PushToArray((uint32_t) 1);
	params["21_arraySample"].PushToArray((uint32_t) 127);
	params["21_arraySample"].PushToArray((uint32_t) 128);
	params["21_arraySample"].PushToArray((uint32_t) 32767);
	params["21_arraySample"].PushToArray((uint32_t) 32768);
	params["21_arraySample"].PushToArray((uint32_t) 0x7fffffffUL);
	params["21_arraySample"].PushToArray((uint32_t) 0x80000000UL);
	params["21_arraySample"].PushToArray(3.2);

	params["22_sampleInnerMap"] = Variant(params);

	functionCall[VM_TEST_FUNCTION_PARAMETERS] = params;
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
	if (!pVM->Call(VM_TEST_TYPE_ECHO_FUNCTION_CALL, input[VM_TEST_FUNCTION_PARAMETERS], result)) {
		FATAL("Calling %s failed", VM_TEST_TYPE_ECHO_FUNCTION_CALL);
		return false;
	}

	//3. Prepare the wanted/got data
	Variant wantedVariant;
	wantedVariant.PushToArray(input[VM_TEST_FUNCTION_PARAMETERS]);
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
				FATAL("w: %02x; g: %02x; i: %d",
						GETIBPOINTER(wantedBuffer)[i],
						GETIBPOINTER(gotBuffer)[i],
						i);
			}
		}
		FINEST("w:\n%s\ng:\n%s", STR(wantedBuffer), STR(gotBuffer));
		FATAL("wnated:\n%s", STR(wantedVariant.ToString()));
		FATAL("got:\n%s", STR(result.ToString()));

		return false;
	}

	//4. We are done
	return true;
}

bool RunTest(BaseVM *pVM, Variant &test) {
	FINEST("Running test %s", STR(test[VM_TEST_TYPE]));
	if (test[VM_TEST_TYPE] == VM_TEST_TYPE_ECHO_FUNCTION_CALL) {
		return RunTestEchoFunctionCall(pVM, test);
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

bool TestLuaVM(Variant &testSuite) {
	//1. Create the VM
	BaseVM *pVM = new TestVMLua();

	//2. Prepare the scripts locations
	vector<string> scriptFiles;
	ADD_VECTOR_END(scriptFiles, "./vmtests/vmtests.lua");
	ADD_VECTOR_END(scriptFiles, "./vmtests/utils.lua");

	//3. Test the machine
	if (!TestVM(pVM, scriptFiles, testSuite)) {
		FATAL("TestVM failed");
		return false;
	}

	//4. delete the vm
	delete pVM;

	//5. We are done here
	return true;
}

