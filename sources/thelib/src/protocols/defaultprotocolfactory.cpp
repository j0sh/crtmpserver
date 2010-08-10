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

#include "protocols/defaultprotocolfactory.h"
#include "protocols/protocoltypes.h"
#include "protocols/tcpprotocol.h"
#include "protocols/rtmp/inboundrtmpprotocol.h"
#include "protocols/rtmp/outboundrtmpprotocol.h"
#include "protocols/sslprotocol.h"
#include "protocols/dns/inbounddnsresolverprotocol.h"
#include "protocols/dns/outbounddnsresolverprotocol.h"
#include "protocols/ts/inboundtsprotocol.h"
#include "protocols/http/inboundhttpprotocol.h"
#include "protocols/rtmp/inboundhttp4rtmp.h"
#include "protocols/http/outboundhttpprotocol.h"
#include "protocols/liveflv/inboundliveflvprotocol.h"
#include "protocols/variant/xmlvariantprotocol.h"
#include "protocols/variant/binvariantprotocol.h"
#include "protocols/udpprotocol.h"
#include "protocols/rtp/rtspprotocol.h"
#include "protocols/rtp/inboundrtpprotocol.h"
#include "protocols/rtp/rtcpprotocol.h"

DefaultProtocolFactory::DefaultProtocolFactory()
: BaseProtocolFactory() {

}

DefaultProtocolFactory::~DefaultProtocolFactory() {
}

vector<uint64_t> DefaultProtocolFactory::HandledProtocols() {
	vector<uint64_t> result;

	ADD_VECTOR_END(result, PT_TCP);
	ADD_VECTOR_END(result, PT_UDP);
	ADD_VECTOR_END(result, PT_SSL);
	ADD_VECTOR_END(result, PT_INBOUND_DNS);
	ADD_VECTOR_END(result, PT_OUTBOUND_DNS);
	ADD_VECTOR_END(result, PT_TIMER);
#ifdef HAS_PROTOCOL_TS
	ADD_VECTOR_END(result, PT_INBOUND_TS);
#endif /* HAS_PROTOCOL_TS */
#ifdef HAS_PROTOCOL_RTMP
	ADD_VECTOR_END(result, PT_INBOUND_RTMP);
	ADD_VECTOR_END(result, PT_OUTBOUND_RTMP);
	ADD_VECTOR_END(result, PT_MONITOR_RTMP);
	ADD_VECTOR_END(result, PT_RTMPE);
#ifdef HAS_PROTOCOL_HTTP
	ADD_VECTOR_END(result, PT_INBOUND_HTTP_FOR_RTMP);
	ADD_VECTOR_END(result, PT_OUTBOUND_HTTP_FOR_RTMP);
#endif /* HAS_PROTOCOL_HTTP */
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_HTTP
	ADD_VECTOR_END(result, PT_INBOUND_HTTP);
	ADD_VECTOR_END(result, PT_OUTBOUND_HTTP);
#endif /* HAS_PROTOCOL_HTTP */

#ifdef HAS_PROTOCOL_LIVEFLV
	ADD_VECTOR_END(result, PT_INBOUND_LIVE_FLV);
	ADD_VECTOR_END(result, PT_OUTBOUND_LIVE_FLV);
#endif /* HAS_PROTOCOL_LIVEFLV */
#ifdef HAS_PROTOCOL_VAR
	ADD_VECTOR_END(result, PT_BIN_VAR);
	ADD_VECTOR_END(result, PT_XML_VAR);
#endif /* HAS_PROTOCOL_VAR */
#ifdef HAS_PROTOCOL_RTP
	ADD_VECTOR_END(result, PT_RTSP);
	ADD_VECTOR_END(result, PT_RTCP);
	ADD_VECTOR_END(result, PT_INBOUND_RTP);
#endif /* HAS_PROTOCOL_RTP */

	return result;
}

vector<string> DefaultProtocolFactory::HandledProtocolChains() {
	vector<string> result;
	ADD_VECTOR_END(result, CONF_PROTOCOL_INBOUND_DNS);
	ADD_VECTOR_END(result, CONF_PROTOCOL_OUTBOUND_DNS);
#ifdef HAS_PROTOCOL_RTMP
	ADD_VECTOR_END(result, CONF_PROTOCOL_INBOUND_RTMP);
	ADD_VECTOR_END(result, CONF_PROTOCOL_OUTBOUND_RTMP);
#ifdef HAS_PROTOCOL_HTTP
	ADD_VECTOR_END(result, CONF_PROTOCOL_INBOUND_RTMPS);
	ADD_VECTOR_END(result, CONF_PROTOCOL_INBOUND_RTMPT);
#endif /* HAS_PROTOCOL_HTTP */
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_TS
	ADD_VECTOR_END(result, CONF_PROTOCOL_INBOUND_TCP_TS);
	ADD_VECTOR_END(result, CONF_PROTOCOL_INBOUND_UDP_TS);
#endif /* HAS_PROTOCOL_TS */
#ifdef HAS_PROTOCOL_HTTP
	ADD_VECTOR_END(result, CONF_PROTOCOL_OUTBOUND_HTTP);
#endif /* HAS_PROTOCOL_HTTP */
#ifdef HAS_PROTOCOL_LIVEFLV
	ADD_VECTOR_END(result, CONF_PROTOCOL_INBOUND_LIVE_FLV);
	ADD_VECTOR_END(result, CONF_PROTOCOL_OUTBOUND_LIVE_FLV);
#endif /* HAS_PROTOCOL_LIVEFLV */
#ifdef HAS_PROTOCOL_VAR
	ADD_VECTOR_END(result, CONF_PROTOCOL_INBOUND_XML_VARIANT);
	ADD_VECTOR_END(result, CONF_PROTOCOL_INBOUND_BIN_VARIANT);
	ADD_VECTOR_END(result, CONF_PROTOCOL_OUTBOUND_XML_VARIANT);
	ADD_VECTOR_END(result, CONF_PROTOCOL_OUTBOUND_BIN_VARIANT);
#ifdef HAS_PROTOCOL_HTTP
	ADD_VECTOR_END(result, CONF_PROTOCOL_INBOUND_HTTP_XML_VARIANT);
	ADD_VECTOR_END(result, CONF_PROTOCOL_INBOUND_HTTP_BIN_VARIANT);
	ADD_VECTOR_END(result, CONF_PROTOCOL_OUTBOUND_HTTP_XML_VARIANT);
	ADD_VECTOR_END(result, CONF_PROTOCOL_OUTBOUND_HTTP_BIN_VARIANT);
#endif /* HAS_PROTOCOL_HTTP */
#endif /* HAS_PROTOCOL_VAR */
#ifdef HAS_PROTOCOL_RTP
	ADD_VECTOR_END(result, CONF_PROTOCOL_INBOUND_RTSP);
	ADD_VECTOR_END(result, CONF_PROTOCOL_RTSP_RTCP);
	ADD_VECTOR_END(result, CONF_PROTOCOL_UDP_RTCP);
	ADD_VECTOR_END(result, CONF_PROTOCOL_INBOUND_RTSP_RTP);
	ADD_VECTOR_END(result, CONF_PROTOCOL_INBOUND_UDP_RTP);
#endif /* HAS_PROTOCOL_RTP */

	return result;
}

vector<uint64_t> DefaultProtocolFactory::ResolveProtocolChain(string name) {
	vector<uint64_t> result;
	if (name == CONF_PROTOCOL_INBOUND_DNS) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_INBOUND_DNS);
	} else if (name == CONF_PROTOCOL_OUTBOUND_DNS) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_OUTBOUND_DNS);
	}
#ifdef HAS_PROTOCOL_RTMP
	else if (name == CONF_PROTOCOL_INBOUND_RTMP) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_INBOUND_RTMP);
	} else if (name == CONF_PROTOCOL_OUTBOUND_RTMP) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_OUTBOUND_RTMP);
	}
#ifdef HAS_PROTOCOL_HTTP
	else if (name == CONF_PROTOCOL_INBOUND_RTMPS) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_SSL);
		ADD_VECTOR_END(result, PT_INBOUND_HTTP);
		ADD_VECTOR_END(result, PT_INBOUND_HTTP_FOR_RTMP);
	} else if (name == CONF_PROTOCOL_INBOUND_RTMPT) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_INBOUND_HTTP);
		ADD_VECTOR_END(result, PT_INBOUND_HTTP_FOR_RTMP);
	}
#endif /* HAS_PROTOCOL_HTTP */
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_TS
	else if (name == CONF_PROTOCOL_INBOUND_TCP_TS) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_INBOUND_TS);
	} else if (name == CONF_PROTOCOL_INBOUND_UDP_TS) {
		ADD_VECTOR_END(result, PT_UDP);
		ADD_VECTOR_END(result, PT_INBOUND_TS);
	}
#endif /* HAS_PROTOCOL_TS */
#ifdef HAS_PROTOCOL_RTP
	else if (name == CONF_PROTOCOL_INBOUND_RTSP) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_RTSP);
	} else if (name == CONF_PROTOCOL_RTSP_RTCP) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_RTSP);
		ADD_VECTOR_END(result, PT_RTCP);
	} else if (name == CONF_PROTOCOL_UDP_RTCP) {
		ADD_VECTOR_END(result, PT_UDP);
		ADD_VECTOR_END(result, PT_RTCP);
	} else if (name == CONF_PROTOCOL_INBOUND_RTSP_RTP) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_RTSP);
		ADD_VECTOR_END(result, PT_INBOUND_RTP);
	} else if (name == CONF_PROTOCOL_INBOUND_UDP_RTP) {
		ADD_VECTOR_END(result, PT_UDP);
		ADD_VECTOR_END(result, PT_INBOUND_RTP);
	}
#endif /* HAS_PROTOCOL_RTP */
#ifdef HAS_PROTOCOL_HTTP
	else if (name == CONF_PROTOCOL_OUTBOUND_HTTP) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_OUTBOUND_HTTP);
	}
#endif /* HAS_PROTOCOL_HTTP */
#ifdef HAS_PROTOCOL_LIVEFLV
	else if (name == CONF_PROTOCOL_INBOUND_LIVE_FLV) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_INBOUND_LIVE_FLV);
	}
#endif /* HAS_PROTOCOL_LIVEFLV */
#ifdef HAS_PROTOCOL_VAR
	else if (name == CONF_PROTOCOL_INBOUND_XML_VARIANT) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_XML_VAR);
	} else if (name == CONF_PROTOCOL_INBOUND_BIN_VARIANT) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_BIN_VAR);
	} else if (name == CONF_PROTOCOL_OUTBOUND_XML_VARIANT) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_XML_VAR);
	} else if (name == CONF_PROTOCOL_OUTBOUND_BIN_VARIANT) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_BIN_VAR);
	}
#ifdef HAS_PROTOCOL_HTTP
	else if (name == CONF_PROTOCOL_INBOUND_HTTP_XML_VARIANT) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_INBOUND_HTTP);
		ADD_VECTOR_END(result, PT_XML_VAR);
	} else if (name == CONF_PROTOCOL_INBOUND_HTTP_BIN_VARIANT) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_INBOUND_HTTP);
		ADD_VECTOR_END(result, PT_BIN_VAR);
	} else if (name == CONF_PROTOCOL_OUTBOUND_HTTP_XML_VARIANT) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_OUTBOUND_HTTP);
		ADD_VECTOR_END(result, PT_XML_VAR);
	} else if (name == CONF_PROTOCOL_OUTBOUND_HTTP_BIN_VARIANT) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_OUTBOUND_HTTP);
		ADD_VECTOR_END(result, PT_BIN_VAR);
	}
#endif /* HAS_PROTOCOL_HTTP */
#endif /* HAS_PROTOCOL_VAR */
	else {
		FATAL("Invalid protocol chain: %s.", STR(name));
	}
	return result;
}

BaseProtocol *DefaultProtocolFactory::SpawnProtocol(uint64_t type, Variant &parameters) {
	BaseProtocol *pResult = NULL;
	switch (type) {
		case PT_TCP:
			pResult = new TCPProtocol();
			break;
		case PT_UDP:
			pResult = new UDPProtocol();
			break;
		case PT_SSL:
			pResult = new SSLProtocol();
			break;
		case PT_INBOUND_DNS:
			pResult = new InboundDNSResolverProtocol();
			break;
		case PT_OUTBOUND_DNS:
			pResult = new OutboundDNSResolverProtocol();
			break;
#ifdef HAS_PROTOCOL_RTMP
		case PT_INBOUND_RTMP:
			pResult = new InboundRTMPProtocol();
			break;
		case PT_OUTBOUND_RTMP:
			pResult = new OutboundRTMPProtocol();
			break;
#ifdef HAS_PROTOCOL_HTTP
		case PT_INBOUND_HTTP_FOR_RTMP:
			pResult = new InboundHTTP4RTMP();
			break;
#endif /* HAS_PROTOCOL_HTTP */
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_TS
		case PT_INBOUND_TS:
			pResult = new InboundTSProtocol();
			break;
#endif /* HAS_PROTOCOL_TS */
#ifdef HAS_PROTOCOL_HTTP
		case PT_INBOUND_HTTP:
			pResult = new InboundHTTPProtocol();
			break;
		case PT_OUTBOUND_HTTP:
			pResult = new OutboundHTTPProtocol();
			break;
#endif /* HAS_PROTOCOL_HTTP */
#ifdef HAS_PROTOCOL_LIVEFLV
		case PT_INBOUND_LIVE_FLV:
			pResult = new InboundLiveFLVProtocol();
			break;
#endif /* HAS_PROTOCOL_LIVEFLV */
#ifdef HAS_PROTOCOL_VAR
		case PT_XML_VAR:
			pResult = new XmlVariantProtocol();
			break;
		case PT_BIN_VAR:
			pResult = new BinVariantProtocol();
			break;
#endif /* HAS_PROTOCOL_VAR */
#ifdef HAS_PROTOCOL_RTP
		case PT_RTSP:
			pResult = new RTSPProtocol();
			break;
		case PT_RTCP:
			pResult = new RTCPProtocol();
			break;
		case PT_INBOUND_RTP:
			pResult = new InboundRTPProtocol();
			break;
#endif /* HAS_PROTOCOL_RTP */
		default:
			FATAL("Spawning protocol %s not yet implemented",
					STR(tagToString(type)));
			break;
	}
	if (pResult != NULL) {
		if (!pResult->Initialize(parameters)) {
			FATAL("Unable to initialize protocol %s",
					STR(tagToString(type)));
			delete pResult;
			pResult = NULL;
		}
	}
	return pResult;
}

