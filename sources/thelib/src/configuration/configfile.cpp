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

#include "configuration/configfile.h"
#include "application/clientapplicationmanager.h"

ConfigFile::ConfigFile(GetApplicationFunction_t staticGetApplicationFunction,
		GetFactoryFunction_t staticGetFactoryFunction) {
	_staticGetApplicationFunction = staticGetApplicationFunction;
	_staticGetFactoryFunction = staticGetFactoryFunction;
	if (((_staticGetApplicationFunction == NULL) && (_staticGetFactoryFunction != NULL))
			|| ((_staticGetApplicationFunction != NULL) && (_staticGetFactoryFunction == NULL))) {
		ASSERT("Invalid config file usage");
	}
	_isOrigin = true;
}

ConfigFile::~ConfigFile() {

	FOR_MAP(_modules, string, Module, i) {
		MAP_VAL(i).Release();
	}
	_modules.clear();
}

bool ConfigFile::IsDaemon() {
	if (_configuration.HasKeyChain(V_BOOL, true, 1, CONF_DAEMON))
		return (bool)_configuration[CONF_DAEMON];
	return false;
}

bool ConfigFile::IsOrigin() {
	return _isOrigin;
}

string ConfigFile::GetServicesInfo() {
	map<uint32_t, BaseClientApplication *> applications = ClientApplicationManager::GetAllApplications();

	stringstream ss;

	ss << "+-----------------------------------------------------------------------------+" << endl;
	ss << "|";
	ss.width(77);
	ss << "Services";
	ss << "|" << endl;
	ss << "+---+---------------+-----+-------------------------+-------------------------+" << endl;
	ss << "| c |      ip       | port|   protocol stack name   |     application name    |" << endl;

	FOR_MAP(applications, uint32_t, BaseClientApplication *, i) {
		ss << MAP_VAL(i)->GetServicesInfo();
	}

	ss << "+---+---------------+-----+-------------------------+-------------------------+";

	return ss.str();
}

bool ConfigFile::LoadLuaFile(string path, bool forceDaemon) {
	if (!ReadLuaFile(path, CONF_CONFIGURATION, _configuration)) {
		FATAL("Unable to read configuration file: %s", STR(path));
		return false;
	}
	if (forceDaemon)
		_configuration[CONF_DAEMON] = (bool)true;
	return Normalize();
}

bool ConfigFile::LoadXmlFile(string path, bool forceDaemon) {
	if (!Variant::DeserializeFromXmlFile(path, _configuration)) {
		FATAL("Unable to read configuration file: %s", STR(path));
		return false;
	}
	if (forceDaemon)
		_configuration[CONF_DAEMON] = (bool)true;
	return Normalize();
}

bool ConfigFile::ConfigLogAppenders() {

	FOR_MAP(_logAppenders, string, Variant, i) {
		if (!ConfigLogAppender(MAP_VAL(i))) {
			FATAL("Unable to configure log appender:\n%s", STR(MAP_VAL(i).ToString()));
			return false;
		}
	}

	return true;
}

bool ConfigFile::ConfigModules() {

	FOR_MAP(_applications, string, Variant, i) {
		if (!ConfigModule(MAP_VAL(i))) {
			FATAL("Unable to configure module:\n%s", STR(MAP_VAL(i).ToString()));
			return false;
		}
	}
	return true;
}

bool ConfigFile::ConfigFactories() {

	FOR_MAP(_modules, string, Module, i) {
		if (!MAP_VAL(i).ConfigFactory()) {
			FATAL("Unable to configure factory");
			return false;
		}
	}
	return true;
}

bool ConfigFile::ConfigAcceptors() {

	FOR_MAP(_modules, string, Module, i) {
		if (!MAP_VAL(i).BindAcceptors()) {
			FATAL("Unable to configure acceptors");
			return false;
		}
	}
	return true;
}

bool ConfigFile::ConfigInstances() {
#ifndef WIN32
	int8_t instancesCount = 0;
	if (_configuration.HasKeyChain(_V_NUMERIC, false, 1, "instancesCount")) {
		instancesCount = (int8_t) _configuration.GetValue("instancesCount", false);
	}
	if (instancesCount > 8) {
		FATAL("Invalid number of instances count. Max value is 8");
		return false;
	}

	if (instancesCount == 0)
		return true;

	if (instancesCount < 0) {
		instancesCount = getCPUCount();
	}

	if ((instancesCount < 0) || (instancesCount > 16)) {
		FATAL("unable to correctly compute the number of instances");
		return false;
	}

	if (!IsDaemon()) {
		WARN("Daemon mode not activated. No additional instances will be spawned");
		return true;
	}

	for (int32_t i = 0; i < instancesCount; i++) {
		pid_t pid = fork();
		if (pid < 0) {
			FATAL("Unable to start child instance. fork() failed");
			return false;
		}

		if (pid == 0) {
			_isOrigin = false;
			Logger::SignalFork();
			break;
		}
	}

	FOR_MAP(_modules, string, Module, i) {
		MAP_VAL(i).config["isOrigin"] = (bool)_isOrigin;
	}

	//dirty fix: need to wait the origin to boot up, so all edges
	//are going to wait 5 seconds
	if (!_isOrigin) {
		sleep(5);
	}

	return true;
#else /* WIN32 */
	WARN("Windows doesn't support multiple instances");
	return true;
#endif /* WIN32 */
}

bool ConfigFile::ConfigApplications() {

	FOR_MAP(_modules, string, Module, i) {
		if (!MAP_VAL(i).ConfigApplication()) {
			FATAL("Unable to configure acceptors");
			return false;
		}
	}
	return true;
}

bool ConfigFile::ConfigLogAppender(Variant &node) {
	BaseLogLocation *pLogLocation = NULL;
	if ((string) node[CONF_LOG_APPENDER_TYPE] == CONF_LOG_APPENDER_TYPE_COLORED_CONSOLE) {
		node[CONF_LOG_APPENDER_COLORED] = (bool)true;
		if (!IsDaemon()) {
			pLogLocation = new ConsoleLogLocation(node);
		}
	} else if ((string) node[CONF_LOG_APPENDER_TYPE] == CONF_LOG_APPENDER_TYPE_CONSOLE) {
		if (!IsDaemon()) {
			pLogLocation = new ConsoleLogLocation(node);
		}
	} else if ((string) node[CONF_LOG_APPENDER_TYPE] == CONF_LOG_APPENDER_TYPE_FILE) {
		pLogLocation = new FileLogLocation(node);
	} else {
		NYIR;
	}
	if (pLogLocation != NULL) {
		pLogLocation->SetLevel((int32_t) node[CONF_LOG_APPENDER_LEVEL]);
		if (!Logger::AddLogLocation(pLogLocation)) {
			FATAL("Unable to add log location to logger:\n%s", STR(node.ToString()));
			delete pLogLocation;
			return false;
		}
	}
	return true;
}

bool ConfigFile::Normalize() {
	if (!NormalizeLogAppenders()) {
		FATAL("Unable to normalize log appenders");
		return false;
	}
	if (!NormalizeApplications()) {
		FATAL("Unable to normalize applications");
		return false;
	}
	return true;
}

bool ConfigFile::ConfigModule(Variant &node) {
	Module module;
	module.config = node;
	if (_staticGetApplicationFunction != NULL) {
		module.getApplication = _staticGetApplicationFunction;
		module.getFactory = _staticGetFactoryFunction;
	}

	if (!module.Load()) {
		FATAL("Unable to load module");
		return false;
	}

	_modules[node[CONF_APPLICATION_NAME]] = module;

	return true;
}

bool ConfigFile::NormalizeLogAppenders() {
	if (!_configuration.HasKeyChain(V_MAP, false, 1, CONF_LOG_APPENDERS)) {
		WARN("No log appenders specified");
		return true;
	}
	Variant temp = _configuration.GetValue(CONF_LOG_APPENDERS, false);

	FOR_MAP(temp, string, Variant, i) {
		if (MAP_VAL(i) != V_MAP) {
			WARN("Invalid log appender:\n%s", STR(MAP_VAL(i).ToString()));
			continue;
		}
		if (!NormalizeLogAppender(MAP_VAL(i))) {
			WARN("Invalid log appender:\n%s", STR(MAP_VAL(i).ToString()));
			continue;
		}
		_logAppenders.PushToArray(MAP_VAL(i));
	}
	return true;
}

bool ConfigFile::NormalizeLogAppender(Variant &node) {
	if (!node.HasKeyChain(V_STRING, false, 1, CONF_LOG_APPENDER_NAME)) {
		WARN("Invalid log appender name");
		return false;
	}
	string name = node.GetValue(CONF_LOG_APPENDER_NAME, false);

	if (!node.HasKeyChain(V_STRING, false, 1, CONF_LOG_APPENDER_TYPE)) {
		WARN("Invalid log appender type");
		return false;
	}
	string type = node.GetValue(CONF_LOG_APPENDER_TYPE, false);
	if ((type != CONF_LOG_APPENDER_TYPE_COLORED_CONSOLE)
			&& (type != CONF_LOG_APPENDER_TYPE_CONSOLE)
			&& (type != CONF_LOG_APPENDER_TYPE_FILE)) {
		WARN("Invalid log appender type");
		return false;
	}

	if (!node.HasKeyChain(_V_NUMERIC, false, 1, CONF_LOG_APPENDER_LEVEL)) {
		WARN("Invalid log appender level");
		return false;
	}
	int8_t level = (int8_t) node.GetValue(CONF_LOG_APPENDER_LEVEL, false);
	if (level < 0) {
		WARN("Invalid log appender level");
		return false;
	}

	node[CONF_LOG_APPENDER_NAME] = name;
	node[CONF_LOG_APPENDER_TYPE] = type;
	node[CONF_LOG_APPENDER_LEVEL] = (uint8_t) level;

	return true;
}

bool ConfigFile::NormalizeApplications() {
	if (!_configuration.HasKeyChain(V_MAP, false, 1, CONF_APPLICATIONS)) {
		WARN("No applications specified");
		return true;
	}
	Variant temp = _configuration.GetValue(CONF_APPLICATIONS, false);

	_rootAppFolder = "";
	if (temp.HasKeyChain(V_STRING, false, 1, CONF_APPLICATIONS_ROOTDIRECTORY))
		_rootAppFolder = (string) temp.GetValue(CONF_APPLICATIONS_ROOTDIRECTORY, false);
	trim(_rootAppFolder);
	if (_rootAppFolder == "")
		_rootAppFolder = ".";
	if (_rootAppFolder[_rootAppFolder.size() - 1] != PATH_SEPARATOR)
		_rootAppFolder += PATH_SEPARATOR;

	_applications.IsArray(true);

	FOR_MAP(temp, string, Variant, i) {
		if (MAP_KEY(i) == CONF_APPLICATIONS_ROOTDIRECTORY)
			continue;
		if (MAP_VAL(i) != V_MAP) {
			FATAL("Invalid application:\n%s", STR(MAP_VAL(i).ToString()));
			return false;
		}
		if (!NormalizeApplication(MAP_VAL(i))) {
			FATAL("Invalid application:\n%s", STR(MAP_VAL(i).ToString()));
			return false;
		}
		_applications.PushToArray(MAP_VAL(i));
	}
	return true;
}

bool ConfigFile::NormalizeApplication(Variant &node) {
	string temp = "";

	if (!node.HasKeyChain(V_STRING, false, 1, CONF_APPLICATION_NAME)) {
		FATAL("Invalid application name");
		return false;
	}
	string name = node.GetValue(CONF_APPLICATION_NAME, false);
	if (name == "") {
		FATAL("Invalid application name");
		return false;
	}
	if (MAP_HAS1(_uniqueNames, name)) {
		FATAL("Application name %s already taken", STR(name));
		return false;
	}
	_uniqueNames[name] = name;
	node[CONF_APPLICATION_NAME] = name;


	string appDir = "";
	if (node.HasKeyChain(V_STRING, false, 1, CONF_APPLICATION_DIRECTORY))
		appDir = (string) node.GetValue(CONF_APPLICATION_DIRECTORY, false);
	if (appDir == "") {
		appDir = _rootAppFolder + name;
	}
	temp = normalizePath(appDir, "");
	if (temp == "") {
		WARN("Path not found: %s", STR(appDir));
	} else {
		appDir = temp;
	}
	if (appDir[appDir.size() - 1] != PATH_SEPARATOR)
		appDir += PATH_SEPARATOR;
	node[CONF_APPLICATION_DIRECTORY] = appDir;

	string mediaFolder = "";
	if (node.HasKeyChain(V_STRING, false, 1, CONF_APPLICATION_MEDIAFOLDER))
		mediaFolder = (string) node.GetValue(CONF_APPLICATION_MEDIAFOLDER, false);
	if (mediaFolder == "") {
		mediaFolder = appDir + "media";
	}
	temp = normalizePath(mediaFolder, "");
	if (temp == "") {
		WARN("Path not found: %s", STR(mediaFolder));
	} else {
		mediaFolder = temp;
	}
	if (mediaFolder[mediaFolder.size() - 1] != PATH_SEPARATOR)
		mediaFolder += PATH_SEPARATOR;
	node[CONF_APPLICATION_MEDIAFOLDER] = mediaFolder;


	string libraryPath = "";
	if (node.HasKeyChain(V_STRING, false, 1, CONF_APPLICATION_LIBRARY))
		libraryPath = (string) node.GetValue(CONF_APPLICATION_LIBRARY, false);
	if (libraryPath == "") {
		libraryPath = appDir + format(LIBRARY_NAME_PATTERN, STR(name));
	}
	temp = normalizePath(libraryPath, "");
	if (temp == "") {
		if ((_staticGetApplicationFunction == NULL)
				|| (_staticGetFactoryFunction == NULL)) {
			FATAL("Library %s not found", STR(libraryPath));
			return false;
		} else {
			libraryPath = temp;
		}
	} else {
		libraryPath = temp;
	}
	node[CONF_APPLICATION_LIBRARY] = libraryPath;

	string initApplicationFunction = "";
	if (node.HasKeyChain(V_STRING, false, 1, CONF_APPLICATION_INIT_APPLICATION_FUNCTION))
		initApplicationFunction = (string) node.GetValue(CONF_APPLICATION_INIT_APPLICATION_FUNCTION, false);
	if (initApplicationFunction == "")
		initApplicationFunction = "GetApplication_" + name;
	node[CONF_APPLICATION_INIT_APPLICATION_FUNCTION] = initApplicationFunction;

	string initFactoryFunction = "";
	if (node.HasKeyChain(V_STRING, false, 1, CONF_APPLICATION_INIT_FACTORY_FUNCTION))
		initFactoryFunction = (string) node.GetValue(CONF_APPLICATION_INIT_FACTORY_FUNCTION, false);
	if (initFactoryFunction == "")
		initFactoryFunction = "GetFactory_" + name;
	node[CONF_APPLICATION_INIT_FACTORY_FUNCTION] = initFactoryFunction;

	bool validateHandshake = true;
	if (node.HasKeyChain(V_BOOL, false, 1, CONF_APPLICATION_VALIDATEHANDSHAKE))
		validateHandshake = (bool) node.GetValue(CONF_APPLICATION_VALIDATEHANDSHAKE, false);
	node[CONF_APPLICATION_VALIDATEHANDSHAKE] = (bool)validateHandshake;

	bool defaultApp = false;
	if (node.HasKeyChain(V_BOOL, false, 1, CONF_APPLICATION_DEFAULT))
		defaultApp = (bool) node.GetValue(CONF_APPLICATION_DEFAULT, false);
	node[CONF_APPLICATION_DEFAULT] = (bool)defaultApp;

	bool generateMetaFiles = false;
	if (node.HasKeyChain(V_BOOL, false, 1, CONF_APPLICATION_GENERATE_META_FILES))
		generateMetaFiles = (bool) node.GetValue(CONF_APPLICATION_GENERATE_META_FILES, false);
	node[CONF_APPLICATION_GENERATE_META_FILES] = (bool)generateMetaFiles;

	bool keyFrameSeek = false;
	if (node.HasKeyChain(V_BOOL, false, 1, CONF_APPLICATION_KEYFRAMESEEK))
		keyFrameSeek = (bool) node.GetValue(CONF_APPLICATION_KEYFRAMESEEK, false);
	node[CONF_APPLICATION_KEYFRAMESEEK] = (bool)keyFrameSeek;

	bool renameBadFiles = false;
	if (node.HasKeyChain(V_BOOL, false, 1, CONF_APPLICATION_RENAMEBADFILES))
		renameBadFiles = (bool) node.GetValue(CONF_APPLICATION_RENAMEBADFILES, false);
	node[CONF_APPLICATION_RENAMEBADFILES] = (bool)renameBadFiles;

	bool externSeekGenerator = false;
	if (node.HasKeyChain(V_BOOL, false, 1, CONF_APPLICATION_EXTERNSEEKGENERATOR))
		externSeekGenerator = (bool) node.GetValue(CONF_APPLICATION_EXTERNSEEKGENERATOR, false);
	node[CONF_APPLICATION_EXTERNSEEKGENERATOR] = (bool)externSeekGenerator;

	int32_t seekGranularity = 1;
	if (node.HasKeyChain(_V_NUMERIC, false, 1, CONF_APPLICATION_SEEKGRANULARITY))
		seekGranularity = (int32_t) node.GetValue(CONF_APPLICATION_SEEKGRANULARITY, false);
	if (seekGranularity < 0 || seekGranularity > 300)
		seekGranularity = 1;
	node[CONF_APPLICATION_SEEKGRANULARITY] = (uint32_t) seekGranularity;

	int64_t clientSideBuffer = 5;
	if (node.HasKeyChain(_V_NUMERIC, false, 1, CONF_APPLICATION_CLIENTSIDEBUFFER))
		clientSideBuffer = (int64_t) node.GetValue(CONF_APPLICATION_CLIENTSIDEBUFFER, false);
	if (seekGranularity < 0 || seekGranularity > 300)
		seekGranularity = 5;
	node[CONF_APPLICATION_CLIENTSIDEBUFFER] = (uint32_t) clientSideBuffer;

	uint8_t rtcpDetectionInterval = 10;
	if (node.HasKeyChain(_V_NUMERIC, false, 1, CONF_APPLICATION_RTCPDETECTIONINTERVAL))
		rtcpDetectionInterval = (uint8_t) node.GetValue(CONF_APPLICATION_RTCPDETECTIONINTERVAL, false);
	if (rtcpDetectionInterval >= 60)
		rtcpDetectionInterval = 60;
	node[CONF_APPLICATION_RTCPDETECTIONINTERVAL] = (uint8_t) rtcpDetectionInterval;

	Variant acceptors;
	acceptors.IsArray(true);
	if (node.HasKeyChain(V_MAP, false, 1, CONF_ACCEPTORS)) {

		FOR_MAP(node[CONF_ACCEPTORS], string, Variant, i) {
			if (MAP_VAL(i) != V_MAP) {
				FATAL("Invalid acceptor:\n%s", STR(MAP_VAL(i).ToString()));
				return false;
			}

			if (!NormalizeApplicationAcceptor(MAP_VAL(i), appDir)) {
				FATAL("Invalid acceptor:\n%s", STR(MAP_VAL(i).ToString()));
				return false;
			}
			acceptors.PushToArray(MAP_VAL(i));
		}
	}
	node[CONF_ACCEPTORS] = acceptors;

	Variant aliases;
	aliases.IsArray(true);
	if (node.HasKeyChain(V_MAP, false, 1, CONF_APPLICATION_ALIASES)) {

		FOR_MAP(node[CONF_APPLICATION_ALIASES], string, Variant, i) {
			if (MAP_VAL(i) != V_STRING) {
				FATAL("Invalid alias value:\n%s", STR(MAP_VAL(i).ToString()));
				return false;
			}
			if ((string) MAP_VAL(i) == "") {
				FATAL("Invalid alias value:\n%s", STR(MAP_VAL(i).ToString()));
				return false;
			}
			if (MAP_HAS1(_uniqueNames, MAP_VAL(i))) {
				FATAL("Alias name %s already taken", STR(MAP_VAL(i)));
				return false;
			}
			_uniqueNames[(string) MAP_VAL(i)] = (string) MAP_VAL(i);
			aliases.PushToArray(MAP_VAL(i));
		}
	}
	node[CONF_APPLICATION_ALIASES] = aliases;

	return true;
}

bool ConfigFile::NormalizeApplicationAcceptor(Variant &node, string baseFolder) {
	string ip = "";
	if (node.HasKeyChain(V_STRING, false, 1, CONF_IP))
		ip = (string) node.GetValue(CONF_IP, false);
	if (ip == "") {
		FATAL("Invalid ip: %s", STR(ip));
		return false;
	}
	if (getHostByName(ip) == "") {
		FATAL("Invalid ip: %s", STR(ip));
		return false;
	}
	node[CONF_IP] = ip;

	int32_t port = 0;
	if (node.HasKeyChain(_V_NUMERIC, false, 1, CONF_PORT))
		port = (int32_t) node.GetValue(CONF_PORT, false);
	if (port <= 0 || port >= 65536) {
		FATAL("Invalid port: %"PRId32, port);
		return false;
	}
	node[CONF_PORT] = (uint16_t) port;

	string protocol = "";
	if (node.HasKeyChain(V_STRING, false, 1, CONF_PROTOCOL))
		protocol = (string) node.GetValue(CONF_PROTOCOL, false);
	if (protocol == "") {
		FATAL("Invalid protocol: %s", STR(protocol));
		return false;
	}
	node[CONF_PROTOCOL] = protocol;

	string sslKey = "";
	if (node.HasKeyChain(V_STRING, false, 1, CONF_SSL_KEY))
		sslKey = (string) node.GetValue(CONF_SSL_KEY, false);
	if (sslKey != "") {
		if ((sslKey[0] != PATH_SEPARATOR) && (sslKey[0] != '.')) {
			sslKey = baseFolder + sslKey;
		}
		string temp = normalizePath(sslKey, "");
		if (temp == "") {
			FATAL("SSL key not found: %s", STR(sslKey));
			return false;
		}
		sslKey = temp;
	}
	node[CONF_SSL_KEY] = sslKey;

	string sslCert = "";
	if (node.HasKeyChain(V_STRING, false, 1, CONF_SSL_CERT))
		sslCert = (string) node.GetValue(CONF_SSL_CERT, false);
	if (sslCert != "") {
		if ((sslCert[0] != PATH_SEPARATOR) && (sslCert[0] != '.')) {
			sslCert = baseFolder + sslCert;
		}
		string temp = normalizePath(sslCert, "");
		if (temp == "") {
			FATAL("SSL key not found: %s", STR(sslCert));
			return false;
		}
		sslCert = temp;
	}
	node[CONF_SSL_CERT] = sslCert;

	if (((sslKey == "") && (sslCert != "")) || ((sslKey != "") && (sslCert == ""))) {
		FATAL("Invalid ssl key/cert");
		return false;
	}

	return true;
}

bool ConfigFile::NormalizeApplicationAliases(Variant &aliases) {
	NYIR;
}
