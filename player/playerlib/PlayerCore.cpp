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

#include "VideoRender.hpp"
#include "AudioRender.hpp"

#include "SDL2Module.hpp"
#include "PlayerEvent.hpp"

#include "Decoder.hpp"
#include "reader/ReaderFactory.hpp"
#include "AudioStream.hpp"
#include "VideoStream.hpp"

namespace player {
	//class PlayerCore::PlayerCoreEventFilter : public FFL::PipelineEventFilter {
	//public:
	//	PlayerCoreEventFilter(PlayerCore* player) :mPlayer(player) {}
	//	virtual ~PlayerCoreEventFilter() {

	//	}
	//	//
	//	// 派发消息前的准备，消息派发前会先到这地方的
	//	// 返回false表示不要向下派发了
	//	//
	//	bool onPrepareDispatch(const FFL::sp<FFL::PipelineEvent> event) {

	//		FFL::sp<event::PlayerEvent> evt =(event::PlayerEvent*) event.get();
	//		mPlayer->onEvent(evt);
	//		return true;
	//	}

	//	PlayerCore* mPlayer;
	//};

	PlayerCore::PlayerCore(FFL::PipelineEventFilter* eventFilter):mSpeed(100){
		mNextStreamId=0;
		mFileReader = NULL;
		mMasterClock = NULL;
		mVideoComposer = NULL;
		mAudioComposer = NULL;
		av_register_all();					
		mPipeline = new FFL::Pipeline();
		mPipeline->setEventFilter(eventFilter);
		setSpeed(100);
		init();
	}
	PlayerCore::~PlayerCore()
	{
		FFL_SafeFree(mDeviceCreator);				
	}
	status_t PlayerCore::init() {
		mDeviceCreator = new SDL2Module();
		//
		//  打开文件，读写文件节点
		//			
		mFileReader = reader::ReaderFactory::getInstance().createReader(NULL, NULL);
		mFileReader->setStreamManager(this);
		mFileReader->create(this);
		//
		// 创建显示窗体
		//
		mVideoDevice = createVideoDisplay(NULL, mSurface);
		return FFL_OK;
	}

	//
	//  没有保证同时进行多次的prepare,外部调用保证
	//
	status_t PlayerCore::syncPrepare(const char* url) {
		mUrl = url;
		mFileReader->open(mUrl.c_str());
		return FFL_OK;
	}

	status_t PlayerCore::play() {
		if (!mPipeline.isEmpty()) {		
			return mPipeline->startup();
		}
		return FFL_FAILED;
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
	status_t PlayerCore::resume() {
		return FFL_OK;
	}
	status_t PlayerCore::release() {
		if (!mPipeline.isEmpty())
		{
			mPipeline->exit();
			mPipeline.clear();
		}
		return FFL_OK;
	}
	//
	//获取，设置播放的位置,总时长
	//
	void PlayerCore::setPositionUs(int64_t pos) {
		mFileReader->seek(pos);
	}
	int64_t PlayerCore::getPositionUs() {
		return mFileReader->getCurrentPosition();
	}
	int64_t PlayerCore::getDurationUs() {
		return mFileReader->getDuration();
	}	
	//
	//  设置，获取播放速度
	//
	void PlayerCore::setSpeed(uint32_t speed) {
		if (speed > 300) {
			speed = 300;
		}
		else if (speed <= 10) {
			speed = 10;
		}

		mSpeed = speed;
		if (!mVideoDevice.isEmpty()) {
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
	void PlayerCore::onEvent(const FFL::sp<event::PlayerEvent> event)
	{
		int32_t eventID = event->getEventId();
		switch (eventID)
		{
		case event::EVENT_PREPARED:
			//FFL_LOG_DEBUG_TAG
			event->getParam1();

			break;
		case event::EVENT_VIDEO_SIZE_CAHNGED:
		{
			int32_t width = FFL_INT64_HIGHT_32( event->getParam1());
			int32_t height= FFL_INT64_LOW_32( event->getParam1());
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

	//  IStreamManager
	//
	//  添加reader下的一个stream到管理中，返回当前这个stream的id
	//
	int32_t  PlayerCore::addStream(reader::ReaderBase* reader,StreamPtr stream) {
		if (stream.isEmpty()) {
			//
			// 最后一条
			//
			int count = 0;
			{
				FFL::CMutex::Autolock l(mStreamLock);
				count = mStreamVec.size();
			}

			event::postPlayerEvent(this, event::EVENT_PREPARED, count>0, 0);
			return -1;
		}		

		bool ret = false;
		switch (stream->getStreamType())
		{
		case STREAM_TYPE_VIDEO:
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

		if (ret) {
			stream->mStreamID = mNextStreamId++;
			FFL::CMutex::Autolock l(mStreamLock);
			mStreamVec.push_back(stream);
			return stream->mStreamID;
		}		
		return -1;
	}
	//
	//  添加reader下的所有stream到管理中，返回成功添加了几个流
	//  
	//
	uint32_t PlayerCore::addStreamVec(reader::ReaderBase* reader, FFL::Vector < StreamPtr > streamVec) {
		FFL_ASSERT(0);
		return 0;
	}
	//
	//  根据流id获取一个流实例
	//
	StreamPtr PlayerCore::getStream(int32_t id) {
		FFL::CMutex::Autolock l(mStreamLock);
		for (uint32_t i = 0; i < mStreamVec.size(); i++) {
			if (mStreamVec[i]->getIndex() == id) {
				return mStreamVec[i];
			}
		}
		return NULL;
	}
	//
	//  获取这个reader下的所有流
	//
	void PlayerCore::getStreamVec(reader::ReaderBase* reader, FFL::Vector < StreamPtr >& streamVec) {
		FFL_ASSERT(0);
	}
	bool PlayerCore::onAddVideoStream(FFL::sp<VideoStream> stream) {
		//
		// 视频处理节点
		//
		if (!mVideoComposer) {
			mVideoComposer = new VideoComposer();			
			mVideoComposer->create(this);
		}	
		//
		//  通过视频设备创建视频渲染节点
		//
		FFL::sp<VideoRender> render = mVideoDevice->getRender(NULL);
		if (!render->isCreated()) {
			render->create(this);
			render->setSpeed(getSpeed());
			mVideoComposer->setOutputRender(render);
		}
		//
		//  解码器解码出来的数据输出到composer中
		//
		FFL::sp<Decoder> decoder = stream->createDecoder(this);		
		decoder->setOutputComposer(mVideoComposer);
		return  true;
	}
	bool PlayerCore::onAddAudioStream(FFL::sp<AudioStream> stream) {
		//
		// 如果音频设备未打开，则打开音频输出设备
		//
		if (mAudioDevice.isEmpty()) {
			mAudioDevice = createAudioDisplay(stream);
		}
		if (mAudioDevice.isEmpty()) {
			FFL_LOG_WARNING("Failed to create audio device.");
			return false;
		}

		//
		//  通过音频设备创建音频渲染节点
		//
		FFL::sp<AudioRender> render = mAudioDevice->getRender(NULL);
		if (!render.isEmpty() && !render->isCreated()) {			
			stream->getTimebase(mAudioTb);			
			render->setSpeed(getSpeed());
			render->create(this);
		}
		//
		// 音频处理后的数据输出到render中
		//
		if (!mAudioComposer) {
			mAudioComposer = new AudioComposer();			
			mAudioComposer->setOutputFormat(mAudioDevice->getOpenFormat());
			mAudioComposer->create(this);
			mAudioComposer->setOutputRender(render);
		}
		//
		//  解码器解码出来的数据输出到composer中
		//
		FFL::sp<Decoder> decoder = stream->createDecoder(this);
		decoder->setOutputComposer(mAudioComposer);
		//
		//  设置这个流为同步的主时钟
		//
		mMasterClock = stream->getSyncClock();	
		return true;
	}
	bool PlayerCore::onAddOtherStream(StreamPtr stream) {
		return false;
	}
	//
	// 设置绘制窗口
	//
	void PlayerCore::setVideoSurface(FFL::sp<VideoSurface> surface) {
		if (mSurface.get() != surface.get()) {
			mSurface = surface;
			if (!mVideoDevice.isEmpty()) {
				mVideoDevice->setSurface(mSurface);
			}
		}
	}
	FFL::sp<VideoDevice> PlayerCore::createVideoDisplay(FFL::sp<VideoStream> stream, FFL::sp<VideoSurface> surface) {
		FFL::sp<VideoDevice> dev = mDeviceCreator->createVideoDevice(this);
		uint32_t width = 400;
		uint32_t height = 300;
		if (!stream.isEmpty()) {
			stream->getSize(width, height);
		}
		dev->open(surface, width, height);
		return dev;
	}
	void PlayerCore::destroyVideoDisplay(FFL::sp<VideoDevice> dev) {
		if (!dev.isEmpty()) {
			dev->close();
			dev = NULL;
		}
	}
	//
	// 创建删除显示音频的设备
	//
	FFL::sp<AudioDevice> PlayerCore::createAudioDisplay(FFL::sp<AudioStream> stream) {		
		if (stream.isEmpty()) {
			return NULL;
		}

		FFL::sp<AudioDevice> dev = mDeviceCreator->createAudioDevice(this);
		AudioFormat fmt;
		stream->getFormat(fmt);

		//
		// 启动音频设备
		//
		AudioFormat obtainedFmt;
		if (!dev->open(fmt, 4096, obtainedFmt)) {
			return NULL;
		}

		return dev;
	}
	void PlayerCore::destroyAudioDisplay(FFL::sp<AudioDevice> dev) {
		if (!dev.isEmpty()) {
			dev->close();
			dev = NULL;
		}
	}
}
