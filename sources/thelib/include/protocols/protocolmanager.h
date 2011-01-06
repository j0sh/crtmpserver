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

#include "common.h"

class BaseProtocol;

/*!
	@class ProtocolManager
	@brief Class where that handles atomic protocols
*/
class DLLEXP ProtocolManager {
private:
	static map<uint32_t, BaseProtocol *> _activeProtocols;
	static map<uint32_t, BaseProtocol *> _deadProtocols;
public:
	/*!
		@brief Function that registers the protocol. If the protocol is already active or dead, this function will do nothing.
		@param pProtocol: The pointer to the protocol to be registered.
	*/
	static void RegisterProtocol(BaseProtocol *pProtocol);
	/*!
		@brief Function that ters erases protocol.
		@param pProtocol: The pointer to the protocol to be erased.
	*/
	static void UnRegisterProtocol(BaseProtocol *pProtocol);
	/*!
		@brief Function which deletes the protocol. This is called during shutdown to delete the protocol and NULL the application set for the protocol.
		@param pProtocol: The pointer to the protocol to be erased.
	*/
	static void EnqueueForDelete(BaseProtocol *pProtocol);
	/*!
		@brief This function makes sure that the memory allocated to all dead protocols are deleted.
	*/
	static uint32_t CleanupDeadProtocols();
	/*!
		@brief This function deletes all active protocols.
	*/
	static void Shutdown();
	/*!
		@brief Returns the BaseProtocol pointer to the protocol referred to by the provided id.
		@param id: ID of the protocol to be returned
		@param includeDeadProtocols: A boolean that can optionally be set to true of dead protocols are desired. It is by default set to false to include active protocols only.
	*/
	static BaseProtocol * GetProtocol(uint32_t id,
			bool includeDeadProtocols = false);
	/*!
		@brief Returns the vector that contains all active protocols
	*/
	static map<uint32_t, BaseProtocol *> GetActiveProtocols();
};


#endif	/* _PROTOCOLMANAGER_H */


