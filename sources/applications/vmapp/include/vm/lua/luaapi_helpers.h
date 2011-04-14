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
#ifndef _LUAAPI_HELPERS_H
#define	_LUAAPI_HELPERS_H

#define LUA_GET_APPLICATION(pApp,L) \
BaseClientApplication *pApp = NULL; \
do { \
	void *___pTempPointer = NULL; \
	lua_getallocf((L), &___pTempPointer); \
	if(___pTempPointer==NULL) { \
		FATAL("Unable to get the application from the lua state"); \
		return 0; \
	} \
	pApp = (BaseClientApplication *) ___pTempPointer; \
} while (0);

#define LUA_GET_PROTOCOL(pProtocol,protocolId,L) \
BaseProtocol *pProtocol = ProtocolManager::GetProtocol(protocolId); \
if (pProtocol == NULL) { \
	FATAL("Protocol %d not available",protocolId); \
	lua_pushboolean(L, false); \
	return 1; \
} \
do{}while(0)

#define LUA_INIT_PARAMS(params, L) \
Variant params; \
if (!PopStack((L), params)) { \
	FATAL("Unable to pop stack"); \
	return 0; \
}

#define LUA_READ_PARAM(params,ctype,vtype,name,implicit,index,mandatory) \
ctype name=(implicit); \
if((params).MapSize()<((index)+1)) { \
	if((mandatory)) { \
		FATAL("Too few parameters"); \
		return 0; \
	} \
} else { \
	if ((params)[(uint32_t) (index)] != (vtype)) { \
		FATAL("Incorrect parameter type"); \
		return 0; \
	} \
	name=(ctype)(params)[(uint32_t) (index)]; \
}

#define LUA_READ_KEY(params,ctype,vtype,name,implicit,key,caseSensitive,mandatory) \
ctype name=(implicit); \
if(!(params).HasKey((key),(caseSensitive))) { \
	if((mandatory)) { \
		FATAL("key not found"); \
		return 0; \
	} \
} else { \
	if((params).GetValue((key),(caseSensitive))!=(vtype)) { \
		FATAL("Incorrect parameter type"); \
		return 0; \
	} \
	name=(ctype)(params).GetValue((key),(caseSensitive)); \
}

#endif	/* _LUAAPI_HELPERS_H */
#endif	/* HAS_LUA */
