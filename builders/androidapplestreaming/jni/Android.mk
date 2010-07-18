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

include $(CLEAR_VARS)
LOCAL_CPP_EXTENSION := cpp
LOCAL_MODULE := crtmpserver_static
LOCAL_C_INCLUDES += $(PROJDIR)/sources/common/include $(PROJDIR)/sources/thelib/include $(PROJDIR)/sources/applications/applestreamingclient/include \
	$(PROJDIR)/sources/androidapplestreaming/include
LOCAL_CFLAGS += -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_SELECT -DANDROID -DHAS_PROTOCOL_HTTP \
	-DHAS_PROTOCOL_RTMP \
	-DHAS_PROTOCOL_LIVEFLV \
	-DHAS_PROTOCOL_RTP \
	-DHAS_PROTOCOL_TS \
	-DHAS_PROTOCOL_VAR
LOCAL_LDLIBS := -lssl -lcrypto -ldl
LOCAL_SRC_FILES := $(shell find $(PROJDIR)/sources/common -type f -name *.cpp)
LOCAL_SRC_FILES += $(shell find $(PROJDIR)/sources/thelib -type f -name *.cpp)
LOCAL_SRC_FILES += $(shell find $(PROJDIR)/sources/applications/applestreamingclient -type f -name *.cpp)
LOCAL_SRC_FILES += $(PROJDIR)/sources/androidapplestreaming/src/api.cpp $(PROJDIR)/sources/androidapplestreaming/src/jniwrapper.cpp
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := crtmpserver_dynamic
LOCAL_STATIC_LIBRARIES := crtmpserver_static
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CPP_EXTENSION := cpp
LOCAL_MODULE := testapp
LOCAL_C_INCLUDES += $(PROJDIR)/sources/common/include $(PROJDIR)/sources/thelib/include $(PROJDIR)/sources/applications/applestreamingclient/include \
    $(PROJDIR)/sources/androidapplestreaming/include
LOCAL_CFLAGS += -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_SELECT -DANDROID -DHAS_PROTOCOL_HTTP \
    -DHAS_PROTOCOL_RTMP \
    -DHAS_PROTOCOL_LIVEFLV \
    -DHAS_PROTOCOL_RTP \
    -DHAS_PROTOCOL_TS \
    -DHAS_PROTOCOL_VAR
LOCAL_LDLIBS := -lssl -lcrypto -ldl
LOCAL_SRC_FILES += $(PROJDIR)/sources/androidapplestreaming/src/main.cpp
LOCAL_STATIC_LIBRARIES := crtmpserver_static
include $(BUILD_EXECUTABLE)

