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

#include "Player.hpp"
#include <pipeline/FFL_PipelineInputHandler.hpp>
#include <pipeline/FFL_PipelineEvent.hpp>

#include "FFMpeg.hpp"
#include "NodeBase.hpp"
#include "NodeReader.hpp"
#include "AudioComposer.hpp"
#include "VideoComposer.hpp"

#include "VideoStream.hpp"
#include "AudioStream.hpp"

#include "VideoRender.hpp"
#include "AudioRender.hpp"

#include "SDL2Module.hpp"
#include "PlayerEvent.hpp"

#include "Decoder.hpp"

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

	FFLPlayer::FFLPlayer() :mTimestampExtrapolator(NULL){
		mVideoComposer = NULL;
		mAudioComposer = NULL;
		av_register_all();			
		mEventFilter = new FFLPlayerEventFilter(this);
		mPipeline = new FFL::Pipeline();
		mPipeline->setEventFilter(mEventFilter);
		mClock = new FFL::Clock();
		setSpeed(100);
		mTimestampExtrapolator = new TimestampExtrapolator(mClock);

		init();
	}
	FFLPlayer::~FFLPlayer()
	{
		FFL_SafeFree(mDeviceCreator);
		FFL_SafeFree(mTimestampExtrapolator);
		FFL_SafeFree(mEventFilter);
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
			if (!mAudioDevice.isEmpty()) {
				mAudioDevice->close();
			}

			if (!mVideoDevice.isEmpty()) {
				mVideoDevice->close();
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
	status_t FFLPlayer::init(){		
		mDeviceCreator = new SDL2Module();
		//
		//  打开文件，读写文件节点
		//	
		mFileReader = new NodeReader();		
		mFileReader->setName("reader");
		mFileReader->create(this);

		////
		//// 音视频合成
		////
		//mAudioComposer = new AudioComposer();
		//mAudioComposer->create(this);

		//mVideoComposer = new VideoComposer();
		//mVideoComposer->create(this);

		createVideoDisplay();
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
			//mWindow->setWindowSize(width,height);
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
	void FFLPlayer::updateClcok(int64_t tm, int32_t streamId, void* uesrdata) {
		//更新同步时钟
		mTimestampExtrapolator->update(tm, mAudioTb);
	}
	void FFLPlayer::onAddVideoStream(FFL::sp<VideoStream> stream) {
		if (mVideoDevice.isEmpty()) {
			mVideoDevice = mDeviceCreator->createVideoDevice(this);
			uint32_t width = 0;
			uint32_t height = 0;
			stream->getSize(width, height);			
			mVideoDevice->open(NULL, width, height);

			mTimestampExtrapolator->reset();
		}	

		if (!mVideoComposer) {
			mVideoComposer = new VideoComposer();
			mVideoComposer->mTimestampExtrapolator = mTimestampExtrapolator;
			mVideoComposer->create(this);
		}
	
		FFL::sp<VideoRender> render = mVideoDevice->getRender(NULL);
		if (!render->isCreated()) {
			render->create(this);
			mVideoComposer->setOutputRender(render);
		}

		FFL::sp<Decoder> decoder = stream->createDecoder();
		if (!decoder->isCreated()) {
			decoder->create(this);
		}
		decoder->setOutputComposer(mVideoComposer);
		

		//
		// 启动decoder和compoer的处理
		//
		mPipeline->startup(decoder->getNodeId());
		mPipeline->startup(mVideoComposer->getNodeId());
		mPipeline->startup(render->getNodeId());
	}
	void FFLPlayer::onAddAudioStream(FFL::sp<AudioStream> stream) {
		if (mAudioDevice.isEmpty()) {
			mAudioDevice = mDeviceCreator->createAudioDevice(this);			

			AudioFormat fmt;
			stream->getFormat(fmt);
			//x 
			// 启动音频设备
			//
			AudioFormat obtainedFmt;
			if (mAudioDevice->open(fmt, 1024, obtainedFmt)) {				
			}
		}

		FFL::sp<AudioRender> render = mAudioDevice->getRender(NULL);
		if (!render.isEmpty() && !render->isCreated()) {
			render->mTimestampExtrapolator = mTimestampExtrapolator;
			stream->getTimebase(mAudioTb);
			render->setClockUpdater(this, 0);
			render->create(this);
		}

		if (!mAudioComposer) {
			mAudioComposer = new AudioComposer();
			mAudioComposer->mTimestampExtrapolator = mTimestampExtrapolator;
			mAudioComposer->setOutputFormat(mAudioDevice->getOpenFormat());
			mAudioComposer->create(this);
			mAudioComposer->setOutputRender(render);
		}		

		FFL::sp<Decoder> decoder = stream->createDecoder();
		if (!decoder->isCreated()) {
			decoder->create(this);
		}
		decoder->setOutputComposer(mAudioComposer);

		//
		// 启动decoder和compoer的处理
		//
		mPipeline->startup(decoder->getNodeId());
		mPipeline->startup(mAudioComposer->getNodeId());
		mPipeline->startup(render->getNodeId());
	}
	void FFLPlayer::onAddOtherStream(FFL::sp<Stream> stream) {
	}
	void FFLPlayer::createVideoDisplay() {
		if (mVideoDevice.isEmpty()) {
			mVideoDevice = mDeviceCreator->createVideoDevice(this);
			
			uint32_t width = 400;
			uint32_t height = 300;
			mVideoDevice->open(NULL, width, height);	
		}
	}
	//  IStreamManager
	bool FFLPlayer::addStream(FFL::sp<Stream> stream) {
		if (stream.isEmpty()) {
			return false;
		}

		{
			FFL::CMutex::Autolock l(mStreamLock);
			mStreamVec.push_back(stream);
		}

		bool ret = false;
		switch (stream->getStreamType())
		{
		case  STREAM_TYPE_VIDEO:
		{ 
			onAddVideoStream((VideoStream*)stream.get());			
			ret = true;
		}
			break;
		case  STREAM_TYPE_AUDIO:
		{			
			onAddAudioStream((AudioStream*)stream.get());			
			ret = true;
		}
			break;
		default:
			onAddOtherStream(stream);
			ret = false;
			break;
		};

		return ret;
	}
	FFL::sp<Stream> FFLPlayer::removeStream(uint32_t index) {	
		FFL_LOG_WARNING("FFLPlayer::removeStream no impl.");
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
}
