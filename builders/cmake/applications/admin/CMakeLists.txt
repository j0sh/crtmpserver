cmake_minimum_required(VERSION 2.6)
PROJECT(admin)

#make the list with all the source files
FILE(GLOB_RECURSE SOURCES "../../../../sources/applications/admin/src/*.cpp" )

#prepare the include directories
INCLUDE_DIRECTORIES(
	../../../../3rdparty/lua-dev
	../../../../sources/common/include
	../../../../sources/thelib/include
	../../../../sources/applications/admin/include
	)

#create THE library
ADD_LIBRARY(admin SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(admin thelib)

