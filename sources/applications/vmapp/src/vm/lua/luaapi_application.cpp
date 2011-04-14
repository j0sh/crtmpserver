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
#include "vm/lua/luaapi_application.h"
#include "vm/lua/luaapi_helpers.h"
#include "application/baseclientapplication.h"

namespace app_vmapp {

	int luaapi_application_getConfig(lua_State *L) {
		LUA_GET_APPLICATION(pApp, L);
		if (!PushVariant(L, pApp->GetConfiguration())) {
			FATAL("Unable to push variant");
			return 0;
		}
		return 1;
	}

	int luaapi_application_outboundConnectionFailed(lua_State *L) {
		NYIA;
		return 0;
	}

	int luaapi_application_pullExternalStream(lua_State *L) {
		LUA_GET_APPLICATION(pApp, L);
		LUA_INIT_PARAMS(params, L);
		LUA_READ_PARAM(params, Variant, V_MAP, streamConfig, Variant(), 0, true);
		Variant result = (bool)pApp->BaseClientApplication::PullExternalStream(streamConfig);
		if (!PushVariant(L, result)) {
			FATAL("Unable to push variant");
			return 0;
		}
		return 1;
	}

	int luaapi_application_pushLocalStream(lua_State *L) {
		NYI;
		return 0;
	}
}
#endif	/* HAS_LUA */
