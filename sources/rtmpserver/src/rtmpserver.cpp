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

#include "netio/netio.h"
#include "configuration/configfile.h"
#include "protocols/protocolmanager.h"
#include "application/clientapplicationmanager.h"
#include "protocols/protocolfactorymanager.h"
#include "protocols/defaultprotocolfactory.h"

//This is a structure that holds the state
//between config re-reads/re-runs

struct RunningStatus {
	// startup parameters
	int argc;
	char **argv;

	//Configuration file
	ConfigFile *pConfigFile;

	//default protocol factory
	DefaultProtocolFactory * pProtocolFactory;

	//should we run again?
	bool run;

	//is this a daemon already?
	bool daemon;
};

void QuitSignalHandler(void);
void ConfRereadSignalHandler(void);
bool Initialize();
void Run();
void Cleanup();

RunningStatus gRs = {0};

#ifdef COMPILE_STATIC
BaseClientApplication *SpawnApplication(Variant configuration);
#endif

int main(int argc, char **argv) {
	SRAND();
	InitNetworking();

	//1. Pick up the startup parameters and hold them inside the running status
	gRs.argc = argc;
	gRs.argv = argv;

	do {
		//2. Reset the run flag
		gRs.run = false;

		//3. Initialize the running status
		if (Initialize()) {
			Run();
		} else {
			gRs.run = false;
		}

		//5. Cleanup
		Cleanup();
	} while (gRs.run);

	//6. We are done
	return 0;
}

bool Initialize() {
	Logger::Init();

	if (gRs.argc < 2) {
		printf("Usage: %s [--use-implicit-console-appender] <config_file_path>\n", gRs.argv[0]);
		printf("\n--use-implicit-console-appender  use this switch to add a default console\n");
		printf("appender. Used when you are not sure about the logeAppenders section in the config file\n");
		return false;
	}

	for (int i = 1; i < gRs.argc - 1; i++) {
		if (string(gRs.argv[i]) == "--use-implicit-console-appender") {
			ConsoleLogLocation *pLogLocation = new ConsoleLogLocation(false);
			pLogLocation->SetLevel(_FINEST_);
			Logger::AddLogLocation(pLogLocation);
		}
	}

	INFO("Reading configuration from %s", gRs.argv[gRs.argc - 1]);
#ifdef COMPILE_STATIC
	gRs.pConfigFile = new ConfigFile(SpawnApplication);
#else
	gRs.pConfigFile = new ConfigFile(NULL);
#endif
	string configFilePath = gRs.argv[gRs.argc - 1];
	string fileName;
	string extension;
	splitFileName(configFilePath, fileName, extension);

	if (lowercase(extension) == "xml") {
		if (!gRs.pConfigFile->LoadXmlFile(configFilePath)) {
			FATAL("Unable to load file %s", STR(configFilePath));
			return false;
		}
	} else if (lowercase(extension) == "lua") {
#ifdef HAS_LUA
		if (!gRs.pConfigFile->LoadLuaFile(configFilePath)) {
			FATAL("Unable to load file %s", STR(configFilePath));
			return false;
		}
#else
		cout << "Lua is not supported by the current build of the server" << endl;
		ASSERT("Lua is not supported by the current build of the server");
		return false;
#endif /* HAS_LUA */
	} else {
		FATAL("Invalid file format: %s", STR(configFilePath));
		return false;
	}

#ifndef WIN32
	if (gRs.pConfigFile->IsDaemon()) {
		if (!gRs.daemon) {
			INFO("Daemonize...");
			pid_t pid = fork();
			if (pid < 0) {
				FATAL("Unable to start daemonize. fork() failed");
				return false;
			}

			if (pid > 0) {
				return false;
			}

			FINEST("Create a new SID for the daemon");
			pid_t sid = setsid();
			if (sid < 0) {
				FATAL("Unable to start daemonize. setsid() failed");
				return false;
			}

			gRs.daemon = true;
		}
	}
#endif /* WIN32 */

	INFO("Configure log appenders from configuration file");
	if (!gRs.pConfigFile->ConfigureLogAppenders()) {
		WARN("Unable to configure log appenders");
	}

	INFO("Plug in the default protocol factory");
	gRs.pProtocolFactory = new DefaultProtocolFactory();
	if (!ProtocolFactoryManager::RegisterProtocolFactory(gRs.pProtocolFactory)) {
		FATAL("Unable to register default protocols factory");
		return false;
	}

	INFO("Initialize I/O handlers manager: %s", NETWORK_REACTOR);
	IOHandlerManager::Initialize();

	INFO("Initialize DNS resolver");
	if (!gRs.pConfigFile->ConfigureDNSResolver()) {
		FATAL("Unable to configure DNS resolver");
		return false;
	}

	INFO("Initialize applications");
	if (!gRs.pConfigFile->ConfigureApplications()) {
		FATAL("Unable to configure applications");
		return false;
	}

	INFO("Install the quit signal");
	InstallQuitSignal(QuitSignalHandler);

	INFO("Install the conf re-read signal");
	InstallConfRereadSignal(ConfRereadSignalHandler);

	return true;
}

void Run() {
	INFO("\n%s", STR(gRs.pConfigFile->GetServicesInfo()));
	INFO("GO! GO! GO! (%d)", getpid());
	while (IOHandlerManager::Pulse()) {
		IOHandlerManager::DeleteDeadHandlers();
		ProtocolManager::CleanupDeadProtocols();
	}
}

void Cleanup() {
	WARN("Shutting down protocols manager");
	ProtocolManager::Shutdown();
	ProtocolManager::CleanupDeadProtocols();

	WARN("Shutting down I/O handlers manager");
	IOHandlerManager::ShutdownIOHandlers();
	IOHandlerManager::DeleteDeadHandlers();
	IOHandlerManager::Shutdown();

	WARN("Unregister and delete default protocol handler");
	ProtocolFactoryManager::UnRegisterProtocolFactory(gRs.pProtocolFactory);
	delete gRs.pProtocolFactory;
	gRs.pProtocolFactory = NULL;

	WARN("Shutting down applications");
	ClientApplicationManager::Shutdown();

	WARN("Delete the configuration");
	delete gRs.pConfigFile;
	gRs.pConfigFile = NULL;

	WARN("Shutting down the logger leaving you in the dark. Bye bye... :(");
	Logger::Free(true);
}

void QuitSignalHandler(void) {
	IOHandlerManager::SignalShutdown();
}

void ConfRereadSignalHandler(void) {
	gRs.run = true;
	IOHandlerManager::SignalShutdown();
}

#ifdef COMPILE_STATIC
extern "C" BaseClientApplication *GetApplication_admin(Variant configuration);
extern "C" BaseClientApplication *GetApplication_applestreamingclient(Variant configuration);
extern "C" BaseClientApplication *GetApplication_appselector(Variant configuration);
extern "C" BaseClientApplication *GetApplication_flvplayback(Variant configuration);
//extern "C" BaseClientApplication *GetApplication_houseband(Variant configuration);
extern "C" BaseClientApplication *GetApplication_proxypublish(Variant configuration);
extern "C" BaseClientApplication *GetApplication_samplefactory(Variant configuration);
extern "C" BaseClientApplication *GetApplication_stresstest(Variant configuration);
extern "C" BaseClientApplication *GetApplication_vptests(Variant configuration);

BaseClientApplication *SpawnApplication(Variant configuration) {
	if (configuration[CONF_APPLICATION_NAME] == "admin") {
		return GetApplication_admin(configuration);
	} else if (configuration[CONF_APPLICATION_NAME] == "applestreamingclient") {
		return GetApplication_applestreamingclient(configuration);
	} else if (configuration[CONF_APPLICATION_NAME] == "appselector") {
		return GetApplication_appselector(configuration);
	} else if (configuration[CONF_APPLICATION_NAME] == "flvplayback") {
		return GetApplication_flvplayback(configuration);
	}
		//		else if (configuration[CONF_APPLICATION_NAME] == "houseband") {
		//		return GetApplication_houseband(configuration);
		//	}
	else if (configuration[CONF_APPLICATION_NAME] == "proxypublish") {
		return GetApplication_proxypublish(configuration);
	} else if (configuration[CONF_APPLICATION_NAME] == "samplefactory") {
		return GetApplication_samplefactory(configuration);
	} else if (configuration[CONF_APPLICATION_NAME] == "stresstest") {
		return GetApplication_stresstest(configuration);
	} else if (configuration[CONF_APPLICATION_NAME] == "vptests") {
		return GetApplication_vptests(configuration);
	} else {
		return NULL;
	}
}
#endif

