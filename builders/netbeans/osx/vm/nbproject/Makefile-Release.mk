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
CND_CONF=Release
CND_DISTDIR=dist

# Include project Makefile
include Makefile.nb

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/vm/src/basevm.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/all/trunk/sources/vm/src/vmlua/basevmlua.o

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
LDLIBSOPTIONS=-L../common/dist/Release/GNU-MacOSX -lcommon

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Release.mk dist/Release/GNU-MacOSX/libvm.dylib

dist/Release/GNU-MacOSX/libvm.dylib: ../common/dist/Release/GNU-MacOSX/libcommon.dylib

dist/Release/GNU-MacOSX/libvm.dylib: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-MacOSX
	${LINK.cc} ../../../../3rdparty/v8/libv8.a -dynamiclib -install_name libvm.dylib -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvm.dylib -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/vm/src/basevm.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/vm/src/basevm.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/vm/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DVM_LUA -I../../../../sources/common/include -I../../../../sources/vm/include -I../../../../3rdparty/v8/include -I/opt/local/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/vm/src/basevm.o ../../../../sources/vm/src/basevm.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/all/trunk/sources/vm/src/vmlua/basevmlua.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/vm/src/vmlua/basevmlua.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/all/trunk/sources/vm/src/vmlua
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DVM_LUA -I../../../../sources/common/include -I../../../../sources/vm/include -I../../../../3rdparty/v8/include -I/opt/local/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/all/trunk/sources/vm/src/vmlua/basevmlua.o ../../../../sources/vm/src/vmlua/basevmlua.cpp

# Subprojects
.build-subprojects:
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} dist/Release/GNU-MacOSX/libvm.dylib

# Subprojects
.clean-subprojects:
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
