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
#include "protocols/rtp/sdp.h"
#include "streaming/codectypes.h"

SDP::SDP()
: Variant() {
}

SDP::~SDP() {
}

bool SDP::ParseSDP(SDP &sdp, string &raw) {
	//1. Reset
	sdp.Reset();

	//2. Prepare the sections
	sdp[SDP_SESSION].IsArray(false);
	sdp[SDP_MEDIATRACKS].IsArray(true);

	//3. Split the raw content into lines
	replace(raw, "\r\n", "\n");
	vector<string> lines;
	split(raw, "\n", lines);

	//4. Detect the media tracks indexes
	vector<uint32_t> trackIndexes;
	for (uint32_t i = 0; i < lines.size(); i++) {
		if (lines[i].find("m=") == 0) {
			ADD_VECTOR_END(trackIndexes, i);
		}
	}
	if (trackIndexes.size() == 0) {
		FATAL("No tracks found");
		return false;
	}

	//5. Parse the header
	if (!ParseSection(sdp[SDP_SESSION], lines, 0, trackIndexes[0])) {
		FATAL("Unable to parse header");
		return false;
	}

	//6. Parse the media sections
	Variant media;
	for (uint32_t i = 0; i < trackIndexes.size() - 1; i++) {
		media.Reset();
		media.IsArray(false);
		if (!ParseSection(media, lines, trackIndexes[i], trackIndexes[i + 1] - trackIndexes[i])) {
			FATAL("Unable to parse header");
			return false;
		}
		sdp[SDP_MEDIATRACKS].PushToArray(media);
	}

	//7. Parse the last media section
	media.Reset();
	media.IsArray(false);
	if (!ParseSection(media, lines,
			trackIndexes[trackIndexes.size() - 1],
			trackIndexes.size() - trackIndexes[trackIndexes.size() - 1])) {
		FATAL("Unable to parse header");
		return false;
	}
	sdp[SDP_MEDIATRACKS].PushToArray(media);


	return true;
}

Variant SDP::GetVideoTrack(uint32_t index, string uri) {
	//1. Find the track
	Variant track = GetTrack(index, "video");
	if (track == V_NULL) {
		FATAL("Video track index %u not found", index);
		return Variant();
	}

	//2. Prepare the info
	Variant result;
	SDP_VIDEO_SERVER_IP(result) = (*this)[SDP_SESSION][SDP_O]["address"];
	string control = track[SDP_A].GetValue("control", false);
	if (control.find("rtsp") == 0)
		SDP_VIDEO_CONTROL_URI(result) = control;
	else
		SDP_VIDEO_CONTROL_URI(result) = uri + "/" + control;
	SDP_VIDEO_CODEC(result) = track[SDP_A].GetValue("rtpmap", false)["encodingName"];
	if ((uint64_t) SDP_VIDEO_CODEC(result) != CODEC_VIDEO_AVC) {
		FATAL("The only supported video codec is h264");
		return Variant();
	}
	SDP_VIDEO_CODEC_H264_SPS(result) = track[SDP_A]
			.GetValue("fmtp", false)
			.GetValue("sprop-parameter-sets", false)["SPS"];
	SDP_VIDEO_CODEC_H264_PPS(result) = track[SDP_A]
			.GetValue("fmtp", false)
			.GetValue("sprop-parameter-sets", false)["PPS"];
	SDP_TRACK_GLOBAL_INDEX(result) = SDP_TRACK_GLOBAL_INDEX(track);
	SDP_TRACK_IS_AUDIO(result) = (bool)false;
	if (track.HasKeyChain(V_UINT32, false, 1, SDP_B))
		SDP_TRACK_BANDWIDTH(result) = track[SDP_B];
	else
		SDP_TRACK_BANDWIDTH(result) = (uint32_t) 0;

	//3. Done
	return result;
}

Variant SDP::GetAudioTrack(uint32_t index, string uri) {
	//1. Find the track
	Variant track = GetTrack(index, "audio");
	if (track == V_NULL) {
		FATAL("Audio track index %u not found", index);
		return Variant();
	}

	//2. Prepare the info
	Variant result;
	SDP_AUDIO_SERVER_IP(result) = (*this)[SDP_SESSION][SDP_O]["address"];
	string control = track[SDP_A].GetValue("control", false);
	if (control.find("rtsp") == 0)
		SDP_AUDIO_CONTROL_URI(result) = control;
	else
		SDP_AUDIO_CONTROL_URI(result) = uri + "/" + control;
	SDP_AUDIO_CODEC(result) = track[SDP_A].GetValue("rtpmap", false)["encodingName"];
	if ((uint64_t) SDP_AUDIO_CODEC(result) != CODEC_AUDIO_AAC) {
		FATAL("The only supported audio codec is aac");
		return Variant();
	}

	SDP_AUDIO_CODEC_SETUP(result) = track[SDP_A]
			.GetValue("fmtp", false)
			.GetValue("config", false);
	SDP_TRACK_GLOBAL_INDEX(result) = SDP_TRACK_GLOBAL_INDEX(track);
	SDP_TRACK_IS_AUDIO(result) = (bool)true;
	if (track.HasKeyChain(V_UINT32, false, 1, SDP_B))
		SDP_TRACK_BANDWIDTH(result) = track[SDP_B];
	else
		SDP_TRACK_BANDWIDTH(result) = (uint32_t) 0;

	//3. Done
	return result;
}

uint32_t SDP::GetTotalBandwidth() {
	if (HasKeyChain(V_UINT32, false, 2, SDP_SESSION, SDP_B))
		return (uint32_t) ((*this)[SDP_SESSION][SDP_B]);
	else
		return 0;
}

string SDP::GetStreamName() {
	if (!HasKey(SDP_SESSION))
		return "";
	if (!(*this)[SDP_SESSION].HasKey(SDP_S))
		return "";
	return (string) (*this)[SDP_SESSION][SDP_S];
}

bool SDP::ParseTransportLine(string raw, Variant &result) {
	result.Reset();

	//1. split after ';'
	vector<string> parts;
	split(raw, ";", parts);

	//2. Construct the result
	for (uint32_t i = 0; i < parts.size(); i++) {
		string part = parts[i];
		trim(part);
		if (part == "")
			continue;
		string::size_type pos = part.find('=');
		if (pos == string::npos) {
			result[lowerCase(part)] = (bool)true;
			continue;
		}
		result[lowerCase(part.substr(0, pos))] = part.substr(pos + 1);
	}

	vector<string> keys;
	ADD_VECTOR_END(keys, "client_port");
	ADD_VECTOR_END(keys, "server_port");
	ADD_VECTOR_END(keys, "interleaved");

	for (uint32_t i = 0; i < keys.size(); i++) {
		string key = keys[i];
		if (!result.HasKey(key))
			continue;
		parts.clear();
		raw = (string) result[key];
		split(raw, "-", parts);
		if ((parts.size() != 2) && (parts.size() != 1)) {
			FATAL("Invalid transport line: %s", STR(raw));
			return false;
		}
		string all = "";
		uint16_t data = 0;
		uint16_t rtcp = 0;
		if (parts.size() == 2) {
			data = atoi(STR(parts[0]));
			rtcp = atoi(STR(parts[1]));
			if (((data % 2) != 0) || ((data + 1) != rtcp)) {
				FATAL("Invalid transport line: %s", STR(raw));
				return false;
			}
			all = format("%"PRIu16"-%"PRIu16, data, rtcp);
		} else {
			data = atoi(STR(parts[0]));
			all = format("%"PRIu16, data);
			rtcp = 0;
		}
		if (all != raw) {
			FATAL("Invalid transport line: %s", STR(raw));
			return false;
		}
		result.RemoveKey(key);
		result[key]["data"] = (uint16_t) data;
		result[key]["rtcp"] = (uint16_t) rtcp;
		result[key]["all"] = all;
	}

	return true;
}

bool SDP::ParseSection(Variant &result, vector<string> &lines,
		uint32_t start, uint32_t length) {
	for (uint32_t i = 0; ((i + start) < lines.size()) && (i < length); i++) {
		if (lines[i + start] == "")
			continue;
		if (!ParseSDPLine(result, lines[i + start])) {
			FATAL("Parsing line %s failed", STR(lines[i + start]));
			return false;
		}
	}
	return true;
}

bool SDP::ParseSDPLine(Variant &result, string &line) {
	//1. test if this is a valid line
	if (line.size() < 2) {
		FATAL("Invalid line: %s", STR(line));
		return false;
	}
	if (!((line[0] >= 'a') && (line[0] <= 'z'))) {
		FATAL("Invalid line: %s", STR(line));
		return false;
	}
	if (line[1] != '=') {
		FATAL("Invalid line: %s", STR(line));
		return false;
	}

#define FORBID_DUPLICATE(x) do{if (result.HasKey(x)) {FATAL("Duplicate value: %s", STR(line));return false;}}while(0);

	switch (line[0]) {
		case 'a':
		{
			string name;
			Variant value;
			if (!ParseSDPLineA(name, value, line.substr(2)))
				return false;
			if (result[SDP_A] == V_NULL)
				result[SDP_A].IsArray(false);
			if (result[SDP_A].HasKey(name, false)) {
				WARN("This attribute overrided a previous one");
			}
			result[SDP_A][name] = value;
			return true;
		}
		case 'b':
		{
			FORBID_DUPLICATE(SDP_B);
			return ParseSDPLineB(result[SDP_B], line.substr(2));
		}
		case 'c':
		{
			FORBID_DUPLICATE(SDP_C);
			return ParseSDPLineC(result[SDP_C], line.substr(2));
		}
		case 'e':
		{
			Variant node;
			if (!ParseSDPLineE(node, line.substr(2)))
				return false;
			result[SDP_E].PushToArray(node);
			return true;
		}
		case 'i':
		{
			FORBID_DUPLICATE(SDP_I);
			return ParseSDPLineI(result[SDP_I], line.substr(2));
		}
		case 'k':
		{
			FORBID_DUPLICATE(SDP_K);
			return ParseSDPLineK(result[SDP_K], line.substr(2));
		}
		case 'm':
		{
			FORBID_DUPLICATE(SDP_M);
			return ParseSDPLineM(result[SDP_M], line.substr(2));
		}
		case 'o':
		{
			FORBID_DUPLICATE(SDP_O);
			return ParseSDPLineO(result[SDP_O], line.substr(2));
		}
		case 'p':
		{
			Variant node;
			if (!ParseSDPLineP(node, line.substr(2)))
				return false;
			result[SDP_P].PushToArray(node);
			return true;
		}
		case 'r':
		{
			FORBID_DUPLICATE(SDP_R);
			return ParseSDPLineR(result[SDP_R], line.substr(2));
		}
		case 's':
		{
			FORBID_DUPLICATE(SDP_S);
			return ParseSDPLineS(result[SDP_S], line.substr(2));
		}
		case 't':
		{
			Variant node;
			if (!ParseSDPLineT(node, line.substr(2)))
				return false;
			result[SDP_T].PushToArray(node);
			return true;
		}
		case 'u':
		{
			FORBID_DUPLICATE(SDP_U);
			return ParseSDPLineU(result[SDP_U], line.substr(2));
		}
		case 'v':
		{
			FORBID_DUPLICATE(SDP_V);
			return ParseSDPLineV(result[SDP_V], line.substr(2));
		}
		case 'z':
		{
			FORBID_DUPLICATE(SDP_Z);
			return ParseSDPLineZ(result[SDP_Z], line.substr(2));
		}
		default:
		{
			FATAL("Invalid line: %s", STR(line));
			return false;
		}
	}
}

bool SDP::ParseSDPLineA(string &attributeName, Variant &value, string line) {
	string::size_type pos = line.find(':');
	if ((pos == string::npos)
			|| (pos == 0)
			|| (pos == (line.size() - 1))) {
		attributeName = line;
		value = (bool)true;
		return true;
	}

	attributeName = line.substr(0, pos);
	string rawValue = line.substr(line.find(':') + 1);
	if (attributeName == "control") {
		value = rawValue;
		return true;
	} else if (attributeName == "maxprate") {
		value = (double) strtod(STR(rawValue), NULL);
		return true;
	} else if (attributeName.find("x-") == 0) {
		value = rawValue;
		return true;
	} else if (attributeName == "rtpmap") {
		//rtpmap:<payload type> <encoding name>/<clock rate>[/<encoding parameters>]
		vector<string> parts;
		split(rawValue, " ", parts);
		if (parts.size() != 2)
			return false;
		value["payloadType"] = (uint8_t) atoi(STR(parts[0]));
		split(parts[1], "/", parts);
		if ((parts.size() != 2) && (parts.size() != 3))
			return false;
		value["encodingName"] = parts[0];
		if (lowerCase((string) value["encodingName"]) == "h264") {
			value["encodingName"] = (uint64_t) CODEC_VIDEO_AVC;
		} else if ((string) lowerCase(value["encodingName"]) == "mpeg4-generic") {
			value["encodingName"] = (uint64_t) CODEC_AUDIO_AAC;
		} else {
			WARN("Invalid codec: %s", STR(value["encodingName"]));
			value.Reset();
			return true;
		}
		value["clockRate"] = (uint32_t) atoi(STR(parts[1]));
		if (parts.size() == 3) {
			value["encodingParameters"] = parts[2];
		}

		return true;
	} else if (attributeName == "fmtp") {
		replace(rawValue, "; ", ";");
		vector<string> parts;
		split(rawValue, " ", parts);
		if (parts.size() != 2)
			return false;
		value["payloadType"] = (uint8_t) atoi(STR(parts[0]));
		map<string, string> temp = mapping(parts[1], ";", "=", false);

		FOR_MAP(temp, string, string, i) {
			value[MAP_KEY(i)] = MAP_VAL(i);
		}

		return true;
	} else {
		WARN("Attribute `%s` with value `%s` not parsed", STR(attributeName), STR(rawValue));
		value = rawValue;
		return true;
	}
}

bool SDP::ParseSDPLineB(Variant &result, string line) {
	//b=<modifier>:<bandwidth-value>
	result.Reset();

	vector<string> parts;
	split(line, ":", parts);
	if (parts.size() != 2)
		return false;

	result["modifier"] = parts[0];
	result["value"] = parts[1];

	if (parts[0] == "AS") {
		uint32_t val = (uint32_t) atoi(STR(parts[1]));
		result = (uint32_t) val;
	} else {
		WARN("Bandwidth modifier %s not implemented", STR(result["modifier"]));
		result = (uint32_t) 0;
	}

	return true;
}

bool SDP::ParseSDPLineC(Variant &result, string line) {
	//c=<network type> <address type> <connection address>
	result.Reset();

	vector<string> parts;
	split(line, " ", parts);
	if (parts.size() != 3)
		return false;

	result["networkType"] = parts[0];
	result["addressType"] = parts[1];
	result["connectionAddress"] = parts[2];

	return true;
}

bool SDP::ParseSDPLineE(Variant &result, string line) {
	result.Reset();
	result = line;
	return true;
}

bool SDP::ParseSDPLineI(Variant &result, string line) {
	result.Reset();
	result = line;
	return true;
}

bool SDP::ParseSDPLineK(Variant &result, string line) {
	result.Reset();
	NYIR;
}

bool SDP::ParseSDPLineM(Variant &result, string line) {
	//m=<media> <port> <transport> <fmt list>
	result.Reset();

	vector<string> parts;
	split(line, " ", parts);
	if (parts.size() != 4)
		return false;

	result["media_type"] = parts[0];
	result["ports"] = parts[1];
	result["transport"] = parts[2];
	result["payloadType"] = parts[3];
	return true;
}

bool SDP::ParseSDPLineO(Variant &result, string line) {
	//o=<username> <session id> <version> <network type> <address type> <address>
	result.Reset();

	vector<string> parts;
	split(line, " ", parts);
	if (parts.size() != 6)
		return false;

	result["username"] = parts[0];
	result["sessionId"] = parts[1];
	result["version"] = parts[2];
	result["networkType"] = parts[3];
	result["addressType"] = parts[4];
	result["address"] = parts[5];

	if ((string) result["networkType"] != "IN") {
		FATAL("Unsupported network type: %s", STR(result["networkType"]));
		return false;
	}

	if ((string) result["addressType"] != "IP4") {
		FATAL("Unsupported address type: %s", STR(result["addressType"]));
		return false;
	}

	string ip = getHostByName(result["address"]);
	if (ip == "") {
		WARN("Invalid address: %s", STR(result["address"]));
	}
	result["ip_address"] = ip;

	return true;
}

bool SDP::ParseSDPLineP(Variant &result, string line) {
	result.Reset();
	result = line;
	return true;
}

bool SDP::ParseSDPLineR(Variant &result, string line) {
	result.Reset();
	NYIR;
}

bool SDP::ParseSDPLineS(Variant &result, string line) {
	result.Reset();
	result = line;
	return true;
}

bool SDP::ParseSDPLineT(Variant &result, string line) {
	//t=<start time>  <stop time>
	result.Reset();

	vector<string> parts;
	split(line, " ", parts);
	if (parts.size() != 2)
		return false;

	result["startTime"] = parts[0];
	result["stopTime"] = parts[1];

	return true;
}

bool SDP::ParseSDPLineU(Variant &result, string line) {
	result.Reset();
	result = line;
	return true;
}

bool SDP::ParseSDPLineV(Variant &result, string line) {
	result.Reset();
	if (line != "0")
		return false;
	result = (uint32_t) 0;
	return true;
}

bool SDP::ParseSDPLineZ(Variant &result, string line) {
	result.Reset();
	NYIR;
}

Variant SDP::GetTrack(uint32_t index, string type) {
	uint32_t videoTracksCount = 0;
	uint32_t audioTracksCount = 0;
	uint32_t globalTrackIndex = 0;
	Variant result;

	FOR_MAP((*this)[SDP_MEDIATRACKS], string, Variant, i) {
		if (MAP_VAL(i)[SDP_M]["media_type"] == type) {
			if (type == "video") {
				videoTracksCount++;
				if (videoTracksCount == (index + 1)) {
					result = ParseVideoTrack(MAP_VAL(i));
					break;
				}
			} else if (type == "audio") {
				audioTracksCount++;
				if (audioTracksCount == (index + 1)) {
					result = ParseAudioTrack(MAP_VAL(i));
					break;
				}
			}
		}
		globalTrackIndex++;
	}
	if (result != V_NULL) {
		SDP_TRACK_GLOBAL_INDEX(result) = globalTrackIndex;
	}
	return result;
}

Variant SDP::ParseVideoTrack(Variant &track) {
	Variant result = track;
	if (!result.HasKey(SDP_A)) {
		FATAL("Track with no attributes");
		return Variant();
	}
	if (!result[SDP_A].HasKey("control", false)) {
		FATAL("Track with no control uri");
		return Variant();
	}
	if (!result[SDP_A].HasKey("rtpmap", false)) {
		FATAL("Track with no rtpmap");
		return Variant();
	}
	if (!result[SDP_A].HasKey("fmtp", false)) {
		FATAL("Track with no fmtp");
		return Variant();
	}
	Variant &fmtp = result[SDP_A].GetValue("fmtp", false);

	if (!fmtp.HasKey("sprop-parameter-sets", false)) {
		FATAL("Video doesn't have sprop-parameter-sets");
		return Variant();
	}
	Variant &temp = fmtp.GetValue("sprop-parameter-sets", false);
	vector<string> parts;
	split((string) temp, ",", parts);
	if (parts.size() != 2) {
		FATAL("Video doesn't have sprop-parameter-sets");
		return Variant();
	}
	temp.Reset();
	temp["SPS"] = parts[0];
	temp["PPS"] = parts[1];

	return result;
}

Variant SDP::ParseAudioTrack(Variant &track) {
	Variant result = track;

	if (!result.HasKey(SDP_A)) {
		FATAL("Track with no attributes");
		return Variant();
	}
	if (!result[SDP_A].HasKey("control", false)) {
		FATAL("Track with no control uri");
		return Variant();
	}
	if (!result[SDP_A].HasKey("rtpmap", false)) {
		FATAL("Track with no rtpmap");
		return Variant();
	}
	if (!result[SDP_A].HasKey("fmtp", false)) {
		FATAL("Track with no fmtp uri");
		return Variant();
	}

	Variant &fmtp = result[SDP_A].GetValue("fmtp", false);
	if (!fmtp.HasKey("config", false)) {
		FATAL("Invalid fmtp line:\n%s", STR(fmtp.ToString()));
		return Variant();
	}
	if (!fmtp.HasKey("mode", false)) {
		FATAL("Invalid fmtp line:\n%s", STR(fmtp.ToString()));
		return Variant();
	}
	if (lowerCase((string) fmtp.GetValue("mode", false)) != "aac-hbr") {
		FATAL("Invalid fmtp line:\n%s", STR(fmtp.ToString()));
		return Variant();
	}
	if (!fmtp.HasKey("SizeLength", false)) {
		FATAL("Invalid fmtp line:\n%s", STR(fmtp.ToString()));
		return Variant();
	}
	if ((string) fmtp.GetValue("sizelength", false) != "13") {
		FATAL("Invalid fmtp line:\n%s", STR(fmtp.ToString()));
		return Variant();
	}
	if (fmtp.HasKey("IndexLength", false)) {
		if ((string) fmtp.GetValue("IndexLength", false) != "3") {
			FATAL("Invalid fmtp line:\n%s", STR(fmtp.ToString()));
			return Variant();
		}
	}
	if (fmtp.HasKey("IndexDeltaLength", false)) {
		if ((string) fmtp.GetValue("IndexDeltaLength", false) != "3") {
			FATAL("Invalid fmtp line:\n%s", STR(fmtp.ToString()));
			return Variant();
		}
	}

	return result;
}
#endif /* HAS_PROTOCOL_RTP */
