/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_Player.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/03/03
*
*  播放器
*
*/

#include "FFL_Player.hpp"
#include <pipeline/FFL_PipelineInputHandler.hpp>
#include <pipeline/FFL_PipelineEvent.hpp>

#include "FFMpeg.hpp"
#include "NodeBase.hpp"
#include "NodeReader.hpp"
#include "AudioComposer.hpp"
#include "VideoComposer.hpp"
#include "VideoSDL2Render.hpp"
#include "AudioSDL2Render.hpp"
#include "SDL2Module.hpp"
#include "FFL_PlayerEvent.hpp"

namespace player {
	class FFLPlayer::FFLPlayerEventFilter : public FFL::PipelineEventFilter {
	public:
		FFLPlayerEventFilter(FFLPlayer* player) :mPlayer(player) {}
		virtual ~FFLPlayerEventFilter() {

		}

		//
		// 派发消息前的准备，消息派发前会先到这地方的
		// 返回false表示不要向下派发了
		//
		bool onPrepareDispatch(const FFL::sp<FFL::PipelineEvent> event) {

			FFL::sp<event::PlayerEvent> evt =(event::PlayerEvent*) event.get();
			mPlayer->onEvent(evt);
			return true;
		}

		FFLPlayer* mPlayer;
	};

	FFLPlayer::FFLPlayer()
	{
		av_register_all();
		mAudioDevice = new FFLAudioDevice();
		mEventFilter = new FFLPlayerEventFilter(this);
		mPipeline = new FFL::Pipeline();
		mPipeline->setEventFilter(mEventFilter);
		mClock = new FFL::Clock();
		mClock->setSpeed(100);
		init();
	}
	FFLPlayer::~FFLPlayer()
	{
		FFL_SafeFree(mEventFilter);
	}
	//
	//  设置绘制目标
	//
	void FFLPlayer::setRender(FFL::sp<VideoRender> video, FFL::sp<AudioRender> audio) {
		mVideoRender = video;
		mAudioRender = audio;

		//
		//  渲染目标
		//				
		mVideoRender->create(this);
		mVideoComposer->setOutputRender(mVideoRender);
		
		//
		//
		//		
		mAudioRender->create(this);
		mAudioComposer->setOutputRender(mAudioRender);

		
		TimestampExtrapolator* extrapolator = new TimestampExtrapolator(mClock);
		mVideoComposer->mTimestampExtrapolator = extrapolator;
		mAudioComposer->mTimestampExtrapolator = extrapolator;
		mAudioRender->mTimestampExtrapolator = extrapolator;
	}

	void FFLPlayer::createAndsetSDL2Render(FFLPlayer* player ) {
		FFL::sp<player::SDL2Module> SDL2 = new player::SDL2Module();
		player::VideoSDL2Render* video  = new player::VideoSDL2Render(SDL2.get());
		player::AudioSDL2Render* audio = new player::AudioSDL2Render(SDL2.get());
		SDL2->create();

		player->mWindow = SDL2->getVideoDevice();
		player->setRender(video, audio);
	}

	status_t FFLPlayer::play(const char* url) {
		if (!mPipeline.isEmpty()) {
			mFileReader->play(url);
			return mPipeline->startup();
		}
		return FFL_ERROR_FAIL;
	}

	status_t FFLPlayer::stop()
	{
		if (!mPipeline.isEmpty())
		{
			if (!mAudioRender.isEmpty()) {
				mAudioRender->stop();
			}

			if (!mVideoRender.isEmpty()) {
				mVideoRender->stop();
			}			

			mPipeline->shutdownAndWait();
			mPipeline->setEventFilter(NULL);
		}
		return FFL_OK;
	}

	void FFLPlayer::release() {
		if (!mPipeline.isEmpty())
		{
			mPipeline->exit();
			mPipeline.clear();
		}
	}

	//
	//获取，设置播放的位置,总时长
	//
	void FFLPlayer::setPositionUs(int64_t pos) {
		mFileReader->seek(pos);
	}
	int64_t FFLPlayer::getPositionUs() {
		return mFileReader->getCurrentUs();
	}
	int64_t FFLPlayer::getDurationUs() {
		return mFileReader->getDurationUs();
	}


	status_t FFLPlayer::init()
	{
		FFL::sp<FFL::Pipeline> pipeline = mPipeline;
		//
		//  打开文件，读写文件节点
		//	
		mFileReader = new NodeReader();		
		mFileReader->setName("reader");
		mFileReader->create(this);

		//
		// 音视频合成
		//
		mAudioComposer = new AudioComposer();
		mAudioComposer->create(this);

		mVideoComposer = new VideoComposer();
		mVideoComposer->create(this);

		

		return FFL_OK;
	}

	void FFLPlayer::onEvent(const FFL::sp<event::PlayerEvent> event)
	{
		int32_t eventID = event->getEventId();
		switch (eventID)
		{
		case event::EVENT_VIDEO_SIZE_CAHNGED:
		{
			int32_t width = event->mInt32Parma1;
			int32_t height = event->mInt32Parma2;
			mWindow->setWindowSize(width,height);
		}
		break;

		case event::EVENT_VIDEO_RENDER_FIRST_FRAME:

			break;
		case event::EVENT_VIDEO_RENDER_LAST_FRAME:
			//mVideoRender->stop();
			//mPipeline->shutdown();
			break;

		case event::EVENT_AUDIO_RENDER_FIRST_FRAME:

			break;

		case event::EVENT_AUDIO_RENDER_LAST_FRAME:	
			//mAudioRender->stop();
			//mPipeline->shutdown();
			break;
		default:
			break;
		}
	}

	void FFLPlayer::registerNode( FFL::sp<NodeBase> node) {
		FFL::CMutex::Autolock l(mNodeLock);
		mNodeList.push_back(node);
	}
	void FFLPlayer::unRegisterNode( FFL::sp<NodeBase> node) {
		FFL::CMutex::Autolock l(mNodeLock);
		for (FFL::List<  FFL::sp<NodeBase> >::iterator it = mNodeList.begin();
			it != mNodeList.end(); it++) {
			if (it->get() == node.get()) {
				mNodeList.erase(it);
				break;
			}
		}
	}

	//  IStreamManager
	bool FFLPlayer::addStream(FFL::sp<Stream> stream) {
		if (stream.isEmpty()) {
			return false;
		}

		bool ret = false;
		switch (stream->getStreamType())
		{
		case  STREAM_TYPE_VIDEO:
		{
			FFL::CMutex::Autolock l(mStreamLock);
			stream->setComposer(mVideoComposer);			
			mStreamVec.push_back(stream);
			ret = true;
		}
			break;
		case  STREAM_TYPE_AUDIO:
		{
			FFL::CMutex::Autolock l(mStreamLock);
			stream->setComposer(mAudioComposer);
			mStreamVec.push_back(stream);
			ret = true;
		}
			break;
		default:
			ret = false;
			break;
		};

		return ret;
	}
	FFL::sp<Stream> FFLPlayer::removeStream(uint32_t index) {	

		return NULL;
	}
	FFL::sp<Stream> FFLPlayer::getStream(uint32_t index) {
		FFL::CMutex::Autolock l(mStreamLock);
		for (uint32_t i = 0; i < mStreamVec.size(); i++) {
			if (mStreamVec[i]->getIndex() == index) {
				return mStreamVec[i];
			}
		}

		return NULL;
	}

	IStreamManager* FFLPlayer::getStreamMgr() {
		return this;
	}

	//
	//  设置，获取播放速度
	//
	void FFLPlayer::setSpeed(uint32_t speed) {
		if (speed > 300) {
			speed = 300;
		}else if (speed <= 10) {
			speed = 10;
		}
		mClock->setSpeed(speed);
	}

	uint32_t FFLPlayer::getSpeed() {
		return mClock->speed();
	}
	//
	//  创建视频渲染窗体
	//
	extern FFL::sp<FFLWindow> createSDL2RenderWindow();
	FFL::sp<FFLWindow> createRenderWindow() {
		return createSDL2RenderWindow();		
	}
}
