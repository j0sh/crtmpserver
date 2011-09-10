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


#include "samplefactoryapplication.h"
#include "protocols/baseprotocol.h"
#include "protocolfactory.h"
#include "protocols/protocolfactorymanager.h"
#include "localdefines.h"
#include "echoappprotocolhandler.h"
using namespace app_samplefactory;

SampleFactoryApplication::SampleFactoryApplication(Variant &configuration)
: BaseClientApplication(configuration) {
	_pEchoHandler = NULL;
}

SampleFactoryApplication::~SampleFactoryApplication() {
	UnRegisterAppProtocolHandler(PT_ECHO_PROTOCOL);
	if (_pEchoHandler != NULL) {
		delete _pEchoHandler;
		_pEchoHandler = NULL;
	}
}

bool SampleFactoryApplication::Initialize() {
	if (!BaseClientApplication::Initialize()) {
		FATAL("Unable to initialize application");
		return false;
	}
	//TODO: Add your app init code here
	//Things like parsing custom sections inside _configuration for example,
	//initialize the protocol handler(s)

	//1. Initialize the protocol handler(s)
	_pEchoHandler = new EchoAppProtocolHandler(_configuration);
	RegisterAppProtocolHandler(PT_ECHO_PROTOCOL, _pEchoHandler);

	return true;
}

