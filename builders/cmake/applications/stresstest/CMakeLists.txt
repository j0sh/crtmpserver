cmake_minimum_required(VERSION 2.6)
PROJECT(stresstest)

#make the list with all the source files
FILE(GLOB_RECURSE SOURCES "../../../../sources/applications/stresstest/src/*.cpp" )

#prepare the include directories
INCLUDE_DIRECTORIES(
	../../../../3rdparty/lua-dev
	../../../../sources/common/include
	../../../../sources/thelib/include
	../../../../sources/applications/stresstest/include
	)

#create THE library
ADD_LIBRARY(stresstest SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(stresstest thelib)

