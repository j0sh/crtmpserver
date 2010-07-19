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


#include "streaming/baseinstream.h"
#include "streaming/baseoutstream.h"
#include "streaming/streamstypes.h"

BaseInStream::BaseInStream(BaseProtocol *pProtocol,
		StreamsManager *pStreamsManager, uint64_t type, string name)
: BaseStream(pProtocol, pStreamsManager, type, name) {
	if (!TAG_KIND_OF(type, ST_IN)) {
		ASSERT("Incorrect stream type. Wanted a stream type in class %s and got %s",
				STR(tagToString(ST_IN)), STR(tagToString(type)));
	}
	_pOutStreams = NULL;
	_canCallOutStreamDetached = true;
}

BaseInStream::~BaseInStream() {
	_canCallOutStreamDetached = false;
	while (_linkedStreams.size() > 0) {
		UnLink(MAP_VAL(_linkedStreams.begin()), true);
	}
}

vector<BaseOutStream *> BaseInStream::GetOutStreams() {
	vector<BaseOutStream *> result;
	LinkedListNode<BaseOutStream *> *pTemp = _pOutStreams;
	while (pTemp != NULL) {
		ADD_VECTOR_END(result, pTemp->info);
		pTemp = pTemp->pPrev;
	}
	return result;
}

bool BaseInStream::Link(BaseOutStream *pOutStream, bool reverseLink) {
	if ((!pOutStream->IsCompatibleWithType(GetType()))
			|| (!IsCompatibleWithType(pOutStream->GetType()))) {
		FATAL("stream type %s not compatible with stream type %s",
				STR(tagToString(GetType())),
				STR(tagToString(pOutStream->GetType())));
		return false;
	}
	//    FINEST("BaseInStream::Link: this: %u; pOutStream: %u; reverseLink: %d",
	//            GetUniqueId(), pOutStream->GetUniqueId(), reverseLink);
	if (MAP_HAS1(_linkedStreams, pOutStream->GetUniqueId())) {
		WARN("BaseInStream::Link: This stream is already linked");
		return true;
	}
	//FINEST("BaseInStream::Link: Linking...");
	_pOutStreams = AddLinkedList(_pOutStreams, pOutStream, true);
	_linkedStreams[pOutStream->GetUniqueId()] = pOutStream;

	if (reverseLink) {
		//FINEST("BaseInStream::Link: Reverse linking...");
		if (!pOutStream->Link(this, false)) {
			FATAL("BaseInStream::Link: Unable to reverse link");
			//TODO: here we must remove the link from _pOutStreams and _linkedStreams
			NYIA;
		}
		//FINEST("BaseInStream::Link: Done reverse link");
	}
	//FINEST("BaseInStream::Link: Signal OutStreamAttached");
	SignalOutStreamAttached(pOutStream);
	//FINEST("BaseInStream::Link: Done");
	return true;
}

bool BaseInStream::UnLink(BaseOutStream *pOutStream, bool reverseUnLink) {
	//    FINEST("BaseInStream::UnLink: this: %u; pOutStream: %u; reverseUnLink: %d",
	//            GetUniqueId(), pOutStream->GetUniqueId(), reverseUnLink);
	if (!MAP_HAS1(_linkedStreams, pOutStream->GetUniqueId())) {
		WARN("BaseInStream::UnLink: This stream is not linked");
		return true;
	}

	//FINEST("BaseInStream::UnLink: UnLinking...");
	_linkedStreams.erase(pOutStream->GetUniqueId());
	LinkedListNode<BaseOutStream *> *pTemp = _pOutStreams;
	while (pTemp != NULL) {
		if (pTemp->info->GetUniqueId() == pOutStream->GetUniqueId()) {
			_pOutStreams = RemoveLinkedList<BaseOutStream *>(pTemp);
			//FINEST("BaseInStream::UnLink: Stream unlinked from the list");
			break;
		}
		pTemp = pTemp->pPrev;
	}

	if (reverseUnLink) {
		//FINEST("BaseInStream::UnLink: Reverse unLinking...");
		if (!pOutStream->UnLink(false)) {
			FATAL("BaseInStream::UnLink: Unable to reverse unLink");
			//TODO: what are we going to do here???
			NYIA;
		}
		//FINEST("BaseInStream::UnLink: Done reverse unLink");
	}
	//    FINEST("BaseInStream::UnLink: _canCallOutStreamDetached: %d",
	//            _canCallOutStreamDetached);
	if (_canCallOutStreamDetached) {
		//FINEST("BaseInStream::UnLink: Signal OutStreamDetached");
		SignalOutStreamDetached(pOutStream);
	}
	//FINEST("BaseInStream::UnLink: Done");
	return true;
}

bool BaseInStream::Play(double absoluteTimestamp, double length) {
	if (!SignalPlay(absoluteTimestamp, length)) {
		FATAL("Unable to signal play");
		return false;
	}
	LinkedListNode<BaseOutStream *> *pTemp = _pOutStreams;
	while (pTemp != NULL) {
		if (!pTemp->info->SignalPlay(absoluteTimestamp, length)) {
			WARN("Unable to signal play on an outbound stream");
		}
		pTemp = pTemp->pPrev;
	}
	return true;
}

bool BaseInStream::Pause() {
	if (!SignalPause()) {
		FATAL("Unable to signal pause");
		return false;
	}
	LinkedListNode<BaseOutStream *> *pTemp = _pOutStreams;
	while (pTemp != NULL) {
		if (!pTemp->info->SignalPause()) {
			WARN("Unable to signal pause on an outbound stream");
		}
		pTemp = pTemp->pPrev;
	}
	return true;
}

bool BaseInStream::Resume() {
	if (!SignalResume()) {
		FATAL("Unable to signal resume");
		return false;
	}
	LinkedListNode<BaseOutStream *> *pTemp = _pOutStreams;
	while (pTemp != NULL) {
		if (!pTemp->info->SignalResume()) {
			WARN("Unable to signal resume on an outbound stream");
		}
		pTemp = pTemp->pPrev;
	}
	return true;
}

bool BaseInStream::Seek(double absoluteTimestamp) {
	LinkedListNode<BaseOutStream *> *pTemp = _pOutStreams;
	while (pTemp != NULL) {
		if (!pTemp->info->SignalSeek(absoluteTimestamp)) {
			WARN("Unable to signal seek on an outbound stream");
		}
		pTemp = pTemp->pPrev;
	}

	if (!SignalSeek(absoluteTimestamp)) {
		FATAL("Unable to signal seek");
		return false;
	}

	return true;
}

bool BaseInStream::Stop() {
	if (!SignalStop()) {
		FATAL("Unable to signal stop");
		return false;
	}
	LinkedListNode<BaseOutStream *> *pTemp = _pOutStreams;
	while (pTemp != NULL) {
		if (!pTemp->info->SignalStop()) {
			WARN("Unable to signal stop on an outbound stream");
		}
		pTemp = pTemp->pPrev;
	}
	return true;
}

