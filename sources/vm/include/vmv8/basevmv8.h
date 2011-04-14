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
#ifndef _BASEVMV8_H
#define	_BASEVMV8_H

#include "basevm.h"
#include "v8.h"
using namespace v8;

class BaseVMV8
: public BaseVM {
private:
	Persistent<Context> _context;
public:
	BaseVMV8();
	virtual ~BaseVMV8();

	virtual bool Supports64bit();
	virtual bool SupportsUndefined();
	virtual bool Initialize();
	virtual bool Shutdown();
	virtual bool LoadScriptFile(string scriptFileName, string scriptName);
	virtual bool LoadScriptString(string scriptContent, string scriptName);
	virtual bool HasFunction(string functionName);
	virtual bool CallWithParams(string functionName, Variant &parameters, Variant &results);
	virtual bool CallWithoutParams(string functionName, Variant &results);
protected:
	static string GetErrorString(TryCatch &tryCatch);
private:
	static bool VariantToV8Value(Variant &variant, Handle<Value> &val);
	static bool V8ValueToVariant(Local<Value> &value, Variant &variant);
};

#endif	/* _BASEVMV8_H */
#endif	/* HAS_V8 */

