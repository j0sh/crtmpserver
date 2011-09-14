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


#ifdef HAS_PROTOCOL_VAR
#include "protocols/variant/basevariantappprotocolhandler.h"
#include "application/baseclientapplication.h"
#include "protocols/protocolfactorymanager.h"
#include "application/baseclientapplication.h"
#include "netio/netio.h"
#include "application/clientapplicationmanager.h"
#include "protocols/variant/basevariantprotocol.h"

BaseVariantAppProtocolHandler::BaseVariantAppProtocolHandler(Variant &configuration)
: BaseAppProtocolHandler(configuration) {
	_urlCache["dummy"] = "dummy";
	_urlCache.RemoveKey("dummy");
#ifdef HAS_PROTOCOL_HTTP
	_outboundHttpBinVariant = ProtocolFactoryManager::ResolveProtocolChain(
			CONF_PROTOCOL_OUTBOUND_HTTP_BIN_VARIANT);
	_outboundHttpXmlVariant = ProtocolFactoryManager::ResolveProtocolChain(
			CONF_PROTOCOL_OUTBOUND_HTTP_XML_VARIANT);
	_outboundBinVariant = ProtocolFactoryManager::ResolveProtocolChain(
			CONF_PROTOCOL_OUTBOUND_BIN_VARIANT);
	_outboundXmlVariant = ProtocolFactoryManager::ResolveProtocolChain(
			CONF_PROTOCOL_OUTBOUND_XML_VARIANT);
	if (_outboundHttpBinVariant.size() == 0) {
		ASSERT("Unable to resolve protocol stack %s",
				CONF_PROTOCOL_OUTBOUND_HTTP_BIN_VARIANT);
	}
	if (_outboundHttpXmlVariant.size() == 0) {
		ASSERT("Unable to resolve protocol stack %s",
				CONF_PROTOCOL_OUTBOUND_HTTP_XML_VARIANT);
	}
	if (_outboundBinVariant.size() == 0) {
		ASSERT("Unable to resolve protocol stack %s",
				CONF_PROTOCOL_OUTBOUND_BIN_VARIANT);
	}
	if (_outboundXmlVariant.size() == 0) {
		ASSERT("Unable to resolve protocol stack %s",
				CONF_PROTOCOL_OUTBOUND_XML_VARIANT);
	}
#else
	FATAL("HTTP protocol not supported");
#endif /* HAS_PROTOCOL_HTTP */
}

BaseVariantAppProtocolHandler::~BaseVariantAppProtocolHandler() {
}

void BaseVariantAppProtocolHandler::RegisterProtocol(BaseProtocol *pProtocol) {
}

void BaseVariantAppProtocolHandler::UnRegisterProtocol(BaseProtocol *pProtocol) {
}

bool BaseVariantAppProtocolHandler::Send(string ip, uint16_t port, Variant &variant, bool xml) {
	//1. Build the parameters
	Variant parameters;
	parameters["ip"] = ip;
	parameters["port"] = (uint16_t) port;
	parameters["applicationName"] = GetApplication()->GetName();
	parameters["payload"] = variant;

	//2. Start the HTTP request
	if (!TCPConnector<BaseVariantAppProtocolHandler>::Connect(parameters["ip"],
			parameters["port"],
			xml ? _outboundXmlVariant : _outboundBinVariant,
			parameters)) {
		FATAL("Unable to open connection");
		return false;
	}

	return true;
}

bool BaseVariantAppProtocolHandler::Send(string url, Variant &variant, bool xml) {
	//1. Build the parameters
	Variant parameters = GetScaffold(url);
	if (parameters != V_MAP) {
		FATAL("Unable to get parameters scaffold");
		return false;
	}
	parameters["payload"] = variant;

	//2. Start the HTTP request
	if (!TCPConnector<BaseVariantAppProtocolHandler>::Connect(parameters["ip"],
			parameters["port"],
			xml ? _outboundHttpXmlVariant : _outboundHttpBinVariant,
			parameters)) {
		FATAL("Unable to open connection");
		return false;
	}

	return true;
}

bool BaseVariantAppProtocolHandler::SignalProtocolCreated(BaseProtocol *pProtocol, Variant &parameters) {
	//1. Get the application
	BaseClientApplication *pApplication = ClientApplicationManager::FindAppByName(
			parameters["applicationName"]);
	if (pApplication == NULL) {
		FATAL("Unable to find application %s",
				STR(parameters["applicationName"]));
		return false;
	}

	//2. get the protocol handler
	BaseAppProtocolHandler *pHandler = pApplication->GetProtocolHandler(PT_BIN_VAR);
	if (pHandler == NULL) {
		pHandler = pApplication->GetProtocolHandler(PT_XML_VAR);
		if (pHandler == NULL) {
			WARN("Unable to get protocol handler for variant protocol");
		}
	}


	//3. Is the connection up
	if (pProtocol == NULL) {
		if (pHandler != NULL) {
			((BaseVariantAppProtocolHandler *) pHandler)->ConnectionFailed(parameters);
		} else {
			WARN("Connection failed:\n%s", STR(parameters.ToString()));
		}
		return false;
	}

	//1. Validate the protocol
	if (pProtocol->GetType() != PT_BIN_VAR &&
			pProtocol->GetType() != PT_XML_VAR) {
		FATAL("Invalid protocol type. Wanted: %s or %s; Got: %s",
				STR(tagToString(PT_BIN_VAR)),
				STR(tagToString(PT_XML_VAR)),
				STR(tagToString(pProtocol->GetType())));
		return false;
	}

	//3. Register the protocol to it
	pProtocol->SetApplication(pApplication);

	if (pProtocol->GetFarProtocol() == NULL) {
		FATAL("Invalid far protocol");
		return false;
	}

	//4. Do the actual request
	if (pProtocol->GetFarProtocol()->GetType() == PT_TCP) {
		return ((BaseVariantProtocol *) pProtocol)->Send(parameters["payload"]);
	} else {
		return ((BaseVariantProtocol *) pProtocol)->Send(parameters);
	}
}

void BaseVariantAppProtocolHandler::ConnectionFailed(Variant &parameters) {
	WARN("Connection failed:\n%s", STR(parameters.ToString()));
}

bool BaseVariantAppProtocolHandler::ProcessMessage(BaseVariantProtocol *pProtocol,
		Variant &lastSent, Variant &lastReceived) {
	FINEST("lastSent:\n%s", STR(lastSent.ToString()));
	FINEST("lastReceived:\n%s", STR(lastReceived.ToString()));
	return true;
}

Variant BaseVariantAppProtocolHandler::GetScaffold(string uriString) {
	//1. Search in the cache first
	if (_urlCache.HasKey(uriString)) {
		return _urlCache[uriString];
	}

	//2. Build it
	Variant result;

	//3. Split the URL into components
	URI uri;
	if (!URI::FromString(uriString, true, uri)) {
		FATAL("Invalid url: %s", STR(uriString));
		return Variant();
	}

	//6. build the end result
	result["username"] = uri.userName();
	result["password"] = uri.password();
	result["host"] = uri.host();
	result["ip"] = uri.ip();
	result["port"] = uri.port();
	result["document"] = uri.fullDocumentPath();
	result["applicationName"] = GetApplication()->GetName();

	//7. Save it in the cache
	_urlCache[uriString] = result;

	//8. Done
	return result;
}
#endif	/* HAS_PROTOCOL_VAR */

