MESSAGE(STATUS "Looking for mhash")
FIND_PATH(MHASH_INCLUDE_PATH 
	NAMES
		mhash.h
  PATHS
		/usr/include
		/usr/local/include
		/sw/include
		/opt/local/include
  NO_DEFAULT_PATH)

FIND_LIBRARY(MHASH_LIBRARY_PATH
	NAMES
		mhash
	PATHS
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
  NO_DEFAULT_PATH)

IF(MHASH_INCLUDE_PATH)
	SET(MHASH_FOUND 1 CACHE STRING "Set to 1 if mhash is found, 0 otherwise")
	MESSAGE(STATUS "Looking for mhash headers - found")
ELSE(MHASH_INCLUDE_PATH)
	SET(MHASH_FOUND 0 CACHE STRING "Set to 1 if mhash is found, 0 otherwise")
	MESSAGE(FATAL_ERROR "Looking for mhash headers - not found")
ENDIF(MHASH_INCLUDE_PATH)

IF(MHASH_LIBRARY_PATH)
  SET(MHASH_FOUND 1 CACHE STRING "Set to 1 if mhash is found, 0 otherwise")
  MESSAGE(STATUS "Looking for mhash library - found")
ELSE(MHASH_LIBRARY_PATH)
  SET(MHASH_FOUND 0 CACHE STRING "Set to 1 if mhash is found, 0 otherwise")
  MESSAGE(FATAL_ERROR "Looking for mhash library - not found")
ENDIF(MHASH_LIBRARY_PATH)

MARK_AS_ADVANCED(MHASH_FOUND)
