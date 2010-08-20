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

public class CommandsInterface {
 
        public native void EnvRun(VideoCallbacks callbackHandler, String ip, int port);

	public native void EnvStop();

	public native HashMap<Object, Object> ContextCreate();

	public native HashMap<Object, Object> ContextList();

	public native HashMap<Object, Object> ContextClose(int contextId);

	public native HashMap<Object, Object> ContextCloseAll();

	public native HashMap<Object, Object> CommandPlayCondensed(int contextId, String connectingString);

	public native HashMap<Object, Object> CommandPlay(int contextId, String m3u8Uri, String httpSessionId, String keyPassword);

	public native HashMap<Object, Object> CommandPause(int contextId);

	public native HashMap<Object, Object> CommandResume(int contextId);

	public native HashMap<Object, Object> CommandSelectBandwidth(int contextId, int bandwidth);

	public native HashMap<Object, Object> CommandSeek(int contextId, double value);

	public native HashMap<Object, Object> CommandSelectAVChannels(int contextId, int audio, int video);

	public native HashMap<Object, Object> InfoListStreams(int contextId);

	public native HashMap<Object, Object> InfoListAllStreams();

	public native HashMap<Object, Object> InfoBandwidth(int contextId);

	public native HashMap<Object, Object> InfoPlayback(int contextId);
}

