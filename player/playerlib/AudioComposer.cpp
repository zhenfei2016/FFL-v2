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
#include "FFL_Player.hpp"
#include "MessageFFMpegFrame.hpp"
#include "FFMpeg.hpp"

namespace player {
	AudioComposer::AudioComposer():mTimestampExtrapolator(NULL){
		setName("AudioComposer");
	}

	AudioComposer::~AudioComposer() {
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
	
	void AudioComposer::handleSamples(const FFL::sp<FFL::PipelineMessage>& msg, FFLSample* sample) {
		int64_t delay = 0;
//		if (mTimestampExtrapolator == NULL) {
//			FFL::sp<FFL::Clock> clock = new FFL::Clock();			
//			mTimestampExtrapolator = new TimestampExtrapolator(clock);
//
//			FFL::sp<Stream> stream = getOwner()->getStream(sample->mStreamId);
//			stream->getTimebase(mTimerUnits);
//			delay = mTimestampExtrapolator->reset(sample->mPts, mTimerUnits);
//		} else {
//			delay = mTimestampExtrapolator->getRelativeTime(sample->mPts, mTimerUnits);
//			if (delay == 0) {
//				delay += 0;
//			}
//		}
//
//		delay = 0;
//		sample->mDuration = (int64_t)((double((double)sample->mSampleNum / sample->mFreq)) * 1000 * 1000);
//
//		FFL_LOG_INFO("AudioComposer duration %" lld64, sample->mDuration);

		//
		// 发送到这个输出接口上
		//		
		if (FFL_OK != postMessageDelayToRender(msg, delay)) {
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
