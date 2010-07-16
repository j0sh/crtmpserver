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
	${OBJECTDIR}/_ext/1106680373/proxypublishapplication.o \
	${OBJECTDIR}/_ext/1106680373/liveflvappprotocolhandler.o \
	${OBJECTDIR}/_ext/1106680373/proxypublish.o \
	${OBJECTDIR}/_ext/1106680373/rtmpappprotocolhandler.o


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
	"${MAKE}"  -f nbproject/Makefile-Release.mk ../Release/GNU-MacOSX/applications/proxypublish/libproxypublish.dylib

../Release/GNU-MacOSX/applications/proxypublish/libproxypublish.dylib: ../common/dist/Release/GNU-MacOSX/libcommon.dylib

../Release/GNU-MacOSX/applications/proxypublish/libproxypublish.dylib: ../thelib/../Release/GNU-MacOSX/libthelib.dylib

../Release/GNU-MacOSX/applications/proxypublish/libproxypublish.dylib: ${OBJECTFILES}
	${MKDIR} -p ../Release/GNU-MacOSX/applications/proxypublish
	${LINK.cc} -dynamiclib -install_name libproxypublish.dylib -o ../${CND_CONF}/${CND_PLATFORM}/applications/proxypublish/libproxypublish.dylib -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/1106680373/proxypublishapplication.o: ../../../../sources/applications/proxypublish/src/proxypublishapplication.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1106680373
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DNET_KQUEUE -DHAS_PROTOCOL_RTMP -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/proxypublish/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1106680373/proxypublishapplication.o ../../../../sources/applications/proxypublish/src/proxypublishapplication.cpp

${OBJECTDIR}/_ext/1106680373/liveflvappprotocolhandler.o: ../../../../sources/applications/proxypublish/src/liveflvappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1106680373
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DNET_KQUEUE -DHAS_PROTOCOL_RTMP -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/proxypublish/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1106680373/liveflvappprotocolhandler.o ../../../../sources/applications/proxypublish/src/liveflvappprotocolhandler.cpp

${OBJECTDIR}/_ext/1106680373/proxypublish.o: ../../../../sources/applications/proxypublish/src/proxypublish.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1106680373
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DNET_KQUEUE -DHAS_PROTOCOL_RTMP -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/proxypublish/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1106680373/proxypublish.o ../../../../sources/applications/proxypublish/src/proxypublish.cpp

${OBJECTDIR}/_ext/1106680373/rtmpappprotocolhandler.o: ../../../../sources/applications/proxypublish/src/rtmpappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1106680373
	${RM} $@.d
	$(COMPILE.cc) -O2 -DOSX -DNET_KQUEUE -DHAS_PROTOCOL_RTMP -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/proxypublish/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1106680373/rtmpappprotocolhandler.o ../../../../sources/applications/proxypublish/src/rtmpappprotocolhandler.cpp

# Subprojects
.build-subprojects:
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Release
	cd ../thelib && ${MAKE}  -f Makefile.nb CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} ../Release/GNU-MacOSX/applications/proxypublish/libproxypublish.dylib

# Subprojects
.clean-subprojects:
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Release clean
	cd ../thelib && ${MAKE}  -f Makefile.nb CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
