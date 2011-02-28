cmake_minimum_required(VERSION 2.6)
PROJECT(samplefactory)

#make the list with all the source files
FILE(GLOB_RECURSE SOURCES "${CRTMPSERVER_SOURCES_ROOT}/applications/samplefactory/src/*.cpp" )

SET(CMAKE_INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${CRTMPSERVER_ROOT}/${PROJECT_NAME}")

#prepare the include directories
INCLUDE_DIRECTORIES(
	${OPENSSL_INCLUDE_PATH}
	${LUA_INCLUDE_PATH}
	${CRTMPSERVER_SOURCES_ROOT}/common/include
	${CRTMPSERVER_SOURCES_ROOT}/thelib/include
	${CRTMPSERVER_SOURCES_ROOT}/applications/samplefactory/include
	)

#create THE library
ADD_LIBRARY(samplefactory SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(samplefactory thelib)
INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CRTMPSERVER_ROOT}/${PROJECT_NAME})

