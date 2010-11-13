#admin
ADMIN_INCLUDE=$(THELIB_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/applications/admin/include
ADMIN_LIBS=$(THELIB_LIBS) -L$(OUTPUT_DYNAMIC) -lthelib
ADMIN_SRCS=$(shell find $(PROJECT_BASE_PATH)/sources/applications/admin/src -type f -name "*.cpp")
ADMIN_OBJS=$(ADMIN_SRCS:.cpp=.admin.o)

admin: thelib $(ADMIN_OBJS)
	@echo ----------- linking application admin
	@mkdir -p $(OUTPUT_DYNAMIC)/applications/admin/mediaFolder
	@mkdir -p $(OUTPUT_STATIC)/applications/admin/mediaFolder
	$(CXX) -fPIC -shared $(ADMIN_LIBS) -o $(call dynamic_lib_name,admin,/applications/admin) $(call dynamic_lib_flags,admin) $(ADMIN_OBJS)
	$(STRIP) -sx $(call dynamic_lib_name,admin,/applications/admin)
	@echo -----------

%.admin.o: %.cpp
	$(CXX) -fPIC $(DEFINES) $(ADMIN_INCLUDE) -c $< -o $@

#applestreamingclient
APPLESTREAMINGCLIENT_INCLUDE=$(THELIB_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/applications/applestreamingclient/include
APPLESTREAMINGCLIENT_LIBS=$(THELIB_LIBS) -L$(OUTPUT_DYNAMIC) -lthelib
APPLESTREAMINGCLIENT_SRCS=$(shell find $(PROJECT_BASE_PATH)/sources/applications/applestreamingclient/src -type f -name "*.cpp")
APPLESTREAMINGCLIENT_OBJS=$(APPLESTREAMINGCLIENT_SRCS:.cpp=.applestreamingclient.o)

applestreamingclient: thelib $(APPLESTREAMINGCLIENT_OBJS)
	@echo ----------- linking application applestreamingclient
	@mkdir -p $(OUTPUT_DYNAMIC)/applications/applestreamingclient/mediaFolder
	@mkdir -p $(OUTPUT_STATIC)/applications/applestreamingclient/mediaFolder
	$(CXX) -fPIC -shared $(APPLESTREAMINGCLIENT_LIBS) -o $(call dynamic_lib_name,applestreamingclient,/applications/applestreamingclient) $(call dynamic_lib_flags,applestreamingclient) $(APPLESTREAMINGCLIENT_OBJS)
	$(STRIP) -sx $(call dynamic_lib_name,applestreamingclient,/applications/applestreamingclient)
	@echo -----------

%.applestreamingclient.o: %.cpp
	$(CXX) -fPIC $(DEFINES) $(APPLESTREAMINGCLIENT_INCLUDE) -c $< -o $@

#appselector
APPSELECTOR_INCLUDE=$(THELIB_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/applications/appselector/include
APPSELECTOR_LIBS=$(THELIB_LIBS) -L$(OUTPUT_DYNAMIC) -lthelib
APPSELECTOR_SRCS=$(shell find $(PROJECT_BASE_PATH)/sources/applications/appselector/src -type f -name "*.cpp")
APPSELECTOR_OBJS=$(APPSELECTOR_SRCS:.cpp=.appselector.o)

appselector: thelib $(APPSELECTOR_OBJS)
	@echo ----------- linking application appselector
	@mkdir -p $(OUTPUT_DYNAMIC)/applications/appselector/mediaFolder
	@mkdir -p $(OUTPUT_STATIC)/applications/appselector/mediaFolder
	$(CXX) -fPIC -shared $(APPSELECTOR_LIBS) -o $(call dynamic_lib_name,appselector,/applications/appselector) $(call dynamic_lib_flags,appselector) $(APPSELECTOR_OBJS)
	$(STRIP) -sx $(call dynamic_lib_name,appselector,/applications/appselector)
	@echo -----------

%.appselector.o: %.cpp
	$(CXX) -fPIC $(DEFINES) $(APPSELECTOR_INCLUDE) -c $< -o $@

#flvplayback
FLVPLAYBACK_INCLUDE=$(THELIB_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/applications/flvplayback/include
FLVPLAYBACK_LIBS=$(THELIB_LIBS) -L$(OUTPUT_DYNAMIC) -lthelib
FLVPLAYBACK_SRCS=$(shell find $(PROJECT_BASE_PATH)/sources/applications/flvplayback/src -type f -name "*.cpp")
FLVPLAYBACK_OBJS=$(FLVPLAYBACK_SRCS:.cpp=.flvplayback.o)

flvplayback: thelib $(FLVPLAYBACK_OBJS)
	@echo ----------- linking application flvplayback
	@mkdir -p $(OUTPUT_DYNAMIC)/applications/flvplayback/mediaFolder
	@mkdir -p $(OUTPUT_STATIC)/applications/flvplayback/mediaFolder
	$(CXX) -fPIC -shared $(FLVPLAYBACK_LIBS) -o $(call dynamic_lib_name,flvplayback,/applications/flvplayback) $(call dynamic_lib_flags,flvplayback) $(FLVPLAYBACK_OBJS)
	$(STRIP) -sx $(call dynamic_lib_name,flvplayback,/applications/flvplayback)
	@echo -----------

%.flvplayback.o: %.cpp
	$(CXX) -fPIC $(DEFINES) $(FLVPLAYBACK_INCLUDE) -c $< -o $@

#proxypublish
PROXYPUBLISH_INCLUDE=$(THELIB_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/applications/proxypublish/include
PROXYPUBLISH_LIBS=$(THELIB_LIBS) -L$(OUTPUT_DYNAMIC) -lthelib
PROXYPUBLISH_SRCS=$(shell find $(PROJECT_BASE_PATH)/sources/applications/proxypublish/src -type f -name "*.cpp")
PROXYPUBLISH_OBJS=$(PROXYPUBLISH_SRCS:.cpp=.proxypublish.o)

proxypublish: thelib $(PROXYPUBLISH_OBJS)
	@echo ----------- linking application proxypublish
	@mkdir -p $(OUTPUT_DYNAMIC)/applications/proxypublish/mediaFolder
	@mkdir -p $(OUTPUT_STATIC)/applications/proxypublish/mediaFolder
	$(CXX) -fPIC -shared $(PROXYPUBLISH_LIBS) -o $(call dynamic_lib_name,proxypublish,/applications/proxypublish) $(call dynamic_lib_flags,proxypublish) $(PROXYPUBLISH_OBJS)
	$(STRIP) -sx $(call dynamic_lib_name,proxypublish,/applications/proxypublish)
	@echo -----------

%.proxypublish.o: %.cpp
	$(CXX) -fPIC $(DEFINES) $(PROXYPUBLISH_INCLUDE) -c $< -o $@

#samplefactory
SAMPLEFACTORY_INCLUDE=$(THELIB_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/applications/samplefactory/include
SAMPLEFACTORY_LIBS=$(THELIB_LIBS) -L$(OUTPUT_DYNAMIC) -lthelib
SAMPLEFACTORY_SRCS=$(shell find $(PROJECT_BASE_PATH)/sources/applications/samplefactory/src -type f -name "*.cpp")
SAMPLEFACTORY_OBJS=$(SAMPLEFACTORY_SRCS:.cpp=.samplefactory.o)

samplefactory: thelib $(SAMPLEFACTORY_OBJS)
	@echo ----------- linking application samplefactory
	@mkdir -p $(OUTPUT_DYNAMIC)/applications/samplefactory/mediaFolder
	@mkdir -p $(OUTPUT_STATIC)/applications/samplefactory/mediaFolder
	$(CXX) -fPIC -shared $(SAMPLEFACTORY_LIBS) -o $(call dynamic_lib_name,samplefactory,/applications/samplefactory) $(call dynamic_lib_flags,samplefactory) $(SAMPLEFACTORY_OBJS)
	$(STRIP) -sx $(call dynamic_lib_name,samplefactory,/applications/samplefactory)
	@echo -----------

%.samplefactory.o: %.cpp
	$(CXX) -fPIC $(DEFINES) $(SAMPLEFACTORY_INCLUDE) -c $< -o $@

#stresstest
STRESSTEST_INCLUDE=$(THELIB_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/applications/stresstest/include
STRESSTEST_LIBS=$(THELIB_LIBS) -L$(OUTPUT_DYNAMIC) -lthelib
STRESSTEST_SRCS=$(shell find $(PROJECT_BASE_PATH)/sources/applications/stresstest/src -type f -name "*.cpp")
STRESSTEST_OBJS=$(STRESSTEST_SRCS:.cpp=.stresstest.o)

stresstest: thelib $(STRESSTEST_OBJS)
	@echo ----------- linking application stresstest
	@mkdir -p $(OUTPUT_DYNAMIC)/applications/stresstest/mediaFolder
	@mkdir -p $(OUTPUT_STATIC)/applications/stresstest/mediaFolder
	$(CXX) -fPIC -shared $(STRESSTEST_LIBS) -o $(call dynamic_lib_name,stresstest,/applications/stresstest) $(call dynamic_lib_flags,stresstest) $(STRESSTEST_OBJS)
	$(STRIP) -sx $(call dynamic_lib_name,stresstest,/applications/stresstest)
	@echo -----------

%.stresstest.o: %.cpp
	$(CXX) -fPIC $(DEFINES) $(STRESSTEST_INCLUDE) -c $< -o $@

#vptests
VPTESTS_INCLUDE=$(THELIB_INCLUDE) -I$(PROJECT_BASE_PATH)/sources/applications/vptests/include
VPTESTS_LIBS=$(THELIB_LIBS) -L$(OUTPUT_DYNAMIC) -lthelib
VPTESTS_SRCS=$(shell find $(PROJECT_BASE_PATH)/sources/applications/vptests/src -type f -name "*.cpp")
VPTESTS_OBJS=$(VPTESTS_SRCS:.cpp=.vptests.o)

vptests: thelib $(VPTESTS_OBJS)
	@echo ----------- linking application vptests
	@mkdir -p $(OUTPUT_DYNAMIC)/applications/vptests/mediaFolder
	@mkdir -p $(OUTPUT_STATIC)/applications/vptests/mediaFolder
	$(CXX) -fPIC -shared $(VPTESTS_LIBS) -o $(call dynamic_lib_name,vptests,/applications/vptests) $(call dynamic_lib_flags,vptests) $(VPTESTS_OBJS)
	$(STRIP) -sx $(call dynamic_lib_name,vptests,/applications/vptests)
	@echo -----------

%.vptests.o: %.cpp
	$(CXX) -fPIC $(DEFINES) $(VPTESTS_INCLUDE) -c $< -o $@

ALL_APPS_OBJS= $(ADMIN_OBJS) $(APPLESTREAMINGCLIENT_OBJS) $(APPSELECTOR_OBJS) $(FLVPLAYBACK_OBJS) $(PROXYPUBLISH_OBJS) $(SAMPLEFACTORY_OBJS) $(STRESSTEST_OBJS) $(VPTESTS_OBJS)
applications: thelib admin applestreamingclient appselector flvplayback proxypublish samplefactory stresstest vptests
