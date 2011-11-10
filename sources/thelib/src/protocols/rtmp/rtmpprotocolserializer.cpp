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
#include "protocols/rtmp/rtmpprotocolserializer.h"
#include "protocols/rtmp/messagefactories/messagefactories.h"
#include "protocols/rtmp/amftypes.h"

RTMPProtocolSerializer::RTMPProtocolSerializer() {
}

RTMPProtocolSerializer::~RTMPProtocolSerializer() {
}

string RTMPProtocolSerializer::GetUserCtrlTypeString(uint16_t type) {
	switch (type) {
		case RM_USRCTRL_TYPE_STREAM_BEGIN:
			return "RM_USRCTRL_TYPE_STREAM_BEGIN";
		case RM_USRCTRL_TYPE_STREAM_EOF:
			return "RM_USRCTRL_TYPE_STREAM_EOF";
		case RM_USRCTRL_TYPE_STREAM_DRY:
			return "RM_USRCTRL_TYPE_STREAM_DRY";
		case RM_USRCTRL_TYPE_STREAM_IS_RECORDED:
			return "RM_USRCTRL_TYPE_STREAM_IS_RECORDED";
		case RM_USRCTRL_TYPE_STREAM_SET_BUFFER_LENGTH:
			return "RM_USRCTRL_TYPE_STREAM_SET_BUFFER_LENGTH";
		case RM_USRCTRL_TYPE_PING_REQUEST:
			return "RM_USRCTRL_TYPE_PING_REQUEST";
		case RM_USRCTRL_TYPE_PING_RESPONSE:
			return "RM_USRCTRL_TYPE_PING_RESPONSE";
		case RM_USRCTRL_TYPE_UNKNOWN1:
			return "RM_USRCTRL_TYPE_UNKNOWN1";
		case RM_USRCTRL_TYPE_UNKNOWN2:
			return "RM_USRCTRL_TYPE_UNKNOWN2";
		default:
			return format("#unknownUCT(%hu)", type);
	}
}

string RTMPProtocolSerializer::GetSOPrimitiveString(uint8_t type) {
	switch (type) {
		case SOT_CS_CONNECT:
			return "SOT_CS_CONNECT";
		case SOT_CS_DISCONNECT:
			return "SOT_CS_DISCONNECT";
		case SOT_CS_SET_ATTRIBUTE:
			return "SOT_CS_SET_ATTRIBUTE";
		case SOT_SC_UPDATE_DATA:
			return "SOT_SC_UPDATE_DATA";
		case SOT_SC_UPDATE_DATA_ACK:
			return "SOT_SC_UPDATE_DATA_ACK";
		case SOT_BW_SEND_MESSAGE:
			return "SOT_BW_SEND_MESSAGE";
		case SOT_SC_STATUS:
			return "SOT_SC_STATUS";
		case SOT_SC_CLEAR_DATA:
			return "SOT_SC_CLEAR_DATA";
		case SOT_SC_DELETE_DATA:
			return "SOT_SC_DELETE_DATA";
		case SOT_CSC_DELETE_DATA:
			return "SOT_CSC_DELETE_DATA";
		case SOT_SC_INITIAL_DATA:
			return "SOT_SC_INITIAL_DATA";
		default:
			return format("#unknownSOP(%hhu)", type);
	}
}

bool RTMPProtocolSerializer::Deserialize(Header &header, IOBuffer &buffer,
		Variant &message) {
	message.Reset();
	message[RM_HEADER] = header.GetVariant();
	switch (H_MT(header)) {
		case RM_HEADER_MESSAGETYPE_NOTIFY:
		{
			return DeserializeNotify(buffer, message[RM_NOTIFY]);
		}
		case RM_HEADER_MESSAGETYPE_FLEXSTREAMSEND:
		{
			return DeserializeFlexStreamSend(buffer, message[RM_FLEXSTREAMSEND]);
		}
		case RM_HEADER_MESSAGETYPE_INVOKE:
		case RM_HEADER_MESSAGETYPE_FLEX:
		{
			message[RM_INVOKE][RM_INVOKE_IS_FLEX] =
					(bool)(H_MT(header) == RM_HEADER_MESSAGETYPE_FLEX);
			return DeserializeInvoke(buffer, message[RM_INVOKE]);
		}
		case RM_HEADER_MESSAGETYPE_FLEXSHAREDOBJECT:
		{
			//TODO: This is a hack. We store the data on RM_SHAREDOBJECT
			//instead of RM_FLEXSHAREDOBJECT
			//return DeserializeFlexSharedObject(buffer, message[RM_FLEXSHAREDOBJECT]);
			return DeserializeFlexSharedObject(buffer, message[RM_SHAREDOBJECT]);
		}
		case RM_HEADER_MESSAGETYPE_SHAREDOBJECT:
		{
			return DeserializeSharedObject(buffer, message[RM_SHAREDOBJECT]);
		}
		case RM_HEADER_MESSAGETYPE_USRCTRL:
		{
			return DeserializeUsrCtrl(buffer, message[RM_USRCTRL]);
		}
		case RM_HEADER_MESSAGETYPE_CHUNKSIZE:
		{
			return DeserializeChunkSize(buffer, message[RM_CHUNKSIZE]);
		}
		case RM_HEADER_MESSAGETYPE_ACK:
		{
			return DeserializeAck(buffer, message[RM_ACK]);
		}
		case RM_HEADER_MESSAGETYPE_WINACKSIZE:
		{
			return DeserializeWinAckSize(buffer, message[RM_WINACKSIZE]);
		}
		case RM_HEADER_MESSAGETYPE_PEERBW:
		{
			return DeserializePeerBW(buffer, message[RM_PEERBW]);
		}
		case RM_HEADER_MESSAGETYPE_ABORTMESSAGE:
		{
			return DeserializeAbortMessage(buffer, message[RM_ABORTMESSAGE]);
		}
		default:
		{
			FATAL("Invalid message type: %u %s", H_MT(header), STR(buffer));
			return false;
		}
	}
}

bool RTMPProtocolSerializer::Serialize(Channel &channel,
		Variant &message, IOBuffer &buffer, uint32_t chunkSize) {
	bool result = false;
	_internalBuffer.Ignore(GETAVAILABLEBYTESCOUNT(_internalBuffer));

	switch ((uint32_t) VH_MT(message)) {
		case RM_HEADER_MESSAGETYPE_INVOKE:
		{
			result = SerializeInvoke(_internalBuffer, message[RM_INVOKE]);
			break;
		}
		case RM_HEADER_MESSAGETYPE_NOTIFY:
		{
			result = SerializeNotify(_internalBuffer, message[RM_NOTIFY]);
			break;
		}
		case RM_HEADER_MESSAGETYPE_FLEXSTREAMSEND:
		{
			result = SerializeFlexStreamSend(_internalBuffer, message[RM_FLEXSTREAMSEND]);
			break;
		}
		case RM_HEADER_MESSAGETYPE_SHAREDOBJECT:
		{
			result = SerializeSharedObject(_internalBuffer, message[RM_SHAREDOBJECT]);
			break;
		}
		case RM_HEADER_MESSAGETYPE_ACK:
		{
			result = SerializeAck(_internalBuffer, message[RM_ACK]);
			break;
		}
		case RM_HEADER_MESSAGETYPE_USRCTRL:
		{
			result = SerializeUsrCtrl(_internalBuffer, message[RM_USRCTRL]);
			break;
		}
		case RM_HEADER_MESSAGETYPE_CHUNKSIZE:
		{
			result = SerializeChunkSize(_internalBuffer, message[RM_CHUNKSIZE]);
			break;
		}
		case RM_HEADER_MESSAGETYPE_WINACKSIZE:
		{
			result = SerializeWinAckSize(_internalBuffer, message[RM_WINACKSIZE]);
			break;
		}
		case RM_HEADER_MESSAGETYPE_PEERBW:
		{
			result = SerializeClientBW(_internalBuffer, message[RM_PEERBW]);
			break;
		}
		case RM_HEADER_MESSAGETYPE_ABORTMESSAGE:
		{
			result = SerializeAbortMessage(_internalBuffer, message[RM_ABORTMESSAGE]);
			break;
		}
		default:
		{
			FATAL("Invalid message type:\n%s", STR(message.ToString()));
		}
	}

	//2. Check out the result
	if (!result) {
		FATAL("Unable to serialize message body");
		return false;
	}

	//3. Update the message length
	VH_ML(message) = GETAVAILABLEBYTESCOUNT(_internalBuffer);

	//4. Extract the header
	Header header;
	if (!Header::GetFromVariant(header, message[RM_HEADER])) {
		FATAL("Unable to read RTMP header: %s", STR(message.ToString()));
		return false;
	}

	//5. Chunk and send the data
	uint32_t available = 0;
	while ((available = GETAVAILABLEBYTESCOUNT(_internalBuffer)) != 0) {
		if (!header.Write(channel, buffer)) {
			FATAL("Unable to serialize message header");
			result = false;
		}
		if (available >= chunkSize) {
			buffer.ReadFromInputBuffer(&_internalBuffer, 0, chunkSize);
			channel.lastOutProcBytes += chunkSize;
			_internalBuffer.Ignore(chunkSize);
		} else {
			buffer.ReadFromInputBuffer(&_internalBuffer, 0, available);
			channel.lastOutProcBytes += available;
			_internalBuffer.Ignore(available);
		}
	}
	channel.lastOutProcBytes = 0;

	//6. Done
	return result;
}

bool RTMPProtocolSerializer::SerializeInvoke(IOBuffer &buffer,
		Variant &message) {

	string functionName = message[RM_INVOKE_FUNCTION];
	if (!_amf0.WriteShortString(buffer, functionName)) {
		FATAL("Unable to write %s", STR(RM_INVOKE_FUNCTION));
		return false;
	}

	if (!_amf0.WriteDouble(buffer, message[RM_INVOKE_ID])) {
		FATAL("Unable to write %s", STR(RM_INVOKE_ID));
		return false;
	}

	FOR_MAP(message[RM_INVOKE_PARAMS], string, Variant, i) {
		if (!_amf0.Write(buffer, MAP_VAL(i))) {
			FATAL("Unable to serialize invoke parameter %s: %s",
					STR(MAP_KEY(i)),
					STR(message.ToString()));
			return false;
		}
	}

	return true;
}

bool RTMPProtocolSerializer::SerializeNotify(IOBuffer &buffer, Variant &message) {

	FOR_MAP(message[RM_NOTIFY_PARAMS], string, Variant, i) {
		if (!_amf0.Write(buffer, MAP_VAL(i))) {
			FATAL("Unable to serialize invoke parameter %s: %s",
					STR(MAP_KEY(i)),
					STR(message.ToString()));
			return false;
		}
	}
	return true;
}

bool RTMPProtocolSerializer::SerializeFlexStreamSend(IOBuffer &buffer, Variant &message) {
	buffer.ReadFromByte((uint8_t) (message[RM_FLEXSTREAMSEND_UNKNOWNBYTE]));

	FOR_MAP(message[RM_FLEXSTREAMSEND_PARAMS], string, Variant, i) {
		if (!_amf0.Write(buffer, MAP_VAL(i))) {
			FATAL("Unable to serialize invoke parameter %s: %s",
					STR(MAP_KEY(i)),
					STR(message.ToString()));
			return false;
		}
	}
	return true;
}

bool RTMPProtocolSerializer::SerializeAck(IOBuffer &buffer, uint32_t value) {
	if (!_amf0.WriteUInt32(buffer, value, false)) {
		FATAL("Unable to write uint32_t value: %u", value);
		return false;
	}
	return true;
}

bool RTMPProtocolSerializer::SerializeUsrCtrl(IOBuffer &buffer, Variant message) {
	if (!_amf0.WriteInt16(buffer, message[RM_USRCTRL_TYPE], false)) {
		FATAL("Unable to write user control message type value");
		return false;
	}

	switch ((uint16_t) message[RM_USRCTRL_TYPE]) {
		case RM_USRCTRL_TYPE_STREAM_BEGIN:
		case RM_USRCTRL_TYPE_STREAM_EOF:
		case RM_USRCTRL_TYPE_STREAM_DRY:
		case RM_USRCTRL_TYPE_STREAM_IS_RECORDED:
		{
			if (!_amf0.WriteInt32(buffer, message[RM_USRCTRL_STREAMID], false)) {
				FATAL("Unable to write stream id from user control message");
				return false;
			}
			return true;
		}
		case RM_USRCTRL_TYPE_PING_RESPONSE:
		{
			if (!_amf0.WriteInt32(buffer, message[RM_USRCTRL_PONG], false)) {
				FATAL("Unable to write timestamp from ping response user control message");
				return false;
			}
			return true;
		}
		default:
		{
			FATAL("Invalid user control message:\n%s", STR(message.ToString()));
			return false;
		}
	}
}

bool RTMPProtocolSerializer::SerializeChunkSize(IOBuffer &buffer, uint32_t value) {
	if (!_amf0.WriteUInt32(buffer, value, false)) {
		FATAL("Unable to write uint32_t value: %u", value);
		return false;
	}
	return true;
}

bool RTMPProtocolSerializer::SerializeWinAckSize(IOBuffer &buffer, uint32_t value) {
	if (!_amf0.WriteUInt32(buffer, value, false)) {
		FATAL("Unable to write uint32_t value: %u", value);
		return false;
	}
	return true;
}

bool RTMPProtocolSerializer::SerializeAbortMessage(IOBuffer &buffer, uint32_t value) {
	if (!_amf0.WriteUInt32(buffer, value, false)) {
		FATAL("Unable to write uint32_t value: %u", value);
		return false;
	}
	return true;
}

bool RTMPProtocolSerializer::SerializeClientBW(IOBuffer &buffer, Variant value) {
	if (!_amf0.WriteUInt32(buffer, value[RM_PEERBW_VALUE], false)) {
		FATAL("Unable to write uint32_t value: %u",
				(uint32_t) value[RM_PEERBW_VALUE]);
		return false;
	}
	if (!_amf0.WriteUInt8(buffer, value[RM_PEERBW_TYPE], false)) {
		FATAL("Unable to write uint8_t value: %hhu",
				(uint8_t) value[RM_PEERBW_TYPE]);
		return false;
	}
	return true;
}

bool RTMPProtocolSerializer::SerializeSharedObject(IOBuffer &buffer,
		Variant &message) {
	string name = message[RM_SHAREDOBJECT_NAME];

	//name
	if (!_amf0.WriteShortString(buffer, name, false)) {
		FATAL("Unable to write %s", STR(RM_SHAREDOBJECT_NAME));
		return false;
	}

	//version
	if (!_amf0.WriteUInt32(buffer, message[RM_SHAREDOBJECT_VERSION], false)) {
		FATAL("Unable to write %s", STR(RM_SHAREDOBJECT_VERSION));
		return false;
	}

	//persistance
	uint32_t persistence = message[RM_SHAREDOBJECT_PERSISTENCE] ? 2 : 0;
	if (!_amf0.WriteUInt32(buffer, persistence, false)) {
		FATAL("Unable to write %s", STR(RM_SHAREDOBJECT_PERSISTENCE));
		return false;
	}

	//unknown bytes
	if (!_amf0.WriteUInt32(buffer, 0, false)) {
		FATAL("Unable to write a 0 value");
		return false;
	}

	//primitives
	for (uint32_t i = 0; i < message[RM_SHAREDOBJECT_PRIMITIVES].MapSize(); i++) {

		Variant primitive = message[RM_SHAREDOBJECT_PRIMITIVES][i];

		//type
		if (!_amf0.WriteUInt8(buffer, primitive[RM_SHAREDOBJECTPRIMITIVE_TYPE],
				false)) {
			FATAL("Unable to write %s", STR(RM_SHAREDOBJECTPRIMITIVE_TYPE));
			return false;
		}

		switch ((uint8_t) primitive[RM_SHAREDOBJECTPRIMITIVE_TYPE]) {
			case SOT_SC_UPDATE_DATA:
			case SOT_SC_INITIAL_DATA:
			{
				uint32_t rawLengthPosition = buffer.GetCurrentWritePosition();
				//length
				if (!_amf0.WriteUInt32(buffer, 0, false)) {
					FATAL("Unable to write data");
					return false;
				}

				if (!primitive.HasKey(RM_SHAREDOBJECTPRIMITIVE_PAYLOAD))
					break;

				FOR_MAP(primitive[RM_SHAREDOBJECTPRIMITIVE_PAYLOAD],
						string, Variant, i) {
					string key = MAP_KEY(i);
					Variant value = MAP_VAL(i);
					if (!_amf0.WriteShortString(buffer, key, false)) {
						FATAL("Unable to write data: %s", STR(key));
						return false;
					}
					if (!_amf0.Write(buffer, value)) {
						FATAL("Unable to write data: %s", STR(value.ToString()));
						return false;
					}
				}

				uint32_t length = buffer.GetCurrentWritePosition()
						- rawLengthPosition - 4;
				EHTONLP(buffer.GetPointer() + rawLengthPosition, length);
				break;
			}
			case SOT_SC_CLEAR_DATA:
			{
				if (!_amf0.WriteUInt32(buffer, 0, false)) {
					FATAL("Unable to write data");
					return false;
				}
				break;
			}
			case SOT_SC_DELETE_DATA:
			case SOT_SC_UPDATE_DATA_ACK:
			{
				uint32_t rawLengthPosition = buffer.GetCurrentWritePosition();
				//length
				if (!_amf0.WriteUInt32(buffer, 0, false)) {
					FATAL("Unable to write data");
					return false;
				}

				Variant &payload = primitive[RM_SHAREDOBJECTPRIMITIVE_PAYLOAD];
				for (uint32_t i = 0; i < payload.MapSize(); i++) {
					string name = payload[i];
					if (!_amf0.WriteShortString(buffer, name, false)) {
						FATAL("Unable to write data: %s", STR(name));
						return false;
					}
				}

				uint32_t length = buffer.GetCurrentWritePosition()
						- rawLengthPosition - 4;
				EHTONLP(buffer.GetPointer() + rawLengthPosition, length);
				break;
			}
			default:
			{
				FATAL("Unable to serialize primitive:\n%s",
						STR(primitive.ToString()));
				return false;
			}
		}
	}

	return true;
}

bool RTMPProtocolSerializer::DeserializeNotify(IOBuffer &buffer, Variant &message) {
	for (uint32_t i = 0; GETAVAILABLEBYTESCOUNT(buffer) > 0; i++) {
		if (!_amf0.Read(buffer, message[RM_NOTIFY_PARAMS][i])) {
			FATAL("Unable to de-serialize invoke parameter %u", i);
			return false;
		}
	}
	return true;
}

bool RTMPProtocolSerializer::DeserializeFlexStreamSend(IOBuffer &buffer, Variant &message) {
	AMF_CHECK_BOUNDARIES(buffer, 1);
	message[RM_FLEXSTREAMSEND_UNKNOWNBYTE] = (uint8_t) GETIBPOINTER(buffer)[0];
	if (!buffer.Ignore(1)) {
		FATAL("Unable to ignore 1 byte");
		return false;
	}

	for (uint32_t i = 0; GETAVAILABLEBYTESCOUNT(buffer) > 0; i++) {
		if (!_amf0.Read(buffer, message[RM_FLEXSTREAMSEND_PARAMS][i])) {
			FATAL("Unable to de-serialize invoke parameter %u", i);
			return false;
		}
	}

	return true;
}

bool RTMPProtocolSerializer::DeserializeInvoke(IOBuffer &buffer, Variant &message) {
	if (message[RM_INVOKE_IS_FLEX]) {
		if (!buffer.Ignore(1)) {
			FATAL("Unable to ignore 1 byte");
			return false;
		}
	}

	if (!_amf0.ReadShortString(buffer, message[RM_INVOKE_FUNCTION])) {
		FATAL("Unable to read %s", STR(RM_INVOKE_FUNCTION));
		return false;
	}

	if (!_amf0.ReadDouble(buffer, message[RM_INVOKE_ID])) {
		FATAL("Unable to read %s", STR(RM_INVOKE_ID));
		return false;
	}

	for (uint32_t i = 0; GETAVAILABLEBYTESCOUNT(buffer) > 0; i++) {
		if (!_amf0.Read(buffer, message[RM_INVOKE_PARAMS][i])) {
			FATAL("Unable to de-serialize invoke parameter %u", i);
			return false;
		}
	}

	return true;
}

bool RTMPProtocolSerializer::DeserializeAck(IOBuffer &buffer,
		Variant &message) {
	AMF_CHECK_BOUNDARIES(buffer, 4);
	message = (uint32_t) ENTOHLP(GETIBPOINTER(buffer)); //----MARKED-LONG---
	return buffer.Ignore(4);
}

bool RTMPProtocolSerializer::DeserializeUsrCtrl(IOBuffer &buffer, Variant &message) {
	AMF_CHECK_BOUNDARIES(buffer, 2);
	message[RM_USRCTRL_TYPE] = ENTOHSP(GETIBPOINTER(buffer)); //----MARKED-SHORT----
	message[RM_USRCTRL_TYPE_STRING] = GetUserCtrlTypeString(message[RM_USRCTRL_TYPE]);
	if (!buffer.Ignore(2)) {
		FATAL("Unable to ignore 2 bytes");
		return false;
	}

	switch ((uint16_t) message[RM_USRCTRL_TYPE]) {
		case RM_USRCTRL_TYPE_STREAM_BEGIN:
		case RM_USRCTRL_TYPE_STREAM_EOF:
		case RM_USRCTRL_TYPE_STREAM_DRY:
		case RM_USRCTRL_TYPE_STREAM_IS_RECORDED:
		{
			AMF_CHECK_BOUNDARIES(buffer, 4);
			message[RM_USRCTRL_STREAMID] = ENTOHLP(GETIBPOINTER(buffer)); //----MARKED-LONG---
			if (!buffer.Ignore(4)) {
				FATAL("Unable to ignore 4 bytes");
				return false;
			}
			return true;
		}
		case RM_USRCTRL_TYPE_STREAM_SET_BUFFER_LENGTH:
		{
			AMF_CHECK_BOUNDARIES(buffer, 8);
			message[RM_USRCTRL_STREAMID] = ENTOHLP(GETIBPOINTER(buffer)); //----MARKED-LONG---
			if (!buffer.Ignore(4)) {
				FATAL("Unable to ignore 4 bytes");
				return false;
			}
			message[RM_USRCTRL_BUFFLEN] = ENTOHLP(GETIBPOINTER(buffer)); //----MARKED-LONG---
			if (!buffer.Ignore(4)) {
				FATAL("Unable to ignore 4 bytes");
				return false;
			}
			return true;
		}
		case RM_USRCTRL_TYPE_PING_REQUEST:
		{
			AMF_CHECK_BOUNDARIES(buffer, 4);
			message[RM_USRCTRL_PING] = ENTOHLP(GETIBPOINTER(buffer)); //----MARKED-LONG---
			if (!buffer.Ignore(4)) {
				FATAL("Unable to ignore 4 bytes");
				return false;
			}
			return true;
		}
		case RM_USRCTRL_TYPE_PING_RESPONSE:
		{
			AMF_CHECK_BOUNDARIES(buffer, 4);
			message[RM_USRCTRL_PONG] = ENTOHLP(GETIBPOINTER(buffer)); //----MARKED-LONG---
			if (!buffer.Ignore(4)) {
				FATAL("Unable to ignore 4 bytes");
				return false;
			}
			return true;
		}
		case RM_USRCTRL_TYPE_UNKNOWN1:
		case RM_USRCTRL_TYPE_UNKNOWN2:
		{
			AMF_CHECK_BOUNDARIES(buffer, 4);
			message[RM_USRCTRL_UNKNOWN_U32] = ENTOHLP(GETIBPOINTER(buffer)); //----MARKED-LONG---
			if (!buffer.Ignore(4)) {
				FATAL("Unable to ignore 4 bytes");
				return false;
			}
			return true;
		}
		default:
		{
			FATAL("Invalid user control message:\n%s;Buffer:\n%s",
					STR(message.ToString()), STR(buffer));
			return false;
		}
	}
}

bool RTMPProtocolSerializer::DeserializeChunkSize(IOBuffer &buffer,
		Variant &message) {
	AMF_CHECK_BOUNDARIES(buffer, 4);
	message = (uint32_t) ENTOHLP(GETIBPOINTER(buffer)); //----MARKED-LONG---
	return buffer.Ignore(4);
}

bool RTMPProtocolSerializer::DeserializeWinAckSize(IOBuffer &buffer, Variant &message) {
	AMF_CHECK_BOUNDARIES(buffer, 4);
	message = (uint32_t) ENTOHLP(GETIBPOINTER(buffer)); //----MARKED-LONG---
	return buffer.Ignore(4);
}

bool RTMPProtocolSerializer::DeserializePeerBW(IOBuffer &buffer, Variant &message) {
	AMF_CHECK_BOUNDARIES(buffer, 4);
	message[RM_PEERBW_VALUE] = (uint32_t) ENTOHLP(GETIBPOINTER(buffer)); //----MARKED-LONG---
	if (!buffer.Ignore(4)) {
		FATAL("Unable to ignore 4 bytes");
		return false;
	}
	message[RM_PEERBW_TYPE] = *(uint8_t *) GETIBPOINTER(buffer);
	return buffer.Ignore(1);
}

bool RTMPProtocolSerializer::DeserializeAbortMessage(IOBuffer &buffer, Variant &message) {
	AMF_CHECK_BOUNDARIES(buffer, 4);
	message = (uint32_t) ENTOHLP(GETIBPOINTER(buffer)); //----MARKED-LONG---
	if (!buffer.Ignore(4)) {
		FATAL("Unable to ignore 4 bytes");
		return false;
	}
	return true;
}

bool RTMPProtocolSerializer::DeserializeFlexSharedObject(IOBuffer &buffer,
		Variant &message) {
	AMF_CHECK_BOUNDARIES(buffer, 1);
	if (GETIBPOINTER(buffer)[0] != 0) {
		FATAL("Encoding %"PRIu8" not supported yet", GETIBPOINTER(buffer)[0]);
		return false;
	}

	if (!buffer.Ignore(1)) {
		FATAL("Unable to ignore 1 bytes");
		return false;
	}

	return DeserializeSharedObject(buffer, message);
}

bool RTMPProtocolSerializer::DeserializeSharedObject(IOBuffer &buffer, Variant &message) {
	//1. Read the name
	if (!_amf0.ReadShortString(buffer, message[RM_SHAREDOBJECT_NAME], false)) {
		FATAL("Unable to read %s", STR(RM_SHAREDOBJECT_NAME));
		return false;
	}

	//2. Read the current version
	if (!_amf0.ReadUInt32(buffer, message[RM_SHAREDOBJECT_VERSION], false)) {
		FATAL("Unable to read %s", STR(RM_SHAREDOBJECT_VERSION));
		return false;
	}

	//3. Read the flags
	Variant persistence;
	if (!_amf0.ReadUInt32(buffer, persistence, false)) {
		FATAL("Unable to read %s", STR(RM_SHAREDOBJECT_PERSISTENCE));
		return false;
	}
	message[RM_SHAREDOBJECT_PERSISTENCE] = (uint32_t) persistence == 2;

	//4. Skip 4 unknown bytes
	AMF_CHECK_BOUNDARIES(buffer, 4);
	if (!buffer.Ignore(4)) {
		FATAL("Unable to ignore 4 bytes");
		return false;
	}

	//5. Read the primitives as long as we have data in the buffer
	Variant primitive;
	uint32_t primitiveIndex = 0;
	while (GETAVAILABLEBYTESCOUNT(buffer) > 0) {
		primitive.Reset();

		//6. Read the type
		if (!_amf0.ReadUInt8(buffer, primitive[RM_SHAREDOBJECTPRIMITIVE_TYPE], false)) {
			FATAL("Unable to read %s", STR(RM_SHAREDOBJECTPRIMITIVE_TYPE));
			return false;
		}
		primitive[RM_SHAREDOBJECTPRIMITIVE_STRTYPE] =
				GetSOPrimitiveString(primitive[RM_SHAREDOBJECTPRIMITIVE_TYPE]);

		//7. Read the length in bytes
		if (!_amf0.ReadUInt32(buffer, primitive[RM_SHAREDOBJECTPRIMITIVE_RAWLENGTH], false)) {
			FATAL("Unable to read %s", STR(RM_SHAREDOBJECTPRIMITIVE_RAWLENGTH));
			return false;
		}
		uint32_t rawLength = primitive[RM_SHAREDOBJECTPRIMITIVE_RAWLENGTH];
		AMF_CHECK_BOUNDARIES(buffer, rawLength);

		//8. Read the rest of the primitive based on it's type
		switch ((uint8_t) primitive[RM_SHAREDOBJECTPRIMITIVE_TYPE]) {
			case SOT_CS_CONNECT:
			{
				break;
			}
			case SOT_CS_DISCONNECT:
			{
				break;
			}
			case SOT_CS_SET_ATTRIBUTE:
			{
				uint32_t read = 0;
				uint32_t beforeRead = 0;
				uint32_t afterRead = 0;
				while (read < rawLength) {
					Variant key;
					Variant value;
					beforeRead = GETAVAILABLEBYTESCOUNT(buffer);
					if (!_amf0.ReadShortString(buffer, key, false)) {
						FATAL("Unable to read key");
						return false;
					}
					afterRead = GETAVAILABLEBYTESCOUNT(buffer);
					read += beforeRead - afterRead;

					beforeRead = GETAVAILABLEBYTESCOUNT(buffer);
					if (!_amf0.Read(buffer, value)) {
						FATAL("Unable to read value");
						return false;
					}
					afterRead = GETAVAILABLEBYTESCOUNT(buffer);
					read += beforeRead - afterRead;
					primitive[RM_SHAREDOBJECTPRIMITIVE_PAYLOAD][STR(key)] = value;
				}
				if (read != rawLength) {
					FATAL("length mismatch");
					return false;
				}
				break;
			}
			case SOT_SC_UPDATE_DATA:
			{
				NYIR;
				break;
			}
			case SOT_SC_UPDATE_DATA_ACK:
			{
				NYIR;
				break;
			}
			case SOT_BW_SEND_MESSAGE:
			{
				NYIR;
				break;
			}
			case SOT_SC_STATUS:
			{
				NYIR;
				break;
			}
			case SOT_SC_CLEAR_DATA:
			{
				NYIR;
				break;
			}
			case SOT_SC_DELETE_DATA:
			{
				NYIR;
				break;
			}
			case SOT_CSC_DELETE_DATA:
			{
				NYIR;
				break;
			}
			case SOT_SC_INITIAL_DATA:
			{
				NYIR;
				break;
			}
			default:
			{
				FATAL("Invalid SO primitive type. Partial result:\n%s",
						STR(message.ToString()));
				return false;
			}
		}

		message[RM_SHAREDOBJECT_PRIMITIVES][(uint32_t) primitiveIndex] = primitive;
		primitiveIndex++;
	}

	return true;
}

void RTMPProtocolSerializer::ChunkBuffer(IOBuffer &destination,
		IOBuffer &source, uint32_t chunkSize, Channel &channel) {
	uint32_t length = GETAVAILABLEBYTESCOUNT(source);
	uint32_t chunksCount = length / chunkSize;
	chunksCount += (length % chunkSize) == 0 ? 0 : 1;
	uint32_t copySizeChunk = 0;

	for (uint32_t i = 0; i < chunksCount - 1; i++) {
		copySizeChunk = length > chunkSize ? chunkSize : length;
		destination.ReadFromInputBuffer(&source, 0, copySizeChunk);
		if (channel.id <= 63) {
			destination.ReadFromRepeat((uint8_t) (channel.id | 0xc0), 1);
		} else {
			NYIA;
		}
		source.Ignore(copySizeChunk);
		length -= copySizeChunk;
		channel.lastOutProcBytes += length;
	}

	destination.ReadFromInputBuffer(&source, 0, length);
}
#endif /* HAS_PROTOCOL_RTMP */

