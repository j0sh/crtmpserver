# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#LOCAL_PATH := $(call my-dir)
LOCAL_PATH :=
PROJDIR := $(PWD)/../..

REVISION= $(shell svnversion $(PROJDIR))
PLATFORM_DEFINES := -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_EPOLL -DANDROID -DHAS_SAFE_LOGGER

MODULES_DEFINES := 
MODULES_DEFINES += -DHAS_PROTOCOL_HTTP
MODULES_DEFINES += -DHAS_PROTOCOL_RTP
MODULES_DEFINES += -DHAS_PROTOCOL_TS
MODULES_DEFINES += -DHAS_PROTOCOL_VAR
#MODULES_DEFINES += -DHAS_PROTOCOL_RTMP
#MODULES_DEFINES += -DHAS_PROTOCOL_LIVEFLV
#MODULES_DEFINES += -DHAS_MEDIA_MP4
#MODULES_DEFINES += -DHAS_MEDIA_MP3
#MODULES_DEFINES += -DHAS_MEDIA_FLV


#RTSP related defines
#CreateRTCPPacket=CreateRTCPPacket_mystyle
CreateRTCPPacket=CreateRTCPPacket_mystyle_only_once
#CreateRTCPPacket=CreateRTCPPacket_live555style
#CreateRTCPPacket=CreateRTCPPacket_none
FeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one
#FeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_aggregate
HandleTSVideoData=HandleVideoData_version3
HAS_MS_TIMER=0

MODULES_DEFINES+=-DCreateRTCPPacket=$(CreateRTCPPacket)
MODULES_DEFINES+=-DFeedDataAudioMPEG4Generic=$(FeedDataAudioMPEG4Generic)
MODULES_DEFINES+=-DHandleTSVideoData=$(HandleTSVideoData)
ifeq ($(HAS_MS_TIMER), 1)
	MODULES_DEFINES+=-DHAS_MS_TIMER
endif

GLOBAL_DEFINES := $(PLATFORM_DEFINES) $(MODULES_DEFINES)
GLOBAL_DEFINES := $(GLOBAL_DEFINES) -DBUILD_SIGNATURE="\"$(REVISION),$(GLOBAL_DEFINES)\""

INCLUDE_DIRECTORIES := $(PROJDIR)/sources/common/include $(PROJDIR)/sources/thelib/include $(PROJDIR)/sources/applications/applestreamingclient/include \
    $(PROJDIR)/sources/androidapplestreaming/include

NEEDED_LIBRARIES := -llog -lssl -lcrypto -ldl

SOURCES := $(shell find $(PROJDIR)/sources/common -type f -name *.cpp)
SOURCES += $(shell find $(PROJDIR)/sources/thelib -type f -name *.cpp)
SOURCES += $(shell find $(PROJDIR)/sources/applications/applestreamingclient -type f -name *.cpp)

include $(CLEAR_VARS)
LOCAL_MODULE := crtmpserver_static
LOCAL_CPP_EXTENSION := cpp
LOCAL_C_INCLUDES += $(INCLUDE_DIRECTORIES) 
LOCAL_CFLAGS += $(GLOBAL_DEFINES) 
LOCAL_SRC_FILES := $(SOURCES) \
	$(PROJDIR)/sources/androidapplestreaming/src/api.cpp \
	$(PROJDIR)/sources/androidapplestreaming/src/variantconnection.cpp
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := crtmpserver_dynamic
LOCAL_CPP_EXTENSION := cpp
LOCAL_C_INCLUDES += $(INCLUDE_DIRECTORIES)
LOCAL_CFLAGS += $(GLOBAL_DEFINES)
LOCAL_LDLIBS := $(NEEDED_LIBRARIES)
LOCAL_STATIC_LIBRARIES := crtmpserver_static
LOCAL_SRC_FILES := $(PROJDIR)/sources/androidapplestreaming/src/jniwrapper.cpp
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := testapp
LOCAL_CPP_EXTENSION := cpp
LOCAL_C_INCLUDES += $(INCLUDE_DIRECTORIES)
LOCAL_CFLAGS += $(GLOBAL_DEFINES)
LOCAL_LDLIBS := $(NEEDED_LIBRARIES)
LOCAL_STATIC_LIBRARIES := crtmpserver_static
LOCAL_SRC_FILES += $(PROJDIR)/sources/androidapplestreaming/src/main.cpp
include $(BUILD_EXECUTABLE)

