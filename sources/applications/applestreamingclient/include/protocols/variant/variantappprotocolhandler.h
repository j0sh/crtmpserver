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

#ifndef _VARIANTAPPPROTOCOLHANDLER_H
#define	_VARIANTAPPPROTOCOLHANDLER_H

#include "protocols/variant/basevariantappprotocolhandler.h"

namespace app_applestreamingclient {
	class ClientContext;

	class VariantAppProtocolHandler
	: public BaseVariantAppProtocolHandler {
	public:
		VariantAppProtocolHandler(Variant &configuration);
		virtual ~VariantAppProtocolHandler();

		virtual bool ProcessMessage(BaseVariantProtocol *pProtocol,
				Variant &lastSent, Variant &lastReceived);
	private:
		ClientContext *GetContext(uint32_t contextId, uint64_t protocolType);
		void ProcessContextCreate(BaseVariantProtocol *pProtocol, Variant &request);
		void ProcessContextList(BaseVariantProtocol *pProtocol, Variant &request);
		void ProcessContextClose(BaseVariantProtocol *pProtocol, Variant &request);
		void ProcessContextCloseAll(BaseVariantProtocol *pProtocol, Variant &request);
		void ProcessCommandSetBitrates(BaseVariantProtocol *pProtocol, Variant &request);
		void ProcessCommandPlay(BaseVariantProtocol *pProtocol, Variant &request);
		void ProcessCommandPause(BaseVariantProtocol *pProtocol, Variant &request);
		void ProcessCommandResume(BaseVariantProtocol *pProtocol, Variant &request);
		void ProcessInfoListStreams(BaseVariantProtocol *pProtocol, Variant &request);
		void ProcessInfoListAllStreams(BaseVariantProtocol *pProtocol, Variant &request);
		void ProcessInfoBandwidth(BaseVariantProtocol *pProtocol, Variant &request);
		void ProcessInfoPlayback(BaseVariantProtocol *pProtocol, Variant &request);
	};
}

#endif	/* _VARIANTAPPPROTOCOLHANDLER_H */
