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


#ifdef HAS_PROTOCOL_TS
#ifndef _INBOUNDTSPROTOCOL_H
#define	_INBOUNDTSPROTOCOL_H

#include "protocols/baseprotocol.h"
#include "protocols/ts/pidtypes.h"
#include "protocols/ts/tspacketheader.h"

#define TS_CHUNK_188 188
#define TS_CHUNK_204 204
#define TS_CHUNK_208 208

class TSProgram;
class TSBaseStream;
class TSVideoStream;
class TSAudioStream;
class TSSubtitleStream;
class BaseTSAppProtocolHandler;
class InNetTSStream;

typedef struct _PIDDescriptor {
	PIDType type;
	uint16_t pid;

	union {
		uint32_t crc;
		InNetTSStream *pStream;
	} payload;
} PIDDescriptor;

class DLLEXP InboundTSProtocol
: public BaseProtocol {
private:
	uint32_t _chunkSizeDetectionCount;
	uint32_t _chunkSize;
	map<uint16_t, PIDDescriptor *> _pidMapping;
	BaseTSAppProtocolHandler *_pProtocolHandler;
	bool _stepByStep;
	map<uint16_t, uint16_t> _unknownPids;
public:
	InboundTSProtocol();
	virtual ~InboundTSProtocol();

	virtual bool Initialize(Variant &parameters);
	virtual bool AllowFarProtocol(uint64_t type);
	virtual bool AllowNearProtocol(uint64_t type);
	virtual bool SignalInputData(int32_t recvAmount);
	virtual bool SignalInputData(IOBuffer &buffer, sockaddr_in *pPeerAddress);
	virtual bool SignalInputData(IOBuffer &buffer);

	virtual void SetApplication(BaseClientApplication *pApplication);
	BaseTSAppProtocolHandler *GetProtocolHandler();
	uint32_t GetChunkSize();
	void SetStepByStep(bool stepByStep);
private:
	void FreePidDescriptor(PIDDescriptor *pPIDDescriptor);
	bool DetermineChunkSize(IOBuffer &buffer);
	bool ProcessPacket(uint32_t packetHeader, IOBuffer &buffer, uint32_t maxCursor);
	bool ProcessPidTypePAT(uint32_t packetHeader,
			PIDDescriptor &pidDescriptor, uint8_t *pBuffer, uint32_t &cursor,
			uint32_t maxCursor);
	bool ProcessPidTypePMT(uint32_t packetHeader, PIDDescriptor &pidDescriptor,
			uint8_t *pBuffer, uint32_t &cursor, uint32_t maxCursor);
};


#endif	/* _INBOUNDTSPROTOCOL_H */
#endif	/* HAS_PROTOCOL_TS */

