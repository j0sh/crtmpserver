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
#include "buffering/iobuffer.h"
#include "application/clientapplicationmanager.h"
#include "protocols/ts/inboundtsprotocol.h"
#include "protocols/ts/tspacketpat.h"
#include "protocols/ts/tspacketpmt.h"
#include "protocols/ts/tsadaptationfield.h"
#include "protocols/ts/basetsappprotocolhandler.h"
#include "protocols/ts/innettsstream.h"

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



	//3. Setup the NULL pid
	PIDDescriptor *pNULL = new PIDDescriptor;
	pNULL->type = PID_TYPE_NULL;
	pNULL->pid = 0x1fff;
	pNULL->payload.crc = 0;
	pNULL->payload.pStream = NULL;
	_pidMapping[0x1fff] = pNULL;

	//TODO. Also take care of some other reserved pids presented here:
	//iso13818-1.pdf page 37/174
	//Table 2-3 – PID table

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
	//    if (type == PT_TCP ||
	//            type == PT_OUTBOUNDHTTP)
	//        return true;
	//    FATAL("Far protocol %d not accepted", type);
	//    return false;
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

		TSPacketHeader packetHeader;
		if (!packetHeader.Read(buffer)) {
			FATAL("Unable to read packet");
			return false;
		}

		if (!ProcessPacket(packetHeader, buffer, _chunkSize)) {
			FATAL("Unable to process packet:\n%s", STR(buffer));
			return false;
		}

		if (!buffer.Ignore(_chunkSize)) {
			FATAL("Unable to ignore %d bytes", _chunkSize);
		}

		buffer.MoveData();

		if (_stepByStep)
			return true;
	}

	return true;
}

void InboundTSProtocol::SetApplication(BaseClientApplication *pApplication) {
	assert(pApplication != NULL);
	BaseProtocol::SetApplication(pApplication);
	_pProtocolHandler = (BaseTSAppProtocolHandler *)
			_pApplication->GetProtocolHandler(this);
}

void InboundTSProtocol::ResetApplication() {
	BaseProtocol::ResetApplication();
	_pProtocolHandler = NULL;
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

bool InboundTSProtocol::ProcessPacket(TSPacketHeader &packetHeader,
		IOBuffer &buffer, uint32_t maxCursor) {
	PIDDescriptor *pPIDDescriptor = NULL;
	if (MAP_HAS1(_pidMapping, packetHeader._header.data.pid)) {
		pPIDDescriptor = _pidMapping[packetHeader._header.data.pid];
	} else {
		pPIDDescriptor = new PIDDescriptor;
		pPIDDescriptor->type = PID_TYPE_UNKNOWN;
		pPIDDescriptor->pid = packetHeader._header.data.pid;
		_pidMapping[pPIDDescriptor->pid] = pPIDDescriptor;
	}

	uint8_t *pBuffer = GETIBPOINTER(buffer);
	uint32_t cursor = 4;

	if (packetHeader.HasAdaptationField()) {
		if (!_tsAdaptationField.Read(pBuffer, cursor, maxCursor)) {
			FATAL("Unable to read adaptation field");
			return false;
		}
		//FINEST("_tsAdaptationField:\n%s", STR(_tsAdaptationField));
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
					_chunkSize - cursor, packetHeader.IsPayloadStart(), true);
			//            return pPIDDescriptor->payload.pAudioStream->FeedData(
			//                    &_tsAdaptationField, pBuffer + cursor, _chunkSize - cursor,
			//                    packetHeader.IsPayloadStart());
		}
		case PID_TYPE_VIDEOSTREAM:
		{
			return pPIDDescriptor->payload.pStream->FeedData(pBuffer + cursor,
					_chunkSize - cursor, packetHeader.IsPayloadStart(), false);
			//            return pPIDDescriptor->payload.pVideoStream->FeedData(
			//                    &_tsAdaptationField, pBuffer + cursor, _chunkSize - cursor,
			//                    packetHeader.IsPayloadStart());
		}
		case PID_TYPE_UNKNOWN:
		{
			WARN("PID %d not known yet", pPIDDescriptor->pid);
			return true;
		}
		case PID_TYPE_NULL:
		{
			//Ignoring NULL pids
			return true;
		}
		default:
		{
			WARN("PID type not implemented: %d", pPIDDescriptor->type);
			return false;
		}
	}
}

bool InboundTSProtocol::ProcessPidTypePAT(TSPacketHeader &packetHeader,
		PIDDescriptor &pidDescriptor, uint8_t *pBuffer, uint32_t &cursor,
		uint32_t maxCursor) {

	//1. Get the crc from the packet and compare it with the last crc.
	//if it is the same, ignore this packet
	uint32_t crc = TSPacketPAT::PeekCRC(pBuffer, cursor,
			packetHeader.IsPayloadStart(), maxCursor);
	if (crc == 0) {
		FATAL("Unable to read crc");
		return false;
	}
	if (pidDescriptor.payload.crc == crc) {
		return true;
	}

	//2. read the packet
	TSPacketPAT packetPAT;
	if (!packetPAT.Read(pBuffer, cursor,
			packetHeader.IsPayloadStart(), maxCursor)) {
		FATAL("Unable to read PAT");
		return false;
	}
	FINEST("packetPAT:\n%s", STR(packetPAT));

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

bool InboundTSProtocol::ProcessPidTypePMT(TSPacketHeader &packetHeader,
		PIDDescriptor &pidDescriptor, uint8_t *pBuffer, uint32_t &cursor,
		uint32_t maxCursor) {

	//1. Get the crc from the packet and compare it with the last crc.
	//if it is the same, ignore this packet. Also test if we have a protocol handler
	uint32_t crc = TSPacketPMT::PeekCRC(pBuffer, cursor,
			packetHeader.IsPayloadStart(), maxCursor);
	if (crc == 0) {
		FATAL("Unable to read crc");
		return false;
	}
	if (pidDescriptor.payload.crc == crc) {
		return true;
	}
	FINEST("crc: %08x != %08x", pidDescriptor.payload.crc, crc);
	if (_pProtocolHandler == NULL) {
		FATAL("This protocol is not yet registered with a protocol handler");
		return false;
	}

	//2. read the packet
	TSPacketPMT packetPMT;
	if (!packetPMT.Read(pBuffer, cursor,
			packetHeader.IsPayloadStart(), maxCursor)) {
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
				WARN("stream type %u not supported yet", MAP_VAL(i).streamType);
				break;
			}
		}
	}

	//4. Create the stream if we have at least videoPid or audioPid
	InNetTSStream *pStream = NULL;
	if ((videoPid != 0) || (audioPid != 0)) {
		pStream = new InNetTSStream(this, GetApplication()->GetStreamsManager(),
				format("ts_%d_%d_%d", GetId(), audioPid, videoPid));
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

