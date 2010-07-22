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

#ifndef _JNIWRAPPER_H
#define	_JNIWRAPPER_H

#include <jni.h>
#include "utils/core.h"

extern "C" void Java_org_apache_android_media_VideoViewDemo_EnvRun(
		JNIEnv* pEnv, jobject thiz, jstring host, jint port);
extern "C" void Java_org_apache_android_media_VideoViewDemo_EnvStop(
		JNIEnv* pEnv, jobject thiz);
extern "C" jobject Java_org_apache_android_media_VideoViewDemo_ContextCreate(
		JNIEnv* pEnv, jobject thiz);
extern "C" jobject Java_org_apache_android_media_VideoViewDemo_ContextList(
		JNIEnv* pEnv, jobject thiz);
extern "C" jobject Java_org_apache_android_media_VideoViewDemo_ContextClose(
		JNIEnv* pEnv, jobject thiz, jint contextId);
extern "C" jobject Java_org_apache_android_media_VideoViewDemo_ContextCloseAll(
		JNIEnv* pEnv, jobject thiz);
extern "C" jobject Java_org_apache_android_media_VideoViewDemo_CommandPlay(
		JNIEnv* pEnv, jobject thiz, jint contextId, jstring m3u8Uri,
		jstring httpSessionId, jstring keyPassword);
extern "C" jobject Java_org_apache_android_media_VideoViewDemo_CommandPause(
		JNIEnv* pEnv, jobject thiz, jint contextId);
extern "C" jobject Java_org_apache_android_media_VideoViewDemo_CommandResume(
		JNIEnv* pEnv, jobject thiz, jint contextId);
jobject VariantToJObject(Variant &value, JNIEnv* pEnv);

#endif	/* _JNIWRAPPER_H */

