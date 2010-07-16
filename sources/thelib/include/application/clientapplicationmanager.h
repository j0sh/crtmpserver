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


#include "utils/core.h"
#include "application/baseclientapplication.h"

class BaseClientApplication;

class DLLEXP ClientApplicationManager {
private:
    static map<uint32_t, BaseClientApplication *> _applicationsById;
    static map<string, BaseClientApplication *> _applicationsByName;
    static BaseClientApplication *_pDefaultApplication;
public:
    static void Shutdown();
    static bool RegisterApplication(BaseClientApplication *pClientApplication);
    static void UnRegisterApplication(BaseClientApplication *pClientApplication);
    static BaseClientApplication *GetDefaultApplication();
    static BaseClientApplication *FindAppByName(string appName);
    static BaseClientApplication *FindAppById(uint32_t id);
    static map<uint32_t, BaseClientApplication *> GetAllApplications();
};


#endif	/* _CLIENTAPPLICATIONMANAGER_H */


