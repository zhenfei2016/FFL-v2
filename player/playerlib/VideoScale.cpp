/* 
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  VideoScale.cpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/06/12 
 *  
 *
 *
*/
#include "VideoScale.hpp"
#include <pipeline/FFL_PipelineAsyncConnectorFixedsize.hpp>

namespace player {
	VideoScale::VideoScale():mSwsCtx(NULL){
		
	}
	VideoScale::~VideoScale() {
		destroySws();
	}
	//
	//  成功创建了node
	//
	void VideoScale::onCreate() {
		NodeBase::onCreate();
	}	
	//
	//  创建图片缩放上下文
	//  src: 源格式
	//  dst: 目标格式
	//
	bool VideoScale::createSws(TextureFormat* src, TextureFormat* dst)
	{
		int flags = SWS_BILINEAR;		
		mSwsCtx = sws_getContext(mSourceFormat.mWidht,
			mSourceFormat.mHeight,
			mSourceFormat.mFormat,
			mDestFormat.mWidht,
			mDestFormat.mHeight,
			mDestFormat.mFormat, flags, 0, 0, 0);
		return true;
	}
	//
	//  开始缩放图片
	//
	bool VideoScale::scaleVideo(
		const uint8_t** srcPix,int* srcLinesize,int height,
		uint8_t** dstPix, int* dstLinesize) {
		if (!mSwsCtx) {
			FFL_LOG_WARNING("Failed to scaleVideo");
			return false;
		}
		sws_scale(mSwsCtx,
			srcPix,
			srcLinesize,
				0,
			height,
			dstPix,
			dstLinesize);

		return false;		
	}
	//
	//  删除缩放上下文
	//
	void VideoScale::destroySws() {
	}
	//
	//   外部setDataInput时候调用此函数，创建对应conn
	//
	FFL::sp<FFL::PipelineConnector > VideoScale::onCreateConnector(
		const OutputInterface& output,
		const InputInterface& input, void* userdata) {
		FFL::PipelineAsyncConnectorFixSize* conn = new FFL::PipelineAsyncConnectorFixSize(5);		
		return conn;
	}
	//
	// 处理接收到的消息，如果返回false表示这个消息没有处理，
	// 返回true表示这个消息处理了，消息处理结束后必须msg->consume();
	//
	bool VideoScale::handleReceivedData(const FFL::sp<FFL::PipelineMessage>& msg, void* userdata)
	{
		bool ret = false;
		//switch (msg->getType())
		//{
	
		//	msg->consume(this);
		//	ret = true;
		//}
		//	break;
		//case MSG_CONTROL_READER_EOF:
		//	break;
		//}
		
		return ret;
	}
}
