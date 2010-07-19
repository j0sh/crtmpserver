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



#include "streaming/baseoutnetstream.h"
#include "streaming/streamstypes.h"

BaseOutNetStream::BaseOutNetStream(BaseProtocol *pProtocol,
		StreamsManager *pStreamsManager, uint64_t type, string name)
: BaseOutStream(pProtocol, pStreamsManager, type, name) {
	if (!TAG_KIND_OF(type, ST_OUT_NET)) {
		ASSERT("Incorrect stream type. Wanted a stream type in class %s and got %s",
				STR(tagToString(ST_OUT_NET)), STR(tagToString(type)));
	}
}

BaseOutNetStream::~BaseOutNetStream() {
}

