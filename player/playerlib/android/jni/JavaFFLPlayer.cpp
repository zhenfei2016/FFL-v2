/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  JavaFFLPlayer.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/06/28
*  https://github.com/zhenfei2016/FFL-v2.git
*
*  java层FFLPlayer和Native层FFLplayer的一些相关导出，导入封装
*/
#include "JavaFFLPlayer.hpp"
#include "NativeFFLPlayer.hpp"
#include "NativeFFLPlayer.hpp"

namespace android{
    static  JavaFFLplayerClass* gJavaFFLplayerClass=NULL;
    //
    // 获取Native层的player
    //
    static NativeFFLPlayer* getNativeFFLPlayer(JNIEnv env, jobject thiz){
        jlong player=env.GetLongField(thiz,gJavaFFLplayerClass->mField_mNativePlayer);
        return  (NativeFFLPlayer*)player;
    }
    JavaFFLplayerClass::JavaFFLplayerClass(JNIEnv &env):
            JavaClass(env,"ffl/player/FFLPlayer"){
    }
    //
    //  进行初始化，反初始化具体逻辑
    //
    bool JavaFFLplayerClass::onInit(JNIEnv& env){
        const char *sign = "ffl/player/FFLPlayer";
        mField_mNativePlayer=getFieldId(env,"mNativePlayer", "J");

        sign = "(Ljava/lang/Object;IIILjava/lang/Object;)V";
        mMethod_postEventFromNative=getStaticMethodId(env,"postEventFromNative", sign);

        //
        //  native层的函数导出到java层
        //
        static JNINativeMethod nativeMethodTables[] = {
                { "native_setup","(Ljava/lang/Object;)V",(void *)JavaFFLplayerClass::setup },
                { "native_setDisplayWindow","(Landroid/view/Surface;)V",(void *)JavaFFLplayerClass::setDisplayWindow },
                { "native_release","()V",(void *)JavaFFLplayerClass::release },
                { "native_finalize","()V",(void *)JavaFFLplayerClass::finalize },
        };
        exportNativeMethod(env,nativeMethodTables,FFL_ARRAY_ELEMS(nativeMethodTables));
        return  true;
    }
    void JavaFFLplayerClass::onUninit(JNIEnv& env){
    }
    //
    //  native的函数
    //
    void JavaFFLplayerClass::setup(JNIEnv env, jobject thiz,jobject weakthis){
        NativeFFLPlayer* player=new NativeFFLPlayer();
        env.SetLongField(thiz,gJavaFFLplayerClass->mField_mNativePlayer,(jlong)player);
        player->mJavaFFLPlayer=new JavaFFLPlayer(thiz);
    }
    void JavaFFLplayerClass::setDisplayWindow(JNIEnv env, jobject thiz, jobject surface){
        NativeFFLPlayer* player= getNativeFFLPlayer(env,thiz);
        player->setSurface(surface);
    }
    void JavaFFLplayerClass::release(JNIEnv env, jobject thiz){

    }
    void JavaFFLplayerClass::finalize(JNIEnv env, jobject thiz){

    }

    JavaFFLPlayer::JavaFFLPlayer(jobject javaAudioTrack):
            JavaObject<JavaFFLplayerClass>(*gJavaFFLplayerClass,javaAudioTrack){

    }
    JavaFFLPlayer::~JavaFFLPlayer(){

    }

    void JavaFFLPlayer::postMessage(){

    }
}