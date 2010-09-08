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

#include "jniwrapper.h"
#include "api.h"

extern "C" void Java_com_rtmpd_CommandsInterface_EnvRun(
		JNIEnv* env,
		jobject thiz,
		jobject callbackHandler,
		jstring callbackClassName,
		jstring callbackMethodName,
		jstring ip,
		jint port,
		jint logLevel) {
	CallBackInfo ci;
	ci.env = env;
	ci.callbackHandler = callbackHandler;
	ci.callbackClassName = env->GetStringUTFChars(callbackClassName, NULL);
	ci.callbackMethodName = env->GetStringUTFChars(callbackMethodName, NULL);
	EnvRun(env->GetStringUTFChars(ip, NULL), (uint16_t) port, ci, (int) logLevel);
}

extern "C" void Java_com_rtmpd_CommandsInterface_WaitEnvReady(
		JNIEnv* pEnv,
		jobject thiz) {
	WaitEnvReady();
}

extern "C" void Java_com_rtmpd_CommandsInterface_EnvStop(
		JNIEnv* pEnv, jobject thiz) {
	EnvStop();
}

extern "C" jobject Java_com_rtmpd_CommandsInterface_ContextCreate(
		JNIEnv* pEnv, jobject thiz) {
	Variant result = ContextCreate();
	return VariantToJObject(result, pEnv);
}

extern "C" jobject Java_com_rtmpd_CommandsInterface_ContextList(
		JNIEnv* pEnv, jobject thiz) {
	Variant result = ContextList();
	return VariantToJObject(result, pEnv);
}

extern "C" jobject Java_com_rtmpd_CommandsInterface_ContextClose(
		JNIEnv* pEnv, jobject thiz, jint contextId) {
	Variant result = ContextClose((uint32_t) contextId);
	return VariantToJObject(result, pEnv);
}

extern "C" jobject Java_com_rtmpd_CommandsInterface_ContextCloseAll(
		JNIEnv* pEnv, jobject thiz) {
	Variant result = ContextCloseAll();
	return VariantToJObject(result, pEnv);
}

extern "C" jobject Java_com_rtmpd_CommandsInterface_CommandPlayCondensed(
		JNIEnv* pEnv, jobject thiz, jint contextId, jstring connectingString) {
	Variant result = CommandPlay(
			(uint32_t) contextId,
			pEnv->GetStringUTFChars(connectingString, NULL));
	return VariantToJObject(result, pEnv);
}

extern "C" jobject Java_com_rtmpd_CommandsInterface_CommandSetBitrates(
		JNIEnv* pEnv, jobject thiz, jint contextId, jstring bitrates) {
	Variant result = CommandSetBitrates(
			(uint32_t) contextId,
			pEnv->GetStringUTFChars(bitrates, NULL));
	return VariantToJObject(result, pEnv);
}

extern "C" jobject Java_com_rtmpd_CommandsInterface_CommandPlay(
		JNIEnv* pEnv, jobject thiz, jint contextId, jstring m3u8Uri,
		jstring httpSessionId, jstring keyPassword) {
	Variant result = CommandPlay(
			(uint32_t) contextId,
			pEnv->GetStringUTFChars(m3u8Uri, NULL),
			pEnv->GetStringUTFChars(httpSessionId, NULL),
			pEnv->GetStringUTFChars(keyPassword, NULL));
	return VariantToJObject(result, pEnv);
}

extern "C" jobject Java_com_rtmpd_CommandsInterface_CommandPause(
		JNIEnv* pEnv, jobject thiz, jint contextId) {
	Variant result = CommandPause((uint32_t) contextId);
	return VariantToJObject(result, pEnv);
}

extern "C" jobject Java_com_rtmpd_CommandsInterface_CommandResume(
		JNIEnv* pEnv, jobject thiz, jint contextId) {
	Variant result = CommandResume((uint32_t) contextId);
	return VariantToJObject(result, pEnv);
}

extern "C" jobject Java_com_rtmpd_CommandsInterface_CommandSelectBandwidth(
		JNIEnv* pEnv, jobject thiz, jint contextId, jint bandwidth) {
	Variant result = CommandSelectBandwidth((uint32_t) contextId,
			(uint32_t) bandwidth);
	return VariantToJObject(result, pEnv);
}

extern "C" jobject Java_com_rtmpd_CommandsInterface_CommandSeek(
		JNIEnv* pEnv, jobject thiz, jint contextId, jdouble value) {
	Variant result = CommandSeek((uint32_t) contextId, (double) value);
	return VariantToJObject(result, pEnv);
}

extern "C" jobject Java_com_rtmpd_CommandsInterface_CommandSelectAVChannels(
		JNIEnv* pEnv, jobject thiz, jint contextId, jint audio, jint video) {
	Variant result = CommandSelectAVChannels((uint32_t) contextId,
			(uint32_t) audio, (uint32_t) video);
	return VariantToJObject(result, pEnv);
}

extern "C" jobject Java_com_rtmpd_CommandsInterface_InfoListStreams(
		JNIEnv* pEnv, jobject thiz, jint contextId) {
	Variant result = InfoListStreams((uint32_t) contextId);
	return VariantToJObject(result, pEnv);
}

extern "C" jobject Java_com_rtmpd_CommandsInterface_InfoListAllStreams(
		JNIEnv* pEnv, jobject thiz) {
	Variant result = InfoListAllStreams();
	return VariantToJObject(result, pEnv);
}

extern "C" jobject Java_com_rtmpd_CommandsInterface_InfoBandwidth(
		JNIEnv* pEnv, jobject thiz, jint contextId) {
	Variant result = InfoBandwidth((uint32_t) contextId);
	return VariantToJObject(result, pEnv);
}

extern "C" jobject Java_com_rtmpd_CommandsInterface_InfoPlayback(
		JNIEnv* pEnv, jobject thiz, jint contextId) {
	Variant result = InfoPlayback((uint32_t) contextId);
	return VariantToJObject(result, pEnv);
}
