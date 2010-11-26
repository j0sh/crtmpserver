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
CC=$(TOOLCHAIN_BASE)$(TOOLCHAIN_PREFIX)gcc
CXX=$(TOOLCHAIN_BASE)$(TOOLCHAIN_PREFIX)g++
AR=$(TOOLCHAIN_BASE)$(TOOLCHAIN_PREFIX)ar
STRIP=$(TOOLCHAIN_BASE)$(TOOLCHAIN_PREFIX)strip

#output settings
OUTPUT_BASE = ./output
OUTPUT_BIN = $(OUTPUT_BASE)
OUTPUT_OBJ = $(OUTPUT_BASE)
OUTPUT_DYNAMIC = $(OUTPUT_BIN)/dynamic
OUTPUT_STATIC = $(OUTPUT_BIN)/static
dynamic_lib_name = $(OUTPUT_DYNAMIC)$(2)/$(DYNAMIC_LIB_PREFIX)$(1)$(DYNAMIC_LIB_SUFIX)
static_lib_name = $(OUTPUT_STATIC)$(2)/$(STATIC_LIB_PREFIX)$(1)$(STATIC_LIB_SUFIX)
dynamic_exec_name = $(OUTPUT_DYNAMIC)$(2)/$(1)
static_exec_name = $(OUTPUT_STATIC)$(2)/$(1)

#project paths
PROJECT_BASE_PATH=../..

FEATURES_DEFINES = \
	-DCreateRTCPPacket=CreateRTCPPacket_mystyle_only_once \
	-DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one \
	-DHandleTSVideoData=HandleVideoData_version3 \
	-DHAS_PROTOCOL_HTTP \
	-DHAS_PROTOCOL_RTMP \
	-DHAS_PROTOCOL_LIVEFLV \
	-DHAS_PROTOCOL_RTP \
	-DHAS_PROTOCOL_TS \
	-DHAS_PROTOCOL_VAR \
	-DHAS_LUA \
	-DHAS_MEDIA_MP3 \
	-DHAS_MEDIA_MP4 \
	-DHAS_MEDIA_FLV \
	-DHAS_MEDIA_MKV

DEFINES = $(PLATFORM_DEFINES) $(FEATURES_DEFINES)

#library paths
SSL_INCLUDE=-I$(SSL_BASE)/include
SSL_LIB=-L$(SSL_BASE)/lib -lssl -lcrypto


#lua
LUA_INCLUDE=-I$(PROJECT_BASE_PATH)/3rdparty/lua-dev
LUA_SRCS = $(shell find $(PROJECT_BASE_PATH)/3rdparty/lua-dev -type f -name "*.c")
LUA_OBJS = $(LUA_SRCS:.c=.lua.o)

#common
COMMON_INCLUDE=$(LUA_INCLUDE) $(SSL_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/common/include
COMMON_LIBS=$(SSL_LIB) -L$(OUTPUT_DYNAMIC) -llua
COMMON_SRCS = $(shell find $(PROJECT_BASE_PATH)/sources/common/src -type f -name "*.cpp")
COMMON_OBJS = $(COMMON_SRCS:.cpp=.common.o)

#thelib
THELIB_INCLUDE=$(COMMON_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/thelib/include
THELIB_LIBS=$(COMMON_LIBS) -L$(OUTPUT_DYNAMIC) -lcommon
THELIB_SRCS = $(shell find $(PROJECT_BASE_PATH)/sources/thelib/src -type f -name "*.cpp")
THELIB_OBJS = $(THELIB_SRCS:.cpp=.thelib.o)

#tests
TESTS_INCLUDE=$(THELIB_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/tests/include
TESTS_LIBS=$(THELIB_LIBS) -L$(OUTPUT_DYNAMIC) -lthelib
TESTS_SRCS=$(shell find $(PROJECT_BASE_PATH)/sources/tests/src -type f -name "*.cpp")
TESTS_OBJS=$(TESTS_SRCS:.cpp=.tests.o)

#rtmpserver
RTMPSERVER_INCLUDE=$(THELIB_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/rtmpserver/include
RTMPSERVER_LIBS=$(THELIB_LIBS) -L$(OUTPUT_DYNAMIC) -lthelib
RTMPSERVER_SRCS=$(shell find $(PROJECT_BASE_PATH)/sources/rtmpserver/src -type f -name "*.cpp")
RTMPSERVER_OBJS_DYNAMIC=$(RTMPSERVER_SRCS:.cpp=.rtmpserver_dynamic.o)
RTMPSERVER_OBJS_STATIC=$(RTMPSERVER_SRCS:.cpp=.rtmpserver_static.o)

#targets
all: create_output_dirs lua common thelib applications tests rtmpserver
	@echo ----------- DONE!!!

include apps.mk

create_output_dirs:
	@echo ----------- Creating output directory
	@mkdir -p $(OUTPUT_STATIC)
	@mkdir -p $(OUTPUT_DYNAMIC)
	@echo -----------

lua: create_output_dirs $(LUA_OBJS)
	@echo ----------- linking shared lua
	$(CC) -shared -o $(call dynamic_lib_name,lua,) $(call dynamic_lib_flags,lua) $(LUA_OBJS)
	@echo -----------

%.lua.o: %.c
	$(CC) $(COMPILE_FLAGS) -c $< -o $@
	
common: lua $(COMMON_OBJS)
	@echo ----------- linking shared common
	$(CXX) -shared $(COMMON_LIBS) -o $(call dynamic_lib_name,common,) $(call dynamic_lib_flags,common) $(COMMON_OBJS)
	@echo -----------

%.common.o: %.cpp
	$(CXX) $(COMPILE_FLAGS) $(DEFINES) $(COMMON_INCLUDE) -c $< -o $@

thelib: common $(THELIB_OBJS)
	@echo ----------- linking shared thelib
	$(CXX) -shared $(THELIB_LIBS) -o $(call dynamic_lib_name,thelib,) $(call dynamic_lib_flags,thelib) $(THELIB_OBJS)
	@echo -----------

%.thelib.o: %.cpp
	$(CXX) $(COMPILE_FLAGS) $(DEFINES) $(THELIB_INCLUDE) -c $< -o $@

tests: thelib $(TESTS_OBJS)
	@echo ----------- linking dynamic tests
	$(CXX)  $(TESTS_LIBS) -o $(call dynamic_exec_name,tests,) $(call dynamic_exec_flags,tests) $(TESTS_OBJS)
	@echo ----------- linking static tests
	$(CXX)  $(SSL_LIB) -o $(call static_exec_name,tests) $(call static_exec_flags,tests) \
		$(TESTS_OBJS) \
		$(LUA_OBJS) \
		$(COMMON_OBJS) \
		$(THELIB_OBJS)
	@echo -----------

%.tests.o: %.cpp
	$(CXX) $(COMPILE_FLAGS) $(DEFINES) $(TESTS_INCLUDE) -c $< -o $@

rtmpserver: applications $(RTMPSERVER_OBJS_DYNAMIC) $(RTMPSERVER_OBJS_STATIC)
	@echo ----------- linking dynamic rtmpserver
	$(CXX) $(RTMPSERVER_LIBS) -o $(call dynamic_exec_name,rtmpserver,) $(call dynamic_exec_flags,rtmpserver) $(RTMPSERVER_OBJS_DYNAMIC)
	@echo ----------- linking static rtmpserver
	$(CXX) $(SSL_LIB) -o $(call static_exec_name,rtmpserver,) $(call static_exec_flags,rtmpserver) \
		$(RTMPSERVER_OBJS_STATIC) \
		$(LUA_OBJS) \
		$(COMMON_OBJS) \
		$(THELIB_OBJS) \
		$(ALL_APPS_OBJS)
	@cp $(PROJECT_BASE_PATH)/builders/cmake/rtmpserver/rtmpserver.lua $(OUTPUT_DYNAMIC)
	@cp $(PROJECT_BASE_PATH)/builders/cmake/rtmpserver/rtmpserver.lua $(OUTPUT_STATIC)
	@echo -----------

%.rtmpserver_dynamic.o: %.cpp
	$(CXX) $(COMPILE_FLAGS) $(DEFINES) $(RTMPSERVER_INCLUDE) -c $< -o $@

%.rtmpserver_static.o: %.cpp
	$(CXX) $(COMPILE_FLAGS) -DCOMPILE_STATIC $(ACTIVE_APPS) $(DEFINES) $(RTMPSERVER_INCLUDE) -c $< -o $@

clean:
	@rm -rfv $(OUTPUT_BASE)
	@sh cleanupobjs.sh
	
