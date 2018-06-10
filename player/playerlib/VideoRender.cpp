/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  VideoRender.cpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/05/25 
 *  
 *  视频渲染
 *
*/
#include "VideoRender.hpp"
#include "MessageFFMpegFrame.hpp"
#include "SDL2Module.hpp"
#include "FFL_Player.hpp"
#include <pipeline/FFL_PipelineAsyncConnectorFixedsize.hpp>

namespace player {
	VideoRender::VideoRender():mStatistic(NULL){
		
	}
	VideoRender::~VideoRender() {
		
	}
	//
	//  成功创建了node
	//
	void VideoRender::onCreate() {
		Render::onCreate();
		mStatistic = &(getOwner()->mStats);
	}
	
	void VideoRender::setRenderWindow(FFLWindow* window) {
	}


	//
	//   外部setDataInput时候调用此函数，创建对应conn
	//
	FFL::sp<FFL::PipelineConnector > VideoRender::onCreateConnector(
		const OutputInterface& output,
		const InputInterface& input, void* userdata) {
		FFL::PipelineAsyncConnectorFixSize* conn = new FFL::PipelineAsyncConnectorFixSize(5);
		//conn->getLooper()->setDebug(true);
		return conn;
	}
	//
	// 处理接收到的消息，如果返回false表示这个消息没有处理，
	// 返回true表示这个消息处理了，消息处理结束后必须msg->consume();
	//
	bool VideoRender::handleReceivedData(const FFL::sp<FFL::PipelineMessage>& msg, void* userdata)
	{
		bool ret = false;
		switch (msg->getType())
		{
		case MSG_FFMPEG_VIDEO_FRAME:
		{
			message::FFMpegVideoFrame* frame = (message::FFMpegVideoFrame*)msg->getPayload();
			onShowTexture(&(frame->mTexture));
			msg->consume(this);
			ret = true;
		}
			break;
		case MSG_CONTROL_READER_EOF:
			break;
		}
		
		return ret;
	}
	
	//
	//  收到待显示的纹理包
	//	
	void VideoRender::onShowTexture(FFLTexture* texture)
	{
		
	}
}
