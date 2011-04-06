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


#ifndef _PROTOCOLFACTORYMANAGER_H
#define	_PROTOCOLFACTORYMANAGER_H

#include "common.h"

class BaseProtocolFactory;
class BaseProtocol;

/*!
	@class ProtocolFactoryManager
	@brief Class that manages protocol factories.
 */
class DLLEXP ProtocolFactoryManager {
private:
	static map<uint32_t, BaseProtocolFactory *> _factoriesById;
	static map<uint64_t, BaseProtocolFactory *> _factoriesByProtocolId;
	static map<string, BaseProtocolFactory *> _factoriesByChainName;
public:
	//(Un)Register functionality
	/*!
		@brief Registers the atomic protocols and protocol chains of the factory.
		@param pFactory: Pointer to the BaseProtocolFactory that contains the protocols and protocol chains.
	 */
	static bool RegisterProtocolFactory(BaseProtocolFactory *pFactory);
	/*!
		@brief Erases the atomic protocols and protocol chains of the factory
		@param factoryId: ID of the protocol factory to be erased.
	 */
	static bool UnRegisterProtocolFactory(uint32_t factoryId);
	/*!
		@brief Erases the atomic protocols and protocol chains of the factory
		@param pFactory: Pointer to the BaseProtocolFactory that contains the protocols and protocol chains.
	 */
	static bool UnRegisterProtocolFactory(BaseProtocolFactory *pFactory);

	//protocol creation
	/*!
		@brief Resolves the protocol chain based on the given name.
		@param name: Name of the protocol chain to be resolved.
	 */
	static vector<uint64_t> ResolveProtocolChain(string name);
	/*!
		@brief Creates protocol chains.
		@param name: The chain's name in string. This will be resolved using the ResolveProtocolChain function.
		@param parameters: The parameters that come with the chain.
	 */
	static BaseProtocol *CreateProtocolChain(string name, Variant &parameters);
	/*!
		@brief The function that spawns the protocols in the protocol chain.
		@param chain: Vector that resulted from resolving the protocol chain.
		@param parameters: The parameters that come with the chain.
	 */
	static BaseProtocol *CreateProtocolChain(vector<uint64_t> &chain,
			Variant &parameters);
private:
	static string Dump();
};


#endif	/* _PROTOCOLFACTORYMANAGER_H */


