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
#include "streaming/basestream.h"
#include "application/clientapplicationmanager.h"
#include "protocols/ts/inboundtsprotocol.h"
#include "protocols/ts/tspacketpat.h"
#include "protocols/ts/tspacketpmt.h"
#include "protocols/ts/basetsappprotocolhandler.h"
#include "protocols/ts/innettsstream.h"
#include "protocols/ts/tsboundscheck.h"
#include "protocols/rtmp/header_le_ba.h"

InboundTSProtocol::InboundTSProtocol()
: BaseProtocol(PT_INBOUND_TS) {
	//1. Set the chunk size
	_chunkSize = 0;

	//2. Setup the PAT pid
	PIDDescriptor *pPAT = new PIDDescriptor;
	pPAT->type = PID_TYPE_PAT;
	pPAT->pid = 0;
	pPAT->payload.crc = 0;
	pPAT->payload.pStream = NULL;
	_pidMapping[0] = pPAT;

	//3. Setup CAT table
	PIDDescriptor *pCAT = new PIDDescriptor;
	pCAT->type = PID_TYPE_CAT;
	pCAT->pid = 1;
	pCAT->payload.crc = 0;
	pCAT->payload.pStream = NULL;
	_pidMapping[1] = pCAT;

	//4. Setup TSDT table
	PIDDescriptor *pTSDT = new PIDDescriptor;
	pTSDT->type = PID_TYPE_TSDT;
	pTSDT->pid = 2;
	pTSDT->payload.crc = 0;
	pTSDT->payload.pStream = NULL;
	_pidMapping[2] = pTSDT;

	//5. Setup reserved tables
	for (uint16_t i = 3; i < 16; i++) {
		PIDDescriptor *pReserved = new PIDDescriptor;
		pReserved->type = PID_TYPE_RESERVED;
		pReserved->pid = i;
		pReserved->payload.crc = 0;
		pReserved->payload.pStream = NULL;
		_pidMapping[i] = pReserved;
	}

	//6. Setup the NULL pid
	PIDDescriptor *pNULL = new PIDDescriptor;
	pNULL->type = PID_TYPE_NULL;
	pNULL->pid = 0x1fff;
	pNULL->payload.crc = 0;
	pNULL->payload.pStream = NULL;
	_pidMapping[0x1fff] = pNULL;

	_pProtocolHandler = NULL;
	_chunkSizeDetectionCount = 0;

	_stepByStep = false;
}

InboundTSProtocol::~InboundTSProtocol() {
	//1. Cleanup the pid mappings

	FOR_MAP(_pidMapping, uint16_t, PIDDescriptor *, i) {
		FreePidDescriptor(MAP_VAL(i));
	}
	_pidMapping.clear();
}

bool InboundTSProtocol::Initialize(Variant &parameters) {
	GetCustomParameters() = parameters;
	return true;
}

bool InboundTSProtocol::AllowFarProtocol(uint64_t type) {
	return true;
}

bool InboundTSProtocol::AllowNearProtocol(uint64_t type) {
	FATAL("This protocol doesn't allow any near protocols");
	return false;
}

bool InboundTSProtocol::SignalInputData(int32_t recvAmount) {
	ASSERT("OPERATION NOT SUPPORTED");
	return false;
}

bool InboundTSProtocol::SignalInputData(IOBuffer &buffer, sockaddr_in *pPeerAddress) {
	return SignalInputData(buffer);
}

bool InboundTSProtocol::SignalInputData(IOBuffer &buffer) {
	if (_chunkSize == 0) {
		if (!DetermineChunkSize(buffer)) {
			FATAL("Unable to determine chunk size");
			return false;
		}
	}

	if (_chunkSize == 0) {
		return true;
	}

	//TODO: Ignore the buffer only once. Also apply the same approach on RTMP
	//if possible
	while (GETAVAILABLEBYTESCOUNT(buffer) >= _chunkSize + 1) {
		if (GETIBPOINTER(buffer)[_chunkSize] != 0x47) {
			WARN("Bogus chunk. Ignore all the buffer and reset the chunk size");
			buffer.IgnoreAll();
			_chunkSizeDetectionCount = 0;
			_chunkSize = 0;
			return true;
		}

		uint32_t packetHeader = ENTOHLP(GETIBPOINTER(buffer));

		if (!ProcessPacket(packetHeader, buffer, _chunkSize)) {
			FATAL("Unable to process packet");
			return false;
		}

		if (!buffer.Ignore(_chunkSize)) {
			FATAL("Unable to ignore %u bytes", _chunkSize);
		}

		buffer.MoveData();

		if (_stepByStep)
			return true;
	}

	return true;
}

void InboundTSProtocol::SetApplication(BaseClientApplication *pApplication) {
	BaseProtocol::SetApplication(pApplication);
	if (pApplication != NULL) {
		_pProtocolHandler = (BaseTSAppProtocolHandler *)
				pApplication->GetProtocolHandler(this);
	} else {
		_pProtocolHandler = NULL;
	}
}

BaseTSAppProtocolHandler *InboundTSProtocol::GetProtocolHandler() {
	return _pProtocolHandler;
}

uint32_t InboundTSProtocol::GetChunkSize() {
	return _chunkSize;
}

void InboundTSProtocol::SetStepByStep(bool stepByStep) {
	_stepByStep = stepByStep;
}

void InboundTSProtocol::FreePidDescriptor(PIDDescriptor *pPIDDescriptor) {
	if (pPIDDescriptor->type == PID_TYPE_AUDIOSTREAM
			|| pPIDDescriptor->type == PID_TYPE_VIDEOSTREAM) {
		if (pPIDDescriptor->payload.pStream != NULL) {
			delete pPIDDescriptor->payload.pStream;
		}
	}
	delete pPIDDescriptor;
}

bool InboundTSProtocol::DetermineChunkSize(IOBuffer &buffer) {
	while (GETAVAILABLEBYTESCOUNT(buffer) >= 3 * TS_CHUNK_208 + 2) {
		if (_chunkSizeDetectionCount >= TS_CHUNK_208) {
			FATAL("I give up. I'm unable to detect the ts chunk size");
			return false;
		}
		if (GETIBPOINTER(buffer)[0] != 0x47) {
			_chunkSizeDetectionCount++;
			buffer.Ignore(1);
			continue;
		}

		if ((GETIBPOINTER(buffer)[TS_CHUNK_188] == 0x47)
				&& (GETIBPOINTER(buffer)[2 * TS_CHUNK_188] == 0x47)) {
			_chunkSize = TS_CHUNK_188;
			return true;
		} else if ((GETIBPOINTER(buffer)[TS_CHUNK_204] == 0x47)
				&& (GETIBPOINTER(buffer)[2 * TS_CHUNK_204] == 0x47)) {
			_chunkSize = TS_CHUNK_204;
			return true;
		} else if ((GETIBPOINTER(buffer)[TS_CHUNK_208] == 0x47)
				&& (GETIBPOINTER(buffer)[2 * TS_CHUNK_208] == 0x47)) {
			_chunkSize = TS_CHUNK_208;
			return true;
		} else {
			_chunkSizeDetectionCount++;
			buffer.Ignore(1);
		}
	}
	return true;
}

bool InboundTSProtocol::ProcessPacket(uint32_t packetHeader,
		IOBuffer &buffer, uint32_t maxCursor) {
	//1. Get the PID descriptor or create it if absent
	PIDDescriptor *pPIDDescriptor = NULL;
	if (MAP_HAS1(_pidMapping, TS_TRANSPORT_PACKET_PID(packetHeader))) {
		pPIDDescriptor = _pidMapping[TS_TRANSPORT_PACKET_PID(packetHeader)];
	} else {
		pPIDDescriptor = new PIDDescriptor;
		pPIDDescriptor->type = PID_TYPE_UNKNOWN;
		pPIDDescriptor->pid = TS_TRANSPORT_PACKET_PID(packetHeader);
		_pidMapping[pPIDDescriptor->pid] = pPIDDescriptor;
	}

	//2. Skip the transport packet structure
	uint8_t *pBuffer = GETIBPOINTER(buffer);
	uint32_t cursor = 4;

	if (TS_TRANSPORT_PACKET_HAS_ADAPTATION_FIELD(packetHeader)) {
		CHECK_BOUNDS(1);
		CHECK_BOUNDS(pBuffer[cursor]);
		cursor += pBuffer[cursor] + 1;
	}
	if (!TS_TRANSPORT_PACKET_HAS_PAYLOAD(packetHeader)) {
		return true;
	}

	switch (pPIDDescriptor->type) {
		case PID_TYPE_PAT:
		{
			return ProcessPidTypePAT(packetHeader, *pPIDDescriptor, pBuffer, cursor, maxCursor);
		}
		case PID_TYPE_PMT:
		{
			return ProcessPidTypePMT(packetHeader, *pPIDDescriptor, pBuffer,
					cursor, maxCursor);
		}
		case PID_TYPE_AUDIOSTREAM:
		{
			return pPIDDescriptor->payload.pStream->FeedData(pBuffer + cursor,
					_chunkSize - cursor,
					TS_TRANSPORT_PACKET_IS_PAYLOAD_START(packetHeader), true,
					(int8_t) packetHeader & 0x0f);
		}
		case PID_TYPE_VIDEOSTREAM:
		{
			return pPIDDescriptor->payload.pStream->FeedData(pBuffer + cursor,
					_chunkSize - cursor,
					TS_TRANSPORT_PACKET_IS_PAYLOAD_START(packetHeader), false,
					(int8_t) packetHeader & 0x0f);
		}
		case PID_TYPE_RESERVED:
		{
			WARN("This PID %hu should not be used because is reserved according to iso13818-1.pdf", pPIDDescriptor->pid);
			return true;
		}
		case PID_TYPE_UNKNOWN:
		{
			if (!MAP_HAS1(_unknownPids, pPIDDescriptor->pid)) {
				WARN("PID %hu not known yet", pPIDDescriptor->pid);
				_unknownPids[pPIDDescriptor->pid] = pPIDDescriptor->pid;
			}
			return true;

		}
		case PID_TYPE_NULL:
		{
			//Ignoring NULL pids
			return true;
		}
		default:
		{
			WARN("PID type not implemented: %hhu. Pid number: %hu",
					pPIDDescriptor->type, pPIDDescriptor->pid);
			return false;
		}
	}
}

bool InboundTSProtocol::ProcessPidTypePAT(uint32_t packetHeader,
		PIDDescriptor &pidDescriptor, uint8_t *pBuffer, uint32_t &cursor,
		uint32_t maxCursor) {
	//1. Advance the pointer field
	if (TS_TRANSPORT_PACKET_IS_PAYLOAD_START(packetHeader)) {
		CHECK_BOUNDS(1);
		CHECK_BOUNDS(pBuffer[cursor]);
		cursor += pBuffer[cursor] + 1;
	}

	//1. Get the crc from the packet and compare it with the last crc.
	//if it is the same, ignore this packet
	uint32_t crc = TSPacketPAT::PeekCRC(pBuffer, cursor, maxCursor);
	if (crc == 0) {
		FATAL("Unable to read crc");
		return false;
	}
	if (pidDescriptor.payload.crc == crc) {
		return true;
	}

	//2. read the packet
	TSPacketPAT packetPAT;
	if (!packetPAT.Read(pBuffer, cursor, maxCursor)) {
		FATAL("Unable to read PAT");
		return false;
	}

	//3. Store the crc
	pidDescriptor.payload.crc = packetPAT.GetCRC();


	//4. Store the pid types found in PAT

	FOR_MAP(packetPAT.GetPMTs(), uint16_t, uint16_t, i) {
		PIDDescriptor *pDescr = new PIDDescriptor;
		pDescr->pid = MAP_VAL(i);
		pDescr->type = PID_TYPE_PMT;
		pDescr->payload.crc = 0;
		_pidMapping[pDescr->pid] = pDescr;
	}

	FOR_MAP(packetPAT.GetNITs(), uint16_t, uint16_t, i) {
		PIDDescriptor *pDescr = new PIDDescriptor;
		pDescr->pid = MAP_VAL(i);
		pDescr->type = PID_TYPE_NIT;
		_pidMapping[pDescr->pid] = pDescr;
	}

	//5. Done
	return true;
}

bool InboundTSProtocol::ProcessPidTypePMT(uint32_t packetHeader,
		PIDDescriptor &pidDescriptor, uint8_t *pBuffer, uint32_t &cursor,
		uint32_t maxCursor) {
	//1. Advance the pointer field
	if (TS_TRANSPORT_PACKET_IS_PAYLOAD_START(packetHeader)) {
		CHECK_BOUNDS(1);
		CHECK_BOUNDS(pBuffer[cursor]);
		cursor += pBuffer[cursor] + 1;
	}

	//1. Get the crc from the packet and compare it with the last crc.
	//if it is the same, ignore this packet. Also test if we have a protocol handler
	uint32_t crc = TSPacketPMT::PeekCRC(pBuffer, cursor, maxCursor);
	if (crc == 0) {
		FATAL("Unable to read crc");
		return false;
	}
	if (pidDescriptor.payload.crc == crc) {
		return true;
	}
	if (_pProtocolHandler == NULL) {
		FATAL("This protocol is not yet registered with a protocol handler");
		return false;
	}

	//2. read the packet
	TSPacketPMT packetPMT;
	if (!packetPMT.Read(pBuffer, cursor, maxCursor)) {
		FATAL("Unable to read PAT");
		return false;
	}

	//3. Store the CRC
	pidDescriptor.payload.crc = packetPMT.GetCRC();

	//4. Gather the info about the streams present inside the program
	//videoPid will contain the selected video stream
	//audioPid will contain the selected audio stream
	//unknownPids will contain the rest of the streams that will be ignored
	map<uint16_t, uint16_t> unknownPids;
	uint16_t videoPid = 0;
	uint16_t audioPid = 0;

	map<uint16_t, TSStreamInfo> streamsInfo = packetPMT.GetStreamsInfo();

	FOR_MAP(streamsInfo, uint16_t, TSStreamInfo, i) {
		switch (MAP_VAL(i).streamType) {
			case TS_STREAMTYPE_VIDEO_H264:
			{
				if (videoPid != 0) {
					unknownPids[MAP_KEY(i)] = MAP_KEY(i);
					WARN("Multiple video streams per program detected");
					break;
				}
				videoPid = MAP_KEY(i);
				break;
			}
			case TS_STREAMTYPE_AUDIO_AAC:
			{
				if (audioPid != 0) {
					unknownPids[MAP_KEY(i)] = MAP_KEY(i);
					WARN("Multiple audio streams per program detected");
					break;
				}
				audioPid = MAP_KEY(i);
				break;
			}
			default:
			{
				unknownPids[MAP_KEY(i)] = MAP_KEY(i);
				WARN("stream type %hhu not supported yet", MAP_VAL(i).streamType);
				break;
			}
		}
	}

	//4. Compute the stream name
	string streamName = "";
	if (GetCustomParameters().HasKeyChain(V_STRING, true, 1, "localStreamName"))
		streamName = (string) GetCustomParameters()["localStreamName"];
	else
		streamName = format("ts_%u_%hu_%hu", GetId(), audioPid, videoPid);

	//4. Create the stream if we have at least videoPid or audioPid
	InNetTSStream *pStream = NULL;
	if ((videoPid != 0) || (audioPid != 0)) {
		if (!GetApplication()->StreamNameAvailable(streamName, this)) {
			FATAL("Stream name %s already taken", STR(streamName));
			return false;
		}
		pStream = new InNetTSStream(this, GetApplication()->GetStreamsManager(),
				streamName, packetPMT.GetBandwidth());
	}

	//5. Create the pid descriptors for audioPid and videoPid and store them
	//inside the pid descriptors vector
	vector<PIDDescriptor *> pidDescriptors;
	PIDDescriptor *pPIDDescriptor = NULL;
	PIDDescriptor *pAudioPidDescriptor = NULL;
	PIDDescriptor *pVideoPidDescriptor = NULL;
	if (audioPid) {
		pAudioPidDescriptor = new PIDDescriptor();
		pAudioPidDescriptor->pid = audioPid;
		pAudioPidDescriptor->type = PID_TYPE_AUDIOSTREAM;
		pAudioPidDescriptor->payload.pStream = pStream;
		ADD_VECTOR_END(pidDescriptors, pAudioPidDescriptor);
	}
	if (videoPid) {
		pVideoPidDescriptor = new PIDDescriptor();
		pVideoPidDescriptor->pid = videoPid;
		pVideoPidDescriptor->type = PID_TYPE_VIDEOSTREAM;
		pVideoPidDescriptor->payload.pStream = pStream;
		ADD_VECTOR_END(pidDescriptors, pVideoPidDescriptor);
	}
	if (pStream != NULL) {
		pStream->SetAudioVideoPidDescriptors(pAudioPidDescriptor,
				pVideoPidDescriptor);
	}

	//6. create the pid descriptors for the unknown pids

	FOR_MAP(unknownPids, uint16_t, uint16_t, i) {
		pPIDDescriptor = new PIDDescriptor();
		pPIDDescriptor->pid = MAP_KEY(i);
		pPIDDescriptor->type = PID_TYPE_NULL;
		pPIDDescriptor->payload.pStream = NULL;
		ADD_VECTOR_END(pidDescriptors, pPIDDescriptor);
	}

	//7. Mount the newly created pids

	FOR_VECTOR(pidDescriptors, i) {
		if (MAP_HAS1(_pidMapping, pidDescriptors[i]->pid)) {
			FreePidDescriptor(_pidMapping[pidDescriptors[i]->pid]);
		}
		_pidMapping[pidDescriptors[i]->pid] = pidDescriptors[i];
	}

	//8. Notify the protocol handler about the presence of the new stream
	if (_pProtocolHandler != NULL) {
		if (pStream != NULL) {
			_pProtocolHandler->ProgramSetupCompleted(pStream);
		}
	}

	return true;
}
#endif	/* HAS_PROTOCOL_TS */

