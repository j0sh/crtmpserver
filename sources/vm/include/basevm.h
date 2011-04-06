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


#ifndef _BASEVM_H
#define	_BASEVM_H

#include "common.h"

class DLLEXP BaseVM {
private:
	Variant _emptyParams;
public:
	BaseVM();
	virtual ~BaseVM();

	virtual bool Supports64bit() = 0;
	virtual bool SupportsUndefined() = 0;
	virtual bool Initialize() = 0;
	virtual bool Shutdown() = 0;
	virtual bool LoadScriptFile(string scriptFileName, string scriptName) = 0;
	virtual bool LoadScriptString(string scriptContent, string scriptName) = 0;
	virtual bool HasFunction(string functionName) = 0;
	virtual bool CallWithParams(string functionName, Variant &parameters, Variant &results) = 0;
	virtual bool CallWithoutParams(string functionName, Variant &results) = 0;
};

#endif	/* _BASEVM_H */


