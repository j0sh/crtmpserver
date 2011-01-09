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
#include "configuration/configfile.h"
#include "application/baseclientapplication.h"
#include "application/clientapplicationmanager.h"
#include "protocols/dns/outbounddnsresolverprotocol.h"
#include "netio/netio.h"
#include "protocols/protocolfactorymanager.h"

ConfigFile::ConfigFile(GetStaticApplication_t pFunction) {
	_pFunction = pFunction;
}

ConfigFile::~ConfigFile() {

	FOR_VECTOR(_libraryHandlers, i) {
		FREE_LIBRARY(_libraryHandlers[i]);
	}
	_libraryHandlers.clear();
}

#ifdef HAS_LUA

bool ConfigFile::LoadLuaFile(string path) {
	InitServiceInfo();
	if (!ReadLuaFile(path, CONF_CONFIGURATION, _configuration)) {
		_listeningSockets.clear();
		FATAL("Unable to read configuration file: %s", STR(path));
		return false;
	}
	return true;
}

bool ConfigFile::LoadLuaString(string script) {
	InitServiceInfo();
	if (!ReadLuaString(script, CONF_CONFIGURATION, _configuration)) {
		FATAL("Unable to read configuration script: %s", STR(script));
		return false;
	}
	return true;
}
#endif /* HAS_LUA */

bool ConfigFile::LoadXmlFile(string path) {
	InitServiceInfo();
	if (!Variant::DeserializeFromXmlFile(path, _configuration)) {
		_listeningSockets.clear();
		FATAL("Unable to read configuration file: %s", STR(path));
		return false;
	}
	return true;
}

bool ConfigFile::LoadXmlString(string xmlContent) {
	InitServiceInfo();
	if (!Variant::DeserializeFromXml(xmlContent, _configuration)) {
		_listeningSockets.clear();
		FATAL("Unable to read configuration:\n%s", STR(xmlContent));
		return false;
	}
	return true;
}

bool ConfigFile::ConfigureLogAppenders() {
	vector<Variant> logAppenders;
	if (!ValidateLogAppenders(logAppenders))
		return false;

	FOR_VECTOR_ITERATOR(Variant, logAppenders, i) {
		if (!ConfigureLogAppender(VECTOR_VAL(i))) {
			FATAL("Unable to configure log appender %s",
					STR(VECTOR_VAL(i).ToString()));
			return false;
		}
	}

	return true;
}

bool ConfigFile::ConfigureDNSResolver() {
	if (!ValidateDNSResolver()) {
		FATAL("Unable to validate DNS resolver");
		return false;
	}

	if (_configuration[CONF_DNSRESOLVER].HasKey(CONF_ACCEPTORS)) {
		if (!ConfigureAcceptors(_configuration[CONF_DNSRESOLVER][CONF_ACCEPTORS], NULL)) {
			FATAL("Unable to configure acceptors on DNS resolver");
			return false;
		}
	}

	if (_configuration[CONF_DNSRESOLVER][CONF_IP] != CONF_DNSRESOLVER_SELF) {
		if (!OutboundDNSResolverProtocol::Connect(
				STR(_configuration[CONF_DNSRESOLVER][CONF_IP]),
				_configuration[CONF_DNSRESOLVER][CONF_PORT],
				_configuration[CONF_DNSRESOLVER])) {
			FATAL("Unable to initialize remote DNS resolver");
			return false;
		}
	}

	return true;
}

bool ConfigFile::ConfigureApplications() {
	if (!ValidateApplications()) {
		FATAL("Unable to validate applications");
		return false;
	}

	FOR_MAP(_configuration[CONF_APPLICATIONS], string, Variant, i) {
		if (!ConfigureApplication(MAP_VAL(i))) {
			FATAL("Unable to configure application %s", STR(MAP_VAL(i).ToString()));
			return false;
		}
	}

	return true;
}

bool ConfigFile::IsDaemon() {
	if (_configuration.HasKey(CONF_DAEMON)) {
		if (_configuration[CONF_DAEMON] == V_BOOL)
			return _configuration[CONF_DAEMON];
		return false;
	}
	return false;
}

string ConfigFile::GetServicesInfo() {
	if (_servicesInfo == "") {
		FinishServiceInfo();
		_servicesInfo = _listeningSockets.str();
	}
	return _servicesInfo;
}

bool ConfigFile::SaveXML(string path) {
	return _configuration.SerializeToXmlFile(path);
}

bool ConfigFile::ValidateString(Variant &node, string name, bool notNull,
		uint32_t minLength, uint32_t maxLength) {
	if (notNull && (VariantType) node[name] == V_NULL) {
		FATAL("Invalid %s. It must be present and not null",
				STR(name));
		return false;
	}

	if ((VariantType) node[name] == V_NULL)
		return true;

	if ((VariantType) node[name] != V_STRING) {
		FATAL("Invalid %s. It must be present and must be string", STR(name));
		return false;
	}

	string temp = node[name];

	if (temp.length() < minLength || temp.length() > maxLength) {
		FATAL("Invalid %s. It must be present, must be string and have length between %d and %d",
				STR(name), minLength, maxLength);
		return false;
	}

	return true;
}

bool ConfigFile::ValidateString(Variant &node, string name, bool notNull,
		uint32_t size, ...) {
	if (notNull && (VariantType) node[name] == V_NULL) {
		FATAL("Invalid %s. It must be present and not null",
				STR(name));
		return false;
	}

	if ((VariantType) node[name] == V_NULL)
		return true;

	if ((VariantType) node[name] != V_STRING) {
		FATAL("Invalid %s. It must be present and must be string", STR(name));
		return false;
	}

	string temp = node[name];

	map<string, string> allowedValues;

	va_list arguments;
	va_start(arguments, size);
	for (uint32_t i = 0; i < size; i++) {
		char *pTemp = va_arg(arguments, char *);
		allowedValues[pTemp] = pTemp;
	}
	va_end(arguments);

	if (!MAP_HAS1(allowedValues, temp)) {
		FATAL("Invalid value: %s. Available alternatives are", STR(temp));

		FOR_MAP(allowedValues, string, string, i) {
			FATAL("%s", STR(MAP_KEY(i)));
		}
		return false;
	}

	return true;
}

bool ConfigFile::ValidateMap(Variant &node, string name, bool notNull,
		uint32_t minLength, uint32_t maxLength) {
	if (!ValidateMap(node[name], notNull, minLength, maxLength)) {
		FATAL("Invalid %s. Rules: NotNull: %d; MinSize: %d; MaxSize: %d",
				STR(name),
				notNull, minLength, maxLength);
		return false;
	}
	return true;
}

bool ConfigFile::ValidateMap(Variant &node, bool notNull,
		uint32_t minLength, uint32_t maxLength) {
	if (notNull && (VariantType) node == V_NULL) {
		FATAL("Array can't be null");
		return false;
	}

	if ((VariantType) node == V_NULL)
		return true;

	if ((VariantType) node != V_MAP) {
		FATAL("This is not an array: %s", STR(node.ToString()));
		return false;
	}

	if (node.MapSize() < minLength || node.MapSize() > maxLength) {
		FATAL("Invalid array size: %d. Must be between %d and %d",
				node.MapSize(), minLength, maxLength);
		return false;
	}

	return true;
}

bool ConfigFile::ValidateInteger(Variant &node, string name, bool notNull,
		uint32_t minVal, uint32_t maxVal) {
	if (notNull && (VariantType) node[name] == V_NULL) {
		FATAL("Invalid %s. It must be present and not null",
				STR(name));
		return false;
	}

	if ((VariantType) node[name] == V_NULL)
		return true;

	if (!(VariantType) node[name].IsNumeric()) {
		FATAL("Invalid %s. It must be numeric",
				STR(name));
		return false;
	}

	uint32_t temp = (uint32_t) node[name];
	if ((double) temp != (double) node[name]) {
		FATAL("Invalid %s. It must be unsigned integer",
				STR(name));
		return false;
	}

	if (temp < minVal || temp > maxVal) {
		FATAL("Invalid %s. It must be between %u and %u",
				STR(name), minVal, maxVal);
		return false;
	}

	return true;
}

bool ConfigFile::ValidateDouble(Variant &node, string name, bool notNull,
		double minVal, double maxVal) {
	if (notNull && (VariantType) node[name] == V_NULL) {
		FATAL("Invalid %s. It must be present and not null",
				STR(name));
		return false;
	}

	if ((VariantType) node[name] == V_NULL)
		return true;

	if (!(VariantType) node[name].IsNumeric()) {
		FATAL("Invalid %s. It must be numeric",
				STR(name));
		return false;
	}

	if ((double) node[name] < minVal || (double) node[name] > maxVal) {
		FATAL("Invalid %s. It must be between %.2f and %.2f",
				STR(name), minVal, maxVal);
		return false;
	}

	return true;
}

bool ConfigFile::ValidateBoolean(Variant &node, string name, bool notNull) {
	if (notNull && (VariantType) node[name] == V_NULL) {
		FATAL("Invalid %s. It must be present and not null",
				STR(name));
		return false;
	}

	if ((VariantType) node[name] == V_NULL)
		return true;

	if ((VariantType) node[name] != V_BOOL) {
		FATAL("This is not a boolean value: %s. In node:\n%s",
				STR(name), STR(node.ToString()));
		return false;
	}

	return true;
}

bool ConfigFile::ValidateLogAppenders(vector<Variant> &logAppenders) {
	if (!ValidateMap(_configuration, CONF_LOG_APPENDERS, true, 1, 999))
		return false;
	for (uint32_t i = 1; i <= _configuration[CONF_LOG_APPENDERS].MapSize(); i++) {
		Variant temp = _configuration[CONF_LOG_APPENDERS][i];
		if (!ValidateLogAppender(temp))
			return false;
		ADD_VECTOR_BEGIN(logAppenders, temp);
	}
	return true;
}

bool ConfigFile::ValidateLogAppender(Variant &node) {
	if (!ValidateMap(node, true, 2, 4)) {
		FATAL("Invalid log appender. It must be present, must be an array and not empty");
		return false;
	}

	if (!ValidateString(node, CONF_LOG_APPENDER_NAME, true, 1, 64)) {
		return false;
	}

	if (!ValidateString(node, CONF_LOG_APPENDER_TYPE, true, 3,
			CONF_LOG_APPENDER_TYPE_COLORED_CONSOLE,
			CONF_LOG_APPENDER_TYPE_CONSOLE,
			CONF_LOG_APPENDER_TYPE_FILE)) {
		return false;
	}

	if (!ValidateInteger(node, CONF_LOG_APPENDER_LEVEL, true, 0, 6)) {
		return false;
	}

	string type = node[CONF_LOG_APPENDER_TYPE];
	if (type == CONF_LOG_APPENDER_TYPE_FILE) {
		if (!ValidateString(node, CONF_LOG_APPENDER_FILE_NAME, true, 1, 999)) {
			return false;
		}
	}

	return true;
}

bool ConfigFile::ValidateAcceptors(Variant &acceptors) {
	if (!ValidateMap(acceptors, true, 1, 999))
		return false;

	FOR_MAP(acceptors, string, Variant, i) {
		if (!ValidateAcceptor(MAP_VAL(i)))
			return false;
	}
	return true;
}

bool ConfigFile::ValidateAcceptor(Variant &node) {
	if (!ValidateMap(node, true, 1, 999)) {
		FATAL("Invalid acceptor. It must be present, must be an array and not empty");
		return false;
	}

	if (!ValidateString(node, CONF_IP, true, 7, 15)) {
		return false;
	}

	if (!ValidateInteger(node, CONF_PORT, true, 1, 65535)) {
		return false;
	}

	if (!ValidateString(node, CONF_SSL_CERT, false, 1, 1024)) {
		return false;
	}

	if (!ValidateString(node, CONF_SSL_KEY, false, 1, 1024)) {
		return false;
	}

	if (!ValidateString(node, CONF_PROTOCOL, true, 1, 999)) {
		return false;
	}

	return true;
}

bool ConfigFile::ValidateApplications() {
	if (!ValidateMap(_configuration, CONF_APPLICATIONS, true, 1, 999))
		return false;
	if (!ValidateString(_configuration[CONF_APPLICATIONS],
			CONF_APPLICATIONS_ROOTDIRECTORY, false, 1, 512)) {
		return false;
	}
	_rootApplicationsDirectory = (string) (_configuration[CONF_APPLICATIONS]
			[CONF_APPLICATIONS_ROOTDIRECTORY]);
	if (_rootApplicationsDirectory.length() > 0) {
		if (_rootApplicationsDirectory[_rootApplicationsDirectory.length() - 1] != PATH_SEPARATOR)
			_rootApplicationsDirectory += PATH_SEPARATOR;
	}
	_configuration[CONF_APPLICATIONS].RemoveKey(CONF_APPLICATIONS_ROOTDIRECTORY);

	FOR_MAP(_configuration[CONF_APPLICATIONS], string, Variant, i) {
		if (!ValidateApplication(MAP_VAL(i)))
			return false;
	}
	return true;
}

bool ConfigFile::ValidateApplication(Variant &node) {
	if (!ValidateMap(node, true, 1, 999)) {
		FATAL("Invalid application. It must be present, must be an array and not empty");
		return false;
	}
	if (!ValidateString(node, CONF_APPLICATION_NAME, true, 1, 64)) {
		return false;
	}

	if (node.HasKey(CONF_ACCEPTORS)) {
		if (!ValidateAcceptors(node[CONF_ACCEPTORS])) {
			FATAL("Unable to validate acceptors on application %s",
					STR(node[CONF_APPLICATION_NAME]));
			return false;
		}
	}

	if (!ValidateString(node, CONF_PROTOCOL, true, 1,
			CONF_PROTOCOL_DYNAMICLINKLIBRARY)) {
		return false;
	}

	if (!ValidateBoolean(node, CONF_APPLICATION_DEFAULT, false)) {
		return false;
	}

	if (!ValidateBoolean(node, CONF_APPLICATION_GENERATE_META_FILES, false)) {
		return false;
	}

	if (!ValidateBoolean(node, CONF_APPLICATION_VALIDATEHANDSHAKE, false)) {
		return false;
	}

	if (!ValidateBoolean(node, CONF_APPLICATION_KEYFRAMESEEK, false)) {
		return false;
	}

	if (!ValidateInteger(node, CONF_APPLICATION_CLIENTSIDEBUFFER, false, 0, 30)) {
		return false;
	}

	if (!ValidateDouble(node, CONF_APPLICATION_SEEKGRANULARITY, false, 0.1, 600)) {
		return false;
	}

	if (!ValidateString(node, CONF_APPLICATION_MEDIAFOLDER, false, 1, 999)) {
		return false;
	}

	if (!ValidateString(node, CONF_APPLICATION_DESCRIPTION, false, 1, 999)) {
		return false;
	}

	if (!ValidateMap(node, CONF_APPLICATION_ALIASES, false, 1, 999)) {
		return false;
	}

	if (node[CONF_PROTOCOL] == CONF_PROTOCOL_DYNAMICLINKLIBRARY) {
		if (!ValidateString(node, CONF_APPLICATION_LIBRARY, false, 1, 9999)) {
			return false;
		}
	} else {
		FATAL("Validation for %s protocol not yet implemented",
				STR(node[CONF_PROTOCOL]));
		return false;
	}

	if (node.HasKey(CONF_APPLICATION_AUTH)) {
		if (!ValidateMap(node, CONF_APPLICATION_AUTH, true, 1, 999)) {
			return false;
		}

		Variant auth = node[CONF_APPLICATION_AUTH];

		if (!ValidateString(auth, CONF_APPLICATION_AUTH_TYPE, true, 1,
				CONF_APPLICATION_AUTH_TYPE_ADOBE)) {
			return false;
		}

		if (!ValidateMap(auth, CONF_APPLICATION_AUTH_ENCODER_AGENTS, false, 1, 999)) {
			return false;
		}
	}

	return true;
}

bool ConfigFile::ValidateDNSResolver() {
	if (!ValidateMap(_configuration, CONF_DNSRESOLVER, true, 1, 999)) {
		FATAL("%s not specified", CONF_DNSRESOLVER);
		return false;
	}
	if (!ValidateString(_configuration[CONF_DNSRESOLVER], CONF_IP, true, 1, 512)) {
		return false;
	}
	if (_configuration[CONF_DNSRESOLVER].HasKey(CONF_ACCEPTORS)) {
		if (!ValidateAcceptors(_configuration[CONF_DNSRESOLVER][CONF_ACCEPTORS])) {
			FATAL("Unable to validate acceptors on dns resolver");
			return false;
		}
	}
	if (_configuration[CONF_DNSRESOLVER][CONF_IP] != CONF_DNSRESOLVER_SELF) {
		if (!ValidateInteger(_configuration[CONF_DNSRESOLVER],
				CONF_PORT, true, 1, 65535)) {
			FATAL("Unable to validate %s port", CONF_DNSRESOLVER);
			return false;
		}
		struct in_addr temp;
		if (inet_aton(STR(_configuration[CONF_DNSRESOLVER][CONF_IP]), &temp) != 1) {
			FATAL("Unable to translate %s into a valid IP adress",
					CONF_DNSRESOLVER"."CONF_IP);
			return false;
		}
		return true;
	} else {
		NYIR;
	}
	return true;
}

bool ConfigFile::ConfigureLogAppender(Variant &node) {
	BaseLogLocation *pLogLocation = NULL;
	if ((string) node[CONF_LOG_APPENDER_TYPE] == CONF_LOG_APPENDER_TYPE_COLORED_CONSOLE) {
		if (!IsDaemon()) {
			pLogLocation = new ConsoleLogLocation();
		}
	} else if ((string) node[CONF_LOG_APPENDER_TYPE] == CONF_LOG_APPENDER_TYPE_CONSOLE) {
		if (!IsDaemon()) {
			pLogLocation = new ConsoleLogLocation(false);
		}
	} else if ((string) node[CONF_LOG_APPENDER_TYPE] == CONF_LOG_APPENDER_TYPE_FILE) {
		pLogLocation = new FileLogLocation(node[CONF_LOG_APPENDER_FILE_NAME], true);
	} else {
		NYIR;
	}
	if (pLogLocation != NULL) {
		pLogLocation->SetLevel((int32_t) node[CONF_LOG_APPENDER_LEVEL]);
		Logger::AddLogLocation(pLogLocation);
	}
	return true;
}

bool ConfigFile::ConfigureAcceptors(Variant &acceptors, BaseClientApplication *pApplication) {
	if (!ValidateAcceptors(acceptors)) {
		FATAL("Unable to validate acceptors");
		return false;
	}

	FOR_MAP(acceptors, string, Variant, i) {
		if (!ConfigureAcceptor(MAP_VAL(i), pApplication)) {
			FATAL("Unable to configure acceptor %s", STR(MAP_VAL(i).ToString()));
			return false;
		}
	}

	return true;
}

bool ConfigFile::ConfigureAcceptor(Variant &node, BaseClientApplication *pApplication) {
	//1. Get the chain
	vector<uint64_t> chain;
	chain = ProtocolFactoryManager::ResolveProtocolChain(node[CONF_PROTOCOL]);
	if (chain.size() == 0) {
		WARN("Invalid protocol chain: %s", STR(node[CONF_PROTOCOL]));
		return true;
	}

	//2. Is it TCP or UDP based?
	if (chain[0] == PT_TCP) {
		//3. This is a tcp acceptor. Instantiate it and start accepting connections
		TCPAcceptor *pAcceptor = new TCPAcceptor(node[CONF_IP],
				node[CONF_PORT], node, chain);
		if (!pAcceptor->StartAccept(pApplication)) {
			FATAL("Unable to fire up acceptor from this config node: %s",
					STR(node.ToString()));
			return false;
		}
		SaveServiceInfo("tcp", node[CONF_IP], node[CONF_PORT], node[CONF_PROTOCOL],
				pApplication != NULL ? STR(pApplication->GetName()) : "#none#");

		return true;
	} else {
		//4. Ok, this is an UDP acceptor. Because of that, we can instantiate
		//the full stack. Get the stack first
		BaseProtocol *pProtocol = ProtocolFactoryManager::CreateProtocolChain(
				chain, node);
		if (pProtocol == NULL) {
			FATAL("Unable to instantiate protocol stack %s", STR(node[CONF_PROTOCOL]));
			return false;
		}

		//5. Create the carrier and bind it
		if (UDPCarrier::Create(node[CONF_IP], node[CONF_PORT], pProtocol) == NULL) {
			FATAL("Unable to instantiate UDP carrier on %s:%d",
					STR(node[CONF_IP]), (uint16_t) node[CONF_PORT]);
			pProtocol->EnqueueForDelete();
			return false;
		}

		pProtocol->SetApplication(pApplication);

		SaveServiceInfo("udp", node[CONF_IP], node[CONF_PORT], node[CONF_PROTOCOL],
				pApplication != NULL ? STR(pApplication->GetName()) : "#none#");

		//6. We are done
		return true;
	}
}

bool ConfigFile::ConfigureApplication(Variant &node) {
	if (node[CONF_PROTOCOL] == CONF_PROTOCOL_DYNAMICLINKLIBRARY) {
		Normalize(node);
		if (_pFunction == NULL) {
			_libDescriptor.libraryPath = (string) node[CONF_APPLICATION_LIBRARY];

			_libDescriptor.libHandler = LOAD_LIBRARY(STR(_libDescriptor.libraryPath), LOAD_LIBRARY_FLAGS);

			if (_libDescriptor.libHandler == NULL) {
				FATAL("Unable to open library %s. Error was: %s",
						STR(node[CONF_APPLICATION_LIBRARY]),
						OPEN_LIBRARY_ERROR);
				return false;
			}

			string functioName = "GetApplication_" + (string) node[CONF_APPLICATION_NAME];

			_libDescriptor.GetApplication =
					(GetApplication_t) GET_PROC_ADDRESS(_libDescriptor.libHandler, STR(functioName));
			if (_libDescriptor.GetApplication == NULL) {
				FATAL("Unable to find %s function. Error was: %s",
						STR(functioName),
						OPEN_LIBRARY_ERROR);
				return false;
			}

			functioName = "ReleaseApplication_" + (string) node[CONF_APPLICATION_NAME];

			_libDescriptor.ReleaseApplication =
					(ReleaseApplication_t) GET_PROC_ADDRESS(_libDescriptor.libHandler, STR(functioName));
			if (_libDescriptor.ReleaseApplication == NULL) {
				FATAL("Unable to find %s function. Error was: %s",
						STR(functioName),
						OPEN_LIBRARY_ERROR);
				return false;
			}
		}

		if (node.HasKey(CONF_APPLICATION_ALIASES) && node[CONF_APPLICATION_ALIASES] != V_NULL) {
			Variant temp = node[CONF_APPLICATION_ALIASES];
			node[CONF_APPLICATION_ALIASES].Reset();

			FOR_MAP(temp, string, Variant, i) {
				node[CONF_APPLICATION_ALIASES].PushToArray(MAP_VAL(i));
			}
		}

		if (node.HasKey(CONF_ACCEPTORS) && node[CONF_ACCEPTORS] != V_NULL) {
			Variant temp = node[CONF_ACCEPTORS];
			node[CONF_ACCEPTORS].Reset();

			FOR_MAP(temp, string, Variant, i) {
				node[CONF_ACCEPTORS].PushToArray(MAP_VAL(i));
			}
		}

		if (node[CONF_APPLICATION_DEFAULT] == V_NULL) {
			node[CONF_APPLICATION_DEFAULT] = (bool)false;
		}

		if (node[CONF_APPLICATION_GENERATE_META_FILES] == V_NULL) {
			node[CONF_APPLICATION_GENERATE_META_FILES] = (bool)false;
		}

		if (node[CONF_APPLICATION_VALIDATEHANDSHAKE] == V_NULL) {
			node[CONF_APPLICATION_VALIDATEHANDSHAKE] = (bool)true;
		}

		if (node[CONF_APPLICATION_KEYFRAMESEEK] == V_NULL) {
			node[CONF_APPLICATION_KEYFRAMESEEK] = (bool)true;
		}

		if (node[CONF_APPLICATION_SEEKGRANULARITY] == V_NULL) {
			node[CONF_APPLICATION_SEEKGRANULARITY] = (double) 1;
		}

		if (node[CONF_APPLICATION_MEDIAFOLDER] == V_NULL) {
			node[CONF_APPLICATION_MEDIAFOLDER] = "";
		}

		if (node[CONF_APPLICATION_CLIENTSIDEBUFFER] == V_NULL) {
			node[CONF_APPLICATION_CLIENTSIDEBUFFER] = (int32_t) 5;
		} else if ((int32_t) node[CONF_APPLICATION_CLIENTSIDEBUFFER] < 5) {
			node[CONF_APPLICATION_CLIENTSIDEBUFFER] = (int32_t) 5;
		}

		BaseClientApplication *pApplication = NULL;
		if (_pFunction == NULL) {
			pApplication = _libDescriptor.GetApplication(node);
		} else {
			pApplication = _pFunction(node);
		}
		if (pApplication == NULL) {
			FATAL("Unable to configure application. Function returned NULL");
			return false;
		}

		if (!pApplication->Initialize()) {
			FATAL("Unable to initialize the application: %s", STR(node.ToString()));
			return false;
		}

		if (node.HasKey(CONF_ACCEPTORS)) {
			if (!ConfigureAcceptors(node[CONF_ACCEPTORS], pApplication)) {
				FATAL("Unable to configure acceptors on application %s",
						STR(node[CONF_APPLICATION_NAME]));
				return false;
			}
		}

		if (!ClientApplicationManager::RegisterApplication(pApplication)) {
			FATAL("Unable to register application %s", STR(node.ToString()));
			return false;
		}

		string message = format("Application \"%s\"",
				STR(pApplication->GetName()));

		if (pApplication->GetAliases().size() > 0) {
			message += " (";
			for (uint32_t i = 0; i < pApplication->GetAliases().size(); i++) {
				message += pApplication->GetAliases()[i];
				if (i != pApplication->GetAliases().size() - 1)
					message += ",";
				else
					message += ")";
			}
		}
		message += " is up and running";

		INFO("%s", STR(message));

		ADD_VECTOR_END(_libraryHandlers, _libDescriptor.libHandler);

		return true;
	} else {
		FATAL("Configuration for %s protocol not yet implemented",
				STR(node[CONF_PROTOCOL]));
		return false;
	}
}

void ConfigFile::InitServiceInfo() {
	_listeningSockets.clear();
	_servicesInfo = "";

	_listeningSockets << "+-----------------------------------------------------------------------------+" << endl;
	_listeningSockets << "|";
	_listeningSockets.width(77);
	_listeningSockets << "Services";
	_listeningSockets << "|" << endl;
	_listeningSockets << "+---+---------------+-----+-------------------------+-------------------------+" << endl;
	_listeningSockets << "| c |      ip       | port|   protocol stack name   |     application name    |" << endl;
}

void ConfigFile::SaveServiceInfo(string carrier, string ip, uint16_t port, string protocol, string appName) {
	_listeningSockets << "+---+---------------+-----+-------------------------+-------------------------+" << endl;
	_listeningSockets << "|";
	_listeningSockets.width(3);
	_listeningSockets << carrier;
	_listeningSockets << "|";

	_listeningSockets.width(3 * 4 + 3);
	_listeningSockets << ip;
	_listeningSockets << "|";

	_listeningSockets.width(5);
	_listeningSockets << port;
	_listeningSockets << "|";

	_listeningSockets.width(25);
	_listeningSockets << protocol;
	_listeningSockets << "|";

	_listeningSockets.width(25);
	_listeningSockets << appName;
	_listeningSockets << "|";

	_listeningSockets << endl;
}

void ConfigFile::FinishServiceInfo() {
	_listeningSockets << "+---+---------------+-----+-------------------------+-------------------------+" << endl;
}

void ConfigFile::Normalize(Variant &appConfigurationNode) {
	//FINEST("Before:\n%s", STR(configuration.ToString()));

	appConfigurationNode[CONF_APPLICATION_DIRECTORY] =
			format("%s%s%c", STR(_rootApplicationsDirectory),
			STR(appConfigurationNode[CONF_APPLICATION_NAME]), PATH_SEPARATOR);

	string name = appConfigurationNode[CONF_APPLICATION_NAME];


	//1. Normalize the library path
	if ((VariantType) appConfigurationNode[CONF_APPLICATION_LIBRARY] == V_NULL) {

		string libraryName = format(LIBRARY_NAME_PATTERN, STR(name));
		appConfigurationNode[CONF_APPLICATION_LIBRARY] = format("%s%s",
				STR(appConfigurationNode[CONF_APPLICATION_DIRECTORY]),
				STR(libraryName));
	}

	//2. Normalize the media folder
	if ((VariantType) appConfigurationNode[CONF_APPLICATION_MEDIAFOLDER] == V_NULL) {
		appConfigurationNode[CONF_APPLICATION_MEDIAFOLDER] = format("%s%s%c",
				STR(appConfigurationNode[CONF_APPLICATION_DIRECTORY]),
				CONF_APPLICATION_MEDIAFOLDER,
				PATH_SEPARATOR);
	} else {
		string mediaFolder = appConfigurationNode[CONF_APPLICATION_MEDIAFOLDER];
		if (mediaFolder[mediaFolder.length() - 1] != PATH_SEPARATOR)
			mediaFolder += PATH_SEPARATOR;
		appConfigurationNode[CONF_APPLICATION_MEDIAFOLDER] = mediaFolder;
	}


	//3. Normalize the users db file
	if (appConfigurationNode.HasKey(CONF_APPLICATION_AUTH)) {
		//FINEST("Before:\n%s", STR(configuration.ToString()));
		if (!appConfigurationNode[CONF_APPLICATION_AUTH].HasKey(
				CONF_APPLICATION_AUTH_USERS_FILE)) {
			appConfigurationNode[CONF_APPLICATION_AUTH][CONF_APPLICATION_AUTH_USERS_FILE] =
					format("%s%s",
					STR(appConfigurationNode[CONF_APPLICATION_DIRECTORY]),
					"users.lua");
		}

		//4. Normalize the encoders list
		if (appConfigurationNode[CONF_APPLICATION_AUTH].HasKey(
				CONF_APPLICATION_AUTH_ENCODER_AGENTS)) {
			Variant temp;

			FOR_MAP(appConfigurationNode[CONF_APPLICATION_AUTH][CONF_APPLICATION_AUTH_ENCODER_AGENTS],
					string, Variant, i) {
				temp[MAP_VAL(i)] = (bool)true;
			}
			appConfigurationNode[CONF_APPLICATION_AUTH][CONF_APPLICATION_AUTH_ENCODER_AGENTS] = temp;

			//FINEST("temp:\n%s", STR(temp.ToString()));
		}

		//FINEST("After:\n%s", STR(configuration.ToString()));
	}

	//4. Normalize the crt/key location inside acceptors
	if (appConfigurationNode.HasKey(CONF_ACCEPTORS)) {

		FOR_MAP(appConfigurationNode[CONF_ACCEPTORS], string, Variant, i) {
			if (MAP_VAL(i)[CONF_SSL_KEY] != V_STRING) {
				MAP_VAL(i).RemoveKey(CONF_SSL_CERT);
				MAP_VAL(i).RemoveKey(CONF_SSL_KEY);
				continue;
			}
			if (MAP_VAL(i)[CONF_SSL_CERT] != V_STRING) {
				MAP_VAL(i).RemoveKey(CONF_SSL_CERT);
				MAP_VAL(i).RemoveKey(CONF_SSL_KEY);
				continue;
			}

			string key = MAP_VAL(i)[CONF_SSL_KEY];
			string cert = MAP_VAL(i)[CONF_SSL_CERT];

			if (key != "") {
				if (key[0] != PATH_SEPARATOR) {
					key = format("%s%s",
							STR(appConfigurationNode[CONF_APPLICATION_DIRECTORY]),
							STR(key));
				}
			}

			if (cert != "") {
				if (cert[0] != PATH_SEPARATOR) {
					cert = format("%s%s",
							STR(appConfigurationNode[CONF_APPLICATION_DIRECTORY]),
							STR(cert));
				}
			}

			MAP_VAL(i)[CONF_SSL_KEY] = key;
			MAP_VAL(i)[CONF_SSL_CERT] = cert;
		}
	}
}

