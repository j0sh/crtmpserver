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
#include "utils/lua/luautils.h"
#include "utils/logging/logging.h"

void * _lua_alloc(void *ud, void *ptr, size_t osize, size_t nsize) {
	if (nsize == 0) {
		if (ptr != NULL)
			free(ptr);
		return NULL;
	} else {
		return realloc(ptr, nsize);
	}
}

lua_State * CreateLuaState(void *pOpaque) {
	lua_State *pResult = lua_newstate(_lua_alloc, pOpaque);
	luaL_openlibs(pResult);
	return pResult;
}

void DestroyLuaState(lua_State *pLuaState) {
	lua_close(pLuaState);
}

bool PopVariant(lua_State *pLuaState, Variant &variant, int32_t idx, bool pop) {
	variant.Reset();
	int32_t type = lua_type(pLuaState, idx);
	switch (type) {
		case LUA_TNIL:
		{
			variant.Reset();
			if (pop)
				lua_remove(pLuaState, idx);
			break;
		}
		case LUA_TBOOLEAN:
		{
			variant = (bool)(lua_toboolean(pLuaState, idx) != 0);
			if (pop)
				lua_remove(pLuaState, idx);
			break;
		}
		case LUA_TNUMBER:
		{
			lua_Number luaNumber = lua_tonumber(pLuaState, idx);
			if (pop)
				lua_remove(pLuaState, idx);
			variant = (double) luaNumber;
			variant.Compact();

			break;
		}
		case LUA_TSTRING:
		{
			string nullable = lua_tostring(pLuaState, idx);
			if (pop)
				lua_remove(pLuaState, idx);
			if (nullable == VAR_NULL_VALUE)
				variant = Variant();
			else
				variant = nullable;
			break;
		}
		case LUA_TTABLE:
		{
			bool isArray = true;
			lua_pushnil(pLuaState);
			while (lua_next(pLuaState, idx) != 0) {
				Variant value;
				if (!PopVariant(pLuaState, value, lua_gettop(pLuaState)))
					return false;

				Variant key;
				if (!PopVariant(pLuaState, key, lua_gettop(pLuaState), false))
					return false;

				variant[key] = value;
				isArray &= (key == _V_NUMERIC);
			}

			variant.IsArray(isArray);

			if (variant.HasKey(VAR_MAP_NAME)) {
				variant.SetTypeName(variant[VAR_MAP_NAME]);
				variant.RemoveKey(VAR_MAP_NAME);
			} else {
				variant.ConvertToTimestamp();
			}
			if (pop)
				lua_remove(pLuaState, idx);
			break;
		}
		default:
		{
			WARN("Element type not supported: %d (0x%x)", type, type);
			return false;
			break;
		}
	}
	return true;
}

bool PopStack(lua_State *pLuaState, Variant &variant) {
	variant.Reset();
	variant.IsArray(true);
	while (lua_gettop(pLuaState) > 0) {
		Variant temp;
		if (!PopVariant(pLuaState, temp)) {
			FATAL("Unable to pop variant");
			return false;
		}
		variant.PushToArray(temp);
	}
	return true;
}

bool PushVariant(lua_State *pLuaState,
		Variant &variant, bool substituteNullables) {
	switch ((VariantType) variant) {
		case V_UNDEFINED:
		case V_NULL:
		{
			if (substituteNullables)
				lua_pushstring(pLuaState, VAR_NULL_VALUE);
			else
				lua_pushnil(pLuaState);
			return true;
			break;
		}
		case V_STRING:
		{
			lua_pushstring(pLuaState, STR(variant));
			return true;
			break;
		}
		case V_INT8:
		case V_INT16:
		case V_INT32:
		case V_INT64:
		case V_UINT8:
		case V_UINT16:
		case V_UINT32:
		case V_UINT64:
		case V_DOUBLE:
		{
			lua_pushnumber(pLuaState, (double) variant);
			return true;
			break;
		}
		case V_BOOL:
		{
			lua_pushboolean(pLuaState, (bool)variant);
			return true;
			break;
		}
		case V_TIMESTAMP:
		{
			struct tm tempTm = (struct tm) variant;

			lua_createtable(pLuaState, 0, 0);

			lua_pushstring(pLuaState, VAR_TYPE);
			lua_pushstring(pLuaState, VAR_TIMESTAMP);
			lua_settable(pLuaState, -3);

			lua_pushstring(pLuaState, VAR_YEAR);
			lua_pushnumber(pLuaState, tempTm.tm_year + 1900);
			lua_settable(pLuaState, -3);

			lua_pushstring(pLuaState, VAR_MONTH);
			lua_pushnumber(pLuaState, tempTm.tm_mon+1);
			lua_settable(pLuaState, -3);

			lua_pushstring(pLuaState, VAR_DAY);
			lua_pushnumber(pLuaState, tempTm.tm_mday);
			lua_settable(pLuaState, -3);

			lua_pushstring(pLuaState, VAR_HOUR);
			lua_pushnumber(pLuaState, tempTm.tm_hour);
			lua_settable(pLuaState, -3);

			lua_pushstring(pLuaState, VAR_MIN);
			lua_pushnumber(pLuaState, tempTm.tm_min);
			lua_settable(pLuaState, -3);

			lua_pushstring(pLuaState, VAR_SEC);
			lua_pushnumber(pLuaState, tempTm.tm_sec);
			lua_settable(pLuaState, -3);

			lua_pushstring(pLuaState, VAR_ISDST);
			lua_pushboolean(pLuaState, false);
			lua_settable(pLuaState, -3);

			return true;
			break;
		}
		case V_DATE:
		{
			struct tm tempTm = (struct tm) variant;

			lua_createtable(pLuaState, 0, 0);

			lua_pushstring(pLuaState, VAR_TYPE);
			lua_pushstring(pLuaState, VAR_DATE);
			lua_settable(pLuaState, -3);

			lua_pushstring(pLuaState, VAR_YEAR);
			lua_pushnumber(pLuaState, tempTm.tm_year + 1900);
			lua_settable(pLuaState, -3);

			lua_pushstring(pLuaState, VAR_MONTH);
			lua_pushnumber(pLuaState, tempTm.tm_mon);
			lua_settable(pLuaState, -3);

			lua_pushstring(pLuaState, VAR_DAY);
			lua_pushnumber(pLuaState, tempTm.tm_mday);
			lua_settable(pLuaState, -3);
			return true;
			break;
		}
		case V_TIME:
		{
			struct tm tempTm = (struct tm) variant;

			lua_createtable(pLuaState, 0, 0);

			lua_pushstring(pLuaState, VAR_TYPE);
			lua_pushstring(pLuaState, VAR_TIME);
			lua_settable(pLuaState, -3);

			lua_pushstring(pLuaState, VAR_YEAR);
			lua_pushnumber(pLuaState, 1970);
			lua_settable(pLuaState, -3);

			lua_pushstring(pLuaState, VAR_MONTH);
			lua_pushnumber(pLuaState, 1);
			lua_settable(pLuaState, -3);

			lua_pushstring(pLuaState, VAR_DAY);
			lua_pushnumber(pLuaState, 1);
			lua_settable(pLuaState, -3);

			lua_pushstring(pLuaState, VAR_HOUR);
			lua_pushnumber(pLuaState, tempTm.tm_hour);
			lua_settable(pLuaState, -3);

			lua_pushstring(pLuaState, VAR_MIN);
			lua_pushnumber(pLuaState, tempTm.tm_min);
			lua_settable(pLuaState, -3);

			lua_pushstring(pLuaState, VAR_SEC);
			lua_pushnumber(pLuaState, tempTm.tm_sec);
			lua_settable(pLuaState, -3);

			lua_pushstring(pLuaState, VAR_ISDST);
			lua_pushboolean(pLuaState, false);
			lua_settable(pLuaState, -3);

			return true;
			break;
		}
		case V_TYPED_MAP:
		case V_MAP:
		{
			lua_createtable(pLuaState, 0, 0);

			if ((VariantType) variant == V_TYPED_MAP) {
				lua_pushstring(pLuaState, VAR_MAP_NAME);
				lua_pushstring(pLuaState, STR(variant.GetTypeName()));
				lua_settable(pLuaState, -3);
			}

			FOR_MAP(variant, string, Variant, i) {
				if (MAP_KEY(i).find(VAR_INDEX_VALUE) == 0) {
					string temp = MAP_KEY(i).substr(VAR_INDEX_VALUE_LEN,
							string::npos);
					char *error = NULL;
					double index = strtod(STR(temp), &error);
					if (error == STR(temp) + temp.size()) {
						lua_pushnumber(pLuaState, index);
					} else {
						lua_pushstring(pLuaState, STR(MAP_KEY(i)));
					}
				} else {
					lua_pushstring(pLuaState, STR(MAP_KEY(i)));
				}
				if (!PushVariant(pLuaState, MAP_VAL(i), true)) {
					FINEST("Unable to push primitive");
					return false;
				}
				lua_settable(pLuaState, -3);
			}
			return true;
			break;
		}
		default:
		{
			FATAL("Unknown type %hhu", (VariantType) variant);
			return false;
			break;
		}
	}
	return true;
}

bool EvalLuaExpression(lua_State *pLuaState, string expression) {
	if (luaL_dostring(pLuaState, STR("return " + expression)) != 0) {
		Variant v;
		PopStack(pLuaState, v);
		FATAL("Unable to evaluate expression %s\n%s", STR(expression), STR(v.ToString()));
		return false;
	}
	return true;
}

bool LoadLuaScriptFromFile(string file, lua_State *pLuaState, bool pCall) {
	if (luaL_loadfile(pLuaState, STR(file)) != 0) {
		FATAL("Error parsing file %s: %s",
				STR(file),
				lua_tostring(pLuaState, -1));
		return false;
	}

	if (pCall) {
		if (lua_pcall(pLuaState, 0, LUA_MULTRET, 0) != 0) {
			FATAL("Error parsing file %s: %s",
					STR(file),
					lua_tostring(pLuaState, -1));
			return false;
		}
	}

	return true;
}

bool LoadLuaScriptFromString(string luaScript, lua_State *pLuaState, bool pCall) {
	if (luaL_loadstring(pLuaState, STR(luaScript)) != 0) {
		FATAL("Error parsing script %s: %s",
				STR(luaScript),
				lua_tostring(pLuaState, -1));
		return false;
	}

	if (pCall) {
		if (lua_pcall(pLuaState, 0, 0, 0) != 0) {
			FATAL("Error parsing script %s: %s",
					STR(luaScript),
					lua_tostring(pLuaState, -1));
			return false;
		}
	}

	return true;
}

bool ReadLuaState(lua_State *pLuaState, string node, Variant &configuration) {

	if (!EvalLuaExpression(pLuaState, node)) {
		FATAL("Unable to read config. No %s section defined", STR(node));
		return false;
	}

	if (!PopVariant(pLuaState, configuration)) {
		FATAL("Unable to read config. Invalid configuration section");
		return false;
	}

	if ((VariantType) configuration != V_MAP) {
		FATAL("Unable to read config. Invalid configuration section");
		return false;
	}

	return true;
}

bool ReadLuaFile(string fileName, string section, Variant &configuration) {
	lua_State *pLuaState = CreateLuaState(NULL);

	if (!LoadLuaScriptFromFile(fileName, pLuaState)) {
		DestroyLuaState(pLuaState);
		return false;
	}

	bool res = ReadLuaState(pLuaState, section, configuration);
	DestroyLuaState(pLuaState);
	return res;
}

bool ReadLuaString(string script, string section, Variant &configuration) {
	lua_State *pLuaState = CreateLuaState(NULL);

	if (!LoadLuaScriptFromString(script, pLuaState)) {
		DestroyLuaState(pLuaState);
		return false;
	}

	bool res = ReadLuaState(pLuaState, section, configuration);
	DestroyLuaState(pLuaState);
	return res;
}
#endif /* HAS_LUA */
