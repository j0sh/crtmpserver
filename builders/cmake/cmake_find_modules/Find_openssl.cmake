FIND_PATH(OPENSSL_INCLUDE_PATH 
	NAMES
		openssl/ssl.h
	PATHS
		/usr/include
		/usr/local/include
		/sw/include
		/opt/local/include
		/usr/local/ssl/include
		NO_DEFAULT_PATH)

FIND_LIBRARY(OPENSSL_LIBRARY_PATH
	NAMES
		ssl
	PATHS
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
		/usr/local/ssl/lib
		/lib
		NO_DEFAULT_PATH)

FIND_LIBRARY(CRYPTO_LIBRARY_PATH
	NAMES
		crypto
	PATHS
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
		/usr/local/ssl/lib
		/lib
		NO_DEFAULT_PATH)


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

MARK_AS_ADVANCED(OPENSSL_FOUND)

