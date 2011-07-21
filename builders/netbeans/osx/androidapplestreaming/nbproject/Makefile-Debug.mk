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
	${OBJECTDIR}/_ext/2044958859/variantconnection.o \
	${OBJECTDIR}/_ext/2044958859/main.o \
	${OBJECTDIR}/_ext/2044958859/jniwrapper.o \
	${OBJECTDIR}/_ext/2044958859/api.o


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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libandroidapplestreaming.dylib

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libandroidapplestreaming.dylib: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -dynamiclib -install_name libandroidapplestreaming.dylib -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libandroidapplestreaming.dylib -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/2044958859/variantconnection.o: ../../../../sources/androidapplestreaming/src/variantconnection.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2044958859
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DNET_KQUEUE -DANDROID -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/androidapplestreaming/include -I../../../../sources/applications/applestreamingclient/include -I../../../../../android/workbench/android-ndk-r4-crystax/build/platforms/android-4/arch-arm/usr/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2044958859/variantconnection.o ../../../../sources/androidapplestreaming/src/variantconnection.cpp

${OBJECTDIR}/_ext/2044958859/main.o: ../../../../sources/androidapplestreaming/src/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2044958859
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DNET_KQUEUE -DANDROID -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/androidapplestreaming/include -I../../../../sources/applications/applestreamingclient/include -I../../../../../android/workbench/android-ndk-r4-crystax/build/platforms/android-4/arch-arm/usr/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2044958859/main.o ../../../../sources/androidapplestreaming/src/main.cpp

${OBJECTDIR}/_ext/2044958859/jniwrapper.o: ../../../../sources/androidapplestreaming/src/jniwrapper.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2044958859
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DNET_KQUEUE -DANDROID -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/androidapplestreaming/include -I../../../../sources/applications/applestreamingclient/include -I../../../../../android/workbench/android-ndk-r4-crystax/build/platforms/android-4/arch-arm/usr/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2044958859/jniwrapper.o ../../../../sources/androidapplestreaming/src/jniwrapper.cpp

${OBJECTDIR}/_ext/2044958859/api.o: ../../../../sources/androidapplestreaming/src/api.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2044958859
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DNET_KQUEUE -DANDROID -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../sources/androidapplestreaming/include -I../../../../sources/applications/applestreamingclient/include -I../../../../../android/workbench/android-ndk-r4-crystax/build/platforms/android-4/arch-arm/usr/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2044958859/api.o ../../../../sources/androidapplestreaming/src/api.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libandroidapplestreaming.dylib

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
