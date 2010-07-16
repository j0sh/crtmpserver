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
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile.nb

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/logging/consoleloglocation.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/utils/variant.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/logging/baseloglocation.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/logging/fileloglocation.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/utils/timerslocation.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/platform/linux/misc.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/logging/logger.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/utils/genericfunctionality.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/utils/dh.o

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
	${MAKE}  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-Linux-x86/libcommon.so

dist/Debug/GNU-Linux-x86/libcommon.so: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${LINK.cc} -shared -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcommon.so -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/logging/consoleloglocation.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/common/src/logging/consoleloglocation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/logging
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/logging/consoleloglocation.o ../../../../sources/common/src/logging/consoleloglocation.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/utils/variant.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/common/src/utils/variant.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/utils
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/utils/variant.o ../../../../sources/common/src/utils/variant.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/logging/baseloglocation.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/common/src/logging/baseloglocation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/logging
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/logging/baseloglocation.o ../../../../sources/common/src/logging/baseloglocation.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/logging/fileloglocation.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/common/src/logging/fileloglocation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/logging
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/logging/fileloglocation.o ../../../../sources/common/src/logging/fileloglocation.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/utils/timerslocation.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/common/src/utils/timerslocation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/utils
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/utils/timerslocation.o ../../../../sources/common/src/utils/timerslocation.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/platform/linux/misc.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/common/src/platform/linux/misc.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/platform/linux
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/platform/linux/misc.o ../../../../sources/common/src/platform/linux/misc.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/logging/logger.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/common/src/logging/logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/logging
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/logging/logger.o ../../../../sources/common/src/logging/logger.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/utils/genericfunctionality.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/common/src/utils/genericfunctionality.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/utils
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/utils/genericfunctionality.o ../../../../sources/common/src/utils/genericfunctionality.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/utils/dh.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/common/src/utils/dh.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/utils
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/common/src/utils/dh.o ../../../../sources/common/src/utils/dh.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/libcommon.so

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
