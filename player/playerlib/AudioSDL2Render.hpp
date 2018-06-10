/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  AudioSDL2Render.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/04/07
*
*  声音渲染类 sdl
*
*/
#pragma once
#include "AudioRender.hpp"

namespace player {
	class AudioResample;
	class SDL2Module;
	class AudioSDL2Render : public AudioRender {
	public:
		AudioSDL2Render(FFL::sp<SDL2Module> sdl);
		~AudioSDL2Render();	

		//
		//  停止render
		//
		virtual void stop();
	private:
		//
		//   外部setDataInput时候调用此函数，创建对应conn
		//
		FFL::sp<FFL::PipelineConnector > onCreateConnector(const OutputInterface& output,
			const InputInterface& input, void* userdata);

		bool handleReceivedData(const FFL::sp<FFL::PipelineMessage>& msg, void* userdata);

		//
		//  开始显示
		//
		void onShowSamples(FFLSample* samples);

		
	private:
		int64_t mFrameIndex;

		FFL::sp<SDL2Module> mSDL2;
		AudioResample*  mResample;
	};
}
