cmake_minimum_required(VERSION 2.6)
PROJECT(vm)

#make the list with all the source files
FILE(GLOB_RECURSE SOURCES "../../../sources/vm/src/*.cpp" )

#prepare the include directories
INCLUDE_DIRECTORIES(
	${OPENSSL_INCLUDE_PATH}
	${V8_INCLUDE_PATH}
	../../../3rdparty/lua-dev
	../../../sources/common/include
	../../../sources/vm/include
	)


IF(${V8_FOUND})
	ADD_DEFINITIONS(-DVM_V8)
ENDIF(${V8_FOUND})

#create THE library
ADD_LIBRARY(vm SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(vm lua common pthread ${EXECINFO_LIBRARY_PATH} ${OPENSSL_LIBRARY_PATH} ${CRYPTO_LIBRARY_PATH} ${V8_LIBRARY_PATH})
ADD_PRECOMPILED_HEADER(vm ../../../sources/common/include/utils/core.h)

