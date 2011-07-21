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
	${OBJECTDIR}/_ext/820541149/monitorstream.o \
	${OBJECTDIR}/_ext/820541149/monitorrtmpprotocolhandler.o \
	${OBJECTDIR}/_ext/820541149/monitorapplication.o \
	${OBJECTDIR}/_ext/820541149/main.o \
	${OBJECTDIR}/_ext/820541149/basefeeder.o \
	${OBJECTDIR}/_ext/820541149/monitorrtmpprotocol.o


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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/trafficdissector

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/trafficdissector: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/trafficdissector ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/820541149/monitorstream.o: ../../../../sources/trafficdissector/src/monitorstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/820541149
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_SELECT -DHAS_LUA -DHAS_PROTOCOL_RTMP -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/trafficdissector/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/820541149/monitorstream.o ../../../../sources/trafficdissector/src/monitorstream.cpp

${OBJECTDIR}/_ext/820541149/monitorrtmpprotocolhandler.o: ../../../../sources/trafficdissector/src/monitorrtmpprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/820541149
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_SELECT -DHAS_LUA -DHAS_PROTOCOL_RTMP -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/trafficdissector/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/820541149/monitorrtmpprotocolhandler.o ../../../../sources/trafficdissector/src/monitorrtmpprotocolhandler.cpp

${OBJECTDIR}/_ext/820541149/monitorapplication.o: ../../../../sources/trafficdissector/src/monitorapplication.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/820541149
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_SELECT -DHAS_LUA -DHAS_PROTOCOL_RTMP -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/trafficdissector/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/820541149/monitorapplication.o ../../../../sources/trafficdissector/src/monitorapplication.cpp

${OBJECTDIR}/_ext/820541149/main.o: ../../../../sources/trafficdissector/src/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/820541149
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_SELECT -DHAS_LUA -DHAS_PROTOCOL_RTMP -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/trafficdissector/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/820541149/main.o ../../../../sources/trafficdissector/src/main.cpp

${OBJECTDIR}/_ext/820541149/basefeeder.o: ../../../../sources/trafficdissector/src/basefeeder.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/820541149
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_SELECT -DHAS_LUA -DHAS_PROTOCOL_RTMP -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/trafficdissector/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/820541149/basefeeder.o ../../../../sources/trafficdissector/src/basefeeder.cpp

${OBJECTDIR}/_ext/820541149/monitorrtmpprotocol.o: ../../../../sources/trafficdissector/src/monitorrtmpprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/820541149
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_SELECT -DHAS_LUA -DHAS_PROTOCOL_RTMP -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/trafficdissector/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/820541149/monitorrtmpprotocol.o ../../../../sources/trafficdissector/src/monitorrtmpprotocol.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/trafficdissector

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
