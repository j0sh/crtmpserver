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


#include "streaming/basestream.h"
#include "streaming/streamsmanager.h"
#include "protocols/baseprotocol.h"

BaseStream::BaseStream(BaseProtocol *pProtocol, StreamsManager *pStreamsManager,
		uint64_t type, string name) {
	_pStreamsManager = pStreamsManager;
	_type = type;
	_uniqueId = _pStreamsManager->GenerateUniqueId();
	_pProtocol = pProtocol;
	_name = name;
	_pStreamsManager->RegisterStream(this);
	//FINEST("Stream %u created: %s", _uniqueId, STR(tagToString(_type)));
}

BaseStream::~BaseStream() {
	_pStreamsManager->UnRegisterStream(this);
	//FINEST("Stream %u deleted: %s", _uniqueId, STR(tagToString(_type)));
}

StreamsManager *BaseStream::GetStreamsManager() {
	return _pStreamsManager;
}

uint64_t BaseStream::GetType() {
	return _type;
}

uint32_t BaseStream::GetUniqueId() {
	return _uniqueId;
}

string BaseStream::GetName() {
	return _name;
}

void BaseStream::SetName(string name) {
	if (_name != "") {
		ASSERT("name already set");
	}
	_name = name;
}

BaseProtocol * BaseStream::GetProtocol() {
	return _pProtocol;
}

bool BaseStream::IsEnqueueForDelete() {
	if (_pProtocol != NULL)
		return _pProtocol->IsEnqueueForDelete();
	return false;
}

void BaseStream::EnqueueForDelete() {
	if (_pProtocol != NULL) {
		_pProtocol->EnqueueForDelete();
	} else {
		delete this;
	}
}

