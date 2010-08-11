package com.rtmpd;

import java.util.HashMap;

public class CommandsInterface {
	public native void EnvRun(String ip, int port);

	public native void EnvStop();

	public native HashMap<Object, Object> ContextCreate();

	public native HashMap<Object, Object> ContextList();

	public native HashMap<Object, Object> ContextClose(int contextId);

	public native HashMap<Object, Object> ContextCloseAll();

	public native HashMap<Object, Object> CommandPlay(int contextId, String m3u8Uri, String httpSessionId, String keyPassword);

	public native HashMap<Object, Object> CommandPause(int contextId);

	public native HashMap<Object, Object> CommandResume(int contextId);

	public native HashMap<Object, Object> CommandSelectBandwidth(int contextId, int bandwidth);

	public native HashMap<Object, Object> CommandSeek(int contextId, double value);

	public native HashMap<Object, Object> CommandSelectAVChannels(int contextId, int audio, int video);

	public native HashMap<Object, Object> InfoListStreams(int contextId);

	public native HashMap<Object, Object> InfoListAllStreams();

	public native HashMap<Object, Object> InfoBandwidth(int contextId);

	public native HashMap<Object, Object> InfoAllBandwidth();

	public native HashMap<Object, Object> InfoPlayback(int contextId);

	public native HashMap<Object, Object> InfoAllPlayback();
}

