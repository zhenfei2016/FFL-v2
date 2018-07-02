/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  JNImain.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/06/28
*  https://github.com/zhenfei2016/FFL-v2.git
*
*  jni入口函数
*/
#include <jni.h>
#include <string>
#include "JavaAudiotrack.hpp"
#include "JavaFFLPlayer.hpp"
static JavaVM* gJvm;

JavaVM* getJavaVM(){
    return  gJvm;
}


extern "C" {
#include "FFL_log.h"
#include <android/log.h>
}

extern "C" int fflv2PrintLog(int level,const char* tag,const char *format, va_list vaList){
    __android_log_print(ANDROID_LOG_ERROR,tag?tag:"FFLv2",format,vaList);
    return 1;
}

void Loginit(){
    FFL_LogHook(fflv2PrintLog);
}

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    gJvm = vm;
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }
    Loginit();

    android::importJavaAudioTrack(*env);
    android::importJavaFFLPlayerClass(*env);

    return JNI_VERSION_1_4;
}

extern "C" JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved) {

}
