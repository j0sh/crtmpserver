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


#ifdef HAS_PROTOCOL_RTMP

#include "protocols/rtmp/sharedobjects/so.h"
#include "protocols/rtmp/sharedobjects/somanager.h"
#include "protocols/protocolmanager.h"
#include "protocols/rtmp/basertmpprotocol.h"
#include "protocols/rtmp/messagefactories/messagefactories.h"



#define ADD_TI(protocolId, primitive) \
    ADD_VECTOR_END(_trackingInfo[(protocolId)],(primitive));

SO::SO(string name, bool persistent) {
	_name = name;
	_version = 1;
	_persistent = persistent;
	_payload["_dummy_"] = "";
	_payload.RemoveKey("_dummy_");
	_versionIncremented = false;
}

SO::~SO() {
}

string SO::GetName() {
	return _name;
}

uint32_t SO::GetVersion() {
	return _version;
}

bool SO::IsPersistent() {
	return _persistent;
}

Variant & SO::GetPayload() {
	return _payload;
}

void SO::RegisterProtocol(uint32_t protocolId) {
	_registeredProtocols[protocolId] = protocolId;

	DirtyInfo di;

	//1. Clear
	di.propertyName = format("SOT_SC_CLEAR_DATA_%u", protocolId);
	di.type = SOT_SC_CLEAR_DATA;
	ADD_VECTOR_END(_dirtyPropsByProtocol[protocolId], di);


	//2. Initial
	di.propertyName = format("SOT_SC_INITIAL_DATA_%u", protocolId);
	di.type = SOT_SC_INITIAL_DATA;
	ADD_VECTOR_END(_dirtyPropsByProtocol[protocolId], di);

	//3. Mark all properties as updated

	FOR_MAP(_payload, string, Variant, i) {
		di.propertyName = MAP_KEY(i);
		di.type = SOT_SC_UPDATE_DATA;
		ADD_VECTOR_END(_dirtyPropsByProtocol[protocolId], di);
	}
}

void SO::UnRegisterProtocol(uint32_t protocolId) {
	if (MAP_HAS1(_registeredProtocols, protocolId))
		_registeredProtocols.erase(protocolId);
	if (MAP_HAS1(_dirtyPropsByProtocol, protocolId))
		_dirtyPropsByProtocol.erase(protocolId);
}

uint32_t SO::GetSubscribersCount() {
	return _registeredProtocols.size();
}

vector<string> SO::GetPropertyNames() {
	vector<string> result;

	FOR_MAP(_payload, string, Variant, i) {
		ADD_VECTOR_END(result, MAP_KEY(i));
	}
	return result;
}

uint32_t SO::PropertiesCount() {
	return _payload.MapSize();
}

bool SO::HasProperties() {
	return _payload.MapSize() > 0;
}

bool SO::HasProperty(string propertyName) {
	return _payload.HasKey(propertyName);
}

string SO::DumpTrack() {
	string result = format("SO: %s; Ver: %u\n", STR(_name), _version);

	FOR_MAP(_dirtyPropsByProtocol, uint32_t, Dirtyness, i) {
		uint32_t protocolId = MAP_KEY(i);
		Dirtyness dirtyness = MAP_VAL(i);
		result += format("Protocol: %u\n", protocolId);

		FOR_VECTOR_ITERATOR(DirtyInfo, dirtyness, j) {
			DirtyInfo di = VECTOR_VAL(j);
			result += format("\tKey: %s; Type: %hhu\n", STR(di.propertyName),
					di.type);
		}
	}
	return result;
}

void SO::Track() {

	FOR_MAP(_dirtyPropsByProtocol, uint32_t, Dirtyness, i) {
		uint32_t protocolId = MAP_KEY(i);
		Dirtyness dirtyness = MAP_VAL(i);

		BaseRTMPProtocol *pTo = (BaseRTMPProtocol *)
				ProtocolManager::GetProtocol(protocolId);

		vector<Variant> primitives;

		FOR_VECTOR_ITERATOR(DirtyInfo, dirtyness, j) {
			string key = VECTOR_VAL(j).propertyName;
			uint8_t type = VECTOR_VAL(j).type;
			Variant primitive;
			primitive[RM_SHAREDOBJECTPRIMITIVE_TYPE] = type;
			switch (type) {
				case SOT_SC_DELETE_DATA:
				case SOT_SC_UPDATE_DATA_ACK:
					primitive[RM_SHAREDOBJECTPRIMITIVE_PAYLOAD][(uint32_t) 0] = key;
					break;
				case SOT_SC_UPDATE_DATA:
					primitive[RM_SHAREDOBJECTPRIMITIVE_PAYLOAD][key] = _payload[key];
					break;
				case SOT_SC_INITIAL_DATA:
				case SOT_SC_CLEAR_DATA:
					break;
				default:
					ASSERT("Unable to handle primitive type: %hhu", type);
			}
			ADD_VECTOR_END(primitives, primitive);
		}

		Variant message = SOMessageFactory::GetSharedObject(3, 0, 0, false, _name,
				_version, _persistent);

		for (uint32_t i = 0; i < primitives.size(); i++)
			M_SO_PRIMITIVE(message, i) = primitives[i];


		if (pTo != NULL) {
			if (!pTo->SendMessage(message)) {
				pTo->EnqueueForDelete();
			}
		}
	}
	_dirtyPropsByProtocol.clear();
	_versionIncremented = false;
}

SO::operator string() {
	return _payload.ToString();
}

Variant & SO::Get(string key) {
	if (!_payload.HasKey(key))
		_payload[key] = Variant();
	return _payload[key];
}

Variant & SO::Set(string key, Variant value, uint32_t protocolId) {
	if (!_versionIncremented) {
		_version++;
		_versionIncremented = true;
	}
	_payload[key] = value;

	FOR_MAP(_registeredProtocols, uint32_t, uint32_t, i) {
		uint32_t currentProtocolId = MAP_VAL(i);
		DirtyInfo di;
		di.propertyName = key;
		if (currentProtocolId == protocolId) {
			di.type = SOT_SC_UPDATE_DATA_ACK;
		} else {
			di.type = SOT_SC_UPDATE_DATA;
		}
		ADD_VECTOR_END(_dirtyPropsByProtocol[currentProtocolId], di);
	}

	return _payload[key];
}

void SO::UnSet(string key) {
	if (!_versionIncremented) {
		_version++;
		_versionIncremented = true;
	}

	if (_payload.HasKey(key))
		_payload.RemoveKey(key);

	FOR_MAP(_registeredProtocols, uint32_t, uint32_t, i) {
		uint32_t currentProtocolId = MAP_VAL(i);
		DirtyInfo di;
		di.propertyName = key;
		di.type = SOT_SC_DELETE_DATA;
		ADD_VECTOR_END(_dirtyPropsByProtocol[currentProtocolId], di);
	}
}
#endif /* HAS_PROTOCOL_RTMP */

