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
	${OBJECTDIR}/_ext/431490565/basevmv8.o \
	${OBJECTDIR}/_ext/910696451/basevm.o \
	${OBJECTDIR}/_ext/491298005/basevmlua.o


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
LDLIBSOPTIONS=-L../common/dist/Debug/GNU-MacOSX -lcommon

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvm.dylib

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvm.dylib: ../common/dist/Debug/GNU-MacOSX/libcommon.dylib

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvm.dylib: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} ../../../../3rdparty/v8/libv8.a -dynamiclib -install_name libvm.dylib -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvm.dylib -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/431490565/basevmv8.o: ../../../../sources/vm/src/vmv8/basevmv8.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/431490565
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DHAS_LUA -DHAS_V8 -I../../../../sources/common/include -I../../../../sources/vm/include -I../../../../3rdparty/v8/include -I/opt/local/include -I../../../../../v8/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/431490565/basevmv8.o ../../../../sources/vm/src/vmv8/basevmv8.cpp

${OBJECTDIR}/_ext/910696451/basevm.o: ../../../../sources/vm/src/basevm.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/910696451
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DHAS_LUA -DHAS_V8 -I../../../../sources/common/include -I../../../../sources/vm/include -I../../../../3rdparty/v8/include -I/opt/local/include -I../../../../../v8/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/910696451/basevm.o ../../../../sources/vm/src/basevm.cpp

${OBJECTDIR}/_ext/491298005/basevmlua.o: ../../../../sources/vm/src/vmlua/basevmlua.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/491298005
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DHAS_LUA -DHAS_V8 -I../../../../sources/common/include -I../../../../sources/vm/include -I../../../../3rdparty/v8/include -I/opt/local/include -I../../../../../v8/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/491298005/basevmlua.o ../../../../sources/vm/src/vmlua/basevmlua.cpp

# Subprojects
.build-subprojects:
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvm.dylib

# Subprojects
.clean-subprojects:
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
