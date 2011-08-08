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

#ifdef HAS_PROTOCOL_LIVEFLV

#include "protocols/liveflv/inboundliveflvprotocol.h"
#include "application/baseclientapplication.h"
#include "protocols/protocoltypes.h"
#include "netio/netio.h"
#include "protocols/liveflv/innetliveflvstream.h"
#include "protocols/rtmp/amf0serializer.h"
#include "streaming/baseoutstream.h"

InboundLiveFLVProtocol::InboundLiveFLVProtocol()
: BaseProtocol(PT_INBOUND_LIVE_FLV) {
	_pStream = NULL;
	_headerParsed = false;
	_waitForMetadata = false;
}

InboundLiveFLVProtocol::~InboundLiveFLVProtocol() {
	if (_pStream != NULL) {
		delete _pStream;
		_pStream = NULL;
	}
}

bool InboundLiveFLVProtocol::Initialize(Variant &parameters) {
	GetCustomParameters() = parameters;
	FINEST("parameters:\n%s", STR(parameters.ToString()));
	if (parameters.HasKey("waitForMetadata"))
		_waitForMetadata = (bool)parameters["waitForMetadata"];
	else
		_waitForMetadata = false;
	FINEST("_waitForMetadata: %hhu", _waitForMetadata);
	return true;
}

bool InboundLiveFLVProtocol::AllowFarProtocol(uint64_t type) {
	return (type == PT_TCP)
			|| (type == PT_INBOUND_HTTP)
			|| (type == PT_OUTBOUND_HTTP);
}

bool InboundLiveFLVProtocol::AllowNearProtocol(uint64_t type) {
	return false;
}

bool InboundLiveFLVProtocol::SignalInputData(int32_t recvAmount) {
	ASSERT("OPERATION NOT SUPPORTED");
	return false;
}

bool InboundLiveFLVProtocol::SignalInputData(IOBuffer &buffer) {
	//1. Initialize the stream
	if (_pStream == NULL) {
		if (!_waitForMetadata) {
			if (!InitializeStream("")) {
				FATAL("Unable to initialize the stream");
				return false;
			}
		}
	}
	for (;;) {
		//2. ignore the first 13 bytes of header
		if (!_headerParsed) {
			if (GETAVAILABLEBYTESCOUNT(buffer) < 13) {
				break;
			}
			buffer.Ignore(13);
			_headerParsed = true;
		}

		//3. Read the frame header
		if (GETAVAILABLEBYTESCOUNT(buffer) < 11) {
			break;
		}

		//4. Read the type, length and the timestamp
		uint8_t type;
		uint32_t length;
		uint32_t timestamp;
		type = GETIBPOINTER(buffer)[0];
		length = ENTOHLP((GETIBPOINTER(buffer) + 1)) >> 8; //----MARKED-LONG---
		if (length >= 1024 * 1024) {
			FATAL("Frame too large: %u", length);
			return false;
		}
		timestamp = ENTOHAP((GETIBPOINTER(buffer) + 4)); //----MARKED-LONG---

		//5. Do we have enough data? 15 bytes are the lead header and the trail length (11+4)
		if (GETAVAILABLEBYTESCOUNT(buffer) < length + 15) {
			return true;
		}

		//6. Ignore the 11 bytes of pre-header
		buffer.Ignore(11);

		//7. Ok, we have enough data. Pump it :)
		switch (type) {
			case 8:
			{
				//audio data
				if (_pStream != NULL) {
					if (!_pStream->FeedData(GETIBPOINTER(buffer), length, 0,
							length, timestamp, true)) {
						FATAL("Unable to feed audio");
						return false;
					}
				}
				break;
			}
			case 9:
			{
				//video data
				if (_pStream != NULL) {
					if (!_pStream->FeedData(GETIBPOINTER(buffer), length, 0,
							length, timestamp, false)) {
						FATAL("Unable to feed audio");
						return false;
					}
				}
				break;
			}
			case 18:
			{
				AMF0Serializer amf0;

				//1. get the raw buffer
				IOBuffer metaBuffer;
				metaBuffer.ReadFromBuffer(GETIBPOINTER(buffer), length);

				//2. Read the notify function
				Variant notifyFunction;
				if (!amf0.Read(metaBuffer, notifyFunction)) {
					FATAL("Unable to read notify function");
					return false;
				}
				if (notifyFunction != V_STRING) {
					FATAL("Unable to read notify function");
					return false;
				}

				//3. Read the rest of the parameters
				Variant parameters;
				string streamName = "";
				while (GETAVAILABLEBYTESCOUNT(metaBuffer) > 0) {
					Variant v;
					if (!amf0.Read(metaBuffer, v)) {
						FATAL("Unable to read metadata item");
						return false;
					}
					if (v.HasKey("streamName")) {
						if (v["streamName"] == V_STRING) {
							streamName = (string) v["streamName"];
						}
					}
					parameters.PushToArray(v);
				}

				if (_pStream == NULL) {
					if (!InitializeStream(streamName)) {
						FATAL("Unable to initialize the stream");
						return false;
					}
				}

				INFO("Stream metadata:\n%s", STR(parameters.ToString()));

				//4. Send the notify
				if (_pStream != NULL) {
					if (!_pStream->SendStreamMessage(notifyFunction, parameters, true)) {
						FATAL("Unable to send the notify");
						return false;
					}
				}

				break;
			}
			default:
			{
				FATAL("Invalid frame type: %hhu", type);
				return false;
			}
		}

		//8. ignore what we've just fed
		buffer.Ignore(length + 4);
	}

	//9. Done
	return true;
}

bool InboundLiveFLVProtocol::InitializeStream(string streamName) {
	if (streamName == "") {
		//1. Compute a stream name based on the nature of the carrier (if any...)
		if (GetIOHandler() != NULL) {
			//we have a carrier
			if (GetIOHandler()->GetType() == IOHT_TCP_CARRIER) {
				//this is a tcp carrier
				streamName = format("%s_%hu",
						STR(((TCPCarrier *) GetIOHandler())->GetFarEndpointAddressIp()),
						((TCPCarrier *) GetIOHandler())->GetFarEndpointPort());
			} else {
				//this is not a TCP carrier
				streamName = format("flv_%u", GetId());
			}
		} else {
			//we don't have a carrier. This protocl might be artificially fed
			streamName = format("flv_%u", GetId());
		}
	}

	if (!GetApplication()->StreamNameAvailable(streamName, this)) {
		FATAL("Stream %s already taken", STR(streamName));
		return false;
	}

	_pStream = new InNetLiveFLVStream(this,
			GetApplication()->GetStreamsManager(), streamName);

	//6. Get the list of waiting subscribers
	map<uint32_t, BaseOutStream *> subscribedOutStreams =
			GetApplication()->GetStreamsManager()->GetWaitingSubscribers(
			streamName, _pStream->GetType());

	//7. Bind the waiting subscribers

	FOR_MAP(subscribedOutStreams, uint32_t, BaseOutStream *, i) {
		BaseOutStream *pBaseOutStream = MAP_VAL(i);
		pBaseOutStream->Link(_pStream);
	}
	return true;
}
#endif /* HAS_PROTOCOL_LIVEFLV */
