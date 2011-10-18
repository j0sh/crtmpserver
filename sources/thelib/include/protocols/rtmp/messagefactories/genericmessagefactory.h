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
#ifndef _GENERICMESSAGEFACTORY_H
#define	_GENERICMESSAGEFACTORY_H

#include "common.h"

#define VH_HT(x)    ((x)[RM_HEADER][RM_HEADER_HEADERTYPE])
#define VH_CI(x)     ((x)[RM_HEADER][RM_HEADER_CHANNELID])
#define VH_TS(x)      ((x)[RM_HEADER][RM_HEADER_TIMESTAMP])
#define VH_ML(x) ((x)[RM_HEADER][RM_HEADER_MESSAGELENGTH])
#define VH_MT(x)   ((x)[RM_HEADER][RM_HEADER_MESSAGETYPE])
#define VH_SI(x)      ((x)[RM_HEADER][RM_HEADER_STREAMID])
#define VH_IA(x)      ((x)[RM_HEADER][RM_HEADER_ISABSOLUTE])
#define VH(x,ht,cid,ts,ml,mt,si,ia) \
    VH_HT((x)) = (uint8_t) (ht); \
    VH_CI((x)) = (uint32_t) (cid); \
    VH_TS((x)) = (uint32_t) (ts); \
    VH_ML((x)) = (uint32_t) (ml); \
    VH_MT((x)) = (uint8_t) (mt); \
    VH_SI((x)) = (uint32_t) (si); \
    VH_IA((x)) = (bool) (ia);

#define M_INVOKE_ID(x)          ((x)[RM_INVOKE][RM_INVOKE_ID])
#define M_INVOKE_FUNCTION(x)    ((x)[RM_INVOKE][RM_INVOKE_FUNCTION])
#define M_INVOKE_PARAMS(x)     ((x)[RM_INVOKE][RM_INVOKE_PARAMS])
#define M_INVOKE_PARAM(x,i)     ((x)[RM_INVOKE][RM_INVOKE_PARAMS][(uint32_t) (i)])

#define M_NOTIFY_FIRST_PARAM(x)     ((x)[RM_NOTIFY][RM_NOTIFY_PARAMS])
#define M_NOTIFY_PARAMS(x)     ((x)[RM_NOTIFY][RM_NOTIFY_PARAMS])
#define M_NOTIFY_PARAM(x,i)     ((x)[RM_NOTIFY][RM_NOTIFY_PARAMS][(uint32_t) (i)])

#define M_FLEXSTREAMSEND_PARAMS(x)     ((x)[RM_FLEXSTREAMSEND][RM_FLEXSTREAMSEND_PARAMS])
#define M_FLEXSTREAMSEND_PARAM(x,i)     ((x)[RM_FLEXSTREAMSEND][RM_FLEXSTREAMSEND_PARAMS][(uint32_t) (i)])

#define M_USRCTRL_TYPE(x)     ((x)[RM_USRCTRL][RM_USRCTRL_TYPE])
#define M_USRCTRL_TYPE_STRING(x)     ((x)[RM_USRCTRL][RM_USRCTRL_TYPE_STRING])
#define M_USRCTRL_STREAMID(x) ((x)[RM_USRCTRL][RM_USRCTRL_STREAMID])
#define M_USRCTRL_PING(x) ((x)[RM_USRCTRL][RM_USRCTRL_PING])
#define M_USRCTRL_PONG(x) ((x)[RM_USRCTRL][RM_USRCTRL_PONG])

#define M_CHUNKSIZE(x) ((x)[RM_CHUNKSIZE])
#define M_ABORTMESSAGE(x) ((x)[RM_ABORTMESSAGE])

#define M_SO_NAME(x) ((x)[RM_SHAREDOBJECT][RM_SHAREDOBJECT_NAME])
#define M_SO_VER(x) ((x)[RM_SHAREDOBJECT][RM_SHAREDOBJECT_VERSION])
#define M_SO_PERSISTANCE(x) ((x)[RM_SHAREDOBJECT][RM_SHAREDOBJECT_PERSISTENCE])
#define M_SO_PRIMITIVES(x) ((x)[RM_SHAREDOBJECT][RM_SHAREDOBJECT_PRIMITIVES])
#define M_SO_PRIMITIVE(x,i) ((x)[RM_SHAREDOBJECT][RM_SHAREDOBJECT_PRIMITIVES][(uint32_t)(i)])

#define M_FSO_NAME(x) ((x)[RM_FLEXSHAREDOBJECT][RM_FLEXSHAREDOBJECT_NAME])
#define M_FSO_VER(x) ((x)[RM_FLEXSHAREDOBJECT][RM_FLEXSHAREDOBJECT_VERSION])
#define M_FSO_PERSISTANCE(x) ((x)[RM_FLEXSHAREDOBJECT][RM_FLEXSHAREDOBJECT_PERSISTENCE])
#define M_FSO_PRIMITIVES(x) ((x)[RM_FLEXSHAREDOBJECT][RM_FLEXSHAREDOBJECT_PRIMITIVES])
#define M_FSO_PRIMITIVE(x,i) ((x)[RM_FLEXSHAREDOBJECT][RM_FLEXSHAREDOBJECT_PRIMITIVES][(uint32_t)(i)])

class DLLEXP GenericMessageFactory {
public:
	//channel specific
	static Variant GetChunkSize(uint32_t chunkSize);
	static Variant GetAck(uint64_t amount);
	static Variant GetWinAckSize(uint32_t value);
	static Variant GetPeerBW(uint32_t value, uint8_t type);
	static Variant GetAbortMessage(uint32_t channelId);

	//invokes and notify
	static Variant GetInvoke(uint32_t channelId, uint32_t streamId,
			double timeStamp, bool isAbsolute, double requestId,
			string functionName, Variant &parameters);
	static Variant GetNotify(uint32_t channelId,
			uint32_t streamId, double timeStamp, bool isAbsolute,
			string handlerName, Variant params);

	static Variant GetInvokeOnBWDone(double kbpsSpeed);

	static Variant GetInvokeOnStatus(uint32_t channelId, uint32_t streamId,
			double timeStamp, bool isAbsolute, double requestId, Variant &message);

	static Variant GetInvokeResult(uint32_t channelId, uint32_t streamId,
			double requestId, Variant firstParam, Variant &secondParam);
	static Variant GetInvokeResult(Variant &request, Variant &parameters);

	static Variant GetInvokeError(uint32_t channelId, uint32_t streamId,
			double requestId, Variant firstParam, Variant &secondParam);
	static Variant GetInvokeError(Variant &request, Variant &parameters);
	static Variant GetInvokeCallFailedError(Variant &request);
};


#endif	/* _GENERICMESSAGEFACTORY_H */

#endif /* HAS_PROTOCOL_RTMP */

