MESSAGE(STATUS "Looking for dl")
FIND_PATH(DL_INCLUDE_PATH
	NAMES
		dlfcn.h
	PATHS
		${TOOLCHAIN_HEADER_PATH}
		/usr/include
		/usr/local/include
		/sw/include
		/opt/local/include
		NO_DEFAULT_PATH)

SET(CMAKE_FIND_LIBRARY_SUFFIXES .so.2 ${CMAKE_FIND_LIBRARY_SUFFIXES})

FIND_LIBRARY(DL_LIBRARY_PATH
	NAMES
		dl
	PATHS
		${TOOLCHAIN_LIBRARY_PATH}
		/lib64
		/usr/lib64
		/usr/local/lib64
		/lib/x86_64-linux-gnu
		/usr/lib/x86_64-linux-gnu
		/opt/local/lib64
		/sw/lib64
		/lib
		/usr/lib
		/usr/local/lib
		/lib/i386-linux-gnu
		/usr/lib/i386-linux-gnu
		/opt/local/lib
		/sw/lib
		NO_DEFAULT_PATH)

MESSAGE(STATUS "DL_INCLUDE_PATH: ${DL_INCLUDE_PATH}")
MESSAGE(STATUS "DL_LIBRARY_PATH: ${DL_LIBRARY_PATH}")

IF(DL_INCLUDE_PATH)
	SET(DL_FOUND 1 CACHE STRING "Set to 1 if dl is found, 0 otherwise")
	MESSAGE(STATUS "Looking for libdl headers - found")
ELSE(DL_INCLUDE_PATH)
	SET(DL_FOUND 0 CACHE STRING "Set to 1 if dl is found, 0 otherwise")
	MESSAGE(FATAL_ERROR "Looking for libdl headers - not found")
ENDIF(DL_INCLUDE_PATH)

IF(DL_LIBRARY_PATH)
	SET(DL_FOUND 1 CACHE STRING "Set to 1 if dl is found, 0 otherwise")
	MESSAGE(STATUS "Looking for libdl library - found")
ELSE(DL_LIBRddARY_PATH)
	SET(DL_LIBRARY_PATH "")
ENDIF(DL_LIBRARY_PATH)

MARK_AS_ADVANCED(DL_FOUND)

