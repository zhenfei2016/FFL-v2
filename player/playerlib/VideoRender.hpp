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
#include "FFL_Window.hpp"
#include "FFL_Texture.hpp"
#include "Statistic.hpp"

namespace player {

	class SDL2Module;
	class VideoRender : public Render {
	public:
		VideoRender();
		~VideoRender();

		//
		//  �ɹ�������node
		//
		virtual void onCreate();

		void setRenderWindow(FFLWindow* window );
		//void setSize(int32_t widht, int32_t height);

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
		virtual void onShowTexture(FFLTexture* texture);
	protected:
		IStatisticVideoRender* mStatistic;
	};
}