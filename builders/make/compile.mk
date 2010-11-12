#toolchain paths
CC=$(TOOLCHAIN_BASE)$(TOOLCHAIN_PREFIX)gcc
CXX=$(TOOLCHAIN_BASE)$(TOOLCHAIN_PREFIX)g++
AR=$(TOOLCHAIN_BASE)$(TOOLCHAIN_PREFIX)ar

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
COMMON_INCLUDE=$(LUA_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/common/include
COMMON_LIBS=$(SSL_LIB) -L$(OUTPUT_DYNAMIC) -llua
COMMON_SRCS = $(shell find $(PROJECT_BASE_PATH)/sources/common/src -type f -name "*.cpp")
COMMON_OBJS = $(COMMON_SRCS:.cpp=.common.o)

#thelib
THELIB_INCLUDE=$(COMMON_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/thelib/include
THELIB_LIBS=$(COMMON_LIBS) -L$(OUTPUT_DYNAMIC) -lcommon
THELIB_SRCS = $(shell find $(PROJECT_BASE_PATH)/sources/thelib/src -type f -name "*.cpp")
THELIB_OBJS = $(THELIB_SRCS:.cpp=.thelib.o)

#rtmpserver
RTMPSERVER_INCLUDE=$(THELIB_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/rtmpserver/include
RTMPSERVER_LIBS=$(THELIB_LIBS) -L$(OUTPUT_DYNAMIC) -lthelib
RTMPSERVER_SRCS=$(shell find $(PROJECT_BASE_PATH)/sources/rtmpserver/src -type f -name "*.cpp")
RTMPSERVER_OBJS=$(RTMPSERVER_SRCS:.cpp=.rtmpserver.o)

#tests
TESTS_INCLUDE=$(THELIB_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/tests/include
TESTS_LIBS=$(THELIB_LIBS) -L$(OUTPUT_DYNAMIC) -lthelib
TESTS_SRCS=$(shell find $(PROJECT_BASE_PATH)/sources/tests/src -type f -name "*.cpp")
TESTS_OBJS=$(TESTS_SRCS:.cpp=.tests.o)

#appselector
APPSELECTOR_INCLUDE=$(THELIB_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/applications/appselector/include
APPSELECTOR_LIBS=$(THELIB_LIBS) -L$(OUTPUT_DYNAMIC) -lthelib
APPSELECTOR_SRCS=$(shell find $(PROJECT_BASE_PATH)/sources/applications/appselector/src -type f -name "*.cpp")
APPSELECTOR_OBJS=$(APPSELECTOR_SRCS:.cpp=.appselector.o)

#targets
all: create_output_dirs lua common thelib rtmpserver tests applications
	@echo ----------- DONE!!!

create_output_dirs:
	@echo ----------- Creating output directory
	@mkdir -p $(OUTPUT_STATIC)
	@mkdir -p $(OUTPUT_DYNAMIC)/applications/appselector/mediaFolder

lua: create_output_dirs $(LUA_OBJS)
	@echo ----------- linking shared lua
	$(CC) -fPIC -shared -o $(call dynamic_lib_name,lua,) $(call dynamic_lib_flags,lua) $(LUA_OBJS)

%.lua.o: %.c
	$(CC) -c $< -o $@

common: lua $(COMMON_OBJS)
	@echo ----------- linking shared common
	$(CXX) -fPIC -shared $(COMMON_LIBS) -o $(call dynamic_lib_name,common,) $(call dynamic_lib_flags,common) $(COMMON_OBJS)

%.common.o: %.cpp
	$(CXX) -fPIC $(DEFINES) $(COMMON_INCLUDE) -c $< -o $@

thelib: common $(THELIB_OBJS)
	@echo ----------- linking shared thelib
	$(CXX) -fPIC -shared $(THELIB_LIBS) -o $(call dynamic_lib_name,thelib,) $(call dynamic_lib_flags,thelib) $(THELIB_OBJS)

%.thelib.o: %.cpp
	$(CXX) -fPIC $(DEFINES) $(THELIB_INCLUDE) -c $< -o $@

rtmpserver: thelib $(RTMPSERVER_OBJS)
	@echo ----------- linking rtmpserver
	$(CXX) -fPIC $(RTMPSERVER_LIBS) -o $(call dynamic_exec_name,rtmpserver,) $(call dynamic_exec_flags,rtmpserver) $(RTMPSERVER_OBJS)

%.rtmpserver.o: %.cpp
	$(CXX) -fPIC $(DEFINES) $(RTMPSERVER_INCLUDE) -c $< -o $@

tests: thelib $(TESTS_OBJS)
	@echo ----------- linking tests
	$(CXX) -fPIC $(TESTS_LIBS) -o $(call dynamic_exec_name,tests,) $(call dynamic_exec_flags,tests) $(TESTS_OBJS)

%.tests.o: %.cpp
	$(CXX) -fPIC $(DEFINES) $(TESTS_INCLUDE) -c $< -o $@

applications: appselector

appselector: thelib $(APPSELECTOR_OBJS)
	@echo ----------- linking application appselector
	$(CXX) -fPIC -shared $(APPSELECTOR_LIBS) -o $(call dynamic_lib_name,appselector,/applications/appselector) $(call dynamic_lib_flags,appselector) $(APPSELECTOR_OBJS)

%.appselector.o: %.cpp
	$(CXX) -fPIC $(DEFINES) $(APPSELECTOR_INCLUDE) -c $< -o $@

clean:
	@rm -rfv $(OUTPUT_BASE)
	@rm -rfv $(LUA_OBJS)
	@rm -rfv $(COMMON_OBJS)
	@rm -rfv $(THELIB_OBJS)
	@rm -rfv $(RTMPSERVER_OBJS)
	@rm -rfv $(TESTS_OBJS)
	@rm -rfv $(APPSELECTOR_OBJS)

