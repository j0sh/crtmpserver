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
#include "new/common.h"

extern "C" void Java_com_rtmpd_CommandsInterface_EnvRun(
		JNIEnv* pEnv, jobject thiz, jstring host, jint port);
extern "C" void Java_com_rtmpd_CommandsInterface_EnvStop(
		JNIEnv* pEnv, jobject thiz);
extern "C" jobject Java_com_rtmpd_CommandsInterface_ContextCreate(
		JNIEnv* pEnv, jobject thiz);
extern "C" jobject Java_com_rtmpd_CommandsInterface_ContextList(
		JNIEnv* pEnv, jobject thiz);
extern "C" jobject Java_com_rtmpd_CommandsInterface_ContextClose(
		JNIEnv* pEnv, jobject thiz, jint contextId);
extern "C" jobject Java_com_rtmpd_CommandsInterface_ContextCloseAll(
		JNIEnv* pEnv, jobject thiz);
extern "C" jobject Java_com_rtmpd_CommandsInterface_CommandPlay(
		JNIEnv* pEnv, jobject thiz, jint contextId, jstring m3u8Uri,
		jstring httpSessionId, jstring keyPassword);
extern "C" jobject Java_com_rtmpd_CommandsInterface_CommandPause(
		JNIEnv* pEnv, jobject thiz, jint contextId);
extern "C" jobject Java_com_rtmpd_CommandsInterface_CommandResume(
		JNIEnv* pEnv, jobject thiz, jint contextId);
extern "C" jobject Java_com_rtmpd_CommandsInterface_InfoListStreams(
		JNIEnv* pEnv, jobject thiz, jint contextId);
extern "C" jobject Java_com_rtmpd_CommandsInterface_InfoListAllStreams(
		JNIEnv* pEnv, jobject thiz);
jobject VariantToJObject(Variant &value, JNIEnv* pEnv);

#endif	/* _JNIWRAPPER_H */

