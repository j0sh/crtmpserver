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

#include "vm/baseappvirtualmachine.h"
#include "vm/lua/luaappvirtualmachine.h"
#include "basevm.h"
using namespace app_vmapp;

BaseAppVirtualMachine::BaseAppVirtualMachine() {

}

BaseAppVirtualMachine *BaseAppVirtualMachine::GetVM(string type, string script,
		BaseClientApplication *pApplication) {
	BaseAppVirtualMachine *pResult = NULL;

	//1. Instantiate the VM
	if (false) {

	}
#ifdef HAS_LUA
	else if (type == "lua") {
		pResult = new LuaAppVirtualMachine();
		pResult->_pApplication = pApplication;
	}
#endif /* HAS_LUA */
	if (pResult == NULL) {
		FATAL("No machine with type %s available", STR(type));
		return NULL;
	}

	//2. Bind the API
	if (!pResult->BindAPI()) {
		FATAL("Unable to bind API");
		delete pResult;
		return NULL;
	}

	//3. Initialize the virtual machine
	if (!pResult->Initialize(script)) {
		FATAL("Unable to initialize virtual machine with script %s", STR(script));
		delete pResult;
		return NULL;
	}

	//3. Done :)
	return pResult;
}

BaseAppVirtualMachine::~BaseAppVirtualMachine() {

}
