#ifndef _JAVA_FFLPLAYER_HPP_
#define _JAVA_FFLPLAYER_HPP_

#include <FFL.h>
#include "jnihelper/JavaClass.hpp"
#include "jnihelper/JavaObject.hpp"
namespace android{
    class JavaFFLPlayerClass;
    class JavaFFLPlayer;
    //
    //  引用进来java层的FFLPlayer
    //
    JavaFFLPlayerClass* importJavaFFLPlayerClass(JNIEnv env);
    //
    //  导出FFLplayer的Nativce层函数
    //
    JavaFFLPlayerClass* exportNativeFFLPlayerClass(JNIEnv env);
    JavaFFLPlayer* getJavaFFLPlayerClass();

    //
    //  FFLplayer的class
    //
    class JavaFFLplayerClass : public  JavaClass{
        friend  JavaFFLPlayerClass* importJavaFFLPlayerClass(JNIEnv env);
        friend  JavaFFLPlayerClass* exportNativeFFLPlayerClass(JNIEnv env);
    private:
        JavaFFLplayerClass(JNIEnv &env);
    protected:
        //
        //  进行初始化，反初始化具体逻辑
        //
        virtual bool onInit(JNIEnv& env);
        virtual void onUninit(JNIEnv& env);
    private:
        //
        //  native的函数
        //
        static void setup(JNIEnv env, jobject thiz,jobject weakthis);
        static void setDisplayWindow(JNIEnv env, jobject thiz, jobject surface);
        static void release(JNIEnv env, jobject thiz);
        static void finalize(JNIEnv env, jobject thiz);

    public:
        friend class JavaFFLPlayer;
        //
        //  java对象中，保存的native层变量
        //
        jfieldID mField_mNativePlayer;
        //
        //  java对象中的函数
        //
        jmethodID mMethod_postEventFromNative;
    };
    //
    //  一个java层的FFLPlayer
    //
    class JavaFFLPlayer : public JavaObject<JavaFFLplayerClass>{
    public:
        JavaFFLPlayer(jobject javaAudioTrack);
        ~JavaFFLPlayer();
    public:
        inline void setJniEnv(JNIEnv* env){
            mJNIEnv=env;
        }

        void postMessage();
    private:
        JNIEnv* mJNIEnv;
    };
}
#endif
