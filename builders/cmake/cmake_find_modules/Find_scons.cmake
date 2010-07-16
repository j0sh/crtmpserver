MESSAGE(STATUS "Looking for scons")
FIND_PATH(SCONS_BIN_PATH 
	NAMES
		scons
	PATHS
		/usr/bin
		/usr/local/bin
		/sw/bin
		/sw/local/bin
		/opt/bin
		/opt/local/bin
	NO_DEFAULT_PATH)

IF(SCONS_BIN_PATH)
	SET(SCONS_FOUND 1 CACHE STRING "Set to 1 if scons is found, 0 otherwise")
	MESSAGE(STATUS "Looking for scons utility - found")
ELSE(SCONS_BIN_PATH)
	SET(SCONS_FOUND 0 CACHE STRING "Set to 1 if scons is found, 0 otherwise")
	MESSAGE(STATUS "Looking for scons binary - not found")
ENDIF(SCONS_BIN_PATH)

MARK_AS_ADVANCED(SCONS_FOUND)

