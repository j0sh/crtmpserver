/*
 *  Copyright (c) 2010,
 *  Gavriloaie Eugen-Andrei (shiretu@gmail.com)
 *
 *  This file is part of crtmpserver.
 *  crtmpserver is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  crtmpserver is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with crtmpserver.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAS_PROTOCOL_RTP
#include "protocols/rtp/connectivity/inboundconnectivity.h"
#include "protocols/baseprotocol.h"
#include "protocols/protocolfactorymanager.h"
#include "protocols/rtp/inboundrtpprotocol.h"
#include "protocols/rtp/rtcpprotocol.h"
#include "protocols/rtp/rtspprotocol.h"
#include "netio/netio.h"
#include "protocols/rtp/sdp.h"
#include "application/baseclientapplication.h"
#include "protocols/rtp/streaming/innetrtpstream.h"
#include "protocols/udpprotocol.h"
#include "protocols/http/basehttpprotocol.h"

InboundConnectivity::InboundConnectivity(RTSPProtocol *pRTSP)
: BaseConnectivity() {
	_pRTSP = pRTSP;
	_pRTPVideo = NULL;
	_pRTCPVideo = NULL;
	_pRTPAudio = NULL;
	_pRTCPAudio = NULL;
	_pInStream = NULL;
	_forceTcp = false;
	memset(_pProtocols, 0, sizeof (_pProtocols));
	memset(&_dummyAddress, 0, sizeof (_dummyAddress));

	memset(_audioRR, 0, sizeof (_audioRR));
	_audioRR[0] = '$'; //marker
	_audioRR[1] = 0; //channel
	_audioRR[2] = 0; //size
	_audioRR[3] = 56; //size
	_audioRR[4] = 0x81; //V,P,RC
	_audioRR[5] = 0xc9; //PT
	_audioRR[6] = 0x00; //length
	_audioRR[7] = 0x07; //length
	EHTONLP(_audioRR + 16, 0x00ffffff); //fraction lost/cumulative number of packets lost
	EHTONLP(_audioRR + 24, 0); //interarrival jitter
	EHTONLP(_audioRR + 32, 0); // delay since last SR (DLSR)
	_audioRR[36] = 0x81; //V,P,RC
	_audioRR[37] = 0xca; //PT
	_audioRR[38] = 0x00; //length
	_audioRR[39] = 0x05; //length
	_audioRR[44] = 0x01; //type
	_audioRR[45] = 0x0d; //length
	memcpy(_audioRR + 46, "machine.local", 0x0d); //name of the machine
	_audioRR[59] = 0; //padding

	memset(_videoRR, 0, sizeof (_videoRR));
	_videoRR[0] = '$'; //marker
	_videoRR[1] = 0; //channel
	_videoRR[2] = 0; //size
	_videoRR[3] = 56; //size
	_videoRR[4] = 0x81; //V,P,RC
	_videoRR[5] = 0xc9; //PT
	_videoRR[6] = 0x00; //length
	_videoRR[7] = 0x07; //length
	EHTONLP(_videoRR + 16, 0x00ffffff); //fraction lost/cumulative number of packets lost
	EHTONLP(_videoRR + 24, 0); //interarrival jitter
	EHTONLP(_videoRR + 32, 0); // delay since last SR (DLSR)
	_videoRR[36] = 0x81; //V,P,RC
	_videoRR[37] = 0xca; //PT
	_videoRR[38] = 0x00; //length
	_videoRR[39] = 0x05; //length
	_videoRR[44] = 0x01; //type
	_videoRR[45] = 0x0d; //length
	memcpy(_videoRR + 46, "machine.local", 0x0d); //name of the machine
	_videoRR[59] = 0; //padding
}

InboundConnectivity::~InboundConnectivity() {
	Cleanup();
}

void InboundConnectivity::EnqueueForDelete() {
	Cleanup();
	_pRTSP->EnqueueForDelete();
}

bool InboundConnectivity::Initialize(Variant &videoTrack, Variant &audioTrack,
		string streamName, bool forceTcp) {
	_forceTcp = forceTcp;

	//1. get the application
	BaseClientApplication *pApplication = _pRTSP->GetApplication();
	if (pApplication == NULL) {
		FATAL("RTSP protocol not yet assigned to an application");
		return false;
	}

	//2. Close existing protocols
	Cleanup();

	//3. create the stacks of protocols
	if (forceTcp) {
		if (!InitializeTCP(videoTrack, audioTrack)) {
			FATAL("Unable to initialize TCP based protocols");
			return false;
		}
	} else {
		if (!InitializeUDP(videoTrack, audioTrack)) {
			FATAL("Unable to initialize UDP based protocols");
			return false;
		}
	}

	//4. Set the application on protocols
	_pRTPVideo->SetApplication(pApplication);
	_pRTCPVideo->SetApplication(pApplication);
	_pRTPAudio->SetApplication(pApplication);
	_pRTCPAudio->SetApplication(pApplication);

	//5. Create the in stream
	if (streamName == "")
		streamName = format("rtsp_%d", _pRTSP->GetId());
	_pInStream = new InNetRTPStream(_pRTSP, pApplication->GetStreamsManager(),
			streamName,
			videoTrack != V_NULL ? unb64((string) SDP_VIDEO_CODEC_H264_SPS(videoTrack)) : "",
			videoTrack != V_NULL ? unb64((string) SDP_VIDEO_CODEC_H264_PPS(videoTrack)) : "",
			audioTrack != V_NULL ? unhex(SDP_AUDIO_CODEC_SETUP(audioTrack)) : ""
			);

	//6. make the stream known to inbound RTP protocols
	_pRTPVideo->SetStream(_pInStream, false);
	_pRTPAudio->SetStream(_pInStream, true);

	//7. Make the this Connectivity known to all protocols
	_pRTPVideo->SetInbboundConnectivity(this);
	_pRTCPVideo->SetInbboundConnectivity(this, false);
	_pRTPAudio->SetInbboundConnectivity(this);
	_pRTCPAudio->SetInbboundConnectivity(this, true);

	//8. Pickup all outbound waiting streams
	map<uint32_t, BaseOutStream *> subscribedOutStreams =
			pApplication->GetStreamsManager()->GetWaitingSubscribers(
			streamName, _pInStream->GetType());
	FINEST("subscribedOutStreams count: %d", subscribedOutStreams.size());


	//9. Bind the waiting subscribers

	FOR_MAP(subscribedOutStreams, uint32_t, BaseOutStream *, i) {
		BaseOutStream *pBaseOutStream = MAP_VAL(i);
		pBaseOutStream->Link(_pInStream);
	}

	//10. Done
	return true;
}

string InboundConnectivity::GetTransportHeaderLine(bool isAudio) {
	if (_forceTcp) {
		BaseProtocol *pProtocol = isAudio ? _pRTPAudio : _pRTPVideo;
		for (uint32_t i = 0; i < 255; i++) {
			if ((_pProtocols[i] != NULL) && (_pProtocols[i]->GetId() == pProtocol->GetId())) {
				string result = format("RTP/AVP/TCP;unicast;interleaved=%d-%d", i, i + 1);
				return result;
			}
		}
		FATAL("No track");
		return "";
	} else {
		return format("RTP/AVP;unicast;client_port=%s",
				isAudio ? STR(GetAudioClientPorts())
				: STR(GetVideoClientPorts()));
	}
}

bool InboundConnectivity::FeedData(uint32_t channelId, uint8_t *pBuffer,
		uint32_t bufferLength) {
	//1. Is the chanel number a valid chanel?
	if (channelId >= 4) {
		FATAL("Invalid chanel number: %d", channelId);
		return false;
	}

	//2. Get the protocol
	BaseProtocol *pProtocol = _pProtocols[channelId];
	if (pProtocol == NULL) {
		FATAL("Invalid chanel number: %d", channelId);
		return false;
	}

	//3. prepare the buffer
	_inputBuffer.IgnoreAll();
	_inputBuffer.ReadFromBuffer(pBuffer, bufferLength);

	//4. feed the data
	return pProtocol->SignalInputData(_inputBuffer, &_dummyAddress);
}

string InboundConnectivity::GetAudioClientPorts() {
	return format("%d-%d",
			((UDPCarrier *) _pRTPAudio->GetIOHandler())->GetNearEndpointPort(),
			((UDPCarrier *) _pRTCPAudio->GetIOHandler())->GetNearEndpointPort());
}

string InboundConnectivity::GetVideoClientPorts() {
	return format("%d-%d",
			((UDPCarrier *) _pRTPVideo->GetIOHandler())->GetNearEndpointPort(),
			((UDPCarrier *) _pRTCPVideo->GetIOHandler())->GetNearEndpointPort());
}

bool InboundConnectivity::SendRR(bool isAudio) {
	if (_forceTcp)
		return true;
	/*
			0                   1                   2                   3
			0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
		   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	header |V=2|P|    RC   |   PT=RR=201   |             length            |0
		   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		   |                     SSRC of packet sender                     |4
		   +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
	report |                 SSRC_1 (SSRC of first source)                 |8
	block  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	  1    | fraction lost |       cumulative number of packets lost       |12
		   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		   |           extended highest sequence number received           |16
		   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		   |                      interarrival jitter                      |20
		   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		   |                         last SR (LSR)                         |24
		   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		   |                   delay since last SR (DLSR)                  |28
		   +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
		   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	header |V=2|P|    SC   |  PT=SDES=202  |             length            |
		   +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
	chunk  |                          SSRC/CSRC_1                          |
	  1    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		   |                           SDES items                          |
		   |                              ...                              |
		   +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
	chunk  |                          SSRC/CSRC_2                          |
	  2    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		   |                           SDES items                          |
		   |                              ...                              |
		   +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
	 */

	InboundRTPProtocol *pRTP = isAudio ? _pRTPAudio : _pRTPVideo;
	RTCPProtocol *pRTCP = isAudio ? _pRTCPAudio : _pRTCPVideo;
	uint8_t *pBuffer = isAudio ? _audioRR : _videoRR;

	//1. prepare the buffer
	EHTONLP(pBuffer + 12, pRTP->GetSSRC()); //SSRC_1 (SSRC of first source)
	EHTONLP(pBuffer + 20, pRTP->GetExtendedSeq()); //extended highest sequence number received
	EHTONLP(pBuffer + 28, pRTCP->GetLastSenderReport()); //last SR (LSR)

	if (_forceTcp) {
		return _pRTSP->SendRaw(pBuffer, 60);
	} else {
		if (pRTCP->GetLastAddress() != NULL) {
			if (sendto(pRTCP->GetIOHandler()->GetOutboundFd(),
					(char *) (pBuffer + 4), 56, 0,
					(sockaddr *) pRTCP->GetLastAddress(), sizeof (sockaddr)) != 56) {
				FATAL("Unable to send data: %d %s", errno, strerror(errno));
				return false;
			}
		} else {
			WARN("Skip this RR because we don't have a valid address yet");
		}
		return true;
	}

	//3. Done
	return true;
}

void InboundConnectivity::ReportSR(uint64_t ntpMicroseconds, uint32_t rtpTimestamp, bool isAudio) {
	if (_pInStream != NULL) {
		_pInStream->ReportSR(ntpMicroseconds, rtpTimestamp, isAudio);
	}
}

bool InboundConnectivity::InitializeUDP(Variant &videoTrack, Variant & audioTrack) {
	//3. Create all protocols
	Variant dummy;
	_pRTPVideo = (InboundRTPProtocol *) ProtocolFactoryManager::CreateProtocolChain(
			CONF_PROTOCOL_INBOUND_UDP_RTP, dummy);
	if (_pRTPVideo == NULL) {
		FATAL("Unable to create the protocol chain");
		Cleanup();
		return false;
	}
	_pRTCPVideo = (RTCPProtocol *) ProtocolFactoryManager::CreateProtocolChain(
			CONF_PROTOCOL_UDP_RTCP, dummy);
	if (_pRTCPVideo == NULL) {
		FATAL("Unable to create the protocol chain");
		Cleanup();
		return false;
	}
	_pRTPAudio = (InboundRTPProtocol *) ProtocolFactoryManager::CreateProtocolChain(
			CONF_PROTOCOL_INBOUND_UDP_RTP, dummy);
	if (_pRTPAudio == NULL) {
		FATAL("Unable to create the protocol chain");
		Cleanup();
		return false;
	}
	_pRTCPAudio = (RTCPProtocol *) ProtocolFactoryManager::CreateProtocolChain(
			CONF_PROTOCOL_UDP_RTCP, dummy);
	if (_pRTCPAudio == NULL) {
		FATAL("Unable to create the protocol chain");
		Cleanup();
		return false;
	}

	//4. Create the carriers
	if (!CreateCarriers(_pRTPVideo, _pRTCPVideo)) {
		FATAL("Unable to create video carriers");
		Cleanup();
		return false;
	}
	if (!CreateCarriers(_pRTPAudio, _pRTCPAudio)) {
		FATAL("Unable to create audio carriers");
		Cleanup();
		return false;
	}

	return true;
}

bool InboundConnectivity::InitializeTCP(Variant &videoTrack, Variant & audioTrack) {
	//1. create the protocols
	_pRTPVideo = new InboundRTPProtocol();
	_pRTCPVideo = new RTCPProtocol();
	_pRTPAudio = new InboundRTPProtocol();
	_pRTCPAudio = new RTCPProtocol();

	//2. Add them in the fast-pickup array
	if (videoTrack != V_NULL) {
		uint8_t idx = (uint8_t) ((uint32_t) SDP_TRACK_GLOBAL_INDEX(videoTrack)*2);
		_pProtocols[idx] = _pRTPVideo;
		_pProtocols[idx + 1] = _pRTCPVideo;
		EHTONLP(_videoRR + 8, _pRTCPVideo->GetSSRC()); //SSRC of packet sender
		EHTONLP(_videoRR + 40, _pRTCPVideo->GetSSRC()); //SSRC of packet sender
		_videoRR[1] = idx + 1;
	}

	if (audioTrack != V_NULL) {
		uint8_t idx = (uint8_t) ((uint32_t) SDP_TRACK_GLOBAL_INDEX(audioTrack)*2);
		_pProtocols[idx] = _pRTPAudio;
		_pProtocols[idx + 1] = _pRTCPAudio;
		EHTONLP(_audioRR + 8, _pRTCPAudio->GetSSRC()); //SSRC of packet sender
		EHTONLP(_audioRR + 40, _pRTCPAudio->GetSSRC()); //SSRC of packet sender
		_audioRR[1] = idx + 1;
	}

	//3. Done
	return true;
}

void InboundConnectivity::Cleanup() {
	if (_pInStream != NULL) {
		delete _pInStream;
		_pInStream = NULL;
	}
	if (_pRTPVideo != NULL) {
		_pRTPVideo->SetStream(NULL, false);
		_pRTPVideo->EnqueueForDelete();
		_pRTPVideo = NULL;
	}
	if (_pRTCPVideo != NULL) {
		_pRTCPVideo->EnqueueForDelete();
		_pRTCPVideo = NULL;
	}
	if (_pRTPAudio != NULL) {
		_pRTPAudio->SetStream(NULL, true);
		_pRTPAudio->EnqueueForDelete();
		_pRTPAudio = NULL;
	}
	if (_pRTCPAudio != NULL) {
		_pRTCPAudio->EnqueueForDelete();
		_pRTCPAudio = NULL;
	}
}

bool InboundConnectivity::CreateCarriers(InboundRTPProtocol *pRTP, RTCPProtocol * pRTCP) {
	UDPCarrier *pCarrier1 = NULL;
	UDPCarrier *pCarrier2 = NULL;
	for (uint32_t i = 0; i < 10; i++) {
		if (pCarrier1 != NULL) {
			delete pCarrier1;
			pCarrier1 = NULL;
		}
		if (pCarrier2 != NULL) {
			delete pCarrier2;
			pCarrier2 = NULL;
		}

		pCarrier1 = UDPCarrier::Create("0.0.0.0", 0);
		if (pCarrier1 == NULL) {
			WARN("Unable to create UDP carrier for RTP");
			continue;
		}

		if ((pCarrier1->GetNearEndpointPort() % 2) == 0) {
			pCarrier2 = UDPCarrier::Create("0.0.0.0",
					pCarrier1->GetNearEndpointPort() + 1);
		} else {
			pCarrier2 = UDPCarrier::Create("0.0.0.0",
					pCarrier1->GetNearEndpointPort() - 1);
		}

		if (pCarrier2 == NULL) {
			WARN("Unable to create UDP carrier for RTP");
			continue;
		}

		if (pCarrier1->GetNearEndpointPort() > pCarrier2->GetNearEndpointPort()) {
			WARN("Switch carriers");
			UDPCarrier *pTemp = pCarrier1;
			pCarrier1 = pCarrier2;
			pCarrier2 = pTemp;
		}

		pCarrier1->SetProtocol(pRTP->GetFarEndpoint());
		pRTP->GetFarEndpoint()->SetIOHandler(pCarrier1);

		pCarrier2->SetProtocol(pRTCP->GetFarEndpoint());
		pRTCP->GetFarEndpoint()->SetIOHandler(pCarrier2);


		return true;
	}

	if (pCarrier1 != NULL) {
		delete pCarrier1;
		pCarrier1 = NULL;
	}
	if (pCarrier2 != NULL) {
		delete pCarrier2;
		pCarrier2 = NULL;
	}

	return false;
}
#endif /* HAS_PROTOCOL_RTP */
