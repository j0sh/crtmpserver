cmake_minimum_required(VERSION 2.6)
PROJECT(vmtests)

#make the list with all the source files
FILE(GLOB_RECURSE SOURCES "${CRTMPSERVER_SOURCES_ROOT}/vmtests/src/*.cpp" )

#prepare the include directories
INCLUDE_DIRECTORIES(
	${OPENSSL_INCLUDE_PATH}
	${LUA_INCLUDE_PATH}
	${CRTMPSERVER_SOURCES_ROOT}/vmtests/include
	${CRTMPSERVER_SOURCES_ROOT}/vm/include
	${CRTMPSERVER_SOURCES_ROOT}/common/include
)

ADD_EXECUTABLE(vmtests ${SOURCES})
TARGET_LINK_LIBRARIES(vmtests ${LUA_LIBRARY_PATH} common vm ${DL_LIBRARY_PATH})
ADD_PRECOMPILED_HEADER(${PROJECT_NAME} "${CRTMPSERVER_SOURCES_ROOT}/common/include/common.h")

