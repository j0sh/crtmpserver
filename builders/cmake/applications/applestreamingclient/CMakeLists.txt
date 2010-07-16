cmake_minimum_required(VERSION 2.6)
PROJECT(applestreamingclient)

#make the list with all the source files
FILE(GLOB_RECURSE SOURCES "../../../../sources/applications/applestreamingclient/src/*.cpp" )

#prepare the include directories
INCLUDE_DIRECTORIES(
	../../../3rdparty/lua-dev
	../../../../sources/common/include
	../../../../sources/thelib/include
	../../../../sources/applications/applestreamingclient/include
	)

#create THE library
ADD_LIBRARY(applestreamingclient SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(applestreamingclient thelib)

