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
#include "rtmpappprotocolhandler.h"
#include "protocols/rtmp/basertmpprotocol.h"
#include "protocols/rtmp/messagefactories/messagefactories.h"
#include "application/baseclientapplication.h"
#include "streaming/baseinnetstream.h"
#include "streaming/streamstypes.h"
using namespace app_flvplayback;

RTMPAppProtocolHandler::RTMPAppProtocolHandler(Variant &configuration)
: BaseRTMPAppProtocolHandler(configuration) {

}

RTMPAppProtocolHandler::~RTMPAppProtocolHandler() {
}

bool RTMPAppProtocolHandler::ProcessInvokeGeneric(BaseRTMPProtocol *pFrom,
		Variant &request) {

	string functionName = M_INVOKE_FUNCTION(request);
	if (functionName == "getAvailableFlvs") {
		return ProcessGetAvailableFlvs(pFrom, request);
	} else if (functionName == "insertMetadata") {
		return ProcessInsertMetadata(pFrom, request);
	} else {
		return BaseRTMPAppProtocolHandler::ProcessInvokeGeneric(pFrom, request);
	}
}

bool RTMPAppProtocolHandler::ProcessGetAvailableFlvs(BaseRTMPProtocol *pFrom, Variant &request) {
	Variant parameters;
	parameters.PushToArray(Variant());
	parameters.PushToArray(Variant());

	vector<string> files;
	if (!listFolder(_configuration[CONF_APPLICATION_MEDIAFOLDER],
			files)) {
		FATAL("Unable to list folder %s",
				STR(_configuration[CONF_APPLICATION_MEDIAFOLDER]));
		return false;
	}

	string file, name, extension;
	size_t normalizedMediaFolderSize = 0;
	string mediaFolderPath = normalizePath(_configuration[CONF_APPLICATION_MEDIAFOLDER], "");
	if ((mediaFolderPath != "") && (mediaFolderPath[mediaFolderPath.size() - 1] == PATH_SEPARATOR))
		normalizedMediaFolderSize = mediaFolderPath.size();
	else
		normalizedMediaFolderSize = mediaFolderPath.size() + 1;

	FOR_VECTOR_ITERATOR(string, files, i) {
		file = VECTOR_VAL(i).substr(normalizedMediaFolderSize);

		splitFileName(file, name, extension);
		extension = lowerCase(extension);

		if (extension != MEDIA_TYPE_FLV
				&& extension != MEDIA_TYPE_MP3
				&& extension != MEDIA_TYPE_MP4
				&& extension != MEDIA_TYPE_M4A
				&& extension != MEDIA_TYPE_M4V
				&& extension != MEDIA_TYPE_MOV
				&& extension != MEDIA_TYPE_F4V
				&& extension != MEDIA_TYPE_TS
				&& extension != MEDIA_TYPE_NSV)
			continue;
		string flashName = "";
		if (extension == MEDIA_TYPE_FLV) {
			flashName = name;
		} else if (extension == MEDIA_TYPE_MP3) {
			flashName = extension + ":" + name;
		} else if (extension == MEDIA_TYPE_NSV) {
			flashName = extension + ":" + name + "." + extension;
		} else {
			if (extension == MEDIA_TYPE_MP4
					|| extension == MEDIA_TYPE_M4A
					|| extension == MEDIA_TYPE_M4V
					|| extension == MEDIA_TYPE_MOV
					|| extension == MEDIA_TYPE_F4V) {
				flashName = MEDIA_TYPE_MP4":" + name + "." + extension;
			} else {
				flashName = extension + ":" + name + "." + extension;
			}
		}

		parameters[(uint32_t) 1].PushToArray(flashName);
	}

	map<uint32_t, BaseStream *> allInboundStreams =
			GetApplication()->GetStreamsManager()->FindByType(ST_IN_NET, true);

	FOR_MAP(allInboundStreams, uint32_t, BaseStream *, i) {
		parameters[(uint32_t) 1].PushToArray(MAP_VAL(i)->GetName());
	}

	Variant message = GenericMessageFactory::GetInvoke(3, 0, 0, false, 0,
			"SetAvailableFlvs", parameters);

	return SendRTMPMessage(pFrom, message);
}

bool RTMPAppProtocolHandler::ProcessInsertMetadata(BaseRTMPProtocol *pFrom, Variant &request) {
	NYIR;
}
#endif /* HAS_PROTOCOL_RTMP */

