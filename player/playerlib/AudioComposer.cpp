/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  AudioComposer.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/04/07
*
*  声音的合成类
*
*/

#include "AudioComposer.hpp"
#include "Player.hpp"
#include "MessageFFMpegFrame.hpp"
#include "AudioResample.hpp"
#include "FFMpeg.hpp"

namespace player {
	AudioComposer::AudioComposer():
		mTimestampExtrapolator(NULL),
		mDstFormat(NULL) {
		setName("AudioComposer");
		mResample = new AudioResample();
	}

	AudioComposer::~AudioComposer() {
		FFL_SafeFree(mResample);
		FFL_SafeFree(mDstFormat);
	}
	//
	//  设置输出的格式
	//
	void AudioComposer::setOutputFormat(AudioFormat* fmt) {
		FFL_SafeFree(mDstFormat);
		if (fmt) {
			mDstFormat = new AudioFormat();
			*mDstFormat = *fmt;
		}		
	}

	//
	//   外部setDataInput时候调用此函数，创建对应conn
	//
	FFL::sp<FFL::PipelineConnector > AudioComposer::onCreateConnector(
		const OutputInterface& output,
		const InputInterface& input, void* userdata) {
		return new FFL::SyncPipelineConnector();
	}

	//
	// 处理接收到的消息，如果返回false表示这个消息没有处理，
	// 返回true表示这个消息处理了，消息处理结束后必须msg->consume();
	//
	bool AudioComposer::handleReceivedData(const FFL::sp<FFL::PipelineMessage>& msg, void* userdata){
		if (msg.isEmpty()) {
			return true;
		}
		
		switch (msg->getType())
		{
		case MSG_FFMPEG_AUDIO_FRAME:
		{
			message::FFMpegAudioFrame* frame = (message::FFMpegAudioFrame*)msg->getPayload();
			handleSamples(msg, &(frame->mSamples));
		}
			break;
		case MSG_CONTROL_READER_EOF:
			handleEOF(msg);
			break;
		default:
            msg->consume(this);
			break;
		}
	
		
		return true;
	}
	
	void AudioComposer::handleSamples(const FFL::sp<FFL::PipelineMessage>& msg, AudioSample* sample) {
		if (!mDstFormat) {
			FFL_LOG_WARNING("Faild to AudioComposer::handleSamples (dstFormat is null)");
			msg->consume(this);
			return;
		}

		AudioFormat fmt;
		sample->getAudioFormat(fmt);
		if (!mDstFormat->equal(fmt)) {
			//
			//  重采样
			//
			AudioSample targetSample;
			targetSample.setAudioFormat(*mDstFormat);			
			mResample->resample(sample, &targetSample, getOwner()->getSpeed());
			sample->moveData(targetSample);
		}else {
			//
			//  不需要重采样
			//		
		}
		//
		// 发送到这个输出接口上
		//		
		if (FFL_OK != postMessageDelayToRender(msg, 0)) {
			msg->consume(this);
		}			
	}
	//
	//  接收到eof消息
	//
	void AudioComposer::handleEOF(const FFL::sp<FFL::PipelineMessage>& eof) {
		postMessageDelayToRender(eof,0);
	}
}
