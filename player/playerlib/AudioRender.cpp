/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  AudioRender.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/04/07
*
*  声音的渲染类
*
*/

#include "AudioRender.hpp"
#include "AudioResample.hpp"
#include "MessageFFMpegFrame.hpp"
#include "FFL_Player.hpp"
#include "SDL2Module.hpp"
#include <pipeline/FFL_PipelineAsyncConnectorFixedsize.hpp>

namespace player {
	AudioRender::AudioRender():mStatistic(NULL){
	
	}
	AudioRender::~AudioRender() {
		
	}
	//
	//  成功创建了node
	//
	void AudioRender::onCreate() {
		Render::onCreate();
		mStatistic = &(getOwner()->mStats);
	}
	//
	//   外部setDataInput时候调用此函数，创建对应conn
	//
	FFL::sp<FFL::PipelineConnector > AudioRender::onCreateConnector(const OutputInterface& output,
		const InputInterface& input,void* userdata) {
		return new FFL::PipelineAsyncConnectorFixSize(5);
	}

	bool AudioRender::handleReceivedData(const FFL::sp<FFL::PipelineMessage>& msg, void* userdata)
	{
		switch (msg->getType())
		{
            case MSG_FFMPEG_AUDIO_FRAME: 
            {
                FFL::PipelineMessagePayload* payload=msg->getPayload();
                message::FFMpegAudioFrame* frame = (message::FFMpegAudioFrame*)(payload);
                onShowSamples(&(frame->mSamples));
                msg->consume(this);
                return true;
            }
			break;
		}
		
		return false;

	}	
	//
	//  收到待显示的采样
	//	
	void AudioRender::onShowSamples(FFLSample* samples)
	{	
	}
}
