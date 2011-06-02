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
	${OBJECTDIR}/_ext/573837895/vptestsapplication.o \
	${OBJECTDIR}/_ext/573837895/vptests.o \
	${OBJECTDIR}/_ext/573837895/rtmpappprotocolhandler.o \
	${OBJECTDIR}/_ext/573837895/variantappprotocolhandler.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../${CND_CONF}/${CND_PLATFORM}/applications/vptests/libvptests.dylib

../${CND_CONF}/${CND_PLATFORM}/applications/vptests/libvptests.dylib: ../thelib/../Debug/GNU-MacOSX/libthelib.dylib

../${CND_CONF}/${CND_PLATFORM}/applications/vptests/libvptests.dylib: ../common/dist/Debug/GNU-MacOSX/libcommon.dylib

../${CND_CONF}/${CND_PLATFORM}/applications/vptests/libvptests.dylib: ${OBJECTFILES}
	${MKDIR} -p ../${CND_CONF}/${CND_PLATFORM}/applications/vptests
	${LINK.cc} -dynamiclib -install_name libvptests.dylib -o ../${CND_CONF}/${CND_PLATFORM}/applications/vptests/libvptests.dylib -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/573837895/vptestsapplication.o: ../../../../sources/applications/vptests/src/vptestsapplication.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/573837895
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DNET_KQUEUE -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_VAR -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/vptests/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/573837895/vptestsapplication.o ../../../../sources/applications/vptests/src/vptestsapplication.cpp

${OBJECTDIR}/_ext/573837895/vptests.o: ../../../../sources/applications/vptests/src/vptests.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/573837895
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DNET_KQUEUE -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_VAR -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/vptests/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/573837895/vptests.o ../../../../sources/applications/vptests/src/vptests.cpp

${OBJECTDIR}/_ext/573837895/rtmpappprotocolhandler.o: ../../../../sources/applications/vptests/src/rtmpappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/573837895
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DNET_KQUEUE -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_VAR -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/vptests/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/573837895/rtmpappprotocolhandler.o ../../../../sources/applications/vptests/src/rtmpappprotocolhandler.cpp

${OBJECTDIR}/_ext/573837895/variantappprotocolhandler.o: ../../../../sources/applications/vptests/src/variantappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/573837895
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DNET_KQUEUE -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_VAR -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/applications/vptests/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/573837895/variantappprotocolhandler.o ../../../../sources/applications/vptests/src/variantappprotocolhandler.cpp

# Subprojects
.build-subprojects:
	cd ../thelib && ${MAKE}  -f Makefile.nb CONF=Debug
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../${CND_CONF}/${CND_PLATFORM}/applications/vptests/libvptests.dylib

# Subprojects
.clean-subprojects:
	cd ../thelib && ${MAKE}  -f Makefile.nb CONF=Debug clean
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
