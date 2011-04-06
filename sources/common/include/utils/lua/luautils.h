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

#ifdef HAS_LUA
#ifndef _LUAUTILS_H
#define	_LUAUTILS_H

extern "C" {
#include <lualib.h>
#include <lauxlib.h>
}
#include "platform/platform.h"
#include "utils/misc/variant.h"

DLLEXP lua_State *CreateLuaState(void *pOpaque);
DLLEXP void DestroyLuaState(lua_State *pLuaState);

DLLEXP bool PopVariant(lua_State *pLuaState, Variant &variant, int32_t idx = 1, bool pop = true);
DLLEXP bool PopStack(lua_State *pLuaState, Variant &variant);
DLLEXP bool PushVariant(lua_State *pLuaState, Variant &variant,
		bool substituteNullables = false);
DLLEXP bool EvalLuaExpression(lua_State *pLuaState, string expression);
DLLEXP bool LoadLuaScriptFromFile(string file, lua_State *pLuaState, bool pCall = true);
DLLEXP bool LoadLuaScriptFromString(string luaScript, lua_State *pLuaState, bool pCall = true);
DLLEXP bool ReadLuaFile(string fileName, string section, Variant &configuration);
DLLEXP bool ReadLuaString(string script, string section, Variant &configuration);

#endif	/* _LUAUTILS_H */
#endif /* HAS_LUA */

