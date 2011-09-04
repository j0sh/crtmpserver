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
#ifndef _MONITORRTMPPROTOCOL_H
#define	_MONITORRTMPPROTOCOL_H

#include "protocols/baseprotocol.h"
#include "protocols/rtmp/channel.h"
#include "protocols/rtmp/rtmpprotocolserializer.h"
#include "streaming/rtmpstream.h"

class DLLEXP MonitorRTMPProtocol
: public BaseProtocol {
protected:
	Channel *_channels;
	int32_t _selectedChannel;
	uint32_t _inboundChunkSize;
	RTMPProtocolSerializer _rtmpProtocolSerializer;
	IOBuffer _input;
	uint32_t _maxStreamCount;
	uint32_t _maxChannelsCount;
public:
	MonitorRTMPProtocol(uint32_t maxStreamIndex, uint32_t maxChannelIndex);
	virtual ~MonitorRTMPProtocol();

	virtual bool Initialize(Variant &parameters);
	virtual bool AllowFarProtocol(uint64_t type);
	virtual bool AllowNearProtocol(uint64_t type);
	virtual bool SignalInputData(int32_t recvAmount);
	virtual bool SignalInputData(IOBuffer &buffer);

	bool SetInboundChunkSize(uint32_t chunkSize);

	bool Feed(IOBuffer &buffer);
private:
	bool ProcessBytes(IOBuffer &buffer);

};


#endif	/* _MONITORRTMPPROTOCOL_H */

#endif /* HAS_PROTOCOL_RTMP */

