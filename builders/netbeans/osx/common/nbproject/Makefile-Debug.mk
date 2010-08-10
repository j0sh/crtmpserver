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
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile.nb

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1319505222/androidplatform.o \
	${OBJECTDIR}/_ext/1554816689/variant.o \
	${OBJECTDIR}/_ext/235409714/tinyxmlerror.o \
	${OBJECTDIR}/_ext/891379406/win32platform.o \
	${OBJECTDIR}/_ext/1432319229/multiconsumerbuffer.o \
	${OBJECTDIR}/_ext/1890074490/consoleloglocation.o \
	${OBJECTDIR}/_ext/1554816689/file.o \
	${OBJECTDIR}/_ext/235409714/tinyxml.o \
	${OBJECTDIR}/_ext/1890074490/logger.o \
	${OBJECTDIR}/_ext/1842641242/baseplatform.o \
	${OBJECTDIR}/_ext/188702099/luautils.o \
	${OBJECTDIR}/_ext/1554816689/timersmanager.o \
	${OBJECTDIR}/_ext/2145541121/linuxplatform.o \
	${OBJECTDIR}/_ext/1061913454/freebsdplatform.o \
	${OBJECTDIR}/_ext/1554816689/genericfunctionality.o \
	${OBJECTDIR}/_ext/301680863/osxplatform.o \
	${OBJECTDIR}/_ext/1890074490/logcatloglocation.o \
	${OBJECTDIR}/_ext/1806172918/solarisplatform.o \
	${OBJECTDIR}/_ext/1797865588/mempool.o \
	${OBJECTDIR}/_ext/1890074490/fileloglocation.o \
	${OBJECTDIR}/_ext/1554816689/crypto.o \
	${OBJECTDIR}/_ext/235409714/tinyxmlparser.o \
	${OBJECTDIR}/_ext/1890074490/baseloglocation.o \
	${OBJECTDIR}/_ext/1319505222/timegm.o \
	${OBJECTDIR}/_ext/1554816689/mmapfile.o \
	${OBJECTDIR}/_ext/1432319229/iobuffer.o


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
	"${MAKE}"  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-MacOSX/libcommon.dylib

dist/Debug/GNU-MacOSX/libcommon.dylib: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-MacOSX
	${LINK.cc} -dynamiclib -install_name libcommon.dylib -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcommon.dylib -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/1319505222/androidplatform.o: ../../../../sources/common/src/new/platform/android/androidplatform.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1319505222
	${RM} $@.d
	$(COMPILE.cc) -g -DANDROID -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -DHAS_LUA -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1319505222/androidplatform.o ../../../../sources/common/src/new/platform/android/androidplatform.cpp

${OBJECTDIR}/_ext/1554816689/variant.o: ../../../../sources/common/src/new/utils/misc/variant.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1554816689
	${RM} $@.d
	$(COMPILE.cc) -g -DANDROID -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -DHAS_LUA -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1554816689/variant.o ../../../../sources/common/src/new/utils/misc/variant.cpp

${OBJECTDIR}/_ext/235409714/tinyxmlerror.o: ../../../../sources/common/src/new/utils/tinyxml/tinyxmlerror.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/235409714
	${RM} $@.d
	$(COMPILE.cc) -g -DANDROID -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -DHAS_LUA -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/235409714/tinyxmlerror.o ../../../../sources/common/src/new/utils/tinyxml/tinyxmlerror.cpp

${OBJECTDIR}/_ext/891379406/win32platform.o: ../../../../sources/common/src/new/platform/windows/win32platform.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/891379406
	${RM} $@.d
	$(COMPILE.cc) -g -DANDROID -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -DHAS_LUA -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/891379406/win32platform.o ../../../../sources/common/src/new/platform/windows/win32platform.cpp

${OBJECTDIR}/_ext/1432319229/multiconsumerbuffer.o: ../../../../sources/common/src/new/utils/buffering/multiconsumerbuffer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1432319229
	${RM} $@.d
	$(COMPILE.cc) -g -DANDROID -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -DHAS_LUA -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1432319229/multiconsumerbuffer.o ../../../../sources/common/src/new/utils/buffering/multiconsumerbuffer.cpp

${OBJECTDIR}/_ext/1890074490/consoleloglocation.o: ../../../../sources/common/src/new/utils/logging/consoleloglocation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1890074490
	${RM} $@.d
	$(COMPILE.cc) -g -DANDROID -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -DHAS_LUA -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1890074490/consoleloglocation.o ../../../../sources/common/src/new/utils/logging/consoleloglocation.cpp

${OBJECTDIR}/_ext/1554816689/file.o: ../../../../sources/common/src/new/utils/misc/file.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1554816689
	${RM} $@.d
	$(COMPILE.cc) -g -DANDROID -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -DHAS_LUA -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1554816689/file.o ../../../../sources/common/src/new/utils/misc/file.cpp

${OBJECTDIR}/_ext/235409714/tinyxml.o: ../../../../sources/common/src/new/utils/tinyxml/tinyxml.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/235409714
	${RM} $@.d
	$(COMPILE.cc) -g -DANDROID -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -DHAS_LUA -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/235409714/tinyxml.o ../../../../sources/common/src/new/utils/tinyxml/tinyxml.cpp

${OBJECTDIR}/_ext/1890074490/logger.o: ../../../../sources/common/src/new/utils/logging/logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1890074490
	${RM} $@.d
	$(COMPILE.cc) -g -DANDROID -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -DHAS_LUA -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1890074490/logger.o ../../../../sources/common/src/new/utils/logging/logger.cpp

${OBJECTDIR}/_ext/1842641242/baseplatform.o: ../../../../sources/common/src/new/platform/baseplatform.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1842641242
	${RM} $@.d
	$(COMPILE.cc) -g -DANDROID -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -DHAS_LUA -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1842641242/baseplatform.o ../../../../sources/common/src/new/platform/baseplatform.cpp

${OBJECTDIR}/_ext/188702099/luautils.o: ../../../../sources/common/src/new/utils/lua/luautils.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/188702099
	${RM} $@.d
	$(COMPILE.cc) -g -DANDROID -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -DHAS_LUA -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/188702099/luautils.o ../../../../sources/common/src/new/utils/lua/luautils.cpp

${OBJECTDIR}/_ext/1554816689/timersmanager.o: ../../../../sources/common/src/new/utils/misc/timersmanager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1554816689
	${RM} $@.d
	$(COMPILE.cc) -g -DANDROID -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -DHAS_LUA -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1554816689/timersmanager.o ../../../../sources/common/src/new/utils/misc/timersmanager.cpp

${OBJECTDIR}/_ext/2145541121/linuxplatform.o: ../../../../sources/common/src/new/platform/linux/linuxplatform.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2145541121
	${RM} $@.d
	$(COMPILE.cc) -g -DANDROID -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -DHAS_LUA -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2145541121/linuxplatform.o ../../../../sources/common/src/new/platform/linux/linuxplatform.cpp

${OBJECTDIR}/_ext/1061913454/freebsdplatform.o: ../../../../sources/common/src/new/platform/freebsd/freebsdplatform.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1061913454
	${RM} $@.d
	$(COMPILE.cc) -g -DANDROID -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -DHAS_LUA -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1061913454/freebsdplatform.o ../../../../sources/common/src/new/platform/freebsd/freebsdplatform.cpp

${OBJECTDIR}/_ext/1554816689/genericfunctionality.o: ../../../../sources/common/src/new/utils/misc/genericfunctionality.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1554816689
	${RM} $@.d
	$(COMPILE.cc) -g -DANDROID -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -DHAS_LUA -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1554816689/genericfunctionality.o ../../../../sources/common/src/new/utils/misc/genericfunctionality.cpp

${OBJECTDIR}/_ext/301680863/osxplatform.o: ../../../../sources/common/src/new/platform/osx/osxplatform.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/301680863
	${RM} $@.d
	$(COMPILE.cc) -g -DANDROID -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -DHAS_LUA -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/301680863/osxplatform.o ../../../../sources/common/src/new/platform/osx/osxplatform.cpp

${OBJECTDIR}/_ext/1890074490/logcatloglocation.o: ../../../../sources/common/src/new/utils/logging/logcatloglocation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1890074490
	${RM} $@.d
	$(COMPILE.cc) -g -DANDROID -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -DHAS_LUA -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1890074490/logcatloglocation.o ../../../../sources/common/src/new/utils/logging/logcatloglocation.cpp

${OBJECTDIR}/_ext/1806172918/solarisplatform.o: ../../../../sources/common/src/new/platform/solaris/solarisplatform.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1806172918
	${RM} $@.d
	$(COMPILE.cc) -g -DANDROID -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -DHAS_LUA -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1806172918/solarisplatform.o ../../../../sources/common/src/new/platform/solaris/solarisplatform.cpp

${OBJECTDIR}/_ext/1797865588/mempool.o: ../../../../sources/common/src/new/utils/mempool/mempool.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1797865588
	${RM} $@.d
	$(COMPILE.cc) -g -DANDROID -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -DHAS_LUA -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1797865588/mempool.o ../../../../sources/common/src/new/utils/mempool/mempool.cpp

${OBJECTDIR}/_ext/1890074490/fileloglocation.o: ../../../../sources/common/src/new/utils/logging/fileloglocation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1890074490
	${RM} $@.d
	$(COMPILE.cc) -g -DANDROID -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -DHAS_LUA -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1890074490/fileloglocation.o ../../../../sources/common/src/new/utils/logging/fileloglocation.cpp

${OBJECTDIR}/_ext/1554816689/crypto.o: ../../../../sources/common/src/new/utils/misc/crypto.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1554816689
	${RM} $@.d
	$(COMPILE.cc) -g -DANDROID -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -DHAS_LUA -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1554816689/crypto.o ../../../../sources/common/src/new/utils/misc/crypto.cpp

${OBJECTDIR}/_ext/235409714/tinyxmlparser.o: ../../../../sources/common/src/new/utils/tinyxml/tinyxmlparser.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/235409714
	${RM} $@.d
	$(COMPILE.cc) -g -DANDROID -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -DHAS_LUA -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/235409714/tinyxmlparser.o ../../../../sources/common/src/new/utils/tinyxml/tinyxmlparser.cpp

${OBJECTDIR}/_ext/1890074490/baseloglocation.o: ../../../../sources/common/src/new/utils/logging/baseloglocation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1890074490
	${RM} $@.d
	$(COMPILE.cc) -g -DANDROID -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -DHAS_LUA -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1890074490/baseloglocation.o ../../../../sources/common/src/new/utils/logging/baseloglocation.cpp

${OBJECTDIR}/_ext/1319505222/timegm.o: ../../../../sources/common/src/new/platform/android/timegm.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1319505222
	${RM} $@.d
	$(COMPILE.cc) -g -DANDROID -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -DHAS_LUA -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1319505222/timegm.o ../../../../sources/common/src/new/platform/android/timegm.cpp

${OBJECTDIR}/_ext/1554816689/mmapfile.o: ../../../../sources/common/src/new/utils/misc/mmapfile.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1554816689
	${RM} $@.d
	$(COMPILE.cc) -g -DANDROID -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -DHAS_LUA -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1554816689/mmapfile.o ../../../../sources/common/src/new/utils/misc/mmapfile.cpp

${OBJECTDIR}/_ext/1432319229/iobuffer.o: ../../../../sources/common/src/new/utils/buffering/iobuffer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1432319229
	${RM} $@.d
	$(COMPILE.cc) -g -DANDROID -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -DHAS_LUA -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1432319229/iobuffer.o ../../../../sources/common/src/new/utils/buffering/iobuffer.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-MacOSX/libcommon.dylib

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
