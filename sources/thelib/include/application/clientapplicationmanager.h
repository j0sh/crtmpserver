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


#ifndef _CLIENTAPPLICATIONMANAGER_H
#define	_CLIENTAPPLICATIONMANAGER_H


#include "common.h"
#include "application/baseclientapplication.h"

class BaseClientApplication;

/*!
	@brief
 */
class DLLEXP ClientApplicationManager {
private:
	static map<uint32_t, BaseClientApplication *> _applicationsById;
	static map<string, BaseClientApplication *> _applicationsByName;
	static BaseClientApplication *_pDefaultApplication;
public:
	/*!
		@brief Deletes applications registered to the base client application
	 */
	static void Shutdown();

	/*!
		@brief Registers the application using its id, name, and/or alias
		@param pClientApplication
	 */
	static bool RegisterApplication(BaseClientApplication *pClientApplication);

	/*!
		@brief Erases the application using its id, name, and/or alias
		@param pClientApplication
	 */
	static void UnRegisterApplication(BaseClientApplication *pClientApplication);
	/*!
		@brief Gets the default application based on what was indicated in the configuration file
	 */
	static BaseClientApplication *GetDefaultApplication();

	/*!
		@brief Returns the application name in string form
		@param appName
	 */
	static BaseClientApplication *FindAppByName(string appName);

	/*!
		@brief Returns the application's id
		@param id
	 */
	static BaseClientApplication *FindAppById(uint32_t id);

	/*!
		@brief Returns all applications by id.
	 */
	static map<uint32_t, BaseClientApplication *> GetAllApplications();
};


#endif	/* _CLIENTAPPLICATIONMANAGER_H */
