cmake_minimum_required(VERSION 2.6)
PROJECT(crtmpserver)

#prepare the include directories
INCLUDE_DIRECTORIES(
	../../../3rdparty/lua-dev
	../../../sources/common/include
	../../../sources/thelib/include
)

IF(NOT DEFINED TEMP_FRAMEWORK_VER)
	EXECUTE_PROCESS(COMMAND svnversion -n ../../sources OUTPUT_VARIABLE TEMP_FRAMEWORK_VER)
	SET(TEMP_FRAMEWORK_VER "0.${TEMP_FRAMEWORK_VER}")
ENDIF(NOT DEFINED TEMP_FRAMEWORK_VER)
ADD_DEFINITIONS(-DRTMPSERVER_VERSION="${TEMP_FRAMEWORK_VER}")
ADD_EXECUTABLE(crtmpserver ../../../sources/crtmpserver/src/crtmpserver.cpp)
TARGET_LINK_LIBRARIES(crtmpserver thelib ${OPENSSL_LIBRARY_PATH} ${CRYPTO_LIBRARY_PATH})
INSTALL(TARGETS crtmpserver RUNTIME DESTINATION sbin)

