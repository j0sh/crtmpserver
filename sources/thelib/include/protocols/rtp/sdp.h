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

#ifdef HAS_PROTOCOL_RTP
#ifndef _SDP_H
#define	_SDP_H

#include "common.h"

#define SDP_SESSION		"session"
#define SDP_MEDIATRACKS	"mediaTracks"
#define SDP_A			"attributes"
#define SDP_B			"bandwidth"
#define SDP_C			"connection"
#define SDP_E			"email"
#define SDP_I			"sessionInfo"
#define SDP_K			"encryptionKey"
#define SDP_M			"media"
#define SDP_O			"owner"
#define SDP_P			"phone"
#define SDP_R			"repeat"
#define SDP_S			"sessionName"
#define SDP_T			"startStopTime"
#define SDP_U			"descriptionUri"
#define SDP_V			"version"
#define SDP_Z			"timeZone"

#define SDP_TRACK_GLOBAL_INDEX(x)	((x)["globalTrackIndex"])
#define SDP_TRACK_IS_AUDIO(x)		((x)["isAudio"])
#define SDP_TRACK_BANDWIDTH(x)		((x)["bandwidth"])
#define SDP_VIDEO_SERVER_IP(x)		((x)["ip"])
#define SDP_VIDEO_CONTROL_URI(x)	((x)["controlUri"])
#define SDP_VIDEO_CODEC(x)			((x)["codec"])
#define SDP_VIDEO_CODEC_H264_SPS(x)	((x)["h264SPS"])
#define SDP_VIDEO_CODEC_H264_PPS(x)	((x)["h264PPS"])
#define SDP_AUDIO_SERVER_IP(x)		((x)["ip"])
#define SDP_AUDIO_CONTROL_URI(x)	((x)["controlUri"])
#define SDP_AUDIO_CODEC(x)			((x)["codec"])
#define SDP_AUDIO_CODEC_SETUP(x)	((x)["codecSetup"])

class DLLEXP SDP
: public Variant {
public:
	SDP();
	virtual ~SDP();

	static bool ParseSDP(SDP &sdp, string &raw);
	Variant GetVideoTrack(uint32_t index, string uri);
	Variant GetAudioTrack(uint32_t index, string uri);
	string GetStreamName();
	uint32_t GetTotalBandwidth();
	static bool ParseTransportLine(string raw, Variant &result);
private:
	static bool ParseSection(Variant &result, vector<string> &lines,
			uint32_t start, uint32_t length);
	static bool ParseSDPLine(Variant &result, string &line);
	static bool ParseSDPLineA(string &attributeName, Variant &value, string line);
	static bool ParseSDPLineB(Variant &result, string line);
	static bool ParseSDPLineC(Variant &result, string line);
	static bool ParseSDPLineE(Variant &result, string line);
	static bool ParseSDPLineI(Variant &result, string line);
	static bool ParseSDPLineK(Variant &result, string line);
	static bool ParseSDPLineM(Variant &result, string line);
	static bool ParseSDPLineO(Variant &result, string line);
	static bool ParseSDPLineP(Variant &result, string line);
	static bool ParseSDPLineR(Variant &result, string line);
	static bool ParseSDPLineS(Variant &result, string line);
	static bool ParseSDPLineT(Variant &result, string line);
	static bool ParseSDPLineU(Variant &result, string line);
	static bool ParseSDPLineV(Variant &result, string line);
	static bool ParseSDPLineZ(Variant &result, string line);

	Variant GetTrack(uint32_t index, string type);
private:
	Variant ParseVideoTrack(Variant &track);
	Variant ParseAudioTrack(Variant &track);
};


#endif	/* _SDP_H */
#endif /* HAS_PROTOCOL_RTP */


