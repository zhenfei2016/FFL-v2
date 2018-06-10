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
#include "FFL_Texture.hpp"
#include "FFL_Player.hpp"
#include "Stream.hpp"
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
			break;
			msg->consume(this);
		}
		
		
		return true;
	}	

	bool gFirst = true;
		 
	//
	//  收到待显示的纹理包
	//	
	void VideoComposer::handleTexture(const FFL::sp<FFL::PipelineMessage>& msg,FFLTexture* texture) {
		int64_t delay = 0;
		if (gFirst&&false) {
			gFirst = false;
			mStatistic->getVideoRenderDelayUs();			
			FFL::sp<Stream> stream=getOwner()->getStream(texture->mStreamId);			
			stream->getTimebase(mTimerUnits);
			mTimestampExtrapolator->update(texture->mOrginalPts, mTimerUnits);
		}else{
			FFL::sp<Stream> stream = getOwner()->getStream(texture->mStreamId);
			stream->getTimebase(mTimerUnits);
			delay = mTimestampExtrapolator->getDelayUsRelativeNow(texture->mOrginalPts, mTimerUnits);
			if (delay == 0) {
				delay += 0;
			}
		}
		
		texture->mRenderus = FFL_getNowUs() + delay;
		delay -= mStatistic->getVideoRenderDelayUs();
		delay = FFL_MAX(0, delay);		

		FFL_LOG_CRIT("VideoComposer pts=%" lld64 " delay=%" lld64 ,	texture->mOrginalPts, delay);

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
