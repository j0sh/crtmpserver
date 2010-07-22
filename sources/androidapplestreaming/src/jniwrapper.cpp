#include <jni.h>
#include <cstdlib>
#include <stdexcept>
#include <string>

// extern "C" JNIEXPORT jint JNICALL
// JNI_OnLoad (JavaVM * vm, void * reserved)
// {
//     return JNI_VERSION_1_6;
// }
extern "C" jstring Java_org_apache_android_media_VideoViewDemo_stringFromJNI(JNIEnv* env, jobject thiz, jstring _host, jint _port)
{
#if 1
    char portStr[16];

    sprintf( portStr, "%d", _port );

    const char* cstr = env->GetStringUTFChars( _host, NULL );

    std::string s = std::string( cstr ) + ":" + portStr;  //"rtsp://localhost:5544/";
#else
    std::string s = "rtsp://localhost:5544/";
#endif

    return env->NewStringUTF(s.c_str());
}

static JNINativeMethod sMethods[] = {
     /* name, signature, funcPtr */

    {"stringFromJNI", "(Ljava/lang/String;I)Ljava/lang/String;", (void*)Java_org_apache_android_media_VideoViewDemo_stringFromJNI},
};

extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }

    jclass clazz;

    clazz = env->FindClass("org/apache/android/media/VideoViewDemo");
    if (clazz == NULL) {
        return JNI_FALSE;
    }

    env->RegisterNatives(clazz, sMethods, 1);
    return JNI_VERSION_1_4;
}

