/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  SDL2Module.hpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/04/28 
 *  
 *  sdl2模块的相关封装
 *
*/
#include "SDL2Module.hpp"
#include <SDL.h>

namespace player {
	SDL2Module::SDL2Module()  {		
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	}
	SDL2Module::~SDL2Module() {
		SDL_Quit();
	}
	//
	//  创建播放视频设备
	//
	FFL::sp<VideoDevice> SDL2Module::createVideoDevice(FFLPlayer* player) {
		if (mVideoDevice.isEmpty()) {
			mVideoDevice = new SDL2VideoDevice();
		}
		return mVideoDevice;
	}
	//
	//  创建播放音频设备
	//
	FFL::sp<AudioDevice> SDL2Module::createAudioDevice(FFLPlayer* player) {
		if (mAudioDevice.isEmpty()) {
			mAudioDevice = new SDL2AudioDevice();
		}
		return mAudioDevice;
	}


	void SDL2Loop(FUNKeyPress fun,void* userdata) {
		bool bQuit=false;
		SDL_Event event;
		//while (SDL_WaitEventTimeout(&event,40) && !bQuit) {
		while (SDL_WaitEvent(&event) && !bQuit) {
			switch (event.type) {
			case SDL_KEYUP:
				if (fun && !fun(userdata, event.key.keysym.sym))
					bQuit = true;
				break;
			case SDL_QUIT:
				bQuit = true;
				break;
			}
		}
	}	
}
