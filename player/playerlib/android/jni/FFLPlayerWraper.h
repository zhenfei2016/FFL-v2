#ifndef _FFL_PLAYER_WRAPER_H_
#define _FFL_PLAYER_WRAPER_H_

#ifdef __cplusplus__
extern "C"{
#endif


/*
 *   暴露给jni的播放器接口
 */

typedef struct FFLPlayerWraper {
	/*  
	 *  真正的播放器 指针
	 */
	void* mPlayer;
	/* native层调用java层接口*/
	//CallJavaInterface* m_pCalljava;

	/*  
	 * 构造函数，主要进行了函数指针的初始化
	 */
	void (*Construct)(struct FFLPlayerWraper *pthis);

	void (*Destruct)(struct FFLPlayerWraper *pthis);
	/*	 
	 *设置绘制目标surface 
	 */
	void (*setNativeWindow)(struct FFLPlayerWraper *pthis, ANativeWindow *pNativeWindow);		
	/*
	 * 发送消息给java层
	 */
	void (*postMessage)(struct FFLPlayerWraper *pthis, int what, int arg1, int age2);
} FFLPlayerWraper;

FFL_DECLEAR_CLASS(FFLPlayerWraper)

#ifdef __cplusplus__
}
#endif

#endif