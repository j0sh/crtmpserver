cmake_minimum_required(VERSION 2.6)
PROJECT(thelib)

#make the list with all the source files
FILE(GLOB_RECURSE SOURCES "../../../sources/thelib/src/*.cpp" )

#prepare the include directories
INCLUDE_DIRECTORIES(
	${OPENSSL_INCLUDE_PATH}
	../../../3rdparty/lua-dev
	../../../sources/common/include
	../../../sources/thelib/include
	)

#create THE library
ADD_LIBRARY(thelib SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(thelib common ${DL_LIBRARY_PATH} ${OPENSSL_LIBRARY_PATH} ${CRYPTO_LIBRARY_PATH})
ADD_PRECOMPILED_HEADER(thelib ../../../sources/common/include/utils/core.h)

