MESSAGE(STATUS "Looking for lua")
FIND_PATH(LUA_INCLUDE_PATH_ 
	NAMES
		lualib.h
	PATHS
		/usr/include
		/usr/local/include
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
		MESSAGE(STATUS "Looking for lua library - not found. Lua library will be compiled from ${CRTMPSERVER_3RDPARTY_ROOT}/lua-dev")
	ENDIF(LUA_LIBRARY_PATH_)
ELSEIF(LUA_FOUND)
	SET(LUA_FOUND 0 CACHE STRING "Set to 1 if lua is found, 0 otherwise")
	SET(LUA_LIBRARY_PATH "lua")
	MESSAGE(STATUS "Looking for lua library - not found. Lua library will be compiled from ${CRTMPSERVER_3RDPARTY_ROOT}/lua-dev")
ENDIF(LUA_FOUND)

MARK_AS_ADVANCED(LUA_FOUND)

