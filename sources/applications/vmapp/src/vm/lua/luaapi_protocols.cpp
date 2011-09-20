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
#include "vm/lua/luaapi_protocols.h"
#include "vm/lua/luaapi_helpers.h"
#include "protocols/baseprotocol.h"
#include "protocols/protocolmanager.h"

namespace app_vmapp {

	int luaapi_protocols_getConfig(lua_State *L) {
		LUA_INIT_PARAMS(params, L);
		LUA_READ_PARAM(params, uint32_t, _V_NUMERIC, protocolId, 0, 0, true);
		LUA_GET_PROTOCOL(pProtocol, protocolId, L);
		Variant stackInfo;
		pProtocol->GetStackStats(stackInfo, 0);
		if (!PushVariant(L, stackInfo)) {
			FATAL("Unable to push variant");
			return 0;
		}
		return 1;
	}

	int luaapi_protocols_enqueueForDelete(lua_State *L) {
		LUA_INIT_PARAMS(params, L);
		LUA_READ_PARAM(params, uint32_t, _V_NUMERIC, protocolId, 0, 0, true);
		LUA_GET_PROTOCOL(pProtocol, protocolId, L);
		pProtocol->EnqueueForDelete();
		lua_pushboolean(L, true);
		return 1;
	}

	int luaapi_protocols_gracefullyEnqueueForDelete(lua_State *L) {
		LUA_INIT_PARAMS(params, L);
		LUA_READ_PARAM(params, uint32_t, _V_NUMERIC, protocolId, 0, 0, true);
		LUA_GET_PROTOCOL(pProtocol, protocolId, L);
		pProtocol->GracefullyEnqueueForDelete();
		lua_pushboolean(L, true);
		return 1;
	}
}
#endif	/* HAS_LUA */
