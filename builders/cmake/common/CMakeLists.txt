cmake_minimum_required(VERSION 2.6)
PROJECT(common)

#make the list with all the source files
FILE(GLOB_RECURSE SOURCES "../../../sources/common/src/*.cpp" )

#prepare the include directories
INCLUDE_DIRECTORIES(
	${OPENSSL_INCLUDE_PATH}
	../../../3rdparty/lua-dev
	../../../sources/common/include
	)

#create THE library
ADD_LIBRARY(common SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(common lua ${OPENSSL_LIBRARY_PATH} ${CRYPTO_LIBRARY_PATH})
ADD_PRECOMPILED_HEADER(common ../../../sources/common/include/common.h) 

