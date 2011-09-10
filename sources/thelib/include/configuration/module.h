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

#ifndef _MODULE_H
#define	_MODULE_H

#include "common.h"

class BaseClientApplication;
class BaseProtocolFactory;
class IOHandler;

typedef BaseClientApplication * (*GetApplicationFunction_t)(Variant configuration);
//typedef void (*ReleaseApplicationFunction_t)(BaseClientApplication * pApplication);

typedef BaseProtocolFactory * (*GetFactoryFunction_t)(Variant configuration);
//typedef void (*ReleaseFactoryFunction_t)(BaseProtocolFactory *pFactory);

struct Module {
	Variant config;
	GetApplicationFunction_t getApplication;
	GetFactoryFunction_t getFactory;
	BaseClientApplication *pApplication;
	BaseProtocolFactory *pFactory;
	LIB_HANDLER libHandler;
	vector<IOHandler *> acceptors;

	Module();
	void Release();
	bool Load();
	bool LoadLibrary();
	bool ConfigFactory();
	bool BindAcceptors();
	bool BindAcceptor(Variant &node);
	bool ConfigApplication();
};

#endif	/* _MODULE_H */

