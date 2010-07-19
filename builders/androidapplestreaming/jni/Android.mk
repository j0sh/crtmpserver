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

PLATFORM_DEFINES := -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_SELECT -DANDROID -DHAS_PROTOCOL_HTTP

MODULES_DEFINES := -DHAS_PROTOCOL_RTMP \
    -DHAS_PROTOCOL_LIVEFLV \
    -DHAS_PROTOCOL_RTP \
    -DHAS_PROTOCOL_TS \
    -DHAS_PROTOCOL_VAR

GLOBAL_DEFINES := $(PLATFORM_DEFINES) $(MODULES_DEFINES)

INCLUDE_DIRECTORIES := $(PROJDIR)/sources/common/include $(PROJDIR)/sources/thelib/include $(PROJDIR)/sources/applications/applestreamingclient/include \
    $(PROJDIR)/sources/androidapplestreaming/include

NEEDED_LIBRARIES := -lssl -lcrypto -ldl

SOURCES := $(shell find $(PROJDIR)/sources/common -type f -name *.cpp)
SOURCES += $(shell find $(PROJDIR)/sources/thelib -type f -name *.cpp)
SOURCES += $(shell find $(PROJDIR)/sources/applications/applestreamingclient -type f -name *.cpp)

include $(CLEAR_VARS)
LOCAL_MODULE := crtmpserver_static
LOCAL_CPP_EXTENSION := cpp
LOCAL_C_INCLUDES += $(INCLUDE_DIRECTORIES) 
LOCAL_CFLAGS += $(GLOBAL_DEFINES) 
LOCAL_SRC_FILES := $(SOURCES) $(PROJDIR)/sources/androidapplestreaming/src/api.cpp 
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

