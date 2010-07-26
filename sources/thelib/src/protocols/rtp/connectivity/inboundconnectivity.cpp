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

InboundConnectivity::InboundConnectivity(RTSPProtocol *pRTSP)
: BaseConnectivity() {
	_pRTSP = pRTSP;
	_pRTPVideo = NULL;
	_pRTCPVideo = NULL;
	_pRTPAudio = NULL;
	_pRTCPAudio = NULL;
	_pInStream = NULL;
}

InboundConnectivity::~InboundConnectivity() {
	Cleanup();
}

void InboundConnectivity::EnqueueForDelete() {
	Cleanup();
	_pRTSP->EnqueueForDelete();
}

bool InboundConnectivity::Initialize(Variant &videoTrack, Variant &audioTrack) {
	//1. get the application
	BaseClientApplication *pApplication = _pRTSP->GetApplication();
	if (pApplication == NULL) {
		FATAL("RTSP protocol not yet assigned to an application");
		return false;
	}

	//2. Close existing protocols
	Cleanup();

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

	//5. Set the application on protocols
	_pRTPVideo->SetApplication(pApplication);
	_pRTCPVideo->SetApplication(pApplication);
	_pRTPAudio->SetApplication(pApplication);
	_pRTCPAudio->SetApplication(pApplication);

	//6. Create the in stream
	_pInStream = new InNetRTPStream(_pRTSP, pApplication->GetStreamsManager(),
			format("rtsp_%d", _pRTSP->GetId()),
			unb64((string) SDP_VIDEO_CODEC_H264_SPS(videoTrack)),
			unb64((string) SDP_VIDEO_CODEC_H264_PPS(videoTrack)));

	//7. make the stream known to inbound RTP protocols
	_pRTPVideo->SetStream(_pInStream);
	_pRTPAudio->SetStream(_pInStream);

	//8. Make the this Connectivity known to all protocols
	_pRTPVideo->SetInbboundConnectivity(this);
	_pRTCPVideo->SetInbboundConnectivity(this);
	_pRTPAudio->SetInbboundConnectivity(this);
	_pRTCPAudio->SetInbboundConnectivity(this);

	//9. Done
	return true;
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

void InboundConnectivity::GetSSRCAndSeq(uint32_t rtpId, uint32_t &ssrc, uint32_t &seq) {
	//1. Reset
	ssrc = 0;
	seq = 0;

	//2. Get the correct protocol
	InboundRTPProtocol *pRTP = NULL;
	if (_pRTCPAudio != NULL) {
		if (_pRTCPAudio->GetId() == rtpId) {
			pRTP = _pRTPAudio;
		}
	}
	if (pRTP == NULL) {
		if (_pRTCPVideo != NULL) {
			if (_pRTCPVideo->GetId() == rtpId) {
				pRTP = _pRTPVideo;
			}
		}
	}
	if (pRTP == NULL) {
		return;
	}

	//3. get the required info
	ssrc = pRTP->GetSSRC();
	//FINEST("ssrc: %08x", ssrc);
	seq = pRTP->GetExtendedSeq();
	//FINEST("seq: %08x", seq);
}

bool InboundConnectivity::SendRTP(sockaddr_in &address, uint32_t rtpId,
		uint8_t *pBuffer, uint32_t length) {
	RTCPProtocol *pRTCP = NULL;
	if (_pRTCPAudio != NULL) {
		if (_pRTCPAudio->GetId() == rtpId) {
			pRTCP = _pRTCPAudio;
		}
	}
	if (pRTCP == NULL) {
		if (_pRTCPVideo != NULL) {
			if (_pRTCPVideo->GetId() == rtpId) {
				pRTCP = _pRTCPVideo;
			}
		}
	}
	if (pRTCP == NULL) {
		FATAL("Unable to find the protocol");
		return false;
	}

	if (ntohs(address.sin_port) == 0) {
		WARN("Invalid address: %s:%d",
				inet_ntoa(address.sin_addr), ntohs(address.sin_port));
		return true;
	}

	//FINEST("%s:%d length: %d", inet_ntoa(address.sin_addr), ntohs(address.sin_port), length);
	return sendto(pRTCP->GetIOHandler()->GetOutboundFd(),
			pBuffer, length, 0, (sockaddr *) & address, sizeof (address)) == (int32_t) length;
}

void InboundConnectivity::Cleanup() {
	if (_pInStream != NULL) {
		delete _pInStream;
		_pInStream = NULL;
	}
	if (_pRTPVideo != NULL) {
		_pRTPVideo->SetStream(NULL);
		_pRTPVideo->EnqueueForDelete();
		_pRTPVideo = NULL;
	}
	if (_pRTCPVideo != NULL) {
		_pRTCPVideo->EnqueueForDelete();
		_pRTCPVideo = NULL;
	}
	if (_pRTPAudio != NULL) {
		_pRTPAudio->SetStream(NULL);
		_pRTPAudio->EnqueueForDelete();
		_pRTPAudio = NULL;
	}
	if (_pRTCPAudio != NULL) {
		_pRTCPAudio->EnqueueForDelete();
		_pRTCPAudio = NULL;
	}
}

bool InboundConnectivity::CreateCarriers(InboundRTPProtocol *pRTP, RTCPProtocol *pRTCP) {
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
		//WARN("pRTP: %s", STR(*pRTP));

		pCarrier2->SetProtocol(pRTCP->GetFarEndpoint());
		pRTCP->GetFarEndpoint()->SetIOHandler(pCarrier2);
		//WARN("pRTCP: %s", STR(*pRTCP));

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
