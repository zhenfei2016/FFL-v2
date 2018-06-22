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

#include "PlayerCore.hpp"
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
	class PlayerCore::PlayerCoreEventFilter : public FFL::PipelineEventFilter {
	public:
		PlayerCoreEventFilter(PlayerCore* player) :mPlayer(player) {}
		virtual ~PlayerCoreEventFilter() {

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

		PlayerCore* mPlayer;
	};

	PlayerCore::PlayerCore():mSpeed(100){
		mMasterClock = NULL;
		mVideoComposer = NULL;
		mAudioComposer = NULL;
		av_register_all();			
		mEventFilter = new PlayerCoreEventFilter(this);
		mPipeline = new FFL::Pipeline();
		mPipeline->setEventFilter(mEventFilter);		
		setSpeed(100);
		init();
	}
	PlayerCore::~PlayerCore()
	{
		FFL_SafeFree(mDeviceCreator);		
		FFL_SafeFree(mEventFilter);
	}
	//
	// 设置绘制窗口
	//
	void PlayerCore::setVideoSurface(FFL::sp<VideoSurface> surface) {
		mSurface = surface;
	}
	status_t PlayerCore::play(const char* url) {
		if (!mPipeline.isEmpty()) {
			mFileReader->play(url);
			return mPipeline->startup();
		}
		return FFL_ERROR_FAIL;
	}

	status_t PlayerCore::stop()
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
	status_t PlayerCore::pause() {
		return FFL_OK;
	}

	void PlayerCore::release() {
		if (!mPipeline.isEmpty())
		{
			mPipeline->exit();
			mPipeline.clear();
		}
	}

	//
	//获取，设置播放的位置,总时长
	//
	void PlayerCore::setPositionUs(int64_t pos) {
		mFileReader->seek(pos);
	}
	int64_t PlayerCore::getPositionUs() {
		return mFileReader->getCurrentUs();
	}
	int64_t PlayerCore::getDurationUs() {
		return mFileReader->getDurationUs();
	}
	status_t PlayerCore::init(){		
		mDeviceCreator = new SDL2Module();
		//
		//  打开文件，读写文件节点
		//	
		mFileReader = new NodeReader();		
		mFileReader->setName("reader");
		mFileReader->create(this);		

		createVideoDisplay();
		return FFL_OK;
	}

	void PlayerCore::onEvent(const FFL::sp<event::PlayerEvent> event)
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

	void PlayerCore::registerNode( FFL::sp<NodeBase> node) {
		FFL::CMutex::Autolock l(mNodeLock);
		mNodeList.push_back(node);
	}
	void PlayerCore::unRegisterNode( FFL::sp<NodeBase> node) {
		FFL::CMutex::Autolock l(mNodeLock);
		for (FFL::List<  FFL::sp<NodeBase> >::iterator it = mNodeList.begin();
			it != mNodeList.end(); it++) {
			if (it->get() == node.get()) {
				mNodeList.erase(it);
				break;
			}
		}
	}
	void PlayerCore::onAddVideoStream(FFL::sp<VideoStream> stream) {
		if (mVideoDevice.isEmpty()) {
			mVideoDevice = mDeviceCreator->createVideoDevice(this);
			uint32_t width = 0;
			uint32_t height = 0;
			stream->getSize(width, height);			
			mVideoDevice->open(NULL, width, height);
		}	

		if (!mVideoComposer) {
			mVideoComposer = new VideoComposer();			
			mVideoComposer->create(this);
		}
	
		FFL::sp<VideoRender> render = mVideoDevice->getRender(NULL);
		if (!render->isCreated()) {
			render->create(this);
			render->setSpeed(getSpeed());
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
	void PlayerCore::onAddAudioStream(FFL::sp<AudioStream> stream) {
		if (mAudioDevice.isEmpty()) {
			mAudioDevice = mDeviceCreator->createAudioDevice(this);			

			AudioFormat fmt;
			stream->getFormat(fmt);
			//x 
			// 启动音频设备
			//
			AudioFormat obtainedFmt;
			if (mAudioDevice->open(fmt, 4096, obtainedFmt)) {				
			}
		}

		FFL::sp<AudioRender> render = mAudioDevice->getRender(NULL);
		if (!render.isEmpty() && !render->isCreated()) {			
			stream->getTimebase(mAudioTb);			
			render->setSpeed(getSpeed());
			render->create(this);
		}

		if (!mAudioComposer) {
			mAudioComposer = new AudioComposer();			
			mAudioComposer->setOutputFormat(mAudioDevice->getOpenFormat());
			mAudioComposer->create(this);
			mAudioComposer->setOutputRender(render);
		}		

		FFL::sp<Decoder> decoder = stream->createDecoder();
		if (!decoder->isCreated()) {
			decoder->create(this);
		}
		decoder->setOutputComposer(mAudioComposer);

		mMasterClock = stream->getSyncClock();
		//
		// 启动decoder和compoer的处理
		//
		mPipeline->startup(decoder->getNodeId());
		mPipeline->startup(mAudioComposer->getNodeId());
		mPipeline->startup(render->getNodeId());
	}
	void PlayerCore::onAddOtherStream(FFL::sp<Stream> stream) {
	}
	void PlayerCore::createVideoDisplay() {
		if (mVideoDevice.isEmpty()) {
			mVideoDevice = mDeviceCreator->createVideoDevice(this);
			
			uint32_t width = 400;
			uint32_t height = 300;
			mVideoDevice->open(NULL, width, height);	
		}
	}
	//  IStreamManager
	bool PlayerCore::addStream(FFL::sp<Stream> stream) {
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
	FFL::sp<Stream> PlayerCore::removeStream(uint32_t index) {	
		FFL_LOG_WARNING("PlayerCore::removeStream no impl.");
		return NULL;
	}
	FFL::sp<Stream> PlayerCore::getStream(uint32_t index) {
		FFL::CMutex::Autolock l(mStreamLock);
		for (uint32_t i = 0; i < mStreamVec.size(); i++) {
			if (mStreamVec[i]->getIndex() == index) {
				return mStreamVec[i];
			}
		}

		return NULL;
	}
	IStreamManager* PlayerCore::getStreamMgr() {
		return this;
	}
	//
	//  设置，获取播放速度
	//
	void PlayerCore::setSpeed(uint32_t speed) {
		if (speed > 300) {
			speed = 300;
		}else if (speed <= 10) {
			speed = 10;
		}

		mSpeed = speed;
		if(!mVideoDevice.isEmpty()){
			FFL::sp<VideoRender> render = mVideoDevice->getRender(0);
			if (!render.isEmpty()) {
				render->getRenderClock()->setSpeed(speed);
			}
		}

		if (!mAudioDevice.isEmpty()) {
			FFL::sp<AudioRender> render = mAudioDevice->getRender(0);
			if (!render.isEmpty()) {
				render->getRenderClock()->setSpeed(speed);
			}
		}
	}
	uint32_t PlayerCore::getSpeed() {
		return mSpeed;
	}
}
