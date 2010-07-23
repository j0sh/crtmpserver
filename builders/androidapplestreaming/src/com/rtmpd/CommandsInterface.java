package com.rtmpd;

import java.util.HashMap;

public class CommandsInterface {
	public native void EnvRun(String ip, int port);

	public native void EnvStop();

	public native HashMap<Object, Object> ContextCreate();

	public native HashMap<Object, Object> ContextList();

	public native HashMap<Object, Object> ContextClose(int contextId);

	public native HashMap<Object, Object> ContextCloseAll();

	public native HashMap<Object, Object> CommandPlay(int contextId,
			String m3u8Uri, String httpSessionId, String keyPassword);

	public native HashMap<Object, Object> CommandPause(int contextId);

	public native HashMap<Object, Object> CommandResume(int contextId);

	public native HashMap<Object, Object> InfoListStreams(int contextId);

	public native HashMap<Object, Object> InfoListAllStreams();
}
