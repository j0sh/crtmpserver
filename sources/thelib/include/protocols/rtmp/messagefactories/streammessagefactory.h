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
#ifndef _STREAMMESSAGEFACTORY_H
#define	_STREAMMESSAGEFACTORY_H

#include "protocols/rtmp/messagefactories/genericmessagefactory.h"

class DLLEXP StreamMessageFactory {
public:
	//stream control
	static Variant GetUserControlStream(uint16_t operation, uint32_t streamId);
	static Variant GetUserControlStreamBegin(uint32_t streamId);
	static Variant GetUserControlStreamEof(uint32_t streamId);
	static Variant GetUserControlStreamDry(uint32_t streamId);
	static Variant GetUserControlStreamIsRecorded(uint32_t streamId);

	//management requests
	static Variant GetInvokeCreateStream();
	static Variant GetInvokeCloseStream(uint32_t channelId, uint32_t streamId);
	static Variant GetInvokeDeleteStream(uint32_t channelId, uint32_t streamId);
	static Variant GetInvokePublish(uint32_t channelId, uint32_t streamId,
			string streamName, string mode = "live");
	static Variant GetInvokePlay(uint32_t channelId, uint32_t streamId,
			string streamName, double start, double length);
	static Variant GetInvokeFCSubscribe(string streamName);

	//management responses
	static Variant GetInvokeCreateStreamResult(Variant &request,
			double createdStreamId);
	static Variant GetInvokeCreateStreamResult(uint32_t channelId,
			uint32_t streamId, uint32_t requestId, double createdStreamId);
	static Variant GetInvokeReleaseStreamResult(uint32_t channelId,
			uint32_t streamId, uint32_t requestId, double releasedStreamId);
	static Variant GetInvokeReleaseStreamErrorNotFound(Variant &request);
	static Variant GetInvokeOnFCPublish(uint32_t channelId,
			uint32_t streamId, double timeStamp, bool isAbsolute,
			double requestId, string code, string description);

	//event notifications
	static Variant GetInvokeOnStatusStreamPublishBadName(Variant &request,
			string streamName);
	static Variant GetInvokeOnStatusStreamPublishBadName(uint32_t channelId,
			uint32_t streamId, double requestId, string streamName);
	static Variant GetInvokeOnStatusStreamPublished(uint32_t channelId,
			uint32_t streamId, double timeStamp, bool isAbsolute,
			double requestId, string level, string code, string description,
			string details, string clientId);
	static Variant GetInvokeOnStatusStreamPlayFailed(Variant &request,
			string streamName);
	static Variant GetInvokeOnStatusStreamPlayFailed(uint32_t channelId,
			uint32_t streamId, double requestId, string streamName);
	static Variant GetInvokeOnStatusStreamPlayReset(uint32_t channelId,
			uint32_t streamId, double timeStamp, bool isAbsolute,
			double requestId, string description, string details, string clientId);
	static Variant GetInvokeOnStatusStreamPlayStart(uint32_t channelId,
			uint32_t streamId, double timeStamp, bool isAbsolute,
			double requestId, string description, string details, string clientId);
	static Variant GetInvokeOnStatusStreamPlayStop(uint32_t channelId,
			uint32_t streamId, double timeStamp, bool isAbsolute,
			double requestId, string description, string details, string clientId);
	static Variant GetInvokeOnStatusStreamPlayUnpublishNotify(uint32_t channelId,
			uint32_t streamId, double timeStamp, bool isAbsolute,
			double requestId, string description, string clientId);
	static Variant GetInvokeOnStatusStreamSeekNotify(uint32_t channelId,
			uint32_t streamId, double timeStamp, bool isAbsolute,
			double requestId, string description, string details, string clientId);
	static Variant GetInvokeOnStatusStreamPauseNotify(uint32_t channelId,
			uint32_t streamId, double timeStamp, bool isAbsolute,
			double requestId, string description, string details, string clientId);
	static Variant GetInvokeOnStatusStreamUnpauseNotify(uint32_t channelId,
			uint32_t streamId, double timeStamp, bool isAbsolute,
			double requestId, string description, string details, string clientId);
	static Variant GetNotifyRtmpSampleAccess(uint32_t channelId,
			uint32_t streamId, double timeStamp, bool isAbsolute,
			bool unknown1, bool unknown2);
	static Variant GetNotifyOnMetaData(uint32_t channelId,
			uint32_t streamId, double timeStamp, bool isAbsolute,
			Variant metadata);
	static Variant GetNotifyOnPlayStatusPlayComplete(uint32_t channelId,
			uint32_t streamId, double timeStamp, bool isAbsolute,
			double bytes, double duration);
	static Variant GetNotifyOnStatusDataStart(uint32_t channelId,
			uint32_t streamId, double timeStamp, bool isAbsolute);

	//generic stream send
	static Variant GetFlexStreamSend(uint32_t channelId, uint32_t streamId,
			double timeStamp, bool isAbsolute, string function,
			Variant &parameters);
};

#endif	/* _STREAMMESSAGEFACTORY_H */

#endif /* HAS_PROTOCOL_RTMP */

