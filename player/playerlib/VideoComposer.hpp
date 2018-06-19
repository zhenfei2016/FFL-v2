/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  VideoComposer.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/05/19
*
*  视频合成
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
		//   外部setDataInput时候调用此函数，创建对应conn
		//
		virtual FFL::sp<FFL::PipelineConnector > onCreateConnector(
			const OutputInterface& output,
			const InputInterface& input,void* userdata);

		//
		// 处理接收到的消息，如果返回false表示这个消息没有处理，
		// 返回true表示这个消息处理了，消息处理结束后必须msg->consume();
		//
		virtual bool handleReceivedData(const FFL::sp<FFL::PipelineMessage>& msg, void* userdata);

	private:
		//
		//  计算多长时间后播放这一桢
		//
		int64_t getDelay(VideoTexture* texture);
		//
		//  音视频同步,返回集体等待的时长，如果<0 则跳过这一帧
		//
		int64_t avSync(int64_t delay);

		//
		//  开始处理收到的纹理数据
		//
		void handleTexture(const FFL::sp<FFL::PipelineMessage>& msg,VideoTexture* texture);
		//
		//  接收到eof消息
		//
		void handleEOF(const FFL::sp<FFL::PipelineMessage>& eof);
	public:
		FFL::TimeBase mTimerUnits;
		TimestampExtrapolator* mTimestampExtrapolator;

	protected:
		IStatisticVideoRender* mStatistic;
	};
}