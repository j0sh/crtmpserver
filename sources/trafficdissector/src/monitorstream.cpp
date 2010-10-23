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

#include "monitorstream.h"
#include "monitorrtmpprotocol.h"
#include "streaming/streamstypes.h"

MonitorStream::MonitorStream(BaseProtocol *pProtocol,
		StreamsManager *pStreamsManager, string name)
: BaseStream(pProtocol, pStreamsManager, ST_NEUTRAL_RTMP, name) {

}

MonitorStream::~MonitorStream() {
}

StreamCapabilities * MonitorStream::GetCapabilities() {
	NYI;
	return NULL;
}

bool MonitorStream::Play(double absoluteTimestamp, double length) {
	NYIR;
}

bool MonitorStream::Pause() {
	NYIR;
}

bool MonitorStream::Resume() {
	NYIR;
}

bool MonitorStream::Seek(double absoluteTimestamp) {
	NYIR;
}

bool MonitorStream::Stop() {
	NYIR;
}

bool MonitorStream::SignalPlay(double &absoluteTimestamp, double &length) {
	NYIR;
}

bool MonitorStream::SignalPause() {
	NYIR;
}

bool MonitorStream::SignalResume() {
	NYIR;
}

bool MonitorStream::SignalSeek(double &absoluteTimestamp) {
	NYIR;
}

bool MonitorStream::SignalStop() {
	NYIR;
}

bool MonitorStream::FeedData(uint8_t *pData, uint32_t dataLength,
		uint32_t processedLength, uint32_t totalLength,
		double absoluteTimestamp, bool isAudio) {
	NYIR;
}

void MonitorStream::ReadyForSend() {
	NYI;
}

bool MonitorStream::IsCompatibleWithType(uint64_t type) {
	NYIR;
}
