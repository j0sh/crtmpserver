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


#include "protocols/baseprotocol.h"
#include "netio/netio.h"
#include "protocols/protocolmanager.h"
#include "application/baseclientapplication.h"
#include "protocols/tcpprotocol.h"

//#define LOG_CONSTRUCTOR_DESTRUCTOR

uint32_t BaseProtocol::_idGenerator = 0;

BaseProtocol::BaseProtocol(uint64_t type) {
	_id = ++_idGenerator;
	_type = type;
	_pFarProtocol = NULL;
	_pNearProtocol = NULL;
	_deleteFar = true;
	_deleteNear = true;
	_enqueueForDelete = false;
	_gracefullyEnqueueForDelete = false;
	_pApplication = NULL;
#ifdef LOG_CONSTRUCTOR_DESTRUCTOR
	FINEST("Protocol with id %u of type %s created; F: %p,N: %p, DF: %hhu, DN: %hhu",
			_id, STR(tagToString(_type)),
			_pFarProtocol, _pNearProtocol, _deleteFar, _deleteNear);
#endif
	ProtocolManager::RegisterProtocol(this);
	GETCLOCKS(_creationTimestamp);
	_creationTimestamp /= (double) CLOCKS_PER_SECOND;
	_creationTimestamp *= 1000.00;
}

BaseProtocol::~BaseProtocol() {
#ifdef LOG_CONSTRUCTOR_DESTRUCTOR
	FINEST("Protocol with id %u of type %s going to be deleted; F: %p,N: %p, DF: %hhu, DN: %hhu",
			_id, STR(tagToString(_type)),
			_pFarProtocol, _pNearProtocol, _deleteFar, _deleteNear);
#endif
	BaseProtocol *pFar = _pFarProtocol;
	BaseProtocol *pNear = _pNearProtocol;

	_pFarProtocol = NULL;
	_pNearProtocol = NULL;
	if (pFar != NULL) {
		pFar->_pNearProtocol = NULL;
		if (_deleteFar) {
			pFar->EnqueueForDelete();
		}
	}
	if (pNear != NULL) {
		pNear->_pFarProtocol = NULL;
		if (_deleteNear) {
			pNear->EnqueueForDelete();
		}
	}
#ifdef LOG_CONSTRUCTOR_DESTRUCTOR
	FINEST("Protocol with id %u of type %s deleted; F: %p,N: %p, DF: %hhu, DN: %hhu",
			_id, STR(tagToString(_type)),
			_pFarProtocol, _pNearProtocol, _deleteFar, _deleteNear);
#endif
	ProtocolManager::UnRegisterProtocol(this);
}

uint64_t BaseProtocol::GetType() {
	return _type;
}

uint32_t BaseProtocol::GetId() {
	return _id;
}

double BaseProtocol::GetSpawnTimestamp() {
	return _creationTimestamp;
}

BaseProtocol *BaseProtocol::GetFarProtocol() {
	return _pFarProtocol;
}

void BaseProtocol::SetFarProtocol(BaseProtocol *pProtocol) {
	if (!AllowFarProtocol(pProtocol->_type)) {
		ASSERT("Protocol %s can't accept a far protocol of type: %s",
				STR(tagToString(_type)),
				STR(tagToString(pProtocol->_type)));
	}
	if (!pProtocol->AllowNearProtocol(_type)) {
		ASSERT("Protocol %s can't accept a near protocol of type: %s",
				STR(tagToString(pProtocol->_type)),
				STR(tagToString(_type)));
	}
	if (_pFarProtocol == NULL) {
		_pFarProtocol = pProtocol;
		pProtocol->SetNearProtocol(this);
#ifdef LOG_CONSTRUCTOR_DESTRUCTOR
		FINEST("Protocol with id %u of type %s setted up; F: %p,N: %p, DF: %hhu, DN: %hhu",
				_id, STR(tagToString(_type)),
				_pFarProtocol, _pNearProtocol, _deleteFar, _deleteNear);
#endif
	} else {
		if (_pFarProtocol != pProtocol) {
			ASSERT("Far protocol already present");
		}
	}
}

void BaseProtocol::ResetFarProtocol() {
	if (_pFarProtocol != NULL) {
		_pFarProtocol->_pNearProtocol = NULL;
	}
	_pFarProtocol = NULL;
}

BaseProtocol *BaseProtocol::GetNearProtocol() {
	return _pNearProtocol;
}

void BaseProtocol::SetNearProtocol(BaseProtocol *pProtocol) {
	if (!AllowNearProtocol(pProtocol->_type)) {
		ASSERT("Protocol %s can't accept a near protocol of type: %s",
				STR(tagToString(_type)),
				STR(tagToString(pProtocol->_type)));
	}
	if (!pProtocol->AllowFarProtocol(_type)) {
		ASSERT("Protocol %s can't accept a far protocol of type: %s",
				STR(tagToString(pProtocol->_type)),
				STR(tagToString(_type)));
	}
	if (_pNearProtocol == NULL) {
		_pNearProtocol = pProtocol;
		pProtocol->SetFarProtocol(this);
#ifdef LOG_CONSTRUCTOR_DESTRUCTOR
		FINEST("Protocol with id %u of type %s setted up; F: %p,N: %p, DF: %hhu, DN: %hhu",
				_id, STR(tagToString(_type)),
				_pFarProtocol, _pNearProtocol, _deleteFar, _deleteNear);
#endif
	} else {
		if (_pNearProtocol != pProtocol) {
			ASSERT("Near protocol already present");
		}
	}
}

void BaseProtocol::ResetNearProtocol() {
	if (_pNearProtocol != NULL)
		_pNearProtocol->_pFarProtocol = NULL;
	_pNearProtocol = NULL;
}

void BaseProtocol::DeleteNearProtocol(bool deleteNear) {
	_deleteNear = deleteNear;
}

void BaseProtocol::DeleteFarProtocol(bool deleteFar) {
	_deleteFar = deleteFar;
}

BaseProtocol *BaseProtocol::GetFarEndpoint() {
	if (_pFarProtocol == NULL) {
		return this;
	} else {
		return _pFarProtocol->GetFarEndpoint();
	}
}

BaseProtocol *BaseProtocol::GetNearEndpoint() {
	if (_pNearProtocol == NULL)
		return this;
	else
		return _pNearProtocol->GetNearEndpoint();
}

void BaseProtocol::EnqueueForDelete() {
	_enqueueForDelete = true;
	ProtocolManager::EnqueueForDelete(this);
}

void BaseProtocol::GracefullyEnqueueForDelete(bool fromFarSide) {
	if (fromFarSide)
		return GetFarEndpoint()->GracefullyEnqueueForDelete(false);

	_gracefullyEnqueueForDelete = true;
	if (GetOutputBuffer() != NULL) {
		return;
	}

	if (_pNearProtocol != NULL) {
		_pNearProtocol->GracefullyEnqueueForDelete(false);
	} else {
		EnqueueForDelete();
	}
}

bool BaseProtocol::IsEnqueueForDelete() {
	return _enqueueForDelete || _gracefullyEnqueueForDelete;

}

BaseClientApplication * BaseProtocol::GetApplication() {
	return _pApplication;
}

void BaseProtocol::SetOutboundConnectParameters(Variant &customParameters) {
	_customParameters = customParameters;
}

void BaseProtocol::GetStackStats(Variant &info, uint32_t namespaceId) {
	IOHandler *pIOHandler = GetIOHandler();
	if (pIOHandler != NULL) {
		pIOHandler->GetStats(info["carrier"], namespaceId);
	} else {
		info["carrier"] = Variant();
	}
	BaseProtocol *pTemp = GetFarEndpoint();
	while (pTemp != NULL) {
		Variant item;
		pTemp->GetStats(item, namespaceId);
		info["stack"].PushToArray(item);
		pTemp = pTemp->GetNearProtocol();
	}
}

Variant &BaseProtocol::GetCustomParameters() {
	return _customParameters;
}

BaseProtocol::operator string() {
	string result = "";
	if (GetIOHandler() != NULL) {
		switch (GetIOHandler()->GetType()) {
			case IOHT_ACCEPTOR:
				result = format("A(%d) <-> ", GetIOHandler()->GetInboundFd());
				break;
			case IOHT_TCP_CARRIER:
				result = format("CTCP(%d) <-> ", GetIOHandler()->GetInboundFd());
				break;
			case IOHT_UDP_CARRIER:
				result = format("CUDP(%d) <-> ", GetIOHandler()->GetInboundFd());
				break;
			case IOHT_TCP_CONNECTOR:
				result = format("CO(%d) <-> ", GetIOHandler()->GetInboundFd());
				break;
			case IOHT_TIMER:
				result = format("T(%d) <-> ", GetIOHandler()->GetInboundFd());
				break;
			case IOHT_STDIO:
				result = format("STDIO <-> ");
				break;
			default:
				result = format("#unknown %hhu#(%d,%d) <-> ",
						GetIOHandler()->GetType(),
						GetIOHandler()->GetInboundFd(),
						GetIOHandler()->GetOutboundFd());
				break;
		}
	}
	BaseProtocol *pTemp = GetFarEndpoint();
	while (pTemp != NULL) {
		result += pTemp->ToString(_id);
		pTemp = pTemp->_pNearProtocol;
		if (pTemp != NULL)
			result += " <-> ";
	}
	return result;
}

bool BaseProtocol::Initialize(Variant &parameters) {
	WARN("You should override bool BaseProtocol::Initialize(Variant &parameters) on protocol %s",
			STR(tagToString(_type)));
	_customParameters = parameters;
	return true;
}

IOHandler *BaseProtocol::GetIOHandler() {
	if (_pFarProtocol != NULL)
		return _pFarProtocol->GetIOHandler();
	return NULL;
}

void BaseProtocol::SetIOHandler(IOHandler *pCarrier) {
	if (_pFarProtocol != NULL)
		_pFarProtocol->SetIOHandler(pCarrier);
}

IOBuffer * BaseProtocol::GetInputBuffer() {
	if (_pFarProtocol != NULL)
		return _pFarProtocol->GetInputBuffer();
	return NULL;
}

IOBuffer * BaseProtocol::GetOutputBuffer() {
	if (_pNearProtocol != NULL)
		return _pNearProtocol->GetOutputBuffer();
	return NULL;
}

uint64_t BaseProtocol::GetDecodedBytesCount() {
	if (_pFarProtocol != NULL)
		return _pFarProtocol->GetDecodedBytesCount();
	return 0;
}

bool BaseProtocol::EnqueueForOutbound() {
	if (_pFarProtocol != NULL)
		return _pFarProtocol->EnqueueForOutbound();
	return true;
}

bool BaseProtocol::EnqueueForTimeEvent(uint32_t seconds) {
	if (_pFarProtocol != NULL)
		return _pFarProtocol->EnqueueForTimeEvent(seconds);
	return true;
}

bool BaseProtocol::TimePeriodElapsed() {
	if (_pNearProtocol != NULL)
		return _pNearProtocol->TimePeriodElapsed();
	return true;
}

void BaseProtocol::ReadyForSend() {
	if (_gracefullyEnqueueForDelete) {
		EnqueueForDelete();
		return;
	}
	if (_pNearProtocol != NULL)
		_pNearProtocol->ReadyForSend();
}

void BaseProtocol::SignalInterProtocolEvent(Variant &event) {
	if (_pNearProtocol != NULL)
		_pNearProtocol->SignalInterProtocolEvent(event);
}

void BaseProtocol::SetApplication(BaseClientApplication *pApplication) {
	//1. Get the old and the new application name and id
	string oldAppName = "(none)";
	uint32_t oldAppId = 0;
	string newAppName = "(none)";
	uint32_t newAppId = 0;
	if (_pApplication != NULL) {
		oldAppName = _pApplication->GetName();
		oldAppId = _pApplication->GetId();
	}
	if (pApplication != NULL) {
		newAppName = pApplication->GetName();
		newAppId = pApplication->GetId();
	}

	//2. Are we landing on the same application?
	if (oldAppId == newAppId) {
		return;
	}

	//3. If the application is the same, return. Otherwise, unregister
	if (_pApplication != NULL) {
		_pApplication->UnRegisterProtocol(this);
		_pApplication = NULL;
	}

	//4. Setup the new application
	_pApplication = pApplication;

	//5. Register to it
	if (_pApplication != NULL) {
		_pApplication->RegisterProtocol(this);
	}

	//6. Trigger log to production
}

bool BaseProtocol::SignalInputData(IOBuffer &buffer, sockaddr_in *pPeerAddress) {
	WARN("This should be overridden. Protocol type is %s", STR(tagToString(_type)));
	return SignalInputData(buffer);
}

bool BaseProtocol::SignalInputData(int32_t recvAmount, sockaddr_in *pPeerAddress) {
	WARN("This should be overridden: %s", STR(tagToString(_type)));
	return SignalInputData(recvAmount);
}

void BaseProtocol::GetStats(Variant &info, uint32_t namespaceId) {
	info["id"] = (((uint64_t) namespaceId) << 32) | GetId();
	info["type"] = tagToString(_type);
	info["creationTimestamp"] = _creationTimestamp;
	double queryTimestamp = 0;
	GETCLOCKS(queryTimestamp);
	queryTimestamp /= (double) CLOCKS_PER_SECOND;
	queryTimestamp *= 1000.00;
	info["queryTimestamp"] = queryTimestamp;
	info["isEnqueueForDelete"] = (bool)IsEnqueueForDelete();
	if (_pApplication != NULL)
		info["applicationId"] = (((uint64_t) namespaceId) << 32) | _pApplication->GetId();
	else
		info["applicationId"] = (((uint64_t) namespaceId) << 32);
}

string BaseProtocol::ToString(uint32_t currentId) {
	string result = "";
	if (_id == currentId)
		result = format("[%s(%u)]", STR(tagToString(_type)), _id);
	else
		result = format("%s(%u)", STR(tagToString(_type)), _id);
	return result;
}


