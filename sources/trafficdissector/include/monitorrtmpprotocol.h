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

#ifndef _MONITORRTMPPROTOCOL_H
#define	_MONITORRTMPPROTOCOL_H

#include "protocols/rtmp/basertmpprotocol.h"

class DLLEXP MonitorRTMPProtocol
: public BaseRTMPProtocol {
private:
	uint8_t _handshakeStage;
	bool _isClient;
public:
	MonitorRTMPProtocol(bool isClient);
	virtual ~MonitorRTMPProtocol();

	uint8_t GetLogLevel();
	virtual bool Initialize(Variant &parameters);
	virtual bool PerformHandshake(IOBuffer &buffer);
	bool SetInboundChunkSize(uint32_t chunkSize);
	bool SetOutboundChunkSize(uint32_t chunkSize);
private:
	bool PerformHandshakeClient(IOBuffer &buffer);
	bool PerformHandshakeServer(IOBuffer &buffer);
};


#endif	/* _MONITORRTMPPROTOCOL_H */

