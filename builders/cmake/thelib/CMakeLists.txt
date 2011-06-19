cmake_minimum_required(VERSION 2.6)
PROJECT(thelib)

#make the list with all the source files
FILE(GLOB_RECURSE SOURCES "${CRTMPSERVER_SOURCES_ROOT}/thelib/src/*.cpp" )

ADD_DEFINITIONS(-DCreateRTCPPacket=CreateRTCPPacket_mystyle_only_once)
ADD_DEFINITIONS(-DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one)
ADD_DEFINITIONS(-DHandleTSVideoData=HandleVideoData_version3)


#prepare the include directories
INCLUDE_DIRECTORIES(
	#/Users/shiretu/work/ffmpeg_bin/include
	${OPENSSL_INCLUDE_PATH}
	${LUA_INCLUDE_PATH}
	${CRTMPSERVER_SOURCES_ROOT}/common/include
	${CRTMPSERVER_SOURCES_ROOT}/thelib/include
	)

#create the library
ADD_LIBRARY(${PROJECT_NAME} ${LIB_TYPE} ${SOURCES})
ADD_PRECOMPILED_HEADER(${PROJECT_NAME} "${CRTMPSERVER_SOURCES_ROOT}/common/include/common.h")
IF(${LIB_TYPE} MATCHES "SHARED")
	TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
		common 
		${DL_LIBRARY_PATH} 
		${OPENSSL_LIBRARY_PATH} 
		${CRYPTO_LIBRARY_PATH} 
		#/opt/local/lib/libiconv.dylib
		#/Users/shiretu/work/ffmpeg_bin/lib/libavcodec.dylib
		#/Users/shiretu/work/ffmpeg_bin/lib/libavformat.dylib
		#/Users/shiretu/work/ffmpeg_bin/lib/libavutil.dylib
		)	
	INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CRTMPSERVER_ROOT})
ENDIF(${LIB_TYPE} MATCHES "SHARED")

