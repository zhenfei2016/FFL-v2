/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  NativeFFLPlayer.hpp   
*  Created by zhufeifei(34008081@qq.com) on 2018/06/28 
*  https://github.com/zhenfei2016/FFL-v2.git
*  Native层的FFLPlayer的一个包装层，把一些调用转到player::FFLPlayer中
*
*/

#ifndef _NATIVE_FFL_PLAYER_HPP_
#define _NATIVE_FFL_PLAYER_HPP_
#include <FFL.h>
#include <jni.h>
#include "JavaFFLPlayer.hpp"
namespace android {
	class NativeFFLPlayer {
	public:
		NativeFFLPlayer();

	public:
		//
		// 设置渲染到的目标
		//
		status_t setSurface(jobject surface);

		//
		// 设置渲染窗口的大小，只有窗口存在的情况下才可以设置大小，否则返回失败
		//
		status_t setSurfaceSize(int32_t widht, int32_t height);

		//
		//  播放，暂停，停止
		//
		status_t prepare();

		status_t start();

		//
		//  pause:1 进行暂停， 0：恢复
		//
		status_t pause(int32_t pause);

		status_t stop();

		//
		//  定位到指定us处
		//  获取当前的播放位置us
		//  获取总的播放时长us
		//
		status_t seekTo(int64_t us);

		int64_t getCurrentPosition();

		int64_t getDuration();

		//
		//  获取，设置播放速度，正常速度=100
		//
		uint32_t getSpeed();

		void setSpeed(uint32_t speed);

		//
		// 获取，设置音量
		//  0-255
		//
		void setVolume(int32_t volume);

		void getVolume(int32_t &volume);

		//
		// 获取，设置循环播放次数
		// 如果<0 : 一直循环播放
		//     =0 : 播放一次
		//     >0 : 播放num+1次
		//
		void setLoop(int32_t num);

		int32_t getLoop();

		//
		// 获取，设置一些特定的选项
		//
		void setOptionString(const char *name, const char *value);

		void getOptionString(const char *name, char *buf, uint32_t bufSize, const char *defaultVal);

		//
		// 获取，设置一些特定的选项
		//
		void setOptionInt64(const char *name, int64_t value);

		void getOptionInt64(const char *name, int64_t *value, int64_t defaultVal);

	public:
		//
		//  可以通过这个指针调用java层的函数
		//
		JavaFFLPlayer *mJavaFFLPlayer;
	};
}
#endif