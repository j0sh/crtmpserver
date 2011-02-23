cmake_minimum_required(VERSION 2.6)
PROJECT(samplefactory)

#make the list with all the source files
FILE(GLOB_RECURSE SOURCES "../../../../sources/applications/samplefactory/src/*.cpp" )

SET(CMAKE_INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${CRTMPSERVER_ROOT}/${PROJECT_NAME}")

#prepare the include directories
INCLUDE_DIRECTORIES(
	${OPENSSL_INCLUDE_PATH}
	../../../../3rdparty/lua-dev
	../../../../sources/common/include
	../../../../sources/thelib/include
	../../../../sources/applications/samplefactory/include
	)

#create THE library
ADD_LIBRARY(samplefactory SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(samplefactory thelib)
INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CRTMPSERVER_ROOT}/${PROJECT_NAME})

