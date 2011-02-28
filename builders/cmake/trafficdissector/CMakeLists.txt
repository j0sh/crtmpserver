cmake_minimum_required(VERSION 2.6.3)
PROJECT(trafficdissector)

#make the list with all the source files
FILE(GLOB_RECURSE SOURCES "${CRTMPSERVER_SOURCES_ROOT}/trafficdissector/src/*.cpp" )


INCLUDE_DIRECTORIES(
	${OPENSSL_INCLUDE_PATH}
	${LUA_INCLUDE_PATH}
	${CRTMPSERVER_SOURCES_ROOT}/common/include
	${CRTMPSERVER_SOURCES_ROOT}/thelib/include
	${CRTMPSERVER_SOURCES_ROOT}/trafficdissector/include
)

ADD_EXECUTABLE(trafficdissector ${SOURCES})
TARGET_LINK_LIBRARIES(trafficdissector common thelib ${DL_LIBRARY_PATH})

