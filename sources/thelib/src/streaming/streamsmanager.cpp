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


#include "streaming/streamsmanager.h"
#include "streaming/baseoutstream.h"
#include "streaming/streamstypes.h"
#include "protocols/baseprotocol.h"
#include "application/baseclientapplication.h"

StreamsManager::StreamsManager(BaseClientApplication *pApplication) {
	_uniqueIdGenerator = 1;
	_pApplication = pApplication;
}

StreamsManager::~StreamsManager() {
}

uint32_t StreamsManager::GenerateUniqueId() {
	return _uniqueIdGenerator++;
}

bool StreamsManager::RegisterStream(BaseStream *pStream) {
	//1. Test to see if we had registered this stream before
	if (MAP_HAS1(_streamsByUniqueId, pStream->GetUniqueId())) {
		FATAL("Stream with unique ID %u already registered", pStream->GetUniqueId());
		return false;
	}

	//2. Register the stream
	_streamsByUniqueId[pStream->GetUniqueId()] = pStream;
	if (pStream->GetProtocol() != NULL)
		_streamsByProtocolId[pStream->GetProtocol()->GetId()][pStream->GetUniqueId()] = pStream;
	_streamsByType[pStream->GetType()][pStream->GetUniqueId()] = pStream;
	_streamsByName[pStream->GetName()][pStream->GetUniqueId()] = pStream;

	_pApplication->SignalStreamRegistered(pStream);

	//3. Done
	return true;
}

void StreamsManager::UnRegisterStream(BaseStream *pStream) {
	bool signalStreamUnregistered = MAP_HAS1(_streamsByUniqueId, pStream->GetUniqueId());
	MAP_ERASE1(_streamsByUniqueId, pStream->GetUniqueId());
	if (pStream->GetProtocol() != NULL)
		MAP_ERASE2(_streamsByProtocolId, pStream->GetProtocol()->GetId(), pStream->GetUniqueId());
	MAP_ERASE2(_streamsByType, pStream->GetType(), pStream->GetUniqueId());
	MAP_ERASE2(_streamsByName, pStream->GetName(), pStream->GetUniqueId());
	if (signalStreamUnregistered)
		_pApplication->SignalStreamUnRegistered(pStream);
}

void StreamsManager::UnRegisterStreams(uint32_t protocolId) {
	map<uint32_t, BaseStream *> streams = FindByProtocolId(protocolId);

	FOR_MAP(streams, uint32_t, BaseStream *, i) {
		UnRegisterStream(MAP_VAL(i));
	}
}

bool StreamsManager::StreamNameAvailable(string streamName) {
	if (_pApplication->GetAllowDuplicateInboundNetworkStreams())
		return true;
	return FindByTypeByName(ST_IN_NET, streamName, true, false).size() == 0;
}

map<uint32_t, BaseStream *> StreamsManager::GetAllStreams() {
	return _streamsByUniqueId;
}

map<uint32_t, BaseOutStream *> StreamsManager::GetWaitingSubscribers(string streamName, uint64_t inboundStreamType) {
	//1. Validate the inbound stream type
	if (!TAG_KIND_OF(inboundStreamType, ST_IN))
		return map<uint32_t, BaseOutStream *>();

	//2. Get get the short version of the stream name
	vector<string> parts;
	split(streamName, "?", parts);
	string shortName = parts[0];
	//FINEST("short name: %s; long name: %s", STR(shortName), STR(streamName));

	//2. get the 2 kinds of subscribers
	map<uint32_t, BaseStream *> shortSubscribers;
	map<uint32_t, BaseStream *> longSubscribers;
	shortSubscribers = FindByTypeByName(ST_OUT, shortName, true, false);
	longSubscribers = FindByTypeByName(ST_OUT, streamName, true, false);

	//	FINEST("short count: %"PRIz"u; long count: %"PRIz"u",
	//			shortSubscribers.size(), longSubscribers.size());

	//3. merge them
	map<uint32_t, BaseOutStream *> result;

	FOR_MAP(shortSubscribers, uint32_t, BaseStream *, i) {
		if (((BaseOutStream *) MAP_VAL(i))->IsLinked())
			continue;
		if (!((BaseOutStream *) MAP_VAL(i))->IsCompatibleWithType(inboundStreamType))
			continue;
		result[MAP_KEY(i)] = (BaseOutStream *) MAP_VAL(i);
	}

	FOR_MAP(longSubscribers, uint32_t, BaseStream *, i) {
		if (((BaseOutStream *) MAP_VAL(i))->IsLinked())
			continue;
		if (!((BaseOutStream *) MAP_VAL(i))->IsCompatibleWithType(inboundStreamType))
			continue;
		result[MAP_KEY(i)] = (BaseOutStream *) MAP_VAL(i);
	}

	//4. Done
	return result;
}

map<uint32_t, BaseStream *> StreamsManager::FindByName(string name, bool partial) {
	map<uint32_t, BaseStream * > result;

	if (!partial) {
		if (MAP_HAS1(_streamsByName, name))
			return _streamsByName[name];
		return result;
	}


	for (map<string, map<uint32_t, BaseStream * > >::iterator i = _streamsByName.begin();
			i != _streamsByName.end(); i++) {
		if (MAP_KEY(i).find(name) == 0) {

			FOR_MAP(MAP_VAL(i), uint32_t, BaseStream *, j) {
				result[MAP_KEY(j)] = MAP_VAL(j);
			}
		}
	}
	return result;
}

map<uint32_t, BaseStream *> StreamsManager::FindByType(uint64_t type, bool partial) {
	map<uint32_t, BaseStream * > result;

	if (!partial) {
		if (MAP_HAS1(_streamsByType, type))
			return _streamsByType[type];
		return result;
	}

	uint64_t mask = getTagMask(type);

	for (map<uint64_t, map<uint32_t, BaseStream * > >::iterator i = _streamsByType.begin();
			i != _streamsByType.end(); i++) {
		if ((MAP_KEY(i) & mask) == type) {

			FOR_MAP(MAP_VAL(i), uint32_t, BaseStream *, j) {
				result[MAP_KEY(j)] = MAP_VAL(j);
			}
		}
	}

	return result;
}

map<uint32_t, BaseStream *> StreamsManager::FindByTypeByName(uint64_t type,
		string name, bool partialType, bool partialName) {
	//1. Filter on type
	map<uint32_t, BaseStream * > possibleResults = FindByType(type, partialType);

	//2. Filter on name
	map<uint32_t, BaseStream * > result;

	FOR_MAP(possibleResults, uint32_t, BaseStream *, i) {
		if (partialName) {
			if (MAP_VAL(i)->GetName().find(name) == 0)
				result[MAP_KEY(i)] = MAP_VAL(i);
		} else {
			if (MAP_VAL(i)->GetName() == name)
				result[MAP_KEY(i)] = MAP_VAL(i);
		}
	}

	//3. Done
	return result;
}

map<uint32_t, BaseStream *> StreamsManager::FindByProtocolId(uint32_t protocolId) {
	map<uint32_t, BaseStream * > result;
	if (MAP_HAS1(_streamsByProtocolId, protocolId))
		return _streamsByProtocolId[protocolId];
	return result;
}

map<uint32_t, BaseStream *> StreamsManager::FindByProtocolIdByType(
		uint32_t protocolId, uint64_t type, bool partial) {
	//1. Filter on protocol id
	map<uint32_t, BaseStream * > possibleResults = FindByProtocolId(protocolId);

	//2. Filter on type
	map<uint32_t, BaseStream * > result;
	uint64_t mask = partial ? getTagMask(type) : 0xffffffffffffffffLL;

	FOR_MAP(possibleResults, uint32_t, BaseStream *, i) {
		if ((MAP_VAL(i)->GetType() & mask) == type)
			result[MAP_KEY(i)] = MAP_VAL(i);
	}

	//3. Done
	return result;
}

map<uint32_t, BaseStream *> StreamsManager::FindByProtocolIdByName(uint32_t protocolId,
		string name, bool partial) {
	//1. Filter on protocol id
	map<uint32_t, BaseStream * > possibleResults = FindByProtocolId(protocolId);

	//2. Filter on name
	map<uint32_t, BaseStream * > result;

	FOR_MAP(possibleResults, uint32_t, BaseStream *, i) {
		if (partial) {
			if (MAP_VAL(i)->GetName().find(name) == 0)
				result[MAP_KEY(i)] = MAP_VAL(i);
		} else {
			if (MAP_VAL(i)->GetName() == name)
				result[MAP_KEY(i)] = MAP_VAL(i);
		}
	}

	//3. Done
	return result;
}

map<uint32_t, BaseStream *> StreamsManager::FindByProtocolIdByTypeByName(
		uint32_t protocolId, uint64_t type, string name, bool partialType,
		bool partialName) {
	//1. Filter on protocol id
	map<uint32_t, BaseStream * > possibleResults1 = FindByProtocolId(protocolId);

	//2. Filter on type
	map<uint32_t, BaseStream * > possibleResults2;
	uint64_t mask = partialType ? getTagMask(type) : 0xffffffffffffffffLL;

	FOR_MAP(possibleResults1, uint32_t, BaseStream *, i) {
		if ((MAP_VAL(i)->GetType() & mask) == type)
			possibleResults2[MAP_KEY(i)] = MAP_VAL(i);
	}

	//3. Filter on name
	map<uint32_t, BaseStream * > result;

	FOR_MAP(possibleResults2, uint32_t, BaseStream *, i) {
		if (partialName) {
			if (MAP_VAL(i)->GetName().find(name) == 0)
				result[MAP_KEY(i)] = MAP_VAL(i);
		} else {
			if (MAP_VAL(i)->GetName() == name)
				result[MAP_KEY(i)] = MAP_VAL(i);
		}
	}

	//4. Done
	return result;
}

BaseStream * StreamsManager::FindByUniqueId(uint32_t uniqueId) {
	if (MAP_HAS1(_streamsByUniqueId, uniqueId))
		return _streamsByUniqueId[uniqueId];
	return NULL;
}

