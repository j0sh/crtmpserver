cmake_minimum_required(VERSION 2.6.3)
PROJECT(tests)

#make the list with all the source files
FILE(GLOB_RECURSE SOURCES "${CRTMPSERVER_SOURCES_ROOT}/tests/src/*.cpp" )


INCLUDE_DIRECTORIES(
	${OPENSSL_INCLUDE_PATH}
	${LUA_INCLUDE_PATH}
	${CRTMPSERVER_SOURCES_ROOT}/common/include
	${CRTMPSERVER_SOURCES_ROOT}/thelib/include
	${CRTMPSERVER_SOURCES_ROOT}/tests/include
)

ADD_EXECUTABLE(tests  ${SOURCES})
TARGET_LINK_LIBRARIES(tests common thelib ${DL_LIBRARY_PATH} ${SOCKET_LIB})

