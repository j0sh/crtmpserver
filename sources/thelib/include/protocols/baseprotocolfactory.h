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


#ifndef _BASEPROTOCOLFACTORY_H
#define	_BASEPROTOCOLFACTORY_H

#include "common.h"

class BaseProtocol;

/*!
	@class BaseProtocolFactory
	@brief The base class on which all protocol factories must derive from.
*/
class DLLEXP BaseProtocolFactory {
private:
	static uint32_t _idGenerator;
	uint32_t _id;
public:
	/*!
		Constructor: Increments the _idGenerator parameter.
	*/
	BaseProtocolFactory();
	virtual ~BaseProtocolFactory();

	/*!
		Returns the factory's ID.
	*/
	uint32_t GetId();

	/*!
		Function that will contain all the protocols that will be handled by the factory.
	*/
	virtual vector<uint64_t> HandledProtocols() = 0;
	/*!
		Function that will contain all the protocol chains that will be handled by the factory.
	*/
	virtual vector<string> HandledProtocolChains() = 0;
	/*!
		This functions is where the protocol chains whose names are all defined, are resolved.
		@param name: The name given to the protocol chain.
	*/
	virtual vector<uint64_t> ResolveProtocolChain(string name) = 0;
	/*!
		This function is where protocols are spawned.
		@param type: The protocol to be spawned.
		@param parameters: Each protocol can have parameters. This is optional.
	*/
	virtual BaseProtocol *SpawnProtocol(uint64_t type, Variant &parameters) = 0;
};

#endif	/* _BASEPROTOCOLFACTORY_H */


