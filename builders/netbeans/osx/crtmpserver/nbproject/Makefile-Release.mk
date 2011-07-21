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
	${OBJECTDIR}/_ext/802483657/crtmpserver.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../${CND_CONF}/${CND_PLATFORM}/rtmpserver

../${CND_CONF}/${CND_PLATFORM}/rtmpserver: ../common/dist/Release/GNU-MacOSX/libcommon.dylib

../${CND_CONF}/${CND_PLATFORM}/rtmpserver: ../thelib/../Release/GNU-MacOSX/libthelib.dylib

../${CND_CONF}/${CND_PLATFORM}/rtmpserver: ${OBJECTFILES}
	${MKDIR} -p ../${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -L/opt/local/lib -llua -o ../${CND_CONF}/${CND_PLATFORM}/rtmpserver ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/802483657/crtmpserver.o: ../../../../sources/crtmpserver/src/crtmpserver.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/802483657
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DNET_KQUEUE -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/802483657/crtmpserver.o ../../../../sources/crtmpserver/src/crtmpserver.cpp

# Subprojects
.build-subprojects:
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Release
	cd ../thelib && ${MAKE}  -f Makefile.nb CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../${CND_CONF}/${CND_PLATFORM}/rtmpserver

# Subprojects
.clean-subprojects:
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Release clean
	cd ../thelib && ${MAKE}  -f Makefile.nb CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
