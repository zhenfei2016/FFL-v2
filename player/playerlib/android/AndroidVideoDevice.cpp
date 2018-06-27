/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  AndroidVideoDevice.cpp   
*  Created by zhufeifei(34008081@qq.com) on 2018/06/28 
*  https://github.com/zhenfei2016/FFL-v2.git
*  android播放音频接口
*
*/
#include "AndroidVideoDevice.hpp"

namespace android {
	AndroidVideoDevice::AndroidVideoDevice(){}
	AndroidVideoDevice::~AndroidVideoDevice(){}

	// 获取绘制窗口
	//		
	FFL::sp<player::VideoSurface> AndroidVideoDevice::getSurface() {
		return 0;
	}
	//
	// 更新绘制的目标窗口
	//
	void AndroidVideoDevice::setSurface(SurfaceHandle surface) {

	}
	//
	//  打开关闭视频设备
	//
	bool AndroidVideoDevice::open(SurfaceHandle surface, int32_t widht, int32_t height) {
		return false;
	}
	void AndroidVideoDevice::close() {

	}
	//
	//  清空cache的数据
	//
	void AndroidVideoDevice::clearCache() {

	}
	//
	//  写一帧数据
	//		
	bool AndroidVideoDevice::showTexture(player::VideoTexture* texture) {
		return false;
	}
}