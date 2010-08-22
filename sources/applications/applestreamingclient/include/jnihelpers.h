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

#ifdef ANDROID
#ifndef _JNIHELPERS_H
#define	_JNIHELPERS_H

#include <jni.h>
#include "common.h"

typedef struct _CallBackInfo {
	JNIEnv* env;
	jobject callbackHandler;
	string callbackClassName;
	jclass callbackClass;
	string callbackMethodName;
	jmethodID callbackMethod;

	_CallBackInfo() {
		env = NULL;
		callbackHandler = NULL;
		callbackClassName = "";
		callbackClass = NULL;
		callbackMethodName = "";
		callbackMethod = NULL;
	};

	_CallBackInfo(_CallBackInfo & ci) {
		copy(ci);
	}

	_CallBackInfo & operator =(_CallBackInfo & ci) {
		copy(ci);
		return *this;
	}

	void copy(_CallBackInfo & ci) {
		this->env = ci.env;
		this->callbackHandler = ci.callbackHandler;
		this->callbackClassName = ci.callbackClassName;
		this->callbackClass = ci.callbackClass;
		this->callbackMethodName = ci.callbackMethodName;
		this->callbackMethod = ci.callbackMethod;
	}

	operator string() {
		return format("env: %p;\ncallbackHandler: %p;\ncallbackClassName: %s;\ncallbackClass: %p;\ncallbackMethodName: %s;\ncallbackMethod: %p",
				env,
				callbackHandler,
				STR(callbackClassName),
				callbackClass,
				STR(callbackMethodName),
				callbackMethod);
	};
} CallBackInfo;

jobject VariantToJObject(Variant &value, JNIEnv* pEnv);
bool CallJava(CallBackInfo &ci, Variant &parameters);

#endif	/* _JNIHELPERS_H */
#endif /* ANDROID */
