#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-MacOSX
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile.nb

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/466364438/tsappprotocolhandler.o \
	${OBJECTDIR}/_ext/581372517/luaapi_handler_rtmp.o \
	${OBJECTDIR}/_ext/581372517/luaappvirtualmachine.o \
	${OBJECTDIR}/_ext/581372517/luaapi_protocols.o \
	${OBJECTDIR}/_ext/581372517/luaapi_handler_mpegts.o \
	${OBJECTDIR}/_ext/466364438/rtspappprotocolhandler.o \
	${OBJECTDIR}/_ext/466364438/rtpappprotocolhandler.o \
	${OBJECTDIR}/_ext/466364438/vmappapplication.o \
	${OBJECTDIR}/_ext/756279036/baseappvirtualmachine.o \
	${OBJECTDIR}/_ext/581372517/luaapi_generics.o \
	${OBJECTDIR}/_ext/466364438/rtmpappprotocolhandler.o \
	${OBJECTDIR}/_ext/466364438/vmapp.o \
	${OBJECTDIR}/_ext/581372517/luaapi_application.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../common/dist/Release/GNU-MacOSX -lcommon -L../thelib/../Release/GNU-MacOSX -lthelib

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../${CND_CONF}/${CND_PLATFORM}/applications/luavmapp/libluavmapp.dylib

../${CND_CONF}/${CND_PLATFORM}/applications/luavmapp/libluavmapp.dylib: ../common/dist/Release/GNU-MacOSX/libcommon.dylib

../${CND_CONF}/${CND_PLATFORM}/applications/luavmapp/libluavmapp.dylib: ../thelib/../Release/GNU-MacOSX/libthelib.dylib

../${CND_CONF}/${CND_PLATFORM}/applications/luavmapp/libluavmapp.dylib: ${OBJECTFILES}
	${MKDIR} -p ../${CND_CONF}/${CND_PLATFORM}/applications/luavmapp
	${LINK.cc} -dynamiclib -install_name libluavmapp.dylib -o ../${CND_CONF}/${CND_PLATFORM}/applications/luavmapp/libluavmapp.dylib -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/466364438/tsappprotocolhandler.o: ../../../../sources/applications/vmapp/src/tsappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/466364438
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DNET_KQUEUE -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/luavmapp/include -I../../../../sources/vm/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/466364438/tsappprotocolhandler.o ../../../../sources/applications/vmapp/src/tsappprotocolhandler.cpp

${OBJECTDIR}/_ext/581372517/luaapi_handler_rtmp.o: ../../../../sources/applications/vmapp/src/vm/lua/luaapi_handler_rtmp.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/581372517
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DNET_KQUEUE -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/luavmapp/include -I../../../../sources/vm/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/581372517/luaapi_handler_rtmp.o ../../../../sources/applications/vmapp/src/vm/lua/luaapi_handler_rtmp.cpp

${OBJECTDIR}/_ext/581372517/luaappvirtualmachine.o: ../../../../sources/applications/vmapp/src/vm/lua/luaappvirtualmachine.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/581372517
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DNET_KQUEUE -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/luavmapp/include -I../../../../sources/vm/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/581372517/luaappvirtualmachine.o ../../../../sources/applications/vmapp/src/vm/lua/luaappvirtualmachine.cpp

${OBJECTDIR}/_ext/581372517/luaapi_protocols.o: ../../../../sources/applications/vmapp/src/vm/lua/luaapi_protocols.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/581372517
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DNET_KQUEUE -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/luavmapp/include -I../../../../sources/vm/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/581372517/luaapi_protocols.o ../../../../sources/applications/vmapp/src/vm/lua/luaapi_protocols.cpp

${OBJECTDIR}/_ext/581372517/luaapi_handler_mpegts.o: ../../../../sources/applications/vmapp/src/vm/lua/luaapi_handler_mpegts.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/581372517
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DNET_KQUEUE -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/luavmapp/include -I../../../../sources/vm/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/581372517/luaapi_handler_mpegts.o ../../../../sources/applications/vmapp/src/vm/lua/luaapi_handler_mpegts.cpp

${OBJECTDIR}/_ext/466364438/rtspappprotocolhandler.o: ../../../../sources/applications/vmapp/src/rtspappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/466364438
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DNET_KQUEUE -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/luavmapp/include -I../../../../sources/vm/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/466364438/rtspappprotocolhandler.o ../../../../sources/applications/vmapp/src/rtspappprotocolhandler.cpp

${OBJECTDIR}/_ext/466364438/rtpappprotocolhandler.o: ../../../../sources/applications/vmapp/src/rtpappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/466364438
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DNET_KQUEUE -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/luavmapp/include -I../../../../sources/vm/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/466364438/rtpappprotocolhandler.o ../../../../sources/applications/vmapp/src/rtpappprotocolhandler.cpp

${OBJECTDIR}/_ext/466364438/vmappapplication.o: ../../../../sources/applications/vmapp/src/vmappapplication.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/466364438
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DNET_KQUEUE -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/luavmapp/include -I../../../../sources/vm/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/466364438/vmappapplication.o ../../../../sources/applications/vmapp/src/vmappapplication.cpp

${OBJECTDIR}/_ext/756279036/baseappvirtualmachine.o: ../../../../sources/applications/vmapp/src/vm/baseappvirtualmachine.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/756279036
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DNET_KQUEUE -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/luavmapp/include -I../../../../sources/vm/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/756279036/baseappvirtualmachine.o ../../../../sources/applications/vmapp/src/vm/baseappvirtualmachine.cpp

${OBJECTDIR}/_ext/581372517/luaapi_generics.o: ../../../../sources/applications/vmapp/src/vm/lua/luaapi_generics.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/581372517
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DNET_KQUEUE -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/luavmapp/include -I../../../../sources/vm/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/581372517/luaapi_generics.o ../../../../sources/applications/vmapp/src/vm/lua/luaapi_generics.cpp

${OBJECTDIR}/_ext/466364438/rtmpappprotocolhandler.o: ../../../../sources/applications/vmapp/src/rtmpappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/466364438
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DNET_KQUEUE -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/luavmapp/include -I../../../../sources/vm/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/466364438/rtmpappprotocolhandler.o ../../../../sources/applications/vmapp/src/rtmpappprotocolhandler.cpp

${OBJECTDIR}/_ext/466364438/vmapp.o: ../../../../sources/applications/vmapp/src/vmapp.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/466364438
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DNET_KQUEUE -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/luavmapp/include -I../../../../sources/vm/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/466364438/vmapp.o ../../../../sources/applications/vmapp/src/vmapp.cpp

${OBJECTDIR}/_ext/581372517/luaapi_application.o: ../../../../sources/applications/vmapp/src/vm/lua/luaapi_application.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/581372517
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DNET_KQUEUE -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/luavmapp/include -I../../../../sources/vm/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/581372517/luaapi_application.o ../../../../sources/applications/vmapp/src/vm/lua/luaapi_application.cpp

# Subprojects
.build-subprojects:
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Release
	cd ../thelib && ${MAKE}  -f Makefile.nb CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../${CND_CONF}/${CND_PLATFORM}/applications/luavmapp/libluavmapp.dylib

# Subprojects
.clean-subprojects:
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Release clean
	cd ../thelib && ${MAKE}  -f Makefile.nb CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
