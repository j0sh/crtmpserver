cmake_minimum_required(VERSION 2.6)
PROJECT(#APPNAME_LC#)

#make the list with all the source files
FILE(GLOB_RECURSE SOURCES "../../../../sources/applications/#APPNAME_LC#/src/*.cpp" )

#prepare the include directories
INCLUDE_DIRECTORIES(
	${LUA_INCLUDE_PATH}
	../../../../sources/common/include
	../../../../sources/thelib/include
	../../../../sources/applications/#APPNAME_LC#/include
	)

#create THE library
ADD_LIBRARY(#APPNAME_LC# SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(#APPNAME_LC# thelib)

