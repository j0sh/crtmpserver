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
	${OBJECTDIR}/_ext/406006454/multiconsumerbuffer.o \
	${OBJECTDIR}/_ext/1145168487/misc.o \
	${OBJECTDIR}/_ext/888692313/timersmanager.o \
	${OBJECTDIR}/_ext/1980509653/timegm.o \
	${OBJECTDIR}/_ext/207925497/logger.o \
	${OBJECTDIR}/_ext/1406839728/misc.o \
	${OBJECTDIR}/_ext/1862590273/tinyxmlparser.o \
	${OBJECTDIR}/_ext/1722917885/misc.o \
	${OBJECTDIR}/_ext/207925497/consoleloglocation.o \
	${OBJECTDIR}/_ext/888692313/crypto.o \
	${OBJECTDIR}/_ext/406006454/iobuffer.o \
	${OBJECTDIR}/_ext/888692313/file.o \
	${OBJECTDIR}/_ext/1416996343/misc.o \
	${OBJECTDIR}/_ext/872974544/misc.o \
	${OBJECTDIR}/_ext/207925497/fileloglocation.o \
	${OBJECTDIR}/_ext/888692313/genericfunctionality.o \
	${OBJECTDIR}/_ext/888692313/variant.o \
	${OBJECTDIR}/_ext/399101721/mempool.o \
	${OBJECTDIR}/_ext/207925497/baseloglocation.o \
	${OBJECTDIR}/_ext/1862590273/tinyxmlerror.o \
	${OBJECTDIR}/_ext/1980509653/misc.o \
	${OBJECTDIR}/_ext/1279190767/luautils.o \
	${OBJECTDIR}/_ext/1862590273/tinyxml.o \
	${OBJECTDIR}/_ext/888692313/mmapfile.o


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

${OBJECTDIR}/_ext/406006454/multiconsumerbuffer.o: ../../../../sources/common/src/buffering/multiconsumerbuffer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/406006454
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/406006454/multiconsumerbuffer.o ../../../../sources/common/src/buffering/multiconsumerbuffer.cpp

${OBJECTDIR}/_ext/1145168487/misc.o: ../../../../sources/common/src/platform/solaris/misc.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1145168487
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1145168487/misc.o ../../../../sources/common/src/platform/solaris/misc.cpp

${OBJECTDIR}/_ext/888692313/timersmanager.o: ../../../../sources/common/src/utils/timersmanager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/888692313
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/888692313/timersmanager.o ../../../../sources/common/src/utils/timersmanager.cpp

${OBJECTDIR}/_ext/1980509653/timegm.o: ../../../../sources/common/src/platform/android/timegm.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1980509653
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1980509653/timegm.o ../../../../sources/common/src/platform/android/timegm.cpp

${OBJECTDIR}/_ext/207925497/logger.o: ../../../../sources/common/src/logging/logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/207925497
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/207925497/logger.o ../../../../sources/common/src/logging/logger.cpp

${OBJECTDIR}/_ext/1406839728/misc.o: ../../../../sources/common/src/platform/linux/misc.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1406839728
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1406839728/misc.o ../../../../sources/common/src/platform/linux/misc.cpp

${OBJECTDIR}/_ext/1862590273/tinyxmlparser.o: ../../../../sources/common/src/tinyxml/tinyxmlparser.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1862590273
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1862590273/tinyxmlparser.o ../../../../sources/common/src/tinyxml/tinyxmlparser.cpp

${OBJECTDIR}/_ext/1722917885/misc.o: ../../../../sources/common/src/platform/freebsd/misc.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1722917885
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1722917885/misc.o ../../../../sources/common/src/platform/freebsd/misc.cpp

${OBJECTDIR}/_ext/207925497/consoleloglocation.o: ../../../../sources/common/src/logging/consoleloglocation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/207925497
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/207925497/consoleloglocation.o ../../../../sources/common/src/logging/consoleloglocation.cpp

${OBJECTDIR}/_ext/888692313/crypto.o: ../../../../sources/common/src/utils/crypto.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/888692313
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/888692313/crypto.o ../../../../sources/common/src/utils/crypto.cpp

${OBJECTDIR}/_ext/406006454/iobuffer.o: ../../../../sources/common/src/buffering/iobuffer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/406006454
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/406006454/iobuffer.o ../../../../sources/common/src/buffering/iobuffer.cpp

${OBJECTDIR}/_ext/888692313/file.o: ../../../../sources/common/src/utils/file.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/888692313
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/888692313/file.o ../../../../sources/common/src/utils/file.cpp

${OBJECTDIR}/_ext/1416996343/misc.o: ../../../../sources/common/src/platform/win32/misc.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1416996343
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1416996343/misc.o ../../../../sources/common/src/platform/win32/misc.cpp

${OBJECTDIR}/_ext/872974544/misc.o: ../../../../sources/common/src/platform/osx/misc.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/872974544
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/872974544/misc.o ../../../../sources/common/src/platform/osx/misc.cpp

${OBJECTDIR}/_ext/207925497/fileloglocation.o: ../../../../sources/common/src/logging/fileloglocation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/207925497
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/207925497/fileloglocation.o ../../../../sources/common/src/logging/fileloglocation.cpp

${OBJECTDIR}/_ext/888692313/genericfunctionality.o: ../../../../sources/common/src/utils/genericfunctionality.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/888692313
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/888692313/genericfunctionality.o ../../../../sources/common/src/utils/genericfunctionality.cpp

${OBJECTDIR}/_ext/888692313/variant.o: ../../../../sources/common/src/utils/variant.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/888692313
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/888692313/variant.o ../../../../sources/common/src/utils/variant.cpp

${OBJECTDIR}/_ext/399101721/mempool.o: ../../../../sources/common/src/mempool/mempool.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/399101721
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/399101721/mempool.o ../../../../sources/common/src/mempool/mempool.cpp

${OBJECTDIR}/_ext/207925497/baseloglocation.o: ../../../../sources/common/src/logging/baseloglocation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/207925497
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/207925497/baseloglocation.o ../../../../sources/common/src/logging/baseloglocation.cpp

${OBJECTDIR}/_ext/1862590273/tinyxmlerror.o: ../../../../sources/common/src/tinyxml/tinyxmlerror.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1862590273
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1862590273/tinyxmlerror.o ../../../../sources/common/src/tinyxml/tinyxmlerror.cpp

${OBJECTDIR}/_ext/1980509653/misc.o: ../../../../sources/common/src/platform/android/misc.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1980509653
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1980509653/misc.o ../../../../sources/common/src/platform/android/misc.cpp

${OBJECTDIR}/_ext/1279190767/luautils.o: ../../../../sources/common/src/luautils/luautils.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1279190767
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1279190767/luautils.o ../../../../sources/common/src/luautils/luautils.cpp

${OBJECTDIR}/_ext/1862590273/tinyxml.o: ../../../../sources/common/src/tinyxml/tinyxml.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1862590273
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1862590273/tinyxml.o ../../../../sources/common/src/tinyxml/tinyxml.cpp

${OBJECTDIR}/_ext/888692313/mmapfile.o: ../../../../sources/common/src/utils/mmapfile.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/888692313
	${RM} $@.d
	$(COMPILE.cc) -g -DOSX -DUSE_MEM_POOL -DDEBUG_MEM_POOL -DLITTLE_ENDIAN_SHORT_ALIGNED -I/opt/local/include -I../../../../sources/common/include -I/opt/local/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/888692313/mmapfile.o ../../../../sources/common/src/utils/mmapfile.cpp

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
