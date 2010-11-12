#toolchain paths
TOOLCHAIN_BASE=
TOOLCHAIN_PREFIX=
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
	-DLITTLE_ENDIAN_BYTE_ALIGNED \
	-DNET_EPOLL

SSL_BASE=/usr/local

