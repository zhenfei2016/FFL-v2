/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  VideoRender.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/05/25
*
*  视频渲染
*
*/

#pragma once
#include "Render.hpp"
#include "RenderWindow.hpp"
#include "VideoTexture.hpp"
#include "Statistic.hpp"


namespace player {

	class VideoDevice;
	class VideoRender : public Render {
	public:
		VideoRender(FFL::sp<VideoDevice> device);
		~VideoRender();

		//
		//  成功创建了node
		//
		virtual void onCreate();
		
		//
		//   外部setDataInput时候调用此函数，创建对应conn
		//
		FFL::sp<FFL::PipelineConnector > onCreateConnector(
			const OutputInterface& output,
			const InputInterface& input, void* userdata);
	protected:
		//
		// 处理接收到的消息，如果返回false表示这个消息没有处理，
		// 返回true表示这个消息处理了，消息处理结束后必须msg->consume();
		//
		virtual bool handleReceivedData(const FFL::sp<FFL::PipelineMessage>& msg, void* userdata);
	protected:
		//
		//  开始显示
		//
		virtual void onShowTexture(VideoTexture* texture);
	protected:
		IStatisticVideoRender* mStatistic;
		FFL::sp<VideoDevice> mDevice;

		int64_t mFrameIndex;
	};
}