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

class DLLEXP ConfigFile {
private:
    Variant _configuration;
    string _rootApplicationsDirectory;
    vector<LIB_HANDLER> _libraryHandlers;
    stringstream _listeningSockets;
    string _servicesInfo;
public:
    ConfigFile();
    virtual ~ConfigFile();

    //public API
#ifdef HAS_LUA
    bool LoadLuaFile(string path);
    bool LoadLuaString(string script);
#endif /* HAS_LUA */
    bool LoadXmlFile(string path);
    bool LoadXmlString(string xmlContent);
    bool ConfigureLogAppenders();
    bool ConfigureDNSResolver();
    bool ConfigureApplications();
    bool IsDaemon();
    string GetServicesInfo();

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


