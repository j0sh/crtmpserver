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
		return (String) getStreamNames().get(new Integer(index));
	}
}
