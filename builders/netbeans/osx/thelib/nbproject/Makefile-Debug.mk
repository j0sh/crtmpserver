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
	${OBJECTDIR}/_ext/1210522057/atomstsc.o \
	${OBJECTDIR}/_ext/1124578851/inboundbasecliprotocol.o \
	${OBJECTDIR}/_ext/126235575/inboundnamedpipecarrier.o \
	${OBJECTDIR}/_ext/1919749812/baseprotocolfactory.o \
	${OBJECTDIR}/_ext/1210522057/atomurl.o \
	${OBJECTDIR}/_ext/517913122/tspacketpmt.o \
	${OBJECTDIR}/_ext/31226533/infilertmpstream.o \
	${OBJECTDIR}/_ext/1625581042/basertmfpprotocol.o \
	${OBJECTDIR}/_ext/772217591/inboundnamedpipecarrier.o \
	${OBJECTDIR}/_ext/1215912322/innetrtpstream.o \
	${OBJECTDIR}/_ext/517913122/pidtypes.o \
	${OBJECTDIR}/_ext/1210522057/atomctts.o \
	${OBJECTDIR}/_ext/1210522057/atommfhd.o \
	${OBJECTDIR}/_ext/1636055853/packetqueue.o \
	${OBJECTDIR}/_ext/1636055853/baseoutnetstream.o \
	${OBJECTDIR}/_ext/1054419429/baseconnectivity.o \
	${OBJECTDIR}/_ext/772217591/tcpcarrier.o \
	${OBJECTDIR}/_ext/554535297/tcpcarrier.o \
	${OBJECTDIR}/_ext/502048917/basehttpprotocol.o \
	${OBJECTDIR}/_ext/1210522057/atomwave.o \
	${OBJECTDIR}/_ext/554535297/stdiocarrier.o \
	${OBJECTDIR}/_ext/1124577818/outbounddnsresolverprotocol.o \
	${OBJECTDIR}/_ext/1826042562/xmlvariantprotocol.o \
	${OBJECTDIR}/_ext/501751224/header_le_ba.o \
	${OBJECTDIR}/_ext/1210522057/atommdhd.o \
	${OBJECTDIR}/_ext/126235575/stdiocarrier.o \
	${OBJECTDIR}/_ext/31226533/outnetrtmp4tsstream.o \
	${OBJECTDIR}/_ext/1124578851/http4cliprotocol.o \
	${OBJECTDIR}/_ext/1636055853/nalutypes.o \
	${OBJECTDIR}/_ext/1966546713/basemediadocument.o \
	${OBJECTDIR}/_ext/1124564181/rtcpprotocol.o \
	${OBJECTDIR}/_ext/1210522057/atomstbl.o \
	${OBJECTDIR}/_ext/772217591/iohandlermanager.o \
	${OBJECTDIR}/_ext/1919749812/udpprotocol.o \
	${OBJECTDIR}/_ext/501751224/inboundrtmpsdiscriminatorprotocol.o \
	${OBJECTDIR}/_ext/1896237080/somessagefactory.o \
	${OBJECTDIR}/_ext/1210522057/atomco64.o \
	${OBJECTDIR}/_ext/1210522057/atomtfhd.o \
	${OBJECTDIR}/_ext/501751224/header_be_ba.o \
	${OBJECTDIR}/_ext/501751224/outboundrtmpprotocol.o \
	${OBJECTDIR}/_ext/1210522057/atomavc1.o \
	${OBJECTDIR}/_ext/1124569200/transcoder.o \
	${OBJECTDIR}/_ext/501751224/basertmpappprotocolhandler.o \
	${OBJECTDIR}/_ext/517913122/tspacketpat.o \
	${OBJECTDIR}/_ext/1124578851/basecliappprotocolhandler.o \
	${OBJECTDIR}/_ext/1636055853/baseinfilestream.o \
	${OBJECTDIR}/_ext/1210522057/atomtkhd.o \
	${OBJECTDIR}/_ext/1210522057/atommp4a.o \
	${OBJECTDIR}/_ext/501751224/inboundrtmpprotocol.o \
	${OBJECTDIR}/_ext/2021747320/somanager.o \
	${OBJECTDIR}/_ext/1124563255/outboundsslprotocol.o \
	${OBJECTDIR}/_ext/501751224/inboundhttp4rtmp.o \
	${OBJECTDIR}/_ext/1210523177/mp3media.o \
	${OBJECTDIR}/_ext/31226533/baseoutnetrtmpstream.o \
	${OBJECTDIR}/_ext/501751224/basertmpprotocol.o \
	${OBJECTDIR}/_ext/1773553217/innetliveflvstream.o \
	${OBJECTDIR}/_ext/502048917/basehttpappprotocolhandler.o \
	${OBJECTDIR}/_ext/1210522057/atomavcc.o \
	${OBJECTDIR}/_ext/554535297/udpcarrier.o \
	${OBJECTDIR}/_ext/1636055853/baseoutstream.o \
	${OBJECTDIR}/_ext/1826042562/basevariantappprotocolhandler.o \
	${OBJECTDIR}/_ext/1210522057/ignoredatom.o \
	${OBJECTDIR}/_ext/1625581042/inboundrtmfpprotocol.o \
	${OBJECTDIR}/_ext/1054419429/inboundconnectivity.o \
	${OBJECTDIR}/_ext/1210522057/atommoov.o \
	${OBJECTDIR}/_ext/1124563255/basesslprotocol.o \
	${OBJECTDIR}/_ext/31226533/innetrtmpstream.o \
	${OBJECTDIR}/_ext/373925503/baseclientapplication.o \
	${OBJECTDIR}/_ext/501751224/amf3serializer.o \
	${OBJECTDIR}/_ext/1210522057/atomnull.o \
	${OBJECTDIR}/_ext/126235575/udpcarrier.o \
	${OBJECTDIR}/_ext/1826042562/binvariantprotocol.o \
	${OBJECTDIR}/_ext/1210522057/atomtraf.o \
	${OBJECTDIR}/_ext/1210515272/flvdocument.o \
	${OBJECTDIR}/_ext/1124578851/inboundjsoncliprotocol.o \
	${OBJECTDIR}/_ext/1210522056/mp3document.o \
	${OBJECTDIR}/_ext/1625581042/outboundrtmfpprotocol.o \
	${OBJECTDIR}/_ext/772217591/udpcarrier.o \
	${OBJECTDIR}/_ext/1210522057/atomstss.o \
	${OBJECTDIR}/_ext/517913122/innettsstream.o \
	${OBJECTDIR}/_ext/1210522057/baseatom.o \
	${OBJECTDIR}/_ext/1210522057/atomdref.o \
	${OBJECTDIR}/_ext/772217591/iohandler.o \
	${OBJECTDIR}/_ext/1210522057/mp4document.o \
	${OBJECTDIR}/_ext/1636055853/baseoutfilestream.o \
	${OBJECTDIR}/_ext/772217591/tcpacceptor.o \
	${OBJECTDIR}/_ext/1210522057/atomhdlr.o \
	${OBJECTDIR}/_ext/1896237080/streammessagefactory.o \
	${OBJECTDIR}/_ext/1210522057/versionedboxatom.o \
	${OBJECTDIR}/_ext/126235575/iohandlermanager.o \
	${OBJECTDIR}/_ext/1124564181/basertpappprotocolhandler.o \
	${OBJECTDIR}/_ext/1210522057/atommoof.o \
	${OBJECTDIR}/_ext/1210522057/atomstco.o \
	${OBJECTDIR}/_ext/502048917/outboundhttpprotocol.o \
	${OBJECTDIR}/_ext/1210522057/atommeta.o \
	${OBJECTDIR}/_ext/1896237080/genericmessagefactory.o \
	${OBJECTDIR}/_ext/547604307/baserawhttpstreamappprotocolhandler.o \
	${OBJECTDIR}/_ext/83984615/module.o \
	${OBJECTDIR}/_ext/31226533/outnetrtmp4rtmpstream.o \
	${OBJECTDIR}/_ext/1210522057/atomudta.o \
	${OBJECTDIR}/_ext/1215912322/baseoutnetrtpudpstream.o \
	${OBJECTDIR}/_ext/517913122/inboundtsprotocol.o \
	${OBJECTDIR}/_ext/1210522056/id3parser.o \
	${OBJECTDIR}/_ext/554535297/iohandlermanager.o \
	${OBJECTDIR}/_ext/1210523177/nsvdocument.o \
	${OBJECTDIR}/_ext/1919749812/baseprotocol.o \
	${OBJECTDIR}/_ext/772217591/stdiocarrier.o \
	${OBJECTDIR}/_ext/1210522057/atomstsz.o \
	${OBJECTDIR}/_ext/1210522057/atommdia.o \
	${OBJECTDIR}/_ext/1210522057/atomftyp.o \
	${OBJECTDIR}/_ext/126235575/tcpacceptor.o \
	${OBJECTDIR}/_ext/1210522057/atomtrex.o \
	${OBJECTDIR}/_ext/1636055853/innetrawstream.o \
	${OBJECTDIR}/_ext/1124564181/basertspappprotocolhandler.o \
	${OBJECTDIR}/_ext/501751224/amf0serializer.o \
	${OBJECTDIR}/_ext/1919749812/protocolmanager.o \
	${OBJECTDIR}/_ext/1124564181/sdp.o \
	${OBJECTDIR}/_ext/1210522057/atomvmhd.o \
	${OBJECTDIR}/_ext/1773553217/inboundliveflvprotocol.o \
	${OBJECTDIR}/_ext/1919749812/defaultprotocolfactory.o \
	${OBJECTDIR}/_ext/772217591/iotimer.o \
	${OBJECTDIR}/_ext/501751224/rtmpprotocolserializer.o \
	${OBJECTDIR}/_ext/502048917/httpauthhelper.o \
	${OBJECTDIR}/_ext/1636055853/baseinnetstream.o \
	${OBJECTDIR}/_ext/126235575/tcpcarrier.o \
	${OBJECTDIR}/_ext/1627100354/basetimerprotocol.o \
	${OBJECTDIR}/_ext/1919749812/protocolfactorymanager.o \
	${OBJECTDIR}/_ext/1210522057/atommetafield.o \
	${OBJECTDIR}/_ext/126235575/iotimer.o \
	${OBJECTDIR}/_ext/501751224/monitorrtmpprotocol.o \
	${OBJECTDIR}/_ext/554535297/iotimer.o \
	${OBJECTDIR}/_ext/1636055853/streamcapabilities.o \
	${OBJECTDIR}/_ext/1210522057/atommvex.o \
	${OBJECTDIR}/_ext/1636055853/basestream.o \
	${OBJECTDIR}/_ext/126235575/iohandler.o \
	${OBJECTDIR}/_ext/1124569200/mmsprotocol.o \
	${OBJECTDIR}/_ext/554535297/inboundnamedpipecarrier.o \
	${OBJECTDIR}/_ext/31226533/outfilertmpflvstream.o \
	${OBJECTDIR}/_ext/1896237080/connectionmessagefactory.o \
	${OBJECTDIR}/_ext/1210522057/atomesds.o \
	${OBJECTDIR}/_ext/1124564181/inboundrtpprotocol.o \
	${OBJECTDIR}/_ext/517913122/basetsappprotocolhandler.o \
	${OBJECTDIR}/_ext/1636055853/outnetrawstream.o \
	${OBJECTDIR}/_ext/1919749812/tcpprotocol.o \
	${OBJECTDIR}/_ext/1210522057/atomdata.o \
	${OBJECTDIR}/_ext/1210522057/atomilst.o \
	${OBJECTDIR}/_ext/1124569200/innetmp3stream.o \
	${OBJECTDIR}/_ext/1124564181/rtspprotocol.o \
	${OBJECTDIR}/_ext/1210522057/atomsmhd.o \
	${OBJECTDIR}/_ext/1054419429/outboundconnectivity.o \
	${OBJECTDIR}/_ext/554535297/iohandler.o \
	${OBJECTDIR}/_ext/83984615/configfile.o \
	${OBJECTDIR}/_ext/501751224/header_le_sa.o \
	${OBJECTDIR}/_ext/501751224/rtmpeprotocol.o \
	${OBJECTDIR}/_ext/547604307/inboundrawhttpstreamprotocol.o \
	${OBJECTDIR}/_ext/1826042562/basevariantprotocol.o \
	${OBJECTDIR}/_ext/1210522057/boxatom.o \
	${OBJECTDIR}/_ext/1625581042/basertmfpappprotocolhandler.o \
	${OBJECTDIR}/_ext/1210522057/atomtrak.o \
	${OBJECTDIR}/_ext/2021747320/so.o \
	${OBJECTDIR}/_ext/1636055853/baseinstream.o \
	${OBJECTDIR}/_ext/1124569200/innetaacstream.o \
	${OBJECTDIR}/_ext/1773553217/baseliveflvappprotocolhandler.o \
	${OBJECTDIR}/_ext/1124577818/inbounddnsresolverprotocol.o \
	${OBJECTDIR}/_ext/1210522057/atomtrun.o \
	${OBJECTDIR}/_ext/1210522057/atomstsd.o \
	${OBJECTDIR}/_ext/517913122/streamdescriptors.o \
	${OBJECTDIR}/_ext/1210522057/atomstts.o \
	${OBJECTDIR}/_ext/1210522057/atomminf.o \
	${OBJECTDIR}/_ext/1210522057/atomdinf.o \
	${OBJECTDIR}/_ext/501751224/channel.o \
	${OBJECTDIR}/_ext/554535297/tcpacceptor.o \
	${OBJECTDIR}/_ext/1124563255/inboundsslprotocol.o \
	${OBJECTDIR}/_ext/1636055853/streamsmanager.o \
	${OBJECTDIR}/_ext/1215912322/outnetrtpudph264stream.o \
	${OBJECTDIR}/_ext/502048917/inboundhttpprotocol.o \
	${OBJECTDIR}/_ext/31226533/rtmpstream.o \
	${OBJECTDIR}/_ext/373925503/clientapplicationmanager.o \
	${OBJECTDIR}/_ext/1210522057/atommvhd.o \
	${OBJECTDIR}/_ext/1124569200/basemmsappprotocolhandler.o \
	${OBJECTDIR}/_ext/1210522057/versionedatom.o \
	${OBJECTDIR}/_ext/373925503/baseappprotocolhandler.o


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
LDLIBSOPTIONS=-L../common/dist/Debug/GNU-MacOSX -lcommon

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../${CND_CONF}/${CND_PLATFORM}/libthelib.dylib

../${CND_CONF}/${CND_PLATFORM}/libthelib.dylib: ../common/dist/Debug/GNU-MacOSX/libcommon.dylib

../${CND_CONF}/${CND_PLATFORM}/libthelib.dylib: ${OBJECTFILES}
	${MKDIR} -p ../${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -L/opt/local/lib -llua -lcrypto -lssl -lfetch -dynamiclib -install_name libthelib.dylib -o ../${CND_CONF}/${CND_PLATFORM}/libthelib.dylib -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/1210522057/atomstsc.o: ../../../../sources/thelib/src/mediaformats/mp4/atomstsc.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomstsc.o ../../../../sources/thelib/src/mediaformats/mp4/atomstsc.cpp

${OBJECTDIR}/_ext/1124578851/inboundbasecliprotocol.o: ../../../../sources/thelib/src/protocols/cli/inboundbasecliprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1124578851
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1124578851/inboundbasecliprotocol.o ../../../../sources/thelib/src/protocols/cli/inboundbasecliprotocol.cpp

${OBJECTDIR}/_ext/126235575/inboundnamedpipecarrier.o: ../../../../sources/thelib/src/netio/epoll/inboundnamedpipecarrier.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/126235575
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/126235575/inboundnamedpipecarrier.o ../../../../sources/thelib/src/netio/epoll/inboundnamedpipecarrier.cpp

${OBJECTDIR}/_ext/1919749812/baseprotocolfactory.o: ../../../../sources/thelib/src/protocols/baseprotocolfactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1919749812
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1919749812/baseprotocolfactory.o ../../../../sources/thelib/src/protocols/baseprotocolfactory.cpp

${OBJECTDIR}/_ext/1210522057/atomurl.o: ../../../../sources/thelib/src/mediaformats/mp4/atomurl.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomurl.o ../../../../sources/thelib/src/mediaformats/mp4/atomurl.cpp

${OBJECTDIR}/_ext/517913122/tspacketpmt.o: ../../../../sources/thelib/src/protocols/ts/tspacketpmt.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/517913122
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/517913122/tspacketpmt.o ../../../../sources/thelib/src/protocols/ts/tspacketpmt.cpp

${OBJECTDIR}/_ext/31226533/infilertmpstream.o: ../../../../sources/thelib/src/protocols/rtmp/streaming/infilertmpstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/31226533
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/31226533/infilertmpstream.o ../../../../sources/thelib/src/protocols/rtmp/streaming/infilertmpstream.cpp

${OBJECTDIR}/_ext/1625581042/basertmfpprotocol.o: ../../../../sources/thelib/src/protocols/rtmfp/basertmfpprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1625581042
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1625581042/basertmfpprotocol.o ../../../../sources/thelib/src/protocols/rtmfp/basertmfpprotocol.cpp

${OBJECTDIR}/_ext/772217591/inboundnamedpipecarrier.o: ../../../../sources/thelib/src/netio/select/inboundnamedpipecarrier.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/772217591
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/772217591/inboundnamedpipecarrier.o ../../../../sources/thelib/src/netio/select/inboundnamedpipecarrier.cpp

${OBJECTDIR}/_ext/1215912322/innetrtpstream.o: ../../../../sources/thelib/src/protocols/rtp/streaming/innetrtpstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1215912322
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1215912322/innetrtpstream.o ../../../../sources/thelib/src/protocols/rtp/streaming/innetrtpstream.cpp

${OBJECTDIR}/_ext/517913122/pidtypes.o: ../../../../sources/thelib/src/protocols/ts/pidtypes.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/517913122
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/517913122/pidtypes.o ../../../../sources/thelib/src/protocols/ts/pidtypes.cpp

${OBJECTDIR}/_ext/1210522057/atomctts.o: ../../../../sources/thelib/src/mediaformats/mp4/atomctts.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomctts.o ../../../../sources/thelib/src/mediaformats/mp4/atomctts.cpp

${OBJECTDIR}/_ext/1210522057/atommfhd.o: ../../../../sources/thelib/src/mediaformats/mp4/atommfhd.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atommfhd.o ../../../../sources/thelib/src/mediaformats/mp4/atommfhd.cpp

${OBJECTDIR}/_ext/1636055853/packetqueue.o: ../../../../sources/thelib/src/streaming/packetqueue.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1636055853
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1636055853/packetqueue.o ../../../../sources/thelib/src/streaming/packetqueue.cpp

${OBJECTDIR}/_ext/1636055853/baseoutnetstream.o: ../../../../sources/thelib/src/streaming/baseoutnetstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1636055853
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1636055853/baseoutnetstream.o ../../../../sources/thelib/src/streaming/baseoutnetstream.cpp

${OBJECTDIR}/_ext/1054419429/baseconnectivity.o: ../../../../sources/thelib/src/protocols/rtp/connectivity/baseconnectivity.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1054419429
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1054419429/baseconnectivity.o ../../../../sources/thelib/src/protocols/rtp/connectivity/baseconnectivity.cpp

${OBJECTDIR}/_ext/772217591/tcpcarrier.o: ../../../../sources/thelib/src/netio/select/tcpcarrier.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/772217591
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/772217591/tcpcarrier.o ../../../../sources/thelib/src/netio/select/tcpcarrier.cpp

${OBJECTDIR}/_ext/554535297/tcpcarrier.o: ../../../../sources/thelib/src/netio/kqueue/tcpcarrier.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/554535297
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/554535297/tcpcarrier.o ../../../../sources/thelib/src/netio/kqueue/tcpcarrier.cpp

${OBJECTDIR}/_ext/502048917/basehttpprotocol.o: ../../../../sources/thelib/src/protocols/http/basehttpprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/502048917
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/502048917/basehttpprotocol.o ../../../../sources/thelib/src/protocols/http/basehttpprotocol.cpp

${OBJECTDIR}/_ext/1210522057/atomwave.o: ../../../../sources/thelib/src/mediaformats/mp4/atomwave.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomwave.o ../../../../sources/thelib/src/mediaformats/mp4/atomwave.cpp

${OBJECTDIR}/_ext/554535297/stdiocarrier.o: ../../../../sources/thelib/src/netio/kqueue/stdiocarrier.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/554535297
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/554535297/stdiocarrier.o ../../../../sources/thelib/src/netio/kqueue/stdiocarrier.cpp

${OBJECTDIR}/_ext/1124577818/outbounddnsresolverprotocol.o: ../../../../sources/thelib/src/protocols/dns/outbounddnsresolverprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1124577818
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1124577818/outbounddnsresolverprotocol.o ../../../../sources/thelib/src/protocols/dns/outbounddnsresolverprotocol.cpp

${OBJECTDIR}/_ext/1826042562/xmlvariantprotocol.o: ../../../../sources/thelib/src/protocols/variant/xmlvariantprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1826042562
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1826042562/xmlvariantprotocol.o ../../../../sources/thelib/src/protocols/variant/xmlvariantprotocol.cpp

${OBJECTDIR}/_ext/501751224/header_le_ba.o: ../../../../sources/thelib/src/protocols/rtmp/header_le_ba.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/501751224
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/501751224/header_le_ba.o ../../../../sources/thelib/src/protocols/rtmp/header_le_ba.cpp

${OBJECTDIR}/_ext/1210522057/atommdhd.o: ../../../../sources/thelib/src/mediaformats/mp4/atommdhd.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atommdhd.o ../../../../sources/thelib/src/mediaformats/mp4/atommdhd.cpp

${OBJECTDIR}/_ext/126235575/stdiocarrier.o: ../../../../sources/thelib/src/netio/epoll/stdiocarrier.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/126235575
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/126235575/stdiocarrier.o ../../../../sources/thelib/src/netio/epoll/stdiocarrier.cpp

${OBJECTDIR}/_ext/31226533/outnetrtmp4tsstream.o: ../../../../sources/thelib/src/protocols/rtmp/streaming/outnetrtmp4tsstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/31226533
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/31226533/outnetrtmp4tsstream.o ../../../../sources/thelib/src/protocols/rtmp/streaming/outnetrtmp4tsstream.cpp

${OBJECTDIR}/_ext/1124578851/http4cliprotocol.o: ../../../../sources/thelib/src/protocols/cli/http4cliprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1124578851
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1124578851/http4cliprotocol.o ../../../../sources/thelib/src/protocols/cli/http4cliprotocol.cpp

${OBJECTDIR}/_ext/1636055853/nalutypes.o: ../../../../sources/thelib/src/streaming/nalutypes.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1636055853
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1636055853/nalutypes.o ../../../../sources/thelib/src/streaming/nalutypes.cpp

${OBJECTDIR}/_ext/1966546713/basemediadocument.o: ../../../../sources/thelib/src/mediaformats/basemediadocument.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1966546713
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1966546713/basemediadocument.o ../../../../sources/thelib/src/mediaformats/basemediadocument.cpp

${OBJECTDIR}/_ext/1124564181/rtcpprotocol.o: ../../../../sources/thelib/src/protocols/rtp/rtcpprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1124564181
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1124564181/rtcpprotocol.o ../../../../sources/thelib/src/protocols/rtp/rtcpprotocol.cpp

${OBJECTDIR}/_ext/1210522057/atomstbl.o: ../../../../sources/thelib/src/mediaformats/mp4/atomstbl.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomstbl.o ../../../../sources/thelib/src/mediaformats/mp4/atomstbl.cpp

${OBJECTDIR}/_ext/772217591/iohandlermanager.o: ../../../../sources/thelib/src/netio/select/iohandlermanager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/772217591
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/772217591/iohandlermanager.o ../../../../sources/thelib/src/netio/select/iohandlermanager.cpp

${OBJECTDIR}/_ext/1919749812/udpprotocol.o: ../../../../sources/thelib/src/protocols/udpprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1919749812
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1919749812/udpprotocol.o ../../../../sources/thelib/src/protocols/udpprotocol.cpp

${OBJECTDIR}/_ext/501751224/inboundrtmpsdiscriminatorprotocol.o: ../../../../sources/thelib/src/protocols/rtmp/inboundrtmpsdiscriminatorprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/501751224
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/501751224/inboundrtmpsdiscriminatorprotocol.o ../../../../sources/thelib/src/protocols/rtmp/inboundrtmpsdiscriminatorprotocol.cpp

${OBJECTDIR}/_ext/1896237080/somessagefactory.o: ../../../../sources/thelib/src/protocols/rtmp/messagefactories/somessagefactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1896237080
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1896237080/somessagefactory.o ../../../../sources/thelib/src/protocols/rtmp/messagefactories/somessagefactory.cpp

${OBJECTDIR}/_ext/1210522057/atomco64.o: ../../../../sources/thelib/src/mediaformats/mp4/atomco64.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomco64.o ../../../../sources/thelib/src/mediaformats/mp4/atomco64.cpp

${OBJECTDIR}/_ext/1210522057/atomtfhd.o: ../../../../sources/thelib/src/mediaformats/mp4/atomtfhd.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomtfhd.o ../../../../sources/thelib/src/mediaformats/mp4/atomtfhd.cpp

${OBJECTDIR}/_ext/501751224/header_be_ba.o: ../../../../sources/thelib/src/protocols/rtmp/header_be_ba.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/501751224
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/501751224/header_be_ba.o ../../../../sources/thelib/src/protocols/rtmp/header_be_ba.cpp

${OBJECTDIR}/_ext/501751224/outboundrtmpprotocol.o: ../../../../sources/thelib/src/protocols/rtmp/outboundrtmpprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/501751224
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/501751224/outboundrtmpprotocol.o ../../../../sources/thelib/src/protocols/rtmp/outboundrtmpprotocol.cpp

${OBJECTDIR}/_ext/1210522057/atomavc1.o: ../../../../sources/thelib/src/mediaformats/mp4/atomavc1.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomavc1.o ../../../../sources/thelib/src/mediaformats/mp4/atomavc1.cpp

${OBJECTDIR}/_ext/1124569200/transcoder.o: ../../../../sources/thelib/src/protocols/mms/transcoder.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1124569200
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1124569200/transcoder.o ../../../../sources/thelib/src/protocols/mms/transcoder.cpp

${OBJECTDIR}/_ext/501751224/basertmpappprotocolhandler.o: ../../../../sources/thelib/src/protocols/rtmp/basertmpappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/501751224
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/501751224/basertmpappprotocolhandler.o ../../../../sources/thelib/src/protocols/rtmp/basertmpappprotocolhandler.cpp

${OBJECTDIR}/_ext/517913122/tspacketpat.o: ../../../../sources/thelib/src/protocols/ts/tspacketpat.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/517913122
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/517913122/tspacketpat.o ../../../../sources/thelib/src/protocols/ts/tspacketpat.cpp

${OBJECTDIR}/_ext/1124578851/basecliappprotocolhandler.o: ../../../../sources/thelib/src/protocols/cli/basecliappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1124578851
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1124578851/basecliappprotocolhandler.o ../../../../sources/thelib/src/protocols/cli/basecliappprotocolhandler.cpp

${OBJECTDIR}/_ext/1636055853/baseinfilestream.o: ../../../../sources/thelib/src/streaming/baseinfilestream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1636055853
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1636055853/baseinfilestream.o ../../../../sources/thelib/src/streaming/baseinfilestream.cpp

${OBJECTDIR}/_ext/1210522057/atomtkhd.o: ../../../../sources/thelib/src/mediaformats/mp4/atomtkhd.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomtkhd.o ../../../../sources/thelib/src/mediaformats/mp4/atomtkhd.cpp

${OBJECTDIR}/_ext/1210522057/atommp4a.o: ../../../../sources/thelib/src/mediaformats/mp4/atommp4a.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atommp4a.o ../../../../sources/thelib/src/mediaformats/mp4/atommp4a.cpp

${OBJECTDIR}/_ext/501751224/inboundrtmpprotocol.o: ../../../../sources/thelib/src/protocols/rtmp/inboundrtmpprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/501751224
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/501751224/inboundrtmpprotocol.o ../../../../sources/thelib/src/protocols/rtmp/inboundrtmpprotocol.cpp

${OBJECTDIR}/_ext/2021747320/somanager.o: ../../../../sources/thelib/src/protocols/rtmp/sharedobjects/somanager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2021747320
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2021747320/somanager.o ../../../../sources/thelib/src/protocols/rtmp/sharedobjects/somanager.cpp

${OBJECTDIR}/_ext/1124563255/outboundsslprotocol.o: ../../../../sources/thelib/src/protocols/ssl/outboundsslprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1124563255
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1124563255/outboundsslprotocol.o ../../../../sources/thelib/src/protocols/ssl/outboundsslprotocol.cpp

${OBJECTDIR}/_ext/501751224/inboundhttp4rtmp.o: ../../../../sources/thelib/src/protocols/rtmp/inboundhttp4rtmp.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/501751224
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/501751224/inboundhttp4rtmp.o ../../../../sources/thelib/src/protocols/rtmp/inboundhttp4rtmp.cpp

${OBJECTDIR}/_ext/1210523177/mp3media.o: ../../../../sources/thelib/src/mediaformats/nsv/mp3media.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210523177
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210523177/mp3media.o ../../../../sources/thelib/src/mediaformats/nsv/mp3media.cpp

${OBJECTDIR}/_ext/31226533/baseoutnetrtmpstream.o: ../../../../sources/thelib/src/protocols/rtmp/streaming/baseoutnetrtmpstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/31226533
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/31226533/baseoutnetrtmpstream.o ../../../../sources/thelib/src/protocols/rtmp/streaming/baseoutnetrtmpstream.cpp

${OBJECTDIR}/_ext/501751224/basertmpprotocol.o: ../../../../sources/thelib/src/protocols/rtmp/basertmpprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/501751224
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/501751224/basertmpprotocol.o ../../../../sources/thelib/src/protocols/rtmp/basertmpprotocol.cpp

${OBJECTDIR}/_ext/1773553217/innetliveflvstream.o: ../../../../sources/thelib/src/protocols/liveflv/innetliveflvstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1773553217
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1773553217/innetliveflvstream.o ../../../../sources/thelib/src/protocols/liveflv/innetliveflvstream.cpp

${OBJECTDIR}/_ext/502048917/basehttpappprotocolhandler.o: ../../../../sources/thelib/src/protocols/http/basehttpappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/502048917
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/502048917/basehttpappprotocolhandler.o ../../../../sources/thelib/src/protocols/http/basehttpappprotocolhandler.cpp

${OBJECTDIR}/_ext/1210522057/atomavcc.o: ../../../../sources/thelib/src/mediaformats/mp4/atomavcc.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomavcc.o ../../../../sources/thelib/src/mediaformats/mp4/atomavcc.cpp

${OBJECTDIR}/_ext/554535297/udpcarrier.o: ../../../../sources/thelib/src/netio/kqueue/udpcarrier.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/554535297
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/554535297/udpcarrier.o ../../../../sources/thelib/src/netio/kqueue/udpcarrier.cpp

${OBJECTDIR}/_ext/1636055853/baseoutstream.o: ../../../../sources/thelib/src/streaming/baseoutstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1636055853
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1636055853/baseoutstream.o ../../../../sources/thelib/src/streaming/baseoutstream.cpp

${OBJECTDIR}/_ext/1826042562/basevariantappprotocolhandler.o: ../../../../sources/thelib/src/protocols/variant/basevariantappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1826042562
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1826042562/basevariantappprotocolhandler.o ../../../../sources/thelib/src/protocols/variant/basevariantappprotocolhandler.cpp

${OBJECTDIR}/_ext/1210522057/ignoredatom.o: ../../../../sources/thelib/src/mediaformats/mp4/ignoredatom.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/ignoredatom.o ../../../../sources/thelib/src/mediaformats/mp4/ignoredatom.cpp

${OBJECTDIR}/_ext/1625581042/inboundrtmfpprotocol.o: ../../../../sources/thelib/src/protocols/rtmfp/inboundrtmfpprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1625581042
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1625581042/inboundrtmfpprotocol.o ../../../../sources/thelib/src/protocols/rtmfp/inboundrtmfpprotocol.cpp

${OBJECTDIR}/_ext/1054419429/inboundconnectivity.o: ../../../../sources/thelib/src/protocols/rtp/connectivity/inboundconnectivity.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1054419429
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1054419429/inboundconnectivity.o ../../../../sources/thelib/src/protocols/rtp/connectivity/inboundconnectivity.cpp

${OBJECTDIR}/_ext/1210522057/atommoov.o: ../../../../sources/thelib/src/mediaformats/mp4/atommoov.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atommoov.o ../../../../sources/thelib/src/mediaformats/mp4/atommoov.cpp

${OBJECTDIR}/_ext/1124563255/basesslprotocol.o: ../../../../sources/thelib/src/protocols/ssl/basesslprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1124563255
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1124563255/basesslprotocol.o ../../../../sources/thelib/src/protocols/ssl/basesslprotocol.cpp

${OBJECTDIR}/_ext/31226533/innetrtmpstream.o: ../../../../sources/thelib/src/protocols/rtmp/streaming/innetrtmpstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/31226533
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/31226533/innetrtmpstream.o ../../../../sources/thelib/src/protocols/rtmp/streaming/innetrtmpstream.cpp

${OBJECTDIR}/_ext/373925503/baseclientapplication.o: ../../../../sources/thelib/src/application/baseclientapplication.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/373925503
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/373925503/baseclientapplication.o ../../../../sources/thelib/src/application/baseclientapplication.cpp

${OBJECTDIR}/_ext/501751224/amf3serializer.o: ../../../../sources/thelib/src/protocols/rtmp/amf3serializer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/501751224
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/501751224/amf3serializer.o ../../../../sources/thelib/src/protocols/rtmp/amf3serializer.cpp

${OBJECTDIR}/_ext/1210522057/atomnull.o: ../../../../sources/thelib/src/mediaformats/mp4/atomnull.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomnull.o ../../../../sources/thelib/src/mediaformats/mp4/atomnull.cpp

${OBJECTDIR}/_ext/126235575/udpcarrier.o: ../../../../sources/thelib/src/netio/epoll/udpcarrier.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/126235575
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/126235575/udpcarrier.o ../../../../sources/thelib/src/netio/epoll/udpcarrier.cpp

${OBJECTDIR}/_ext/1826042562/binvariantprotocol.o: ../../../../sources/thelib/src/protocols/variant/binvariantprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1826042562
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1826042562/binvariantprotocol.o ../../../../sources/thelib/src/protocols/variant/binvariantprotocol.cpp

${OBJECTDIR}/_ext/1210522057/atomtraf.o: ../../../../sources/thelib/src/mediaformats/mp4/atomtraf.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomtraf.o ../../../../sources/thelib/src/mediaformats/mp4/atomtraf.cpp

${OBJECTDIR}/_ext/1210515272/flvdocument.o: ../../../../sources/thelib/src/mediaformats/flv/flvdocument.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210515272
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210515272/flvdocument.o ../../../../sources/thelib/src/mediaformats/flv/flvdocument.cpp

${OBJECTDIR}/_ext/1124578851/inboundjsoncliprotocol.o: ../../../../sources/thelib/src/protocols/cli/inboundjsoncliprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1124578851
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1124578851/inboundjsoncliprotocol.o ../../../../sources/thelib/src/protocols/cli/inboundjsoncliprotocol.cpp

${OBJECTDIR}/_ext/1210522056/mp3document.o: ../../../../sources/thelib/src/mediaformats/mp3/mp3document.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522056
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522056/mp3document.o ../../../../sources/thelib/src/mediaformats/mp3/mp3document.cpp

${OBJECTDIR}/_ext/1625581042/outboundrtmfpprotocol.o: ../../../../sources/thelib/src/protocols/rtmfp/outboundrtmfpprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1625581042
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1625581042/outboundrtmfpprotocol.o ../../../../sources/thelib/src/protocols/rtmfp/outboundrtmfpprotocol.cpp

${OBJECTDIR}/_ext/772217591/udpcarrier.o: ../../../../sources/thelib/src/netio/select/udpcarrier.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/772217591
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/772217591/udpcarrier.o ../../../../sources/thelib/src/netio/select/udpcarrier.cpp

${OBJECTDIR}/_ext/1210522057/atomstss.o: ../../../../sources/thelib/src/mediaformats/mp4/atomstss.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomstss.o ../../../../sources/thelib/src/mediaformats/mp4/atomstss.cpp

${OBJECTDIR}/_ext/517913122/innettsstream.o: ../../../../sources/thelib/src/protocols/ts/innettsstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/517913122
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/517913122/innettsstream.o ../../../../sources/thelib/src/protocols/ts/innettsstream.cpp

${OBJECTDIR}/_ext/1210522057/baseatom.o: ../../../../sources/thelib/src/mediaformats/mp4/baseatom.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/baseatom.o ../../../../sources/thelib/src/mediaformats/mp4/baseatom.cpp

${OBJECTDIR}/_ext/1210522057/atomdref.o: ../../../../sources/thelib/src/mediaformats/mp4/atomdref.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomdref.o ../../../../sources/thelib/src/mediaformats/mp4/atomdref.cpp

${OBJECTDIR}/_ext/772217591/iohandler.o: ../../../../sources/thelib/src/netio/select/iohandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/772217591
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/772217591/iohandler.o ../../../../sources/thelib/src/netio/select/iohandler.cpp

${OBJECTDIR}/_ext/1210522057/mp4document.o: ../../../../sources/thelib/src/mediaformats/mp4/mp4document.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/mp4document.o ../../../../sources/thelib/src/mediaformats/mp4/mp4document.cpp

${OBJECTDIR}/_ext/1636055853/baseoutfilestream.o: ../../../../sources/thelib/src/streaming/baseoutfilestream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1636055853
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1636055853/baseoutfilestream.o ../../../../sources/thelib/src/streaming/baseoutfilestream.cpp

${OBJECTDIR}/_ext/772217591/tcpacceptor.o: ../../../../sources/thelib/src/netio/select/tcpacceptor.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/772217591
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/772217591/tcpacceptor.o ../../../../sources/thelib/src/netio/select/tcpacceptor.cpp

${OBJECTDIR}/_ext/1210522057/atomhdlr.o: ../../../../sources/thelib/src/mediaformats/mp4/atomhdlr.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomhdlr.o ../../../../sources/thelib/src/mediaformats/mp4/atomhdlr.cpp

${OBJECTDIR}/_ext/1896237080/streammessagefactory.o: ../../../../sources/thelib/src/protocols/rtmp/messagefactories/streammessagefactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1896237080
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1896237080/streammessagefactory.o ../../../../sources/thelib/src/protocols/rtmp/messagefactories/streammessagefactory.cpp

${OBJECTDIR}/_ext/1210522057/versionedboxatom.o: ../../../../sources/thelib/src/mediaformats/mp4/versionedboxatom.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/versionedboxatom.o ../../../../sources/thelib/src/mediaformats/mp4/versionedboxatom.cpp

${OBJECTDIR}/_ext/126235575/iohandlermanager.o: ../../../../sources/thelib/src/netio/epoll/iohandlermanager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/126235575
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/126235575/iohandlermanager.o ../../../../sources/thelib/src/netio/epoll/iohandlermanager.cpp

${OBJECTDIR}/_ext/1124564181/basertpappprotocolhandler.o: ../../../../sources/thelib/src/protocols/rtp/basertpappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1124564181
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1124564181/basertpappprotocolhandler.o ../../../../sources/thelib/src/protocols/rtp/basertpappprotocolhandler.cpp

${OBJECTDIR}/_ext/1210522057/atommoof.o: ../../../../sources/thelib/src/mediaformats/mp4/atommoof.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atommoof.o ../../../../sources/thelib/src/mediaformats/mp4/atommoof.cpp

${OBJECTDIR}/_ext/1210522057/atomstco.o: ../../../../sources/thelib/src/mediaformats/mp4/atomstco.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomstco.o ../../../../sources/thelib/src/mediaformats/mp4/atomstco.cpp

${OBJECTDIR}/_ext/502048917/outboundhttpprotocol.o: ../../../../sources/thelib/src/protocols/http/outboundhttpprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/502048917
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/502048917/outboundhttpprotocol.o ../../../../sources/thelib/src/protocols/http/outboundhttpprotocol.cpp

${OBJECTDIR}/_ext/1210522057/atommeta.o: ../../../../sources/thelib/src/mediaformats/mp4/atommeta.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atommeta.o ../../../../sources/thelib/src/mediaformats/mp4/atommeta.cpp

${OBJECTDIR}/_ext/1896237080/genericmessagefactory.o: ../../../../sources/thelib/src/protocols/rtmp/messagefactories/genericmessagefactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1896237080
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1896237080/genericmessagefactory.o ../../../../sources/thelib/src/protocols/rtmp/messagefactories/genericmessagefactory.cpp

${OBJECTDIR}/_ext/547604307/baserawhttpstreamappprotocolhandler.o: ../../../../sources/thelib/src/protocols/rawhttpstream/baserawhttpstreamappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/547604307
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/547604307/baserawhttpstreamappprotocolhandler.o ../../../../sources/thelib/src/protocols/rawhttpstream/baserawhttpstreamappprotocolhandler.cpp

${OBJECTDIR}/_ext/83984615/module.o: ../../../../sources/thelib/src/configuration/module.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/83984615
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/83984615/module.o ../../../../sources/thelib/src/configuration/module.cpp

${OBJECTDIR}/_ext/31226533/outnetrtmp4rtmpstream.o: ../../../../sources/thelib/src/protocols/rtmp/streaming/outnetrtmp4rtmpstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/31226533
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/31226533/outnetrtmp4rtmpstream.o ../../../../sources/thelib/src/protocols/rtmp/streaming/outnetrtmp4rtmpstream.cpp

${OBJECTDIR}/_ext/1210522057/atomudta.o: ../../../../sources/thelib/src/mediaformats/mp4/atomudta.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomudta.o ../../../../sources/thelib/src/mediaformats/mp4/atomudta.cpp

${OBJECTDIR}/_ext/1215912322/baseoutnetrtpudpstream.o: ../../../../sources/thelib/src/protocols/rtp/streaming/baseoutnetrtpudpstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1215912322
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1215912322/baseoutnetrtpudpstream.o ../../../../sources/thelib/src/protocols/rtp/streaming/baseoutnetrtpudpstream.cpp

${OBJECTDIR}/_ext/517913122/inboundtsprotocol.o: ../../../../sources/thelib/src/protocols/ts/inboundtsprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/517913122
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/517913122/inboundtsprotocol.o ../../../../sources/thelib/src/protocols/ts/inboundtsprotocol.cpp

${OBJECTDIR}/_ext/1210522056/id3parser.o: ../../../../sources/thelib/src/mediaformats/mp3/id3parser.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522056
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522056/id3parser.o ../../../../sources/thelib/src/mediaformats/mp3/id3parser.cpp

${OBJECTDIR}/_ext/554535297/iohandlermanager.o: ../../../../sources/thelib/src/netio/kqueue/iohandlermanager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/554535297
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/554535297/iohandlermanager.o ../../../../sources/thelib/src/netio/kqueue/iohandlermanager.cpp

${OBJECTDIR}/_ext/1210523177/nsvdocument.o: ../../../../sources/thelib/src/mediaformats/nsv/nsvdocument.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210523177
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210523177/nsvdocument.o ../../../../sources/thelib/src/mediaformats/nsv/nsvdocument.cpp

${OBJECTDIR}/_ext/1919749812/baseprotocol.o: ../../../../sources/thelib/src/protocols/baseprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1919749812
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1919749812/baseprotocol.o ../../../../sources/thelib/src/protocols/baseprotocol.cpp

${OBJECTDIR}/_ext/772217591/stdiocarrier.o: ../../../../sources/thelib/src/netio/select/stdiocarrier.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/772217591
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/772217591/stdiocarrier.o ../../../../sources/thelib/src/netio/select/stdiocarrier.cpp

${OBJECTDIR}/_ext/1210522057/atomstsz.o: ../../../../sources/thelib/src/mediaformats/mp4/atomstsz.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomstsz.o ../../../../sources/thelib/src/mediaformats/mp4/atomstsz.cpp

${OBJECTDIR}/_ext/1210522057/atommdia.o: ../../../../sources/thelib/src/mediaformats/mp4/atommdia.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atommdia.o ../../../../sources/thelib/src/mediaformats/mp4/atommdia.cpp

${OBJECTDIR}/_ext/1210522057/atomftyp.o: ../../../../sources/thelib/src/mediaformats/mp4/atomftyp.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomftyp.o ../../../../sources/thelib/src/mediaformats/mp4/atomftyp.cpp

${OBJECTDIR}/_ext/126235575/tcpacceptor.o: ../../../../sources/thelib/src/netio/epoll/tcpacceptor.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/126235575
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/126235575/tcpacceptor.o ../../../../sources/thelib/src/netio/epoll/tcpacceptor.cpp

${OBJECTDIR}/_ext/1210522057/atomtrex.o: ../../../../sources/thelib/src/mediaformats/mp4/atomtrex.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomtrex.o ../../../../sources/thelib/src/mediaformats/mp4/atomtrex.cpp

${OBJECTDIR}/_ext/1636055853/innetrawstream.o: ../../../../sources/thelib/src/streaming/innetrawstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1636055853
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1636055853/innetrawstream.o ../../../../sources/thelib/src/streaming/innetrawstream.cpp

${OBJECTDIR}/_ext/1124564181/basertspappprotocolhandler.o: ../../../../sources/thelib/src/protocols/rtp/basertspappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1124564181
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1124564181/basertspappprotocolhandler.o ../../../../sources/thelib/src/protocols/rtp/basertspappprotocolhandler.cpp

${OBJECTDIR}/_ext/501751224/amf0serializer.o: ../../../../sources/thelib/src/protocols/rtmp/amf0serializer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/501751224
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/501751224/amf0serializer.o ../../../../sources/thelib/src/protocols/rtmp/amf0serializer.cpp

${OBJECTDIR}/_ext/1919749812/protocolmanager.o: ../../../../sources/thelib/src/protocols/protocolmanager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1919749812
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1919749812/protocolmanager.o ../../../../sources/thelib/src/protocols/protocolmanager.cpp

${OBJECTDIR}/_ext/1124564181/sdp.o: ../../../../sources/thelib/src/protocols/rtp/sdp.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1124564181
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1124564181/sdp.o ../../../../sources/thelib/src/protocols/rtp/sdp.cpp

${OBJECTDIR}/_ext/1210522057/atomvmhd.o: ../../../../sources/thelib/src/mediaformats/mp4/atomvmhd.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomvmhd.o ../../../../sources/thelib/src/mediaformats/mp4/atomvmhd.cpp

${OBJECTDIR}/_ext/1773553217/inboundliveflvprotocol.o: ../../../../sources/thelib/src/protocols/liveflv/inboundliveflvprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1773553217
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1773553217/inboundliveflvprotocol.o ../../../../sources/thelib/src/protocols/liveflv/inboundliveflvprotocol.cpp

${OBJECTDIR}/_ext/1919749812/defaultprotocolfactory.o: ../../../../sources/thelib/src/protocols/defaultprotocolfactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1919749812
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1919749812/defaultprotocolfactory.o ../../../../sources/thelib/src/protocols/defaultprotocolfactory.cpp

${OBJECTDIR}/_ext/772217591/iotimer.o: ../../../../sources/thelib/src/netio/select/iotimer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/772217591
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/772217591/iotimer.o ../../../../sources/thelib/src/netio/select/iotimer.cpp

${OBJECTDIR}/_ext/501751224/rtmpprotocolserializer.o: ../../../../sources/thelib/src/protocols/rtmp/rtmpprotocolserializer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/501751224
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/501751224/rtmpprotocolserializer.o ../../../../sources/thelib/src/protocols/rtmp/rtmpprotocolserializer.cpp

${OBJECTDIR}/_ext/502048917/httpauthhelper.o: ../../../../sources/thelib/src/protocols/http/httpauthhelper.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/502048917
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/502048917/httpauthhelper.o ../../../../sources/thelib/src/protocols/http/httpauthhelper.cpp

${OBJECTDIR}/_ext/1636055853/baseinnetstream.o: ../../../../sources/thelib/src/streaming/baseinnetstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1636055853
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1636055853/baseinnetstream.o ../../../../sources/thelib/src/streaming/baseinnetstream.cpp

${OBJECTDIR}/_ext/126235575/tcpcarrier.o: ../../../../sources/thelib/src/netio/epoll/tcpcarrier.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/126235575
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/126235575/tcpcarrier.o ../../../../sources/thelib/src/netio/epoll/tcpcarrier.cpp

${OBJECTDIR}/_ext/1627100354/basetimerprotocol.o: ../../../../sources/thelib/src/protocols/timer/basetimerprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1627100354
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1627100354/basetimerprotocol.o ../../../../sources/thelib/src/protocols/timer/basetimerprotocol.cpp

${OBJECTDIR}/_ext/1919749812/protocolfactorymanager.o: ../../../../sources/thelib/src/protocols/protocolfactorymanager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1919749812
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1919749812/protocolfactorymanager.o ../../../../sources/thelib/src/protocols/protocolfactorymanager.cpp

${OBJECTDIR}/_ext/1210522057/atommetafield.o: ../../../../sources/thelib/src/mediaformats/mp4/atommetafield.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atommetafield.o ../../../../sources/thelib/src/mediaformats/mp4/atommetafield.cpp

${OBJECTDIR}/_ext/126235575/iotimer.o: ../../../../sources/thelib/src/netio/epoll/iotimer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/126235575
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/126235575/iotimer.o ../../../../sources/thelib/src/netio/epoll/iotimer.cpp

${OBJECTDIR}/_ext/501751224/monitorrtmpprotocol.o: ../../../../sources/thelib/src/protocols/rtmp/monitorrtmpprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/501751224
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/501751224/monitorrtmpprotocol.o ../../../../sources/thelib/src/protocols/rtmp/monitorrtmpprotocol.cpp

${OBJECTDIR}/_ext/554535297/iotimer.o: ../../../../sources/thelib/src/netio/kqueue/iotimer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/554535297
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/554535297/iotimer.o ../../../../sources/thelib/src/netio/kqueue/iotimer.cpp

${OBJECTDIR}/_ext/1636055853/streamcapabilities.o: ../../../../sources/thelib/src/streaming/streamcapabilities.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1636055853
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1636055853/streamcapabilities.o ../../../../sources/thelib/src/streaming/streamcapabilities.cpp

${OBJECTDIR}/_ext/1210522057/atommvex.o: ../../../../sources/thelib/src/mediaformats/mp4/atommvex.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atommvex.o ../../../../sources/thelib/src/mediaformats/mp4/atommvex.cpp

${OBJECTDIR}/_ext/1636055853/basestream.o: ../../../../sources/thelib/src/streaming/basestream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1636055853
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1636055853/basestream.o ../../../../sources/thelib/src/streaming/basestream.cpp

${OBJECTDIR}/_ext/126235575/iohandler.o: ../../../../sources/thelib/src/netio/epoll/iohandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/126235575
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/126235575/iohandler.o ../../../../sources/thelib/src/netio/epoll/iohandler.cpp

${OBJECTDIR}/_ext/1124569200/mmsprotocol.o: ../../../../sources/thelib/src/protocols/mms/mmsprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1124569200
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1124569200/mmsprotocol.o ../../../../sources/thelib/src/protocols/mms/mmsprotocol.cpp

${OBJECTDIR}/_ext/554535297/inboundnamedpipecarrier.o: ../../../../sources/thelib/src/netio/kqueue/inboundnamedpipecarrier.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/554535297
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/554535297/inboundnamedpipecarrier.o ../../../../sources/thelib/src/netio/kqueue/inboundnamedpipecarrier.cpp

${OBJECTDIR}/_ext/31226533/outfilertmpflvstream.o: ../../../../sources/thelib/src/protocols/rtmp/streaming/outfilertmpflvstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/31226533
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/31226533/outfilertmpflvstream.o ../../../../sources/thelib/src/protocols/rtmp/streaming/outfilertmpflvstream.cpp

${OBJECTDIR}/_ext/1896237080/connectionmessagefactory.o: ../../../../sources/thelib/src/protocols/rtmp/messagefactories/connectionmessagefactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1896237080
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1896237080/connectionmessagefactory.o ../../../../sources/thelib/src/protocols/rtmp/messagefactories/connectionmessagefactory.cpp

${OBJECTDIR}/_ext/1210522057/atomesds.o: ../../../../sources/thelib/src/mediaformats/mp4/atomesds.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomesds.o ../../../../sources/thelib/src/mediaformats/mp4/atomesds.cpp

${OBJECTDIR}/_ext/1124564181/inboundrtpprotocol.o: ../../../../sources/thelib/src/protocols/rtp/inboundrtpprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1124564181
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1124564181/inboundrtpprotocol.o ../../../../sources/thelib/src/protocols/rtp/inboundrtpprotocol.cpp

${OBJECTDIR}/_ext/517913122/basetsappprotocolhandler.o: ../../../../sources/thelib/src/protocols/ts/basetsappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/517913122
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/517913122/basetsappprotocolhandler.o ../../../../sources/thelib/src/protocols/ts/basetsappprotocolhandler.cpp

${OBJECTDIR}/_ext/1636055853/outnetrawstream.o: ../../../../sources/thelib/src/streaming/outnetrawstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1636055853
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1636055853/outnetrawstream.o ../../../../sources/thelib/src/streaming/outnetrawstream.cpp

${OBJECTDIR}/_ext/1919749812/tcpprotocol.o: ../../../../sources/thelib/src/protocols/tcpprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1919749812
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1919749812/tcpprotocol.o ../../../../sources/thelib/src/protocols/tcpprotocol.cpp

${OBJECTDIR}/_ext/1210522057/atomdata.o: ../../../../sources/thelib/src/mediaformats/mp4/atomdata.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomdata.o ../../../../sources/thelib/src/mediaformats/mp4/atomdata.cpp

${OBJECTDIR}/_ext/1210522057/atomilst.o: ../../../../sources/thelib/src/mediaformats/mp4/atomilst.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomilst.o ../../../../sources/thelib/src/mediaformats/mp4/atomilst.cpp

${OBJECTDIR}/_ext/1124569200/innetmp3stream.o: ../../../../sources/thelib/src/protocols/mms/innetmp3stream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1124569200
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1124569200/innetmp3stream.o ../../../../sources/thelib/src/protocols/mms/innetmp3stream.cpp

${OBJECTDIR}/_ext/1124564181/rtspprotocol.o: ../../../../sources/thelib/src/protocols/rtp/rtspprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1124564181
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1124564181/rtspprotocol.o ../../../../sources/thelib/src/protocols/rtp/rtspprotocol.cpp

${OBJECTDIR}/_ext/1210522057/atomsmhd.o: ../../../../sources/thelib/src/mediaformats/mp4/atomsmhd.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomsmhd.o ../../../../sources/thelib/src/mediaformats/mp4/atomsmhd.cpp

${OBJECTDIR}/_ext/1054419429/outboundconnectivity.o: ../../../../sources/thelib/src/protocols/rtp/connectivity/outboundconnectivity.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1054419429
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1054419429/outboundconnectivity.o ../../../../sources/thelib/src/protocols/rtp/connectivity/outboundconnectivity.cpp

${OBJECTDIR}/_ext/554535297/iohandler.o: ../../../../sources/thelib/src/netio/kqueue/iohandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/554535297
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/554535297/iohandler.o ../../../../sources/thelib/src/netio/kqueue/iohandler.cpp

${OBJECTDIR}/_ext/83984615/configfile.o: ../../../../sources/thelib/src/configuration/configfile.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/83984615
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/83984615/configfile.o ../../../../sources/thelib/src/configuration/configfile.cpp

${OBJECTDIR}/_ext/501751224/header_le_sa.o: ../../../../sources/thelib/src/protocols/rtmp/header_le_sa.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/501751224
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/501751224/header_le_sa.o ../../../../sources/thelib/src/protocols/rtmp/header_le_sa.cpp

${OBJECTDIR}/_ext/501751224/rtmpeprotocol.o: ../../../../sources/thelib/src/protocols/rtmp/rtmpeprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/501751224
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/501751224/rtmpeprotocol.o ../../../../sources/thelib/src/protocols/rtmp/rtmpeprotocol.cpp

${OBJECTDIR}/_ext/547604307/inboundrawhttpstreamprotocol.o: ../../../../sources/thelib/src/protocols/rawhttpstream/inboundrawhttpstreamprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/547604307
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/547604307/inboundrawhttpstreamprotocol.o ../../../../sources/thelib/src/protocols/rawhttpstream/inboundrawhttpstreamprotocol.cpp

${OBJECTDIR}/_ext/1826042562/basevariantprotocol.o: ../../../../sources/thelib/src/protocols/variant/basevariantprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1826042562
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1826042562/basevariantprotocol.o ../../../../sources/thelib/src/protocols/variant/basevariantprotocol.cpp

${OBJECTDIR}/_ext/1210522057/boxatom.o: ../../../../sources/thelib/src/mediaformats/mp4/boxatom.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/boxatom.o ../../../../sources/thelib/src/mediaformats/mp4/boxatom.cpp

${OBJECTDIR}/_ext/1625581042/basertmfpappprotocolhandler.o: ../../../../sources/thelib/src/protocols/rtmfp/basertmfpappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1625581042
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1625581042/basertmfpappprotocolhandler.o ../../../../sources/thelib/src/protocols/rtmfp/basertmfpappprotocolhandler.cpp

${OBJECTDIR}/_ext/1210522057/atomtrak.o: ../../../../sources/thelib/src/mediaformats/mp4/atomtrak.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomtrak.o ../../../../sources/thelib/src/mediaformats/mp4/atomtrak.cpp

${OBJECTDIR}/_ext/2021747320/so.o: ../../../../sources/thelib/src/protocols/rtmp/sharedobjects/so.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2021747320
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2021747320/so.o ../../../../sources/thelib/src/protocols/rtmp/sharedobjects/so.cpp

${OBJECTDIR}/_ext/1636055853/baseinstream.o: ../../../../sources/thelib/src/streaming/baseinstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1636055853
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1636055853/baseinstream.o ../../../../sources/thelib/src/streaming/baseinstream.cpp

${OBJECTDIR}/_ext/1124569200/innetaacstream.o: ../../../../sources/thelib/src/protocols/mms/innetaacstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1124569200
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1124569200/innetaacstream.o ../../../../sources/thelib/src/protocols/mms/innetaacstream.cpp

${OBJECTDIR}/_ext/1773553217/baseliveflvappprotocolhandler.o: ../../../../sources/thelib/src/protocols/liveflv/baseliveflvappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1773553217
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1773553217/baseliveflvappprotocolhandler.o ../../../../sources/thelib/src/protocols/liveflv/baseliveflvappprotocolhandler.cpp

${OBJECTDIR}/_ext/1124577818/inbounddnsresolverprotocol.o: ../../../../sources/thelib/src/protocols/dns/inbounddnsresolverprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1124577818
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1124577818/inbounddnsresolverprotocol.o ../../../../sources/thelib/src/protocols/dns/inbounddnsresolverprotocol.cpp

${OBJECTDIR}/_ext/1210522057/atomtrun.o: ../../../../sources/thelib/src/mediaformats/mp4/atomtrun.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomtrun.o ../../../../sources/thelib/src/mediaformats/mp4/atomtrun.cpp

${OBJECTDIR}/_ext/1210522057/atomstsd.o: ../../../../sources/thelib/src/mediaformats/mp4/atomstsd.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomstsd.o ../../../../sources/thelib/src/mediaformats/mp4/atomstsd.cpp

${OBJECTDIR}/_ext/517913122/streamdescriptors.o: ../../../../sources/thelib/src/protocols/ts/streamdescriptors.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/517913122
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/517913122/streamdescriptors.o ../../../../sources/thelib/src/protocols/ts/streamdescriptors.cpp

${OBJECTDIR}/_ext/1210522057/atomstts.o: ../../../../sources/thelib/src/mediaformats/mp4/atomstts.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomstts.o ../../../../sources/thelib/src/mediaformats/mp4/atomstts.cpp

${OBJECTDIR}/_ext/1210522057/atomminf.o: ../../../../sources/thelib/src/mediaformats/mp4/atomminf.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomminf.o ../../../../sources/thelib/src/mediaformats/mp4/atomminf.cpp

${OBJECTDIR}/_ext/1210522057/atomdinf.o: ../../../../sources/thelib/src/mediaformats/mp4/atomdinf.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atomdinf.o ../../../../sources/thelib/src/mediaformats/mp4/atomdinf.cpp

${OBJECTDIR}/_ext/501751224/channel.o: ../../../../sources/thelib/src/protocols/rtmp/channel.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/501751224
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/501751224/channel.o ../../../../sources/thelib/src/protocols/rtmp/channel.cpp

${OBJECTDIR}/_ext/554535297/tcpacceptor.o: ../../../../sources/thelib/src/netio/kqueue/tcpacceptor.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/554535297
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/554535297/tcpacceptor.o ../../../../sources/thelib/src/netio/kqueue/tcpacceptor.cpp

${OBJECTDIR}/_ext/1124563255/inboundsslprotocol.o: ../../../../sources/thelib/src/protocols/ssl/inboundsslprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1124563255
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1124563255/inboundsslprotocol.o ../../../../sources/thelib/src/protocols/ssl/inboundsslprotocol.cpp

${OBJECTDIR}/_ext/1636055853/streamsmanager.o: ../../../../sources/thelib/src/streaming/streamsmanager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1636055853
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1636055853/streamsmanager.o ../../../../sources/thelib/src/streaming/streamsmanager.cpp

${OBJECTDIR}/_ext/1215912322/outnetrtpudph264stream.o: ../../../../sources/thelib/src/protocols/rtp/streaming/outnetrtpudph264stream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1215912322
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1215912322/outnetrtpudph264stream.o ../../../../sources/thelib/src/protocols/rtp/streaming/outnetrtpudph264stream.cpp

${OBJECTDIR}/_ext/502048917/inboundhttpprotocol.o: ../../../../sources/thelib/src/protocols/http/inboundhttpprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/502048917
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/502048917/inboundhttpprotocol.o ../../../../sources/thelib/src/protocols/http/inboundhttpprotocol.cpp

${OBJECTDIR}/_ext/31226533/rtmpstream.o: ../../../../sources/thelib/src/protocols/rtmp/streaming/rtmpstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/31226533
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/31226533/rtmpstream.o ../../../../sources/thelib/src/protocols/rtmp/streaming/rtmpstream.cpp

${OBJECTDIR}/_ext/373925503/clientapplicationmanager.o: ../../../../sources/thelib/src/application/clientapplicationmanager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/373925503
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/373925503/clientapplicationmanager.o ../../../../sources/thelib/src/application/clientapplicationmanager.cpp

${OBJECTDIR}/_ext/1210522057/atommvhd.o: ../../../../sources/thelib/src/mediaformats/mp4/atommvhd.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/atommvhd.o ../../../../sources/thelib/src/mediaformats/mp4/atommvhd.cpp

${OBJECTDIR}/_ext/1124569200/basemmsappprotocolhandler.o: ../../../../sources/thelib/src/protocols/mms/basemmsappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1124569200
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1124569200/basemmsappprotocolhandler.o ../../../../sources/thelib/src/protocols/mms/basemmsappprotocolhandler.cpp

${OBJECTDIR}/_ext/1210522057/versionedatom.o: ../../../../sources/thelib/src/mediaformats/mp4/versionedatom.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1210522057
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1210522057/versionedatom.o ../../../../sources/thelib/src/mediaformats/mp4/versionedatom.cpp

${OBJECTDIR}/_ext/373925503/baseappprotocolhandler.o: ../../../../sources/thelib/src/application/baseappprotocolhandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/373925503
	${RM} $@.d
	$(COMPILE.cc) -g -DCreateRTCPPacket=CreateRTCPPacket_none -DFeedDataAudioMPEG4Generic=FeedDataAudioMPEG4Generic_one_by_one -DHAS_LUA -DHAS_MEDIA_FLV -DHAS_MEDIA_MKV -DHAS_MEDIA_MKV -DHAS_MEDIA_MP3 -DHAS_MEDIA_MP4 -DHAS_MEDIA_NSV -DHAS_PROTOCOL_CLI -DHAS_PROTOCOL_HTTP -DHAS_PROTOCOL_LIVEFLV -DHAS_PROTOCOL_MMS -DHAS_PROTOCOL_RAWHTTPSTREAM -DHAS_PROTOCOL_RTMFP -DHAS_PROTOCOL_RTMP -DHAS_PROTOCOL_RTP -DHAS_PROTOCOL_TS -DHAS_PROTOCOL_VAR -DHAS_SYSLOG -DLITTLE_ENDIAN_BYTE_ALIGNED -DNET_KQUEUE -DOSX -I/opt/local/include -I../../../../sources/common/include -I../../../../sources/thelib/include -I../../../../../ffmpeg_bin/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/373925503/baseappprotocolhandler.o ../../../../sources/thelib/src/application/baseappprotocolhandler.cpp

# Subprojects
.build-subprojects:
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../${CND_CONF}/${CND_PLATFORM}/libthelib.dylib

# Subprojects
.clean-subprojects:
	cd ../common && ${MAKE}  -f Makefile.nb CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
