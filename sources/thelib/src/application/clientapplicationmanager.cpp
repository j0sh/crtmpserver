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


#include "application/clientapplicationmanager.h"
#include "application/baseclientapplication.h"

map<uint32_t, BaseClientApplication *> ClientApplicationManager::_applicationsById;
map<string, BaseClientApplication *> ClientApplicationManager::_applicationsByName;
BaseClientApplication *ClientApplicationManager::_pDefaultApplication = NULL;

void ClientApplicationManager::Shutdown() {

	FOR_MAP(_applicationsById, uint32_t, BaseClientApplication *, i) {
		delete MAP_VAL(i);
	}
	_applicationsById.clear();
	_applicationsByName.clear();
	_pDefaultApplication = NULL;
}

bool ClientApplicationManager::RegisterApplication(BaseClientApplication* pClientApplication) {
	if (MAP_HAS1(_applicationsById, pClientApplication->GetId())) {
		FATAL("Client application with id %u already registered",
				pClientApplication->GetId());
		return false;
	}
	if (MAP_HAS1(_applicationsByName, pClientApplication->GetName())) {
		FATAL("Client application with name `%s` already registered",
				STR(pClientApplication->GetName()));
		return false;
	}

	vector<string> aliases = pClientApplication->GetAliases();

	FOR_VECTOR_ITERATOR(string, aliases, i) {
		if (MAP_HAS1(_applicationsByName, VECTOR_VAL(i))) {
			FATAL("Client application with alias `%s` already registered",
					STR(VECTOR_VAL(i)));
			return false;
		}
	}
	_applicationsById[pClientApplication->GetId()] = pClientApplication;
	_applicationsByName[pClientApplication->GetName()] = pClientApplication;

	FOR_VECTOR_ITERATOR(string, aliases, i) {
		_applicationsByName[VECTOR_VAL(i)] = pClientApplication;
	}

	if (pClientApplication->IsDefault())
		_pDefaultApplication = pClientApplication;
	return true;
}

void ClientApplicationManager::UnRegisterApplication(BaseClientApplication* pClientApplication) {
	if (MAP_HAS1(_applicationsById, pClientApplication->GetId()))
		_applicationsById.erase(pClientApplication->GetId());
	if (MAP_HAS1(_applicationsByName, pClientApplication->GetName()))
		_applicationsByName.erase(pClientApplication->GetName());

	vector<string> aliases = pClientApplication->GetAliases();

	for (uint32_t i = 0; i < aliases.size(); i++) {
		if (MAP_HAS1(_applicationsByName, aliases[i]))
			_applicationsByName.erase(aliases[i]);
	}

	if (_pDefaultApplication != NULL) {
		if (_pDefaultApplication->GetId() == pClientApplication->GetId()) {
			_pDefaultApplication = NULL;
		}
	}

	FINEST("Application `%s` (%u) unregistered", STR(pClientApplication->GetName()),
			pClientApplication->GetId());
}

BaseClientApplication *ClientApplicationManager::GetDefaultApplication() {
	return _pDefaultApplication;
}

BaseClientApplication *ClientApplicationManager::FindAppByName(string appName) {
	if (MAP_HAS1(_applicationsByName, appName))
		return _applicationsByName[appName];
	return NULL;
}

BaseClientApplication *ClientApplicationManager::FindAppById(uint32_t id) {
	if (MAP_HAS1(_applicationsById, id))
		return _applicationsById[id];
	return NULL;
}

map<uint32_t, BaseClientApplication *> ClientApplicationManager::GetAllApplications() {
	return _applicationsById;
}

