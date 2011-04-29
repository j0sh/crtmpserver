#admin
ADMIN_INCLUDE=$(THELIB_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/applications/admin/include
ADMIN_LIBS=$(THELIB_LIBS) -L$(OUTPUT_DYNAMIC) -lthelib
ADMIN_SRCS=$(shell find $(PROJECT_BASE_PATH)/sources/applications/admin/src -type f -name "*.cpp")
ADMIN_OBJS=$(ADMIN_SRCS:.cpp=.admin.o)

admin: thelib $(ADMIN_OBJS)
	@mkdir -p $(OUTPUT_DYNAMIC)/applications/admin/mediaFolder
	@mkdir -p $(OUTPUT_STATIC)/applications/admin/mediaFolder
	$(CXXCOMPILER)  -shared $(ADMIN_LIBS) -o $(call dynamic_lib_name,admin,/applications/admin) $(call dynamic_lib_flags,admin) $(ADMIN_OBJS)

%.admin.o: %.cpp
	$(CXXCOMPILER) $(COMPILE_FLAGS) $(DEFINES) $(ADMIN_INCLUDE) -c $< -o $@

#applestreamingclient
APPLESTREAMINGCLIENT_INCLUDE=$(THELIB_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/applications/applestreamingclient/include
APPLESTREAMINGCLIENT_LIBS=$(THELIB_LIBS) -L$(OUTPUT_DYNAMIC) -lthelib
APPLESTREAMINGCLIENT_SRCS=$(shell find $(PROJECT_BASE_PATH)/sources/applications/applestreamingclient/src -type f -name "*.cpp")
APPLESTREAMINGCLIENT_OBJS=$(APPLESTREAMINGCLIENT_SRCS:.cpp=.applestreamingclient.o)

applestreamingclient: thelib $(APPLESTREAMINGCLIENT_OBJS)
	@mkdir -p $(OUTPUT_DYNAMIC)/applications/applestreamingclient/mediaFolder
	@mkdir -p $(OUTPUT_STATIC)/applications/applestreamingclient/mediaFolder
	$(CXXCOMPILER)  -shared $(APPLESTREAMINGCLIENT_LIBS) -o $(call dynamic_lib_name,applestreamingclient,/applications/applestreamingclient) $(call dynamic_lib_flags,applestreamingclient) $(APPLESTREAMINGCLIENT_OBJS)

%.applestreamingclient.o: %.cpp
	$(CXXCOMPILER) $(COMPILE_FLAGS) $(DEFINES) $(APPLESTREAMINGCLIENT_INCLUDE) -c $< -o $@

#appselector
APPSELECTOR_INCLUDE=$(THELIB_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/applications/appselector/include
APPSELECTOR_LIBS=$(THELIB_LIBS) -L$(OUTPUT_DYNAMIC) -lthelib
APPSELECTOR_SRCS=$(shell find $(PROJECT_BASE_PATH)/sources/applications/appselector/src -type f -name "*.cpp")
APPSELECTOR_OBJS=$(APPSELECTOR_SRCS:.cpp=.appselector.o)

appselector: thelib $(APPSELECTOR_OBJS)
	@mkdir -p $(OUTPUT_DYNAMIC)/applications/appselector/mediaFolder
	@mkdir -p $(OUTPUT_STATIC)/applications/appselector/mediaFolder
	$(CXXCOMPILER)  -shared $(APPSELECTOR_LIBS) -o $(call dynamic_lib_name,appselector,/applications/appselector) $(call dynamic_lib_flags,appselector) $(APPSELECTOR_OBJS)

%.appselector.o: %.cpp
	$(CXXCOMPILER) $(COMPILE_FLAGS) $(DEFINES) $(APPSELECTOR_INCLUDE) -c $< -o $@

#flvplayback
FLVPLAYBACK_INCLUDE=$(THELIB_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/applications/flvplayback/include
FLVPLAYBACK_LIBS=$(THELIB_LIBS) -L$(OUTPUT_DYNAMIC) -lthelib
FLVPLAYBACK_SRCS=$(shell find $(PROJECT_BASE_PATH)/sources/applications/flvplayback/src -type f -name "*.cpp")
FLVPLAYBACK_OBJS=$(FLVPLAYBACK_SRCS:.cpp=.flvplayback.o)

flvplayback: thelib $(FLVPLAYBACK_OBJS)
	@mkdir -p $(OUTPUT_DYNAMIC)/applications/flvplayback/mediaFolder
	@mkdir -p $(OUTPUT_STATIC)/applications/flvplayback/mediaFolder
	$(CXXCOMPILER)  -shared $(FLVPLAYBACK_LIBS) -o $(call dynamic_lib_name,flvplayback,/applications/flvplayback) $(call dynamic_lib_flags,flvplayback) $(FLVPLAYBACK_OBJS)

%.flvplayback.o: %.cpp
	$(CXXCOMPILER) $(COMPILE_FLAGS) $(DEFINES) $(FLVPLAYBACK_INCLUDE) -c $< -o $@

#proxypublish
PROXYPUBLISH_INCLUDE=$(THELIB_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/applications/proxypublish/include
PROXYPUBLISH_LIBS=$(THELIB_LIBS) -L$(OUTPUT_DYNAMIC) -lthelib
PROXYPUBLISH_SRCS=$(shell find $(PROJECT_BASE_PATH)/sources/applications/proxypublish/src -type f -name "*.cpp")
PROXYPUBLISH_OBJS=$(PROXYPUBLISH_SRCS:.cpp=.proxypublish.o)

proxypublish: thelib $(PROXYPUBLISH_OBJS)
	@mkdir -p $(OUTPUT_DYNAMIC)/applications/proxypublish/mediaFolder
	@mkdir -p $(OUTPUT_STATIC)/applications/proxypublish/mediaFolder
	$(CXXCOMPILER)  -shared $(PROXYPUBLISH_LIBS) -o $(call dynamic_lib_name,proxypublish,/applications/proxypublish) $(call dynamic_lib_flags,proxypublish) $(PROXYPUBLISH_OBJS)

%.proxypublish.o: %.cpp
	$(CXXCOMPILER) $(COMPILE_FLAGS) $(DEFINES) $(PROXYPUBLISH_INCLUDE) -c $< -o $@

#samplefactory
SAMPLEFACTORY_INCLUDE=$(THELIB_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/applications/samplefactory/include
SAMPLEFACTORY_LIBS=$(THELIB_LIBS) -L$(OUTPUT_DYNAMIC) -lthelib
SAMPLEFACTORY_SRCS=$(shell find $(PROJECT_BASE_PATH)/sources/applications/samplefactory/src -type f -name "*.cpp")
SAMPLEFACTORY_OBJS=$(SAMPLEFACTORY_SRCS:.cpp=.samplefactory.o)

samplefactory: thelib $(SAMPLEFACTORY_OBJS)
	@mkdir -p $(OUTPUT_DYNAMIC)/applications/samplefactory/mediaFolder
	@mkdir -p $(OUTPUT_STATIC)/applications/samplefactory/mediaFolder
	$(CXXCOMPILER)  -shared $(SAMPLEFACTORY_LIBS) -o $(call dynamic_lib_name,samplefactory,/applications/samplefactory) $(call dynamic_lib_flags,samplefactory) $(SAMPLEFACTORY_OBJS)

%.samplefactory.o: %.cpp
	$(CXXCOMPILER) $(COMPILE_FLAGS) $(DEFINES) $(SAMPLEFACTORY_INCLUDE) -c $< -o $@

#stresstest
STRESSTEST_INCLUDE=$(THELIB_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/applications/stresstest/include
STRESSTEST_LIBS=$(THELIB_LIBS) -L$(OUTPUT_DYNAMIC) -lthelib
STRESSTEST_SRCS=$(shell find $(PROJECT_BASE_PATH)/sources/applications/stresstest/src -type f -name "*.cpp")
STRESSTEST_OBJS=$(STRESSTEST_SRCS:.cpp=.stresstest.o)

stresstest: thelib $(STRESSTEST_OBJS)
	@mkdir -p $(OUTPUT_DYNAMIC)/applications/stresstest/mediaFolder
	@mkdir -p $(OUTPUT_STATIC)/applications/stresstest/mediaFolder
	$(CXXCOMPILER)  -shared $(STRESSTEST_LIBS) -o $(call dynamic_lib_name,stresstest,/applications/stresstest) $(call dynamic_lib_flags,stresstest) $(STRESSTEST_OBJS)

%.stresstest.o: %.cpp
	$(CXXCOMPILER) $(COMPILE_FLAGS) $(DEFINES) $(STRESSTEST_INCLUDE) -c $< -o $@

#vptests
VPTESTS_INCLUDE=$(THELIB_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/applications/vptests/include
VPTESTS_LIBS=$(THELIB_LIBS) -L$(OUTPUT_DYNAMIC) -lthelib
VPTESTS_SRCS=$(shell find $(PROJECT_BASE_PATH)/sources/applications/vptests/src -type f -name "*.cpp")
VPTESTS_OBJS=$(VPTESTS_SRCS:.cpp=.vptests.o)

vptests: thelib $(VPTESTS_OBJS)
	@mkdir -p $(OUTPUT_DYNAMIC)/applications/vptests/mediaFolder
	@mkdir -p $(OUTPUT_STATIC)/applications/vptests/mediaFolder
	$(CXXCOMPILER)  -shared $(VPTESTS_LIBS) -o $(call dynamic_lib_name,vptests,/applications/vptests) $(call dynamic_lib_flags,vptests) $(VPTESTS_OBJS)

%.vptests.o: %.cpp
	$(CXXCOMPILER) $(COMPILE_FLAGS) $(DEFINES) $(VPTESTS_INCLUDE) -c $< -o $@

ALL_APPS_OBJS= $(ADMIN_OBJS) $(APPLESTREAMINGCLIENT_OBJS) $(APPSELECTOR_OBJS) $(FLVPLAYBACK_OBJS) $(PROXYPUBLISH_OBJS) $(SAMPLEFACTORY_OBJS) $(STRESSTEST_OBJS) $(VPTESTS_OBJS)
ACTIVE_APPS= -DHAS_APP_ADMIN -DHAS_APP_APPLESTREAMINGCLIENT -DHAS_APP_APPSELECTOR -DHAS_APP_FLVPLAYBACK -DHAS_APP_PROXYPUBLISH -DHAS_APP_SAMPLEFACTORY -DHAS_APP_STRESSTEST -DHAS_APP_VPTESTS
applications: thelib admin applestreamingclient appselector flvplayback proxypublish samplefactory stresstest vptests
