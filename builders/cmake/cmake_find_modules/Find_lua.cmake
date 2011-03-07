MESSAGE(STATUS "Looking for lua")
FIND_PATH(LUA_INCLUDE_PATH_ 
	NAMES
		lualib.h
	PATHS
		/usr/include
		/usr/local/include
		/usr/local/include/lua51
		/sw/include
		/opt/local/include
		NO_DEFAULT_PATH)

FIND_LIBRARY(LUA_LIBRARY_PATH_
	NAMES
		lua
	PATHS
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/usr/local/lib64/lua51
		/usr/local/lib/lua51
		/sw/lib
		/opt/local/lib
		NO_DEFAULT_PATH)

IF(LUA_INCLUDE_PATH_)
	SET(LUA_FOUND 1)
	SET(LUA_INCLUDE_PATH "${LUA_INCLUDE_PATH_}")
	MESSAGE(STATUS "Looking for lua headers - found")
ELSE(LUA_INCLUDE_PATH_)
	SET(LUA_FOUND 0)
	SET(LUA_INCLUDE_PATH "${CRTMPSERVER_3RDPARTY_ROOT}/lua-dev")
	MESSAGE(STATUS "Looking for lua headers - not found. Defaulting to ${CRTMPSERVER_3RDPARTY_ROOT}/lua-dev")
ENDIF(LUA_INCLUDE_PATH_)

IF(LUA_FOUND)
	IF(LUA_LIBRARY_PATH_)
		SET(LUA_LIBRARY_PATH "${LUA_LIBRARY_PATH_}")
		SET(LUA_FOUND 1 CACHE STRING "Set to 1 if lua is found, 0 otherwise")
		MESSAGE(STATUS "Looking for lua library - found")
	ELSE(LUA_LIBRARY_PATH_)
		SET(LUA_FOUND 0 CACHE STRING "Set to 1 if lua is found, 0 otherwise")
		SET(LUA_LIBRARY_PATH "lua")
		SET(LUA_INCLUDE_PATH "${CRTMPSERVER_3RDPARTY_ROOT}/lua-dev")
		MESSAGE(STATUS "Looking for lua library - not found. Lua library will be compiled from ${CRTMPSERVER_3RDPARTY_ROOT}/lua-dev")
	ENDIF(LUA_LIBRARY_PATH_)
ELSE(LUA_FOUND)
	SET(LUA_FOUND 0 CACHE STRING "Set to 1 if lua is found, 0 otherwise")
	SET(LUA_LIBRARY_PATH "lua")
	MESSAGE(STATUS "Looking for lua library - not found. Lua library will be compiled from ${CRTMPSERVER_3RDPARTY_ROOT}/lua-dev")
ENDIF(LUA_FOUND)

#MESSAGE("final LUA_FOUND: ${LUA_FOUND}")
#MESSAGE("final LUA_INCLUDE_PATH: ${LUA_INCLUDE_PATH}")
#MESSAGE("final LUA_LIBRARY_PATH: ${LUA_LIBRARY_PATH}")
MARK_AS_ADVANCED(LUA_FOUND)

