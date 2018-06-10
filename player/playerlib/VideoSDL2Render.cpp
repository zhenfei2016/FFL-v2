/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  VideoSDL2Render.cpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/05/27 
 *  
 *
 *
*/
#include "VideoSDL2Render.hpp"
#include "MessageFFMpegFrame.hpp"
#include "SDL2Module.hpp"
#include "FFL_PlayerEvent.hpp"

namespace player {
	VideoSDL2Render::VideoSDL2Render(FFL::sp<SDL2Module> sdl) {
		setName("VideoSDL2Render");
		mSDL2 = sdl;
		mFrameIndex = -1;
	}
	VideoSDL2Render::~VideoSDL2Render() {
		mSDL2 = NULL;
		mFrameIndex = -1;

		FFL::sp<FFL::PipelineInput> input;
		
	}
	//
	//  停止render
	//
	void VideoSDL2Render::stop() {
		mSDL2->closeVideoDevice();
	}
	//
	//  收到待显示的纹理包
	//	
	int64_t gLastRenderUs = 0;
	void VideoSDL2Render::onShowTexture(FFLTexture* texture)
	{
		mStatistic->renderVideoFrame(texture->mOrginalPts, FFL_getNowUs());
		int64_t t1 = FFL_getNowUs();
		FFL_LOG_CRIT("VideoSDL2Render pts=%" lld64 " now=%" lld64 " render=%" lld64 " delay=%" lld64  " lastrenderus=%" lld64,
			texture->mOrginalPts,
			t1,texture->mRenderus, t1- texture->mRenderus,t1- gLastRenderUs);
		gLastRenderUs = t1;
		mSDL2->showTexture(texture);
		int64_t t2 = FFL_getNowUs();

		FFL_LOG_CRIT("VideoSDL2Render pts=%" lld64 " now=%" lld64 " render=%" lld64 " delay=%" lld64,
			texture->mOrginalPts,
			t2, texture->mRenderus, t2 - texture->mRenderus);		
		mStatistic->renderVideoDelayUs(t2 - t1);

		if (mFrameIndex==-1) {
			mFrameIndex = 0;
			event::postPlayerEvent(this, event::EVENT_VIDEO_RENDER_FIRST_FRAME);
		}
		mFrameIndex++;
	}
	bool VideoSDL2Render::handleReceivedData(const FFL::sp<FFL::PipelineMessage>& msg, void* userdata) {
		if (!VideoRender::handleReceivedData(msg, userdata))
		{
			if (msg->getType() == MSG_CONTROL_READER_EOF) {
				event::postPlayerEvent(this, event::EVENT_VIDEO_RENDER_LAST_FRAME);				
			}
		}
		return false;
	}
}