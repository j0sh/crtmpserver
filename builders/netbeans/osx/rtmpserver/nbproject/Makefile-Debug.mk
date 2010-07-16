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
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU-MacOSX
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile.nb

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/rtmpserver/src/rtmpserver.o

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
	${MAKE}  -f nbproject/Makefile-Debug.mk ../Debug/GNU-MacOSX/rtmpserver

../Debug/GNU-MacOSX/rtmpserver: ../thelib/../Debug/GNU-MacOSX/libthelib.dylib

../Debug/GNU-MacOSX/rtmpserver: ../common/dist/Debug/GNU-MacOSX/libcommon.dylib

../Debug/GNU-MacOSX/rtmpserver: ${OBJECTFILES}
	${MKDIR} -p ../Debug/GNU-MacOSX
	${LINK.cc} -L/opt/local/lib -llua -o ../${CND_CONF}/${CND_PLATFORM}/rtmpserver ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/rtmpserver/src/rtmpserver.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/rtmpserver/src/rtmpserver.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/rtmpserver/src
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DNET_KQUEUE -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/rtmpserver/src/rtmpserver.o ../../../../sources/rtmpserver/src/rtmpserver.cpp

# Subprojects
.build-subprojects:
	cd ../thelib && ${MAKE}  -f Makefile.nb CONF=Debug
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} ../Debug/GNU-MacOSX/rtmpserver

# Subprojects
.clean-subprojects:
	cd ../thelib && ${MAKE}  -f Makefile.nb CONF=Debug clean
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
