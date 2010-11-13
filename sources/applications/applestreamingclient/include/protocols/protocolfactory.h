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


#ifndef _PROTOCOLFACTORY_H
#define	_PROTOCOLFACTORY_H

#include "protocols/baseprotocolfactory.h"
#include "protocols/protocoltypes.h"

#define PT_INBOUND_MASTER_M3U8	MAKE_TAG6('I','M','M','3','U','8')
#define PT_INBOUND_CHILD_M3U8	MAKE_TAG6('I','C','M','3','U','8')
#define PT_INBOUND_KEY			MAKE_TAG4('I','K','E','Y')
#define PT_HTTP_BUFF			MAKE_TAG5('H','B','U','F','F')
#define PT_INBOUND_AES			MAKE_TAG4('I','A','E','S')

#define PC_MASTER_PLAYLIST	"outboundHttpInboundMasterM3U8"
#define PC_CHILD_PLAYLIST	"outboundHttpInboundChildM3U8"
#define PC_ITEM_KEY			"outboundHttpInboundKey"
#define PC_ITEM_ENC_TS		"bufferedHttpEncTS"
#define PC_ITEM_TS			"bufferedHttpTS"
#define PC_ITEM_ENC			"bufferedHttpEnc"
#define PC_ITEM				"bufferedHttp"

#ifdef HAS_MS_TIMER
#define PT_FINE_TIMER		MAKE_TAG4('F','T','M','R')
#define PC_FINE_TIMER		"fineTimer"
#endif /* HAS_MS_TIMER */

namespace app_applestreamingclient {

	class DLLEXP ProtocolFactory
	: public BaseProtocolFactory {
	public:
		ProtocolFactory();
		virtual ~ProtocolFactory();

		virtual vector<uint64_t> HandledProtocols();
		virtual vector<string> HandledProtocolChains();
		virtual vector<uint64_t> ResolveProtocolChain(string name);
		virtual BaseProtocol *SpawnProtocol(uint64_t type, Variant &parameters);
	};
}

#endif	/* _PROTOCOLFACTORY_H */
