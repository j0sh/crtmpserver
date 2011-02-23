cmake_minimum_required(VERSION 2.6)
PROJECT(proxypublish)

#make the list with all the source files
FILE(GLOB_RECURSE SOURCES "../../../../sources/applications/proxypublish/src/*.cpp" )

SET(CMAKE_INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${CRTMPSERVER_ROOT}/${PROJECT_NAME}")

#prepare the include directories
INCLUDE_DIRECTORIES(
	../../../../3rdparty/lua-dev
	../../../../sources/common/include
	../../../../sources/thelib/include
	../../../../sources/applications/proxypublish/include
	)

#create THE library
ADD_LIBRARY(proxypublish SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(proxypublish thelib)
INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CRTMPSERVER_ROOT}/${PROJECT_NAME})

