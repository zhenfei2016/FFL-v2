/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  AudioDevice.cpp   
*  Created by zhufeifei(34008081@qq.com) on 2018/06/22 
*  https://github.com/zhenfei2016/FFL-v2.git
*  音频设备的基类
*
*/
#include "AudioDevice.hpp"
#include "AudioRender.hpp"
namespace player {
	AudioDevice::AudioDevice() {
	}
	AudioDevice::~AudioDevice(){
	}
	//
	// 获取render
	//
	FFL::sp<AudioRender> AudioDevice::getRender(void* userdata) {
		if (mAudioRender.isEmpty()) {
			mAudioRender = new AudioRender(this);
		}
		return mAudioRender;
	}
}
