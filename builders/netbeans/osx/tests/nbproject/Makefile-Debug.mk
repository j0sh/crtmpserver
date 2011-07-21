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
	${OBJECTDIR}/_ext/2055716895/basetestssuite.o \
	${OBJECTDIR}/_ext/2055716895/main.o \
	${OBJECTDIR}/_ext/2055716895/thelibtestssuite.o \
	${OBJECTDIR}/_ext/2055716895/varianttestssuite.o \
	${OBJECTDIR}/_ext/2055716895/commontestssuite.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtests.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtests.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtests.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtests.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtests.a

${OBJECTDIR}/_ext/2055716895/basetestssuite.o: ../../../../sources/tests/src/basetestssuite.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2055716895
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DNET_KQUEUE -DHAS_PROTOCOL_RTMP -DLITTLE_ENDIAN_BYTE_ALIGNED -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/tests/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2055716895/basetestssuite.o ../../../../sources/tests/src/basetestssuite.cpp

${OBJECTDIR}/_ext/2055716895/main.o: ../../../../sources/tests/src/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2055716895
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DNET_KQUEUE -DHAS_PROTOCOL_RTMP -DLITTLE_ENDIAN_BYTE_ALIGNED -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/tests/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2055716895/main.o ../../../../sources/tests/src/main.cpp

${OBJECTDIR}/_ext/2055716895/thelibtestssuite.o: ../../../../sources/tests/src/thelibtestssuite.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2055716895
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DNET_KQUEUE -DHAS_PROTOCOL_RTMP -DLITTLE_ENDIAN_BYTE_ALIGNED -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/tests/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2055716895/thelibtestssuite.o ../../../../sources/tests/src/thelibtestssuite.cpp

${OBJECTDIR}/_ext/2055716895/varianttestssuite.o: ../../../../sources/tests/src/varianttestssuite.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2055716895
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DNET_KQUEUE -DHAS_PROTOCOL_RTMP -DLITTLE_ENDIAN_BYTE_ALIGNED -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/tests/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2055716895/varianttestssuite.o ../../../../sources/tests/src/varianttestssuite.cpp

${OBJECTDIR}/_ext/2055716895/commontestssuite.o: ../../../../sources/tests/src/commontestssuite.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2055716895
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DNET_KQUEUE -DHAS_PROTOCOL_RTMP -DLITTLE_ENDIAN_BYTE_ALIGNED -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/tests/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2055716895/commontestssuite.o ../../../../sources/tests/src/commontestssuite.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtests.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
