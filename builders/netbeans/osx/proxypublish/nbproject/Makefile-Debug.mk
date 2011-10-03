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
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile.nb

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1106680373/jobstimerappprotocolhandler.o \
	${OBJECTDIR}/_ext/1106680373/rtpappprotocolhandler.o \
	${OBJECTDIR}/_ext/1106680373/proxypublishapplication.o \
	${OBJECTDIR}/_ext/1106680373/liveflvappprotocolhandler.o \
	${OBJECTDIR}/_ext/1106680373/rtspappprotocolhandler.o \
	${OBJECTDIR}/_ext/1106680373/proxypublish.o \
	${OBJECTDIR}/_ext/1106680373/rtmpappprotocolhandler.o \
	${OBJECTDIR}/_ext/1106680373/jobstimerprotocol.o


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
LDLIBSOPTIONS=-L../thelib/../Debug/GNU-MacOSX -lthelib -L../common/dist/Debug/GNU-MacOSX -lcommon

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../${CND_CONF}/${CND_PLATFORM}/applications/proxypublish/libproxypublish.dylib

../${CND_CONF}/${CND_PLATFORM}/applications/proxypublish/libproxypublish.dylib: ../thelib/../Debug/GNU-MacOSX/libthelib.dylib

../${CND_CONF}/${CND_PLATFORM}/applications/proxypublish/libproxypublish.dylib: ../common/dist/Debug/GNU-MacOSX/libcommon.dylib

../${CND_CONF}/${CND_PLATFORM}/applications/proxypublish/libproxypublish.dylib: ${OBJECTFILES}
	${MKDIR} -p ../${CND_CONF}/${CND_PLATFORM}/applications/proxypublish
	${LINK.cc} -dynamiclib -install_name libproxypublish.dylib -o ../${CND_CONF}/${CND_PLATFORM}/applications/proxypublish/libproxypublish.dylib -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/1106680373/jobstimerappprotocolhandler.o: ../../../../sources/applications/proxypublish/src/jobstimerappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1106680373
	${RM} $@.d
	$(COMPILE.cc) -g -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/proxypublish/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1106680373/jobstimerappprotocolhandler.o ../../../../sources/applications/proxypublish/src/jobstimerappprotocolhandler.cpp

${OBJECTDIR}/_ext/1106680373/rtpappprotocolhandler.o: ../../../../sources/applications/proxypublish/src/rtpappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1106680373
	${RM} $@.d
	$(COMPILE.cc) -g -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/proxypublish/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1106680373/rtpappprotocolhandler.o ../../../../sources/applications/proxypublish/src/rtpappprotocolhandler.cpp

${OBJECTDIR}/_ext/1106680373/proxypublishapplication.o: ../../../../sources/applications/proxypublish/src/proxypublishapplication.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1106680373
	${RM} $@.d
	$(COMPILE.cc) -g -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/proxypublish/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1106680373/proxypublishapplication.o ../../../../sources/applications/proxypublish/src/proxypublishapplication.cpp

${OBJECTDIR}/_ext/1106680373/liveflvappprotocolhandler.o: ../../../../sources/applications/proxypublish/src/liveflvappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1106680373
	${RM} $@.d
	$(COMPILE.cc) -g -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/proxypublish/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1106680373/liveflvappprotocolhandler.o ../../../../sources/applications/proxypublish/src/liveflvappprotocolhandler.cpp

${OBJECTDIR}/_ext/1106680373/rtspappprotocolhandler.o: ../../../../sources/applications/proxypublish/src/rtspappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1106680373
	${RM} $@.d
	$(COMPILE.cc) -g -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/proxypublish/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1106680373/rtspappprotocolhandler.o ../../../../sources/applications/proxypublish/src/rtspappprotocolhandler.cpp

${OBJECTDIR}/_ext/1106680373/proxypublish.o: ../../../../sources/applications/proxypublish/src/proxypublish.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1106680373
	${RM} $@.d
	$(COMPILE.cc) -g -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/proxypublish/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1106680373/proxypublish.o ../../../../sources/applications/proxypublish/src/proxypublish.cpp

${OBJECTDIR}/_ext/1106680373/rtmpappprotocolhandler.o: ../../../../sources/applications/proxypublish/src/rtmpappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1106680373
	${RM} $@.d
	$(COMPILE.cc) -g -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/proxypublish/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1106680373/rtmpappprotocolhandler.o ../../../../sources/applications/proxypublish/src/rtmpappprotocolhandler.cpp

${OBJECTDIR}/_ext/1106680373/jobstimerprotocol.o: ../../../../sources/applications/proxypublish/src/jobstimerprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1106680373
	${RM} $@.d
	$(COMPILE.cc) -g -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/proxypublish/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1106680373/jobstimerprotocol.o ../../../../sources/applications/proxypublish/src/jobstimerprotocol.cpp

# Subprojects
.build-subprojects:
	cd ../thelib && ${MAKE}  -f Makefile.nb CONF=Debug
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../${CND_CONF}/${CND_PLATFORM}/applications/proxypublish/libproxypublish.dylib

# Subprojects
.clean-subprojects:
	cd ../thelib && ${MAKE}  -f Makefile.nb CONF=Debug clean
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
