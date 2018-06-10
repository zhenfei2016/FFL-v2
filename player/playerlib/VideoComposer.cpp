/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  VideoComposer.cpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/05/19 
 *  
 *  ��Ƶ�ϳ�
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
	//  �ɹ�������node
	//
	void VideoComposer::onCreate() {
		Composer::onCreate();
		mStatistic = &(getOwner()->mStats);
	}


	//
	//   �ⲿsetDataInputʱ����ô˺�����������Ӧconn
	//
	FFL::sp<FFL::PipelineConnector > VideoComposer::onCreateConnector(
		const OutputInterface& output,
		const InputInterface& input,
		void* userdata) {
		return new FFL::SyncPipelineConnector();
	}
	//
	//
	// ������յ�����Ϣ���������false��ʾ�����Ϣû�д���
	// ����true��ʾ�����Ϣ�����ˣ���Ϣ������������msg->consume();
	//
	bool VideoComposer::handleReceivedData(const FFL::sp<FFL::PipelineMessage>& msg, void* userdata){	
		if (msg.isEmpty()) {			
			return true;
		}

		int64_t delay = 0;
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
	//  �յ�����ʾ�������
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
		// ���͵��������ӿ���
		//		
		postMessageDelayToRender(msg, delay);
	}	
	//
	//  ���յ�eof��Ϣ
	//
	void VideoComposer::handleEOF(const FFL::sp<FFL::PipelineMessage>& eof) {
		postMessageDelayToRender(eof,1000);
	}

}
