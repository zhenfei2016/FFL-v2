/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  VideoSDL2Render.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/05/27
*
*
*
*/

#pragma once
#include "VideoRender.hpp"
#include "FFL_Window.hpp"
#include "FFL_Texture.hpp"

namespace player {

	class SDL2Module;
	class VideoSDL2Render : public VideoRender {
	public:
		VideoSDL2Render(FFL::sp<SDL2Module> sdl);
		~VideoSDL2Render();
		
		//
		//  Í£Ö¹render
		//
		virtual void stop();
	private:
		bool handleReceivedData(const FFL::sp<FFL::PipelineMessage>& msg, void* userdata);
		//
		//  ¿ªÊ¼ÏÔÊ¾
		//
		void onShowTexture(FFLTexture* texture);
	private:
		int64_t mFrameIndex;
	public:
		FFL::sp<SDL2Module> mSDL2;
		
	};
}