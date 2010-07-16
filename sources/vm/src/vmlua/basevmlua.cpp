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


#ifdef VM_LUA

#include "vmlua/basevmlua.h"

BaseVMLua::BaseVMLua()
: BaseVM() {
    _pGlobalState = NULL;
}

BaseVMLua::~BaseVMLua() {
    Shutdown();
}

bool BaseVMLua::Supports64bit() {
    return false;
}

bool BaseVMLua::SupportsUndefined() {
    return false;
}

bool BaseVMLua::Initialize() {
    _pGlobalState = CreateLuaState();
    if (_pGlobalState == NULL) {
        FATAL("Unable to initialize lua virtual machine");
        return false;
    }
    return true;
}

bool BaseVMLua::Shutdown() {
    if (_pGlobalState != NULL) {
        DestroyLuaState(_pGlobalState);
        _pGlobalState = NULL;
    }
    return true;
}

bool BaseVMLua::LoadScriptFile(string scriptFileName, string scriptName) {
    if (!LoadLuaScriptFromFile(scriptFileName, _pGlobalState, true)) {
        FATAL("Unable to load script: %s", STR(scriptFileName));
        return false;
    }
    return true;
}

bool BaseVMLua::LoadScriptString(string scriptContent, string scriptName) {
    NYIR;
}

bool BaseVMLua::Call(string functionName, Variant &parameters, Variant &results) {
    //1. Find out if the function is available or not
    lua_getglobal(_pGlobalState, STR(functionName));
    if (!lua_isfunction(_pGlobalState, -1)) {
        lua_pop(_pGlobalState, 1);
        FATAL("Function not available: %s", STR(functionName));
        return false;
    }

    //2. Push the parameters
    if (!PushVariant(_pGlobalState, parameters, true)) {
        FATAL("Unable to push parameters");
        return false;
    }

    if (lua_resume(_pGlobalState, 1) != 0) {
        Variant error;
        PopVariant(_pGlobalState, error);
        FATAL("Unable to call function %s:\n%s", STR(functionName),
                STR(error.ToString()));
        return false;
    }

    results.Reset();
    results.IsArray(true);

    int32_t returnCount = lua_gettop(_pGlobalState);
    for (int32_t i = 0; i < returnCount; i++) {
        if (!PopVariant(_pGlobalState, results[(uint32_t) returnCount - i - 1])) {
            FATAL("Unable to pop variant");
            return false;
        }
    }
    returnCount = lua_gettop(_pGlobalState);

    return true;
}

#endif /* VM_LUA */


