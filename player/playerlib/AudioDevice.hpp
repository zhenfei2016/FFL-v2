/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  AudioDevice.hpp   
*  Created by zhufeifei(34008081@qq.com) on 2018/06/22 
*  https://github.com/zhenfei2016/FFL-v2.git
*  音频设备的基类
*
*/
#ifndef _AUDIO_DEVICE_HPP_
#define _AUDIO_DEVICE_HPP_

#include <ref/FFL_Ref.hpp>
#include <utils/FFL_List.hpp>
#include <utils/FFL_Clock.hpp>
#include "AudioFormat.hpp"
#include "AudioSample.hpp"

namespace player {
	class AudioRender;
	class AudioDevice : public FFL::RefBase{
	public:		
		AudioDevice();
		virtual ~AudioDevice();
		//
		//  获取支持的格式
		//  wanted: 如果为nUll则返回所有支持的格式
		//           非null 返回跟他匹配的 
		//  fmtList: 返回支持的格式list
		//
		virtual void getSupportFormat(const AudioFormat* wanted,FFL::List<AudioFormat>& fmtList)=0;
		virtual bool isSupportFormat(const AudioFormat* wanted) = 0;
		//
		// 设置缓冲多长时间的数据
		//
		virtual void setCacheUs(int64_t us) = 0;
		//
		//  打开关闭音频设备
		//
		virtual bool open(const AudioFormat& wanted,int32_t sampleNum, AudioFormat& obtained)=0;
		virtual void close()=0;
		//
		//获取当前打开的格式，如果未打开则返回null
		//
		virtual AudioFormat* getOpenFormat()=0;
		//
		//  写一帧数据
		//
		virtual bool writeFrame(AudioSample* samples)=0;
		//
		//  获取播放设备硬件的延迟
		//
		virtual int64_t getDeviceDelayUs()=0;
		//
		// 获取缓冲的数据量，还没有播放
		//
		virtual int64_t getCacheBytes()=0;	
		virtual int64_t getCacheDelay(FFL::TimeBase& base) = 0;
		//
		// 获取render
		//
		virtual FFL::sp<AudioRender> getRender(void* userdata) ;	
		//
		// 获取播放中的音频的pts
		//
		virtual int64_t getRenderingPts()=0;
	private:
		FFL::sp<AudioRender> mAudioRender;
	};
}
#endif