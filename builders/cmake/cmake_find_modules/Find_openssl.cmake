FIND_PATH(OPENSSL_INCLUDE_PATH 
	NAMES
		openssl/ssl.h
	PATHS
		/opt/local/include
		/usr/include
		/usr/local/include
		/sw/include
		/usr/local/ssl/include
		NO_DEFAULT_PATH)

IF($ENV{COMPILE_STATIC} MATCHES "1")
	SET(CMAKE_FIND_LIBRARY_SUFFIXES_OLD ${CMAKE_FIND_LIBRARY_SUFFIXES})
	SET(CMAKE_FIND_LIBRARY_SUFFIXES .a .so .dylib ${CMAKE_FIND_LIBRARY_SUFFIXES})
ENDIF($ENV{COMPILE_STATIC} MATCHES "1")

FIND_LIBRARY(OPENSSL_LIBRARY_PATH
	NAMES
		ssl
	PATHS
		/opt/local/lib
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/usr/local/ssl/lib
		/lib
		NO_DEFAULT_PATH)

FIND_LIBRARY(CRYPTO_LIBRARY_PATH
	NAMES
		crypto
	PATHS
		/opt/local/lib
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/usr/local/ssl/lib
		/lib
		NO_DEFAULT_PATH)

IF($ENV{COMPILE_STATIC} MATCHES "1")
	FIND_LIBRARY(Z_LIBRARY_PATH
		NAMES
			z
		PATHS
			/opt/local/lib
			/usr/lib64
			/usr/lib
			/usr/local/lib64
			/usr/local/lib
			/sw/lib
			/usr/local/ssl/lib
			/lib
			NO_DEFAULT_PATH)
	SET(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES_OLD})
ELSEIF($ENV{COMPILE_STATIC} MATCHES "1")
	SET(Z_LIBRARY_PATH "")
ENDIF($ENV{COMPILE_STATIC} MATCHES "1")

MESSAGE("OPENSSL_INCLUDE_PATH: ${OPENSSL_INCLUDE_PATH}")
MESSAGE("OPENSSL_LIBRARY_PATH: ${OPENSSL_LIBRARY_PATH}")
MESSAGE("CRYPTO_LIBRARY_PATH: ${CRYPTO_LIBRARY_PATH}")
MESSAGE("Z_LIBRARY_PATH: ${Z_LIBRARY_PATH}")

IF(OPENSSL_INCLUDE_PATH)
	SET(OPENSSL_FOUND 1 CACHE STRING "Set to 1 if openssl is found, 0 otherwise")
	MESSAGE(STATUS "Looking for openssl headers - found")
ELSE(OPENSSL_INCLUDE_PATH)
	SET(OPENSSL_FOUND 0 CACHE STRING "Set to 1 if openssl is found, 0 otherwise")
	MESSAGE(FATAL_ERROR "Looking for openssl headers - not found")
ENDIF(OPENSSL_INCLUDE_PATH)

IF(OPENSSL_LIBRARY_PATH)
	SET(OPENSSL_FOUND 1 CACHE STRING "Set to 1 if openssl is found, 0 otherwise")
	MESSAGE(STATUS "Looking for openssl library - found")
ELSE(OPENSSL_LIBRARY_PATH)
	SET(OPENSSL_FOUND 0 CACHE STRING "Set to 1 if openssl is found, 0 otherwise")
	MESSAGE(FATAL_ERROR "Looking for openssl library - not found")
ENDIF(OPENSSL_LIBRARY_PATH)

IF(CRYPTO_LIBRARY_PATH)
	SET(OPENSSL_FOUND 1 CACHE STRING "Set to 1 if openssl is found, 0 otherwise")
	MESSAGE(STATUS "Looking for crypto library - found")
ELSE(CRYPTO_LIBRARY_PATH)
	SET(OPENSSL_FOUND 0 CACHE STRING "Set to 1 if openssl is found, 0 otherwise")
	MESSAGE(FATAL_ERROR "Looking for crypto library - not found")
ENDIF(CRYPTO_LIBRARY_PATH)

IF($ENV{COMPILE_STATIC} MATCHES "1")
	IF(Z_LIBRARY_PATH)
		SET(OPENSSL_FOUND 1 CACHE STRING "Set to 1 if openssl is found, 0 otherwise")
		MESSAGE(STATUS "Looking for z library - found")
	ELSE(Z_LIBRARY_PATH)
		SET(OPENSSL_FOUND 0 CACHE STRING "Set to 1 if openssl is found, 0 otherwise")
		MESSAGE(FATAL_ERROR "Looking for z library - not found")
	ENDIF(Z_LIBRARY_PATH)
ENDIF($ENV{COMPILE_STATIC} MATCHES "1")

MARK_AS_ADVANCED(OPENSSL_FOUND)

