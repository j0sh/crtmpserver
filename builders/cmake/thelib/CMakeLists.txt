cmake_minimum_required(VERSION 2.6)
PROJECT(thelib)

#make the list with all the source files
FILE(GLOB_RECURSE SOURCES "${CRTMPSERVER_SOURCES_ROOT}/thelib/src/*.cpp" )

ADD_DEFINITIONS(-DCreateRTCPPacket=CreateRTCPPacket_mystyle_only_once)
ADD_DEFINITIONS(-DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one)
ADD_DEFINITIONS(-DHandleTSVideoData=HandleVideoData_version3)


#prepare the include directories
INCLUDE_DIRECTORIES(
	${OPENSSL_INCLUDE_PATH}
	${LUA_INCLUDE_PATH}
	${CRTMPSERVER_SOURCES_ROOT}/common/include
	${CRTMPSERVER_SOURCES_ROOT}/thelib/include
	)

#create THE library
ADD_LIBRARY(thelib SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(thelib common ${DL_LIBRARY_PATH} ${OPENSSL_LIBRARY_PATH} ${CRYPTO_LIBRARY_PATH})
ADD_PRECOMPILED_HEADER(${PROJECT_NAME} "${CRTMPSERVER_SOURCES_ROOT}/common/include/common.h")
INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CRTMPSERVER_ROOT})

