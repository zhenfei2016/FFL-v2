/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  VideoRender.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/05/25
*
*  ��Ƶ��Ⱦ
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
		//  �ɹ�������node
		//
		virtual void onCreate();
		
		//
		//   �ⲿsetDataInputʱ����ô˺�����������Ӧconn
		//
		FFL::sp<FFL::PipelineConnector > onCreateConnector(
			const OutputInterface& output,
			const InputInterface& input, void* userdata);
	protected:
		//
		// ������յ�����Ϣ���������false��ʾ�����Ϣû�д���
		// ����true��ʾ�����Ϣ�����ˣ���Ϣ������������msg->consume();
		//
		virtual bool handleReceivedData(const FFL::sp<FFL::PipelineMessage>& msg, void* userdata);
	protected:
		//
		//  ��ʼ��ʾ
		//
		virtual void onShowTexture(VideoTexture* texture);
	protected:
		IStatisticVideoRender* mStatistic;
		FFL::sp<VideoDevice> mDevice;

		int64_t mFrameIndex;
	};
}