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
OBJECTFILES=


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
	"${MAKE}"  -f nbproject/Makefile-Debug.mk ../Debug/GNU-MacOSX/applications/flvplayback/libflvplayback.dylib

../Debug/GNU-MacOSX/applications/flvplayback/libflvplayback.dylib: ../thelib/../Debug/GNU-MacOSX/libthelib.dylib

../Debug/GNU-MacOSX/applications/flvplayback/libflvplayback.dylib: ../common/dist/Debug/GNU-MacOSX/libcommon.dylib

../Debug/GNU-MacOSX/applications/flvplayback/libflvplayback.dylib: ${OBJECTFILES}
	${MKDIR} -p ../Debug/GNU-MacOSX/applications/flvplayback
	${LINK.c} -dynamiclib -install_name libflvplayback.dylib -o ../${CND_CONF}/${CND_PLATFORM}/applications/flvplayback/libflvplayback.dylib -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

# Subprojects
.build-subprojects:
	cd ../thelib && ${MAKE}  -f Makefile.nb CONF=Debug
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} ../Debug/GNU-MacOSX/applications/flvplayback/libflvplayback.dylib

# Subprojects
.clean-subprojects:
	cd ../thelib && ${MAKE}  -f Makefile.nb CONF=Debug clean
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
