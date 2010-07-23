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

VariantEventSink::VariantEventSink()
: BaseEventSink(EVENT_SYNC_VARIANT) {
}

VariantEventSink::~VariantEventSink() {
}

bool VariantEventSink::SignalStreamRegistered(string streamName) {
	_streamNames[streamName] = streamName;
	return true;
}

bool VariantEventSink::SignalStreamUnRegistered(string streamName) {
	_streamNames.erase(streamName);
	return true;
}

vector<string> VariantEventSink::GetStreamNames() {
	vector<string> result;

	FOR_MAP(_streamNames, string, string, i) {
		ADD_VECTOR_END(result, MAP_KEY(i));
	}

	return result;
}

#endif /* HAS_PROTOCOL_VAR */

