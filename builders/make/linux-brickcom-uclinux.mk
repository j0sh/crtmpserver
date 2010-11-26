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
TOOLCHAIN_BASE=/opt/toolchain/arm-linux-uclibc/bin
TOOLCHAIN_PREFIX=/
#output settings
STATIC_LIB_SUFIX=.a
STATIC_LIB_PREFIX=lib
DYNAMIC_LIB_SUFIX=.so
DYNAMIC_LIB_PREFIX=lib

FPIC = -fPIC
OPTIMIZATIONS = -Os
COMPILE_FLAGS = $(FPIC) $(OPTIMIZATIONS)

#linking flags
dynamic_lib_flags = $(FPIC) $(OPTIMIZATIONS) -Wl,-soname,$(DYNAMIC_LIB_PREFIX)$(1)$(DYNAMIC_LIB_SUFIX) -Wl,-rpath,"\$$ORIGIN"
dynamic_exec_flags = $(FPIC) $(OPTIMIZATIONS) -Wl,-rpath,"\$$ORIGIN"

#compile switches
PLATFORM_DEFINES = \
	-DLINUX \
	-DLITTLE_ENDIAN_SHORT_ALIGNED \
	-DNET_EPOLL

SSL_BASE=/home/shiretu/work/openssl-dist-brickcom

