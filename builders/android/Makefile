#Copyright (c) 2009, Gavriloaie Eugen-Andrei (shiretu@gmail.com)
#All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#              *Redistributions of source code must retain the above copyright notice,
#               this list of conditions and the following disclaimer.
#              *Redistributions in binary form must reproduce the above copyright
#               notice, this list of conditions and the following disclaimer in the
#               documentation and/or other materials provided with the distribution.
#              *Neither the name of the DEVSS nor the names of its
#               contributors may be used to endorse or promote products derived from
#               this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.


#toolchain
ifdef ANDROID_NDK_ROOT
	TOOLCHAIN_BASE=$(ANDROID_NDK_ROOT)
else
all:
	@echo Need to define env variable ANDROID_NDK_ROOT, e.g. /Users/gz/projects/android/android-sdk-mac_86/android-ndk-r3-crystax
endif


ifdef ANDROID_NDK_VERSION
	TOOLCHAIN_ANDROID_VER=$(ANDROID_NDK_VERSION)
else
all:
	@echo Need to define env variable ANDROID_NDK_VERSION, e.g. android-8
endif

ifdef ANDROID_NDK_PLATFORM
	TOOLCHAIN_ANDROID_PLATFORM=$(ANDROID_NDK_PLATFORM)
else
all:
	@echo Need to define env variable ANDROID_NDK_PLATFORM, e.g. darwin-x86 or linux-x86
endif

TOOLCHAIN_INCLUDE=-I$(TOOLCHAIN_BASE)/build/platforms/$(TOOLCHAIN_ANDROID_VER)/arch-arm/usr/include
TOOLCHAIN_STDCPP_STATIC=$(TOOLCHAIN_BASE)/build/prebuilt/$(TOOLCHAIN_ANDROID_PLATFORM)/arm-eabi-4.4.0/arm-eabi/lib/libstdc++.a
TOOLCHAIN_SUPCPP_STATIC=$(TOOLCHAIN_BASE)/build/prebuilt/$(TOOLCHAIN_ANDROID_PLATFORM)/arm-eabi-4.4.0/arm-eabi/lib/libsupc++.a
TOOLCHAIN_LIBGCC_STATIC=$(TOOLCHAIN_BASE)/build/prebuilt/$(TOOLCHAIN_ANDROID_PLATFORM)/arm-eabi-4.4.0/lib/gcc/arm-eabi/4.4.0/libgcc.a
TOOLCHAIN_LIBC_DYNAMIC=$(TOOLCHAIN_BASE)/build/platforms/$(TOOLCHAIN_ANDROID_VER)/arch-arm/usr/lib/libc.so
TOOLCHAIN_LIBM_DYNAMIC=$(TOOLCHAIN_BASE)/build/platforms/$(TOOLCHAIN_ANDROID_VER)/arch-arm/usr/lib/libm.so
TOOLCHAIN_RPATH=-Wl,-rpath-link=$(TOOLCHAIN_BASE)/build/platforms/$(TOOLCHAIN_ANDROID_VER)/arch-arm/usr/lib
TOOLCHAIN_CRTBEGINEND=$(TOOLCHAIN_BASE)/build/platforms/$(TOOLCHAIN_ANDROID_VER)/arch-arm/usr/lib/crtbegin_dynamic.o $(TOOLCHAIN_BASE)/build/platforms/$(TOOLCHAIN_ANDROID_VER)/arch-arm/usr/lib/crtend_android.o
TOOLCHAIN_FLAGS=-mthumb-interwork \
	-ffunction-sections \
	-funwind-tables \
	-fstack-protector \
	-fno-short-enums \
	-D__ARM_ARCH_5__ \
	-D__ARM_ARCH_5T__ \
	-D__ARM_ARCH_5E__ \
	-D__ARM_ARCH_5TE__ \
	-march=armv5te \
	-mtune=xscale \
	-msoft-float \
	-mthumb \
	-fomit-frame-pointer \
	-fno-strict-aliasing \
	-finline-limit=64 \
	-DANDROID \
	-c #-O3 -g

TOOLCHAIN_SHARED_LINK_FLAGS=-nostdlib \
        -Wl,-shared,-Bsymbolic \
        -Wl,--whole-archive \
        -Wl,--no-whole-archive \
        -Wl,--no-undefined \
        $(TOOLCHAIN_RPATH) \
	-L$(TOOLCHAIN_BASE)/build/platforms/$(TOOLCHAIN_ANDROID_VER)/arch-arm/usr/lib \
	-L$(OUTPUT_DYNAMIC)
TOOLCHAIN_EXEC_LINK_FLAGS=-nostdlib \
	-Bdynamic \
	-Wl,-dynamic-linker,/system/bin/linker \
	-Wl,--gc-sections \
	-Wl,-z,nocopyreloc \
	-Wl,--no-undefined \
	$(TOOLCHAIN_RPATH) \
	-L$(TOOLCHAIN_BASE)/build/platforms/$(TOOLCHAIN_ANDROID_VER)/arch-arm/usr/lib \
	-L$(OUTPUT_DYNAMIC)


CC=$(TOOLCHAIN_BASE)/build/prebuilt/$(TOOLCHAIN_ANDROID_PLATFORM)/arm-eabi-4.4.0/bin/arm-eabi-gcc
CXX=$(TOOLCHAIN_BASE)/build/prebuilt/$(TOOLCHAIN_ANDROID_PLATFORM)/arm-eabi-4.4.0/bin/arm-eabi-g++
AR=$(TOOLCHAIN_BASE)/build/prebuilt/$(TOOLCHAIN_ANDROID_PLATFORM)/arm-eabi-4.4.0/bin/arm-eabi-ar
STRIP=$(TOOLCHAIN_BASE)/build/prebuilt/$(TOOLCHAIN_ANDROID_PLATFORM)/arm-eabi-4.4.0/bin/arm-eabi-strip

#project location settings
PROJECT_BASE=../..
SOURCES_BASE=$(PROJECT_BASE)/sources
OUTPUT_BASE=./output
OUTPUT_STATIC=$(OUTPUT_BASE)/static
OUTPUT_DYNAMIC=$(OUTPUT_BASE)/dynamic
TARGET_OUTPUT=/data/output/dynamic

#target system settings
STATIC_LIB_SUFIX=.a
STATIC_LIB_PREFIX=lib
DYNAMIC_LIB_SUFIX=.so
DYNAMIC_LIB_PREFIX=lib

#project build settings
PLATFORM_DEFINES=-DLITTLE_ENDIAN_BYTE_ALIGNED \
	-DNET_EPOLL
FEATURES_DEFINES=-DHAS_PROTOCOL_HTTP \
	-DHAS_PROTOCOL_RTMP \
	-DHAS_PROTOCOL_LIVEFLV \
	-DHAS_PROTOCOL_RTP \
	-DHAS_PROTOCOL_TS \
	-DHAS_PROTOCOL_VAR \
	-DHAS_LUA \
	-DCreateRTCPPacket=CreateRTCPPacket_mystyle_only_once \
	-DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one \
	-DHandleTSVideoData=HandleVideoData_version3

DEFINES=$(PLATFORM_DEFINES) $(FEATURES_DEFINES)

define BUILD_C_LIB_template
$$($1_TARGET): $$($1_OBJS)
	@echo Linking $$($1_OUTPUT_FILE)
	@mkdir -p $$($1_OUTPUT_DIR)
	@$$(CC) $$($1_LINK_FLAGS) $$($1_OBJS) $$(TOOLCHAIN_STDCPP_STATIC) $$(TOOLCHAIN_SUPCPP_STATIC) $$(TOOLCHAIN_LIBGCC_STATIC) $$(TOOLCHAIN_LIBC_DYNAMIC) $$(TOOLCHAIN_LIBM_DYNAMIC) -o $$($1_OUTPUT_FILE)
	@$$(STRIP) $$($1_OUTPUT_FILE)
%.$($1_TARGET).o: %.c
	@echo Compile $$<
	@$$(CC) $$(TOOLCHAIN_FLAGS) $$($1_DEFINES) $$(TOOLCHAIN_INCLUDE) $$($1_INCLUDE) -c $$< -o $$@   
endef

define BUILD_CPP_LIB_template
$$($1_TARGET): $$($1_OBJS)
	@echo Linking $$($1_OUTPUT_FILE)
	@mkdir -p $$($1_OUTPUT_DIR)
	@$$(CXX) $$($1_LINK_FLAGS) $$($1_OBJS) $$(TOOLCHAIN_STDCPP_STATIC) $$(TOOLCHAIN_SUPCPP_STATIC) $$(TOOLCHAIN_LIBGCC_STATIC) $$(TOOLCHAIN_LIBC_DYNAMIC) $$(TOOLCHAIN_LIBM_DYNAMIC) -o $$($1_OUTPUT_FILE)
	@$$(STRIP) $$($1_OUTPUT_FILE)
%.$($1_TARGET).o: %.cpp
	@echo Compile $$<
	@$$(CXX) $$(TOOLCHAIN_FLAGS) $$($1_DEFINES) $$(TOOLCHAIN_INCLUDE) $$($1_INCLUDE) -c $$< -o $$@
endef

define BUILD_CPP_EXEC_template
$$($1_TARGET): $$($1_OBJS)
	@echo Linking $$($1_OUTPUT_FILE)
	@mkdir -p $$($1_OUTPUT_DIR)
	@$$(CXX) $$($1_LINK_FLAGS) $$($1_OBJS) $$(TOOLCHAIN_CRTBEGINEND) $$(TOOLCHAIN_STDCPP_STATIC) $$(TOOLCHAIN_SUPCPP_STATIC) $$(TOOLCHAIN_LIBGCC_STATIC) $$(TOOLCHAIN_LIBC_DYNAMIC) $$(TOOLCHAIN_LIBM_DYNAMIC) -o $$($1_OUTPUT_FILE)
	@$$(STRIP) $$($1_OUTPUT_FILE)
%.$($1_TARGET).o: %.cpp
	@echo Compile $$<
	@$$(CXX) $$(TOOLCHAIN_FLAGS) $$($1_DEFINES) $$(TOOLCHAIN_INCLUDE) $$($1_INCLUDE) -c $$< -o $$@
endef


define APPLICATION_template
$1_TARGET=$2
$1_TARGET_STATIC=$$(STATIC_LIB_PREFIX)$$($1_TARGET)$$(STATIC_LIB_SUFIX)
$1_TARGET_DYNAMIC=$$(DYNAMIC_LIB_PREFIX)$$($1_TARGET)$$(DYNAMIC_LIB_SUFIX)
$1_SRCS = $$(shell find $$(SOURCES_BASE)/applications/$$($1_TARGET)/src -type f -name *.cpp)
$1_OBJS=$$($1_SRCS:.cpp=.$$($1_TARGET).o)
$1_DEFINES=$$(DEFINES)
$1_INCLUDE=$$(THELIB_INCLUDE) -I$$(SOURCES_BASE)/applications/$$($1_TARGET)/include
$1_LINK_FLAGS=$$(TOOLCHAIN_SHARED_LINK_FLAGS) -Wl,-soname,$$(TARGET_OUTPUT)/applications/$$($1_TARGET)/$$($1_TARGET_DYNAMIC) -llog -ldl -lcrypto -lssl -llua -lcommon -lthelib
$1_OUTPUT_DIR=$$(OUTPUT_DYNAMIC)/applications/$$($1_TARGET)
$1_OUTPUT_FILE=$$($1_OUTPUT_DIR)/$$($1_TARGET_DYNAMIC)
endef

#lua library
LUA_TARGET=lua
LUA_TARGET_STATIC=$(STATIC_LIB_PREFIX)$(LUA_TARGET)$(STATIC_LIB_SUFIX)
LUA_TARGET_DYNAMIC=$(DYNAMIC_LIB_PREFIX)$(LUA_TARGET)$(DYNAMIC_LIB_SUFIX)
LUA_SRCS=$(shell find $(PROJECT_BASE)/3rdparty/lua-dev -type f -name *.c)
LUA_OBJS=$(LUA_SRCS:.c=.lua.o)
LUA_DEFINES=$(DEFINES) -DLUA_USE_LINUX
LUA_INCLUDE=-I$(PROJECT_BASE)/3rdparty/lua-dev/
LUA_LINK_FLAGS=$(TOOLCHAIN_SHARED_LINK_FLAGS) -Wl,-soname,$(TARGET_OUTPUT)/$(LUA_TARGET_DYNAMIC) -ldl
LUA_OUTPUT_DIR=$(OUTPUT_DYNAMIC)
LUA_OUTPUT_FILE=$(LUA_OUTPUT_DIR)/$(LUA_TARGET_DYNAMIC)

#common library
COMMON_TARGET=common
COMMON_TARGET_STATIC=$(STATIC_LIB_PREFIX)$(COMMON_TARGET)$(STATIC_LIB_SUFIX)
COMMON_TARGET_DYNAMIC=$(DYNAMIC_LIB_PREFIX)$(COMMON_TARGET)$(DYNAMIC_LIB_SUFIX)
COMMON_SRCS = $(shell find $(SOURCES_BASE)/$(COMMON_TARGET)/src -type f -name *.cpp)
COMMON_OBJS=$(COMMON_SRCS:.cpp=.common.o)
COMMON_DEFINES=$(DEFINES)
COMMON_INCLUDE=$(LUA_INCLUDE) -I$(SOURCES_BASE)/$(COMMON_TARGET)/include
COMMON_LINK_FLAGS=$(TOOLCHAIN_SHARED_LINK_FLAGS) -Wl,-soname,$(TARGET_OUTPUT)/$(COMMON_TARGET_DYNAMIC) -llog -ldl -lcrypto -lssl -llua
COMMON_OUTPUT_DIR=$(OUTPUT_DYNAMIC)
COMMON_OUTPUT_FILE=$(COMMON_OUTPUT_DIR)/$(COMMON_TARGET_DYNAMIC)

#thelib library
THELIB_TARGET=thelib
THELIB_TARGET_STATIC=$(STATIC_LIB_PREFIX)$(THELIB_TARGET)$(STATIC_LIB_SUFIX)
THELIB_TARGET_DYNAMIC=$(DYNAMIC_LIB_PREFIX)$(THELIB_TARGET)$(DYNAMIC_LIB_SUFIX)
THELIB_SRCS = $(shell find $(SOURCES_BASE)/$(THELIB_TARGET)/src -type f -name *.cpp)
THELIB_OBJS=$(THELIB_SRCS:.cpp=.thelib.o)
THELIB_DEFINES=$(DEFINES)
THELIB_INCLUDE=$(COMMON_INCLUDE) -I$(SOURCES_BASE)/$(THELIB_TARGET)/include
THELIB_LINK_FLAGS=$(TOOLCHAIN_SHARED_LINK_FLAGS) -Wl,-soname,$(TARGET_OUTPUT)/$(THELIB_TARGET_DYNAMIC) -llog -ldl -lcrypto -lssl -llua -lcommon
THELIB_OUTPUT_DIR=$(OUTPUT_DYNAMIC)
THELIB_OUTPUT_FILE=$(THELIB_OUTPUT_DIR)/$(THELIB_TARGET_DYNAMIC)

#tests executable
TESTS_TARGET=tests
TESTS_TARGET_STATIC=$(TESTS_TARGET)
TESTS_TARGET_DYNAMIC=$(TESTS_TARGET)
TESTS_SRCS = $(shell find $(SOURCES_BASE)/$(TESTS_TARGET)/src -type f -name *.cpp)
TESTS_OBJS=$(TESTS_SRCS:.cpp=.tests.o)
TESTS_DEFINES=$(DEFINES)
TESTS_INCLUDE=$(THELIB_INCLUDE) -I$(SOURCES_BASE)/$(TESTS_TARGET)/include
TESTS_LINK_FLAGS=$(TOOLCHAIN_EXEC_LINK_FLAGS) -llog -ldl -lcrypto -lssl -llua -lcommon -lthelib
TESTS_OUTPUT_DIR=$(OUTPUT_DYNAMIC)
TESTS_OUTPUT_FILE=$(TESTS_OUTPUT_DIR)/$(TESTS_TARGET_DYNAMIC)

#rtmpserver executable
RTMPSERVER_TARGET=rtmpserver
RTMPSERVER_TARGET_STATIC=$(RTMPSERVER_TARGET)
RTMPSERVER_TARGET_DYNAMIC=$(RTMPSERVER_TARGET)
RTMPSERVER_SRCS = $(shell find $(SOURCES_BASE)/$(RTMPSERVER_TARGET)/src -type f -name *.cpp)
RTMPSERVER_OBJS=$(RTMPSERVER_SRCS:.cpp=.rtmpserver.o)
RTMPSERVER_DEFINES=$(DEFINES)
RTMPSERVER_INCLUDE=$(THELIB_INCLUDE) -I$(SOURCES_BASE)/$(RTMPSERVER_TARGET)/include
RTMPSERVER_LINK_FLAGS=$(TOOLCHAIN_EXEC_LINK_FLAGS) -llog -ldl -lcrypto -lssl -llua -lcommon -lthelib
RTMPSERVER_OUTPUT_DIR=$(OUTPUT_DYNAMIC)
RTMPSERVER_OUTPUT_FILE=$(RTMPSERVER_OUTPUT_DIR)/$(RTMPSERVER_TARGET_DYNAMIC)

#applications
$(eval $(call APPLICATION_template,APPSELECTOR,appselector))
$(eval $(call APPLICATION_template,FLVPLAYBACK,flvplayback))
$(eval $(call APPLICATION_template,PROXYPUBLISH,proxypublish))
$(eval $(call APPLICATION_template,SAMPLEFACTORY,samplefactory))
$(eval $(call APPLICATION_template,STRESSTEST,stresstest))
$(eval $(call APPLICATION_template,APPLESTREAMINGCLIENT,applestreamingclient))

#targets
$all: $(LUA_TARGET) \
	$(COMMON_TARGET) \
	$(THELIB_TARGET) \
	$(TESTS_TARGET) \
	$(RTMPSERVER_TARGET) \
	$(APPSELECTOR_TARGET) \
	$(FLVPLAYBACK_TARGET) \
	$(PROXYPUBLISH_TARGET) \
	$(SAMPLEFACTORY_TARGET) \
	$(STRESSTEST_TARGET) \
	$(APPLESTREAMINGCLIENT_TARGET) \
	pack
	@echo Done!!!


$(eval $(call BUILD_C_LIB_template,LUA))
$(eval $(call BUILD_CPP_LIB_template,COMMON))
$(eval $(call BUILD_CPP_LIB_template,THELIB))
$(eval $(call BUILD_CPP_EXEC_template,TESTS))
$(eval $(call BUILD_CPP_EXEC_template,RTMPSERVER))
$(eval $(call BUILD_CPP_LIB_template,APPSELECTOR))
$(eval $(call BUILD_CPP_LIB_template,FLVPLAYBACK))
$(eval $(call BUILD_CPP_LIB_template,PROXYPUBLISH))
$(eval $(call BUILD_CPP_LIB_template,SAMPLEFACTORY))
$(eval $(call BUILD_CPP_LIB_template,STRESSTEST))
$(eval $(call BUILD_CPP_LIB_template,APPLESTREAMINGCLIENT))

pack:
	@echo Packing...
	sh pack.sh $(OUTPUT_BASE) $(OUTPUT_STATIC) $(OUTPUT_DYNAMIC) 
#	cp ./rtmpserver.lua $(OUTPUT_DYNAMIC)
#	cp ./rtmpserver.lua $(OUTPUT_STATIC)
#	for i in `find output/dynamic/applications -type d -depth 1`;do mkdir $i/mediaFolder;done
#	tar -pcf ./rtmpd-android-.tar.gz $(OUTPUT_BASE)
#	tar -pcf ./rtmpd-android-dynamic.tar.gz $(OUTPUT_DYNAMIC)
#	tar -pcf ./rtmpd-android-static.tar.gz $(OUTPUT_STATIC)

ALLOBJS = $(LUA_OBJS) \
	$(COMMON_OBJS) \
	$(THELIB_OBJS) \
	$(TESTS_OBJS) \
	$(RTMPSERVER_OBJS) \
	$(APPSELECTOR_OBJS) \
	$(FLVPLAYBACK_OBJS) \
	$(PROXYPUBLISH_OBJS) \
	$(SAMPLEFACTORY_OBJS) \
	$(STRESSTEST_OBJS) \
	$(APPLESTREAMINGCLIENT_OBJS) \
	$(OUTPUT_BASE)

clean:
	@$(RM) -rf \
		$(ALLOBJS) $(ALLOBJS:.o=.d) \
		*.tar.gz
