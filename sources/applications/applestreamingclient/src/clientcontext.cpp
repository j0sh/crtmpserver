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

#include "clientcontext.h"
#include "protocols/protocolfactory.h"
#include "protocols/protocolfactorymanager.h"
#include "netio/netio.h"
#include "application/clientapplicationmanager.h"
#include "playlist.h"
#include "protocols/protocolmanager.h"
#include "speedcomputer.h"
#include "eventsink/baseeventsink.h"
#include "applestreamingclient.h"
#include "protocols/timer/scheduletimerprotocol.h"
#include "protocols/ts/inboundtsprotocol.h"
#include "streaming/basestream.h"
#include "streaming/streamsmanager.h"
#include "protocols/ts/innettsstream.h"
#include "applestreamingclientapplication.h"
#include "protocols/timer/finetimer.h"
using namespace app_applestreamingclient;

uint32_t ClientContext::_idGenerator = 0;
map<uint32_t, ClientContext *> ClientContext::_contexts;

ClientContext::ClientContext() {
	_id = ++_idGenerator;
	_pEventSink = NULL;
	_pMasterPlaylist = NULL;
	_parsedChildPlaylistsCount = 0;
	_currentSequence = 0;
	_optimalBw = 0;
	_lastUsedBw = 0;
	_pSpeedComputer = NULL;
	_tsId = 0;
	_scheduleTimerId = 0;
	_maxAVBufferSize = 4 * 1024 * 1024;
	_streamName = "";
	_streamId = 0;
	_pStreamsManager = NULL;
	_lastWallClock = 0;
	_avData.EnsureSize(_maxAVBufferSize * 3);
	_firstFeedTime = 0;
}

ClientContext::~ClientContext() {
	if (_pMasterPlaylist != NULL) {
		delete _pMasterPlaylist;
		_pMasterPlaylist = NULL;
	}

	FOR_MAP(_childPlaylists, uint32_t, Playlist *, i) {
		delete MAP_VAL(i);
	}
	_childPlaylists.clear();

	BaseProtocol *pProtocol = ProtocolManager::GetProtocol(_tsId);
	if (pProtocol != NULL)
		pProtocol->EnqueueForDelete();

	pProtocol = ProtocolManager::GetProtocol(_scheduleTimerId);
	if (pProtocol != NULL)
		pProtocol->EnqueueForDelete();

	if (_pSpeedComputer != NULL) {
		delete _pSpeedComputer;
		_pSpeedComputer = NULL;
	}

	if (_pEventSink != NULL) {
		delete _pEventSink;
		_pEventSink = NULL;
	}
#ifdef HAS_MS_TIMER
	AppleStreamingClientApplication *pApp = GetApplication();
	if (pApp != NULL) {
		FineTimer *pFineTimer = pApp->GetFineTimer();
		if (pFineTimer != NULL) {
			pFineTimer->UnRegisterContextId(_id);
		}
	}
#endif /* HAS_MS_TIMER */
}

vector<uint32_t> ClientContext::GetContextIds() {
	vector<uint32_t> result;

	FOR_MAP(_contexts, uint32_t, ClientContext *, i) {
		ADD_VECTOR_END(result, MAP_KEY(i));
	}
	return result;
}

ClientContext *ClientContext::GetContext(uint32_t &contextId,
		uint32_t applicationId, uint64_t masterProtocolType) {
	ClientContext *pResult = NULL;
	if (contextId == 0) {
		pResult = new ClientContext();
		pResult->_applicationId = applicationId;
		pResult->_pEventSink = BaseEventSink::GetInstance(masterProtocolType,
				pResult->_id);
		contextId = pResult->_id;
		_contexts[pResult->_id] = pResult;
	} else {
		if (MAP_HAS1(_contexts, contextId))
			pResult = _contexts[contextId];
	}
	return pResult;
}

void ClientContext::ReleaseContext(uint32_t contextId) {
	if (MAP_HAS1(_contexts, contextId)) {
		delete _contexts[contextId];
		_contexts.erase(contextId);
	}
}

uint32_t ClientContext::Id() {
	return _id;
}

BaseEventSink * ClientContext::EventSink() {
	return _pEventSink;
}

string ClientContext::RawConnectingString() {
	return _rawConnectingString;
}

void ClientContext::RawConnectingString(string connectingString) {
	_rawConnectingString = connectingString;
}

ConnectingString &ClientContext::GetConnectingString() {
	return _connectingString;
}

Playlist *ClientContext::MasterPlaylist() {
	return _pMasterPlaylist;
}

Playlist *ClientContext::ChildPlaylist(uint32_t bw) {
	if (!MAP_HAS1(_childPlaylists, bw)) {
		FATAL("Playlist for bandwidth %u not found", bw);
		return NULL;
	}
	return _childPlaylists[bw];
}

vector<double> ClientContext::GetAvailableBandwidths() {
	vector<double> result;

	FOR_MAP(_childPlaylists, uint32_t, Playlist *, i) {
		ADD_VECTOR_END(result, MAP_KEY(i));
	}
	return result;
}

double ClientContext::GetDetectedBandwidth() {
	return (uint32_t) _pSpeedComputer->GetMeanSpeed()*8.0;
}

double ClientContext::GetSelectedBandwidth() {
	return _optimalBw;
}

uint32_t ClientContext::GetBufferLevel() {
	return GETAVAILABLEBYTESCOUNT(_avData);
}

uint32_t ClientContext::GetMaxBufferLevel() {
	return _maxAVBufferSize;
}

double ClientContext::GetBufferLevelPercent() {
	return ((double) GetBufferLevel() / (double) GetMaxBufferLevel())*100.00;
}

double ClientContext::GetMinTimestamp() {
	return 0;
}

double ClientContext::GetMaxTimestamp() {
	return 0;
}

uint32_t ClientContext::GetChunksCount() {
	return 0;
}

double ClientContext::GetCurrentTimestamp() {
	return 0;
}

uint32_t ClientContext::GetCurrentChunkIndex() {
	return 0;
}

AppleStreamingClientApplication *ClientContext::GetApplication() {
	BaseClientApplication *pApplication = ClientApplicationManager::FindAppById(
			_applicationId);
	return (AppleStreamingClientApplication *) pApplication;
}

void ClientContext::SetAllowedBitrates(map<uint32_t, uint32_t> allowedBitrates) {
	_allowedBitrates = allowedBitrates;
}

bool ClientContext::StartProcessing() {
	//1. Parse the connecting string and split it into usable pieces
	if (!ParseConnectingString()) {
		FATAL("Unable to parse connecting string");
		return false;
	}

	//2. Create the master playlist
	_pMasterPlaylist = new Playlist();

	//3. Create the speed computer
	_pSpeedComputer = new SpeedComputer();

	//4. Create the schedule
	ScheduleTimerProtocol *pScheduleTimer = new ScheduleTimerProtocol(_id);
	_scheduleTimerId = pScheduleTimer->GetId();
	pScheduleTimer->EnqueueForTimeEvent(1);

	//5. Add the recurring job for consuming the A/V data buffer
#ifdef HAS_MS_TIMER
	AppleStreamingClientApplication *pApp = GetApplication();
	if (pApp != NULL) {
		FineTimer *pFineTimer = pApp->GetFineTimer();
		if (pFineTimer != NULL) {
			pFineTimer->RegisterContextId(_id);
		} else {
			ASSERT("Unable to find fine timer");
		}
	} else {
		ASSERT("Unable to find application");
	}
#else
	Variant job;
	job["type"] = "consumeAVBuffer";
	pScheduleTimer->AddJob(job, true);
#endif /* HAS_MS_TIMER */

	//6. Start the master M3U8 fetching
	return FetchMasterPlaylist();
}

bool ClientContext::StartFeeding() {
	if (GETAVAILABLEBYTESCOUNT(_avData) > _maxAVBufferSize) {
		return EnqueueStartFeeding();
	}
	//1. Wait for all playlists
	if (_parsedChildPlaylistsCount < _childPlaylists.size()) {
		return true;
	}

	//2. Get the optimal bandwidth
	uint32_t optimalBw = GetOptimalBw();

	//3. Get the corresponding playlist
	Playlist *pPlaylist = _childPlaylists[optimalBw];

	//4. Is this the last item in the playlis?
	string uri = pPlaylist->GetItemUri(_currentSequence);
	if (uri == "") {
		FINEST("---------------------------------------------------");
		WARN("End of list. Wait one sec and try again");
		FINEST("---------------------------------------------------");
		return EnqueueFetchChildPlaylist(_childPlaylists[optimalBw]->GetPlaylistUri(), optimalBw);
	}

	//4. Get the item URI and the key URI if available
	string keyUri = pPlaylist->GetItemKeyUri(_currentSequence);
	if (keyUri != "")
		keyUri += "&" + _connectingString.sessionId;

	if (keyUri != "") {
		//5. Encrypted stream. fetch the key first
		return FetchKey(keyUri, uri, optimalBw);
	} else {
		//6. not encrypted
		return FetchTS(uri, optimalBw, "", 0);
	}
}

bool ClientContext::FetchChildPlaylist(string uri, uint32_t bw) {
	Variant customParameters;
	customParameters["protocolChain"] = PC_CHILD_PLAYLIST;
	customParameters["bw"] = bw;
	return FetchURI(uri, "childPlaylist", customParameters);
}

bool ClientContext::ConsumeAVBuffer() {
	//1. initialize _lastFeedTime
	if (_lastWallClock == 0) {
#ifdef HAS_MS_TIMER
		GETCLOCKS(_lastWallClock);
#else
		_lastWallClock = time(NULL)*1000.0;
#endif /* HAS_MS_TIMER */
		return true;
	}

	//2. Get the TS protocol
	InboundTSProtocol *pTS = (InboundTSProtocol *) ProtocolManager::GetProtocol(_tsId);
	if (pTS == NULL) {
		WARN("No TS protocol");
		return true;
	}

	//3. First, feed up until the stream name is registered
	if (_streamId == 0) {
		while ((_streamId == 0) && (GETAVAILABLEBYTESCOUNT(_avData) > 8192)) {
			if (!pTS->SignalInputData(_avData)) {
				FATAL("Unable to feed TS protocol");
				return false;
			}
		}
	}


	//4. Get the inbound TS stream
	if (_pStreamsManager == NULL) {
		WARN("No stream manager yet");
		return true;
	}
	InNetTSStream *pStream = (InNetTSStream *) _pStreamsManager->FindByUniqueId(
			_streamId);
	if (pStream == NULL) {
		FATAL("Unable to get the inbound stream");
		return false;
	}

	//5. Continue feeding until we have stream capabilities
	if ((pStream->GetCapabilities()->videoCodecId != CODEC_VIDEO_AVC)
			|| (pStream->GetCapabilities()->audioCodecId != CODEC_AUDIO_AAC)) {
		while (GETAVAILABLEBYTESCOUNT(_avData) > 8192) {
			if ((pStream->GetCapabilities()->videoCodecId == CODEC_VIDEO_AVC)
					&& (pStream->GetCapabilities()->audioCodecId == CODEC_AUDIO_AAC)) {
				_pEventSink->SignalStreamRegistered(_streamName);
				break;
			}
			if (!pTS->SignalInputData(_avData)) {
				FATAL("Unable to feed TS protocol");
				return false;
			}
		}
		if ((pStream->GetCapabilities()->videoCodecId != CODEC_VIDEO_AVC)
				|| (pStream->GetCapabilities()->audioCodecId != CODEC_AUDIO_AAC)) {
			FINEST("SPS/PPS not yet available");
			return true;
		}
	}

	//5. Does it have any registered consumers?
	if (pStream->GetOutStreams().size() <= 0) {
		WARN("No registered consumers or stream capabilities not yet known. Take a break...");
		return true;
	}

	//5. How much seconds do we have to feed?
#ifdef HAS_MS_TIMER
	double currentTime;
	GETCLOCKS(currentTime);
	double wallClockDelta = ((currentTime - _lastWallClock) / (double) CLOCKS_PER_SECOND)*1000.00;
#else
	double wallClockDelta = time(NULL)*1000.00 - _lastWallClock;
#endif /* HAS_MS_TIMER */

	if (_firstFeedTime == 0) {
		_firstFeedTime = pStream->GetFeedTime();
	}

	//6. Feed
	while ((wallClockDelta + 1000 > (pStream->GetFeedTime() - _firstFeedTime)) &&
			(GETAVAILABLEBYTESCOUNT(_avData) > 8192)) {
		if (!pTS->SignalInputData(_avData)) {
			FATAL("Unable to feed TS protocol");
			return false;
		}
	}

	//7. Done
	return true;
}

uint32_t ClientContext::GetOptimalBw() {
	if (_optimalBw == 0) {
		_optimalBw = MAP_KEY(_childPlaylists.begin());
	}
	return _optimalBw;
}

bool ClientContext::ParseConnectingString() {
	vector<string> parts;
	split(_rawConnectingString, "|", parts);
	if ((parts.size() != 3) && (parts.size() != 1)) {
		FATAL("Invalid master m3u8 URL: %s", STR(_rawConnectingString));
		return false;
	}

	if (parts[0] == "") {
		FATAL("Invalid master m3u8 URL: %s", STR(_rawConnectingString));
		return false;
	}

	_connectingString.masterM3U8Url = parts[0];

	if (parts.size() == 3) {
		if ((parts[1] == "") || (parts[2] == "")) {
			FATAL("Invalid master m3u8 URL: %s", STR(_rawConnectingString));
			return false;
		}
		_connectingString.keyPassword = parts[1];
		_connectingString.sessionId = parts[2];
	} else {
		_connectingString.keyPassword = "";
		_connectingString.sessionId = "";
	}

	FINEST("_connectingString:\n%s", STR(_connectingString));

	return true;
}

bool ClientContext::SignalProtocolCreated(BaseProtocol *pProtocol, Variant &parameters) {
	if (pProtocol == NULL) {
		FATAL("Connection failed:\n%s", STR(parameters.ToString()));
		return false;
	}

	//1. Get the context
	uint32_t contextId = parameters["contextId"];
	assert(contextId != 0);
	ClientContext *pContext = GetContext(contextId, 0, 0);
	if (pContext == NULL) {
		FATAL("Unable to get the context");
		return false;
	}



	//2. Get the application
	BaseClientApplication *pApp = ClientApplicationManager::FindAppById(
			parameters["applicationId"]);
	if (pApp == NULL) {
		FATAL("Application id %u not found", (uint32_t) parameters["applicationId"]);
		return false;
	}

	//3. Set it up inside the protocol
	pProtocol->SetApplication(pApp);

	//4. Done
	return true;
}

bool ClientContext::SignalMasterPlaylistAvailable() {
	//1. for each entry in the playlist, fire up the fetcher
	for (uint32_t i = 0; i < _pMasterPlaylist->GetItemsCount(); i++) {
		//2. Get the bandwidth and the URI
		uint32_t bw = _pMasterPlaylist->GetItemBandwidth(i);
		if (bw < 10000)
			bw *= 1024;
		if (_allowedBitrates.size() > 0) {
			if (!MAP_HAS1(_allowedBitrates, bw)) {
				WARN("Skipping bitrate %u", bw);
				continue;
			}
		}
		string uri = _pMasterPlaylist->GetItemUri(i);

		if (MAP_HAS1(_childPlaylists, bw)) {
			FATAL("Duplicate bandwidth detected: %u", bw);
			return false;
		}

		//3. Create the corresponding playlist
		Playlist *pChildPlaylist = new Playlist();
		_childPlaylists[bw] = pChildPlaylist;

		//4. Fire up the fetch process
		if (!FetchChildPlaylist(uri, bw)) {
			FATAL("Unable to fetch child playlist");
			return false;
		}
	}

	//5. Done
	return true;
}

bool ClientContext::SignalChildPlaylistAvailable(uint32_t bw) {
	_parsedChildPlaylistsCount++;
	return StartFeeding();
}

bool ClientContext::SignalChildPlaylistNotAvailable(uint32_t bw) {
	if (!MAP_HAS1(_childPlaylists, bw))
		return true;
	Playlist *pPl = _childPlaylists[bw];
	delete pPl;
	_childPlaylists.erase(bw);
	WARN("bw %u removed", bw);
	return StartFeeding();
}

bool ClientContext::SignalAESKeyAvailable(Variant &parameters) {
	//1. Get the item uri, bw and the key
	string key = parameters["payload"]["key"];
	string itemUri = parameters["payload"]["itemUri"];
	uint32_t bw = parameters["payload"]["bw"];
	uint64_t iv = _currentSequence;

	return FetchTS(itemUri, bw, key, iv);
}

bool ClientContext::SignalTSProtocolAvailable(uint32_t protocolId, uint32_t bw) {
	if (_tsId != 0) {
		FATAL("TS protocol already present");
		return false;
	}
	_tsId = protocolId;
	return true;
}

bool ClientContext::SignalTSChunkComplete(uint32_t bw) {
	_currentSequence++;
	return StartFeeding();
}

bool ClientContext::SignalSpeedDetected(double instantAmount, double instantTime) {
	return true;
}

bool ClientContext::SignalAVDataAvailable(IOBuffer &buffer) {
	_avData.ReadFromBuffer(GETIBPOINTER(buffer), GETAVAILABLEBYTESCOUNT(buffer));
	buffer.IgnoreAll();
	return true;
}

bool ClientContext::SignalStreamRegistered(BaseStream *pStream) {
	if (_pEventSink != NULL) {
		_streamName = pStream->GetName();
		_streamId = pStream->GetUniqueId();
		_pStreamsManager = pStream->GetStreamsManager();
		return true;
		//return _pEventSink->SignalStreamRegistered(pStream->GetName());
	} else {
		_streamName = "";
		_streamId = 0;
		_pStreamsManager = NULL;
		FATAL("No event sync available");
		return false;
	}
}

bool ClientContext::SignalStreamUnRegistered(BaseStream *pStream) {
	_streamName = "";
	_streamId = 0;
	_pStreamsManager = NULL;
	if (_pEventSink != NULL) {
		return _pEventSink->SignalStreamUnRegistered(pStream->GetName());
	} else {
		FATAL("No event sync available");
		return false;
	}
}

bool ClientContext::FetchMasterPlaylist() {
	Variant customParameters;
	customParameters["protocolChain"] = PC_MASTER_PLAYLIST;
	return FetchURI(_connectingString.masterM3U8Url,
			"masterPlaylist", customParameters);
}

bool ClientContext::FetchKey(string keyUri, string itemUri, uint32_t bw) {
	Variant customParameters;
	customParameters["protocolChain"] = PC_ITEM_KEY;
	customParameters["itemUri"] = itemUri;
	customParameters["bw"] = bw;
	return FetchURI(keyUri, "key", customParameters);
}

bool ClientContext::FetchTS(string uri, uint32_t bw, string key, uint64_t iv) {
	//1. Prepare custom parameters
	Variant customParameters;
	if (key == "") {
		if (_tsId > 0) {
			customParameters["protocolChain"] = PC_ITEM;
			customParameters["tsId"] = _tsId;
		} else {
			customParameters["protocolChain"] = PC_ITEM_TS;
		}
	} else {
		if (_tsId > 0) {
			customParameters["protocolChain"] = PC_ITEM_ENC;
			customParameters["tsId"] = _tsId;
		} else {
			customParameters["protocolChain"] = PC_ITEM_ENC_TS;
		}
	}
	customParameters["key"] = key;
	customParameters["iv"] = iv;
	customParameters["bw"] = bw;

	//2. setup last bw used
	if (_lastUsedBw == 0)
		_lastUsedBw = bw;

	if (_lastUsedBw != bw) {
		if (_pEventSink->GetType() == EVENT_SYNC_VARIANT) {
			_avData.IgnoreAll();
			InNetTSStream *pStream = (InNetTSStream *) _pStreamsManager->FindByUniqueId(
					_streamId);
			if (pStream == NULL) {
				FATAL("Unable to get the inbound stream");
				return false;
			}
			StreamCapabilities *pSC = pStream->GetCapabilities();
			pSC->Clear();
			_pEventSink->SignalStreamUnRegistered(_streamName);
		}
		if (_lastUsedBw < bw) {
			_pEventSink->SignalUpgradeBandwidth(_lastUsedBw, bw);
		} else {
			_pEventSink->SignalDowngradeBandwidth(_lastUsedBw, bw);
		}
	}

	_lastUsedBw = bw;


	return FetchURI(uri, "ts", customParameters);
}

bool ClientContext::FetchURI(string uriString, string requestType, Variant &customParameters) {
	INFO("Fetch: %s", STR(uriString));
	customParameters["httpRequestType"] = requestType;
	customParameters["contextId"] = _id;

	//1. Get the protocol chain
	vector<uint64_t> protocolStackTypes =
			ProtocolFactoryManager::ResolveProtocolChain(customParameters["protocolChain"]);
	if (protocolStackTypes.size() == 0) {
		ASSERT("Unable to resolve protocol stack %s", STR(customParameters["protocolChain"]));
		return false;
	}

	//2. Parse the URI and extract all needed data
	URI uri;
	if (!URI::FromString(uriString, true, uri)) {
		FATAL("Invalid uri: %s", STR(uriString));
		return false;
	}

	if (uri.scheme() == "https") {

		FOR_VECTOR_ITERATOR(uint64_t, protocolStackTypes, i) {
			if (VECTOR_VAL(i) == PT_INBOUND_HTTP) {
				protocolStackTypes.insert(i, PT_INBOUND_SSL);
				break;
			} else if (VECTOR_VAL(i) == PT_OUTBOUND_HTTP) {
				protocolStackTypes.insert(i, PT_OUTBOUND_SSL);
				break;
			}
		}
	}

	//3. Prepare the HTTP info
	Variant parameters;
	parameters["fullUri"] = uriString;
	parameters["document"] = uri.fullDocumentPath();
	parameters["host"] = uri.host();
	parameters["applicationId"] = _applicationId;
	parameters["contextId"] = _id;
	parameters["payload"] = customParameters;

	//4. Start the connection process
	if (!TCPConnector<ClientContext>::Connect(uri.ip(), uri.port(), protocolStackTypes, parameters)) {
		FATAL("Unable to open connection to origin");
		return false;
	}

	return true;
}

bool ClientContext::EnqueueStartFeeding() {
	ScheduleTimerProtocol *pSchedule = (ScheduleTimerProtocol *) ProtocolManager::GetProtocol(_scheduleTimerId);
	if (pSchedule == NULL) {
		FATAL("Unable to obtain job scheduler");
		return false;
	}

	Variant job;
	job["type"] = "startFeeding";

	pSchedule->AddJob(job, false);

	return true;
}

bool ClientContext::EnqueueFetchChildPlaylist(string uri, uint32_t bw) {
	ScheduleTimerProtocol *pSchedule = (ScheduleTimerProtocol *) ProtocolManager::GetProtocol(_scheduleTimerId);
	if (pSchedule == NULL) {
		FATAL("Unable to obtain job scheduler");
		return false;
	}

	Variant job;
	job["type"] = "fetchChildPlaylist";
	job["uri"] = uri;
	job["bw"] = bw;

	pSchedule->AddJob(job, false);

	return true;
}
