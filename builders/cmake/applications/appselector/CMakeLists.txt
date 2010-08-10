cmake_minimum_required(VERSION 2.6)
PROJECT(appselector)

#make the list with all the source files
FILE(GLOB_RECURSE SOURCES "../../../../sources/applications/appselector/src/*.cpp" )

#prepare the include directories
INCLUDE_DIRECTORIES(
	${OPENSSL_INCLUDE_PATH}
	../../../../3rdparty/lua-dev
	../../../../sources/common/include
	../../../../sources/thelib/include
	../../../../sources/applications/appselector/include
	)

#create THE library
ADD_LIBRARY(appselector SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(appselector thelib)

