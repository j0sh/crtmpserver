cmake_minimum_required(VERSION 2.6)
PROJECT(tinyxml)

#make the list with all the source files
FILE(GLOB_RECURSE SOURCES "${CRTMPSERVER_3RDPARTY_ROOT}/tinyxml/*.cpp" )

#prepare the include directories
INCLUDE_DIRECTORIES(
	${TINYXML_INCLUDE_PATH}
)

#create tinyxml library
ADD_LIBRARY(tinyxml SHARED ${SOURCES})
INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CRTMPSERVER_ROOT})


