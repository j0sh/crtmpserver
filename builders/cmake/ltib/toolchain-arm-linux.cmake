SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 1)

ADD_DEFINITIONS(-DLITTLE_ENDIAN_SHORT_ALIGNED)
SET(NO_ENDIANESS 1)	# tell main CMakeFiles.txt we already defined endianess
SET(NO_USE_DL 1)	# tell main CMakeFiles.txt we don't need DL
SET(PCH_CXX_FLAGS -c)	# PCH support on this platform requires the -c flag for compile
ADD_DEFINITIONS(-DLINUX)

SET(OPENSSL_LIBRARY_PATH "libssl.so")
SET(CRYPTO_LIBRARY_PATH "libcrypto.so")
#SET(LUA_LIBRARY_PATH "liblua5.1.so")

#Specify cross compiler
#Allow the ltib spoof to work, probably not necessary since cmake is apparently fooled by the ltib spoof
#SET(CMAKE_C_COMPILER gcc)
#SET(CMAKE_CXX_COMPILER g++)

#Specify the target environment
SET(CMAKE_FIND_ROOT_PATH $ENV{TOP}/rootfs)

#Never search for programs in the build target directories (they won't run)
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# Only search for libraries and headers in the build target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

SET(OPENSSL_FIND_REQUIRED false)

