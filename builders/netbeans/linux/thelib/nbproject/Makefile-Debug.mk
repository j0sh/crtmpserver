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
FC=gfortran
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
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomstsc.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomgnre.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tspacketpmt.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomwide.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/configuration/configfile.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom_day.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/tsfileinboundstream.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomstts.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomtkhd.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/basetsappprotocolhandler.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomflla.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom____.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/netio/epoll/iohandlermanager.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomminf.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tspacketpat.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomtrak.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/inboundtsprotocol.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tsadaptationfield.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomstsd.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atommp3.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/netio/epoll/tcpcarrier.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomstps.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomwave.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/appmessagefactory.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/sslprotocol.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomdref.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/baseoutboundstream.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomoles.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomuuid.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom_nam.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomesds.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/timer/basetimerprotocol.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/fileinboundstream.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/netio/epoll/iotimer.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/protocolchainfactory.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/streamdescriptors.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atommvhd.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomtmcd.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomavcc.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tspacketheader.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomhdlr.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomnull.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom_des.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomtrkn.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/basertmpappprotocolhandler.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomstss.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/application/clientapplicationmanager.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tsvideostream.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/basemediadocument.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atommp4a.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/dns/outbounddnsresolverprotocol.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/versionedboxatom.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomcpil.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/amf3serializer.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/buffering/inputbuffer.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tsstreamsmanager.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/application/baseclientapplication.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/tcpprotocol.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomprfl.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/versionedatom.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/mp4document.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomtapt.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/networkinboundstream.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4fileinboundstream.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/baseatom.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/header.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/dns/inbounddnsresolverprotocol.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tssubtitlestream.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomalis.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomwloc.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atombtrt.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomiods.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/baseprotocol.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomdisk.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/rtmpprotocolserializer.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomftyp.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomcolr.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomallf.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tsaudiostream.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomtmpo.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/inboundrtmpprotocol.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/streammanager.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/http/outboundhttpprotocol.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/baseinboundstream.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/sharedobjects/somanager.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/sharedobjects/so.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom_cpy.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomstbl.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomselo.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomdinf.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomsdtp.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomrmra.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomvmhd.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom_too.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/application/baseappprotocolhandler.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/protocolmanager.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/fileinboundstreamtimer.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/netio/epoll/iohandler.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/netio/epoll/tcpacceptor.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atommdhd.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom_alb.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomchan.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomcslg.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom_art.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/buffering/multiconsumerbuffer.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/amf0serializer.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atommdia.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/basertmpprotocol.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/flvinboundstream.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/outboundrtmpprotocol.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomilst.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomavc1.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/neutralstream.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/channel.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/fileoutboundstream.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom_cmt.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomfree.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/boxatom.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/buffering/outputbuffer.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomedts.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atompgap.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomstco.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomudta.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/networkoutboundstream.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomstsz.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/http/basehttpprotocol.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/rtmpeprotocol.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomurl.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atommoov.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomsmhd.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomload.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tsbasestream.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomctts.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atommdat.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomgmhd.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atommeta.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tsprogram.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomfrma.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/basestream.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/http/inboundhttpprotocol.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/lua/luautils.o

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
LDLIBSOPTIONS=-Wl,-rpath ../common/dist/Debug/GNU-Linux-x86 -L../common/dist/Debug/GNU-Linux-x86 -lcommon

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-Linux-x86/libthelib.so

dist/Debug/GNU-Linux-x86/libthelib.so: ../common/dist/Debug/GNU-Linux-x86/libcommon.so

dist/Debug/GNU-Linux-x86/libthelib.so: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${LINK.cc} -shared -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libthelib.so -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomstsc.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomstsc.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomstsc.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomstsc.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomgnre.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomgnre.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomgnre.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomgnre.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tspacketpmt.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/ts/tspacketpmt.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tspacketpmt.o ../../../../sources/thelib/src/protocols/ts/tspacketpmt.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomwide.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomwide.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomwide.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomwide.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/configuration/configfile.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/configuration/configfile.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/configuration
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/configuration/configfile.o ../../../../sources/thelib/src/configuration/configfile.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom_day.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atom_day.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom_day.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atom_day.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/tsfileinboundstream.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/tsfileinboundstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/tsfileinboundstream.o ../../../../sources/thelib/src/protocols/rtmp/streaming/tsfileinboundstream.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomstts.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomstts.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomstts.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomstts.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomtkhd.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomtkhd.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomtkhd.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomtkhd.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/basetsappprotocolhandler.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/ts/basetsappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/basetsappprotocolhandler.o ../../../../sources/thelib/src/protocols/ts/basetsappprotocolhandler.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomflla.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomflla.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomflla.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomflla.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom____.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atom____.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom____.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atom____.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/netio/epoll/iohandlermanager.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/netio/epoll/iohandlermanager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/netio/epoll
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/netio/epoll/iohandlermanager.o ../../../../sources/thelib/src/netio/epoll/iohandlermanager.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomminf.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomminf.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomminf.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomminf.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tspacketpat.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/ts/tspacketpat.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tspacketpat.o ../../../../sources/thelib/src/protocols/ts/tspacketpat.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomtrak.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomtrak.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomtrak.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomtrak.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/inboundtsprotocol.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/ts/inboundtsprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/inboundtsprotocol.o ../../../../sources/thelib/src/protocols/ts/inboundtsprotocol.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tsadaptationfield.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/ts/tsadaptationfield.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tsadaptationfield.o ../../../../sources/thelib/src/protocols/ts/tsadaptationfield.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomstsd.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomstsd.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomstsd.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomstsd.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atommp3.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atommp3.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atommp3.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atommp3.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/netio/epoll/tcpcarrier.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/netio/epoll/tcpcarrier.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/netio/epoll
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/netio/epoll/tcpcarrier.o ../../../../sources/thelib/src/netio/epoll/tcpcarrier.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomstps.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomstps.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomstps.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomstps.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomwave.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomwave.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomwave.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomwave.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/appmessagefactory.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/appmessagefactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/appmessagefactory.o ../../../../sources/thelib/src/protocols/rtmp/appmessagefactory.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/sslprotocol.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/sslprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/sslprotocol.o ../../../../sources/thelib/src/protocols/sslprotocol.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomdref.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomdref.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomdref.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomdref.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/baseoutboundstream.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/baseoutboundstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/baseoutboundstream.o ../../../../sources/thelib/src/protocols/rtmp/streaming/baseoutboundstream.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomoles.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomoles.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomoles.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomoles.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomuuid.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomuuid.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomuuid.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomuuid.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom_nam.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atom_nam.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom_nam.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atom_nam.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomesds.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomesds.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomesds.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomesds.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/timer/basetimerprotocol.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/timer/basetimerprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/timer
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/timer/basetimerprotocol.o ../../../../sources/thelib/src/protocols/timer/basetimerprotocol.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/fileinboundstream.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/fileinboundstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/fileinboundstream.o ../../../../sources/thelib/src/protocols/rtmp/streaming/fileinboundstream.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/netio/epoll/iotimer.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/netio/epoll/iotimer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/netio/epoll
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/netio/epoll/iotimer.o ../../../../sources/thelib/src/netio/epoll/iotimer.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/protocolchainfactory.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/protocolchainfactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/protocolchainfactory.o ../../../../sources/thelib/src/protocols/protocolchainfactory.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/streamdescriptors.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/ts/streamdescriptors.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/streamdescriptors.o ../../../../sources/thelib/src/protocols/ts/streamdescriptors.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atommvhd.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atommvhd.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atommvhd.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atommvhd.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomtmcd.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomtmcd.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomtmcd.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomtmcd.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomavcc.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomavcc.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomavcc.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomavcc.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tspacketheader.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/ts/tspacketheader.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tspacketheader.o ../../../../sources/thelib/src/protocols/ts/tspacketheader.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomhdlr.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomhdlr.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomhdlr.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomhdlr.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomnull.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomnull.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomnull.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomnull.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom_des.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atom_des.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom_des.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atom_des.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomtrkn.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomtrkn.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomtrkn.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomtrkn.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/basertmpappprotocolhandler.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/basertmpappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/basertmpappprotocolhandler.o ../../../../sources/thelib/src/protocols/rtmp/basertmpappprotocolhandler.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomstss.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomstss.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomstss.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomstss.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/application/clientapplicationmanager.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/application/clientapplicationmanager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/application
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/application/clientapplicationmanager.o ../../../../sources/thelib/src/application/clientapplicationmanager.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tsvideostream.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/ts/tsvideostream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tsvideostream.o ../../../../sources/thelib/src/protocols/ts/tsvideostream.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/basemediadocument.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/basemediadocument.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/basemediadocument.o ../../../../sources/thelib/src/protocols/rtmp/streaming/basemediadocument.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atommp4a.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atommp4a.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atommp4a.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atommp4a.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/dns/outbounddnsresolverprotocol.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/dns/outbounddnsresolverprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/dns
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/dns/outbounddnsresolverprotocol.o ../../../../sources/thelib/src/protocols/dns/outbounddnsresolverprotocol.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/versionedboxatom.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/versionedboxatom.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/versionedboxatom.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/versionedboxatom.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomcpil.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomcpil.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomcpil.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomcpil.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/amf3serializer.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/amf3serializer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/amf3serializer.o ../../../../sources/thelib/src/protocols/rtmp/amf3serializer.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/buffering/inputbuffer.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/buffering/inputbuffer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/buffering
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/buffering/inputbuffer.o ../../../../sources/thelib/src/buffering/inputbuffer.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tsstreamsmanager.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/ts/tsstreamsmanager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tsstreamsmanager.o ../../../../sources/thelib/src/protocols/ts/tsstreamsmanager.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/application/baseclientapplication.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/application/baseclientapplication.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/application
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/application/baseclientapplication.o ../../../../sources/thelib/src/application/baseclientapplication.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/tcpprotocol.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/tcpprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/tcpprotocol.o ../../../../sources/thelib/src/protocols/tcpprotocol.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomprfl.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomprfl.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomprfl.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomprfl.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/versionedatom.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/versionedatom.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/versionedatom.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/versionedatom.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/mp4document.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/mp4document.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/mp4document.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/mp4document.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomtapt.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomtapt.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomtapt.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomtapt.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/networkinboundstream.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/networkinboundstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/networkinboundstream.o ../../../../sources/thelib/src/protocols/rtmp/streaming/networkinboundstream.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4fileinboundstream.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4fileinboundstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4fileinboundstream.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4fileinboundstream.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/baseatom.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/baseatom.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/baseatom.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/baseatom.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/header.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/header.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/header.o ../../../../sources/thelib/src/protocols/rtmp/header.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/dns/inbounddnsresolverprotocol.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/dns/inbounddnsresolverprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/dns
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/dns/inbounddnsresolverprotocol.o ../../../../sources/thelib/src/protocols/dns/inbounddnsresolverprotocol.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tssubtitlestream.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/ts/tssubtitlestream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tssubtitlestream.o ../../../../sources/thelib/src/protocols/ts/tssubtitlestream.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomalis.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomalis.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomalis.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomalis.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomwloc.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomwloc.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomwloc.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomwloc.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atombtrt.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atombtrt.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atombtrt.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atombtrt.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomiods.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomiods.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomiods.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomiods.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/baseprotocol.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/baseprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/baseprotocol.o ../../../../sources/thelib/src/protocols/baseprotocol.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomdisk.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomdisk.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomdisk.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomdisk.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/rtmpprotocolserializer.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/rtmpprotocolserializer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/rtmpprotocolserializer.o ../../../../sources/thelib/src/protocols/rtmp/rtmpprotocolserializer.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomftyp.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomftyp.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomftyp.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomftyp.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomcolr.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomcolr.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomcolr.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomcolr.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomallf.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomallf.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomallf.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomallf.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tsaudiostream.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/ts/tsaudiostream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tsaudiostream.o ../../../../sources/thelib/src/protocols/ts/tsaudiostream.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomtmpo.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomtmpo.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomtmpo.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomtmpo.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/inboundrtmpprotocol.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/inboundrtmpprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/inboundrtmpprotocol.o ../../../../sources/thelib/src/protocols/rtmp/inboundrtmpprotocol.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/streammanager.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/streammanager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/streammanager.o ../../../../sources/thelib/src/protocols/rtmp/streaming/streammanager.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/http/outboundhttpprotocol.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/http/outboundhttpprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/http
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/http/outboundhttpprotocol.o ../../../../sources/thelib/src/protocols/http/outboundhttpprotocol.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/baseinboundstream.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/baseinboundstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/baseinboundstream.o ../../../../sources/thelib/src/protocols/rtmp/streaming/baseinboundstream.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/sharedobjects/somanager.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/sharedobjects/somanager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/sharedobjects
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/sharedobjects/somanager.o ../../../../sources/thelib/src/protocols/rtmp/sharedobjects/somanager.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/sharedobjects/so.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/sharedobjects/so.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/sharedobjects
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/sharedobjects/so.o ../../../../sources/thelib/src/protocols/rtmp/sharedobjects/so.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom_cpy.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atom_cpy.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom_cpy.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atom_cpy.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomstbl.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomstbl.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomstbl.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomstbl.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomselo.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomselo.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomselo.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomselo.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomdinf.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomdinf.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomdinf.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomdinf.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomsdtp.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomsdtp.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomsdtp.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomsdtp.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomrmra.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomrmra.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomrmra.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomrmra.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomvmhd.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomvmhd.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomvmhd.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomvmhd.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom_too.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atom_too.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom_too.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atom_too.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/application/baseappprotocolhandler.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/application/baseappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/application
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/application/baseappprotocolhandler.o ../../../../sources/thelib/src/application/baseappprotocolhandler.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/protocolmanager.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/protocolmanager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/protocolmanager.o ../../../../sources/thelib/src/protocols/protocolmanager.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/fileinboundstreamtimer.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/fileinboundstreamtimer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/fileinboundstreamtimer.o ../../../../sources/thelib/src/protocols/rtmp/streaming/fileinboundstreamtimer.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/netio/epoll/iohandler.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/netio/epoll/iohandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/netio/epoll
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/netio/epoll/iohandler.o ../../../../sources/thelib/src/netio/epoll/iohandler.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/netio/epoll/tcpacceptor.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/netio/epoll/tcpacceptor.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/netio/epoll
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/netio/epoll/tcpacceptor.o ../../../../sources/thelib/src/netio/epoll/tcpacceptor.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atommdhd.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atommdhd.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atommdhd.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atommdhd.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom_alb.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atom_alb.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom_alb.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atom_alb.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomchan.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomchan.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomchan.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomchan.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomcslg.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomcslg.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomcslg.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomcslg.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom_art.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atom_art.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom_art.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atom_art.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/buffering/multiconsumerbuffer.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/buffering/multiconsumerbuffer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/buffering
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/buffering/multiconsumerbuffer.o ../../../../sources/thelib/src/buffering/multiconsumerbuffer.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/amf0serializer.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/amf0serializer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/amf0serializer.o ../../../../sources/thelib/src/protocols/rtmp/amf0serializer.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atommdia.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atommdia.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atommdia.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atommdia.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/basertmpprotocol.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/basertmpprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/basertmpprotocol.o ../../../../sources/thelib/src/protocols/rtmp/basertmpprotocol.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/flvinboundstream.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/flvinboundstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/flvinboundstream.o ../../../../sources/thelib/src/protocols/rtmp/streaming/flvinboundstream.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/outboundrtmpprotocol.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/outboundrtmpprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/outboundrtmpprotocol.o ../../../../sources/thelib/src/protocols/rtmp/outboundrtmpprotocol.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomilst.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomilst.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomilst.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomilst.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomavc1.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomavc1.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomavc1.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomavc1.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/neutralstream.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/neutralstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/neutralstream.o ../../../../sources/thelib/src/protocols/rtmp/streaming/neutralstream.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/channel.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/channel.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/channel.o ../../../../sources/thelib/src/protocols/rtmp/channel.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/fileoutboundstream.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/fileoutboundstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/fileoutboundstream.o ../../../../sources/thelib/src/protocols/rtmp/streaming/fileoutboundstream.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom_cmt.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atom_cmt.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atom_cmt.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atom_cmt.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomfree.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomfree.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomfree.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomfree.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/boxatom.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/boxatom.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/boxatom.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/boxatom.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/buffering/outputbuffer.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/buffering/outputbuffer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/buffering
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/buffering/outputbuffer.o ../../../../sources/thelib/src/buffering/outputbuffer.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomedts.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomedts.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomedts.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomedts.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atompgap.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atompgap.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atompgap.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atompgap.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomstco.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomstco.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomstco.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomstco.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomudta.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomudta.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomudta.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomudta.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/networkoutboundstream.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/networkoutboundstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/networkoutboundstream.o ../../../../sources/thelib/src/protocols/rtmp/streaming/networkoutboundstream.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomstsz.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomstsz.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomstsz.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomstsz.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/http/basehttpprotocol.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/http/basehttpprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/http
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/http/basehttpprotocol.o ../../../../sources/thelib/src/protocols/http/basehttpprotocol.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/rtmpeprotocol.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/rtmpeprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/rtmpeprotocol.o ../../../../sources/thelib/src/protocols/rtmp/rtmpeprotocol.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomurl.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomurl.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomurl.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomurl.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atommoov.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atommoov.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atommoov.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atommoov.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomsmhd.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomsmhd.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomsmhd.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomsmhd.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomload.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomload.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomload.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomload.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tsbasestream.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/ts/tsbasestream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tsbasestream.o ../../../../sources/thelib/src/protocols/ts/tsbasestream.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomctts.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomctts.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomctts.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomctts.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atommdat.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atommdat.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atommdat.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atommdat.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomgmhd.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomgmhd.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomgmhd.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomgmhd.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atommeta.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atommeta.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atommeta.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atommeta.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tsprogram.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/ts/tsprogram.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/ts/tsprogram.o ../../../../sources/thelib/src/protocols/ts/tsprogram.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomfrma.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomfrma.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/mp4/atomfrma.o ../../../../sources/thelib/src/protocols/rtmp/streaming/mp4/atomfrma.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/basestream.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/rtmp/streaming/basestream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/rtmp/streaming/basestream.o ../../../../sources/thelib/src/protocols/rtmp/streaming/basestream.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/http/inboundhttpprotocol.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/protocols/http/inboundhttpprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/http
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/protocols/http/inboundhttpprotocol.o ../../../../sources/thelib/src/protocols/http/inboundhttpprotocol.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/lua/luautils.o: nbproject/Makefile-${CND_CONF}.mk ../../../../sources/thelib/src/lua/luautils.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/lua
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -DNET_EPOLL -I../../../../sources/common/include -I../../../../sources/thelib/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/_DOTDOT/sources/thelib/src/lua/luautils.o ../../../../sources/thelib/src/lua/luautils.cpp

# Subprojects
.build-subprojects:
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/libthelib.so

# Subprojects
.clean-subprojects:
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
