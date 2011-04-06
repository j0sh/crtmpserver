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


#include "vmappapplication.h"
#include "rtmpappprotocolhandler.h"
#include "protocols/baseprotocol.h"
#include "vm/baseappvirtualmachine.h"
#include "rtspappprotocolhandler.h"
#include "rtpappprotocolhandler.h"
#include "rtmpappprotocolhandler.h"
#include "tsappprotocolhandler.h"
using namespace app_vmapp;

VMAppApplication::VMAppApplication(Variant &configuration)
: BaseClientApplication(configuration) {
	_pAppVM = NULL;
	_pRTMPHandler = NULL;
	_pRTPHandler = NULL;
	_pRTSPHandler = NULL;
	_pTSHandler = NULL;
}

VMAppApplication::~VMAppApplication() {
	if (_pAppVM != NULL) {
		delete _pAppVM;
		_pAppVM = NULL;
	}
	UnRegisterAppProtocolHandler(PT_INBOUND_RTMP);
	UnRegisterAppProtocolHandler(PT_OUTBOUND_RTMP);
	if (_pRTMPHandler != NULL) {
		delete _pRTMPHandler;
		_pRTMPHandler = NULL;
	}
	UnRegisterAppProtocolHandler(PT_INBOUND_RTP);
	if (_pRTPHandler != NULL) {
		delete _pRTPHandler;
		_pRTPHandler = NULL;
	}

	UnRegisterAppProtocolHandler(PT_RTSP);
	if (_pRTSPHandler != NULL) {
		delete _pRTSPHandler;
		_pRTSPHandler = NULL;
	}

	UnRegisterAppProtocolHandler(PT_INBOUND_TS);
	if (_pTSHandler != NULL) {
		delete _pTSHandler;
		_pTSHandler = NULL;
	}
}

bool VMAppApplication::Initialize() {
	//1. Normalize the path to the script
	if ((!_configuration.HasKey("script"))
			|| (_configuration["script"] != V_STRING)
			|| (_configuration["script"] == "")) {
		FATAL("script value provided in the config file is incorrect");
		return false;
	}
	string script = _configuration["script"];
	if (script[0] != '/') {
		script = normalizePath(_configuration[CONF_APPLICATION_DIRECTORY], script);
	}
	if (script == "") {
		FATAL("file `%s` not found", STR(_configuration["script"]));
		return false;
	}
	_configuration["script"] = script;

	//2. Get the virtual machine type
	if ((!_configuration.HasKey("vmType"))
			|| (_configuration["vmType"] != V_STRING)
			|| (_configuration["vmType"] == "")) {
		FATAL("vmType value provided in the config file is incorrect");
		return false;
	}
	_configuration["vmType"] = lowerCase(_configuration["vmType"]);

	//3. Create the virtual machine
	_pAppVM = BaseAppVirtualMachine::GetVM(_configuration["vmType"],
			_configuration["script"], this);
	if (_pAppVM == NULL) {
		FATAL("Unable to get the %s virtual machine with script %s",
				STR(_configuration["vmType"]),
				STR(_configuration["script"]));
		return false;
	}

	//4. Plug in the handlers
	_pRTMPHandler = new RTMPAppProtocolHandler(_configuration, _pAppVM);
	RegisterAppProtocolHandler(PT_INBOUND_RTMP, _pRTMPHandler);
	RegisterAppProtocolHandler(PT_INBOUND_RTMPS_DISC, _pRTMPHandler);
	RegisterAppProtocolHandler(PT_OUTBOUND_RTMP, _pRTMPHandler);
	_pRTPHandler = new RTPAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_INBOUND_RTP, _pRTPHandler);
	RegisterAppProtocolHandler(PT_RTCP, _pRTPHandler);
	_pRTSPHandler = new RTSPAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_RTSP, _pRTSPHandler);
	_pTSHandler = new TSAppProtocolHandler(_configuration, _pAppVM);
	RegisterAppProtocolHandler(PT_INBOUND_TS, _pTSHandler);

	if (_pAppVM->HasFunction(handlers_application_initCompleted)) {
		return _pAppVM->Call(handlers_application_initCompleted);
	}

	//5. Done
	return true;
}

bool VMAppApplication::OutboundConnectionFailed(Variant &customParameters) {
	if (_pAppVM->HasFunction(handlers_application_outboundConnectionFailed)) {
		_pAppVM->Call(handlers_application_outboundConnectionFailed,
				customParameters);
	}
	return false;
}

void VMAppApplication::RegisterProtocol(BaseProtocol *pProtocol) {
	if (_pAppVM->HasFunction(handlers_application_preRegisterProtocol)) {
		_pAppVM->Call(handlers_application_preRegisterProtocol, pProtocol);
	}
	BaseClientApplication::RegisterProtocol(pProtocol);
	if (_pAppVM->HasFunction(handlers_application_postRegisterProtocol)) {
		_pAppVM->Call(handlers_application_postRegisterProtocol, pProtocol);
	}
}

void VMAppApplication::UnRegisterProtocol(BaseProtocol *pProtocol) {
	if (_pAppVM->HasFunction(handlers_application_preUnRegisterProtocol)) {
		_pAppVM->Call(handlers_application_preUnRegisterProtocol, pProtocol);
	}
	BaseClientApplication::UnRegisterProtocol(pProtocol);
	if (_pAppVM->HasFunction(handlers_application_postUnRegisterProtocol)) {
		_pAppVM->Call(handlers_application_postUnRegisterProtocol, pProtocol);
	}
}

void VMAppApplication::SignalStreamRegistered(BaseStream *pStream) {
	if (_pAppVM->HasFunction(handlers_application_signalStreamRegistered)) {
		_pAppVM->Call(handlers_application_signalStreamRegistered, pStream);
	}
}

void VMAppApplication::SignalStreamUnRegistered(BaseStream *pStream) {
	if (_pAppVM->HasFunction(handlers_application_signalStreamUnRegistered)) {
		_pAppVM->Call(handlers_application_signalStreamUnRegistered, pStream);
	}
}

bool VMAppApplication::PullExternalStream(Variant streamConfig) {
	if (_pAppVM->HasFunction(handlers_application_pullExternalStream))
		return _pAppVM->Call(handlers_application_pullExternalStream, streamConfig);
	else
		return BaseClientApplication::PullExternalStream(streamConfig);
}

bool VMAppApplication::PushLocalStream(Variant streamConfig) {
	if (_pAppVM->HasFunction(handlers_application_pushLocalStream))
		return _pAppVM->Call(handlers_application_pushLocalStream, streamConfig);
	else
		return BaseClientApplication::PushLocalStream(streamConfig);
}
