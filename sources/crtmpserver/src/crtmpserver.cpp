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
	Variant commandLine;

	//Configuration file
	ConfigFile *pConfigFile;

	//default protocol factory
	DefaultProtocolFactory * pProtocolFactory;

	//should we run again?
	bool run;

	//is this a daemon already?
	bool daemon;

	RunningStatus() {
		pConfigFile = NULL;
		pProtocolFactory = NULL;
		run = false;
		daemon = false;
	}
};

void QuitSignalHandler(void);
void ConfRereadSignalHandler(void);
bool Initialize();
void Run();
void Cleanup();
void PrintHelp();
void PrintVersion();
void NormalizeCommandLine(string configFile);
bool ApplyUIDGID();

RunningStatus gRs;

#ifdef COMPILE_STATIC
BaseClientApplication *SpawnApplication(Variant configuration);
#endif

int main(int argc, char **argv) {
	SRAND();
	InitNetworking();

	//1. Pick up the startup parameters and hold them inside the running status
	if (argc < 2) {
		cout << "Invalid command line. Use --help" << endl;
		return -1;
	}

	if (!Variant::DeserializeFromCmdLineArgs(argc, argv, gRs.commandLine)) {
		PrintHelp();
		return -1;
	}
	string configFile = argv[argc - 1];
	if (configFile.find("--") == 0)
		configFile = "";
	NormalizeCommandLine(configFile);

	if ((bool)gRs.commandLine["arguments"]["--help"]) {
		PrintHelp();
		return 0;
	}

	if ((bool)gRs.commandLine["arguments"]["--version"]) {
		PrintVersion();
		return 0;
	}

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

	if ((bool)gRs.commandLine["arguments"]["--use-implicit-console-appender"]) {
		ConsoleLogLocation * pLogLocation = new ConsoleLogLocation(false);
		pLogLocation->SetLevel(_FINEST_);
		Logger::AddLogLocation(pLogLocation);
	}

	INFO("Reading configuration from %s", STR(gRs.commandLine["arguments"]["configFile"]));
#ifdef COMPILE_STATIC
	gRs.pConfigFile = new ConfigFile(SpawnApplication);
#else
	gRs.pConfigFile = new ConfigFile(NULL);
#endif
	string configFilePath = gRs.commandLine["arguments"]["configFile"];
	string fileName;
	string extension;
	splitFileName(configFilePath, fileName, extension);

	if (lowercase(extension) == "xml") {
		if (!gRs.pConfigFile->LoadXmlFile(configFilePath,
				(bool)gRs.commandLine["arguments"]["--daemon"])) {
			FATAL("Unable to load file %s", STR(configFilePath));
			return false;
		}
	} else if (lowercase(extension) == "lua") {
#ifdef HAS_LUA
		if (!gRs.pConfigFile->LoadLuaFile(configFilePath,
				(bool)gRs.commandLine["arguments"]["--daemon"])) {
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

#ifdef  HAS_PROTOCOL_DNS 
	INFO("Initialize DNS resolver");
	if (!gRs.pConfigFile->ConfigureDNSResolver()) {
		FATAL("Unable to configure DNS resolver");
		return false;
	}
#endif /* HAS_PROTOCOL_DNS */

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
	if (!ApplyUIDGID()) {
		FATAL("Unable to apply user id");
		exit(-1);
	}
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

	WARN("Doing final OpenSSL cleanup");
	CleanupSSL();

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

void PrintHelp() {
	cout << "Usage: \n" << (string) gRs.commandLine["program"] << " [OPTIONS] [config_file_path]" << endl << endl;
	cout << "OPTIONS:" << endl;
	cout << "    --help" << endl;
	cout << "      Prints this help and exit\n" << endl;
	cout << "    --version" << endl;
	cout << "      Prints the version and exit.\n" << endl;
	cout << "    --use-implicit-console-appender" << endl;
	cout << "      Adds a console log appender." << endl;
	cout << "      Particulary useful when the server starts and stops immediatly." << endl;
	cout << "      Allows you to see if something is wrong with the config file\n" << endl;
	cout << "    --daemon" << endl;
	cout << "      Overrides the daemon setting inside the config file and forces" << endl;
	cout << "      the server to start in daemon mode.\n" << endl;
	cout << "    --uid=<uid>" << endl;
	cout << "      Run the process with the specified user id\n" << endl;
	cout << "    --gid=<gid>" << endl;
	cout << "      Run the process with the specified group id\n" << endl;
}

void PrintVersion() {
#ifndef RTMPSERVER_VERSION
#define RTMPSERVER_VERSION "(version not available)"
#endif
	cout << HTTP_HEADERS_SERVER_US << " version " << RTMPSERVER_VERSION << endl;
}

void NormalizeCommandLine(string configFile) {
	gRs.commandLine["arguments"]["configFile"] = configFile;
	gRs.commandLine["arguments"].RemoveKey(configFile);
	gRs.commandLine["arguments"]["--help"] = (bool)gRs.commandLine["arguments"].HasKey("--help");
	gRs.commandLine["arguments"]["--version"] = (bool)gRs.commandLine["arguments"].HasKey("--version");
	gRs.commandLine["arguments"]["--use-implicit-console-appender"] = (bool)gRs.commandLine["arguments"].HasKey("--use-implicit-console-appender");
	gRs.commandLine["arguments"]["--daemon"] = (bool)gRs.commandLine["arguments"].HasKey("--daemon");
	if (gRs.commandLine["arguments"].HasKey("--uid")) {
		gRs.commandLine["arguments"]["--uid"] = (uint32_t) atoi(STR(gRs.commandLine["arguments"]["--uid"]));
	} else {
		gRs.commandLine["arguments"]["--uid"] = (uint32_t) 0;
	}
	if (gRs.commandLine["arguments"].HasKey("--gid")) {
		gRs.commandLine["arguments"]["--gid"] = (uint32_t) atoi(STR(gRs.commandLine["arguments"]["--gid"]));
	} else {
		gRs.commandLine["arguments"]["--gid"] = (uint32_t) 0;
	}
}

bool ApplyUIDGID() {
#ifndef WIN32
	if ((uint32_t) gRs.commandLine["arguments"]["--gid"] != 0) {
		if (setgid((uid_t) gRs.commandLine["arguments"]["--gid"]) != 0) {
			FATAL("Unable to set GID");
			return false;
		}
	}
	if ((uint32_t) gRs.commandLine["arguments"]["--uid"] != 0) {
		if (setuid((uid_t) gRs.commandLine["arguments"]["--uid"]) != 0) {
			FATAL("Unable to set UID");
			return false;
		}
	}
#endif
	return true;
}

#ifdef COMPILE_STATIC
#ifdef HAS_APP_ADMIN
extern "C" BaseClientApplication *GetApplication_admin(Variant configuration);
#endif
#ifdef HAS_APP_APPLESTREAMINGCLIENT
extern "C" BaseClientApplication *GetApplication_applestreamingclient(Variant configuration);
#endif
#ifdef HAS_APP_APPSELECTOR
extern "C" BaseClientApplication *GetApplication_appselector(Variant configuration);
#endif
#ifdef HAS_APP_FLVPLAYBACK
extern "C" BaseClientApplication *GetApplication_flvplayback(Variant configuration);
#endif
#ifdef HAS_APP_HOUSEBAND
extern "C" BaseClientApplication *GetApplication_houseband(Variant configuration);
#endif
#ifdef HAS_APP_PROXYPUBLISH
extern "C" BaseClientApplication *GetApplication_proxypublish(Variant configuration);
#endif
#ifdef HAS_APP_SAMPLEFACTORY
extern "C" BaseClientApplication *GetApplication_samplefactory(Variant configuration);
#endif
#ifdef HAS_APP_STRESSTEST
extern "C" BaseClientApplication *GetApplication_stresstest(Variant configuration);
#endif
#ifdef HAS_APP_VPTESTS
extern "C" BaseClientApplication *GetApplication_vptests(Variant configuration);
#endif

BaseClientApplication *SpawnApplication(Variant configuration) {
	if (false) {

	}
#ifdef HAS_APP_ADMIN
	else if (configuration[CONF_APPLICATION_NAME] == "admin") {
		return GetApplication_admin(configuration);
	}
#endif
#ifdef HAS_APP_APPLESTREAMINGCLIENT
	else if (configuration[CONF_APPLICATION_NAME] == "applestreamingclient") {
		return GetApplication_applestreamingclient(configuration);
	}
#endif
#ifdef HAS_APP_APPSELECTOR
	else if (configuration[CONF_APPLICATION_NAME] == "appselector") {
		return GetApplication_appselector(configuration);
	}
#endif
#ifdef HAS_APP_FLVPLAYBACK
	else if (configuration[CONF_APPLICATION_NAME] == "flvplayback") {
		return GetApplication_flvplayback(configuration);
	}
#endif
#ifdef HAS_APP_HOUSEBAND
	else if (configuration[CONF_APPLICATION_NAME] == "houseband") {
		return GetApplication_houseband(configuration);
	}
#endif
#ifdef HAS_APP_PROXYPUBLISH
	else if (configuration[CONF_APPLICATION_NAME] == "proxypublish") {
		return GetApplication_proxypublish(configuration);
	}
#endif
#ifdef HAS_APP_SAMPLEFACTORY
	else if (configuration[CONF_APPLICATION_NAME] == "samplefactory") {
		return GetApplication_samplefactory(configuration);
	}
#endif
#ifdef HAS_APP_STRESSTEST
	else if (configuration[CONF_APPLICATION_NAME] == "stresstest") {
		return GetApplication_stresstest(configuration);
	}
#endif
#ifdef HAS_APP_VPTESTS
	else if (configuration[CONF_APPLICATION_NAME] == "vptests") {
		return GetApplication_vptests(configuration);
	}
#endif
	else {
		return NULL;
	}
}
#endif
