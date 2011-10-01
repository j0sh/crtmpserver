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
	${OBJECTDIR}/_ext/482292139/mmsappprotocolhandler.o \
	${OBJECTDIR}/_ext/482292139/flvplaybackapplication.o \
	${OBJECTDIR}/_ext/482292139/flvplayback.o \
	${OBJECTDIR}/_ext/482292139/liveflvappprotocolhandler.o \
	${OBJECTDIR}/_ext/482292139/rtmpappprotocolhandler.o \
	${OBJECTDIR}/_ext/482292139/rtpappprotocolhandler.o \
	${OBJECTDIR}/_ext/482292139/rawhttpstreamappprotocolhandler.o \
	${OBJECTDIR}/_ext/482292139/httpappprotocolhandler.o \
	${OBJECTDIR}/_ext/482292139/tsappprotocolhandler.o \
	${OBJECTDIR}/_ext/482292139/rtspappprotocolhandler.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../${CND_CONF}/${CND_PLATFORM}/applications/flvplayback/libflvplayback.dylib

../${CND_CONF}/${CND_PLATFORM}/applications/flvplayback/libflvplayback.dylib: ../common/dist/Release/GNU-MacOSX/libcommon.dylib

../${CND_CONF}/${CND_PLATFORM}/applications/flvplayback/libflvplayback.dylib: ../thelib/../Release/GNU-MacOSX/libthelib.dylib

../${CND_CONF}/${CND_PLATFORM}/applications/flvplayback/libflvplayback.dylib: ${OBJECTFILES}
	${MKDIR} -p ../${CND_CONF}/${CND_PLATFORM}/applications/flvplayback
	${LINK.cc} -dynamiclib -install_name libflvplayback.dylib -o ../${CND_CONF}/${CND_PLATFORM}/applications/flvplayback/libflvplayback.dylib -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/482292139/mmsappprotocolhandler.o: ../../../../sources/applications/flvplayback/src/mmsappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/482292139
	${RM} $@.d
	$(COMPILE.cc) -O2 -DHAS_LUA -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/flvplayback/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/482292139/mmsappprotocolhandler.o ../../../../sources/applications/flvplayback/src/mmsappprotocolhandler.cpp

${OBJECTDIR}/_ext/482292139/flvplaybackapplication.o: ../../../../sources/applications/flvplayback/src/flvplaybackapplication.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/482292139
	${RM} $@.d
	$(COMPILE.cc) -O2 -DHAS_LUA -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/flvplayback/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/482292139/flvplaybackapplication.o ../../../../sources/applications/flvplayback/src/flvplaybackapplication.cpp

${OBJECTDIR}/_ext/482292139/flvplayback.o: ../../../../sources/applications/flvplayback/src/flvplayback.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/482292139
	${RM} $@.d
	$(COMPILE.cc) -O2 -DHAS_LUA -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/flvplayback/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/482292139/flvplayback.o ../../../../sources/applications/flvplayback/src/flvplayback.cpp

${OBJECTDIR}/_ext/482292139/liveflvappprotocolhandler.o: ../../../../sources/applications/flvplayback/src/liveflvappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/482292139
	${RM} $@.d
	$(COMPILE.cc) -O2 -DHAS_LUA -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/flvplayback/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/482292139/liveflvappprotocolhandler.o ../../../../sources/applications/flvplayback/src/liveflvappprotocolhandler.cpp

${OBJECTDIR}/_ext/482292139/rtmpappprotocolhandler.o: ../../../../sources/applications/flvplayback/src/rtmpappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/482292139
	${RM} $@.d
	$(COMPILE.cc) -O2 -DHAS_LUA -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/flvplayback/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/482292139/rtmpappprotocolhandler.o ../../../../sources/applications/flvplayback/src/rtmpappprotocolhandler.cpp

${OBJECTDIR}/_ext/482292139/rtpappprotocolhandler.o: ../../../../sources/applications/flvplayback/src/rtpappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/482292139
	${RM} $@.d
	$(COMPILE.cc) -O2 -DHAS_LUA -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/flvplayback/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/482292139/rtpappprotocolhandler.o ../../../../sources/applications/flvplayback/src/rtpappprotocolhandler.cpp

${OBJECTDIR}/_ext/482292139/rawhttpstreamappprotocolhandler.o: ../../../../sources/applications/flvplayback/src/rawhttpstreamappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/482292139
	${RM} $@.d
	$(COMPILE.cc) -O2 -DHAS_LUA -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/flvplayback/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/482292139/rawhttpstreamappprotocolhandler.o ../../../../sources/applications/flvplayback/src/rawhttpstreamappprotocolhandler.cpp

${OBJECTDIR}/_ext/482292139/httpappprotocolhandler.o: ../../../../sources/applications/flvplayback/src/httpappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/482292139
	${RM} $@.d
	$(COMPILE.cc) -O2 -DHAS_LUA -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/flvplayback/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/482292139/httpappprotocolhandler.o ../../../../sources/applications/flvplayback/src/httpappprotocolhandler.cpp

${OBJECTDIR}/_ext/482292139/tsappprotocolhandler.o: ../../../../sources/applications/flvplayback/src/tsappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/482292139
	${RM} $@.d
	$(COMPILE.cc) -O2 -DHAS_LUA -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/flvplayback/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/482292139/tsappprotocolhandler.o ../../../../sources/applications/flvplayback/src/tsappprotocolhandler.cpp

${OBJECTDIR}/_ext/482292139/rtspappprotocolhandler.o: ../../../../sources/applications/flvplayback/src/rtspappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/482292139
	${RM} $@.d
	$(COMPILE.cc) -O2 -DHAS_LUA -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/flvplayback/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/482292139/rtspappprotocolhandler.o ../../../../sources/applications/flvplayback/src/rtspappprotocolhandler.cpp

# Subprojects
.build-subprojects:
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Release
	cd ../thelib && ${MAKE}  -f Makefile.nb CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../${CND_CONF}/${CND_PLATFORM}/applications/flvplayback/libflvplayback.dylib

# Subprojects
.clean-subprojects:
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Release clean
	cd ../thelib && ${MAKE}  -f Makefile.nb CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
