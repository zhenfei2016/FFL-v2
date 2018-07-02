/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  AndroidAudioDevice.cpp   
*  Created by zhufeifei(34008081@qq.com) on 2018/06/28 
*  https://github.com/zhenfei2016/FFL-v2.git
*  android下播放音频的封装
*
*/
#include "AndroidAudioDevice.hpp"
#include "jni/JavaAudiotrack.hpp"
namespace android {
	AndroidAudioDevice::AndroidAudioDevice(){

	}
	AndroidAudioDevice::~AndroidAudioDevice(){}

	//
	//  获取支持的格式
	//  wanted: 如果为nUll则返回所有支持的格式
	//           非null 返回跟他匹配的 
	//  fmtList: 返回支持的格式list
	//
	void AndroidAudioDevice::getSupportFormat(const player::AudioFormat* wanted, FFL::List<player::AudioFormat>& fmtList) {

	}
	bool AndroidAudioDevice::isSupportFormat(const player::AudioFormat* wanted) {
		return true;
	}
	//
	// 设置缓冲多长时间的数据
	//
	void AndroidAudioDevice::setCacheUs(int64_t us) {

	}

	//
	//  打开关闭音频设备
	//
	bool AndroidAudioDevice::onOpen(const player::AudioFormat& wanted, int32_t sampleNum, player::AudioFormat& obtained) {
		return false;
	}
	void AndroidAudioDevice::onClose() {

		//udioTrack->
	}
	//
	//获取当前打开的格式，如果未打开则返回null
	//
	player::AudioFormat* AndroidAudioDevice::getOpenFormat() {
		return 0;
	}
	//
	//  写一帧数据
	//
	bool AndroidAudioDevice::writeFrame(player::AudioSample* samples) {
		return false;
	}
	//
	//  获取播放设备硬件的延迟
	//
	int64_t AndroidAudioDevice::getDeviceDelayUs() {
		return 0;
	}

	//
	// 获取缓冲的数据量，还没有播放
	//
	int64_t AndroidAudioDevice::getCacheBytes() {
		return 0;
	}
	int64_t AndroidAudioDevice::getCacheDelay(FFL::TimeBase& base) {
		return 0;
	}
	//
	//  清空缓冲的数据
	//
	int64_t AndroidAudioDevice::clearCache() {
		return 0;
	}
	//
	// 获取播放中的音频的pts
	//
	int64_t AndroidAudioDevice::getRenderingPts() {
		return 0;
	}

}