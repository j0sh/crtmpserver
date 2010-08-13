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

#include "api.h"
#include "protocols/protocolfactorymanager.h"
#include "protocols/defaultprotocolfactory.h"
#include "protocols/protocolmanager.h"
#include "netio/netio.h"
#include "applestreamingclientapplication.h"
#include "application/clientapplicationmanager.h"
#include "variantconnection.h"
#include "protocols/variant/messagestructure.h"

static sockaddr_in gAddress = {0};

#define SEND_VARIANT_REQUEST(request,response) \
do { \
	VariantConnection vc; \
	if (!vc.Connect(gAddress)) { \
		FATAL("Unable to connect to worker thread"); \
		response.Reset(); \
	} else { \
		if (!vc.SendMessage(request, response)) { \
			response.Reset(); \
		} \
	} \
	/*FINEST("request:\n%s\nresponse:\n%s",STR(request.ToString()),STR(response.ToString()));*/ \
}while(0)

void EnvRun(string ip, uint16_t port) {
	//1. Initialize the logger
	Logger::Init();
	BaseLogLocation *pLogLocation = new ConsoleLogLocation(true);
	pLogLocation->SetLevel(_FINEST_);
	Logger::AddLogLocation(pLogLocation);
#ifdef ANDROID
	pLogLocation = new LogCatLogLocation();
	pLogLocation->SetLevel(_FINEST_);
	Logger::AddLogLocation(pLogLocation);
#endif /* ANDROID */

	//2. Create the default protocol factory
	DefaultProtocolFactory *pFactory = new DefaultProtocolFactory();
	if (!ProtocolFactoryManager::RegisterProtocolFactory(pFactory)) {
		ASSERT("Unable to register default protocols factory");
	}

	//3. Initialize the I/O
	IOHandlerManager::Initialize();

	//4. Create the AppleStreamingClientApplication application
	Variant configuration;
	configuration[CONF_APPLICATION_NAME] = "applestreaminclient";
	configuration[CONF_APPLICATION_ALIASES].PushToArray("asc");
	configuration[CONF_APPLICATION_DEFAULT] = (bool)true;
	configuration[CONF_APPLICATION_VALIDATEHANDSHAKE] = (bool)false;
	configuration[CONF_APPLICATION_KEYFRAMESEEK] = (bool)true;
	configuration[CONF_APPLICATION_CLIENTSIDEBUFFER] = (int32_t) 15;
	configuration[CONF_APPLICATION_SEEKGRANULARITY] = 1.00;
	configuration[CONF_APPLICATION_MEDIAFOLDER] = "./";
	configuration[CONF_APPLICATION_GENERATE_META_FILES] = (bool)false;
	AppleStreamingClientApplication *pApp = new AppleStreamingClientApplication(
			configuration);
	if (!pApp->Initialize()) {
		ASSERT("Unable to initialize the application");
	}

	//6. Register it to the app manager
	if (!ClientApplicationManager::RegisterApplication(pApp)) {
		ASSERT("Unable to register application");
	}

	//7. Create the RTSP acceptor
	Variant acceptorConfig;
	acceptorConfig[CONF_IP] = "127.0.0.1";
	acceptorConfig[CONF_PORT] = (uint16_t) port;
	acceptorConfig[CONF_PROTOCOL] = CONF_PROTOCOL_INBOUND_RTSP;
	vector<uint64_t> chain;
	chain = ProtocolFactoryManager::ResolveProtocolChain(CONF_PROTOCOL_INBOUND_RTSP);
	if (chain.size() == 0) {
		ASSERT("Invalid protocol chain: %s", CONF_PROTOCOL_INBOUND_RTSP);
	}
	TCPAcceptor *pAcceptor = new TCPAcceptor(ip, port, acceptorConfig, chain);
	if (!pAcceptor->StartAccept(pApp)) {
		ASSERT("Unable to fire up acceptor");
	}

	//8. Create the bin variant acceptor
	acceptorConfig[CONF_PORT] = (uint16_t) (port + 1);
	acceptorConfig[CONF_PROTOCOL] = CONF_PROTOCOL_INBOUND_BIN_VARIANT;
	chain = ProtocolFactoryManager::ResolveProtocolChain(CONF_PROTOCOL_INBOUND_BIN_VARIANT);
	if (chain.size() == 0) {
		ASSERT("Invalid protocol chain: %s", CONF_PROTOCOL_INBOUND_BIN_VARIANT);
	}
	pAcceptor = new TCPAcceptor(ip, port + 1, acceptorConfig, chain);
	if (!pAcceptor->StartAccept(pApp)) {
		ASSERT("Unable to fire up acceptor");
	}

	//9. Create the xml variant acceptor
	acceptorConfig[CONF_PORT] = (uint16_t) (port + 2);
	acceptorConfig[CONF_PROTOCOL] = CONF_PROTOCOL_INBOUND_XML_VARIANT;
	chain = ProtocolFactoryManager::ResolveProtocolChain(CONF_PROTOCOL_INBOUND_XML_VARIANT);
	if (chain.size() == 0) {
		ASSERT("Invalid protocol chain: %s", CONF_PROTOCOL_INBOUND_XML_VARIANT);
	}
	pAcceptor = new TCPAcceptor(ip, port + 2, acceptorConfig, chain);
	if (!pAcceptor->StartAccept(pApp)) {
		ASSERT("Unable to fire up acceptor");
	}

	inet_aton("127.0.0.1", &gAddress.sin_addr);
	gAddress.sin_family = AF_INET;
	gAddress.sin_port = htons(port + 1);

	//10. Run
	while (IOHandlerManager::Pulse()) {
		IOHandlerManager::DeleteDeadHandlers();
		ProtocolManager::CleanupDeadProtocols();
	}

	//11. Shutting down protocols manager;
	ProtocolManager::Shutdown();
	ProtocolManager::CleanupDeadProtocols();

	//12. Shutting down I/O handlers manager
	IOHandlerManager::ShutdownIOHandlers();
	IOHandlerManager::DeleteDeadHandlers();
	IOHandlerManager::Shutdown();

	//13. Unregister and delete default protocol handler
	ProtocolFactoryManager::UnRegisterProtocolFactory(pFactory);
	delete pFactory;
	pFactory = NULL;

	//14. Shutting down applications
	ClientApplicationManager::Shutdown();

	//15. Shutting down the logger leaving you in the dark. Bye bye
	Logger::Free(true);
}

void EnvStop() {
	IOHandlerManager::SignalShutdown();
}

Variant ContextCreate() {
	Variant request;
	ASC_REQ_BUILD_CONTEXT_CREATE(request);
	Variant response;
	SEND_VARIANT_REQUEST(request, response);
	return response;
}

Variant ContextList() {
	Variant request;
	ASC_REQ_BUILD_CONTEXT_LIST(request);
	Variant response;
	SEND_VARIANT_REQUEST(request, response);
	return response;
}

Variant ContextClose(uint32_t contextId) {
	Variant request;
	ASC_REQ_BUILD_CONTEXT_CLOSE(request, contextId);
	Variant response;
	SEND_VARIANT_REQUEST(request, response);
	return response;
}

Variant ContextCloseAll() {
	Variant request;
	ASC_REQ_BUILD_CONTEXT_CLOSE_ALL(request);
	Variant response;
	SEND_VARIANT_REQUEST(request, response);
	return response;
}

Variant CommandPlay(uint32_t contextId, string m3u8Uri, string httpSessionId,
		string keyPassword) {
	Variant request;
	ASC_REQ_BUILD_COMMAND_PLAY(request, contextId, m3u8Uri, httpSessionId, keyPassword);
	Variant response;
	SEND_VARIANT_REQUEST(request, response);
	return response;
}

Variant CommandPause(uint32_t contextId) {
	Variant request;
	ASC_REQ_BUILD_COMMAND_PAUSE(request, contextId);
	Variant response;
	SEND_VARIANT_REQUEST(request, response);
	return response;
}

Variant CommandResume(uint32_t contextId) {
	Variant request;
	ASC_REQ_BUILD_COMMAND_RESUME(request, contextId);
	Variant response;
	SEND_VARIANT_REQUEST(request, response);
	return response;
}

Variant CommandSelectBandwidth(uint32_t contextId, uint32_t bandwidth) {
	Variant request;
	ASC_REQ_BUILD_COMMAND_SELECT_BANDWIDTH(request, contextId, bandwidth);
	Variant response;
	SEND_VARIANT_REQUEST(request, response);
	return response;
}

Variant CommandSeek(uint32_t contextId, double value) {
	Variant request;
	ASC_REQ_BUILD_COMMAND_SEEK(request, contextId, value);
	Variant response;
	SEND_VARIANT_REQUEST(request, response);
	return response;
}

Variant CommandSelectAVChannels(uint32_t contextId, uint32_t audioChannel,
		uint32_t videoChannel) {
	Variant request;
	ASC_REQ_BUILD_COMMAND_SELECT_AV_CHANNELS(request, contextId, audioChannel, videoChannel);
	Variant response;
	SEND_VARIANT_REQUEST(request, response);
	return response;
}

Variant InfoListStreams(uint32_t contextId) {
	Variant request;
	ASC_REQ_BUILD_INFO_LIST_STREAMS(request, contextId);
	Variant response;
	SEND_VARIANT_REQUEST(request, response);
	return response;
}

Variant InfoListAllStreams() {
	Variant request;
	ASC_REQ_BUILD_INFO_LIST_ALL_STREAMS(request);
	Variant response;
	SEND_VARIANT_REQUEST(request, response);
	return response;
}

Variant InfoBandwidth(uint32_t contextId) {
	Variant request;
	ASC_REQ_BUILD_INFO_BANDWIDTH(request, contextId);
	Variant response;
	SEND_VARIANT_REQUEST(request, response);
	return response;
}

Variant InfoPlayback(uint32_t contextId) {
	Variant request;
	ASC_REQ_BUILD_INFO_PLAYBACK(request, contextId);
	Variant response;
	SEND_VARIANT_REQUEST(request, response);
	return response;
}
