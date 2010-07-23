package com.rtmpd;

import java.util.HashMap;

public class MessageContextCreate extends MessageBase {
	public MessageContextCreate(HashMap<Object, Object> hashMap) {
		super(hashMap);
	}

	public int getCreatedContextId() {
		return ((Long) getResponseParameters().get("contextId")).intValue();
	}
}
