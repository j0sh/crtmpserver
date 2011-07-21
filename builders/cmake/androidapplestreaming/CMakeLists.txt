cmake_minimum_required(VERSION 2.6)
PROJECT(androidapplestreaming)

#prepare the include directories
INCLUDE_DIRECTORIES(
		${CRTMPSERVER_SOURCES_ROOT}/common/include
		${CRTMPSERVER_SOURCES_ROOT}/thelib/include
		${CRTMPSERVER_SOURCES_ROOT}/applications/applestreamingclient/include
		${CRTMPSERVER_SOURCES_ROOT}/androidapplestreaming/include
        )

ADD_EXECUTABLE(androidapplestreaming
	${CRTMPSERVER_SOURCES_ROOT}/androidapplestreaming/src/api.cpp
	${CRTMPSERVER_SOURCES_ROOT}/androidapplestreaming/src/variantconnection.cpp
	${CRTMPSERVER_SOURCES_ROOT}/androidapplestreaming/src/main.cpp)
TARGET_LINK_LIBRARIES(
	${PROJECT_NAME}
	thelib 
	applestreamingclient 
	${OPENSSL_LIBRARY_PATH} 
	${CRYPTO_LIBRARY_PATH} 
	${Z_LIBRARY_PATH})
