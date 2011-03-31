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
#include "monitorapplication.h"
#include "application/clientapplicationmanager.h"

bool ValidateParameters(Variant &parameters);

int main(int argc, char **ppArgv) {
	//1. Init the logger
	Logger::Init();
	ConsoleLogLocation *pLogLocation = new ConsoleLogLocation();
	pLogLocation->SetLevel(_FINEST_);
	Logger::AddLogLocation(pLogLocation);
	INFO("Traffic dissector started (%u)", (uint32_t)getpid());

	//2. Parse the parameters
	if (argc != 2) {
		FATAL("Invalid parameters");
		return -1;
	}
	Variant parameters;
	if (!ReadLuaFile(ppArgv[1], "config", parameters)) {
		FATAL("Unable to read config file");
		return -1;
	}

	//3. Validate the parameters
	if (!ValidateParameters(parameters)) {
		FATAL("Unable to validate parameters");
		return -1;
	}

	//4. Initialize the monitor application
	Variant monitorApplicationSettings;
	monitorApplicationSettings[CONF_APPLICATION_NAME] = "monitor";
	monitorApplicationSettings[CONF_APPLICATION_DEFAULT] = (bool)true;
	monitorApplicationSettings[CONF_APPLICATION_MEDIAFOLDER] = "/somewhere";
	MonitorApplication application(monitorApplicationSettings);
	ClientApplicationManager::RegisterApplication(&application);


	ASSERT("\n%s", STR(parameters.ToString()));


	return 0;
}

bool ValidateParameters(Variant &parameters) {
	NYIR;
}
