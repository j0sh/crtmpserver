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
	${OBJECTDIR}/_ext/1256058352/stresstestapplication.o \
	${OBJECTDIR}/_ext/1256058352/stresstest.o \
	${OBJECTDIR}/_ext/1256058352/rtmpappprotocolhandler.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../${CND_CONF}/${CND_PLATFORM}/applications/stresstest/libstresstest.dylib

../${CND_CONF}/${CND_PLATFORM}/applications/stresstest/libstresstest.dylib: ../thelib/../Debug/GNU-MacOSX/libthelib.dylib

../${CND_CONF}/${CND_PLATFORM}/applications/stresstest/libstresstest.dylib: ../common/dist/Debug/GNU-MacOSX/libcommon.dylib

../${CND_CONF}/${CND_PLATFORM}/applications/stresstest/libstresstest.dylib: ${OBJECTFILES}
	${MKDIR} -p ../${CND_CONF}/${CND_PLATFORM}/applications/stresstest
	${LINK.cc} -dynamiclib -install_name libstresstest.dylib -o ../${CND_CONF}/${CND_PLATFORM}/applications/stresstest/libstresstest.dylib -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/1256058352/stresstestapplication.o: ../../../../sources/applications/stresstest/src/stresstestapplication.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1256058352
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DNET_KQUEUE -DHAS_PROTOCOL_RTMP -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/stresstest/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1256058352/stresstestapplication.o ../../../../sources/applications/stresstest/src/stresstestapplication.cpp

${OBJECTDIR}/_ext/1256058352/stresstest.o: ../../../../sources/applications/stresstest/src/stresstest.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1256058352
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DNET_KQUEUE -DHAS_PROTOCOL_RTMP -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/stresstest/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1256058352/stresstest.o ../../../../sources/applications/stresstest/src/stresstest.cpp

${OBJECTDIR}/_ext/1256058352/rtmpappprotocolhandler.o: ../../../../sources/applications/stresstest/src/rtmpappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1256058352
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DNET_KQUEUE -DHAS_PROTOCOL_RTMP -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/stresstest/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1256058352/rtmpappprotocolhandler.o ../../../../sources/applications/stresstest/src/rtmpappprotocolhandler.cpp

# Subprojects
.build-subprojects:
	cd ../thelib && ${MAKE}  -f Makefile.nb CONF=Debug
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../${CND_CONF}/${CND_PLATFORM}/applications/stresstest/libstresstest.dylib

# Subprojects
.clean-subprojects:
	cd ../thelib && ${MAKE}  -f Makefile.nb CONF=Debug clean
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
