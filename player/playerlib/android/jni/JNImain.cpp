#include <jni.h>
#include <string>
#include "JavaAudiotrack.hpp"
#include "JavaFFLPlayer.hpp"
static JavaVM* gJvm;
extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    gJvm = vm;
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }

    android::importJavaAudioTrack(*env);
    //android::importJavaFFLPlayerClass(*env);
    //android::exportNativeFFLPlayerClass(*env);

    return JNI_VERSION_1_4;
}

extern "C" JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved) {

}
