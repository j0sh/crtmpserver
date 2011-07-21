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
	${OBJECTDIR}/_ext/1218434808/main.o \
	${OBJECTDIR}/_ext/1218434808/testvmlua.o \
	${OBJECTDIR}/_ext/1218434808/testvmv8.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/vmtests

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/vmtests: ../common/dist/Debug/GNU-MacOSX/libcommon.dylib

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/vmtests: ../vm/dist/Debug/GNU-MacOSX/libvm.dylib

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/vmtests: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/vmtests ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/1218434808/main.o: ../../../../sources/vmtests/src/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1218434808
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DHAS_LUA -DHAS_V8 -I../../../../sources/common/include -I../../../../sources/vm/include -I../../../../sources/vmtests/include -I../../../../../v8/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1218434808/main.o ../../../../sources/vmtests/src/main.cpp

${OBJECTDIR}/_ext/1218434808/testvmlua.o: ../../../../sources/vmtests/src/testvmlua.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1218434808
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DHAS_LUA -DHAS_V8 -I../../../../sources/common/include -I../../../../sources/vm/include -I../../../../sources/vmtests/include -I../../../../../v8/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1218434808/testvmlua.o ../../../../sources/vmtests/src/testvmlua.cpp

${OBJECTDIR}/_ext/1218434808/testvmv8.o: ../../../../sources/vmtests/src/testvmv8.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1218434808
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DHAS_LUA -DHAS_V8 -I../../../../sources/common/include -I../../../../sources/vm/include -I../../../../sources/vmtests/include -I../../../../../v8/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1218434808/testvmv8.o ../../../../sources/vmtests/src/testvmv8.cpp

# Subprojects
.build-subprojects:
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Debug
	cd ../vm && ${MAKE}  -f Makefile.nb CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/vmtests

# Subprojects
.clean-subprojects:
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Debug clean
	cd ../vm && ${MAKE}  -f Makefile.nb CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
