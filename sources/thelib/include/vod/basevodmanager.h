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

#ifdef HAS_VOD_MANAGER
#ifndef _BASEVODMANAGER_H
#define	_BASEVODMANAGER_H

#include "common.h"

class BaseClientApplication;

class BaseVODManager {
private:
	BaseClientApplication *_pApplication;
	Variant _configuration;
	Variant _seekSettings;
	string _metadataFolder;
	vector<string> _origins;
	vector<string> _speedCache;
public:
	BaseVODManager(BaseClientApplication *pApplication, Variant &configuration);
	virtual ~BaseVODManager();

	bool Initialize();
	bool GetVODSourceByFileName(string type, string fileName,
			Variant &metaData);
	bool GetVODSourceByAdobeNotation(string streamName, Variant &metaData);
	bool GetAsyncVODSourceByFile(string originFilePath, Variant &parameters);
	bool GetAsyncVODSourceByAdobeNotation(string streamName, Variant &parameters);
private:
	bool GetVODSource(string &type, string &originFolder,
			string &originFilePath, Variant &metaData);
	bool GenerateMetaAndSeekFiles(Variant &metaData);
};

#endif	/* _BASEVODMANAGER_H */
#endif /* HAS_VOD_MANAGER */

