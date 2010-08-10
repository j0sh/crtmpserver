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


#ifndef _PROTOCOLMANAGER_H
#define	_PROTOCOLMANAGER_H

#include "new/common.h"

class BaseProtocol;

class DLLEXP ProtocolManager {
private:
	static map<uint32_t, BaseProtocol *> _activeProtocols;
	static map<uint32_t, BaseProtocol *> _deadProtocols;
public:
	static void RegisterProtocol(BaseProtocol *pProtocol);
	static void UnRegisterProtocol(BaseProtocol *pProtocol);
	static void EnqueueForDelete(BaseProtocol *pProtocol);
	static void CleanupDeadProtocols();
	static void Shutdown();

	static BaseProtocol * GetProtocol(uint32_t id,
			bool includeDeadProtocols = false);
};


#endif	/* _PROTOCOLMANAGER_H */


