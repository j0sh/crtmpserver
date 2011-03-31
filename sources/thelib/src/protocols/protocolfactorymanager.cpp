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




#include "protocols/protocolfactorymanager.h"
#include "protocols/baseprotocolfactory.h"
#include "protocols/baseprotocol.h"

map<uint32_t, BaseProtocolFactory *> ProtocolFactoryManager::_factoriesById;
map<uint64_t, BaseProtocolFactory *> ProtocolFactoryManager::_factoriesByProtocolId;
map<string, BaseProtocolFactory *> ProtocolFactoryManager::_factoriesByChainName;

bool ProtocolFactoryManager::RegisterProtocolFactory(BaseProtocolFactory *pFactory) {

	//1. Test to see if this factory is already registered
	if (MAP_HAS1(_factoriesById, pFactory->GetId())) {
		FATAL("Factory id %u already registered", pFactory->GetId());
		return false;
	}

	//2. Test to see if the protocol chains exported by this factory are already in use
	vector<string> protocolChains = pFactory->HandledProtocolChains();

	FOR_VECTOR(protocolChains, i) {
		if (MAP_HAS1(_factoriesByChainName, protocolChains[i])) {
			FATAL("protocol chain %s already handled by factory %u",
					STR(protocolChains[i]),
					_factoriesByChainName[protocolChains[i]]->GetId());
			return false;
		}
	}

	//3. Test to see if the protocols exported by this factory are already in use
	vector<uint64_t> protocols = pFactory->HandledProtocols();

	FOR_VECTOR(protocols, i) {
		if (MAP_HAS1(_factoriesByProtocolId, protocols[i])) {
			FATAL("protocol %"PRIx64" already handled by factory %u", protocols[i],
					_factoriesByProtocolId[protocols[i]]->GetId());
			return false;
		}
	}

	//4. Register everything

	FOR_VECTOR(protocolChains, i) {
		_factoriesByChainName[protocolChains[i]] = pFactory;
	}

	FOR_VECTOR(protocols, i) {
		_factoriesByProtocolId[protocols[i]] = pFactory;
	}

	_factoriesById[pFactory->GetId()] = pFactory;

	return true;
}

bool ProtocolFactoryManager::UnRegisterProtocolFactory(uint32_t factoryId) {
	if (!MAP_HAS1(_factoriesById, factoryId)) {
		WARN("Factory id not found: %u", factoryId);
		return true;
	}
	return UnRegisterProtocolFactory(_factoriesById[factoryId]);
}

bool ProtocolFactoryManager::UnRegisterProtocolFactory(BaseProtocolFactory *pFactory) {
	if (pFactory == NULL) {
		WARN("pFactory is NULL");
		return true;
	}

	if (!MAP_HAS1(_factoriesById, pFactory->GetId())) {
		WARN("Factory id not found: %u", pFactory->GetId());
		return true;
	}

	vector<string> protocolChains = pFactory->HandledProtocolChains();
	vector<uint64_t> protocols = pFactory->HandledProtocols();

	FOR_VECTOR(protocolChains, i) {
		_factoriesByChainName.erase(protocolChains[i]);
	}

	FOR_VECTOR(protocols, i) {
		_factoriesByProtocolId.erase(protocols[i]);
	}

	_factoriesById.erase(pFactory->GetId());

	return true;
}

vector<uint64_t> ProtocolFactoryManager::ResolveProtocolChain(string name) {
	if (!MAP_HAS1(_factoriesByChainName, name)) {
		FATAL("chain %s not registered by any protocol factory", STR(name));
		return vector<uint64_t > ();
	}

	return _factoriesByChainName[name]->ResolveProtocolChain(name);
}

BaseProtocol *ProtocolFactoryManager::CreateProtocolChain(string name,
		Variant &parameters) {
	vector<uint64_t> chain = ResolveProtocolChain(name);
	if (chain.size() == 0) {
		FATAL("Unable to create protocol chain");
		return NULL;
	}

	return CreateProtocolChain(chain, parameters);
}

BaseProtocol *ProtocolFactoryManager::CreateProtocolChain(vector<uint64_t> &chain,
		Variant &parameters) {
	BaseProtocol *pResult = NULL;

	//1. Check and see if all the protocols are handled by a factory

	FOR_VECTOR(chain, i) {
		if (!MAP_HAS1(_factoriesByProtocolId, chain[i])) {
			FATAL("protocol %"PRIx64" not handled by anyone", chain[i]);
			return NULL;
		}
	}

	//2. Spawn the protocols

	bool failed = false;

	FOR_VECTOR(chain, i) {
		BaseProtocol *pProtocol = _factoriesByProtocolId[chain[i]]->SpawnProtocol(
				chain[i], parameters);
		if (pProtocol == NULL) {
			FATAL("Unable to spawn protocol %s handled by factory %u",
					STR(tagToString(chain[i])),
					_factoriesByProtocolId[chain[i]]->GetId());
			failed = true;
			break;
		}
		if (pResult != NULL)
			pResult->SetNearProtocol(pProtocol);

		pResult = pProtocol;
	}

	if (failed) {
		if (pResult != NULL)
			delete pResult->GetFarEndpoint();
		pResult = NULL;
	} else {
		pResult = pResult->GetNearEndpoint();
	}

	return pResult;
}

string ProtocolFactoryManager::Dump() {
	string result = "Factories by id\n";

	FOR_MAP(_factoriesById, uint32_t, BaseProtocolFactory *, i) {
		result += format("\t%u\t%p\n", MAP_KEY(i), MAP_VAL(i));
	}

	result += "Factories by protocol id\n";

	FOR_MAP(_factoriesByProtocolId, uint64_t, BaseProtocolFactory *, i) {
		result += format("\t%s\t%p\n",
				STR(tagToString(MAP_KEY(i))),
				MAP_VAL(i));
	}

	result += "Factories by chain name\n";

	FOR_MAP(_factoriesByChainName, string, BaseProtocolFactory *, i) {
		result += format("\t%s\t%p\n",
				STR(MAP_KEY(i)),
				MAP_VAL(i));
	}

	return result;
}

