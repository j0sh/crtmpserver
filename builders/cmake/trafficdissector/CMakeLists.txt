cmake_minimum_required(VERSION 2.6.3)
PROJECT(trafficdissector)

#make the list with all the source files
FILE(GLOB_RECURSE SOURCES "../../../sources/trafficdissector/src/*.cpp" )


INCLUDE_DIRECTORIES(
	${OPENSSL_INCLUDE_PATH}
	../../../3rdparty/lua-dev
	../../../sources/common/include
	../../../sources/thelib/include
	../../../sources/trafficdissector/include
)

ADD_EXECUTABLE(trafficdissector ${SOURCES})
TARGET_LINK_LIBRARIES(trafficdissector common thelib ${DL_LIBRARY_PATH})

