/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  VideoComposer.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/05/19
*
*  ��Ƶ�ϳ�
*
*/

#pragma once
#include "Composer.hpp"
#include "VideoTexture.hpp"
#include "Stream.hpp"
#include "Statistic.hpp"


namespace player {	
	class TimestampExtrapolator;
	class VideoComposer : public Composer {
	public:
		VideoComposer();
		~VideoComposer();

	protected:
		void onCreate();
		//
		//   �ⲿsetDataInputʱ����ô˺�����������Ӧconn
		//
		virtual FFL::sp<FFL::PipelineConnector > onCreateConnector(
			const OutputInterface& output,
			const InputInterface& input,void* userdata);

		//
		// ������յ�����Ϣ���������false��ʾ�����Ϣû�д���
		// ����true��ʾ�����Ϣ�����ˣ���Ϣ������������msg->consume();
		//
		virtual bool handleReceivedData(const FFL::sp<FFL::PipelineMessage>& msg, void* userdata);

	private:
		//
		//  ����೤ʱ��󲥷���һ��
		//
		int64_t getDelay(VideoTexture* texture);
		//
		//  ����Ƶͬ��,���ؼ���ȴ���ʱ�������<0 ��������һ֡
		//
		int64_t avSync(int64_t delay);

		//
		//  ��ʼ�����յ�����������
		//
		void handleTexture(const FFL::sp<FFL::PipelineMessage>& msg,VideoTexture* texture);
		//
		//  ���յ�eof��Ϣ
		//
		void handleEOF(const FFL::sp<FFL::PipelineMessage>& eof);
	public:
		FFL::TimeBase mTimerUnits;
		TimestampExtrapolator* mTimestampExtrapolator;

	protected:
		IStatisticVideoRender* mStatistic;
	};
}