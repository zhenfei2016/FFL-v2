/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  Player.cpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/06/20 
 *  https://github.com/zhenfei2016/FFL-v2.git
 *  
 *
 *
*/
#include "Player.hpp"
#include "PlayerCore.hpp"
#include "PlayerEvent.hpp"
#include "AudioDevice.hpp"
#include "VideoDevice.hpp"
#include <utils/FFL_File.hpp>
#include <pipeline/FFL_Pipeline.hpp>
#include <pipeline/FFL_PipelineEvent.hpp>
#include "VideoSurface.hpp"
#include "AudioDevice.hpp"
#include "VideoDevice.hpp"
#include "Statistic.hpp"
#include "VideoRender.hpp"
#include "AudioRender.hpp"
#include "DeviceFactory.hpp"
#include "reader/Stream.hpp"

#if defined(ANDROID)
#include "android/AndroidModule.hpp"
#else
#include "SDL2Module.hpp"
#endif

namespace player {
	class FFLPlayer::FFLPlayerDeviceManager : public DeviceManager {
	public:
		FFLPlayerDeviceManager(FFLPlayer* player){
			mSurfaceHandle = NULL;
			mPlayer = player;
#if defined(ANDROID)
			mDeviceCreator =new android::AndroidModule();
#else
			mDeviceCreator = new SDL2Module();
#endif
		}
        virtual ~FFLPlayerDeviceManager() {
			FFL_SafeFree(mDeviceCreator);
		}

		virtual FFL::sp<VideoDevice> getVideoDisplay(FFL::sp<VideoStream> stream) {
			if (stream.isEmpty()) {
				return mVideoDevice;
			}

			if (mVideoDevice.isEmpty()) {
				mVideoDevice = createVideoDisplay(stream,NULL);
			}
			return mVideoDevice;
		}
		//
		// 创建删除显示音频的设备
		//
		virtual FFL::sp<AudioDevice> getAudioDisplay(FFL::sp<AudioStream> stream) {
			if (stream.isEmpty()) {
				return mAudioDevice;
			}

			if (mAudioDevice.isEmpty()) {
				mAudioDevice = createAudioDisplay(stream);
			}
			return mAudioDevice;
		}
	public:
		//
		// 设置绘制窗口
		//
		void setVideoSurface(SurfaceHandle surface) {
			if (surface != NULL && surface != mSurfaceHandle) {
				mSurfaceHandle = surface;
				if (!mVideoDevice.isEmpty()) {
					mVideoDevice->setSurface(surface);
				}
			}
		}
		//
		// 获取绘制中的窗口
		//
		FFL::sp<VideoSurface> getVideoSurface() {
			if (!mVideoDevice.isEmpty()) {
				return mVideoDevice->getSurface();
			}
			return NULL;
		}
		//
		// 创建，删除显示视频设备
		//
		FFL::sp<VideoDevice> createVideoDisplay(FFL::sp<VideoStream> stream, SurfaceHandle surface){
			FFL::sp<VideoDevice> dev = mDeviceCreator->createVideoDevice(NULL);
			uint32_t width = 400;
			uint32_t height = 300;
			if (!stream.isEmpty()) {
				stream->getSize(width, height);
			}
			dev->open(surface, width, height);
			return dev;
		}
		void destroyVideoDisplay(FFL::sp<VideoDevice> dev) {
			if (!dev.isEmpty()) {
				dev->close();
				dev = NULL;
			}
		}
		//
		// 创建删除显示音频的设备
		//
		FFL::sp<AudioDevice> createAudioDisplay(FFL::sp<AudioStream> stream) {
			if (stream.isEmpty()) {
				return NULL;
			}

			FFL::sp<AudioDevice> dev = mDeviceCreator->createAudioDevice(NULL);
			AudioFormat fmt;
			stream->getFormat(fmt);

			//
			// 启动音频设备
			//
			AudioFormat obtainedFmt;
			if (!dev->open(fmt, 1024, obtainedFmt)) {
				return NULL;
			}

			return dev;
		}
		void destroyAudioDisplay(FFL::sp<AudioDevice> dev) {
			if (!dev.isEmpty()) {
				dev->close();
				dev = NULL;
			}
		}
	public:
		DeviceFactory* mDeviceCreator;
		FFLPlayer* mPlayer;
		FFL::sp<AudioDevice> mAudioDevice;
		FFL::sp<VideoDevice> mVideoDevice;

		SurfaceHandle mSurfaceHandle;
	};

	FFLPlayer::FFLPlayer():mListener(NULL){
		mCore = new PlayerCore(this);
		mEventPrepare = new FFL::PipelineEvent(
			new FFL::ClassMethodCallback<player::FFLPlayer>(this, &FFLPlayer::onPrepare));	

		mSpeed = 100;
		mSurfaceHandle = NULL;		
		mDevManager = new FFLPlayerDeviceManager(this);
		mCore->setDeviceManager(mDevManager);
		mDevManager->mVideoDevice=mDevManager->createVideoDisplay(NULL,NULL);
	}
	FFLPlayer::~FFLPlayer() {
		FFL_SafeFree(mDevManager);		
		FFL_SafeFree(mCore);
	}

	//
	// FFL::PipelineEventFilter
	// 派发消息前的准备，消息派发前会先到这地方的
	// 返回false表示不要向下派发了
	//
	bool FFLPlayer::onPrepareDispatch(const FFL::sp<FFL::PipelineEvent> e) {
		FFL::sp<event::PlayerEvent> event = (event::PlayerEvent*) e.get();

		int32_t eventID = event->getEventId();
		switch (eventID)
		{
		case event::EVENT_PREPARED:
			if (mListener) {
				mListener->onPrepared(event->getParam1() == 1 ? 1 : 0);
			} else {
				//mCore->play();
				//mFlag.modifyFlags(FLAG_LOOPING, FLAG_PREPARED);
			}
			break;
		case event::EVENT_VIDEO_SIZE_CAHNGED:{
			int32_t width = FFL_INT64_HIGHT_32(event->getParam1());
			int32_t height = FFL_INT64_LOW_32(event->getParam1());
			if (mListener) {
				mListener->onVideoSizeChanged(width,height, 
					FFL_INT64_HIGHT_32(event->getParam2()),
					FFL_INT64_LOW_32(event->getParam2()));
			}
			setSurfaceSize(width,height);
			
		}
		break;
		case event::EVENT_SEEK_START:
			break;
		case event::EVENT_SEEK_END:
			if (mListener) {
				mListener->onSeekComplete(event->getParam1(), event->getParam2());
			}
			break;
		case event::EVENT_VIDEO_RENDER_FIRST_FRAME:
			if (mListener) {
				mListener->onMessage(event::EVENT_VIDEO_RENDER_FIRST_FRAME, 0, 0);
			}
			break;
		case event::EVENT_VIDEO_RENDER_LAST_FRAME:
			if (mListener) {
				mListener->onMessage(event::EVENT_VIDEO_RENDER_LAST_FRAME, 0, 0);
			}
			break;

		case event::EVENT_AUDIO_RENDER_FIRST_FRAME:
			if (mListener) {
				mListener->onMessage(event::EVENT_AUDIO_RENDER_FIRST_FRAME, 0, 0);
			}
			break;

		case event::EVENT_AUDIO_RENDER_LAST_FRAME:
			if (mListener) {
				mListener->onMessage(event::EVENT_AUDIO_RENDER_LAST_FRAME, 0, 0);
			}
			break;
		default:
			break;
		}

		mCore->onEvent(event);
		return true;
	}
	//
	//  设置播放url
	//
	status_t FFLPlayer::setUrl(const char *url){
		if (url == NULL || url[0] == 0) {
			return FFL_FAILED;
		}

		{
			FFL::CMutex::Autolock l(mMutex);
			if (isLooping() || isStarting()) {
				FFL_LOG_WARNING("Failed to seturl(%s). Player is playing.",url);
				return FFL_FAILED;
			}
		}

		mUrl = url;
		if (!FFL::fileIsExist(url)) {
			return FFL_FAILED;
		}
		return FFL_OK;
	}
	//
	// 设置渲染到的目标
	//
	status_t FFLPlayer::setSurface(void* surface){
		SurfaceHandle wnd;
		wnd=(SurfaceHandle)(surface);
		mDevManager->setVideoSurface(wnd);
		FFL_ASSERT_LOG(0, "setSurface not impl.");
		return FFL_OK;
	}
	//
	// 设置渲染窗口的大小，只有窗口存在的情况下才可以设置大小，否则返回失败
	//
	status_t FFLPlayer::setSurfaceSize(int32_t widht, int32_t height) {	
		FFL::sp<VideoSurface> surface=mDevManager->getVideoSurface();
		if (!surface.isEmpty()) {
			surface->setWindowSize(widht, height);
		}
		return FFL_FAILED;
	}
	//
	//  播放，暂停，停止
	//
	status_t FFLPlayer::prepare(){
		FFL::CMutex::Autolock l(mMutex);
		if (!isPrepared()) {
			mFlag.modifyFlags(FLAG_PREPAREING, 0);
			mCore->getPipeline()->postEvent(mEventPrepare);
			return FFL_OK;
		}
		return FFL_ERROR_FAIL;
	}
	status_t FFLPlayer::start(){ 
		{
			FFL::CMutex::Autolock l(mMutex);
			if (!isPrepared()|| isStarted() || isStarting()) {
				return FFL_ERROR_FAIL;
			}
			mFlag.modifyFlags(FLAG_STARTING, FLAG_PREPARED);
		}
		onStart(NULL);
		return FFL_OK;
	}
	status_t FFLPlayer::pause(int32_t pause){
		FFL::sp<FFL::PipelineEvent> event = new FFL::PipelineEvent(0);
		{
			FFL::CMutex::Autolock l(mMutex);
			if (!isLooping()) {
				return FFL_ERROR_FAIL;
			}
			event->setParams(pause,0);
			if (pause == 1) {
				mFlag.modifyFlags(FLAG_PAUSEING, 0);
			}else {
				mFlag.modifyFlags(0, FLAG_PAUSEING);
			}
		}

		onPause(event);
		return FFL_OK;
	}
	status_t FFLPlayer::stop(){
		{
			FFL::CMutex::Autolock l(mMutex);
			if (!isLooping() || isStoping() ) {
				return FFL_ERROR_FAIL;
			}
			mFlag.modifyFlags(FLAG_STOPING, 0);
		}
		onStop(NULL);
		return FFL_OK;
	}
	//
	//  定位到指定us处
	//  获取当前的播放位置us
	//  获取总的播放时长us
	//
	status_t FFLPlayer::seekTo(int64_t us){
		{
			FFL::CMutex::Autolock l(mMutex);
			if (!isLooping()) {
				return FFL_ERROR_FAIL;
			}
		}
		mCore->setPositionUs(us);
		return FFL_OK;
	}
	int64_t  FFLPlayer::getCurrentPosition(){
		return mCore->getPositionUs();
	}
	int64_t  FFLPlayer::getDuration(){
		return mCore->getDurationUs();
	}
	//
	//  设置，获取播放速度
	//
	void FFLPlayer::setSpeed(uint32_t speed) {
		if (speed > 300) {
			speed = 300;
		}
		else if (speed <= 10) {
			speed = 10;
		}

		mSpeed = speed;

		FFL::sp<VideoDevice> videoDevice=mDevManager->getVideoDisplay(NULL);
		if (!videoDevice.isEmpty()) {
			//
			//  更新绘制的速度
			//
			FFL::sp<VideoRender> render = videoDevice->getRender(0);
			if (!render.isEmpty()) {
				render->getRenderClock()->setSpeed(speed);
			}
		}

		FFL::sp<AudioDevice> audioDevice = mDevManager->getAudioDisplay(NULL);
		if (!audioDevice.isEmpty()) {
			//
			//  更新音频速度
			//
			FFL::sp<AudioRender> render = audioDevice->getRender(0);
			if (!render.isEmpty()) {
				render->getRenderClock()->setSpeed(speed);
			}
		}
		mCore->setSpeed(mSpeed);
	}
	uint32_t FFLPlayer::getSpeed() {
		return mSpeed;
	}
	//
	// 获取，设置音量
	//
	void FFLPlayer::setVolume(int32_t volume){
		FFL::sp<player::AudioDevice >  audioDevice= mDevManager->getAudioDisplay(NULL);
		if (!audioDevice.isEmpty()) {
			audioDevice->setVolume(volume);		
		}	
	}
	void FFLPlayer::getVolume(int32_t& volume){
		FFL::sp<player::AudioDevice >  audioDevice = mDevManager->getAudioDisplay(NULL);
		if (!audioDevice.isEmpty()) {
			audioDevice->getVolume(volume);
		}
	}
	//
	// 获取，设置循环播放次数
	// 如果<0 : 一直循环播放
	//     =0 : 播放一次
	//     >0 : 播放num+1次
	//
	void FFLPlayer::setLoop(int32_t num){
		mCore->setLoop(num);		
	}
	int32_t FFLPlayer::getLoop(){
		FFL_ASSERT_LOG(0, "getLoop not impl.");
		return 0;
	}
	//
	// 获取，设置一些特定的选项
	//
	void FFLPlayer::setOptionString(const char* name, const char* value){
		FFL_ASSERT_LOG(0, "setOptionString not impl.");
	}
	void FFLPlayer::getOptionString(const char* name, char* buf, uint32_t bufSize, const char* defaultVal){
		FFL_ASSERT_LOG(0, "getOptionString not impl.");
	}
	//
	// 获取，设置一些特定的选项
	//
	void FFLPlayer::setOptionInt64(const char* name, int64_t value){
		FFL_ASSERT_LOG(0, "setOptionInt64 not impl.");
	}
	void FFLPlayer::getOptionInt64(const char* name, int64_t* value, int64_t defaultVal){
		FFL_ASSERT_LOG(0, "getOptionInt64 not impl.");
	}
	//
	//  设置播放器一系列事件的回调
	//
	void FFLPlayer::setListener(IPlayerListener* listener){
		FFL::CMutex::Autolock l(mMutex);
		if (mFlag.getFlags() == FLAG_INIT) {
			mListener = listener;
		}else {
		}
	}
	//
	//  是否prepated
	//
	bool FFLPlayer::isPrepared() const {
		return mFlag.hasFlags(FLAG_PREPARED);
	}
	//
	//  是否已经启动了
	//
	bool FFLPlayer::isStarted() const {
		return mFlag.hasFlags(FLAG_LOOPING);
	}
	bool FFLPlayer::isPaused() const {
		return mFlag.hasFlags(FLAG_PAUSED);
	}
	bool FFLPlayer::isStoped() const {
		return mFlag.getFlags()==FLAG_INIT;
	}
	bool FFLPlayer::isStarting() const {
		return mFlag.hasFlags(FLAG_STARTING);
	}
	bool FFLPlayer::isPauseing() const {
		return mFlag.hasFlags(FLAG_PAUSEING);
	}
	bool FFLPlayer::isStoping() const {
		return mFlag.hasFlags(FLAG_STOPING);
	}
	bool FFLPlayer::isLooping() const {
		return mFlag.hasFlags(FLAG_LOOPING);
	}
	//
	//  prepare 播放，暂停，停止具体实现函数
	//
	void  FFLPlayer::onPrepare(const FFL::sp<FFL::PipelineEvent>& even) {
		mCore->syncPrepare(mUrl.c_str());
		FFL::CMutex::Autolock l(mMutex);
		mFlag.modifyFlags(FLAG_PREPARED,FLAG_PREPAREING);
	}
	void  FFLPlayer::onStart(const FFL::sp<FFL::PipelineEvent>& even) {
		if (mUrl.empty()) {

		}

		FFL::CMutex::Autolock l(mMutex);
		mFlag.modifyFlags(FLAG_LOOPING, FLAG_STARTING);
		mCore->play();
	}
	void  FFLPlayer::onPause(const FFL::sp<FFL::PipelineEvent>& even) {
		FFL::CMutex::Autolock l(mMutex);
		mFlag.modifyFlags(FLAG_PAUSED, FLAG_PAUSEING);

		if(even->getParam1()==1)
		    mCore->pause();
		else
			mCore->resume();
	}
	void  FFLPlayer::onStop(const FFL::sp<FFL::PipelineEvent>& even) {
		FFL::CMutex::Autolock l(mMutex);
		mFlag.resetFlags(FLAG_INIT);

		FFL::sp<AudioDevice> audioDevice = mDevManager->getAudioDisplay(NULL);
		if (!audioDevice.isEmpty()) {
			audioDevice->close();
		}

		FFL::sp<VideoDevice> videoDevice = mDevManager->getVideoDisplay(NULL);
		if (!videoDevice.isEmpty()) {
			videoDevice->close();
		}
		mCore->stop();		
	}
	////
	//// 设置绘制窗口
	////
	//void FFLPlayer::setVideoSurface(SurfaceHandle surface) {
	//	if (surface != NULL && surface != mSurfaceHandle) {
	//		mSurfaceHandle = surface;
	//		if (!mVideoDevice.isEmpty()) {
	//			mVideoDevice->setSurface(surface);
	//		}
	//	}
	//}	
}
