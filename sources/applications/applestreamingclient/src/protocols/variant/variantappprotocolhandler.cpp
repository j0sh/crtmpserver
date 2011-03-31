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

#include "protocols/variant/variantappprotocolhandler.h"
#include "protocols/variant/basevariantprotocol.h"
#include "protocols/variant/messagestructure.h"
#include "clientcontext.h"
#include "application/baseclientapplication.h"
#include "eventsink/varianteventsink.h"
#include "applestreamingclientapplication.h"
using namespace app_applestreamingclient;

#define GET_CONTEXT(pProtocol, request) \
uint32_t contextId = ASC_REQ_CONTEXT_ID(request); \
if (contextId == 0) {\
	ASC_RES_BUILD_CONTEXT_NOT_FOUND(request); \
	return; \
} \
ClientContext *pContext = GetContext(contextId, pProtocol->GetType()); \
if (pContext == NULL) { \
	ASC_RES_BUILD_CONTEXT_NOT_FOUND(request); \
	return; \
}

VariantAppProtocolHandler::VariantAppProtocolHandler(Variant &configuration)
: BaseVariantAppProtocolHandler(configuration) {

}

VariantAppProtocolHandler::~VariantAppProtocolHandler() {
}

ClientContext * VariantAppProtocolHandler::GetContext(uint32_t contextId,
		uint64_t protocolType) {
	ClientContext *pContext = ClientContext::GetContext(contextId,
			GetApplication()->GetId(), protocolType);
	if (pContext == NULL) {
		FATAL("Unable to get context");
		return NULL;
	}
	return pContext;
}

bool VariantAppProtocolHandler::ProcessMessage(BaseVariantProtocol *pProtocol,
		Variant &lastSent, Variant &lastReceived) {
	string type = ASC_REQ_TYPE(lastReceived);
	if (type == ASC_REQ_TYPE_CONTEXT_CREATE) {
		ProcessContextCreate(pProtocol, lastReceived);
	} else if (type == ASC_REQ_TYPE_CONTEXT_LIST) {
		ProcessContextList(pProtocol, lastReceived);
	} else if (type == ASC_REQ_TYPE_CONTEXT_CLOSE) {
		ProcessContextClose(pProtocol, lastReceived);
	} else if (type == ASC_REQ_TYPE_CONTEXT_CLOSE_ALL) {
		ProcessContextCloseAll(pProtocol, lastReceived);
	} else if (type == ASC_REQ_TYPE_COMMAND_PLAY) {
		ProcessCommandPlay(pProtocol, lastReceived);
	} else if (type == ASC_REQ_TYPE_COMMAND_SET_BITRATES) {
		ProcessCommandSetBitrates(pProtocol, lastReceived);
	} else if (type == ASC_REQ_TYPE_COMMAND_PAUSE) {
		ProcessCommandPause(pProtocol, lastReceived);
	} else if (type == ASC_REQ_TYPE_COMMAND_RESUME) {
		ProcessCommandResume(pProtocol, lastReceived);
	} else if (type == ASC_REQ_TYPE_INFO_LIST_STREAMS) {
		ProcessInfoListStreams(pProtocol, lastReceived);
	} else if (type == ASC_REQ_TYPE_INFO_LIST_ALL_STREAMS) {
		ProcessInfoListAllStreams(pProtocol, lastReceived);
	} else if (type == ASC_REQ_TYPE_INFO_BANDWIDTH) {
		ProcessInfoBandwidth(pProtocol, lastReceived);
	} else if (type == ASC_REQ_TYPE_INFO_PLAYBACK) {
		ProcessInfoPlayback(pProtocol, lastReceived);
	} else {
		WARN("Processing type `%s` not yet implemented", STR(type));
		ASC_RES_BUILD_UNKNOWN_REQUEST_TYPE(lastReceived);
	}
	return pProtocol->Send(lastReceived);
}

void VariantAppProtocolHandler::ProcessContextCreate(
		BaseVariantProtocol *pProtocol, Variant &request) {
	ClientContext *pContext = GetContext(0, pProtocol->GetType());
	if (pContext == NULL) {
		ASC_RES_BUILD_CONTEXT_CREATE_FAILED(request);
		return;
	} else {
		ASC_RES_BUILD_OK_CONTEXT_CREATE(request, pContext->Id());
	}
}

void VariantAppProtocolHandler::ProcessContextList(BaseVariantProtocol *pProtocol,
		Variant &request) {
	vector<uint32_t> contextIds = ClientContext::GetContextIds();
	ASC_RES_BUILD_OK_CONTEXT_LIST(request, contextIds);
}

void VariantAppProtocolHandler::ProcessContextClose(BaseVariantProtocol *pProtocol,
		Variant &request) {
	GET_CONTEXT(pProtocol, request);
	ClientContext::ReleaseContext(contextId);
	Variant tempVariant;
	ASC_RES_BUILD_OK(request, tempVariant);
}

void VariantAppProtocolHandler::ProcessContextCloseAll(
		BaseVariantProtocol *pProtocol, Variant &request) {
	((AppleStreamingClientApplication *) GetApplication())->CloseAllContexts();
	Variant tempVariant;
	ASC_RES_BUILD_OK(request, tempVariant);
}

void VariantAppProtocolHandler::ProcessCommandSetBitrates(
		BaseVariantProtocol *pProtocol, Variant &request) {
	GET_CONTEXT(pProtocol, request);
	map<uint32_t, uint32_t> bws;

	FOR_MAP(ASC_REQ_COMMAND_SET_BITRATES_BWS(request), string, Variant, i) {
		FINEST("(uint32_t)MAP_VAL(i): %u", (uint32_t) MAP_VAL(i));
		bws[(uint32_t) MAP_VAL(i)] = (uint32_t) MAP_VAL(i);
	}
	pContext->SetAllowedBitrates(bws);
	Variant tempVariant;
	ASC_RES_BUILD_OK(request, tempVariant);
}

void VariantAppProtocolHandler::ProcessCommandPlay(
		BaseVariantProtocol *pProtocol, Variant &request) {
	GET_CONTEXT(pProtocol, request);
	string connectingString = ASC_REQ_COMMAND_PLAY_URI(request);
	if (ASC_REQ_COMMAND_PLAY_PASSWORD(request) != "") {
		connectingString += "|" + (string) ASC_REQ_COMMAND_PLAY_PASSWORD(request);
		connectingString += "|" + (string) ASC_REQ_COMMAND_PLAY_SESSION_ID(request);
	}

	pContext->RawConnectingString(connectingString);

	if (!pContext->StartProcessing()) {
		ASC_RES_BUILD_COMMAND_PLAY_FAILED(request);
		return;
	}
	Variant tempVariant;
	ASC_RES_BUILD_OK(request, tempVariant);
}

void VariantAppProtocolHandler::ProcessCommandPause(
		BaseVariantProtocol *pProtocol, Variant &request) {
	ASC_RES_BUILD_NYI(request);
}

void VariantAppProtocolHandler::ProcessCommandResume(
		BaseVariantProtocol *pProtocol, Variant &request) {
	ASC_RES_BUILD_NYI(request);
}

void VariantAppProtocolHandler::ProcessInfoListStreams(
		BaseVariantProtocol *pProtocol, Variant &request) {
	GET_CONTEXT(pProtocol, request);
	if (pContext->EventSink()->GetType() == EVENT_SYNC_VARIANT) {
		VariantEventSink *pSink = (VariantEventSink *) pContext->EventSink();
		vector<string> streams = pSink->GetStreamNames();
		ASC_RES_BUILD_OK_INFO_LIST_STREAMS(request, streams);
	} else {
		ASC_RES_BUILD_CONTEXT_NOT_FOUND(request);
	}
}

void VariantAppProtocolHandler::ProcessInfoListAllStreams(
		BaseVariantProtocol *pProtocol, Variant &request) {
	vector<uint32_t> contextIds = ClientContext::GetContextIds();
	vector<string> allStreams;
	for (uint32_t i = 0; i < contextIds.size(); i++) {
		FINEST("Inspecting context id: %u", contextIds[i]);
		ClientContext *pContext = GetContext(contextIds[i],
				pProtocol->GetType());
		if (pContext == NULL) {
			WARN("Context id %u is NULL", contextIds[i]);
			continue;
		}
		if (pContext->EventSink()->GetType() != EVENT_SYNC_VARIANT) {
			WARN("Context id %u is not Variant friendly", contextIds[i]);
			continue;
		}
		VariantEventSink *pSink = (VariantEventSink *) pContext->EventSink();
		vector<string> streams = pSink->GetStreamNames();
		for (uint32_t j = 0; j < streams.size(); j++) {
			FINEST("Adding stream %s", STR(streams[j]));
			ADD_VECTOR_END(allStreams, streams[j]);
		}
	}
	ASC_RES_BUILD_OK_INFO_LIST_ALL_STREAMS(request, allStreams);
}

void VariantAppProtocolHandler::ProcessInfoBandwidth(
		BaseVariantProtocol *pProtocol, Variant &request) {
	GET_CONTEXT(pProtocol, request);
	ASC_RES_BUILD_OK_INFO_BANDWIDTH(request,
			pContext->GetAvailableBandwidths(),
			pContext->GetDetectedBandwidth(),
			pContext->GetSelectedBandwidth(),
			pContext->GetBufferLevel(),
			pContext->GetMaxBufferLevel(),
			pContext->GetBufferLevelPercent());
}

void VariantAppProtocolHandler::ProcessInfoPlayback(
		BaseVariantProtocol *pProtocol, Variant &request) {
	GET_CONTEXT(pProtocol, request);
	ASC_RES_BUILD_OK_INFO_PLAYBACK(request,
			pContext->GetMinTimestamp(),
			pContext->GetMaxTimestamp(),
			pContext->GetChunksCount(),
			pContext->GetCurrentTimestamp(),
			pContext->GetCurrentChunkIndex());
}
