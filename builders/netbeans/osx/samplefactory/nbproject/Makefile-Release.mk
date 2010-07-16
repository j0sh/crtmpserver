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
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src/samplefactoryapplication.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src/dbaccessprotocol.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src/rtmpappprotocolhandler.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src/controller.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src/samplefactory.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src/protocolfactory.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src/dbaccessprotocolhandler.o

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
	${MAKE}  -f nbproject/Makefile-Release.mk ../Release/GNU-MacOSX/applications/samplefactory/libsamplefactory.dylib

../Release/GNU-MacOSX/applications/samplefactory/libsamplefactory.dylib: ../common/dist/Release/GNU-MacOSX/libcommon.dylib

../Release/GNU-MacOSX/applications/samplefactory/libsamplefactory.dylib: ../thelib/../Release/GNU-MacOSX/libthelib.dylib

../Release/GNU-MacOSX/applications/samplefactory/libsamplefactory.dylib: ${OBJECTFILES}
	${MKDIR} -p ../Release/GNU-MacOSX/applications/samplefactory
	${LINK.cc} -dynamiclib -install_name libsamplefactory.dylib -o ../${CND_CONF}/${CND_PLATFORM}/applications/samplefactory/libsamplefactory.dylib -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src/samplefactoryapplication.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/applications/samplefactory/src/samplefactoryapplication.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DNET_KQUEUE -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/samplefactory/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src/samplefactoryapplication.o ../../../../sources/applications/samplefactory/src/samplefactoryapplication.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src/dbaccessprotocol.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/applications/samplefactory/src/dbaccessprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DNET_KQUEUE -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/samplefactory/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src/dbaccessprotocol.o ../../../../sources/applications/samplefactory/src/dbaccessprotocol.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src/rtmpappprotocolhandler.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/applications/samplefactory/src/rtmpappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DNET_KQUEUE -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/samplefactory/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src/rtmpappprotocolhandler.o ../../../../sources/applications/samplefactory/src/rtmpappprotocolhandler.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src/controller.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/applications/samplefactory/src/controller.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DNET_KQUEUE -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/samplefactory/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src/controller.o ../../../../sources/applications/samplefactory/src/controller.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src/samplefactory.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/applications/samplefactory/src/samplefactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DNET_KQUEUE -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/samplefactory/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src/samplefactory.o ../../../../sources/applications/samplefactory/src/samplefactory.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src/protocolfactory.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/applications/samplefactory/src/protocolfactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DNET_KQUEUE -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/samplefactory/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src/protocolfactory.o ../../../../sources/applications/samplefactory/src/protocolfactory.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src/dbaccessprotocolhandler.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/applications/samplefactory/src/dbaccessprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DNET_KQUEUE -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/samplefactory/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/applications/samplefactory/src/dbaccessprotocolhandler.o ../../../../sources/applications/samplefactory/src/dbaccessprotocolhandler.cpp

# Subprojects
.build-subprojects:
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Release
	cd ../thelib && ${MAKE}  -f Makefile.nb CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} ../Release/GNU-MacOSX/applications/samplefactory/libsamplefactory.dylib

# Subprojects
.clean-subprojects:
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Release clean
	cd ../thelib && ${MAKE}  -f Makefile.nb CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
