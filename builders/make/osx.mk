#toolchain paths
TOOLCHAIN_BASE=
TOOLCHAIN_PREFIX=
#output settings
STATIC_LIB_SUFIX=.a
STATIC_LIB_PREFIX=lib
DYNAMIC_LIB_SUFIX=.dylib
DYNAMIC_LIB_PREFIX=lib

#linking flags
dynamic_lib_flags = -headerpad_max_install_names -install_name $(DYNAMIC_LIB_PREFIX)$(1)$(DYNAMIC_LIB_SUFIX)
dynamic_exec_flags = -Wl,-search_paths_first -headerpad_max_install_names

#compile switches
PLATFORM_DEFINES = \
	-DOSX \
	-DLITTLE_ENDIAN_BYTE_ALIGNED \
	-DNET_KQUEUE

SSL_BASE=/usr/local

