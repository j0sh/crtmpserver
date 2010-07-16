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
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/vmtests/src/main.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/vmtests/src/testvmlua.o

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
LDLIBSOPTIONS=-L../common/dist/Debug/GNU-MacOSX -lcommon -L../vm/dist/Debug/GNU-MacOSX -lvm

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-MacOSX/vmtests

dist/Debug/GNU-MacOSX/vmtests: ../common/dist/Debug/GNU-MacOSX/libcommon.dylib

dist/Debug/GNU-MacOSX/vmtests: ../vm/dist/Debug/GNU-MacOSX/libvm.dylib

dist/Debug/GNU-MacOSX/vmtests: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-MacOSX
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/vmtests ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/vmtests/src/main.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/vmtests/src/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/vmtests/src
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DVM_LUA -I../../../../sources/common/include -I../../../../sources/vm/include -I../../../../sources/vmtests/include -I../../../../3rdparty/v8/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/vmtests/src/main.o ../../../../sources/vmtests/src/main.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/vmtests/src/testvmlua.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/vmtests/src/testvmlua.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/vmtests/src
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DVM_LUA -I../../../../sources/common/include -I../../../../sources/vm/include -I../../../../sources/vmtests/include -I../../../../3rdparty/v8/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/vmtests/src/testvmlua.o ../../../../sources/vmtests/src/testvmlua.cpp

# Subprojects
.build-subprojects:
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Debug
	cd ../vm && ${MAKE}  -f Makefile.nb CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-MacOSX/vmtests

# Subprojects
.clean-subprojects:
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Debug clean
	cd ../vm && ${MAKE}  -f Makefile.nb CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
