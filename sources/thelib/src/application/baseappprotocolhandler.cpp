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


#include "common.h"
#include "application/baseappprotocolhandler.h"
#include "application/baseclientapplication.h"

BaseAppProtocolHandler::BaseAppProtocolHandler(Variant &configuration) {
	_configuration = configuration;
}

BaseAppProtocolHandler::~BaseAppProtocolHandler() {
}

bool BaseAppProtocolHandler::ParseAuthenticationNode(Variant &node, Variant &result) {
	return false;
}

void BaseAppProtocolHandler::SetApplication(BaseClientApplication *pApplication) {
	_pApplication = pApplication;
}

BaseClientApplication *BaseAppProtocolHandler::GetApplication() {
	return _pApplication;
}

BaseAppProtocolHandler * BaseAppProtocolHandler::GetProtocolHandler(uint64_t protocolType) {
	if (_pApplication == NULL)
		return NULL;
	return _pApplication->GetProtocolHandler(protocolType);
}

bool BaseAppProtocolHandler::PullExternalStream(URI uri, Variant streamConfig) {
	WARN("Pulling in streams for scheme %s in application %s not yet implemented. Stream configuration was:\n%s",
			STR(uri.scheme()),
			STR(GetApplication()->GetName()),
			STR(streamConfig.ToString()));
	return false;
}

bool BaseAppProtocolHandler::PushLocalStream(Variant streamConfig) {
	WARN("Pushing out streams for this protocol handler in application %s not yet implemented.",
			STR(GetApplication()->GetName()));
	return false;
}
