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
CCOMPILER?=$(TOOLCHAIN_BASE)$(TOOLCHAIN_PREFIX)gcc
CXXCOMPILER?=$(TOOLCHAIN_BASE)$(TOOLCHAIN_PREFIX)g++

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
	-DHAS_MEDIA_FLV


DEFINES = $(PLATFORM_DEFINES) $(FEATURES_DEFINES)

#library paths
SSL_INCLUDE=-I$(SSL_BASE)/include
SSL_LIB=-L$(SSL_BASE)/lib -lssl -lcrypto

#lua
LUA_INCLUDE=-I$(PROJECT_BASE_PATH)/3rdparty/lua-dev
LUA_SRCS = $(shell find $(PROJECT_BASE_PATH)/3rdparty/lua-dev -type f -name "*.c")
LUA_OBJS = $(LUA_SRCS:.c=.lua.o)

#tinyxml
TINYXML_INCLUDE=-I$(PROJECT_BASE_PATH)/3rdparty/tinyxml
TINYXML_SRCS = $(shell find $(PROJECT_BASE_PATH)/3rdparty/tinyxml -type f -name "*.cpp")
TINYXML_OBJS = $(TINYXML_SRCS:.cpp=.tinyxml.o)

#common
COMMON_INCLUDE=$(LUA_INCLUDE) $(TINYXML_INCLUDE) $(SSL_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/common/include
COMMON_LIBS=$(SSL_LIB) -L$(OUTPUT_DYNAMIC) -llua -ltinyxml
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

#crtmpserver
CRTMPSERVER_INCLUDE=$(THELIB_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/crtmpserver/include
CRTMPSERVER_LIBS=$(THELIB_LIBS) -L$(OUTPUT_DYNAMIC) -lthelib
CRTMPSERVER_SRCS=$(shell find $(PROJECT_BASE_PATH)/sources/crtmpserver/src -type f -name "*.cpp")
CRTMPSERVER_OBJS_DYNAMIC=$(CRTMPSERVER_SRCS:.cpp=.crtmpserver_dynamic.o)
CRTMPSERVER_OBJS_STATIC=$(CRTMPSERVER_SRCS:.cpp=.crtmpserver_static.o)

#targets
all: create_output_dirs lua common thelib applications tests crtmpserver

include apps.mk

create_output_dirs:
	@mkdir -p $(OUTPUT_STATIC)
	@mkdir -p $(OUTPUT_DYNAMIC)

lua: create_output_dirs $(LUA_OBJS)
	$(CCOMPILER) -shared -o $(call dynamic_lib_name,lua,) $(call dynamic_lib_flags,lua) $(LUA_OBJS)

%.lua.o: %.c
	$(CCOMPILER) $(COMPILE_FLAGS) -c $< -o $@

tinyxml: lua $(TINYXML_OBJS)
	$(CXXCOMPILER) -shared -o $(call dynamic_lib_name,tinyxml,) $(call dynamic_lib_flags,tinyxml) $(TINYXML_OBJS)

%.tinyxml.o: %.cpp
	$(CXXCOMPILER) $(COMPILE_FLAGS) $(DEFINES) $(COMMON_INCLUDE) -c $< -o $@

common: tinyxml $(COMMON_OBJS)
	$(CXXCOMPILER) -shared $(COMMON_LIBS) -o $(call dynamic_lib_name,common,) $(call dynamic_lib_flags,common) $(COMMON_OBJS)

%.common.o: %.cpp
	$(CXXCOMPILER) $(COMPILE_FLAGS) $(DEFINES) $(COMMON_INCLUDE) -c $< -o $@

thelib: common $(THELIB_OBJS)
	$(CXXCOMPILER) -shared $(THELIB_LIBS) -o $(call dynamic_lib_name,thelib,) $(call dynamic_lib_flags,thelib) $(THELIB_OBJS)

%.thelib.o: %.cpp
	$(CXXCOMPILER) $(COMPILE_FLAGS) $(DEFINES) $(THELIB_INCLUDE) -c $< -o $@

tests: thelib $(TESTS_OBJS)
	$(CXXCOMPILER)  $(TESTS_LIBS) -o $(call dynamic_exec_name,tests,) $(call dynamic_exec_flags,tests) $(TESTS_OBJS)
	$(CXXCOMPILER)  $(SSL_LIB) -o $(call static_exec_name,tests) $(call static_exec_flags,tests) \
		$(TESTS_OBJS) \
		$(LUA_OBJS) \
		$(TINYXML_OBJS) \
		$(COMMON_OBJS) \
		$(THELIB_OBJS)

%.tests.o: %.cpp
	$(CXXCOMPILER) $(COMPILE_FLAGS) $(DEFINES) $(TESTS_INCLUDE) -c $< -o $@

crtmpserver: applications $(CRTMPSERVER_OBJS_DYNAMIC) $(CRTMPSERVER_OBJS_STATIC)
	$(CXXCOMPILER) $(CRTMPSERVER_LIBS) -o $(call dynamic_exec_name,crtmpserver,) $(call dynamic_exec_flags,crtmpserver) $(CRTMPSERVER_OBJS_DYNAMIC)
	$(CXXCOMPILER) $(SSL_LIB) -o $(call static_exec_name,crtmpserver,) $(call static_exec_flags,crtmpserver) \
		$(CRTMPSERVER_OBJS_STATIC) \
		$(LUA_OBJS) \
		$(TINYXML_OBJS) \
		$(COMMON_OBJS) \
		$(THELIB_OBJS) \
		$(ALL_APPS_OBJS)
	@cp $(PROJECT_BASE_PATH)/builders/cmake/crtmpserver/crtmpserver.lua $(OUTPUT_DYNAMIC)
	@cp $(PROJECT_BASE_PATH)/builders/cmake/crtmpserver/crtmpserver.lua $(OUTPUT_STATIC)

%.crtmpserver_dynamic.o: %.cpp
	$(CXXCOMPILER) $(COMPILE_FLAGS) $(DEFINES) $(CRTMPSERVER_INCLUDE) -c $< -o $@

%.crtmpserver_static.o: %.cpp
	$(CXXCOMPILER) $(COMPILE_FLAGS) -DCOMPILE_STATIC $(ACTIVE_APPS) $(DEFINES) $(CRTMPSERVER_INCLUDE) -c $< -o $@

clean:
	@rm -rfv $(OUTPUT_BASE)
	@sh cleanupobjs.sh
	
