FIND_PATH(FCGI_INCLUDE_PATH 
	NAMES
		fcgi_config.h
  PATHS
		/usr/include
		/usr/local/include
		/sw/include
		/opt/local/include
  NO_DEFAULT_PATH)

FIND_LIBRARY(FCGI_LIBRARY_PATH
	NAMES
		fcgi
	PATHS
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
  NO_DEFAULT_PATH)

IF(FCGI_INCLUDE_PATH)
	SET(FCGI_FOUND 1)
	MESSAGE(STATUS "Looking for fcgi headers - found")
ELSE(FCGI_INCLUDE_PATH)
	SET(FCGI_FOUND 0)
	MESSAGE(STATUS "Looking for fcgi headers - not found")
ENDIF(FCGI_INCLUDE_PATH)

IF(FCGI_LIBRARY_PATH)
  SET(FCGI_FOUND 1)
  MESSAGE(STATUS "Looking for fcgi library - found")
ELSE(FCGI_LIBRARY_PATH)
  SET(FCGI_FOUND 0)
  MESSAGE(STATUS "Looking for fcgi library - not found")
ENDIF(FCGI_LIBRARY_PATH)

MARK_AS_ADVANCED(FCGI_FOUND)
