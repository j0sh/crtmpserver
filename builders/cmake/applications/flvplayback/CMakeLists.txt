cmake_minimum_required(VERSION 2.6)
PROJECT(flvplayback)

#make the list with all the source files
FILE(GLOB_RECURSE SOURCES "../../../../sources/applications/flvplayback/src/*.cpp" )

#prepare the include directories
INCLUDE_DIRECTORIES(
	${OPENSSL_INCLUDE_PATH}
	../../../3rdparty/lua-dev
	../../../../sources/common/include
	../../../../sources/thelib/include
	../../../../sources/applications/flvplayback/include
	)

#create THE library
ADD_LIBRARY(flvplayback SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(flvplayback thelib)

