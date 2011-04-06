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
#ifndef _CONNECTIONMESSAGEFACTORY_H
#define	_CONNECTIONMESSAGEFACTORY_H

#include "protocols/rtmp/messagefactories/genericmessagefactory.h"

class DLLEXP ConnectionMessageFactory {
public:
	static Variant GetPong();
	static Variant GetInvokeConnect(string appName,
			string tcUrl = "",
			double audioCodecs = 615,
			double capabilities = 15,
			string flashVer = "LNX 9,0,48,00",
			bool fPad = false,
			string pageUrl = "file:///mac.html",
			string swfUrl = "file:///mac.flv",
			double videoCodecs = 124,
			double videoFunction = 1,
			double objectEncoding = 0);

	static Variant GetInvokeClose();

	static Variant GetInvokeConnectResult(uint32_t channelId, uint32_t streamId,
			uint32_t requestId, string level, string code, string decription,
			double objectEncoding);
	static Variant GetInvokeConnectResult(Variant request,
			string level = RM_INVOKE_PARAMS_RESULT_LEVEL_STATUS,
			string code = RM_INVOKE_PARAMS_RESULT_CODE_NETCONNECTIONCONNECTSUCCESS,
			string decription = RM_INVOKE_PARAMS_RESULT_DESCRIPTION_CONNECTIONSUCCEEDED);

	static Variant GetInvokeConnectError(uint32_t channelId, uint32_t streamId,
			uint32_t requestId, string level, string code, string decription);
	static Variant GetInvokeConnectError(Variant request,
			string decription,
			string level = RM_INVOKE_PARAMS_RESULT_LEVEL_ERROR,
			string code = RM_INVOKE_PARAMS_RESULT_CODE_NETCONNECTIONCONNECTREJECTED);
};


#endif	/* _CONNECTIONMESSAGEFACTORY_H */

#endif /* HAS_PROTOCOL_RTMP */

