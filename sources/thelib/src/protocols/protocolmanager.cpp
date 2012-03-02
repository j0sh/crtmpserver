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


#include "protocols/protocolmanager.h"
#include "protocols/baseprotocol.h"
#include "netio/netio.h"

map<uint32_t, BaseProtocol *> ProtocolManager::_activeProtocols;
map<uint32_t, BaseProtocol *> ProtocolManager::_deadProtocols;

void ProtocolManager::RegisterProtocol(BaseProtocol *pProtocol) {
	if (MAP_HAS1(_activeProtocols, pProtocol->GetId()))
		return;
	if (MAP_HAS1(_deadProtocols, pProtocol->GetId()))
		return;
	_activeProtocols[pProtocol->GetId()] = pProtocol;
}

void ProtocolManager::UnRegisterProtocol(BaseProtocol *pProtocol) {
	if (MAP_HAS1(_activeProtocols, pProtocol->GetId()))
		_activeProtocols.erase(pProtocol->GetId());
	if (MAP_HAS1(_deadProtocols, pProtocol->GetId()))
		_deadProtocols.erase(pProtocol->GetId());
}

void ProtocolManager::EnqueueForDelete(BaseProtocol *pProtocol) {
	if (pProtocol->GetNearProtocol() == NULL) {
		FINEST("Enqueue for delete for protocol %s", STR(*pProtocol));
	}
	pProtocol->SetApplication(NULL);
	if (MAP_HAS1(_activeProtocols, pProtocol->GetId()))
		_activeProtocols.erase(pProtocol->GetId());
	if (!MAP_HAS1(_deadProtocols, pProtocol->GetId()))
		_deadProtocols[pProtocol->GetId()] = pProtocol;
}

uint32_t ProtocolManager::CleanupDeadProtocols() {
	uint32_t result = 0;
	while (_deadProtocols.size() > 0) {
		BaseProtocol *pBaseProtocol = MAP_VAL(_deadProtocols.begin());
		delete pBaseProtocol;
		result++;
	}
	return result;
}

void ProtocolManager::Shutdown() {
	while (_activeProtocols.size() > 0) {
		EnqueueForDelete(MAP_VAL(_activeProtocols.begin()));
	}
}

BaseProtocol * ProtocolManager::GetProtocol(uint32_t id,
		bool includeDeadProtocols) {
	if (!includeDeadProtocols && MAP_HAS1(_deadProtocols, id))
		return NULL;
	if (MAP_HAS1(_activeProtocols, id))
		return _activeProtocols[id];
	if (MAP_HAS1(_deadProtocols, id))
		return _deadProtocols[id];
	return NULL;
}

const map<uint32_t, BaseProtocol *> & ProtocolManager::GetActiveProtocols() {
	return _activeProtocols;
}

void ProtocolManager::GetActiveProtocols(map<uint32_t, BaseProtocol *> &result,
		protocolManagerFilter_f filter) {
	result.clear();
	if (filter == NULL) {
		result = _activeProtocols;
		return;
	}

	FOR_MAP(_activeProtocols, uint32_t, BaseProtocol *, i) {
		if (!filter(MAP_VAL(i)))
			continue;
		result[MAP_VAL(i)->GetId()] = MAP_VAL(i);
	}
}

bool protocolManagerNetworkedProtocolsFilter(BaseProtocol *pProtocol) {
	IOHandler *pIOHandler = pProtocol->GetIOHandler();
	if ((pIOHandler == NULL)
			|| ((pIOHandler->GetType() != IOHT_TCP_CARRIER)
			&& (pIOHandler->GetType() != IOHT_UDP_CARRIER)))
		return false;
	return true;
}

bool protocolManagerNearProtocolsFilter(BaseProtocol *pProtocol) {
	return pProtocol->GetNearProtocol() == NULL;
}
