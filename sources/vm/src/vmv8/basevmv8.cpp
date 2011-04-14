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

#ifdef HAS_V8

#include "vmv8/basevmv8.h"

BaseVMV8::BaseVMV8() {

}

BaseVMV8::~BaseVMV8() {
	Shutdown();
}

bool BaseVMV8::Supports64bit() {
	return false;
}

bool BaseVMV8::SupportsUndefined() {
	return true;
}

bool BaseVMV8::Initialize() {
	//1. Create a handle scope to hold the temporary references.
	HandleScope handleScope;

	//2. Create the global object template
	Handle<ObjectTemplate> global = ObjectTemplate::New();

	//3. Create the context
	_context = Context::New(NULL, global);

	//4. Done
	return true;
}

bool BaseVMV8::Shutdown() {
	_context.Dispose();
	return true;
}

bool BaseVMV8::LoadScriptFile(string scriptFileName, string scriptName) {
	//1. Open the file and read all its contents
	File file;
	if (!file.Initialize(scriptFileName, FILE_OPEN_MODE_READ)) {
		FATAL("Unable to open file %s", STR(scriptFileName));
		return false;
	}
	string scriptContent;
	if (!file.ReadAll(scriptContent)) {
		FATAL("Unable to read script content");
		return false;
	}

	//2. Create a handle scope to hold the temporary references.
	HandleScope handleScope;

	//3. Enter the new context so all the following operations take place within it
	Context::Scope scope(_context);

	//4. We're just about to compile the script; set up an error handler to
	//catch any exceptions the script might throw.
	TryCatch tryCatch;

	//5. Compile the script and check for errors.
	Handle<String> source = String::New(STR(scriptContent));
	Handle<Script> script = Script::Compile(source);
	if (script.IsEmpty()) {
		FATAL("Unable to compile the script:\n%s", STR(GetErrorString(tryCatch)));
		return false;
	}

	//6. Run the script!
	Handle<Value> val = script->Run();
	if (val.IsEmpty()) {
		FATAL("Unable to run the script:\n%s", STR(GetErrorString(tryCatch)));
		return false;
	}

	return true;
}

bool BaseVMV8::LoadScriptString(string scriptContent, string scriptName) {
	NYIR;
}

bool BaseVMV8::HasFunction(string functionName) {
	//1. Create a handle scope to hold the temporary references.
	HandleScope handleScope;

	//2. Enter the context so all the following operations take place within it
	Context::Scope scope(_context);

	//3. Prepare the try/catch
	TryCatch tryCatch;

	//4. Get the function
	Handle<String> temp = String::New(STR(functionName));
	Handle<Value> function = _context->Global()->Get(temp);
	if (function.IsEmpty()) {
		FATAL("Unable to get function %s. Error was:\n%s",
				STR(functionName),
				STR(GetErrorString(tryCatch)));
		return false;
	}

	//5. Done
	return function->IsFunction();
}

bool BaseVMV8::CallWithParams(string functionName, Variant &parameters, Variant &results) {
	//1. Create a handle scope to hold the temporary references.
	HandleScope handleScope;

	//2. Enter the context so all the following operations take place within it
	Context::Scope scope(_context);

	//3. Prepare the try/catch
	TryCatch tryCatch;

	//4. Get the function
	Handle<String> temp = String::New(STR(functionName));
	Handle<Value> functionVal = _context->Global()->Get(temp);
	if (functionVal.IsEmpty()) {
		FATAL("Unable to get function %s. Error was:\n%s",
				STR(functionName),
				STR(GetErrorString(tryCatch)));
		return false;
	}

	Handle<Function> function = Handle<Function>::Cast(functionVal);
#define V8_MAX_ARGUMENTS 256
	Handle<Value> arguments[V8_MAX_ARGUMENTS];
	if (parameters.MapSize() > 0) {
		uint32_t index = 0;

		FOR_MAP(parameters, string, Variant, i) {
			if (index >= V8_MAX_ARGUMENTS) {
				FATAL("Too many arguments");
				return false;
			}
			if (!VariantToV8Value(MAP_VAL(i), arguments[index])) {
				FATAL("Unable to convert argument");
				return false;
			}
			index++;
		}
	}

	Local<Value> result = function->Call(_context->Global(),
			parameters.MapSize(), arguments);

	if (result.IsEmpty()) {
		FATAL("Unable to execute function %s. Error was:\n%s",
				STR(functionName),
				STR(GetErrorString(tryCatch)));
		return false;
	}

	results.IsArray(true);

	if (!V8ValueToVariant(result, results[(uint32_t) 0])) {
		FATAL("Unable to get result");
		return false;
	}

	results.Compact();
	//ASSERT("results:\n%s", STR(results.ToString()));

	return true;
}

bool BaseVMV8::CallWithoutParams(string functionName, Variant &results) {
	NYIR;
}

string BaseVMV8::GetErrorString(TryCatch &tryCatch) {
	HandleScope handleScope;
	return *(String::Utf8Value(tryCatch.StackTrace()));
}

bool BaseVMV8::VariantToV8Value(Variant &variant, Handle<Value> &val) {
	switch ((VariantType) variant) {
		case V_NULL:
		{
			val = Null();
			return true;
		}
		case V_UNDEFINED:
		{
			val = Undefined();
			return true;
		}
		case V_BOOL:
		{
			val = Boolean::New((bool)variant);
			return true;
		}
		case V_INT8:
		case V_INT16:
		case V_INT32:
		{
			val = Int32::New((int32_t) variant);
			return true;
		}
		case V_UINT8:
		case V_UINT16:
		case V_UINT32:
		{
			val = Uint32::NewFromUnsigned((uint32_t) variant);
			return true;
		}
		case V_INT64:
		case V_UINT64:
		{
			variant.Compact();
			if ((variant == V_UINT64) || (variant == V_INT64)) {
				ASSERT("V8 doesn't support 64 bit integers");
			}
			return VariantToV8Value(variant, val);
		}
		case V_DOUBLE:
		{
			val = Number::New((double) variant);
			return true;
		}
		case V_TIMESTAMP:
		case V_DATE:
		case V_TIME:
		{
			val = Date::New(1111);
			NYI;
			return true;
		}
		case V_STRING:
		{
			val = String::New(STR(variant), ((string) variant).length());
			return true;
		}
		case V_TYPED_MAP:
		{
			NYIR;
		}
		case V_MAP:
		{
			bool isArray = true;
			if (variant.IsArray()) {
				for (uint32_t i = 0; i < variant.MapSize(); i++) {
					isArray &= variant.HasKey(format(VAR_INDEX_VALUE"%u", i));
					if (!isArray)
						break;
				}
			} else {
				isArray = false;
			}
			if (isArray) {
				val = Array::New(variant.MapSize());
			} else {
				val = Object::New();
			}
			Local<Object> object = val->ToObject();

			FOR_MAP(variant, string, Variant, i) {
				Local<Value> v;
				if (!VariantToV8Value(MAP_VAL(i), v)) {
					FATAL("Unable to convert variant to V8 value");
					return false;
				}
				if (MAP_KEY(i).find(VAR_INDEX_VALUE) == 0) {
					string temp = MAP_KEY(i).substr(VAR_INDEX_VALUE_LEN,
							string::npos);
					char *error = NULL;
					double index = strtod(STR(temp), &error);
					if (error == STR(temp) + temp.size()) {
						object->Set(Number::New(index), v);
					} else {
						object->Set(String::New(STR(MAP_KEY(i)), MAP_KEY(i).length()), v);
					}
				} else {
					object->Set(String::New(STR(MAP_KEY(i)), MAP_KEY(i).length()), v);
				}
			}
			return true;
		}
		case V_BYTEARRAY:
		{
			NYIR;
		}
		default:
		{
			ASSERT("Invalid variant type: %d", (VariantType) variant);
		}
	}
}

bool BaseVMV8::V8ValueToVariant(Local<Value> &value, Variant &variant) {
	variant.Reset();
	if (value.IsEmpty()) {
		FATAL("Empty value");
		return false;
	}

	if (value->IsBoolean()) {
		variant = (bool) value->ToBoolean()->Value();
		return true;
	} else if (value->IsNumber()) {
		variant = (double) value->ToNumber()->Value();
		return true;
	} else if (value->IsString()) {
		String::Utf8Value temp(value);
		variant = string(*temp);
		return true;
	} else if (value->IsUndefined()) {
		variant.Reset(true);
		return true;
	} else if (value->IsNull()) {
		variant.Reset();
		return true;
	} else if ((value->IsObject())
			|| (value->IsArray())) {
		Local<Object> object = value->ToObject();
		Local<Array> keys = object->GetPropertyNames();
		for (uint32_t i = 0; i < keys->Length(); i++) {
			Local<Value> k = keys->Get(i);
			Local<Value> v = object->Get(k);
			if (k->IsNumber()) {
				if (!V8ValueToVariant(v, variant[k->ToUint32()->Value()])) {
					FATAL("Invalid value");
					return false;
				}
			} else if (k->IsString()) {
				String::Utf8Value utfkey(k);
				string stringKey = *utfkey;
				if (!V8ValueToVariant(v, variant[stringKey])) {
					FATAL("Invalid value");
					return false;
				}
			} else {
				FATAL("Invalid key type");
				return false;
			}
		}
		variant.IsArray(value->IsArray());
		return true;
	} else {
		FATAL("Invalid value type");
		return false;
	}
}
#endif /* HAS_V8 */
