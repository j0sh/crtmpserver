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


#ifdef HAS_PROTOCOL_VAR
#include "eventsink/varianteventsink.h"
#include "clientcontext.h"
#include "applestreamingclientapplication.h"
#include "jnihelpers.h"
using namespace app_applestreamingclient;

VariantEventSink::VariantEventSink(uint32_t contextId)
: BaseEventSink(EVENT_SYNC_VARIANT, contextId) {
}

VariantEventSink::~VariantEventSink() {
}

bool VariantEventSink::SignalStreamRegistered(string streamName) {
	if (MAP_HAS1(_streamNames, streamName))
		return true;
	_streamNames[streamName] = streamName;
#ifdef ANDROID
	Variant message;
	message["eventtype"] = "localuri";
	message["localuri"] = GetRTSPHost() + streamName;
	message["reason"] = "start";
	return CallJava(message);
#else
	return true;
#endif
}

bool VariantEventSink::SignalStreamUnRegistered(string streamName) {
	_streamNames.erase(streamName);
	return true;
}

bool VariantEventSink::SignalUpgradeBandwidth(uint32_t oldBw, uint32_t newBw) {
#ifdef ANDROID
	Variant message;
	message["eventtype"] = "switchup";
	message["oldBw"] = (uint32_t) oldBw;
	message["newBw"] = (uint32_t) newBw;
	if (!CallJava(message)) {
		FATAL("Unable to call java");
		return false;
	}

	message.Reset();
	message["eventtype"] = "localuri";
	message["localuri"] = GetRTSPHost() + MAP_VAL(_streamNames.begin());
	message["reason"] = "switchup";
	return CallJava(message);
#else
	return true;
#endif
}

bool VariantEventSink::SignalDowngradeBandwidth(uint32_t oldBw, uint32_t newBw) {
#ifdef ANDROID
	Variant message;
	message["eventtype"] = "switchdown";
	message["oldBw"] = (uint32_t) oldBw;
	message["newBw"] = (uint32_t) newBw;
	if (!CallJava(message)) {
		FATAL("Unable to call java");
		return false;
	}

	message.Reset();
	message["eventtype"] = "localuri";
	message["localuri"] = GetRTSPHost() + MAP_VAL(_streamNames.begin());
	message["reason"] = "switchdown";
	return CallJava(message);
#else
	return true;
#endif
}

vector<string> VariantEventSink::GetStreamNames() {
	vector<string> result;

	FOR_MAP(_streamNames, string, string, i) {
		ADD_VECTOR_END(result, MAP_KEY(i));
	}

	return result;
}

#ifdef ANDROID

bool VariantEventSink::CallJava(Variant& message) {
	ClientContext *pContext = GetContext();
	if (pContext == NULL) {
		FATAL("Unable to get context");
		return false;
	}
	AppleStreamingClientApplication *pApp = pContext->GetApplication();
	if (pApp == NULL) {
		FATAL("Unable to get the application");
		return false;
	}
	CallBackInfo &ci = pApp->GetJavaCallBackInterface();
	FINEST("CallJava message:\n%s", STR(message.ToString()));
	return ::CallJava(ci, message);
}

string VariantEventSink::GetRTSPHost() {
	if (_rtspHost != "")
		return _rtspHost;
	ClientContext *pContext = GetContext();
	if (pContext == NULL) {
		FATAL("Unable to get context");
		return false;
	}
	AppleStreamingClientApplication *pApp = pContext->GetApplication();
	_rtspHost = (string) pApp->GetConfiguration()["rtspHost"];
	return _rtspHost;
}
#endif

#endif /* HAS_PROTOCOL_VAR */

