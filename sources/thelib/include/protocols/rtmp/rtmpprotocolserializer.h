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
#ifndef _RTMPPROTOCOLSERIALIZER_H
#define	_RTMPPROTOCOLSERIALIZER_H


#include "protocols/rtmp/amf0serializer.h"
#include "protocols/rtmp/amf3serializer.h"
#include "protocols/rtmp/channel.h"

class DLLEXP RTMPProtocolSerializer {
private:
	AMF0Serializer _amf0;
	AMF3Serializer _amf3;
	IOBuffer _internalBuffer;
public:
	RTMPProtocolSerializer();
	virtual ~RTMPProtocolSerializer();

	static string GetUserCtrlTypeString(uint16_t type);
	static string GetSOPrimitiveString(uint8_t type);
	bool Deserialize(Header &header, IOBuffer &buffer, Variant &message);
	bool Serialize(Channel &channel, Variant &message,
			IOBuffer &buffer, uint32_t chunkSize);
private:
	bool SerializeInvoke(IOBuffer &buffer, Variant &message);
	bool SerializeNotify(IOBuffer &buffer, Variant &message);
	bool SerializeFlexStreamSend(IOBuffer &buffer, Variant &message);
	bool SerializeAck(IOBuffer &buffer, uint32_t value);
	bool SerializeUsrCtrl(IOBuffer &buffer, Variant message);
	bool SerializeChunkSize(IOBuffer &buffer, uint32_t value);
	bool SerializeWinAckSize(IOBuffer &buffer, uint32_t value);
	bool SerializeAbortMessage(IOBuffer &buffer, uint32_t value);
	bool SerializeClientBW(IOBuffer &buffer, Variant value);
	bool SerializeSharedObject(IOBuffer &buffer, Variant &message);


	bool DeserializeNotify(IOBuffer &buffer, Variant &message);
	bool DeserializeFlexStreamSend(IOBuffer &buffer, Variant &message);
	bool DeserializeInvoke(IOBuffer &buffer, Variant &message);
	bool DeserializeAck(IOBuffer &buffer, Variant &message);
	bool DeserializeUsrCtrl(IOBuffer &buffer, Variant &message);
	bool DeserializeChunkSize(IOBuffer &buffer, Variant &message);
	bool DeserializeWinAckSize(IOBuffer &buffer, Variant &message);
	bool DeserializePeerBW(IOBuffer &buffer, Variant &message);
	bool DeserializeAbortMessage(IOBuffer &buffer, Variant &message);
	bool DeserializeFlexSharedObject(IOBuffer &buffer, Variant &message);
	bool DeserializeSharedObject(IOBuffer &buffer, Variant &message);


	void ChunkBuffer(IOBuffer &destination, IOBuffer &source,
			uint32_t chunkSize, Channel &channel);
};


#endif	/* _RTMPPROTOCOLSERIALIZER_H */

#endif /* HAS_PROTOCOL_RTMP */

