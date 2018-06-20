/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  PlayerCore.hpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/03/03 
 *  
 *  播放器
 *
*/
#pragma once
#include <pipeline/FFL_Pipeline.hpp>
#include <utils/FFL_Utils.hpp>
#include "PlayerEvent.hpp"
#include "VideoSurface.hpp"
#include "AudioDevice.hpp"
#include "VideoDevice.hpp"
#include "StreamManager.hpp"
#include "Statistic.hpp"
#include "AVDeviceCreator.hpp"
#include "ClockUpdater.hpp"


namespace player {
	class NodeBase;
	class NodeReader;

	class VideoStream;
	class AudioStream;

	class VideoComposer;
	class AudioComposer;

	class VideoRender;
	class AudioRender;

	class TimestampExtrapolator;

	class SDL2Module;

	class PlayerCore : public IStreamManager ,public ClockUpdater
	{
		friend class NodeBase;
		friend class FFLPlayer;
	public:
		PlayerCore();
		~PlayerCore();
		//
		// 设置绘制窗口
		//
		void setVideoSurface(FFL::sp<VideoSurface> surface);
		//
		//  开始播放，停止
		//
		status_t play(const char* url);
		status_t stop();
		status_t pause();
		void release();

		//
		//获取，设置播放的位置,总时长
		//
		void setPositionUs(int64_t pos);
		int64_t getPositionUs();
		int64_t getDurationUs();

		//
		//  设置，获取播放速度
		//
		void setSpeed(uint32_t speed);
		uint32_t getSpeed();
	public:
		FFL::sp<FFL::Pipeline> getPipeline() { return mPipeline; };
		IStreamManager* getStreamMgr();
	protected:
		status_t init();

		void onEvent(const FFL::sp<event::PlayerEvent> event);

		void updateClcok(int64_t tm, int32_t streamId, void* uesrdata);
	public:
		//  IStreamManager
		virtual bool addStream(FFL::sp<Stream> stream);
		virtual FFL::sp<Stream> removeStream(uint32_t index);
		virtual FFL::sp<Stream> getStream(uint32_t index);
	protected:
		void onAddVideoStream(FFL::sp<VideoStream> stream);
		void onAddAudioStream(FFL::sp<AudioStream> stream);
		void onAddOtherStream(FFL::sp<Stream> stream);

		void createVideoDisplay();
	private:	
		FFL::CMutex mStreamLock;
		FFL::Vector< FFL::sp<Stream> >  mStreamVec;
		FFL::TimeBase mAudioTb;

	protected:
		FFL::CMutex mNodeLock;
		FFL::List< FFL::sp<NodeBase> > mNodeList;
		void registerNode(FFL::sp<NodeBase> node);
		void unRegisterNode(FFL::sp<NodeBase> node);
	private:
		FFL::sp<FFL::Pipeline> mPipeline;
		FFL::sp<NodeReader> mFileReader;		
	public:
		//
		//  接收系统中发布的事件
		//
		class PlayerCoreEventFilter;
		friend class PlayerCoreEventFilter;
		PlayerCoreEventFilter* mEventFilter;

		AVDeviceCreator* mDeviceCreator;
		FFL::sp<AudioDevice> mAudioDevice;
		FFL::sp<VideoDevice> mVideoDevice;		

		VideoComposer* mVideoComposer;
		AudioComposer* mAudioComposer;

		FFL::sp<VideoSurface> mSurface;
	private:
		FFL::sp<FFL::Clock> mClock;
		TimestampExtrapolator* mTimestampExtrapolator;
	public:
		PlayerStatistic mStats;
	public:
		FFL::String mUrl;
	};
}
