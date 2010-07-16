cmake_minimum_required(VERSION 2.6)
PROJECT(vmtests)

#make the list with all the source files
FILE(GLOB_RECURSE SOURCES "../../../sources/vmtests/src/*.cpp" )

#prepare the include directories
INCLUDE_DIRECTORIES(
	${OPENSSL_INCLUDE_PATH}
	../../../3rdparty/lua-dev
	../../../sources/vmtests/include
	../../../sources/vm/include
	../../../sources/common/include
        )

ADD_EXECUTABLE(vmtests ${SOURCES})
TARGET_LINK_LIBRARIES(vmtests lua common vm ${DL_LIBRARY_PATH})

