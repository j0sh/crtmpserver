cmake_minimum_required(VERSION 2.6.3)
PROJECT(tests)

#make the list with all the source files
FILE(GLOB_RECURSE SOURCES "../../../sources/tests/src/*.cpp" )


INCLUDE_DIRECTORIES(
	${OPENSSL_INCLUDE_PATH}
	../../../sources/common/include
	../../../sources/thelib/include
	../../../sources/tests/include
)

ADD_EXECUTABLE(tests  ${SOURCES})
TARGET_LINK_LIBRARIES(tests common thelib ${DL_LIBRARY_PATH})

