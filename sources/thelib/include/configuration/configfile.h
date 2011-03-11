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


#ifndef _CONFIGFILE_H
#define	_CONFIGFILE_H

#include "common.h"

class Logger;
class BaseClientApplication;

typedef BaseClientApplication * (*GetStaticApplication_t)(Variant);
typedef BaseClientApplication * (*GetApplication_t)(Variant configuration);
typedef void (*ReleaseApplication_t)(BaseClientApplication * pApplication);

/*!
	@class ConfigFile
 */
class DLLEXP ConfigFile {
private:

	struct LibDescriptor {
		string libraryPath;
		LIB_HANDLER libHandler;
		GetApplication_t GetApplication;
		ReleaseApplication_t ReleaseApplication;
	} _libDescriptor;
private:
	Variant _configuration;
	string _rootApplicationsDirectory;
	vector<LIB_HANDLER> _libraryHandlers;
	stringstream _listeningSockets;
	string _servicesInfo;
	GetStaticApplication_t _pFunction;
public:
	ConfigFile(GetStaticApplication_t pFunction);
	virtual ~ConfigFile();

	//public API
#ifdef HAS_LUA
	/*!
		@brief Loads the lua configuration file.
		@param path: The true path to the lua configuration file.
	 */
	bool LoadLuaFile(string path, bool forceDaemon);
	bool LoadLuaString(string script, bool forceDaemon);
#endif /* HAS_LUA */
	/*!
		@brief Loads the xml configuration file.
		@param path: The true path to the xml configuration file.
	 */
	bool LoadXmlFile(string path, bool forceDaemon);
	bool LoadXmlString(string xmlContent, bool forceDaemon);
	/*!
		@brief Fetches the logAppenders attribute in the configuration file. 
	 */
	bool ConfigureLogAppenders();

#ifdef HAS_PROTOCOL_DNS
	/*!
		@brief Fetches the dnsResolver attribute in the configuration file. It also gets the parameters of the dns resolver like its port, ip, etc.
	 */
	bool ConfigureDNSResolver();
#endif /* HAS_PROTOCOL_DNS */

	/*!
		@brief Fetches the applications attribute in the configuration file. 
	 */
	bool ConfigureApplications();

	/*!
		@brief Fetches the isDaemon attribute in the configuration file.
	 */
	bool IsDaemon();
	string GetServicesInfo();

	/*!
		@brief Serializes the file into an XML format.
		@param path: True path of the file to be serialized to XML.
	 */
	bool SaveXML(string path);
private:

	//Validation
	bool ValidateString(Variant &node, string name, bool notNull,
			uint32_t minLength, uint32_t maxLength);
	bool ValidateString(Variant &node, string name, bool notNull,
			uint32_t size, ...);
	bool ValidateMap(Variant &node, string name, bool notNull,
			uint32_t minLength, uint32_t maxLength);
	bool ValidateMap(Variant &node, bool notNull,
			uint32_t minLength, uint32_t maxLength);
	bool ValidateInteger(Variant &node, string name, bool notNull,
			uint32_t minVal, uint32_t maxVal);
	bool ValidateDouble(Variant &node, string name, bool notNull,
			double minVal, double maxVal);
	bool ValidateBoolean(Variant &node, string name, bool notNull);
	bool ValidateLogAppenders(vector<Variant> &logAppenders);
	bool ValidateLogAppender(Variant &node);
	bool ValidateAcceptors(Variant &acceptors);
	bool ValidateAcceptor(Variant &node);
	bool ValidateApplications();
	bool ValidateApplication(Variant &node);
	bool ValidateDNSResolver();

	//configuration
	bool ConfigureLogAppender(Variant &node);
	bool ConfigureAcceptors(Variant &acceptors,
			BaseClientApplication *pApplication);
	bool ConfigureAcceptor(Variant &node,
			BaseClientApplication *pApplication);
	bool ConfigureApplication(Variant &node);

	//pretty print
	void InitServiceInfo();
	void SaveServiceInfo(string carrier, string ip, uint16_t port, string protocol, string appName);
	void FinishServiceInfo();

	//normalization
	void Normalize(Variant &configuration);
};


#endif	/* _CONFIGFILE_H */


