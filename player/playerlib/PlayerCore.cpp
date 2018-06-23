/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_Player.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/03/03
*
*  ������
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
	//	// �ɷ���Ϣǰ��׼������Ϣ�ɷ�ǰ���ȵ���ط���
	//	// ����false��ʾ��Ҫ�����ɷ���
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
		//  ���ļ�����д�ļ��ڵ�
		//			
		mFileReader = reader::ReaderFactory::getInstance().createReader(NULL, NULL);
		mFileReader->setStreamManager(this);
		mFileReader->create(this);
		//
		// ������ʾ����
		//
		mVideoDevice = createVideoDisplay(NULL, mSurface);
		return FFL_OK;
	}

	//
	//  û�б�֤ͬʱ���ж�ε�prepare,�ⲿ���ñ�֤
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
	//��ȡ�����ò��ŵ�λ��,��ʱ��
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
	//  ���ã���ȡ�����ٶ�
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
	//  ���reader�µ�һ��stream�������У����ص�ǰ���stream��id
	//
	int32_t  PlayerCore::addStream(reader::ReaderBase* reader,StreamPtr stream) {
		if (stream.isEmpty()) {
			//
			// ���һ��
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
	//  ���reader�µ�����stream�������У����سɹ�����˼�����
	//  
	//
	uint32_t PlayerCore::addStreamVec(reader::ReaderBase* reader, FFL::Vector < StreamPtr > streamVec) {
		FFL_ASSERT(0);
		return 0;
	}
	//
	//  ������id��ȡһ����ʵ��
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
	//  ��ȡ���reader�µ�������
	//
	void PlayerCore::getStreamVec(reader::ReaderBase* reader, FFL::Vector < StreamPtr >& streamVec) {
		FFL_ASSERT(0);
	}
	bool PlayerCore::onAddVideoStream(FFL::sp<VideoStream> stream) {
		//
		// ��Ƶ����ڵ�
		//
		if (!mVideoComposer) {
			mVideoComposer = new VideoComposer();			
			mVideoComposer->create(this);
		}	
		//
		//  ͨ����Ƶ�豸������Ƶ��Ⱦ�ڵ�
		//
		FFL::sp<VideoRender> render = mVideoDevice->getRender(NULL);
		if (!render->isCreated()) {
			render->create(this);
			render->setSpeed(getSpeed());
			mVideoComposer->setOutputRender(render);
		}
		//
		//  ������������������������composer��
		//
		FFL::sp<Decoder> decoder = stream->createDecoder(this);		
		decoder->setOutputComposer(mVideoComposer);
		return  true;
	}
	bool PlayerCore::onAddAudioStream(FFL::sp<AudioStream> stream) {
		//
		// �����Ƶ�豸δ�򿪣������Ƶ����豸
		//
		if (mAudioDevice.isEmpty()) {
			mAudioDevice = createAudioDisplay(stream);
		}
		if (mAudioDevice.isEmpty()) {
			FFL_LOG_WARNING("Failed to create audio device.");
			return false;
		}

		//
		//  ͨ����Ƶ�豸������Ƶ��Ⱦ�ڵ�
		//
		FFL::sp<AudioRender> render = mAudioDevice->getRender(NULL);
		if (!render.isEmpty() && !render->isCreated()) {			
			stream->getTimebase(mAudioTb);			
			render->setSpeed(getSpeed());
			render->create(this);
		}
		//
		// ��Ƶ���������������render��
		//
		if (!mAudioComposer) {
			mAudioComposer = new AudioComposer();			
			mAudioComposer->setOutputFormat(mAudioDevice->getOpenFormat());
			mAudioComposer->create(this);
			mAudioComposer->setOutputRender(render);
		}
		//
		//  ������������������������composer��
		//
		FFL::sp<Decoder> decoder = stream->createDecoder(this);
		decoder->setOutputComposer(mAudioComposer);
		//
		//  ���������Ϊͬ������ʱ��
		//
		mMasterClock = stream->getSyncClock();	
		return true;
	}
	bool PlayerCore::onAddOtherStream(StreamPtr stream) {
		return false;
	}
	//
	// ���û��ƴ���
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
	// ����ɾ����ʾ��Ƶ���豸
	//
	FFL::sp<AudioDevice> PlayerCore::createAudioDisplay(FFL::sp<AudioStream> stream) {		
		if (stream.isEmpty()) {
			return NULL;
		}

		FFL::sp<AudioDevice> dev = mDeviceCreator->createAudioDevice(this);
		AudioFormat fmt;
		stream->getFormat(fmt);

		//
		// ������Ƶ�豸
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
