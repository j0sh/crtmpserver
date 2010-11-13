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


#ifndef _CLIENTCONTEXT_H
#define	_CLIENTCONTEXT_H

#include "protocols/m3u8/basem3u8protocol.h"
#include "playlist.h"

class BaseProtocol;
class BaseStream;
class StreamsManager;

namespace app_applestreamingclient {
	class BaseEventSink;
	class Playlist;
	class SpeedComputer;
	class AppleStreamingClientApplication;

	typedef struct _ConnectingString {
		string masterM3U8Url;
		string keyPassword;
		string sessionId;

		operator string() {
			return format("masterM3U8Url: %s\nkeyPassword: %s\nsessionId: %s",
					STR(masterM3U8Url),
					STR(keyPassword),
					STR(sessionId));
		}
	} ConnectingString;

	class ClientContext {
	private:
		static uint32_t _idGenerator;
		static map<uint32_t, ClientContext *> _contexts;
		uint32_t _id;
		BaseEventSink *_pEventSink;
		string _rawConnectingString;
		ConnectingString _connectingString;
		uint32_t _applicationId;
		Playlist *_pMasterPlaylist;
		map<uint32_t, Playlist *> _childPlaylists;
		uint32_t _tsId;
		uint32_t _parsedChildPlaylistsCount;
		uint32_t _currentSequence;
		uint32_t _optimalBw;
		uint32_t _lastUsedBw;
		SpeedComputer *_pSpeedComputer;
		uint32_t _scheduleTimerId;
		IOBuffer _avData;
		uint32_t _maxAVBufferSize;
		string _streamName;
		uint32_t _streamId;
		StreamsManager *_pStreamsManager;
		double _lastWallClock;
		double _firstFeedTime;
		map<uint32_t, uint32_t> _allowedBitrates;
	private:
		ClientContext();
	public:
		virtual ~ClientContext();

		//Context spawn/tear down
		static vector<uint32_t> GetContextIds();
		static ClientContext *GetContext(uint32_t &contextId, uint32_t applicationId,
				uint64_t masterProtocolType);
		static void ReleaseContext(uint32_t contextId);

		//getters/setters
		uint32_t Id();
		BaseEventSink * EventSink();
		string RawConnectingString();
		void RawConnectingString(string connectingString);
		ConnectingString &GetConnectingString();
		Playlist *MasterPlaylist();
		Playlist *ChildPlaylist(uint32_t bw);
		vector<double> GetAvailableBandwidths();
		double GetDetectedBandwidth();
		double GetSelectedBandwidth();
		uint32_t GetBufferLevel();
		uint32_t GetMaxBufferLevel();
		double GetBufferLevelPercent();
		double GetMinTimestamp();
		double GetMaxTimestamp();
		uint32_t GetChunksCount();
		double GetCurrentTimestamp();
		uint32_t GetCurrentChunkIndex();
		AppleStreamingClientApplication *GetApplication();
		void SetAllowedBitrates(map<uint32_t, uint32_t> allowedBitrates);

		//processing
		bool StartProcessing();

		//signaling
		static bool SignalProtocolCreated(BaseProtocol *pProtocol, Variant &parameters);
		bool SignalMasterPlaylistAvailable();
		bool SignalChildPlaylistAvailable(uint32_t bw);
		bool SignalChildPlaylistNotAvailable(uint32_t bw);
		bool SignalAESKeyAvailable(Variant &parameters);
		bool SignalTSProtocolAvailable(uint32_t protocolId, uint32_t bw);
		bool SignalTSChunkComplete(uint32_t bw);
		bool SignalSpeedDetected(double instantAmount, double instantTime);
		bool SignalAVDataAvailable(IOBuffer &buffer);
		bool SignalStreamRegistered(BaseStream *pStream);
		bool SignalStreamUnRegistered(BaseStream *pStream);

		bool StartFeeding();
		bool FetchChildPlaylist(string uri, uint32_t bw);
		bool ConsumeAVBuffer();
	private:
		uint32_t GetOptimalBw();
		bool ParseConnectingString();
		bool FetchMasterPlaylist();
		bool FetchKey(string keyUri, string itemUri, uint32_t bw);
		bool FetchTS(string uri, uint32_t bw, string key, uint64_t iv);
		bool FetchURI(string uriString, string requestType, Variant &customParameters);

		bool EnqueueStartFeeding();
		bool EnqueueFetchChildPlaylist(string uri, uint32_t bw);
	};
};
#endif	/* _CLIENTCONTEXT_H */

