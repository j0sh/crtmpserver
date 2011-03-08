cmake_minimum_required(VERSION 2.6)
PROJECT(flvplayback)

#make the list with all the source files
FILE(GLOB_RECURSE SOURCES "${CRTMPSERVER_SOURCES_ROOT}/applications/flvplayback/src/*.cpp" )

SET(CMAKE_INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${CRTMPSERVER_ROOT}/${PROJECT_NAME}")

#prepare the include directories
INCLUDE_DIRECTORIES(
	${OPENSSL_INCLUDE_PATH}
	${LUA_INCLUDE_PATH}
	${CRTMPSERVER_SOURCES_ROOT}/common/include
	${CRTMPSERVER_SOURCES_ROOT}/thelib/include
	${CRTMPSERVER_SOURCES_ROOT}/applications/flvplayback/include
	)

#create THE library
ADD_LIBRARY(flvplayback SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(flvplayback thelib)
ADD_PRECOMPILED_HEADER(${PROJECT_NAME} "${CRTMPSERVER_SOURCES_ROOT}/common/include/common.h")
INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CRTMPSERVER_ROOT}/${PROJECT_NAME})

