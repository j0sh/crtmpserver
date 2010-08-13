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

package com.rtmpd;

import java.util.HashMap;

public class MessageInfoListStreams extends MessageBase {

	public MessageInfoListStreams(HashMap<Object, Object> hashMap) {
		super(hashMap);
	}

	public HashMap<Object, Object> getStreamNames() {
		return (HashMap<Object, Object>) getResponseParameters();
	}

	public int getStreamNamesLength() {
		return getStreamNames().size();
	}

	public String getStreamName(int index) {
		return (String) getStreamNames().get(new Long(index));
	}
}
