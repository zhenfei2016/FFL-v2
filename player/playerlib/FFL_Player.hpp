/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  FFL_Player.hpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/03/03 
 *  
 *  播放器
 *
*/
#pragma once
#include <pipeline/FFL_Pipeline.hpp>
#include <utils/FFL_Utils.hpp>
#include "FFL_PlayerEvent.hpp"
#include "FFL_Window.hpp"
#include "FFL_AudioDevice.hpp"
#include "StreamManager.hpp"
#include "Statistic.hpp"


namespace player {
	class NodeBase;
	class NodeReader;

	class VideoComposer;
	class AudioComposer;

	class VideoRender;
	class AudioRender;

	class SDL2Module;

	class FFLPlayer : public IStreamManager
	{
		friend class NodeBase;
	public:
		FFLPlayer();
		~FFLPlayer();

		//
		//  设置绘制目标
		//
		void setRender(FFL::sp<VideoRender> video, FFL::sp<AudioRender> audio);
		static void createAndsetSDL2Render(FFLPlayer* player);

		//
		//  开始播放，停止
		//
		status_t play(const char* url);
		status_t stop();
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
	private:
		FFL::sp<FFL::Pipeline> mPipeline;

	public:
		//  IStreamManager
		virtual bool addStream(FFL::sp<Stream> stream);
		virtual FFL::sp<Stream> removeStream(uint32_t index);
		virtual FFL::sp<Stream> getStream(uint32_t index);
	private:	
		FFL::CMutex mStreamLock;
		FFL::Vector< FFL::sp<Stream> >  mStreamVec;

	protected:
		FFL::CMutex mNodeLock;
		FFL::List< FFL::sp<NodeBase> > mNodeList;
		void registerNode(FFL::sp<NodeBase> node);
		void unRegisterNode(FFL::sp<NodeBase> node);
	private:
		FFL::sp<NodeReader> mFileReader;
		
		
	public:
		FFL::sp<FFLAudioDevice> mAudioDevice;

		//
		//  接收系统中发布的事件
		//
		class FFLPlayerEventFilter;
		friend class FFLPlayerEventFilter;
		FFLPlayerEventFilter* mEventFilter;

		VideoComposer* mVideoComposer;
		AudioComposer* mAudioComposer;

		FFL::sp<VideoRender> mVideoRender;
		FFL::sp<AudioRender> mAudioRender;

		FFL::sp<FFLWindow> mWindow;
	public:
		FFL::sp<FFL::Clock> mClock;
	public:
		PlayerStatistic mStats;
	public:
		FFL::String mUrl;

	};
	//
	//  创建视频渲染窗体
	//
	FFL::sp<FFLWindow> createRenderWindow();
}
