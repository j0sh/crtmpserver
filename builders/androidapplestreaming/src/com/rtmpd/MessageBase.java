package com.rtmpd;

import java.util.HashMap;
import android.util.Log;

public class MessageBase extends HashMap<Object, Object> 
{
	public MessageBase(HashMap<Object, Object> hashMap){
		super(hashMap);
		Log.v("####", "MessageBase");
	}
	/*
	 * request section
	 */
	public HashMap<Object, Object> getRequest() {
		Log.v("####", "getRequest");
		return (HashMap<Object, Object>) this.get("request");
	}

	public HashMap<Object, Object> getRequestDebug() {
		Log.v("####", "getRequestDebug");
		return (HashMap<Object, Object>) getRequest().get("debug");
	}

	public String getRequestDebugFileName() {
		Log.v("####", "getRequestDebugFileName");
		return (String) getRequestDebug().get("fileName");
	}

	public int getRequestDebugLineNumber() {
		Log.v("####", "getRequestDebugLineNumber");
		return ((Integer) getRequestDebug().get("fileName")).intValue();
	}

	public String getRequestType() {
		Log.v("####", "getRequestType");
		return (String) getRequest().get("type");
	}

	public int getRequestContextId() {
		Log.v("####", "getRequestContextId");
		return ((Integer) getRequest().get("contextId")).intValue();
	}

	public HashMap<Object, Object> getRequestParameters() {
		Log.v("####", "getRequestParameters");
		return (HashMap<Object, Object>) getRequest().get("parameters");
	}

	/*
	 * response section
	 */
	public HashMap<Object, Object> getResponse() {
		Log.v("####", "getResponse");
		return (HashMap<Object, Object>) this.get("response");
	}

	public HashMap<Object, Object> getResponseDebug() {
		Log.v("####", "getResponseDebug");
		return (HashMap<Object, Object>) getResponse().get("debug");
	}

	public String getResponseDebugFileName() {
		Log.v("####", "getResponseDebugFileName");
		return (String) getResponseDebug().get("fileName");
	}

	public int getResponseDebugLineNumber() {
		Log.v("####", "getResponseDebugLineNumber");
		return ((Integer) getResponseDebug().get("fileName")).intValue();
	}

	public int getResponseStatus() {
		Log.v("####", "getResponseStatus");
		return ((Integer) getResponse().get("status")).intValue();
	}

	public String getResponseStatusDescription() {
		Log.v("####", "getResponseStatusDescription");
		return (String) getResponse().get("statusDescription");
	}

	public HashMap<Object, Object> getResponseParameters() {
		Log.v("####", "getResponseParameters");
		return (HashMap<Object, Object>) getResponse().get("parameters");
	}
}
