cmake_minimum_required(VERSION 2.6)
PROJECT(rtmpserver)

#prepare the include directories
INCLUDE_DIRECTORIES(
        ../../../3rdparty/lua-dev
        ../../../sources/common/include
	../../../sources/thelib/include
        )

EXECUTE_PROCESS(COMMAND svnversion -n ../../sources OUTPUT_VARIABLE TEMP_FRAMEWORK_VER)
ADD_DEFINITIONS(-DRTMPSERVER_VERSION="0.${TEMP_FRAMEWORK_VER}")
ADD_EXECUTABLE(rtmpserver ../../../sources/rtmpserver/src/rtmpserver.cpp)
TARGET_LINK_LIBRARIES(rtmpserver thelib ${OPENSSL_LIBRARY_PATH} ${CRYPTO_LIBRARY_PATH})

