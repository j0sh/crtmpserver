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
#include "vm/lua/luaapi_generics.h"
#include "vm/lua/luaapi_helpers.h"

namespace app_vmapp {

	int luaapi_generics_listFolder(lua_State *L) {
		LUA_INIT_PARAMS(params, L);

		LUA_READ_PARAM(params, string, V_STRING, path, "", 0, true);
		LUA_READ_PARAM(params, bool, V_BOOL, normalize, true, 1, false);
		LUA_READ_PARAM(params, bool, V_BOOL, includeFolders, false, 2, false);
		LUA_READ_PARAM(params, bool, V_BOOL, recursive, true, 3, false);

		vector<string> result;
		listFolder(path, result, normalize, includeFolders, recursive);

		for (uint32_t i = 0; i < result.size(); i++) {
			lua_pushstring(L, STR(result[i]));
		}

		return result.size();
	}

	int luaapi_generics_normalizePath(lua_State *L) {
		LUA_INIT_PARAMS(params, L);

		LUA_READ_PARAM(params, string, V_STRING, folder, "", 0, true);
		LUA_READ_PARAM(params, string, V_STRING, file, "", 1, false);

		lua_pushstring(L, STR(normalizePath(folder, file)));

		return 1;
	}

	int luaapi_generics_splitFileName(lua_State *L) {
		LUA_INIT_PARAMS(params, L);
		LUA_READ_PARAM(params, string, V_STRING, file, "", 0, true);
		string name;
		string extension;
		splitFileName(file, name, extension);
		Variant result;
		result["file"] = file;
		result["name"] = name;
		result["extension"] = extension;
		if (!PushVariant(L, result)) {
			FATAL("Unable to push result");
			return 0;
		}
		return 1;
	}
}
#endif	/* HAS_LUA */
