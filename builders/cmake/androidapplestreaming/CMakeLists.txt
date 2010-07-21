cmake_minimum_required(VERSION 2.6)
PROJECT(androidapplestreaming)

#prepare the include directories
INCLUDE_DIRECTORIES(
        ../../../sources/common/include
		../../../sources/thelib/include
		../../../sources/applications/applestreamingclient/include
		../../../sources/androidapplestreaming/include
        )

ADD_EXECUTABLE(androidapplestreaming
	../../../sources/androidapplestreaming/src/api.cpp
	../../../sources/androidapplestreaming/src/variantconnection.cpp
	../../../sources/androidapplestreaming/src/main.cpp)
TARGET_LINK_LIBRARIES(androidapplestreaming thelib applestreamingclient ${OPENSSL_LIBRARY_PATH} ${CRYPTO_LIBRARY_PATH})

