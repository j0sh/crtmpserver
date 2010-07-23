package com.rtmpd;

import java.util.HashMap;

public class MessageContextList extends MessageBase {
	public MessageContextList(HashMap<Object, Object> hashMap) {
		super(hashMap);
	}

	public HashMap<Object, Object> getContextIds() {
		return (HashMap<Object, Object>) getResponseParameters();
	}
	
	public int getContextIdsLength(){
		return getContextIds().size();
	}
	
	public int getContextId(int index){
		return ((Integer)getContextIds().get(new Integer(index))).intValue();
	}
}
