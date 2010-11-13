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


#include "protocols/protocolfactory.h"
#include "protocols/baseprotocol.h"
#include "protocols/m3u8/masterm3u8protocol.h"
#include "protocols/m3u8/childm3u8protocol.h"
#include "protocols/key/inboundkeyprotocol.h"
#include "protocols/aes/inboundaesprotocol.h"
#include "protocols/httpbuff/httpbufferprotocol.h"
#include "protocols/timer/finetimer.h"
using namespace app_applestreamingclient;

ProtocolFactory::ProtocolFactory() {

}

ProtocolFactory::~ProtocolFactory() {
}

vector<uint64_t> ProtocolFactory::HandledProtocols() {
	vector<uint64_t> result;
	ADD_VECTOR_END(result, PT_INBOUND_MASTER_M3U8);
	ADD_VECTOR_END(result, PT_INBOUND_CHILD_M3U8);
	ADD_VECTOR_END(result, PT_INBOUND_KEY);
	ADD_VECTOR_END(result, PT_HTTP_BUFF);
	ADD_VECTOR_END(result, PT_INBOUND_AES);
#ifdef HAS_MS_TIMER
	ADD_VECTOR_END(result, PT_FINE_TIMER);
#endif /* HAS_MS_TIMER */
	return result;
}

vector<string> ProtocolFactory::HandledProtocolChains() {
	vector<string> result;
#ifdef HAS_MS_TIMER
	ADD_VECTOR_END(result, PC_FINE_TIMER);
#endif /* HAS_MS_TIMER */

	ADD_VECTOR_END(result, PC_MASTER_PLAYLIST);
	ADD_VECTOR_END(result, PC_CHILD_PLAYLIST);
	ADD_VECTOR_END(result, PC_ITEM_KEY);
	ADD_VECTOR_END(result, PC_ITEM_ENC_TS);
	ADD_VECTOR_END(result, PC_ITEM_TS);
	ADD_VECTOR_END(result, PC_ITEM_ENC);
	ADD_VECTOR_END(result, PC_ITEM);
	return result;
}

vector<uint64_t> ProtocolFactory::ResolveProtocolChain(string name) {
	vector<uint64_t> result;
	if (false) {

	}
#ifdef HAS_MS_TIMER
	else if (name == PC_FINE_TIMER) {
		ADD_VECTOR_END(result, PT_UDP);
		ADD_VECTOR_END(result, PT_FINE_TIMER);
	}
#endif /* HAS_MS_TIMER */
	else if (name == PC_MASTER_PLAYLIST) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_OUTBOUND_HTTP);
		ADD_VECTOR_END(result, PT_INBOUND_MASTER_M3U8);
	} else if (name == PC_CHILD_PLAYLIST) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_OUTBOUND_HTTP);
		ADD_VECTOR_END(result, PT_INBOUND_CHILD_M3U8);
	} else if (name == PC_ITEM_KEY) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_OUTBOUND_HTTP);
		ADD_VECTOR_END(result, PT_INBOUND_KEY);
	} else if (name == PC_ITEM_ENC_TS) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_OUTBOUND_HTTP);
		ADD_VECTOR_END(result, PT_HTTP_BUFF);
		ADD_VECTOR_END(result, PT_INBOUND_AES);
		ADD_VECTOR_END(result, PT_INBOUND_TS);
	} else if (name == PC_ITEM_TS) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_OUTBOUND_HTTP);
		ADD_VECTOR_END(result, PT_HTTP_BUFF);
		ADD_VECTOR_END(result, PT_INBOUND_TS);
	} else if (name == PC_ITEM_ENC) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_OUTBOUND_HTTP);
		ADD_VECTOR_END(result, PT_HTTP_BUFF);
		ADD_VECTOR_END(result, PT_INBOUND_AES);
	} else if (name == PC_ITEM) {
		ADD_VECTOR_END(result, PT_TCP);
		ADD_VECTOR_END(result, PT_OUTBOUND_HTTP);
		ADD_VECTOR_END(result, PT_HTTP_BUFF);
	} else {
		ASSERT("This protocol stack should not land here");
	}
	return result;
}

BaseProtocol *ProtocolFactory::SpawnProtocol(uint64_t type, Variant &parameters) {
	BaseProtocol *pResult = NULL;
	switch (type) {
#ifdef HAS_MS_TIMER
		case PT_FINE_TIMER:
			pResult = new FineTimer();
			break;
#endif /* HAS_MS_TIMER */
		case PT_INBOUND_MASTER_M3U8:
			pResult = new MasterM3U8Protocol();
			break;
		case PT_INBOUND_CHILD_M3U8:
			pResult = new ChildM3U8Protocol();
			break;
		case PT_INBOUND_KEY:
			pResult = new InboundKeyProtocol();
			break;
		case PT_HTTP_BUFF:
			pResult = new HTTPBufferProtocol();
			break;
		case PT_INBOUND_AES:
			pResult = new InboundAESProtocol();
			break;
		default:
			FATAL("Spawning protocol %s not yet implemented",
					STR(tagToString(type)));
			break;
	}

	if (pResult != NULL) {
		if (!pResult->Initialize(parameters)) {
			FATAL("Unable to initialize protocol %s",
					STR(tagToString(type)));
			delete pResult;
			pResult = NULL;
		}
	}

	return pResult;
}

