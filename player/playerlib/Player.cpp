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
#include <utils/FFL_File.hpp>
#include <pipeline/FFL_Pipeline.hpp>
#include <pipeline/FFL_PipelineEvent.hpp>

namespace player {
	FFLPlayer::FFLPlayer():mListener(NULL){
		mCore = new PlayerCore();
		mEventPrepare = new FFL::PipelineEvent(
			new FFL::ClassMethodCallback<player::FFLPlayer>(this, &FFLPlayer::onPrepare));
	}
	FFLPlayer::~FFLPlayer() {
		FFL_SafeFree(mCore);
	}

	//
	// FFL::PipelineEventFilter
	// 派发消息前的准备，消息派发前会先到这地方的
	// 返回false表示不要向下派发了
	//
	bool FFLPlayer::onPrepareDispatch(const FFL::sp<FFL::PipelineEvent> event) {
		FFL::sp<event::PlayerEvent> evt = (event::PlayerEvent*) event.get();
		mCore->onEvent(evt);
		return true;
	}
	//
	//  设置播放url
	//
	status_t FFLPlayer::setUrl(const char *url){
		if (url == NULL || url[0] == NULL) {
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
		FFL_ASSERT_LOG(0, "setSurface not impl.");
		return FFL_NOT_IMPLEMENT;
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
			mFlag.modifyFlags(FLAG_STARTING, 0);
		}

		onStart(NULL);
		return FFL_OK; }
	status_t FFLPlayer::pause(){
		{
			FFL::CMutex::Autolock l(mMutex);
			if (!isLooping()) {
				return FFL_ERROR_FAIL;
			}
			mFlag.modifyFlags(FLAG_PAUSEING, 0);
		}

		onPause(NULL);
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
	//  获取，设置播放速度，正常速度=100
	//
	uint32_t FFLPlayer::getSpeed(){
		return mCore->getSpeed();
	}
	void FFLPlayer::setSpeed(uint32_t speed){
		mCore->setSpeed(speed);
	}
	//
	// 获取，设置音量
	//
	void FFLPlayer::setVolume(float left, float right){
		FFL_ASSERT_LOG(0, "setVolume not impl.");
	}
	void FFLPlayer::getVolume(float* left, float* right){
		FFL_ASSERT_LOG(0, "getVolume not impl.");
	}
	//
	// 获取，设置循环播放次数
	// 如果<0 : 一直循环播放
	//     =0 : 播放一次
	//     >0 : 播放num+1次
	//
	void FFLPlayer::setLoop(int32_t num){
		FFL_ASSERT_LOG(0, "setLoop not impl.");
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
		FFL::CMutex::Autolock l(mMutex);
		mFlag.modifyFlags(FLAG_PREPARED,FLAG_PREPAREING);
	}
	void  FFLPlayer::onStart(const FFL::sp<FFL::PipelineEvent>& even) {
		if (mUrl.empty()) {

		}

		FFL::CMutex::Autolock l(mMutex);
		mFlag.modifyFlags(FLAG_LOOPING, FLAG_STARTING);

		mCore->play(mUrl.c_str());
	}
	void  FFLPlayer::onPause(const FFL::sp<FFL::PipelineEvent>& even) {
		FFL::CMutex::Autolock l(mMutex);
		mFlag.modifyFlags(FLAG_PAUSED, FLAG_PAUSEING);

		mCore->pause();
	}
	void  FFLPlayer::onStop(const FFL::sp<FFL::PipelineEvent>& even) {
		FFL::CMutex::Autolock l(mMutex);
		mFlag.resetFlags(FLAG_INIT);

		mCore->stop();
	}
}
