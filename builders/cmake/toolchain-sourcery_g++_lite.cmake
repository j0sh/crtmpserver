#########################################################
# Copyright (C) 2012 Andriy Beregovenko <jet@jet.kiev.ua>
# 
# "Sourcery G++ Lite" toolchain configurator for CMake
#
# Read docs/toolchains.txt for more details
#########################################################

SET(CMAKE_SYSTEM_NAME Linux)

# Uncomment next line and set valid path to toolchain root
#SET(TOOLCHAIN_ROOT /path/to/toolchain)

# Path to where OpenSSL(compiled for target platform) is installed
#SET(TARGET_ENV_ROOT /home/jet/src/arm-env)

SET(TOOLCHAIN_ROOT ${TOOLCHAIN_ROOT} CACHE PATH "Path to toolchain")
SET(TARGET_ENV_ROOT ${TARGET_ENV_ROOT} CACHE PATH "Path to target environment")
SET(TOOLCHAIN_LIBRARY_PATH ${TOOLCHAIN_ROOT}/lib ${TOOLCHAIN_ROOT}/*/libc/lib ${TOOLCHAIN_ROOT}/*/libc/usr/lib)
SET(TOOLCHAIN_HEADER_PATH  ${TOOLCHAIN_ROOT}/include ${TOOLCHAIN_ROOT}/*/libc/include ${TOOLCHAIN_ROOT}/*/libc/usr/include ${TARGET_ENV_ROOT}/include ${TARGET_ENV_ROOT}/usr/include)


IF(NOT DEFINED TOOLCHAIN_ROOT)
  MESSAGE(FATAL_ERROR "Variable TOOLCHAIN_ROOT is not set.It must points to Toolchain's root directory.")
ENDIF(NOT DEFINED TOOLCHAIN_ROOT)

IF(NOT DEFINED TARGET_ENV_ROOT)
  MESSAGE(FATAL_ERROR "Variable TARGET_ENV_ROOT is not set.It must points to directory where OpenSSL for target platform is installed.")
ENDIF(NOT DEFINED TARGET_ENV_ROOT)

IF(NOT DEFINED TOOLCHAIN_PREFIX)
  SET(TOOLCHAIN_PREFIX arm-none-linux-gnueabi-)
ENDIF(NOT DEFINED TOOLCHAIN_PREFIX)

SET(TOOLCHAIN_BIN ${TOOLCHAIN_ROOT}/bin)

SET(CMAKE_C_COMPILER ${TOOLCHAIN_BIN}/${TOOLCHAIN_PREFIX}gcc)
SET(CMAKE_CXX_COMPILER ${TOOLCHAIN_BIN}/${TOOLCHAIN_PREFIX}g++)

SET(CMAKE_FIND_ROOT_PATH  ${TOOLCHAIN_ROOT} ${TARGET_ENV_ROOT})

SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


