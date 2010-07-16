MESSAGE(STATUS "Looking for execinfo")
FIND_PATH(EXECINFO_INCLUDE_PATH 
	NAMES
		execinfo.h
	PATHS
		/usr/include
		/usr/local/include
		/sw/include
		/opt/local/include
		NO_DEFAULT_PATH)

FIND_LIBRARY(EXECINFO_LIBRARY_PATH
	NAMES
		execinfo
	PATHS
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
		NO_DEFAULT_PATH)

IF(EXECINFO_INCLUDE_PATH)
	SET(EXECINFO_FOUND 1 CACHE STRING "Set to 1 if execinfo is found, 0 otherwise")
	MESSAGE(STATUS "Looking for execinfo headers - found")
ELSE(EXECINFO_INCLUDE_PATH)
	SET(EXECINFO_FOUND 0 CACHE STRING "Set to 1 if execinfo is found, 0 otherwise")
	MESSAGE(FATAL_ERROR "Looking for execinfo headers - not found")
ENDIF(EXECINFO_INCLUDE_PATH)

IF(EXECINFO_LIBRARY_PATH)
	SET(EXECINFO_FOUND 1 CACHE STRING "Set to 1 if execinfo is found, 0 otherwise")
	MESSAGE(STATUS "Looking for execinfo library - found")
ELSE(EXECINFO_LIBRARY_PATH)
	SET(EXECINFO_FOUND 0 CACHE STRING "Set to 1 if execinfo is found, 0 otherwise")
	MESSAGE(FATAL_ERROR "Looking for execinfo library - not found")
ENDIF(EXECINFO_LIBRARY_PATH)

MARK_AS_ADVANCED(EXECINFO_FOUND)
