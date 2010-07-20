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

uint32_t ClientContext::_idGenerator = 0;
map<uint32_t, ClientContext *> ClientContext::_contexts;

ClientContext::ClientContext() {
	_id = ++_idGenerator;
	_pEventSink = NULL;
	_pMasterPlaylist = NULL;
	_parsedChildPlaylistsCount = 0;
	_currentItemIndex = 0;
	_optimalBw = 0;
	_pSpeedComputer = NULL;
	_tsId = 0;
	INFO("Context created: %d (%p)", _id, this);
}

ClientContext::~ClientContext() {
	INFO("Context destroyed: %d (%p)", _id, this);
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

	if (_pSpeedComputer != NULL) {
		delete _pSpeedComputer;
		_pSpeedComputer = NULL;
	}

	if (_pEventSink != NULL) {
		delete _pEventSink;
		_pEventSink = NULL;
	}
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
		pResult->_pEventSink = BaseEventSink::GetInstance(masterProtocolType);
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
		FATAL("Playlist for bandwidth %d not found", bw);
		return NULL;
	}
	return _childPlaylists[bw];
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

	//4. Start the master M3U8 fetching
	return FetchMasterPlaylist();
}

uint32_t ClientContext::GetOptimalBw() {
	if (_optimalBw == 0) {
		_optimalBw = MAP_KEY(_childPlaylists.begin());
	}
	return _optimalBw;
}

bool ClientContext::StartFeeding() {
	//1. Wait for all playlists
	if (_parsedChildPlaylistsCount < _childPlaylists.size()) {
		FINEST("Waiting for the rest of the playlists. Got: %d; Wanted: %d",
				_parsedChildPlaylistsCount, _childPlaylists.size());
		return true;
	}

	//2. Get the optimal bandwidth
	uint32_t optimalBw = GetOptimalBw();
	FINEST("optimalBw: %d", optimalBw);

	//3. Get the corresponding playlist
	Playlist *pPlaylist = _childPlaylists[optimalBw];

	if (_currentItemIndex == 0)
		_currentItemIndex = pPlaylist->GetItemsCount() / 2 + 10;

	//4. Is this the last item in the playlis?
	if (_currentItemIndex >= pPlaylist->GetItemsCount()) {
		FINEST("End of list. Wait one sec and try again");
		sleep(1);
		return FetchChildPlaylist(_childPlaylists[optimalBw]->GetPlaylistUri(), optimalBw);
	}

	//4. Get the item URI and the key URI if available
	string uri = pPlaylist->GetItemUri(_currentItemIndex);
	string keyUri = pPlaylist->GetItemKeyUri(_currentItemIndex);
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

bool ClientContext::ParseConnectingString() {
	vector<string> parts;
	split(_rawConnectingString, "|", parts);
	if ((parts.size() != 3) && (parts.size() != 1)) {
		FATAL("Invalid master m3u8 URL: %s", STR(_rawConnectingString));
		return false;
	}
	//	for (uint32_t i = 0; i < parts.size(); i++) {
	//		FINEST("%d: %s", i, STR(parts[i]));
	//	}

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
	//1. Get the context
	FINEST("PC: %s", STR(*pProtocol));
	uint32_t contextId = parameters["contextId"];
	assert(contextId != 0);
	ClientContext *pContext = GetContext(contextId, 0, 0);
	if (pContext == NULL) {
		FATAL("Unable to get the context");
		sleep(5);
		return false;
	}



	//2. Get the application
	BaseClientApplication *pApp = ClientApplicationManager::FindAppById(
			parameters["applicationId"]);
	if (pApp == NULL) {
		FATAL("Application id %d not found", (uint32_t) parameters["applicationId"]);
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
		string uri = _pMasterPlaylist->GetItemUri(i);

		if (MAP_HAS1(_childPlaylists, bw)) {
			FATAL("Duplicate bandwidth detected: %d", bw);
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

bool ClientContext::SignalAESKeyAvailable(Variant &parameters) {
	//1. Get the item uri, bw and the key
	string key = parameters["payload"]["key"];
	string itemUri = parameters["payload"]["itemUri"];
	uint32_t bw = parameters["payload"]["bw"];
	uint64_t iv = _childPlaylists[bw]->GetItemMediaSequence(_currentItemIndex);
	//FINEST("itemUri: %s; bw: %d; key: %s", STR(itemUri), bw, STR(key));

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
	_currentItemIndex++;
	return StartFeeding();
}

static int aaa = 0;

bool ClientContext::SignalSpeedDetected(double instantAmount, double instantTime) {
	//	FINEST("instantAmount: %.2f; instantTime: %.8f; Speed: %.2f KB/s",
	//			instantAmount, instantTime, instantAmount / instantTime / 1024);
	_pSpeedComputer->PushAmount(instantAmount, instantTime);
	double meanSpeed = _pSpeedComputer->GetMeanSpeed();

	uint32_t before = _optimalBw;
	meanSpeed *= 8.0;
	if (((aaa++) % 200) == 0) {
		double ms = meanSpeed / 1024.00 / 8;
		string um = "KB/s";
		if (ms > 1024) {
			ms = ms / 1024.00;
			um = "MB/s";
		}
		FINEST("Speed: %.2f %s", ms, STR(um));
	}

	_optimalBw = MAP_KEY(_childPlaylists.begin());

	FOR_MAP(_childPlaylists, uint32_t, Playlist *, i) {
		uint32_t testBandwidth = MAP_KEY(i);
		//FINEST("meanSpeed: %.2f; testBandwidth: %.2f", meanSpeed, testBandwidth);
		if (meanSpeed > testBandwidth) {
			_optimalBw = testBandwidth;
		}
	}
	if (before != _optimalBw) {
		INFO("BW changed: before: %d; after: %d; speed: %.3f",
				before, _optimalBw, meanSpeed);
		//, STR(Bandwidths().ToString()))
	}

	return true;
}

bool ClientContext::FetchMasterPlaylist() {
	Variant customParameters;
	customParameters["protocolChain"] = PC_MASTER_PLAYLIST;
	return FetchURI(_connectingString.masterM3U8Url,
			"masterPlaylist", customParameters);
}

bool ClientContext::FetchChildPlaylist(string uri, uint32_t bw) {
	Variant customParameters;
	customParameters["protocolChain"] = PC_CHILD_PLAYLIST;
	customParameters["bw"] = bw;
	return FetchURI(uri, "childPlaylist", customParameters);
}

bool ClientContext::FetchKey(string keyUri, string itemUri, uint32_t bw) {
	Variant customParameters;
	customParameters["protocolChain"] = PC_ITEM_KEY;
	customParameters["itemUri"] = itemUri;
	customParameters["bw"] = bw;
	return FetchURI(keyUri, "key", customParameters);
}

bool ClientContext::FetchTS(string uri, uint32_t bw, string key, uint64_t iv) {
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
	return FetchURI(uri, "ts", customParameters);
}

bool ClientContext::FetchURI(string uri, string requestType, Variant &customParameters) {
	INFO("Fetch: %s", STR(uri));
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
	string host;
	string ip;
	uint16_t port;
	string document;
	string dummy;

	if (!ParseURL(uri, host, port, dummy, dummy, document)) {
		FATAL("Invalid uri: %s", STR(uri));
		return false;
	}

	ip = GetHostByName(host);
	if (ip == "") {
		FATAL("Unable to resolve host `%s`", STR(host));
		return false;
	}

	if (document == "") {
		document = "/";
	}

	//3. Prepare the HTTP info
	Variant parameters;
	parameters["fullUri"] = uri;
	parameters["document"] = document;
	parameters["host"] = host;
	parameters["applicationId"] = _applicationId;
	parameters["contextId"] = _id;
	parameters["payload"] = customParameters;

	//4. Start the connection process
	if (!TCPConnector<ClientContext>::Connect(ip, port, protocolStackTypes, parameters)) {
		FATAL("Unable to open connection to origin");
		return false;
	}

	return true;
}
