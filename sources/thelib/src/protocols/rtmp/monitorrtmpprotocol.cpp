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


#ifdef HAS_PROTOCOL_RTMP
#include "application/clientapplicationmanager.h"
#include "netio/netio.h"
#include "protocols/protocolmanager.h"
#include "protocols/rtmp/monitorrtmpprotocol.h"
#include "protocols/rtmp/basertmpappprotocolhandler.h"
#include "protocols/rtmp/messagefactories/messagefactories.h"
#include "protocols/rtmp/streaming/baseoutnetrtmpstream.h"
#include "streaming/streamstypes.h"
#include "protocols/rtmp/streaming/innetrtmpstream.h"
#include "protocols/rtmp/streaming/infilertmpstream.h"
#include "protocols/rtmp/streaming/rtmpstream.h"
#include "streaming/streamstypes.h"

MonitorRTMPProtocol::MonitorRTMPProtocol(uint32_t maxStreamCount,
		uint32_t maxChannelsCount)
: BaseProtocol(PT_INBOUND_RTMP) {
	_maxChannelsCount = maxChannelsCount;
	_channels = NULL;
	_channels = new Channel[maxChannelsCount];
	for (uint32_t i = 0; i < _maxChannelsCount; i++) {
		memset(&_channels[i], 0, sizeof (Channel));
		_channels[i].id = i;
		_channels[i].lastOutStreamId = 0xffffffff;
	}
	_selectedChannel = -1;
	_inboundChunkSize = 128;
	_maxStreamCount = maxStreamCount;
}

MonitorRTMPProtocol::~MonitorRTMPProtocol() {
	if (_channels != NULL) {
		delete[] _channels;
		_channels = NULL;
	}
}

bool MonitorRTMPProtocol::Initialize(Variant &parameters) {
	GetCustomParameters() = parameters;
	return true;
}

bool MonitorRTMPProtocol::AllowFarProtocol(uint64_t type) {
	return false;
}

bool MonitorRTMPProtocol::AllowNearProtocol(uint64_t type) {
	return false;
}

bool MonitorRTMPProtocol::SignalInputData(int32_t recvAmount) {
	ASSERT("OPERATION NOT SUPPORTED");
	return false;
}

bool MonitorRTMPProtocol::SignalInputData(IOBuffer &buffer) {
	return ProcessBytes(buffer);
}

bool MonitorRTMPProtocol::SetInboundChunkSize(uint32_t chunkSize) {
	_inboundChunkSize = chunkSize;
	return true;
}

bool MonitorRTMPProtocol::Feed(IOBuffer &buffer) {
	_input.ReadFromBuffer(GETIBPOINTER(buffer), GETAVAILABLEBYTESCOUNT(buffer));
	return SignalInputData(_input);
}

bool MonitorRTMPProtocol::ProcessBytes(IOBuffer &buffer) {
	while (true) {
		uint32_t availableBytesCount = GETAVAILABLEBYTESCOUNT(buffer);
		if (_selectedChannel < 0) {
			if (availableBytesCount < 1) {
				return true;
			} else {
				switch (GETIBPOINTER(buffer)[0]&0x3f) {
					case 0:
					{
						if (availableBytesCount < 2) {
							FINEST("Not enough data");
							return true;
						}
						_selectedChannel = 64 + GETIBPOINTER(buffer)[1];
						_channels[_selectedChannel].lastInHeaderType = GETIBPOINTER(buffer)[0] >> 6;
						buffer.Ignore(2);
						availableBytesCount -= 2;
						break;
					}
					case 1:
					{
						//						if (availableBytesCount < 3) {
						//							FINEST("Not enough data");
						//							return true;
						//						}
						//						_selectedChannel = GETIBPOINTER(buffer)[2]*256 + GETIBPOINTER(buffer)[1] + 64;
						//						_channels[_selectedChannel].lastInHeaderType = GETIBPOINTER(buffer)[0] >> 6;
						//						buffer.Ignore(3);
						//						availableBytesCount -= 3;
						//						break;
						FATAL("The server doesn't support channel ids bigger than 319");
						return false;
					};
					default:
					{
						_selectedChannel = GETIBPOINTER(buffer)[0]&0x3f;
						_channels[_selectedChannel].lastInHeaderType = GETIBPOINTER(buffer)[0] >> 6;
						buffer.Ignore(1);
						availableBytesCount -= 1;
						break;
					}
				}
			}
		}

		Channel &channel = _channels[_selectedChannel];
		Header &header = channel.lastInHeader;
		FINEST("header: %s", STR(header));

		if (channel.state == CS_HEADER) {
			if (!header.Read(_selectedChannel, channel.lastInHeaderType,
					buffer, availableBytesCount)) {
				FATAL("Unable to read header");
				return false;
			} else {
				if (!header.readCompleted)
					return true;

				if (H_SI(header) >= _maxStreamCount) {
					FATAL("%s", STR(header));
					FATAL("buffer:\n%s", STR(buffer));
					ASSERT("invalid stream index");
				}

				if (H_CI(header) >= _maxChannelsCount) {
					FATAL("%s", STR(header));
					FATAL("buffer:\n%s", STR(buffer));
					ASSERT("invalid channel index");
				}

				switch ((uint8_t) H_MT(header)) {
					case RM_HEADER_MESSAGETYPE_ABORTMESSAGE:
					case RM_HEADER_MESSAGETYPE_ACK:
					case RM_HEADER_MESSAGETYPE_AGGREGATE:
					case RM_HEADER_MESSAGETYPE_AUDIODATA:
					case RM_HEADER_MESSAGETYPE_CHUNKSIZE:
					case RM_HEADER_MESSAGETYPE_FLEX:
					case RM_HEADER_MESSAGETYPE_FLEXSHAREDOBJECT:
					case RM_HEADER_MESSAGETYPE_FLEXSTREAMSEND:
					case RM_HEADER_MESSAGETYPE_INVOKE:
					case RM_HEADER_MESSAGETYPE_NOTIFY:
					case RM_HEADER_MESSAGETYPE_PEERBW:
					case RM_HEADER_MESSAGETYPE_SHAREDOBJECT:
					case RM_HEADER_MESSAGETYPE_USRCTRL:
					case RM_HEADER_MESSAGETYPE_VIDEODATA:
					case RM_HEADER_MESSAGETYPE_WINACKSIZE:
					{
						break;
					}
					default:
					{
						FATAL("%s", STR(header));
						FATAL("buffer:\n%s", STR(buffer));
						ASSERT("invalid message type");
					}
				}
				channel.state = CS_PAYLOAD;
				switch (channel.lastInHeaderType) {
					case HT_FULL:
					{
						channel.lastInAbsTs = H_TS(header);
						break;
					}
					case HT_SAME_STREAM:
					case HT_SAME_LENGTH_AND_STREAM:
					{
						channel.lastInAbsTs += H_TS(header);
						break;
					}
					case HT_CONTINUATION:
					{
						if (channel.lastInProcBytes == 0) {
							channel.lastInAbsTs += H_TS(header);
						}
						break;
					}
				}
			}
		}

		if (channel.state == CS_PAYLOAD) {
			uint32_t tempSize = H_ML(header) - channel.lastInProcBytes;
			tempSize = (tempSize >= _inboundChunkSize) ? _inboundChunkSize : tempSize;
			uint32_t availableBytes = GETAVAILABLEBYTESCOUNT(buffer);
			if (tempSize > availableBytes)
				return true;
			channel.state = CS_HEADER;
			_selectedChannel = -1;
			switch (H_MT(header)) {
				case RM_HEADER_MESSAGETYPE_VIDEODATA:
				{
					if (H_SI(header) >= _maxStreamCount) {
						FATAL("Incorrect stream index");
						return false;
					}

					//FINEST("Video data");

					channel.lastInProcBytes += tempSize;
					if (H_ML(header) == channel.lastInProcBytes) {
						channel.lastInProcBytes = 0;
					}
					if (!buffer.Ignore(tempSize)) {
						FATAL("V: Unable to ignore %u bytes", tempSize);
						return false;
					}
					break;
				}
				case RM_HEADER_MESSAGETYPE_AUDIODATA:
				{
					if (H_SI(header) >= _maxStreamCount) {
						FATAL("Incorrect stream index");
						return false;
					}

					//FINEST("Audio data");

					channel.lastInProcBytes += tempSize;
					if (H_ML(header) == channel.lastInProcBytes) {
						channel.lastInProcBytes = 0;
					}
					if (!buffer.Ignore(tempSize)) {
						FATAL("A: Unable to ignore %u bytes", tempSize);
						return false;
					}
					break;
				}
				default:
				{
					channel.inputData.ReadFromInputBuffer(buffer, tempSize);
					channel.lastInProcBytes += tempSize;
					if (!buffer.Ignore(tempSize)) {
						FATAL("Unable to ignore %u bytes", tempSize);
						return false;
					}
					if (H_ML(header) == channel.lastInProcBytes) {
						channel.lastInProcBytes = 0;
						Variant msg;
						if (!_rtmpProtocolSerializer.Deserialize(header, channel.inputData, msg)) {
							FATAL("Unable to deserialize message");
							return false;
						}

						if ((uint8_t) VH_MT(msg) == RM_HEADER_MESSAGETYPE_CHUNKSIZE) {
							_inboundChunkSize = (uint32_t) msg[RM_CHUNKSIZE];
						}

						if ((uint8_t) VH_MT(msg) == RM_HEADER_MESSAGETYPE_ABORTMESSAGE) {
							uint32_t channelId = (uint32_t) msg[RM_ABORTMESSAGE];
							if (channelId >= _maxChannelsCount) {
								FATAL("Invalid channel id in reset message: %"PRIu32, channelId);
								return false;
							}
							assert(_channels[channelId].id == channelId);
							_channels[channelId].Reset();
						}

						if (GETAVAILABLEBYTESCOUNT(channel.inputData) != 0) {
							FATAL("Invalid message!!! We have leftovers: %u bytes",
									GETAVAILABLEBYTESCOUNT(channel.inputData));
							return false;
						}
					}
					break;
				}
			}
		}
	}
}
#endif /* HAS_PROTOCOL_RTMP */

