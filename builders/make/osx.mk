#  Copyright (c) 2010,
#  Gavriloaie Eugen-Andrei (shiretu@gmail.com)
#
#  This file is part of crtmpserver.
#  crtmpserver is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  crtmpserver is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with crtmpserver.  If not, see <http://www.gnu.org/licenses/>.

#toolchain paths
TOOLCHAIN_BASE=
TOOLCHAIN_PREFIX=
CCOMPILER=$(CC)
CXXCOMPILER=$(CXX)
#output settings
STATIC_LIB_SUFIX=.a
STATIC_LIB_PREFIX=lib
DYNAMIC_LIB_SUFIX=.dylib
DYNAMIC_LIB_PREFIX=lib

FPIC = -fPIC
OPTIMIZATIONS = -O3
COMPILE_FLAGS = $(FPIC) $(OPTIMIZATIONS) $(CFLAGS)

#linking flags
dynamic_lib_flags = $(FPIC) $(OPTIMIZATIONS) -headerpad_max_install_names -install_name $(DYNAMIC_LIB_PREFIX)$(1)$(DYNAMIC_LIB_SUFIX)
dynamic_exec_flags = $(FPIC) $(OPTIMIZATIONS) -Wl,-search_paths_first -headerpad_max_install_names

#compile switches
PLATFORM_DEFINES = \
	-DOSX \
	-DLITTLE_ENDIAN_BYTE_ALIGNED \
	-DNET_KQUEUE

SSL_BASE=/usr/local

