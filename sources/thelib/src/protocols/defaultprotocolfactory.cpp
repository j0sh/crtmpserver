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
#include "protocols/ssl/inboundsslprotocol.h"
#include "protocols/ssl/outboundsslprotocol.h"
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
#include "protocols/cli/inboundjsoncliprotocol.h"
#include "protocols/rtmp/inboundrtmpsdiscriminatorprotocol.h"
#include "protocols/cli/http4cliprotocol.h"
#include "protocols/mms/mmsprotocol.h"
#include "protocols/rawhttpstream/inboundrawhttpstreamprotocol.h"
#include "protocols/rtp/nattraversalprotocol.h"

DefaultProtocolFactory::DefaultProtocolFactory()
: BaseProtocolFactory() {

}

DefaultProtocolFactory::~DefaultProtocolFactory() {
}

vector<uint64_t> DefaultProtocolFactory::HandledProtocols() {
	vector<uint64_t> result;

	ADD_VECTOR_END(result, PT_TCP);
	ADD_VECTOR_END(result, PT_UDP);
	ADD_VECTOR_END(result, PT_INBOUND_SSL);
	ADD_VECTOR_END(result, PT_OUTBOUND_SSL);
#ifdef HAS_PROTOCOL_DNS
	ADD_VECTOR_END(result, PT_INBOUND_DNS);
	ADD_VECTOR_END(result, PT_OUTBOUND_DNS);
#endif /* HAS_PROTOCOL_DNS */
	ADD_VECTOR_END(result, PT_TIMER);
#ifdef HAS_PROTOCOL_TS
	ADD_VECTOR_END(result, PT_INBOUND_TS);
#endif /* HAS_PROTOCOL_TS */
#ifdef HAS_PROTOCOL_RTMP
	ADD_VECTOR_END(result, PT_INBOUND_RTMP);
	ADD_VECTOR_END(result, PT_INBOUND_RTMPS_DISC);
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
	ADD_VECTOR_END(result, PT_RTP_NAT_TRAVERSAL);
#endif /* HAS_PROTOCOL_RTP */
#ifdef HAS_PROTOCOL_CLI
	ADD_VECTOR_END(result, PT_INBOUND_JSONCLI);
	ADD_VECTOR_END(result, PT_HTTP_4_CLI);
#endif /* HAS_PROTOCOL_CLI */
#ifdef HAS_PROTOCOL_MMS
	ADD_VECTOR_END(result, PT_OUTBOUND_MMS);
#endif /* HAS_PROTOCOL_MMS */
#ifdef HAS_PROTOCOL_RAWHTTPSTREAM
	ADD_VECTOR_END(result, PT_INBOUND_RAW_HTTP_STREAM);
#endif /* HAS_PROTOCOL_RAWHTTPSTREAM */
	return result;
}

vector<string> DefaultProtocolFactory::HandledProtocolChains() {
	vector<string> result;
#ifdef HAS_PROTOCOL_DNS
	ADD_VECTOR_END(result, CONF_PROTOCOL_INBOUND_DNS);
	ADD_VECTOR_END(result, CONF_PROTOCOL_OUTBOUND_DNS);
#endif /* HAS_PROTOCOL_DNS */
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
	ADD_VECTOR_END(result, CONF_PROTOCOL_RTP_NAT_TRAVERSAL);
#endif /* HAS_PROTOCOL_RTP */
#ifdef HAS_PROTOCOL_CLI
	ADD_VECTOR_END(result, CONF_PROTOCOL_INBOUND_CLI_JSON);
#ifdef HAS_PROTOCOL_HTTP
	ADD_VECTOR_END(result, CONF_PROTOCOL_INBOUND_HTTP_CLI_JSON);
#endif /* HAS_PROTOCOL_HTTP */
#endif /* HAS_PROTOCOL_CLI */
#ifdef HAS_PROTOCOL_MMS
	ADD_VECTOR_END(result, CONF_PROTOCOL_OUTBOUND_MMS);
#endif /* HAS_PROTOCOL_MMS */
#ifdef HAS_PROTOCOL_RAWHTTPSTREAM
	ADD_VECTOR_END(result, CONF_PROTOCOL_INBOUND_RAW_HTTP_STREAM);
	ADD_VECTOR_END(result, CONF_PROTOCOL_INBOUND_RAW_HTTPS_STREAM);
#endif /* HAS_PROTOCOL_RAWHTTPSTREAM */
	return result;
}

vector<uint64_t> DefaultProtocolFactory::ResolveProtocolChain(string name) {
	vector<uint64_t> result;
	if (false) {

	}
#ifdef HAS_PROTOCOL_DNS
	else if (name == CONF_PROTOCOL_INBOUND_DNS) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_INBOUND_DNS);
	} else if (name == CONF_PROTOCOL_OUTBOUND_DNS) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_OUTBOUND_DNS);
	}
#endif /* HAS_PROTOCOL_DNS */
#ifdef HAS_PROTOCOL_RTMP
	else if (name == CONF_PROTOCOL_INBOUND_RTMP) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_INBOUND_RTMP);
	} else if (name == CONF_PROTOCOL_OUTBOUND_RTMP) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_OUTBOUND_RTMP);
	} else if (name == CONF_PROTOCOL_INBOUND_RTMPS) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_INBOUND_SSL);
		ADD_VECTOR_END(result, PT_INBOUND_RTMPS_DISC);
	}
#ifdef HAS_PROTOCOL_HTTP
	else if (name == CONF_PROTOCOL_INBOUND_RTMPT) {
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
	} else if (name == CONF_PROTOCOL_RTP_NAT_TRAVERSAL) {
		ADD_VECTOR_END(result, PT_UDP);
		ADD_VECTOR_END(result, PT_RTP_NAT_TRAVERSAL);
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
#ifdef HAS_PROTOCOL_CLI
	else if (name == CONF_PROTOCOL_INBOUND_CLI_JSON) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_INBOUND_JSONCLI);
	}
#ifdef HAS_PROTOCOL_HTTP
	else if (name == CONF_PROTOCOL_INBOUND_HTTP_CLI_JSON) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_INBOUND_HTTP);
		ADD_VECTOR_END(result, PT_HTTP_4_CLI);
		ADD_VECTOR_END(result, PT_INBOUND_JSONCLI);
	}
#endif /* HAS_PROTOCOL_HTTP */
#endif /* HAS_PROTOCOL_CLI */
#ifdef HAS_PROTOCOL_MMS
	else if (name == CONF_PROTOCOL_OUTBOUND_MMS) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_OUTBOUND_MMS);
	}
#endif /* HAS_PROTOCOL_MMS */
#ifdef HAS_PROTOCOL_RAWHTTPSTREAM
	else if (name == CONF_PROTOCOL_INBOUND_RAW_HTTP_STREAM) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_INBOUND_RAW_HTTP_STREAM);
	} else if (name == CONF_PROTOCOL_INBOUND_RAW_HTTPS_STREAM) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_INBOUND_SSL);
		ADD_VECTOR_END(result, PT_INBOUND_RAW_HTTP_STREAM);
	}
#endif /* HAS_PROTOCOL_RAWHTTPSTREAM */
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
		case PT_INBOUND_SSL:
			pResult = new InboundSSLProtocol();
			break;
		case PT_OUTBOUND_SSL:
			pResult = new OutboundSSLProtocol();
			break;
#ifdef HAS_PROTOCOL_DNS
		case PT_INBOUND_DNS:
			pResult = new InboundDNSResolverProtocol();
			break;
		case PT_OUTBOUND_DNS:
			pResult = new OutboundDNSResolverProtocol();
			break;
#endif /* HAS_PROTOCOL_DNS */
#ifdef HAS_PROTOCOL_RTMP
		case PT_INBOUND_RTMP:
			pResult = new InboundRTMPProtocol();
			break;
		case PT_INBOUND_RTMPS_DISC:
			pResult = new InboundRTMPSDiscriminatorProtocol();
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
		case PT_RTP_NAT_TRAVERSAL:
			pResult = new NATTraversalProtocol();
			break;
#endif /* HAS_PROTOCOL_RTP */
#ifdef HAS_PROTOCOL_CLI
		case PT_INBOUND_JSONCLI:
			pResult = new InboundJSONCLIProtocol();
			break;
		case PT_HTTP_4_CLI:
			pResult = new HTTP4CLIProtocol();
			break;
#endif /* HAS_PROTOCOL_CLI */
#ifdef HAS_PROTOCOL_MMS
		case PT_OUTBOUND_MMS:
			pResult = new MMSProtocol();
			break;
#endif /* HAS_PROTOCOL_MMS */
#ifdef HAS_PROTOCOL_RAWHTTPSTREAM
		case PT_INBOUND_RAW_HTTP_STREAM:
			pResult = new InboundRawHTTPStreamProtocol();
			break;
#endif /* HAS_PROTOCOL_RAWHTTPSTREAM */
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

