#include <jni.h>
#include <cstdlib>
#include <stdexcept>
#include <string>

extern "C" jstring Java_org_apache_android_media_VideoViewDemo_stringFromJNI(JNIEnv* env, jobject thiz){
	std::string s = "rtsp://localhost:5544/";
	return env->NewStringUTF(s.c_str());
}

