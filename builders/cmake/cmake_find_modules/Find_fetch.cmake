FIND_PATH(FETCH_INCLUDE_PATH 
	NAMES
		fetch.h
	PATHS
		/usr/include
		/usr/local/include
		/sw/include
		/opt/local/include
		NO_DEFAULT_PATH)

FIND_LIBRARY(FETCH_LIBRARY_PATH
	NAMES
		fetch
	PATHS
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
		NO_DEFAULT_PATH)

IF(FETCH_INCLUDE_PATH)
	SET(FETCH_FOUND 1 CACHE STRING "Set to 1 if fetch is found, 0 otherwise")
	MESSAGE(STATUS "Looking for fetch headers - found")
ELSE(FETCH_INCLUDE_PATH)
	SET(FETCH_FOUND 0 CACHE STRING "Set to 1 if fetch is found, 0 otherwise")
	MESSAGE(FATAL_ERROR "Looking for fetch headers - not found")
ENDIF(FETCH_INCLUDE_PATH)

IF(FETCH_LIBRARY_PATH)
	SET(FETCH_FOUND 1 CACHE STRING "Set to 1 if fetch is found, 0 otherwise")
	MESSAGE(STATUS "Looking for fetch library - found")
ELSE(FETCH_LIBRARY_PATH)
	SET(FETCH_FOUND 0 CACHE STRING "Set to 1 if fetch is found, 0 otherwise")
	MESSAGE(FATAL_ERROR "Looking for fetch library - not found")
ENDIF(FETCH_LIBRARY_PATH)

MARK_AS_ADVANCED(FETCH_FOUND)

