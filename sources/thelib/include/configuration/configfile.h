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
#include "configuration/module.h"

class DLLEXP ConfigFile {
private:
	Variant _configuration;
	Variant _logAppenders;
	string _rootAppFolder;
	Variant _applications;
	map<string, string> _uniqueNames;
	GetApplicationFunction_t _staticGetApplicationFunction;
	GetFactoryFunction_t _staticGetFactoryFunction;
	map<string, Module> _modules;
	bool _isOrigin;
public:
	ConfigFile(GetApplicationFunction_t staticGetApplicationFunction,
			GetFactoryFunction_t staticGetFactoryFunction);
	virtual ~ConfigFile();

	bool IsDaemon();
	bool IsOrigin();
	string GetServicesInfo();

	bool LoadLuaFile(string path, bool forceDaemon);
	bool LoadXmlFile(string path, bool forceDaemon);

	bool ConfigLogAppenders();
	bool ConfigModules();
	bool ConfigFactories();
	bool ConfigAcceptors();
	bool ConfigInstances();
	bool ConfigApplications();
private:
	bool ConfigLogAppender(Variant &node);
	bool ConfigModule(Variant &node);

	bool Normalize();
	bool NormalizeLogAppenders();
	bool NormalizeLogAppender(Variant &node);
	bool NormalizeApplications();
	bool NormalizeApplication(Variant &node);
	bool NormalizeApplicationAcceptor(Variant &node, string baseFolder);
	bool NormalizeApplicationAliases(Variant &node);
};


#endif	/* _CONFIGFILE_H */
