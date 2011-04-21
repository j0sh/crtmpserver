MESSAGE(STATUS "Looking for v8")
FIND_PATH(V8_INCLUDE_PATH 
	NAMES
		v8.h
	PATHS
		/usr/include
		/usr/local/include
		/sw/include
		/opt/local/include
		~/work/v8/include
		NO_DEFAULT_PATH)

FIND_LIBRARY(V8_LIBRARY_PATH
	NAMES
		v8_g
	PATHS
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
		~/work/v8		
		NO_DEFAULT_PATH)

IF(V8_INCLUDE_PATH)
	SET(V8_FOUND 1 CACHE STRING "Set to 1 if v8 is found, 0 otherwise")
	MESSAGE(STATUS "Looking for v8 headers - found")
ELSE(V8_INCLUDE_PATH)
	SET(V8_FOUND 0 CACHE STRING "Set to 1 if v8 is found, 0 otherwise")
	MESSAGE(STATUS "Looking for v8 headers - not found")
	SET(V8_INCLUDE_PATH "")
ENDIF(V8_INCLUDE_PATH)

IF(V8_LIBRARY_PATH)
	SET(V8_FOUND 1 CACHE STRING "Set to 1 if v8 is found, 0 otherwise")
	MESSAGE(STATUS "Looking for v8 library - found")
ELSE(V8_LIBRARY_PATH)
	SET(V8_FOUND 0 CACHE STRING "Set to 1 if v8 is found, 0 otherwise")
	MESSAGE(STATUS "Looking for v8 library - not found")
	SET(V8_LIBRARY_PATH "")
ENDIF(V8_LIBRARY_PATH)

MARK_AS_ADVANCED(V8_FOUND)

