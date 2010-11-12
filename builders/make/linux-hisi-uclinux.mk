#toolchain paths
TOOLCHAIN_BASE=/home/shiretu/work/hisi/hisi-linux/x86-arm/gcc-3.4.3-uClibc-0.9.28
TOOLCHAIN_PREFIX=/usr/bin/arm-hismall-linux-
#output settings
STATIC_LIB_SUFIX=.a
STATIC_LIB_PREFIX=lib
DYNAMIC_LIB_SUFIX=.so
DYNAMIC_LIB_PREFIX=lib

#linking flags
dynamic_lib_flags = -Wl,-soname,$(DYNAMIC_LIB_PREFIX)$(1)$(DYNAMIC_LIB_SUFIX) -Wl,-rpath,"\$$ORIGIN"
dynamic_exec_flags = -Wl,-rpath,"\$$ORIGIN"

#compile switches
PLATFORM_DEFINES = \
	-DLINUX \
	-DLITTLE_ENDIAN_SHORT_ALIGNED \
	-DNET_EPOLL

SSL_BASE=/home/shiretu/work/hisi/openssl-dist

