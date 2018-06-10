/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  Statistic.cpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/05/01 
 *  
 *  ͳ�ƽӿ�
 *
*/
#include "Statistic.hpp"

namespace player {
	PlayerStatistic::PlayerStatistic() {
		mVideoRenderDelayUs = 0;
	}

	PlayerStatistic::~PlayerStatistic() {
	}

	void PlayerStatistic::renderAudioDelayUs(int64_t delay) {

	}

	int64_t PlayerStatistic::getAudioRenderDelayUs() {
		return 0;
	}

	void PlayerStatistic::renderVideoDelayUs(int64_t delay) {
		FFL_LOG_CRIT("vid: delay=%" lld64 ,delay);

		if (mVideoRenderDelayUs != 0) {
			mVideoRenderDelayUs = (mVideoRenderDelayUs + delay) / 2;
		}
		else {
			mVideoRenderDelayUs = delay;
		}
	}
	int64_t PlayerStatistic::getVideoRenderDelayUs() {
		return mVideoRenderDelayUs;
	}

	void PlayerStatistic::setAudioComposerDelayUs(int64_t delay) {
	}
	int64_t PlayerStatistic::getAudioComposerDelayUs() {
		return 0;
	}

	void PlayerStatistic::setVideoComposerDelayUs(int64_t delay) {
	}
	int64_t PlayerStatistic::getVideoComposerDelayUs() {
		return 0;
	}

	void PlayerStatistic::setAudioDecoderDelayUs(int64_t delay) {
	}
	int64_t PlayerStatistic::getAudioDecoderDelayUs() {
		return 0;
	}

	void PlayerStatistic::setVideoDecoderDelayUs(int64_t delay) {
	}
	int64_t PlayerStatistic::getVideoDecoderDelayUs() {
		return 0;
	}


	//
	//  ��ʼ��Ⱦtimestamp��Ӧ��һ֡����
	//  now:��ǰ�ı���ʱ��
	//
	void PlayerStatistic::renderAudioFrame(int64_t timestamp, int64_t now) {
		
	}
	//
	//  ��ʼ��Ⱦtimestamp��Ӧ��һ֡����
	//  now:��ǰ�ı���ʱ��
	//
	void PlayerStatistic::renderVideoFrame(int64_t timestamp, int64_t now) {
		
	}
}