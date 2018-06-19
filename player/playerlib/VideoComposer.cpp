/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  VideoComposer.cpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/05/19 
 *  
 *  视频合成
 *
*/
#include "VideoComposer.hpp"
#include "MessageFFMpegFrame.hpp"
#include "VideoTexture.hpp"
#include "Player.hpp"
#include "Stream.hpp"
#include "PlayerConstant.hpp"
#include "TimestampExtrapolator.hpp"

namespace player {
	VideoComposer::VideoComposer():mTimestampExtrapolator(NULL){
		setName("VideoComposer");
	}

	VideoComposer::~VideoComposer(){
	}
	//
	//  成功创建了node
	//
	void VideoComposer::onCreate() {
		Composer::onCreate();
		mStatistic = &(getOwner()->mStats);
	}


	//
	//   外部setDataInput时候调用此函数，创建对应conn
	//
	FFL::sp<FFL::PipelineConnector > VideoComposer::onCreateConnector(
		const OutputInterface& output,
		const InputInterface& input,
		void* userdata) {
		return new FFL::SyncPipelineConnector();
	}
	//
	//
	// 处理接收到的消息，如果返回false表示这个消息没有处理，
	// 返回true表示这个消息处理了，消息处理结束后必须msg->consume();
	//
	bool VideoComposer::handleReceivedData(const FFL::sp<FFL::PipelineMessage>& msg, void* userdata){
		if (msg.isEmpty()) {			
			return true;
		}
		
		switch (msg->getType())
		{
		case MSG_FFMPEG_VIDEO_FRAME:
		{
			message::FFMpegVideoFrame* frame = (message::FFMpegVideoFrame*)msg->getPayload();
			handleTexture(msg, &(frame->mTexture));
		}
			break;
		case MSG_CONTROL_READER_EOF:
			handleEOF(msg);
			msg->consume(this);
			break;
		default:			
			msg->consume(this);
			break;
		}

		return true;
	}	
	//
	//  计算多长时间后播放这一桢
	//
	int64_t VideoComposer::getDelay(VideoTexture* texture) {
		FFL::sp<Stream> stream = getOwner()->getStream(texture->mStreamId);
		stream->getTimebase(mTimerUnits);
		int64_t delay = mTimestampExtrapolator->getDelayUsRelativeNow(texture->mPts, mTimerUnits);
		if (delay < 0) {
			delay += 0;
		}

		texture->mRenderus = FFL_getNowUs() + delay;
		delay -= mStatistic->getVideoRenderDelayUs();
		delay = FFL_MAX(0, delay);

		FFL_LOG_DEBUG_TAG(TAG_TIMESTAMP, "VideoComposer pts=%" lld64 " delay=%" lld64,
			texture->mPts, delay);
		return delay;
	}
	//
	//  音视频同步,返回集体等待的时长，如果<0 则跳过这一帧
	//
	int64_t VideoComposer::avSync(int64_t delay) {
		if (delay < -SYNC_DROP_FRAME_THRESHOLD_US || delay>SYNC_DROP_FRAME_THRESHOLD_US) {
			//
			//  丢帧
			//
			FFL::sp<FFL::PipelineMessage> msgAvSync = new FFL::PipelineMessage(MSG_CONTROL_AV_SYNC);			
			postMessageDelayToRender(msgAvSync, 0);
			return -1;
		}else if (delay < -SYNC_CHANGE_SPEED_THRESHOLD_US || delay>SYNC_CHANGE_SPEED_THRESHOLD_US) {
			//
			//  调整播放速度
			//

		}
		return delay;
	}
	//
	//  收到待显示的纹理包
	//	
	void VideoComposer::handleTexture(const FFL::sp<FFL::PipelineMessage>& msg,VideoTexture* texture) {
		int64_t delay = getDelay(texture);
		delay =avSync(delay);
		if (delay < 0) {
			msg->consume(this);
			return;
		}		
		//
		// 发送到这个输出接口上
		//		
		postMessageDelayToRender(msg, delay);
	}	
	//
	//  接收到eof消息
	//
	void VideoComposer::handleEOF(const FFL::sp<FFL::PipelineMessage>& eof) {
		postMessageDelayToRender(eof,1000);
	}
}
