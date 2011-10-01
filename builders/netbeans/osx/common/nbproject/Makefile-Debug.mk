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
	${OBJECTDIR}/_ext/674965218/mmapfile.o \
	${OBJECTDIR}/_ext/1980509653/timegm.o \
	${OBJECTDIR}/_ext/1874657879/syslogloglocation.o \
	${OBJECTDIR}/_ext/674965218/variant.o \
	${OBJECTDIR}/_ext/1874657879/fileloglocation.o \
	${OBJECTDIR}/_ext/1874657879/baseloglocation.o \
	${OBJECTDIR}/_ext/420206423/tinyxml.o \
	${OBJECTDIR}/_ext/113024148/iobuffer.o \
	${OBJECTDIR}/_ext/230374975/strptime.o \
	${OBJECTDIR}/_ext/230374975/timegm.o \
	${OBJECTDIR}/_ext/1722917885/freebsdplatform.o \
	${OBJECTDIR}/_ext/1874657879/formatter.o \
	${OBJECTDIR}/_ext/674965218/crypto.o \
	${OBJECTDIR}/_ext/1145168487/timegm.o \
	${OBJECTDIR}/_ext/1874657879/logcatloglocation.o \
	${OBJECTDIR}/_ext/1545610023/dfreebsdplatform.o \
	${OBJECTDIR}/_ext/1874657879/logeventfactory.o \
	${OBJECTDIR}/_ext/230374975/win32platform.o \
	${OBJECTDIR}/_ext/1912657765/openbsdplatform.o \
	${OBJECTDIR}/_ext/674965218/file.o \
	${OBJECTDIR}/_ext/1145168487/solarisplatform.o \
	${OBJECTDIR}/_ext/420206423/tinyxmlparser.o \
	${OBJECTDIR}/_ext/1874657879/consoleloglocation.o \
	${OBJECTDIR}/_ext/872974544/osxplatform.o \
	${OBJECTDIR}/_ext/674965218/timersmanager.o \
	${OBJECTDIR}/_ext/420206423/tinyxmlerror.o \
	${OBJECTDIR}/_ext/1874657879/logger.o \
	${OBJECTDIR}/_ext/714509122/luautils.o \
	${OBJECTDIR}/_ext/674965218/uri.o \
	${OBJECTDIR}/_ext/230374975/strncasecmp.o \
	${OBJECTDIR}/_ext/1998150091/baseplatform.o \
	${OBJECTDIR}/_ext/1267630661/mempool.o \
	${OBJECTDIR}/_ext/1406839728/linuxplatform.o \
	${OBJECTDIR}/_ext/1980509653/androidplatform.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcommon.dylib

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcommon.dylib: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -dynamiclib -install_name libcommon.dylib -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcommon.dylib -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/674965218/mmapfile.o: ../../../../sources/common/src/utils/misc/mmapfile.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/674965218
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/674965218/mmapfile.o ../../../../sources/common/src/utils/misc/mmapfile.cpp

${OBJECTDIR}/_ext/1980509653/timegm.o: ../../../../sources/common/src/platform/android/timegm.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1980509653
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1980509653/timegm.o ../../../../sources/common/src/platform/android/timegm.cpp

${OBJECTDIR}/_ext/1874657879/syslogloglocation.o: ../../../../sources/common/src/utils/logging/syslogloglocation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1874657879
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1874657879/syslogloglocation.o ../../../../sources/common/src/utils/logging/syslogloglocation.cpp

${OBJECTDIR}/_ext/674965218/variant.o: ../../../../sources/common/src/utils/misc/variant.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/674965218
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/674965218/variant.o ../../../../sources/common/src/utils/misc/variant.cpp

${OBJECTDIR}/_ext/1874657879/fileloglocation.o: ../../../../sources/common/src/utils/logging/fileloglocation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1874657879
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1874657879/fileloglocation.o ../../../../sources/common/src/utils/logging/fileloglocation.cpp

${OBJECTDIR}/_ext/1874657879/baseloglocation.o: ../../../../sources/common/src/utils/logging/baseloglocation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1874657879
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1874657879/baseloglocation.o ../../../../sources/common/src/utils/logging/baseloglocation.cpp

${OBJECTDIR}/_ext/420206423/tinyxml.o: ../../../../3rdparty/tinyxml/tinyxml.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/420206423
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/420206423/tinyxml.o ../../../../3rdparty/tinyxml/tinyxml.cpp

${OBJECTDIR}/_ext/113024148/iobuffer.o: ../../../../sources/common/src/utils/buffering/iobuffer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/113024148
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/113024148/iobuffer.o ../../../../sources/common/src/utils/buffering/iobuffer.cpp

${OBJECTDIR}/_ext/230374975/strptime.o: ../../../../sources/common/src/platform/windows/strptime.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/230374975
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/230374975/strptime.o ../../../../sources/common/src/platform/windows/strptime.cpp

${OBJECTDIR}/_ext/230374975/timegm.o: ../../../../sources/common/src/platform/windows/timegm.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/230374975
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/230374975/timegm.o ../../../../sources/common/src/platform/windows/timegm.cpp

${OBJECTDIR}/_ext/1722917885/freebsdplatform.o: ../../../../sources/common/src/platform/freebsd/freebsdplatform.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1722917885
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1722917885/freebsdplatform.o ../../../../sources/common/src/platform/freebsd/freebsdplatform.cpp

${OBJECTDIR}/_ext/1874657879/formatter.o: ../../../../sources/common/src/utils/logging/formatter.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1874657879
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1874657879/formatter.o ../../../../sources/common/src/utils/logging/formatter.cpp

${OBJECTDIR}/_ext/674965218/crypto.o: ../../../../sources/common/src/utils/misc/crypto.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/674965218
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/674965218/crypto.o ../../../../sources/common/src/utils/misc/crypto.cpp

${OBJECTDIR}/_ext/1145168487/timegm.o: ../../../../sources/common/src/platform/solaris/timegm.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1145168487
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1145168487/timegm.o ../../../../sources/common/src/platform/solaris/timegm.cpp

${OBJECTDIR}/_ext/1874657879/logcatloglocation.o: ../../../../sources/common/src/utils/logging/logcatloglocation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1874657879
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1874657879/logcatloglocation.o ../../../../sources/common/src/utils/logging/logcatloglocation.cpp

${OBJECTDIR}/_ext/1545610023/dfreebsdplatform.o: ../../../../sources/common/src/platform/dfreebsd/dfreebsdplatform.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1545610023
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1545610023/dfreebsdplatform.o ../../../../sources/common/src/platform/dfreebsd/dfreebsdplatform.cpp

${OBJECTDIR}/_ext/1874657879/logeventfactory.o: ../../../../sources/common/src/utils/logging/logeventfactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1874657879
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1874657879/logeventfactory.o ../../../../sources/common/src/utils/logging/logeventfactory.cpp

${OBJECTDIR}/_ext/230374975/win32platform.o: ../../../../sources/common/src/platform/windows/win32platform.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/230374975
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/230374975/win32platform.o ../../../../sources/common/src/platform/windows/win32platform.cpp

${OBJECTDIR}/_ext/1912657765/openbsdplatform.o: ../../../../sources/common/src/platform/openbsd/openbsdplatform.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1912657765
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1912657765/openbsdplatform.o ../../../../sources/common/src/platform/openbsd/openbsdplatform.cpp

${OBJECTDIR}/_ext/674965218/file.o: ../../../../sources/common/src/utils/misc/file.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/674965218
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/674965218/file.o ../../../../sources/common/src/utils/misc/file.cpp

${OBJECTDIR}/_ext/1145168487/solarisplatform.o: ../../../../sources/common/src/platform/solaris/solarisplatform.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1145168487
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1145168487/solarisplatform.o ../../../../sources/common/src/platform/solaris/solarisplatform.cpp

${OBJECTDIR}/_ext/420206423/tinyxmlparser.o: ../../../../3rdparty/tinyxml/tinyxmlparser.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/420206423
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/420206423/tinyxmlparser.o ../../../../3rdparty/tinyxml/tinyxmlparser.cpp

${OBJECTDIR}/_ext/1874657879/consoleloglocation.o: ../../../../sources/common/src/utils/logging/consoleloglocation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1874657879
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1874657879/consoleloglocation.o ../../../../sources/common/src/utils/logging/consoleloglocation.cpp

${OBJECTDIR}/_ext/872974544/osxplatform.o: ../../../../sources/common/src/platform/osx/osxplatform.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/872974544
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/872974544/osxplatform.o ../../../../sources/common/src/platform/osx/osxplatform.cpp

${OBJECTDIR}/_ext/674965218/timersmanager.o: ../../../../sources/common/src/utils/misc/timersmanager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/674965218
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/674965218/timersmanager.o ../../../../sources/common/src/utils/misc/timersmanager.cpp

${OBJECTDIR}/_ext/420206423/tinyxmlerror.o: ../../../../3rdparty/tinyxml/tinyxmlerror.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/420206423
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/420206423/tinyxmlerror.o ../../../../3rdparty/tinyxml/tinyxmlerror.cpp

${OBJECTDIR}/_ext/1874657879/logger.o: ../../../../sources/common/src/utils/logging/logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1874657879
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1874657879/logger.o ../../../../sources/common/src/utils/logging/logger.cpp

${OBJECTDIR}/_ext/714509122/luautils.o: ../../../../sources/common/src/utils/lua/luautils.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/714509122
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/714509122/luautils.o ../../../../sources/common/src/utils/lua/luautils.cpp

${OBJECTDIR}/_ext/674965218/uri.o: ../../../../sources/common/src/utils/misc/uri.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/674965218
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/674965218/uri.o ../../../../sources/common/src/utils/misc/uri.cpp

${OBJECTDIR}/_ext/230374975/strncasecmp.o: ../../../../sources/common/src/platform/windows/strncasecmp.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/230374975
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/230374975/strncasecmp.o ../../../../sources/common/src/platform/windows/strncasecmp.cpp

${OBJECTDIR}/_ext/1998150091/baseplatform.o: ../../../../sources/common/src/platform/baseplatform.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1998150091
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1998150091/baseplatform.o ../../../../sources/common/src/platform/baseplatform.cpp

${OBJECTDIR}/_ext/1267630661/mempool.o: ../../../../sources/common/src/utils/mempool/mempool.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1267630661
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1267630661/mempool.o ../../../../sources/common/src/utils/mempool/mempool.cpp

${OBJECTDIR}/_ext/1406839728/linuxplatform.o: ../../../../sources/common/src/platform/linux/linuxplatform.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1406839728
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1406839728/linuxplatform.o ../../../../sources/common/src/platform/linux/linuxplatform.cpp

${OBJECTDIR}/_ext/1980509653/androidplatform.o: ../../../../sources/common/src/platform/android/androidplatform.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1980509653
	${RM} $@.d
	$(COMPILE.cc) -g -DDEBUG_MEM_POOL -DHAS_LUA -DHAS_SYSLOG -DLITTLE_ENDIAN_SHORT_ALIGNED -DOSX -DUSE_MEM_POOL -I/opt/local/include -I../../../../sources/common/include -I../../../../3rdparty/tinyxml -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1980509653/androidplatform.o ../../../../sources/common/src/platform/android/androidplatform.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcommon.dylib

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
