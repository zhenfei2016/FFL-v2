/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  SDL2Module.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/04/28
*
*  sdl2模块的相关封装
*
*/

#ifndef _SDL2_MODULE_HPP
#define _SDL2_MODULE_HPP

#include <SDL.h>
#include "SDL2VideoDevice.hpp"
#include "SDL2AudioDevice.hpp"
#include "AVDeviceCreator.hpp"


namespace player {
	class SDL2VideoDevice;
	class SDL2AudioDevice;
	
	class SDL2Module : public AVDeviceCreator {
	public:
		SDL2Module();
		~SDL2Module();	
		//
		//  创建播放视频设备
		//
		virtual FFL::sp<VideoDevice> createVideoDevice(FFLPlayer* player) ;
		//
		//  创建播放音频设备
		//
		virtual FFL::sp<AudioDevice> createAudioDevice(FFLPlayer* player) ;

	private:
		FFL::sp<SDL2VideoDevice> mVideoDevice;
		FFL::sp<SDL2AudioDevice> mAudioDevice;				
	};

	//返回false就会退出循环
	typedef bool (*FUNKeyPress)(void* userdata,int key);
	void SDL2Loop(FUNKeyPress fun, void* userdata);
}

#endif